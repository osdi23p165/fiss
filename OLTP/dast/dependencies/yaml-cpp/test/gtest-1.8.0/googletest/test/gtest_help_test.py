#!/usr/bin/env python
#
# Copyright 2009, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Tests the --help flag of Google C++ Testing Framework.

SYNOPSIS
       gtest_help_test.py --build_dir=BUILD/DIR
         # where BUILD/DIR contains the built gtest_help_test_ file.
       gtest_help_test.py
"""

__author__ = 'wan@google.com (Zhanyong Wan)'

import os
import re
import gtest_test_utils


IS_LINUX = os.name == 'posix' and os.uname()[0] == 'Linux'
IS_WINDOWS = os.name == 'nt'

PROGRAM_PATH = gtest_test_utils.GetTestExecutablePath('gtest_help_test_')
FLAG_PREFIX = '--gtest_'
DEATH_TEST_STYLE_FLAG = FLAG_PREFIX + 'death_test_style'
STREAM_RESULT_TO_FLAG = FLAG_PREFIX + 'stream_result_to'
UNKNOWN_FLAG = FLAG_PREFIX + 'unknown_flag_for_testing'
LIST_TESTS_FLAG = FLAG_PREFIX + 'list_tests'
INCORRECT_FLAG_VARIANTS = [re.sub('^--', '-', LIST_TESTS_FLAG),
                           re.sub('^--', '/', LIST_TESTS_FLAG),
                           re.sub('_', '-', LIST_TESTS_FLAG)]
INTERNAL_FLAG_FOR_TESTING = FLAG_PREFIX + 'internal_flag_for_testing'

SUPPORTS_DEATH_TESTS = "DeathTest" in gtest_test_utils.Subprocess(
    [PROGRAM_PATH, LIST_TESTS_FLAG]).output

# The help message must match this regex.
HELP_REGEX = re.compile(
    FLAG_PREFIX + r'list_tests.*' +
    FLAG_PREFIX + r'filter=.*' +
    FLAG_PREFIX + r'also_run_disabled_tests.*' +
    FLAG_PREFIX + r'repeat=.*' +
    FLAG_PREFIX + r'shuffle.*' +
    FLAG_PREFIX + r'random_seed=.*' +
    FLAG_PREFIX + r'color=.*' +
    FLAG_PREFIX + r'print_time.*' +
    FLAG_PREFIX + r'output=.*' +
    FLAG_PREFIX + r'break_on_failure.*' +
    FLAG_PREFIX + r'throw_on_failure.*' +
    FLAG_PREFIX + r'catch_exceptions=0.*',
    re.DOTALL)


def RunWithFlag(flag):
  """Runs gtest_help_test_ with the given flag.

  Returns:
    the exit code and the text output as a tuple.
  Args:
    flag: the command-line flag to pass to gtest_help_test_, or None.
  """

  if flag is None:
    command = [PROGRAM_PATH]
  else:
    command = [PROGRAM_PATH, flag]
  child = gtest_test_utils.Subprocess(command)
  return child.exit_code, child.output


class GTestHelpTest(gtest_test_utils.TestCase):
  """Tests the --help flag and its equivalent forms."""

  def TestHelpFlag(self, flag):
    """Verifies correct behavior when help flag is specified.

    The right message must be printed and the tests must
    skipped when the given flag is specified.

    Args:
      flag:  A flag to pass to the binary or None.
    """

    exit_code, output = RunWithFlag(flag)
    self.assertEquals(0, exit_code)
    self.assert_(HELP_REGEX.search(output), output)

    if IS_LINUX:
      self.assert_(STREAM_RESULT_TO_FLAG in output, output)
    else:
      self.assert_(STREAM_RESULT_TO_FLAG not in output, output)

    if SUPPORTS_DEATH_TESTS and not IS_WINDOWS:
      self.assert_(DEATH_TEST_STYLE_FLAG in output, output)
    else:
      self.assert_(DEATH_TEST_STYLE_FLAG not in output, output)

  def TestNonHelpFlag(self, flag):
    """Verifies correct behavior when no help flag is specified.

    Verifies that when no help flag is specified, the tests are run
    and the help message is not printed.

    Args:
].*\.(\w+(/\w+)?)')

# The command line flag to tell Google Test to output the list of tests it
# will run.
LIST_TESTS_FLAG = '--gtest_list_tests'

# Indicates whether Google Test supports death tests.
SUPPORTS_DEATH_TESTS = 'HasDeathTest' in gtest_test_utils.Subprocess(
    [COMMAND, LIST_TESTS_FLAG]).output

# Full names of all tests in gtest_filter_unittests_.
PARAM_TESTS = [
    'SeqP/ParamTest.TestX/0',
    'SeqP/ParamTest.TestX/1',
    'SeqP/ParamTest.TestY/0',
    'SeqP/ParamTest.TestY/1',
    'SeqQ/ParamTest.TestX/0',
    'SeqQ/ParamTest.TestX/1',
    'SeqQ/ParamTest.TestY/0',
    'SeqQ/ParamTest.TestY/1',
    ]

DISABLED_TESTS = [
    'BarTest.DISABLED_TestFour',
    'BarTest.DISABLED_TestFive',
    'BazTest.DISABLED_TestC',
    'DISABLED_FoobarTest.Test1',
    'DISABLED_FoobarTest.DISABLED_Test2',
    'DISABLED_FoobarbazTest.TestA',
    ]

if SUPPORTS_DEATH_TESTS:
  DEATH_TESTS = [
    'HasDeathTest.Test1',
    'HasDeathTest.Test2',
    ]
else:
  DEATH_TESTS = []

# All the non-disabled tests.
ACTIVE_TESTS = [
    'FooTest.Abc',
    'FooTest.Xyz',

    'BarTest.TestOne',
    'BarTest.TestTwo',
    'BarTest.TestThree',

    'BazTest.TestOne',
    'BazTest.TestA',
    'BazTest.TestB',
    ] + DEATH_TESTS + PARAM_TESTS

param_tests_present = None

# Utilities.

environ = os.environ.copy()


def SetEnvVar(env_var, value):
  """Sets the env variable to 'value'; unsets it when 'value' is None."""

  if value is not None:
    environ[env_var] = value
  elif env_var in environ:
    del environ[env_var]


def RunAndReturnOutput(args = None):
  """Runs the test program and returns its output."""

  return gtest_test_utils.Subprocess([COMMAND] + (args or []),
                                     env=environ).output


def RunAndExtractTestList(args = None):
  """Runs the test program and returns its exit code and a list of tests run."""

  p = gtest_test_utils.Subprocess([COMMAND] + (args or []), env=environ)
  tests_run = []
  test_case = ''
  test = ''
  for line in p.output.split('\n'):
    match = TEST_CASE_REGEX.match(line)
    if match is not None:
      test_case = match.group(1)
    else:
      match = TEST_REGEX.match(line)
      if match is not None:
        test = match.group(1)
        tests_run.append(test_case + '.' + test)
  return (tests_run, p.exit_code)


def InvokeWithModifiedEnv(extra_env, function, *args, **kwargs):
  """Runs the given function and arguments in a modified environment."""
  try:
    original_env = environ.copy()
    environ.update(extra_env)
    return function(*args, **kwargs)
  finally:
    environ.clear()
    environ.update(original_env)


def RunWithSharding(total_shards, shard_index, command):
  """Runs a test program shard and returns exit code and a list of tests run."""

  extra_env = {SHARD_INDEX_ENV_VAR: str(shard_index),
               TOTAL_SHARDS_ENV_VAR: str(total_shards)}
  return InvokeWithModifiedEnv(extra_env, RunAndExtractTestList, command)

# The unit test.


class GTestFilterUnitTest(gtest_test_utils.TestCase):
  """Tests the env variable or the command line flag to filter tests."""

  # Utilities.

  def AssertSetEqual(self, lhs, rhs):
    """Asserts that two sets are equal."""

    for elem in lhs:
      self.assert_(elem in rhs, '%s in %s' % (elem, rhs))

    for elem in rhs:
      self.assert_(elem in lhs, '%s in %s' % (elem, lhs))

  def AssertPartitionIsValid(self, set_var, list_of_sets):
    """Asserts that list_of_sets is a valid partition of set_var."""

    full_partition = []
    for slice_var in list_of_sets:
      full_partition.extend(slice_var)
    self.assertEqual(len(set_var), len(full_partition))
    self.assertEqual(set(set_var), set(full_partition))

  def AdjustForParameterizedTests(self, tests_to_run):
    """Adjust tests_to_run in case value parameterized tests are disabled."""

    global param_tests_present
    if not param_tests_present:
      return list(set(tests_to_run) - set(PARAM_TESTS))
    else:
      return tests_to_run

  def RunAndVerify(self, gtest_filter, tests_to_run):
    """Checks that the binary runs correct set of tests for a given filter."""

    tests_to_run = self.AdjustForParameterizedTests(tests_to_run)

    # First, tests using the environment variable.

    # Windows removes empty variables from the environment when passing it
    # to a new process.  This means it is impossible to pass an empty filter
    # into a process using the environment variable.  However, we can still
    # test the case when the variable is not supplied (i.e., gtest_filter is
    # None).
    # pylint: disable-msg=C6403
    if CAN_TEST_EMPTY_FILTER or gtest_filter != '':
      SetEnvVar(FILTER_ENV_VAR, gtest_filter)
      tests_run = RunAndExtractTestList()[0]
      SetEnvVar(FILTER_ENV_VAR, None)
      self.AssertSetEqual(tests_run, tests_to_run)
    # pylint: enable-msg=C6403

    # Next, tests using the command line flag.

    if gtest_filter is None:
      args = []
    else:
      args = ['--%s=%s' % (FILTER_FLAG, gtest_filter)]

    tests_run = RunAndExtractTestList(args)[0]
    self.AssertSetEqual(tests_run, tests_to_run)

  def RunAndVerifyWithSharding(self, gtest_filter, total_shards, tests_to_run,
                               args=None, check_exit_0=False):
    """Checks that binary runs correct tests for the given filter and shard.

    Runs all shards of gtest_filter_unittest_ with the given filter, and
    verifies that the right set of tests were run. The union of tests run
    on each shard should be identical to tests_to_run, without duplicates.

    Args:
      gtest_filter: A filter to apply to the tests.
      total_shards: A total number of shards to split test run into.
      tests_to_run: A set of tests expected to run.
      args   :      Arguments to pass to the to the test binary.
      check_exit_0: When set to a true value, make sure that all shards
                    return 0.
    """

    tests_to_run = self.AdjustForParameterizedTests(tests_to_run)

    # Windows removes empty variables from the environment when passing it
    # to a new process.  This means it is impossible to pass an empty filter
    # into a process using the environment variable.  However, we can still
    # test the case when the variable is not supplied (i.e., gtest_filter is
    # None).
    # pylint: disable-msg=C6403
    if CAN_TEST_EMPTY_FILTER or gtest_filter != '':
      SetEnvVar(FILTER_ENV_VAR, gtest_filter)
      partition = []
      for i in range(0, total_shards):
        (tests_run, exit_code) = RunWithSharding(total_shards, i, args)
        if check_exit_0:
          self.assertEqual(0, exit_code)
        partition.append(tests_run)

      self.AssertPartitionIsValid(tests_to_run, partition)
      SetEnvVar(FILTER_ENV_VAR, None)
    # pylint: enable-msg=C6403

  def RunAndVerifyAllowingDisabled(self, gtest_filter, tests_to_run):
    """Checks that the binary runs correct set of tests for the given filter.

    Runs gtest_filter_unittest_ with the given filter, and enables
    disabled tests. Verifies that the right set of tests were run.

    Args:
      gtest_filter: A filter to apply to the tests.
      tests_to_run: A set of tests expected to run.
    """

    tests_to_run = self.AdjustForParameterizedTests(tests_to_run)

    # Construct the command line.
    args = ['--%s' % ALSO_RUN_DISABED_TESTS_FLAG]
    if gtest_filter is not None:
      args.append('--%s=%s' % (FILTER_FLAG, gtest_filter))

    tests_run = RunAndExtractTestList(args)[0]
    self.AssertSetEqual(tests_run, tests_to_run)

  def setUp(self):
    """Sets up test case.

    Determines whether value-parameterized tests are enabled in the binary and
    sets the flags accordingly.
    """

    global param_tests_present
    if param_tests_present is None:
      param_tests_present = PARAM_TEST_REGEX.search(
          RunAndReturnOutput()) is not None

  def testDefaultBehavior(self):
    """Tests the behavior of not specifying the filter."""

    self.RunAndVerify(None, ACTIVE_TESTS)

  def testDefaultBehaviorWithShards(self):
    """Tests the behavior without the filter, with sharding enabled."""

    self.RunAndVerifyWithSharding(None, 1, ACTIVE_TESTS)
    self.RunAndVerifyWithSharding(None, 2, ACTIVE_TESTS)
    self.RunAndVerifyWithSharding(None, len(ACTIVE_TESTS) - 1, ACTIVE_TESTS)
    self.RunAndVerifyWithSharding(None, len(ACTIVE_TESTS), ACTIVE_TESTS)
    self.RunAndVerifyWithSharding(None, len(ACTIVE_TESTS) + 1, ACTIVE_TESTS)

  def testEmptyFilter(self):
    """Tests an empty filter."""

    self.RunAndVerify('', [])
    self.RunAndVerifyWithSharding('', 1, [])
    self.RunAndVerifyWithSharding('', 2, [])

  def testBadFilter(self):
    """Tests a filter that matches nothing."""

    self.RunAndVerify('BadFilter', [])
    self.RunAndVerifyAllowingDisabled('BadFilter', [])

  def testFullName(self):
    """Tests filtering by full name."""

    self.RunAndVerify('FooTest.Xyz', ['FooTest.Xyz'])
    self.RunAndVerifyAllowingDisabled('FooTest.Xyz', ['FooTest.Xyz'])
    self.RunAndVerifyWithSharding('FooTest.Xyz', 5, ['FooTest.Xyz'])

  def testUniversalFilters(self):
    """Tests filters that match everything."""

    self.RunAndVerify('*', ACTIVE_TESTS)
    self.RunAndVerify('*.*', ACTIVE_TESTS)
    self.RunAndVerifyWithSharding('*.*', len(ACTIVE_TESTS) - 3, ACTIVE_TESTS)
    self.RunAndVerifyAllowingDisabled('*', ACTIVE_TESTS + DISABLED_TESTS)
    self.RunAndVerifyAllowingDisabled('*.*', ACTIVE_TESTS + DISABLED_TESTS)

  def testFilterByTestCase(self):
    """Tests filtering by test case name."""

    self.RunAndVerify('FooTest.*', ['FooTest.Abc', 'FooTest.Xyz'])

    BAZ_TESTS = ['BazTest.TestOne', 'BazTest.TestA', 'BazTest.TestB']
    self.RunAndVerify('BazTest.*', BAZ_TESTS)
    self.RunAndVerifyAllowingDisabled('BazTest.*',
                                      BAZ_TESTS + ['BazTest.DISABLED_TestC'])

  def testFilterByTest(self):
    """Tests filtering by test name."""

    self.RunAndVerify('*.TestOne', ['BarTest.TestOne', 'BazTest.TestOne'])

  def testFilterDisabledTests(self):
    """Select only the disabled tests to run."""

    self.RunAndVerify('DISABLED_FoobarTest.Test1', [])
    self.RunAndVerifyAllowingDisabled('DISABLED_FoobarTest.Test1',
                                      ['DISABLED_FoobarTest.Test1'])

    self.RunAndVerify('*DISABLED_*', [])
    self.RunAndVerifyAllowingDisabled('*DISABLED_*', DISABLED_TESTS)

    self.RunAndVerify('*.DISABLED_*', [])
    self.RunAndVerifyAllowingDisabled('*.DISABLED_*', [
        'BarTest.DISABLED_TestFour',
        'BarTest.DISABLED_TestFive',
        'BazTest.DISABLED_TestC',
        'DISABLED_FoobarTest.DISABLED_Test2',
        ])

    self.RunAndVerify('DISABLED_*', [])
    self.RunAndVerifyAllowingDisabled('DISABLED_*', [
        'DISABLED_FoobarTest.Test1',
        'DISABLED_FoobarTest.DISABLED_Test2',
        'DISABLED_FoobarbazTest.TestA',
        ])

  def testWildcardInTestCaseName(self):
    """Tests using wildcard in the test case name."""

    self.RunAndVerify('*a*.*', [
        'BarTest.TestOne',
        'BarTest.TestTwo',
        'BarTest.TestThree',

        'BazTest.TestOne',
        'BazTest.TestA',
        'BazTest.TestB', ] + DEATH_TESTS + PARAM_TESTS)

  def testWildcardInTestName(self):
    """Tests using wildcard in the test name."""

    self.RunAndVerify('*.*A*', ['FooTest.Abc', 'BazTest.TestA'])

  def testFilterWithoutDot(self):
    """Tests a filter that has no '.' in it."""

    self.RunAndVerify('*z*', [
        'FooTest.Xyz',

        'BazTest.TestOne',
        'BazTest.TestA',
        'BazTest.TestB',
        ])

  def testTwoPatterns(self):
    """Tests filters that consist of two patterns."""

    self.RunAndVerify('Foo*.*:*A*', [
        'FooTest.Abc',
        'FooTest.Xyz',

        'BazTest.TestA',
        ])

    # An empty pattern + a non-empty one
    self.RunAndVerify(':*A*', ['FooTest.Abc', 'BazTest.TestA'])

  def testThreePatterns(self):
    """Tests filters that consist of three patterns."""

    self.RunAndVerify('*oo*:*A*:*One', [
        'FooTest.Abc',
        'FooTest.Xyz',

        'BarTest.TestOne',

        'BazTest.TestOne',
        'BazTest.TestA',
        ])

    # The 2nd pattern is empty.
    self.RunAndVerify('*oo*::*One', [
        'FooTest.Abc',
        'FooTest.Xyz',

        'BarTest.TestOne',

        'BazTest.TestOne',
        ])

    # The last 2 patterns are empty.
    self.RunAndVerify('*oo*::', [
        'FooTest.Abc',
        'FooTest.Xyz',
        ])

  def testNegativeFilters(self):
    self.RunAndVerify('*-BazTest.TestOne', [
        'FooTest.Abc',
        'FooTest.Xyz',

        'BarTest.TestOne',
        'BarTest.TestTwo',
        'BarTest.TestThree',

        'BazTest.TestA',
        'BazTest.TestB',
        ] + DEATH_TESTS + PARAM_TESTS)

    self.RunAndVerify('*-FooTest.Abc:BazTest.*', [
        'FooTest.Xyz',

        'BarTest.TestOne',
        'BarTest.TestTwo',
        'BarTest.TestThree',
        ] + DEATH_TESTS + PARAM_TESTS)

    self.RunAndVerify('BarTest.*-BarTest.TestOne', [
        'BarTest.TestTwo',
        'BarTest.TestThree',
        ])

    # Tests without leading '*'.
    self.RunAndVerify('-FooTest.Abc:FooTest.Xyz:BazTest.*', [
        'BarTest.TestOne',
        'BarTest.TestTwo',
        'BarTest.TestThree',
        ] + DEATH_TESTS + PARAM_TESTS)

    # Value parameterized tests.
    self.RunAndVerify('*/*', PARAM_TESTS)

    # Value parameterized tests filtering by the sequence name.
    self.RunAndVerify('SeqP/*', [
        'SeqP/ParamTest.TestX/0',
        'SeqP/ParamTest.TestX/1',
        'SeqP/ParamTest.TestY/0',
        'SeqP/ParamTest.TestY/1',
        ])

    # Value parameterized tests filtering by the test name.
    self.RunAndVerify('*/0', [
        'SeqP/ParamTest.TestX/0',
        'SeqP/ParamTest.TestY/0',
        'SeqQ/ParamTest.TestX/0',
        'SeqQ/ParamTest.TestY/0',
        ])

  def testFlagOverridesEnvVar(self):
    """Tests that the filter flag overrides the filtering env. variable."""

    SetEnvVar(FILTER_ENV_VAR, 'Foo*')
    args = ['--%s=%s' % (FILTER_FLAG, '*One')]
    tests_run = RunAndExtractTestList(args)[0]
    SetEnvVar(FILTER_ENV_VAR, None)

    self.AssertSetEqual(tests_run, ['BarTest.TestOne', 'BazTest.TestOne'])

  def testShardStatusFileIsCreated(self):
    """Tests that the shard file is created if specified in the environment."""

    shard_status_file = os.path.join(gtest_test_utils.GetTempDir(),
                                     'shard_status_file')
    self.assert_(not os.path.exists(shard_status_file))

    extra_env = {SHARD_STATUS_FILE_ENV_VAR: shard_status_file}
    try:
      InvokeWithModifiedEnv(extra_env, RunAndReturnOutput)
    finally:
      self.assert_(os.path.exists(shard_status_file))
      os.remove(shard_status_file)

  def testShardStatusFileIsCreatedWithListTests(self):
    """Tests that the shard file is created with the "list_tests" flag."""

    shard_status_file = os.path.join(gtest_test_utils.GetTempDir(),
                                     'shard_status_file2')
    self.assert_(not os.path.exists(shard_status_file))

    extra_env = {SHARD_STATUS_FILE_ENV_VAR: shard_status_file}
    try:
      output = InvokeWithModifiedEnv(extra_env,
                                     RunAndReturnOutput,
                                     [LIST_TESTS_FLAG])
    finally:
      # This assertion ensures that Google Test enumerated the tests as
      # opposed to running them.
      self.assert_('[==========]' not in output,
                   'Unexpected output during test enumeration.\n'
                   'Please ensure that LIST_TESTS_FLAG is assigned the\n'
                   'correct flag value for listing Google Test tests.')

      self.assert_(os.path.exists(shard_status_file))
      os.remove(shard_status_file)

  if SUPPORTS_DEATH_TESTS:
    def testShardingWorksWithDeathTests(self):
      """Tests integration with death tests and sharding."""

      gtest_filter = 'HasDeathTest.*:SeqP/*'
      expected_tests = [
          'HasDeathTest.Test1',
          'HasDeathTest.Test2',

          'SeqP/ParamTest.TestX/0',
          'SeqP/ParamTest.TestX/1',
          'SeqP/ParamTest.TestY/0',
          'SeqP/ParamTest.TestY/1',
          ]

      for flag in ['--gtest_death_test_style=threadsafe',
                   '--gtest_death_test_style=fast']:
        self.RunAndVerifyWithSharding(gtest_filter, 3, expected_tests,
                                      check_exit_0=True, args=[flag])
        self.RunAndVerifyWithSharding(gtest_filter, 5, expected_tests,
                                      check_exit_0=True, args=[flag])

if __name__ == '__main__':
  gtest_test_utils.Main()
