// Copyright 2007, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: wan@google.com (Zhanyong Wan)

// Google Mock - a framework for writing C++ mock classes.
//
// This file tests the function mocker classes.

#include "gmock/gmock-generated-function-mockers.h"

#if GTEST_OS_WINDOWS
// MSDN says the header file to be included for STDMETHOD is BaseTyps.h but
// we are getting compiler errors if we use basetyps.h, hence including
// objbase.h for definition of STDMETHOD.
# include <objbase.h>
#endif  // GTEST_OS_WINDOWS

#include <map>
#include <string>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

// There is a bug in MSVC (fixed in VS 2008) that prevents creating a
// mock for a function with const arguments, so we don't test such
// cases for MSVC versions older than 2008.
#if !GTEST_OS_WINDOWS || (_MSC_VER >= 1500)
# define GMOCK_ALLOWS_CONST_PARAM_FUNCTIONS
#endif  // !GTEST_OS_WINDOWS || (_MSC_VER >= 1500)

namespace testing {
namespace gmock_generated_function_mockers_test {

using testing::internal::string;
using testing::_;
using testing::A;
using testing::An;
using testing::AnyNumber;
using testing::Const;
using testing::DoDefault;
using testing::Eq;
using testing::Lt;
using testing::MockFunction;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::TypedEq;

class FooInterface {
 public:
  virtual ~FooInterface() {}

  virtual void VoidReturning(int x) = 0;

  virtual int Nullary() = 0;
  virtual bool Unary(int x) = 0;
  virtual long Binary(short x, int y) = 0;  // NOLINT
  virtual int Decimal(bool b, char c, short d, int e, long f,  // NOLINT
                      float g, double h, unsigned i, char* j, const string& k)
      = 0;

  virtual bool TakesNonConstReference(int& n) = 0;  // NOLINT
  virtual string TakesConstReference(const int& n) = 0;
#ifdef GMOCK_ALLOWS_CONST_PARAM_FUNCTIONS
  virtual bool TakesConst(const int x) = 0;
#endif  // GMOCK_ALLOWS_CONST_PARAM_FUNCTIONS

  virtual int OverloadedOnArgumentNumber() = 0;
  virtual int OverloadedOnArgumentNumber(int n) = 0;

  virtual int OverloadedOnArgumentType(int n) = 0;
  virtual char OverloadedOnArgumentType(char c) = 0;

  virtual int OverloadedOnConstness() = 0;
  virtual char OverloadedOnConstness() const = 0;

  virtual int TypeWithHole(int (*func)()) = 0;
  virtual int TypeWithComma(const std::map<int, string>& a_map) = 0;

#if GTEST_OS_WINDOWS
  STDMETHOD_(int, CTNullary)() = 0;
  STDMETHOD_(bool, CTUnary)(int x) = 0;
  STDMETHOD_(int, CTDecimal)(bool b, char c, short d, int e, long f,  // NOLINT
      float g, double h, unsigned i, char* j, const string& k) = 0;
  STDMETHOD_(char, CTConst)(int x) const = 0;
#endif  // GTEST_OS_WINDOWS
};

// Const qualifiers on arguments were once (incorrectly) considered
// significant in determining whether two virtual functions had the same
// signature. This was fixed in Visual Studio 2008. However, the compiler
// still emits a warning that alerts about this change in behavior.
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable : 4373)
#endif
class MockFoo : public FooInterface {
 public:
  MockFoo() {}

  // Makes sure that a mock function parameter can be named.
  MOCK_METHOD1(VoidReturning, void(int n));  // NOLINT

  MOCK_METHOD0(Nullary, int());  // NOLINT

  // Makes sure that a mock function parameter can be unnamed.
  MOCK_METHOD1(Unary, bool(int));  // NOLINT
  MOCK_METHOD2(Binary, long(short, int));  // NOLINT
  MOCK_METHOD10(Decimal, int(bool, char, short, int, long, float,  // NOLINT
                             double, unsigned, char*, const string& str));

  MOCK_METHOD1(TakesNonConstReference, bool(int&));  // NOLINT
  MOCK_METHOD1(TakesConstReference, string(const int&));

#ifdef GMOCK_ALLOWS_CONST_PARAM_FUNCTIONS
  MOCK_METHOD1(TakesConst, bool(const int));  // NOLINT
#endif

  // Tests that the function return type can contain unprotected comma.
  MOCK_METHOD0(ReturnTypeWithComma, std::map<int, string>());
  MOCK_CONST_METHOD1(ReturnTypeWithComma,
                     std::map<int, string>(int));  // NOLINT

  MOCK_METHOD0(OverloadedOnArgumentNumber, int());  // NOLINT
  MOCK_METHOD1(OverloadedOnArgumentNumber, int(int));  // NOLINT

  MOCK_METHOD1(OverloadedOnArgumentType, int(int));  // NOLINT
  MOCK_METHOD1(OverloadedOnArgumentType, char(char));  // NOLINT

  MOCK_METHOD0(OverloadedOnConstness, int());  // NOLINT
  MOCK_CONST_METHOD0(OverloadedOnConsm(make_tuple(UnaryFunctor())));
}

// Tests using InvokeArgument with a 5-ary function.
TEST(InvokeArgumentTest, Function5) {
  Action<int(int(*)(int, int, int, int, int))> a =  // NOLINT
      InvokeArgument<0>(10000, 2000, 300, 40, 5);
  EXPECT_EQ(12345, a.Perform(make_tuple(&SumOf5)));
}

// Tests using InvokeArgument with a 5-ary functor.
TEST(InvokeArgumentTest, Functor5) {
  Action<int(SumOf5Functor)> a =  // NOLINT
      InvokeArgument<0>(10000, 2000, 300, 40, 5);
  EXPECT_EQ(12345, a.Perform(make_tuple(SumOf5Functor())));
}

// Tests using InvokeArgument with a 6-ary function.
TEST(InvokeArgumentTest, Function6) {
  Action<int(int(*)(int, int, int, int, int, int))> a =  // NOLINT
      InvokeArgument<0>(100000, 20000, 3000, 400, 50, 6);
  EXPECT_EQ(123456, a.Perform(make_tuple(&SumOf6)));
}

// Tests using InvokeArgument with a 6-ary functor.
TEST(InvokeArgumentTest, Functor6) {
  Action<int(SumOf6Functor)> a =  // NOLINT
      InvokeArgument<0>(100000, 20000, 3000, 400, 50, 6);
  EXPECT_EQ(123456, a.Perform(make_tuple(SumOf6Functor())));
}

// Tests using InvokeArgument with a 7-ary function.
TEST(InvokeArgumentTest, Function7) {
  Action<string(string(*)(const char*, const char*, const char*,
                          const char*, const char*, const char*,
                          const char*))> a =
      InvokeArgument<0>("1", "2", "3", "4", "5", "6", "7");
  EXPECT_EQ("1234567", a.Perform(make_tuple(&Concat7)));
}

// Tests using InvokeArgument with a 8-ary function.
TEST(InvokeArgumentTest, Function8) {
  Action<string(string(*)(const char*, const char*, const char*,
                          const char*, const char*, const char*,
                          const char*, const char*))> a =
      InvokeArgument<0>("1", "2", "3", "4", "5", "6", "7", "8");
  EXPECT_EQ("12345678", a.Perform(make_tuple(&Concat8)));
}

// Tests using InvokeArgument with a 9-ary function.
TEST(InvokeArgumentTest, Function9) {
  Action<string(string(*)(const char*, const char*, const char*,
                          const char*, const char*, const char*,
                          const char*, const char*, const char*))> a =
      InvokeArgument<0>("1", "2", "3", "4", "5", "6", "7", "8", "9");
  EXPECT_EQ("123456789", a.Perform(make_tuple(&Concat9)));
}

// Tests using InvokeArgument with a 10-ary function.
TEST(InvokeArgumentTest, Function10) {
  Action<string(string(*)(const char*, const char*, const char*,
                          const char*, const char*, const char*,
                          const char*, const char*, const char*,
                          const char*))> a =
      InvokeArgument<0>("1", "2", "3", "4", "5", "6", "7", "8", "9", "0");
  EXPECT_EQ("1234567890", a.Perform(make_tuple(&Concat10)));
}

// Tests using InvokeArgument with a function that takes a pointer argument.
TEST(InvokeArgumentTest, ByPointerFunction) {
  Action<const char*(const char*(*)(const char* input, short n))> a =  // NOLINT
      InvokeArgument<0>(static_cast<const char*>("Hi"), Short(1));
  EXPECT_STREQ("i", a.Perform(make_tuple(&Binary)));
}

// Tests using InvokeArgument with a function that takes a const char*
// by passing it a C-string literal.
TEST(InvokeArgumentTest, FunctionWithCStringLiteral) {
  Action<const char*(const char*(*)(const char* input, short n))> a =  // NOLINT
      InvokeArgument<0>("Hi", Short(1));
  EXPECT_STREQ("i", a.Perform(make_tuple(&Binary)));
}

// Tests using InvokeArgument with a function that takes a const reference.
TEST(InvokeArgumentTest, ByConstReferenceFunction) {
  Action<bool(bool(*function)(const string& s))> a =  // NOLINT
      InvokeArgument<0>(string("Hi"));
  // When action 'a' is constructed, it makes a copy of the temporary
  // string object passed to it, so it's OK to use 'a' later, when the
  // temporary object has already died.
  EXPECT_TRUE(a.Perform(make_tuple(&ByConstRef)));
}

// Tests using InvokeArgument with ByRef() and a function that takes a
// const reference.
TEST(InvokeArgumentTest, ByExplicitConstReferenceFunction) {
  Action<bool(bool(*)(const double& x))> a =  // NOLINT
      InvokeArgument<0>(ByRef(g_double));
  // The above line calls ByRef() on a const value.
  EXPECT_TRUE(a.Perform(make_tuple(&ReferencesGlobalDouble)));

  double x = 0;
  a = InvokeArgument<0>(ByRef(x));  // This calls ByRef() on a non-const.
  EXPECT_FALSE(a.Perform(make_tuple(&ReferencesGlobalDouble)));
}

// Tests using WithArgs and with an action that takes 1 argument.
TEST(WithArgsTest, OneArg) {
  Action<bool(double x, int n)> a = WithArgs<1>(Invoke(Unary));  // NOLINT
  EXPECT_TRUE(a.Perform(make_tuple(1.5, -1)));
  EXPECT_FALSE(a.Perform(make_tuple(1.5, 1)));
}

// Tests using WithArgs with an action that takes 2 arguments.
TEST(WithArgsTest, TwoArgs) {
  Action<const char*(const char* s, double x, short n)> a =
      WithArgs<0, 2>(Invoke(Binary));
  const char s[] = "Hello";
  EXPECT_EQ(s + 2, a.Perform(make_tuple(CharPtr(s), 0.5, Short(2))));
}

// Tests using WithArgs with an action that takes 3 arguments.
TEST(WithArgsTest, ThreeArgs) {
  Action<int(int, double, char, short)> a =  // NOLINT
      WithArgs<0, 2, 3>(Invoke(Ternary));
  EXPECT_EQ(123, a.Perform(make_tuple(100, 6.5, Char(20), Short(3))));
}

// Tests using WithArgs with an action that takes 4 arguments.
TEST(WithArgsTest, FourArgs) {
  Action<string(const char*, const char*, double, const char*, const char*)> a =
      WithArgs<4, 3, 1, 0>(Invoke(Concat4));
  EXPECT_EQ("4310", a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), 2.5,
                                         CharPtr("3"), CharPtr("4"))));
}

// Tests using WithArgs with an action that takes 5 arguments.
TEST(WithArgsTest, FiveArgs) {
  Action<string(const char*, const char*, const char*,
                const char*, const char*)> a =
      WithArgs<4, 3, 2, 1, 0>(Invoke(Concat5));
  EXPECT_EQ("43210",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"),
                                 CharPtr("3"), CharPtr("4"))));
}

// Tests using WithArgs with an action that takes 6 arguments.
TEST(WithArgsTest, SixArgs) {
  Action<string(const char*, const char*, const char*)> a =
      WithArgs<0, 1, 2, 2, 1, 0>(Invoke(Concat6));
  EXPECT_EQ("012210",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"))));
}

// Tests using WithArgs with an action that takes 7 arguments.
TEST(WithArgsTest, SevenArgs) {
  Action<string(const char*, const char*, const char*, const char*)> a =
      WithArgs<0, 1, 2, 3, 2, 1, 0>(Invoke(Concat7));
  EXPECT_EQ("0123210",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"),
                                 CharPtr("3"))));
}

// Tests using WithArgs with an action that takes 8 arguments.
TEST(WithArgsTest, EightArgs) {
  Action<string(const char*, const char*, const char*, const char*)> a =
      WithArgs<0, 1, 2, 3, 0, 1, 2, 3>(Invoke(Concat8));
  EXPECT_EQ("01230123",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"),
                                 CharPtr("3"))));
}

// Tests using WithArgs with an action that takes 9 arguments.
TEST(WithArgsTest, NineArgs) {
  Action<string(const char*, const char*, const char*, const char*)> a =
      WithArgs<0, 1, 2, 3, 1, 2, 3, 2, 3>(Invoke(Concat9));
  EXPECT_EQ("012312323",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"),
                                 CharPtr("3"))));
}

// Tests using WithArgs with an action that takes 10 arguments.
TEST(WithArgsTest, TenArgs) {
  Action<string(const char*, const char*, const char*, const char*)> a =
      WithArgs<0, 1, 2, 3, 2, 1, 0, 1, 2, 3>(Invoke(Concat10));
  EXPECT_EQ("0123210123",
            a.Perform(make_tuple(CharPtr("0"), CharPtr("1"), CharPtr("2"),
                                 CharPtr("3"))));
}

// Tests using WithArgs with an action that is not Invoke().
class SubstractAction : public ActionInterface<int(int, int)> {  // NOLINT
 public:
  virtual int Perform(const tuple<int, int>& args) {
    return get<0>(args) - get<1>(args);
  }
};

TEST(WithArgsTest, NonInvokeAction) {
  Action<int(const string&, int, int)> a =  // NOLINT
      WithArgs<2, 1>(MakeAction(new SubstractAction));
  string s("hello");
  EXPECT_EQ(8, a.Perform(tuple<const string&, int, int>(s, 2, 10)));
}

// Tests using WithArgs to pass all original arguments in the original order.
TEST(WithArgsTest, Identity) {
  Action<int(int x, char y, short z)> a =  // NOLINT
      WithArgs<0, 1, 2>(Invoke(Ternary));
  EXPECT_EQ(123, a.Perform(make_tuple(100, Char(20), Short(3))));
}

// Tests using WithArgs with repeated arguments.
TEST(WithArgsTest, RepeatedArguments) {
  Action<int(bool, int m, int n)> a =  // NOLINT
      WithArgs<1, 1, 1, 1>(Invoke(SumOf4));
  EXPECT_EQ(4, a.Perform(make_tuple(false, 1, 10)));
}

// Tests using WithArgs with reversed argument order.
TEST(WithArgsTest, ReversedArgumentOrder) {
  Action<const char*(short n, const char* input)> a =  // NOLINT
      WithArgs<1, 0>(Invoke(Binary));
  const char s[] = "Hello";
  EXPECT_EQ(s + 2, a.Perform(make_tuple(Short(2), CharPtr(s))));
}

// Tests using WithArgs with compatible, but not identical, argument types.
TEST(WithArgsTest, ArgsOfCompatibleTypes) {
  Action<long(short x, char y, double z, char c)> a =  // NOLINT
      WithArgs<0, 1, 3>(Invoke(Ternary));
  EXPECT_EQ(123, a.Perform(make_tuple(Short(100), Char(20), 5.6, Char(3))));
}

// Tests using WithArgs with an action that returns void.
TEST(WithArgsTest, VoidAction) {
  Action<void(double x, char c, int n)> a = WithArgs<2, 1>(Invoke(VoidBinary));
  g_done = false;
  a.Perform(make_tuple(1.5, 'a', 3));
  EXPECT_TRUE(g_done);
}

// Tests DoAll(a1, a2).
TEST(DoAllTest, TwoActions) {
  int n = 0;
  Action<int(int*)> a = DoAll(SetArgPointee<0>(1),  // NOLINT
                              Return(2));
  EXPECT_EQ(2, a.Perform(make_tuple(&n)));
  EXPECT_EQ(1, n);
}

// Tests DoAll(a1, a2, a3).
TEST(DoAllTest, ThreeActions) {
  int m = 0, n = 0;
  Action<int(int*, int*)> a = DoAll(SetArgPointee<0>(1),  // NOLINT
                                    SetArgPointee<1>(2),
                                    Return(3));
  EXPECT_EQ(3, a.Perform(make_tuple(&m, &n)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
}

// Tests DoAll(a1, a2, a3, a4).
TEST(DoAllTest, FourActions) {
  int m = 0, n = 0;
  char ch = '\0';
  Action<int(int*, int*, char*)> a =  // NOLINT
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            Return(3));
  EXPECT_EQ(3, a.Perform(make_tuple(&m, &n, &ch)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', ch);
}

// Tests DoAll(a1, a2, a3, a4, a5).
TEST(DoAllTest, FiveActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0';
  Action<int(int*, int*, char*, char*)> action =  // NOLINT
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
}

// Tests DoAll(a1, a2, ..., a6).
TEST(DoAllTest, SixActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0', c = '\0';
  Action<int(int*, int*, char*, char*, char*)> action =  // NOLINT
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            SetArgPointee<4>('c'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b, &c)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
  EXPECT_EQ('c', c);
}

// Tests DoAll(a1, a2, ..., a7).
TEST(DoAllTest, SevenActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0', c = '\0', d = '\0';
  Action<int(int*, int*, char*, char*, char*, char*)> action =  // NOLINT
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            SetArgPointee<4>('c'),
            SetArgPointee<5>('d'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b, &c, &d)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
  EXPECT_EQ('c', c);
  EXPECT_EQ('d', d);
}

// Tests DoAll(a1, a2, ..., a8).
TEST(DoAllTest, EightActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0', c = '\0', d = '\0', e = '\0';
  Action<int(int*, int*, char*, char*, char*, char*,  // NOLINT
             char*)> action =
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            SetArgPointee<4>('c'),
            SetArgPointee<5>('d'),
            SetArgPointee<6>('e'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b, &c, &d, &e)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
  EXPECT_EQ('c', c);
  EXPECT_EQ('d', d);
  EXPECT_EQ('e', e);
}

// Tests DoAll(a1, a2, ..., a9).
TEST(DoAllTest, NineActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0', c = '\0', d = '\0', e = '\0', f = '\0';
  Action<int(int*, int*, char*, char*, char*, char*,  // NOLINT
             char*, char*)> action =
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            SetArgPointee<4>('c'),
            SetArgPointee<5>('d'),
            SetArgPointee<6>('e'),
            SetArgPointee<7>('f'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b, &c, &d, &e, &f)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
  EXPECT_EQ('c', c);
  EXPECT_EQ('d', d);
  EXPECT_EQ('e', e);
  EXPECT_EQ('f', f);
}

// Tests DoAll(a1, a2, ..., a10).
TEST(DoAllTest, TenActions) {
  int m = 0, n = 0;
  char a = '\0', b = '\0', c = '\0', d = '\0';
  char e = '\0', f = '\0', g = '\0';
  Action<int(int*, int*, char*, char*, char*, char*,  // NOLINT
             char*, char*, char*)> action =
      DoAll(SetArgPointee<0>(1),
            SetArgPointee<1>(2),
            SetArgPointee<2>('a'),
            SetArgPointee<3>('b'),
            SetArgPointee<4>('c'),
            SetArgPointee<5>('d'),
            SetArgPointee<6>('e'),
            SetArgPointee<7>('f'),
            SetArgPointee<8>('g'),
            Return(3));
  EXPECT_EQ(3, action.Perform(make_tuple(&m, &n, &a, &b, &c, &d, &e, &f, &g)));
  EXPECT_EQ(1, m);
  EXPECT_EQ(2, n);
  EXPECT_EQ('a', a);
  EXPECT_EQ('b', b);
  EXPECT_EQ('c', c);
  EXPECT_EQ('d', d);
  EXPECT_EQ('e', e);
  EXPECT_EQ('f', f);
  EXPECT_EQ('g', g);
}

// The ACTION*() macros trigger warning C4100 (unreferenced formal
// parameter) in MSVC with -W4.  Unfortunately they cannot be fixed in
// the macro definition, as the warnings are generated when the macro
// is expanded and macro expansion cannot contain #pragma.  Therefore
// we suppress them here.
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4100)
#endif

// Tests the ACTION*() macro family.

// Tests that ACTION() can define an action that doesn't reference the
// mock function arguments.
ACTION(Return5) { return 5; }

TEST(ActionMacroTest, WorksWhenNotReferencingArguments) {
  Action<double()> a1 = Return5();
  EXPECT_DOUBLE_EQ(5, a1.Perform(make_tuple()));

  Action<int(double, bool)> a2 = Return5();
  EXPECT_EQ(5, a2.Perform(make_tuple(1, true)));
}

// Tests that ACTION() can define an action that returns void.
ACTION(IncrementArg1) { (*arg1)++; }

TEST(ActionMacroTest, WorksWhenReturningVoid) {
  Action<void(int, int*)> a1 = IncrementArg1();
  int n = 0;
  a1.Perform(make_tuple(5, &n));
  EXPECT_EQ(1, n);
}

// Tests that the body of ACTION() can reference the type of the
// argument.
ACTION(IncrementArg2) {
  StaticAssertTypeEq<int*, arg2_type>();
  arg2_type temp = arg2;
  (*temp)++;
}

TEST(ActionMacroTest, CanReferenceArgumentType) {
  Action<void(int, bool, int*)> a1 = IncrementArg2();
  int n = 0;
  a1.Perform(make_tuple(5, false, &n));
  EXPECT_EQ(1, n);
}

// Tests that the body of ACTION() can reference the argument tuple
// via args_type and args.
ACTION(Sum2) {
  StaticAssertTypeEq<tuple<int, char, int*>, args_type>();
  args_type args_copy = args;
  return get<0>(args_copy) + get<1>(args_copy);
}

TEST(ActionMacroTest, CanReferenceArgumentTuple) {
  Action<int(int, char, int*)> a1 = Sum2();
  int dummy = 0;
  EXPECT_EQ(11, a1.Perform(make_tuple(5, Char(6), &dummy)));
}

// Tests that the body of ACTION() can reference the mock function
// type.
int Dummy(bool flag) { return flag? 1 : 0; }

ACTION(InvokeDummy) {
  StaticAssertTypeEq<int(bool), function_type>();
  function_type* fp = &Dummy;
  return (*fp)(true);
}

TEST(ActionMacroTest, CanReferenceMockFunctionType) {
  Action<int(bool)> a1 = InvokeDummy();
  EXPECT_EQ(1, a1.Perform(make_tuple(true)));
  EXPECT_EQ(1, a1.Perform(make_tuple(false)));
}

// Tests that the body of ACTION() can reference the mock function's
// return type.
ACTION(InvokeDummy2) {
  StaticAssertTypeEq<int, return_type>();
  return_type result = Dummy(true);
  return result;
}

TEST(ActionMacroTest, CanReferenceMockFunctionReturnType) {
  Action<int(bool)> a1 = InvokeDummy2();
  EXPECT_EQ(1, a1.Perform(make_tuple(true)));
  EXPECT_EQ(1, a1.Perform(make_tuple(false)));
}

// Tests that ACTION() works for arguments passed by const reference.
ACTION(ReturnAddrOfConstBoolReferenceArg) {
  StaticAssertTypeEq<const bool&, arg1_type>();
  return &arg1;
}

TEST(ActionMacroTest, WorksForConstReferenceArg) {
  Action<const bool*(int, const bool&)> a = ReturnAddrOfConstBoolReferenceArg();
  const bool b = false;
  EXPECT_EQ(&b, a.Perform(tuple<int, const bool&>(0, b)));
}

// Tests that ACTION() works for arguments passed by non-const reference.
ACTION(ReturnAddrOfIntReferenceArg) {
  StaticAssertTypeEq<int&, arg0_type>();
  return &arg0;
}

TEST(ActionMacroTest, WorksForNonConstReferenceArg) {
  Action<int*(int&, bool, int)> a = ReturnAddrOfIntReferenceArg();
  int n = 0;
  EXPECT_EQ(&n, a.Perform(tuple<int&, bool, int>(n, true, 1)));
}

// Tests that ACTION() can be used in a namespace.
namespace action_test {
ACTION(Sum) { return arg0 + arg1; }
}  // namespace action_test

TEST(ActionMacroTest, WorksInNamespace) {
  Action<int(int, int)> a1 = action_test::Sum();
  EXPECT_EQ(3, a1.Perform(make_tuple(1, 2)));
}

// Tests that the same ACTION definition works for mock functions with
// different argument numbers.
ACTION(PlusTwo) { return arg0 + 2; }

TEST(ActionMacroTest, WorksForDifferentArgumentNumbers) {
  Action<int(int)> a1 = PlusTwo();
  EXPECT_EQ(4, a1.Perform(make_tuple(2)));

  Action<double(float, void*)> a2 = PlusTwo();
  int dummy;
  EXPECT_DOUBLE_EQ(6, a2.Perform(make_tuple(4.0f, &dummy)));
}

// Tests that ACTION_P can define a parameterized action.
ACTION_P(Plus, n) { return arg0 + n; }

TEST(ActionPMacroTest, DefinesParameterizedAction) {
  Action<int(int m, bool t)> a1 = Plus(9);
  EXPECT_EQ(10, a1.Perform(make_tuple(1, true)));
}

// Tests that the body of ACTION_P can reference the argument types
// and the parameter type.
ACTION_P(TypedPlus, n) {
  arg0_type t1 = arg0;
  n_type t2 = n;
  return t1 + t2;
}

TEST(ActionPMacroTest, CanReferenceArgumentAndParameterTypes) {
  Action<int(char m, bool t)> a1 = TypedPlus(9);
  EXPECT_EQ(10, a1.Perform(make_tuple(Char(1), true)));
}

// Tests that a parameterized action can be used in any mock function
// whose type is compatible.
TEST(ActionPMacroTest, WorksInCompatibleMockFunction) {
  Action<std::string(const std::string& s)> a1 = Plus("tail");
  const std::string re = "re";
  EXPECT_EQ("retail", a1.Perform(tuple<const std::string&>(re)));
}

// Tests that we can use ACTION*() to define actions overloaded on the
// number of parameters.

ACTION(OverloadedAction) { return arg0 ? arg1 : "hello"; }

ACTION_P(OverloadedAction, default_value) {
  return arg0 ? arg1 : default_value;
}

ACTION_P2(OverloadedAction, true_value, false_value) {
  return arg0 ? true_value : false_value;
}

TEST(ActionMacroTest, CanDefineOverloadedActions) {
  typedef Action<const char*(bool, const char*)> MyAction;

  const MyAction a1 = OverloadedAction();
  EXPECT_STREQ("hello", a1.Perform(make_tuple(false, CharPtr("world"))));
  EXPECT_STREQ("world", a1.Perform(make_tuple(true, CharPtr("world"))));

  const MyAction a2 = OverloadedAction("hi");
  EXPECT_STREQ("hi", a2.Perform(make_tuple(false, CharPtr("world"))));
  EXPECT_STREQ("world", a2.Perform(make_tuple(true, CharPtr("world"))));

  const MyAction a3 = OverloadedAction("hi", "you");
  EXPECT_STREQ("hi", a3.Perform(make_tuple(true, CharPtr("world"))));
  EXPECT_STREQ("you", a3.Perform(make_tuple(false, CharPtr("world"))));
}

// Tests ACTION_Pn where n >= 3.

ACTION_P3(Plus, m, n, k) { return arg0 + m + n + k; }

TEST(ActionPnMacroTest, WorksFor3Parameters) {
  Action<double(int m, bool t)> a1 = Plus(100, 20, 3.4);
  EXPECT_DOUBLE_EQ(3123.4, a1.Perform(make_tuple(3000, true)));

  Action<std::string(const std::string& s)> a2 = Plus("tail", "-", ">");
  const std::string re = "re";
  EXPECT_EQ("retail->", a2.Perform(tuple<const std::string&>(re)));
}

ACTION_P4(Plus, p0, p1, p2, p3) { return arg0 + p0 + p1 + p2 + p3; }

TEST(ActionPnMacroTest, WorksFor4Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4, a1.Perform(make_tuple(10)));
}

ACTION_P5(Plus, p0, p1, p2, p3, p4) { return arg0 + p0 + p1 + p2 + p3 + p4; }

TEST(ActionPnMacroTest, WorksFor5Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5, a1.Perform(make_tuple(10)));
}

ACTION_P6(Plus, p0, p1, p2, p3, p4, p5) {
  return arg0 + p0 + p1 + p2 + p3 + p4 + p5;
}

TEST(ActionPnMacroTest, WorksFor6Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5, 6);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5 + 6, a1.Perform(make_tuple(10)));
}

ACTION_P7(Plus, p0, p1, p2, p3, p4, p5, p6) {
  return arg0 + p0 + p1 + p2 + p3 + p4 + p5 + p6;
}

TEST(ActionPnMacroTest, WorksFor7Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5, 6, 7);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5 + 6 + 7, a1.Perform(make_tuple(10)));
}

ACTION_P8(Plus, p0, p1, p2, p3, p4, p5, p6, p7) {
  return arg0 + p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
}

TEST(ActionPnMacroTest, WorksFor8Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5, 6, 7, 8);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8, a1.Perform(make_tuple(10)));
}

ACTION_P9(Plus, p0, p1, p2, p3, p4, p5, p6, p7, p8) {
  return arg0 + p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
}

TEST(ActionPnMacroTest, WorksFor9Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5, 6, 7, 8, 9);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9, a1.Perform(make_tuple(10)));
}

ACTION_P10(Plus, p0, p1, p2, p3, p4, p5, p6, p7, p8, last_param) {
  arg0_type t0 = arg0;
  last_param_type t9 = last_param;
  return t0 + p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + t9;
}

TEST(ActionPnMacroTest, WorksFor10Parameters) {
  Action<int(int)> a1 = Plus(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(10 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10,
            a1.Perform(make_tuple(10)));
}

// Tests that the action body can promote the parameter types.

ACTION_P2(PadArgument, prefix, suffix) {
  // The following lines promote the two parameters to desired types.
  std::string prefix_str(prefix);
  char suffix_char = static_cast<char>(suffix);
  return prefix_str + arg0 + suffix_char;
}

TEST(ActionPnMacroTest, SimpleTypePromotion) {
  Action<std::string(const char*)> no_promo =
      PadArgument(std::string("foo"), 'r');
  Action<std::string(const char*)> promo =
      PadArgument("foo", static_cast<int>('r'));
  EXPECT_EQ("foobar", no_promo.Perform(make_tuple(CharPtr("ba"))));
  EXPECT_EQ("foobar", promo.Perform(make_tuple(CharPtr("ba"))));
}

// Tests that we can partially restrict parameter types using a
// straight-forward pattern.

// Defines a generic action that doesn't restrict the types of its
// parameters.
ACTION_P3(ConcatImpl, a, b, c) {
  std::stringstream ss;
  ss << a << b << c;
  return ss.str();
}

// Next, we try to restrict that either the first parameter is a
// string, or the second parameter is an int.

// Defines a partially specialized wrapper that restricts the first
// parameter to std::string.
template <typename T1, typename T2>
// ConcatImplActionP3 is the class template ACTION_P3 uses to
// implement ConcatImpl.  We shouldn't change the name as this
// pattern requires the user to use it directly.
ConcatImplActionP3<std::string, T1, T2>
Concat(const std::string& a, T1 b, T2 c) {
  GTEST_INTENTIONAL_CONST_COND_PUSH_()
  if (true) {
  GTEST_INTENTIONAL_CONST_COND_POP_()
    // This branch verifies that ConcatImpl() can be invoked without
    // explicit template arguments.
    return ConcatImpl(a, b, c);
  } else {
    // This branch verifies that ConcatImpl() can also be invoked with
    // explicit template arguments.  It doesn't really need to be
    // executed as this is a compile-time verification.
    return ConcatImpl<std::string, T1, T2>(a, b, c);
  }
}

// Defines another partially specialized wrapper that restricts the
// second parameter to int.
template <typename T1, typename T2>
ConcatImplActionP3<T1, int, T2>
Concat(T1 a, int b, T2 c) {
  return ConcatImpl(a, b, c);
}

TEST(ActionPnMacroTest, CanPartiallyRestrictParameterTypes) {
  Action<const std::string()> a1 = Concat("Hello", "1", 2);
  EXPECT_EQ("Hello12", a1.Perform(make_tuple()));

  a1 = Concat(1, 2, 3);
  EXPECT_EQ("123", a1.Perform(make_tuple()));
}

// Verifies the type of an ACTION*.

ACTION(DoFoo) {}
ACTION_P(DoFoo, p) {}
ACTION_P2(DoFoo, p0, p1) {}

TEST(ActionPnMacroTest, TypesAreCorrect) {
  // DoFoo() must be assignable to a DoFooAction variable.
  DoFooAction a0 = DoFoo();

  // DoFoo(1) must be assignable to a DoFooActionP variable.
  DoFooActionP<int> a1 = DoFoo(1);

  // DoFoo(p1, ..., pk) must be assignable to a DoFooActionPk
  // variable, and so on.
  DoFooActionP2<int, char> a2 = DoFoo(1, '2');
  PlusActionP3<int, int, char> a3 = Plus(1, 2, '3');
  PlusActionP4<int, int, int, char> a4 = Plus(1, 2, 3, '4');
  PlusActionP5<int, int, int, int, char> a5 = Plus(1, 2, 3, 4, '5');
  PlusActionP6<int, int, int, int, int, char> a6 = Plus(1, 2, 3, 4, 5, '6');
  PlusActionP7<int, int, int, int, int, int, char> a7 =
      Plus(1, 2, 3, 4, 5, 6, '7');
  PlusActionP8<int, int, int, int, int, int, int, char> a8 =
      Plus(1, 2, 3, 4, 5, 6, 7, '8');
  PlusActionP9<int, int, int, int, int, int, int, int, char> a9 =
      Plus(1, 2, 3, 4, 5, 6, 7, 8, '9');
  PlusActionP10<int, int, int, int, int, int, int, int, int, char> a10 =
      Plus(1, 2, 3, 4, 5, 6, 7, 8, 9, '0');

  // Avoid "unused variable" warnings.
  (void)a0;
  (void)a1;
  (void)a2;
  (void)a3;
  (void)a4;
  (void)a5;
  (void)a6;
  (void)a7;
  (void)a8;
  (void)a9;
  (void)a10;
}

// Tests that an ACTION_P*() action can be explicitly instantiated
// with reference-typed parameters.

ACTION_P(Plus1, x) { return x; }
ACTION_P2(Plus2, x, y) { return x + y; }
ACTION_P3(Plus3, x, y, z) { return x + y + z; }
ACTION_P10(Plus10, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) {
  return a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9;
}

TEST(ActionPnMacroTest, CanExplicitlyInstantiateWithReferenceTypes) {
  int x = 1, y = 2, z = 3;
  const tuple<> empty = make_tuple();

  Action<int()> a = Plus1<int&>(x);
  EXPECT_EQ(1, a.Perform(empty));

  a = Plus2<const int&, int&>(x, y);
  EXPECT_EQ(3, a.Perform(empty));

  a = Plus3<int&, const int&, int&>(x, y, z);
  EXPECT_EQ(6, a.Perform(empty));

  int n[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  a = Plus10<const int&, int&, const int&, int&, const int&, int&, const int&,
      int&, const int&, int&>(n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7],
                              n[8], n[9]);
  EXPECT_EQ(55, a.Perform(empty));
}

class NullaryConstructorClass {
 public:
  NullaryConstructorClass() : value_(123) {}
  int value_;
};

// Tests using ReturnNew() with a nullary constructor.
TEST(ReturnNewTest, NoArgs) {
  Action<NullaryConstructorClass*()> a = ReturnNew<NullaryConstructorClass>();
  NullaryConstructorClass* c = a.Perform(make_tuple());
  EXPECT_EQ(123, c->value_);
  delete c;
}

class UnaryConstructorClass {
 public:
  explicit UnaryConstructorClass(int value) : value_(value) {}
  int value_;
};

// Tests using ReturnNew() with a unary constructor.
TEST(ReturnNewTest, Unary) {
  Action<UnaryConstructorClass*()> a = ReturnNew<UnaryConstructorClass>(4000);
  UnaryConstructorClass* c = a.Perform(make_tuple());
  EXPECT_EQ(4000, c->value_);
  delete c;
}

TEST(ReturnNewTest, UnaryWorksWhenMockMethodHasArgs) {
  Action<UnaryConstructorClass*(bool, int)> a =
      ReturnNew<UnaryConstructorClass>(4000);
  UnaryConstructorClass* c = a.Perform(make_tuple(false, 5));
  EXPECT_EQ(4000, c->value_);
  delete c;
}

TEST(ReturnNewTest, UnaryWorksWhenMockMethodReturnsPointerToConst) {
  Action<const UnaryConstructorClass*()> a =
      ReturnNew<UnaryConstructorClass>(4000);
  const UnaryConstructorClass* c = a.Perform(make_tuple());
  EXPECT_EQ(4000, c->value_);
  delete c;
}

class TenArgConstructorClass {
 public:
  TenArgConstructorClass(int a1, int a2, int a3, int a4, int a5,
                         int a6, int a7, int a8, int a9, int a10)
    : value_(a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10) {
  }
  int value_;
};

// Tests using ReturnNew() with a 10-argument constructor.
TEST(ReturnNewTest, ConstructorThatTakes10Arguments) {
  Action<TenArgConstructorClass*()> a =
      ReturnNew<TenArgConstructorClass>(1000000000, 200000000, 30000000,
                                        4000000, 500000, 60000,
                                        7000, 800, 90, 0);
  TenArgConstructorClass* c = a.Perform(make_tuple());
  EXPECT_EQ(1234567890, c->value_);
  delete c;
}

// Tests that ACTION_TEMPLATE works when there is no value parameter.
ACTION_TEMPLATE(CreateNew,
                HAS_1_TEMPLATE_PARAMS(typename, T),
                AND_0_VALUE_PARAMS()) {
  return new T;
}

TEST(ActionTemplateTest, WorksWithoutValueParam) {
  const Action<int*()> a = CreateNew<int>();
  int* p = a.Perform(make_tuple());
  delete p;
}

// Tests that ACTION_TEMPLATE works when there are value parameters.
ACTION_TEMPLATE(CreateNew,
                HAS_1_TEMPLATE_PARAMS(typename, T),
                AND_1_VALUE_PARAMS(a0)) {
  return new T(a0);
}

TEST(ActionTemplateTest, WorksWithValueParams) {
  const Action<int*()> a = CreateNew<int>(42);
  int* p = a.Perform(make_tuple());
  EXPECT_EQ(42, *p);
  delete p;
}

// Tests that ACTION_TEMPLATE works for integral template parameters.
ACTION_TEMPLATE(MyDeleteArg,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_0_VALUE_PARAMS()) {
  delete get<k>(args);
}

// Resets a bool variable in the destructor.
class BoolResetter {
 public:
  explicit BoolResetter(bool* value) : value_(value) {}
  ~BoolResetter() { *value_ = false; }
 private:
  bool* value_;
};

TEST(ActionTemplateTest, WorksForIntegralTemplateParams) {
  const Action<void(int*, BoolResetter*)> a = MyDeleteArg<1>();
  int n = 0;
  bool b = true;
  BoolResetter* resetter = new BoolResetter(&b);
  a.Perform(make_tuple(&n, resetter));
  EXPECT_FALSE(b);  // Verifies that resetter is deleted.
}

// Tests that ACTION_TEMPLATES works for template template parameters.
ACTION_TEMPLATE(ReturnSmartPointer,
                HAS_1_TEMPLATE_PARAMS(template <typename Pointee> class,
                                      Pointer),
                AND_1_VALUE_PARAMS(pointee)) {
  return Pointer<pointee_type>(new pointee_type(pointee));
}

TEST(ActionTemplateTest, WorksForTemplateTemplateParameters) {
  using ::testing::internal::linked_ptr;
  const Action<linked_ptr<int>()> a = ReturnSmartPointer<linked_ptr>(42);
  linked_ptr<int> p = a.Perform(make_tuple());
  EXPECT_EQ(42, *p);
}

// Tests that ACTION_TEMPLATE works for 10 template parameters.
template <typename T1, typename T2, typename T3, int k4, bool k5,
          unsigned int k6, typename T7, typename T8, typename T9>
struct GiantTemplate {
 public:
  explicit GiantTemplate(int a_value) : value(a_value) {}
  int value;
};

ACTION_TEMPLATE(ReturnGiant,
                HAS_10_TEMPLATE_PARAMS(
                    typename, T1,
                    typename, T2,
                    typename, T3,
                    int, k4,
                    bool, k5,
                    unsigned int, k6,
                    class, T7,
                    class, T8,
                    class, T9,
                    template <typename T> class, T10),
                AND_1_VALUE_PARAMS(value)) {
  return GiantTemplate<T10<T1>, T2, T3, k4, k5, k6, T7, T8, T9>(value);
}

TEST(ActionTemplateTest, WorksFor10TemplateParameters) {
  using ::testing::internal::linked_ptr;
  typedef GiantTemplate<linked_ptr<int>, bool, double, 5,
      true, 6, char, unsigned, int> Giant;
  const Action<Giant()> a = ReturnGiant<
      int, bool, double, 5, true, 6, char, unsigned, int, linked_ptr>(42);
  Giant giant = a.Perform(make_tuple());
  EXPECT_EQ(42, giant.value);
}

// Tests that ACTION_TEMPLATE works for 10 value parameters.
ACTION_TEMPLATE(ReturnSum,
                HAS_1_TEMPLATE_PARAMS(typename, Number),
                AND_10_VALUE_PARAMS(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)) {
  return static_cast<Number>(v1) + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10;
}

TEST(ActionTemplateTest, WorksFor10ValueParameters) {
  const Action<int()> a = ReturnSum<int>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(55, a.Perform(make_tuple()));
}

// Tests that ACTION_TEMPLATE and ACTION/ACTION_P* can be overloaded
// on the number of value parameters.

ACTION(ReturnSum) { return 0; }

ACTION_P(ReturnSum, x) { return x; }

ACTION_TEMPLATE(ReturnSum,
                HAS_1_TEMPLATE_PARAMS(typename, Number),
                AND_2_VALUE_PARAMS(v1, v2)) {
  return static_cast<Number>(v1) + v2;
}

ACTION_TEMPLATE(ReturnSum,
                HAS_1_TEMPLATE_PARAMS(typename, Number),
                AND_3_VALUE_PARAMS(v1, v2, v3)) {
  return static_cast<Number>(v1) + v2 + v3;
}

ACTION_TEMPLATE(ReturnSum,
                HAS_2_TEMPLATE_PARAMS(typename, Number, int, k),
                AND_4_VALUE_PARAMS(v1, v2, v3, v4)) {
  return static_cast<Number>(v1) + v2 + v3 + v4 + k;
}

TEST(ActionTemplateTest, CanBeOverloadedOnNumberOfValueParameters) {
  const Action<int()> a0 = ReturnSum();
  const Action<int()> a1 = ReturnSum(1);
  const Action<int()> a2 = ReturnSum<int>(1, 2);
  const Action<int()> a3 = ReturnSum<int>(1, 2, 3);
  const Action<int()> a4 = ReturnSum<int, 10000>(2000, 300, 40, 5);
  EXPECT_EQ(0, a0.Perform(make_tuple()));
  EXPECT_EQ(1, a1.Perform(make_tuple()));
  EXPECT_EQ(3, a2.Perform(make_tuple()));
  EXPECT_EQ(6, a3.Perform(make_tuple()));
  EXPECT_EQ(12345, a4.Perform(make_tuple()));
}

#ifdef _MSC_VER
# pragma warning(pop)
#endif

}  // namespace gmock_generated_actions_test
}  // namespace testing
