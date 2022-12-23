#!/usr/bin/env python
#
# Copyright 2007 Neal Norwitz
# Portions Copyright 2007 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Generate an Abstract Syntax Tree (AST) for C++."""

__author__ = 'nnorwitz@google.com (Neal Norwitz)'


# TODO:
#  * Tokens should never be exported, need to convert to Nodes
#    (return types, parameters, etc.)
#  * Handle static class data for templatized classes
#  * Handle casts (both C++ and C-style)
#  * Handle conditions and loops (if/else, switch, for, while/do)
#
# TODO much, much later:
#  * Handle #define
#  * exceptions


try:
    # Python 3.x
    import builtins
except ImportError:
    # Python 2.x
    import __builtin__ as builtins

import sys
import traceback

from cpp import keywords
from cpp import tokenize
from cpp import utils


if not hasattr(builtins, 'reversed'):
    # Support Python 2.3 and earlier.
    def reversed(seq):
        for i in range(len(seq)-1, -1, -1):
            yield seq[i]

if not hasattr(builtins, 'next'):
    # Support Python 2.5 and earlier.
    def next(obj):
        return obj.next()


VISIBILITY_PUBLIC, VISIBILITY_PROTECTED, VISIBILITY_PRIVATE = range(3)

FUNCTION_NONE = 0x00
FUNCTION_CONST = 0x01
FUNCTION_VIRTUAL = 0x02
FUNCTION_PURE_VIRTUAL = 0x04
FUNCTION_CTOR = 0x08
FUNCTION_DTOR = 0x10
FUNCTION_ATTRIBUTE = 0x20
FUNCTION_UNKNOWN_ANNOTATION = 0x40
FUNCTION_THROW = 0x80
FUNCTION_OVERRIDE = 0x100

"""
These are currently unused.  Should really handle these properly at some point.

TYPE_MODIFIER_INLINE   = 0x010000
TYPE_MODIFIER_EXTERN   = 0x020000
TYPE_MODIFIER_STATIC   = 0x040000
TYPE_MODIFIER_CONST    = 0x080000
TYPE_MODIFIER_REGISTER = 0x100000
TYPE_MODIFIER_VOLATILE = 0x200000
TYPE_MODIFIER_MUTABLE  = 0x400000

TYPE_MODIFIER_MAP = {
    'inline': TYPE_MODIFIER_INLINE,
    'extern': TYPE_MODIFIER_EXTERN,
    'static': TYPE_MODIFIER_STATIC,
    'const': TYPE_MODIFIER_CONST,
    'register': TYPE_MODIFIER_REGISTER,
    'volatile': TYPE_MODIFIER_VOLATILE,
    'mutable': TYPE_MODIFIER_MUTABLE,
    }
"""

_INTERNAL_TOKEN = 'internal'
_NAMESPACE_POP = 'ns-pop'


# TODO(nnorwitz): use this as a singleton for templated_types, etc
# where we don't want to create a new empty dict each time.  It is also const.
class _NullDict(object):
    __contains__ = lambda self: False
    keys = values = items = iterkeys = itervalues = iteritems = lambda self: ()


# TODO(nnorwitz): move AST nodes into a separate module.
class Node(object):
    """Base AST node."""

    def __init__(self, start, end):
        self.start = start
        self.end = end

    def IsDeclaration(self):
        """Returns bool if this node is a declaration."""
        return False

    def IsDefinition(self):
        """Returns bool if this node is a definition."""
        return False

    def IsExportable(self):
        """Returns bool if this node exportable from a header file."""
        return False

    def Requires(self, node):
        """Does this AST node require the definition of the node passed in?"""
        return False

    def XXX__str__(self):
        return self._StringHelper(self.__class__.__name__, '')

    def _StringHelper(self, name, suffix):
        if not utils.DEBUG:
            return '%s(%s)' % (name, suffix)
        return '%s(%d, %d, %s)' % (name, self.start, self.end, suffix)

    def __repr__(self):
        return str(self)


class Define(Node):
    def __init__(self, start, end, name, definition):
        Node.__init__(self, start, end)
        self.name = name
        self.definition = definition

    def __str__(self):
        value = '%s %s' % (self.name, self.definition)
        return self._StringHelper(self.__class__.__name__, value)


class Include(Node):
    def __init__(self, start, end, filename, system):
        Node.__init__(self, start, end)
        self.filename = filename
        self.system = system

    def __str__(self):
        fmt = '"%s"'
        if self.system:
            fmt = '<%s>'
        return self._StringHelper(self.__class__.__name__, fmt % self.filename)


class Goto(Node):
    def __init__(self, start, end, label):
        Node.__init__(self, start, end)
        self.label = label

    def __str__(self):
        return self._StringHelper(self.__class__.__name__, str(self.label))


class Expr(Node):
    def __init__(self, start, end, expr):
        Node.__init__(self, start, end)
        self.expr = expr

    def Requires(self, node):
        # TODO(nnorwitz): impl.
        return False

    def __str__(self):
        return self._StringHelper(self.__class__.__name__, str(self.expr))


class Return(Expr):
    pass


class Delete(Expr):
    pass


class Friend(Expr):
    def __init__(self, start, end, expr, namespace):
        Expr.__init__(self, start, end, expr)
        self.namespace = namespace[:]


class Using(Node):
    def __init__(self, start, end, names):
        Node.__init__(self, start, end)
        self.names = names

    def __str__(self):
        return self._StringHelper(self.__class__.__name__, str(self.names))


class Parameter(Node):
    def __init__(self, start, end, name, parameter_type, default):
        Node.__init__(self, start, end)
        self.name = name
        self.type = parameter_type
        self.default = default

    def Requires(self, node):
        # TODO(nnorwitz): handle namespaces, etc.
        return self.type.name == node.name

    def __str__(self):
        name = str(self.type)
        suffix = '%s %s' % (name, self.name)
        if self.default:
            suffix += ' = ' + ''.join([d.name for d in self.default])
        return self._StringHelper(self.__class__.__name__, suffix)


class _GenericDeclaration(Node):
    def __init__(self, start, end, name, namespace):
        Node.__init__(self, start, end)
        self.name = name
        self.namespace = namespace[:]

    def FullName(self):
        prefix = ''
        if self.namespace and self.namespace[-1]:
            prefix = '::'.join(self.namespace) + '::'
        return prefix + self.name

    def _TypeStringHelper(self, suffix):
        if self.namespace:
            names = [n or '<anonymous>' for n in self.namespace]
            suffix += ' in ' + '::'.join(names)
        return self._StringHelper(self.__class__.__name__, suffix)


# TODO(nnorwitz): merge with Parameter in some way?
class VariableDeclaration(_GenericDeclaration):
    def __init__(self, start, end, name, var_type, initial_value, namespace):
        _GenericDeclaration.__init__(self, start, end, name, namespace)
        self.type = var_type
        self.initial_value = initial_value

    def Requires(self, node):
        # TODO(nnorwitz): handle namespaces, etc.
        return self.type.name == node.name

    def ToString(self):
        """Return a string that tries to reconstitute the variable decl."""
        suffix = '%s %s' % (self.type, self.name)
        if self.initial_value:
            suffix += ' = ' + self.initial_value
        return suffix

    def __str__(self):
        return self._StringHelper(self.__class__.__name__, self.ToString())


class Typedef(_GenericDeclaration):
    def __init__(self, start, end, name, alias, namespace):
        _GenericDeclaration.__init__(self, start, end, name, namespace)
        self.alias = alias

    def IsDefinition(self):
        return True

    def IsExportable(self):
        return True

    def Requires(self, node):
        # TODO(nnorwitz): handle namespaces, etc.
        name = node.name
        for token in self.alias:
            if token is not None and name == token.name:
                return True
        return False

    def __str__(self):
        suffix = '%s, %s' % (self.name, self.alias)
        return self._TypeStringHelper(suffix)


class _NestedType(_GenericDeclaration):
    def __init__(self, start, end, name, fields, namespace):
        _GenericDeclaration.__init__(self, start, end, name, namespace)
        self.fields = fields

    def IsDefinition(self):
        return True

    def IsExportable(self):
        return True

    def __str__(self):
        suffix = '%s, {%s}' % (self.name, self.fields)
        return self._TypeStringHelper(suffix)


class Union(_NestedType):
    pass


class Enum(_NestedType):
    pass


class Class(_GenericDeclaration):
    def __init__(self, start, end, name, bases, templated_types, body, namespace):
        _GenericDeclaration.__init__(self, start, end, name, namespace)
        self.bases = bases
        self.body = body
        self.templated_types = templated_types

    def IsDeclaration(self):
        return self.bases is None and self.body is None

    def IsDefinition(self):
        return not self.IsDeclaration()

    def IsExportable(self):
        return not self.IsDeclaration()

    def Requires(self, node):
        # TODO(nnorwitz): handle namespaces, etc.
        if self.bases:
            for token_list in self.bases:
                # TODO(nnorwitz): bases are tokens, do name comparision.
                for token in token_list:
                    if token.name == node.name:
                        return True
        # TODO(nnorwitz): search in body too.
        return False

    def __str__(self):
        name = self.name
        if self.templated_types:
            name += '<%s>' % self.templated_types
        suffix = '%s, %s, %s' % (name, self.bases, self.body)
        return self._TypeStringHelper(suffix)


class Struct(Class):
    pass


class Function(_GenericDeclaration):
    def __init__(self, start, end, name, return_type, parameters,
                 modifiers, templated_types, body, namespace):
        _GenericDeclaration.__init__(self, start, end, name, namespace)
        converter = TypeConverter(namespace)
        self.return_type = converter.CreateReturnType(return_type)
        self.parameters = converter.ToParameters(parameters)
        self.modifiers = modifiers
        self.body = body
        self.templated_types = templated_types

    def IsDeclaration(self):
        return self.body is None

    def IsDefinition(self):
        return self.body is not None

    def IsExportable(self):
        if self.return_type and 'static' in self.return_type.modifiers:
            return False
        return None not in self.namespace

    def Requires(self, node):
        if self.parameters:
            # TODO(nnorwitz): parameters are tokens, do name comparision.
            for p in self.parameters:
                if p.name == node.name:
                    return True
        # TODO(nnorwitz): search in body too.
        return False

    def __str__(self):
        # TODO(nnorwitz): add templated_types.
        suffix = ('%s %s(%s), 0x%02x, %s' %
                  (self.return_type, self.name, self.parameters,
                   self.modifiers, self.body))
        return self._TypeStringHelper(suffix)


class Method(Function):
    def __init__(self, start, end, name, in_class, return_type, parameters,
                 modifiers, templated_types, body, namespace):
        Function.__init__(self, start, end, name, return_type, parameters,
                          modifiers, templated_types, body, namespace)
        # TODO(nnorwitz): in_class could also be a namespace which can
        # mess up finding functions properly.
        self.in_class = in_class


class Type(_GenericDeclaration):
    """Type used for any variable (eg class, primitive, struct, etc)."""

    def __init__(self, start, end, name, templated_types, modifiers,
                 reference, pointer, array):
        """
        Args:
          name: str name of main type
          templated_types: [Class (Type?)] template type info between <>
          modifiers: [str] type modifiers (keywords) eg, const, mutable, etc.
          reference, pointer, array: bools
        """
        _GenericDeclaration.__init__(self, start, end, name, [])
        self.templated_types = templated_types
        if not name and modifiers:
            self.name = modifiers.pop()
        self.modifiers = modifiers
        self.reference = reference
        self.pointer = pointer
        self.array = array

    def __str__(self):
        prefix = ''
        if self.modifiers:
            prefix = ' '.join(self.modifiers) + ' '
        name = str(self.name)
        if self.templated_types:
            name += '<%s>' % self.templated_types
        suffix = prefix + name
        if self.reference:
            suffix += '&'
        if self.pointer:
            suffix += '*'
        if self.array:
            suffix += '[]'
        return self._TypeStringHelper(suffix)

    # By definition, Is* are always False.  A Type can only exist in
    # some sort of variable declaration, parameter, or return value.
    def IsDeclaration(self):
        return False

    def IsDefinition(self):
        return False

    def IsExportable(self):
        return False


class TypeConverter(object):

    def __init__(self, namespace_stack):
        self.namespace_stack = namespace_stack

    def _GetTemplateEnd(self, tokens, start):
        count = 1
        end = start
        while 1:
            token = tokens[end]
            end += 1
            if token.name == '<':
                count += 1
            elif token.name == '>':
                count -= 1
                if count == 0:
                    break
        return tokens[start:end-1], end

    def ToType(self, tokens):
        """Convert [Token,...] to [Class(...), ] useful for base classes.
        For example, code like class Foo : public Bar<x, y> { ... };
        the "Bar<x, y>" portion gets converted to an AST.

        Returns:
          [Class(...), ...]
        """
        result = []
        name_tokens = []
        reference = pointer = array = False

        def AddType(templated_types):
            # Partition tokens into name and modifier tokens.
            names = []
            modifiers = []
            for t in name_tokens:
                if keywords.IsKeyword(t.name):
                    modifiers.append(t.name)
                else:
                    names.append(t.name)
            name = ''.join(names)
            if name_tokens:
                result.append(Type(name_tokens[0].start, name_tokens[-1].end,
                                   name, templated_types, modifiers,
                                   reference, pointer, array))
            del name_tokens[:]

        i = 0
        end = len(tokens)
        while i < end:
            token = tokens[i]
            if token.name == '<':
                new_tokens, new_end = self._GetTemplateEnd(tokens, i+1)
                AddType(self.ToType(new_tokens))
                # If there is a comma after the template, we need to consume
                # that here otherwise it becomes part of the name.
                i = new_end
                reference = pointer = array = False
            elif token.name == ',':
                AddType([])
                reference = pointer = array = False
            elif token.name == '*':
                pointer = True
            elif token.name == '&':
                reference = True
            elif token.name == '[':
               pointer = True
            elif token.name == ']':
                pass
            else:
                name_tokens.append(token)
            i += 1

        if name_tokens:
            # No '<' in the tokens, just a simple name and no template.
            AddType([])
        return result

    def DeclarationToParts(self, parts, needs_name_removed):
        name = None
        default = []
        if needs_name_removed:
            # Handle default (initial) values properly.
            for i, t in enumerate(parts):
                if t.name == '=':
                    default = parts[i+1:]
                    name = parts[i-1].name
                    if name == ']' and parts[i-2].name == '[':
                        name = parts[i-3].name
                        i -= 1
                    parts = parts[:i-1]
                    break
            else:
                if parts[-1].token_type == tokenize.NAME:
                    name = parts.pop().name
                else:
                    # TODO(nnorwitz): this is a hack that happens for code like
                    # Register(Foo<T>); where it thinks this is a function call
                    # but it's actually a declaration.
                    name = '???'
        modifiers = []
        type_name = []
        other_tokens = []
        templated_types = []
        i = 0
        end = len(parts)
        while i < end:
            p = parts[i]
            if keywords.IsKeyword(p.name):
                modifiers.append(p.name)
            elif p.name == '<':
                templated_tokens, new_end = self._GetTemplateEnd(parts, i+1)
                templated_types = self.ToType(templated_tokens)
                i = new_end - 1
                # Don't add a spurious :: to data members being initialized.
                next_index = i + 1
                if next_index < end and parts[next_index].name == '::':
                    i += 1
            elif p.name in ('[', ']', '='):
                # These are handled elsewhere.
                other_tokens.append(p)
            elif p.name not in ('*', '&', '>'):
                # Ensure that names have a space between them.
                if (type_name and type_name[-1].token_type == tokenize.NAME and
                    p.token_type == tokenize.NAME):
                    type_name.append(tokenize.Token(tokenize.SYNTAX, ' ', 0, 0))
                type_name.append(p)
            else:
                other_tokens.append(p)
            i += 1
        type_name = ''.join([t.name for t in type_name])
        return name, type_name, templated_types, modifiers, default, other_tokens

    def ToParameters(self, tokens):
        if not tokens:
            return []

        result = []
        name = type_name = ''
        type_modifiers = []
        pointer = reference = array = False
        first_token = None
        default = []

        def AddParameter(end):
            if default:
                del default[0]  # Remove flag.
            parts = self.DeclarationToParts(type_modifiers, True)
            (name, type_name, templated_types, modifiers,
             unused_default, unused_other_tokens) = parts
            parameter_type = Type(first_token.start, first_token.end,
                                  type_name, templated_types, modifiers,
                                  reference, pointer, array)
            p = Parameter(first_token.start, end, name,
                          parameter_type, default)
            result.append(p)

        template_count = 0
        for s in tokens:
            if not first_token:
                first_token = s
            if s.name == '<':
                template_count += 1
            elif s.name == '>':
                template_count -= 1
            if template_count > 0:
                type_modifiers.append(s)
                continue

            if s.name == ',':
                AddParameter(s.start)
                name = type_name = ''
                type_modifiers = []
                pointer = reference = array = False
                first_token = None
                default = []
            elif s.name == '*':
                pointer = True
            elif s.name == '&':
                reference = True
            elif s.name == '[':
                array = True
            elif s.name == ']':
                pass  # Just don't add to type_modifiers.
            elif s.name == '=':
                # Got a default value.  Add any value (None) as a flag.
                default.append(None)
            elif default:
                default.append(s)
            else:
                type_modifiers.append(s)
        AddParameter(tokens[-1].end)
        return result

    def CreateReturnType(self, return_type_seq):
        if not return_type_seq:
            return None
        start = return_type_seq[0].start
        end = return_type_seq[-1].end
        _, name, templated_types, modifiers, default, other_tokens = \
           self.DeclarationToParts(return_type_seq, False)
        names = [n.name for n in other_tokens]
        reference = '&' in names
        pointer = '*' in names
        array = '[' in names
        return Type(start, end, name, templated_types, modifiers,
                    reference, pointer, array)

    def GetTemplateIndices(self, names):
        # names is a list of strings.
        start = names.index('<')
        end = len(names) - 1
        while end > 0:
            if names[end] == '>':
                break
            end -= 1
        return start, end+1

class AstBuilder(object):
    def __init__(self, token_stream, filename, in_class='', visibility=None,
                 namespace_stack=[]):
        self.tokens = token_stream
        self.filename = filename
        # TODO(nnorwitz): use a better data structure (deque) for the queue.
        # Switching directions of the "queue" improved perf by about 25%.
        # Using a deque should be even better since we access from both sides.
        self.token_queue = []
        self.namespace_stack = namespace_stack[:]
        self.in_class = in_class
        if in_class is None:
            self.in_class_name_only = None
        else:
            self.in_class_name_only = in_class.split('::')[-1]
        self.visibility = visibility
        self.in_function = False
        self.current_token = None
        # Keep the state whether we are currently handling a typedef or not.
        self._handling_typedef = False

        self.converter = TypeConverter(self.namespace_stack)

    def HandleError(self, msg, token):
        printable_queue = list(reversed(self.token_queue[-20:]))
        sys.stderr.write('Got %s in %s @ %s %s\n' %
                         (msg, self.filename, token, printable_queue))

    def Generate(self):
        while 1:
            token = self._GetNextToken()
            if not token:
                break

            # Get the next token.
            self.current_token = token

            # Dispatch on the next token type.
            if token.token_type == _INTERNAL_TOKEN:
                if token.name == _NAMESPACE_POP:
                    self.namespace_stack.pop()
                continue

            try:
                result = self._GenerateOne(token)
                if result is not None:
                    yield result
            except:
                self.HandleError('exception', token)
                raise

    def _CreateVariable(self, pos_token, name, type_name, type_modifiers,
                        ref_pointer_name_seq, templated_types, value=None):
        reference = '&' in ref_pointer_name_seq
        pointer = '*' in ref_pointer_name_seq
        array = '[' in ref_pointer_name_seq
        var_type = Type(pos_token.start, pos_token.end, type_name,
                        templated_types, type_modifiers,
                        reference, pointer, array)
        return VariableDeclaration(pos_token.start, pos_token.end,
                                   name, var_type, value, self.namespace_stack)

    def _GenerateOne(self, token):
        if token.token_type == tokenize.NAME:
            if (keywords.IsKeyword(token.name) and
                not keywords.IsBuiltinType(token.name)):
                method = getattr(self, 'handle_' + token.name)
                return method()
            elif token.name == self.in_class_name_only:
                # The token name is the same as the class, must be a ctor if
                # there is a paren.  Otherwise, it's the return type.
                # Peek ahead to get the next token to figure out which.
                next = self._GetNextToken()
                self._AddBackToken(next)
                if next.token_type == tokenize.SYNTAX and next.name == '(':
                    return self._GetMethod([token], FUNCTION_CTOR, None, True)
                # Fall through--handle like any other method.

            # Handle data or function declaration/definition.
            syntax = tokenize.SYNTAX
            temp_tokens, last_token = \
                self._GetVarTokensUpTo(syntax, '(', ';', '{', '[')
            temp_tokens.insert(0, token)
            if last_token.name == '(':
                # If there is an assignment before the paren,
                # this is an expression, not a method.
                expr = bool([e for e in temp_tokens if e.name == '='])
                if expr:
                    new_temp = self._GetTokensUpTo(tokenize.SYNTAX, ';')
                    temp_tokens.append(last_token)
                    temp_tokens.extend(new_temp)
                    last_token = tokenize.Token(tokenize.SYNTAX, ';', 0, 0)

            if last_token.name == '[':
                # Handle array, this isn't a method, unless it's an operator.
                # TODO(nnorwitz): keep the size somewhere.
                # unused_size = self._GetTokensUpTo(tokenize.SYNTAX, ']')
                temp_tokens.append(last_token)
                if temp_tokens[-2].name == 'operator':
                    temp_tokens.append(self._GetNextToken())
                else:
                    temp_tokens2, last_token = \
                        self._GetVarTokensUpTo(tokenize.SYNTAX, ';')
                    temp_tokens.extend(temp_tokens2)

            if last_token.name == ';':
                # Handle data, this isn't a method.
                parts = self.converter.DeclarationToParts(temp_tokens, True)
                (name, type_name, templated_types, modifiers, default,
                 unused_other_tokens) = parts

                t0 = temp_tokens[0]
                names = [t.name for t in temp_tokens]
                if templated_types:
                    start, end = self.converter.GetTemplateIndices(names)
                    names = names[:start] + names[end:]
                default = ''.join([t.name for t in default])
                return self._CreateVariable(t0, name, type_name, modifiers,
                                            names, templated_types, default)
            if last_token.name == '{':
                self._AddBackTokens(temp_tokens[1:])
                self._AddBackToken(last_token)
                method_name = temp_tokens[0].name
                method = getattr(self, 'handle_' + method_name, None)
                if not method:
                    # Must be declaring a variable.
                    # TODO(nnorwitz): handle the declaration.
                    return None
                return method()
            return self._GetMethod(temp_tokens, 0, None, False)
        elif token.token_type == tokenize.SYNTAX:
            if token.name == '~' and self.in_class:
                # Must be a dtor (probably not in method body).
                token = self._GetNextToken()
                # self.in_class can contain A::Name, but the dtor will only
                # be Name.  Make sure to compare against the right value.
                if (token.token_type == tokenize.NAME and
                    token.name == self.in_class_name_only):
                    return self._GetMethod([token], FUNCTION_DTOR, None, True)
            # TODO(nnorwitz): handle a lot more syntax.
        elif token.token_type == tokenize.PREPROCESSOR:
            # TODO(nnorwitz): handle more preprocessor directives.
            # token starts with a #, so remove it and strip whitespace.
            name = token.name[1:].lstrip()
            if name.startswith('include'):
                # Remove "include".
                name = name[7:].strip()
                assert name
                # Handle #include \<newline> "header-on-second-line.h".
                if name.startswith('\\'):
                    name = name[1:].strip()
                assert name[0] in '<"', token
                assert name[-1] in '>"', token
                system = name[0] == '<'
                filename = name[1:-1]
                return Include(token.start, token.end, filename, system)
            if name.startswith('define'):
                # Remove "define".
                name = name[6:].strip()
                assert name
                value = ''
                for i, c in enumerate(name):
                    if c.isspace():
                        value = name[i:].lstrip()
                        name = name[:i]
                        break
                return Define(token.start, token.end, name, value)
            if name.startswith('if') and name[2:3].isspace():
                condition = name[3:].strip()
                if condition.startswith('0') or condition.startswith('(0)'):
                    self._SkipIf0Blocks()
        return None

    def _GetTokensUpTo(self, expected_token_type, expected_token):
        return self._GetVarTokensUpTo(expected_token_type, expected_token)[0]

    def _GetVarTokensUpTo(self, expected_token_type, *expected_tokens):
        last_token = self._GetNextToken()
        tokens = []
        while (last_token.token_type != expected_token_type or
               last_token.name not in expected_tokens):
            tokens.append(last_token)
            last_token = self._GetNextToken()
        return tokens, last_token

    # TODO(nnorwitz): remove _IgnoreUpTo() it shouldn't be necesary.
    def _IgnoreUpTo(self, token_type, token):
        unused_tokens = self._GetTokensUpTo(token_type, token)

    def _SkipIf0Blocks(self):
        count = 1
        while 1:
            token = self._GetNextToken()
            if token.token_type != tokenize.PREPROCESSOR:
                continue

            name = token.name[1:].lstrip()
            if name.startswith('endif'):
                count -= 1
                if count == 0:
                    break
            elif name.startswith('if'):
                count += 1

    def _GetMatchingChar(self, open_paren, close_paren, GetNextToken=None):
        if GetNextToken is None:
            GetNextToken = self._GetNextToken
        # Assumes the current token is open_paren and we will consume
        # and return up to the close_paren.
        count = 1
        token = GetNextToken()
        while 1:
            if token.token_type == tokenize.SYNTAX:
                if token.name == open_paren:
                    count += 1
                elif token.name == close_paren:
                    count -= 1
                    if count == 0:
                        break
            yield token
            token = GetNextToken()
        yield token

    def _GetParameters(self):
        return self._GetMatchingChar('(', ')')

    def GetScope(self):
        return self._GetMatchingChar('{', '}')

    def _GetNextToken(self):
        if self.token_queue:
            return self.token_queue.pop()
        return next(self.tokens)

    def _AddBackToken(self, token):
        if token.whence == tokenize.WHENCE_STREAM:
            token.whence = tokenize.WHENCE_QUEUE
            self.token_queue.insert(0, token)
        else:
            assert token.whence == tokenize.WHENCE_QUEUE, token
            self.token_queue.append(token)

    def _AddBackTokens(self, tokens):
        if tokens:
            if tokens[-1].whence == tokenize.WHENCE_STREAM:
                for token in tokens:
                    token.whence = tokenize.WHENCE_QUEUE
                self.token_queue[:0] = reversed(tokens)
            else:
                assert tokens[-1].whence == tokenize.WHENCE_QUEUE, tokens
                self.token_queue.extend(reversed(tokens))

    def GetName(self, seq=None):
        """Returns ([tokens], next_token_info)."""
        GetNextToken = self._GetNextToken
        if seq is not None:
            it = iter(seq)
            GetNextToken = lambda: next(it)
        next_token = GetNextToken()
        tokens = []
        last_token_was_name = False
        while (next_token.token_type == tokenize.NAME or
               (next_token.token_type == tokenize.SYNTAX and
                next_token.name in ('::', '<'))):
            # Two NAMEs in a row means the identifier should terminate.
            # It's probably some sort of variable declaration.
            if last_token_was_name and next_token.token_type == tokenize.NAME:
                break
            last_token_was_name = next_token.token_type == tokenize.NAME
            tokens.append(next_token)
            # Handle templated names.
            if next_token.name == '<':
                tokens.extend(self._GetMatchingChar('<', '>', GetNextToken))
                last_token_was_name = True
            next_token = GetNextToken()
        return tokens, next_token

    def GetMethod(self, modifiers, templated_types):
        return_type_and_name = self._GetTokensUpTo(tokenize.SYNTAX, '(')
        assert len(return_type_and_name) >= 1
        return self._GetMethod(return_type_and_name, modifiers, templated_types,
                               False)

    def _GetMethod(self, return_type_and_name, modifiers, templated_types,
                   get_paren):
        template_portion = None
        if get_paren:
            token = self._GetNextToken()
            assert token.token_type == tokenize.SYNTAX, token
            if token.name == '<':
                # Handle templatized dtors.
                template_portion = [token]
                template_portion.extend(self._GetMatchingChar('<', '>'))
                token = self._GetNextToken()
            assert token.token_type == tokenize.SYNTAX, token
            assert token.name == '(', token

        name = return_type_and_name.pop()
        # Handle templatized ctors.
        if name.name == '>':
            index = 1
            while return_type_and_name[index].name != '<':
                index += 1
            template_portion = return_type_and_name[index:] + [name]
            del return_type_and_name[index:]
            name = return_type_and_name.pop()
        elif name.name == ']':
            rt = return_type_and_name
            assert rt[-1].name == '[', return_type_and_name
            assert rt[-2].name == 'operator', return_type_and_name
            name_seq = return_type_and_name[-2:]
            del return_type_and_name[-2:]
            name = tokenize.Token(tokenize.NAME, 'operator[]',
                                  name_seq[0].start, name.end)
            # Get the open paren so _GetParameters() below works.
            unused_open_paren = self._GetNextToken()

        # TODO(nnorwitz): store template_portion.
        return_type = return_type_and_name
        indices = name
        if return_type:
            indices = return_type[0]

        # Force ctor for templatized ctors.
        if name.name == self.in_class and not modifiers:
            modifiers |= FUNCTION_CTOR
        parameters = list(self._GetParameters())
        del parameters[-1]              # Remove trailing ')'.

        # Handling operator() is especially weird.
        if name.name == 'operator' and not parameters:
            token = self._GetNextToken()
            assert token.name == '(', token
            parameters = list(self._GetParameters())
            del parameters[-1]          # Remove trailing ')'.

        token = self._GetNextToken()
        while token.token_type == tokenize.NAME:
            modifier_token = token
            token = self._GetNextToken()
            if modifier_token.name == 'const':
                modifiers |= FUNCTION_CONST
            elif modifier_token.name == '__attribute__':
                # TODO(nnorwitz): handle more __attribute__ details.
                modifiers |= FUNCTION_ATTRIBUTE
                assert token.name == '(', token
                # Consume everything between the (parens).
                unused_tokens = list(self._GetMatchingChar('(', ')'))
                token = self._GetNextToken()
            elif modifier_token.name == 'throw':
                modifiers |= FUNCTION_THROW
                assert token.name == '(', token
                # Consume everything between the (parens).
                unused_tokens = list(self._GetMatchingChar('(', ')'))
                token = self._GetNextToken()
            elif modifier_token.name == 'override':
          