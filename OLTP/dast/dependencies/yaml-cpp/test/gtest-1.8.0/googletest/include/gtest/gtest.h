// Copyright 2005, Google Inc.
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
//
// The Google C++ Testing Framework (Google Test)
//
// This header file defines the public API for Google Test.  It should be
// included by any test program that uses Google Test.
//
// IMPORTANT NOTE: Due to limitation of the C++ language, we have to
// leave some internal implementation details in this header file.
// They are clearly marked by comments like this:
//
//   // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
//
// Such code is NOT meant to be used by a user directly, and is subject
// to CHANGE WITHOUT NOTICE.  Therefore DO NOT DEPEND ON IT in a user
// program!
//
// Acknowledgment: Google Test borrowed the idea of automatic test
// registration from Barthelemy Dagenais' (barthelemy@prologique.com)
// easyUnit framework.

#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#define GTEST_INCLUDE_GTEST_GTEST_H_

#include <limits>
#include <ostream>
#include <vector>

#include "gtest/internal/gtest-internal.h"
#include "gtest/internal/gtest-string.h"
#include "gtest/gtest-death-test.h"
#include "gtest/gtest-message.h"
#include "gtest/gtest-param-test.h"
#include "gtest/gtest-printers.h"
#include "gtest/gtest_prod.h"
#include "gtest/gtest-test-part.h"
#include "gtest/gtest-typed-test.h"

// Depending on the platform, different string classes are available.
// On Linux, in addition to ::std::string, Google also makes use of
// class ::string, which has the same interface as ::std::string, but
// has a different implementation.
//
// You can define GTEST_HAS_GLOBAL_STRING to 1 to indicate that
// ::string is available AND is a distinct type to ::std::string, or
// define it to 0 to indicate otherwise.
//
// If ::std::string and ::string are the same class on your platform
// due to aliasing, you should define GTEST_HAS_GLOBAL_STRING to 0.
//
// If you do not define GTEST_HAS_GLOBAL_STRING, it is defined
// heuristically.

namespace testing {

// Declares the flags.

// This flag temporary enables the disabled tests.
GTEST_DECLARE_bool_(also_run_disabled_tests);

// This flag brings the debugger on an assertion failure.
GTEST_DECLARE_bool_(break_on_failure);

// This flag controls whether Google Test catches all test-thrown exceptions
// and logs them as failures.
GTEST_DECLARE_bool_(catch_exceptions);

// This flag enables using colors in terminal output. Available values are
// "yes" to enable colors, "no" (disable colors), or "auto" (the default)
// to let Google Test decide.
GTEST_DECLARE_string_(color);

// This flag sets up the filter to select by name using a glob pattern
// the tests to run. If the filter is not given all tests are executed.
GTEST_DECLARE_string_(filter);

// This flag causes the Google Test to list tests. None of the tests listed
// are actually run if the flag is provided.
GTEST_DECLARE_bool_(list_tests);

// This flag controls whether Google Test emits a detailed XML report to a file
// in addition to its normal textual output.
GTEST_DECLARE_string_(output);

// This flags control whether Google Test prints the elapsed time for each
// test.
GTEST_DECLARE_bool_(print_time);

// This flag specifies the random number seed.
GTEST_DECLARE_int32_(random_seed);

// This flag sets how many times the tests are repeated. The default value
// is 1. If the value is -1 the tests are repeating forever.
GTEST_DECLARE_int32_(repeat);

// This flag controls whether Google Test includes Google Test internal
// stack frames in failure stack traces.
GTEST_DECLARE_bool_(show_internal_stack_frames);

// When this flag is specified, tests' order is randomized on every iteration.
GTEST_DECLARE_bool_(shuffle);

// This flag specifies the maximum number of stack frames to be
// printed in a failure message.
GTEST_DECLARE_int32_(stack_trace_depth);

// When this flag is specified, a failed assertion will throw an
// exception if exceptions are enabled, or exit the program with a
// non-zero code otherwise.
GTEST_DECLARE_bool_(throw_on_failure);

// When this flag is set with a "host:port" string, on supported
// platforms test results are streamed to the specified port on
// the specified host machine.
GTEST_DECLARE_string_(stream_result_to);

// The upper limit for valid stack trace depths.
const int kMaxStackTraceDepth = 100;

namespace internal {

class AssertHelper;
class DefaultGlobalTestPartResultReporter;
class ExecDeathTest;
class NoExecDeathTest;
class FinalSuccessChecker;
class GTestFlagSaver;
class StreamingListenerTest;
class TestResultAccessor;
class TestEventListenersAccessor;
class TestEventRepeater;
class UnitTestRecordPropertyTestHelper;
class WindowsDeathTest;
class UnitTestImpl* GetUnitTestImpl();
void ReportFailureInUnknownLocation(TestPartResult::Type result_type,
                                    const std::string& message);

}  // namespace internal

// The friend relationship of some of these classes is cyclic.
// If we don't forward declare them the compiler might confuse the classes
// in friendship clauses with same named classes on the scope.
class Test;
class TestCase;
class TestInfo;
class UnitTest;

// A class for indicating whether an assertion was successful.  When
// the assertion wasn't successful, the AssertionResult object
// remembers a non-empty message that describes how it failed.
//
// To create an instance of this class, use one of the factory functions
// (AssertionSuccess() and AssertionFailure()).
//
// This class is useful for two purposes:
//   1. Defining predicate functions to be used with Boolean test assertions
//      EXPECT_TRUE/EXPECT_FALSE and their ASSERT_ counterparts
//   2. Defining predicate-format functions to be
//      used with predicate assertions (ASSERT_PRED_FORMAT*, etc).
//
// For example, if you define IsEven predicate:
//
//   testing::AssertionResult IsEven(int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess();
//     else
//       return testing::AssertionFailure() << n << " is odd";
//   }
//
// Then the failed expectation EXPECT_TRUE(IsEven(Fib(5)))
// will print the message
//
//   Value of: IsEven(Fib(5))
//     Actual: false (5 is odd)
//   Expected: true
//
// instead of a more opaque
//
//   Value of: IsEven(Fib(5))
//     Actual: false
//   Expected: true
//
// in case IsEven is a simple Boolean predicate.
//
// If you expect your predicate to be reused and want to support informative
// messages in EXPECT_FALSE and ASSERT_FALSE (negative assertions show up
// about half as often as positive ones in our tests), supply messages for
// both success and failure cases:
//
//   testing::AssertionResult IsEven(int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess() << n << " is even";
//     else
//       return testing::AssertionFailure() << n << " is odd";
//   }
//
// Then a statement EXPECT_FALSE(IsEven(Fib(6))) will print
//
//   Value of: IsEven(Fib(6))
//     Actual: true (8 is even)
//   Expected: false
//
// NB: Predicates that support negative Boolean assertions have reduced
// performance in positive ones so be careful not to use them in tests
// that have lots (tens of thousands) of positive Boolean assertions.
//
// To use this class with EXPECT_PRED_FORMAT assertions such as:
//
//   // Verifies that Foo() returns an even number.
//   EXPECT_PRED_FORMAT1(IsEven, Foo());
//
// you need to define:
//
//   testing::AssertionResult IsEven(const char* expr, int n) {
//     if ((n % 2) == 0)
//       return testing::AssertionSuccess();
//     else
//       return testing::AssertionFailure()
//         << "Expected: " << expr << " is even\n  Actual: it's " << n;
//   }
//
// If Foo() returns 5, you will see the following message:
//
//   Expected: Foo() is even
//     Actual: it's 5
//
class GTEST_API_ AssertionResult {
 public:
  // Copy constructor.
  // Used in EXPECT_TRUE/FALSE(assertion_result).
  AssertionResult(const AssertionResult& other);

  GTEST_DISABLE_MSC_WARNINGS_PUSH_(4800 /* forcing value to bool */)

  // Used in the EXPECT_TRUE/FALSE(bool_expression).
  //
  // T must be contextually convertible to bool.
  //
  // The second parameter prevents this overload from being considered if
  // the argument is implicitly convertible to AssertionResult. In that case
  // we want AssertionResult's copy constructor to be used.
  template <typename T>
  explicit AssertionResult(
      const T& success,
      typename internal::EnableIf<
          !internal::ImplicitlyConvertible<T, AssertionResult>::value>::type*
          /*enabler*/ = NULL)
      : success_(success) {}

  GTEST_DISABLE_MSC_WARNINGS_POP_()

  // Assignment operator.
  AssertionResult& operator=(AssertionResult other) {
    swap(other);
    return *this;
  }

  // Returns true iff the assertion succeeded.
  operator bool() const { return success_; }  // NOLINT

  // Returns the assertion's negation. Used with EXPECT/ASSERT_FALSE.
  AssertionResult operator!() const;

  // Returns the text streamed into this AssertionResult. Test assertions
  // use it when they fail (i.e., the predicate's outcome doesn't match the
  // assertion's expectation). When nothing has been streamed into the
  // object, returns an empty string.
  const char* message() const {
    return message_.get() != NULL ?  message_->c_str() : "";
  }
  // TODO(vladl@google.com): Remove this after making sure no clients use it.
  // Deprecated; please use message() instead.
  const char* failure_message() const { return message(); }

  // Streams a custom failure message into this object.
  template <typename T> AssertionResult& operator<<(const T& value) {
    AppendMessage(Message() << value);
    return *this;
  }

  // Allows streaming basic output manipulators such as endl or flush into
  // this object.
  AssertionResult& operator<<(
      ::std::ostream& (*basic_manipulator)(::std::ostream& stream)) {
    AppendMessage(Message() << basic_manipulator);
    return *this;
  }

 private:
  // Appends the contents of message to message_.
  void AppendMessage(const Message& a_message) {
    if (message_.get() == NULL)
      message_.reset(new ::std::string);
    message_->append(a_message.GetString().c_str());
  }

  // Swap the contents of this AssertionResult with other.
  void swap(AssertionResult& other);

  // Stores result of the assertion predicate.
  bool success_;
  // Stores the message describing the condition in case the expectation
  // construct is not satisfied with the predicate's outcome.
  // Referenced via a pointer to avoid taking too much stack frame space
  // with test assertions.
  internal::scoped_ptr< ::std::string> message_;
};

// Makes a successful assertion result.
GTEST_API_ AssertionResult AssertionSuccess();

// Makes a failed assertion result.
GTEST_API_ AssertionResult AssertionFailure();

// Makes a failed assertion result with the given failure message.
// Deprecated; use AssertionFailure() << msg.
GTEST_API_ AssertionResult AssertionFailure(const Message& msg);

// The abstract class that all tests inherit from.
//
// In Google Test, a unit test program contains one or many TestCases, and
// each TestCase contains one or many Tests.
//
// When you define a test using the TEST macro, you don't need to
// explicitly derive from Test - the TEST macro automatically does
// this for you.
//
// The only time you derive from Test is when defining a test fixture
// to be used a TEST_F.  For example:
//
//   class FooTest : public testing::Test {
//    protected:
//     void SetUp() override { ... }
//     void TearDown() override { ... }
//     ...
//   };
//
//   TEST_F(FooTest, Bar) { ... }
//   TEST_F(FooTest, Baz) { ... }
//
// Test is not copyable.
class GTEST_API_ Test {
 public:
  friend class TestInfo;

  // Defines types for pointers to functions that set up and tear down
  // a test case.
  typedef internal::SetUpTestCaseFunc SetUpTestCaseFunc;
  typedef internal::TearDownTestCaseFunc TearDownTestCaseFunc;

  // The d'tor is virtual as we intend to inherit from Test.
  virtual ~Test();

  // Sets up the stuff shared by all tests in this test case.
  //
  // Google Test will call Foo::SetUpTestCase() before running the first
  // test in test case Foo.  Hence a sub-class can define its own
  // SetUpTestCase() method to shadow the one defined in the super
  // class.
  static void SetUpTestCase() {}

  // Tears down the stuff shared by all tests in this test case.
  //
  // Google Test will call Foo::TearDownTestCase() after running the last
  // test in test case Foo.  Hence a sub-class can define its own
  // TearDownTestCase() method to shadow the one defined in the super
  // class.
  static void TearDownTestCase() {}

  // Returns true iff the current test has a fatal failure.
  static bool HasFatalFailure();

  // Returns true iff the current test has a non-fatal failure.
  static bool HasNonfatalFailure();

  // Returns true iff the current test has a (either fatal or
  // non-fatal) failure.
  static bool HasFailure() { return HasFatalFailure() || HasNonfatalFailure(); }

  // Logs a property for the current test, test case, or for the entire
  // invocation of the test program when used outside of the context of a
  // test case.  Only the last value for a given key is remembered.  These
  // are public static so they can be called from utility functions that are
  // not members of the test fixture.  Calls to RecordProperty made during
  // lifespan of the test (from the moment its constructor starts to the
  // moment its destructor finishes) will be output in XML as attributes of
  // the <testcase> element.  Properties recorded from fixture's
  // SetUpTestCase or TearDownTestCase are logged as attributes of the
  // corresponding <testsuite> element.  Calls to RecordProperty made in the
  // global context (before or after invocation of RUN_ALL_TESTS and from
  // SetUp/TearDown method of Environment objects registered with Google
  // Test) will be output as attributes of the <testsuites> element.
  static void RecordProperty(const std::string& key, const std::string& value);
  static void RecordProperty(const std::string& key, int value);

 protected:
  // Creates a Test object.
  Test();

  // Sets up the test fixture.
  virtual void SetUp();

  // Tears down the test fixture.
  virtual void TearDown();

 private:
  // Returns true iff the current test has the same fixture class as
  // the first test in the current test case.
  static bool HasSameFixtureClass();

  // Runs the test after the test fixture has been set up.
  //
  // A sub-class must implement this to define the test logic.
  //
  // DO NOT OVERRIDE THIS FUNCTION DIRECTLY IN A USER PROGRAM.
  // Instead, use the TEST or TEST_F macro.
  virtual void TestBody() = 0;

  // Sets up, executes, and tears down the test.
  void Run();

  // Deletes self.  We deliberately pick an unusual name for this
  // internal method to avoid clashing with names used in user TESTs.
  void DeleteSelf_() { delete this; }

  const internal::scoped_ptr< GTEST_FLAG_SAVER_ > gtest_flag_saver_;

  // Often a user misspells SetUp() as Setup() and spends a long time
  // wondering why it is never called by Google Test.  The declaration of
  // the following method is solely for catching such an error at
  // compile time:
  //
  //   - The return type is deliberately chosen to be not void, so it
  //   will be a conflict if void Setup() is declared in the user's
  //   test fixture.
  //
  //   - This method is private, so it will be another compiler error
  //   if the method is called from the user's test fixture.
  //
  // DO NOT OVERRIDE THIS FUNCTION.
  //
  // If you see an error about overriding the following function or
  // about it being private, you have mis-spelled SetUp() as Setup().
  struct Setup_should_be_spelled_SetUp {};
  virtual Setup_should_be_spelled_SetUp* Setup() { return NULL; }

  // We disallow copying Tests.
  GTEST_DISALLOW_COPY_AND_ASSIGN_(Test);
};

typedef internal::TimeInMillis TimeInMillis;

// A copyable object representing a user specified test property which can be
// output as a key/value string pair.
//
// Don't inherit from TestProperty as its destructor is not virtual.
class TestProperty {
 public:
  // C'tor.  TestProperty does NOT have a default constructor.
  // Always use this constructor (with parameters) to create a
  // TestProperty object.
  TestProperty(const std::string& a_key, const std::string& a_value) :
    key_(a_key), value_(a_value) {
  }

  // Gets the user supplied key.
  const char* key() const {
    return key_.c_str();
  }

  // Gets the user supplied value.
  const char* value() const {
    return value_.c_str();
  }

  // Sets a new value, overriding the one supplied in the constructor.
  void SetValue(const std::string& new_value) {
    value_ = new_value;
  }

 private:
  // The key supplied by the user.
  std::string key_;
  // The value supplied by the user.
  std::string value_;
};

// The result of a single Test.  This includes a list of
// TestPartResults, a list of TestProperties, a count of how many
// death tests there are in the Test, and how much time it took to run
// the Test.
//
// TestResult is not copyable.
class GTEST_API_ TestResult {
 public:
  // Creates an empty TestResult.
  TestResult();

  // D'tor.  Do not inherit from TestResult.
  ~TestResult();

  // Gets the number of all test parts.  This is the sum of the number
  // of successful test parts and the number of failed test parts.
  int total_part_count() const;

  // Returns the number of the test properties.
  int test_property_count() const;

  // Returns true iff the test passed (i.e. no test part failed).
  bool Passed() const { return !Failed(); }

  // Returns true iff the test failed.
  bool Failed() const;

  // Returns true iff the test fatally failed.
  bool HasFatalFailure() const;

  // Returns true iff the test has a non-fatal failure.
  bool HasNonfatalFailure() const;

  // Returns the elapsed time, in milliseconds.
  TimeInMillis elapsed_time() const { return elapsed_time_; }

  // Returns the i-th test part result among all the results. i can range
  // from 0 to test_property_count() - 1. If i is not in that range, aborts
  // the program.
  const TestPartResult& GetTestPartResult(int i) const;

  // Returns the i-th test property. i can range from 0 to
  // test_property_count() - 1. If i is not in that range, aborts the
  // program.
  const TestProperty& GetTestProperty(int i) const;

 private:
  friend class TestInfo;
  friend class TestCase;
  friend class UnitTest;
  friend class internal::DefaultGlobalTestPartResultReporter;
  friend class internal::ExecDeathTest;
  friend class internal::TestResultAccessor;
  friend class internal::UnitTestImpl;
  friend class internal::WindowsDeathTest;

  // Gets the vector of TestPartResults.
  const std::vector<TestPartResult>& test_part_results() const {
    return test_part_results_;
  }

  // Gets the vector of TestProperties.
  const std::vector<TestProperty>& test_properties() const {
    return test_properties_;
  }

  // Sets the elapsed time.
  void set_elapsed_time(TimeInMillis elapsed) { elapsed_time_ = elapsed; }

  // Adds a test property to the list. The property is validated and may add
  // a non-fatal failure if invalid (e.g., if it conflicts with reserved
  // key names). If a property is already recorded for the same key, the
  // value will be updated, rather than storing multiple values for the same
  // key.  xml_element specifies the element for which the property is being
  // recorded and is used for validation.
  void RecordProperty(const std::string& xml_element,
                      const TestProperty& test_property);

  // Adds a failure if the key is a reserved attribute of Google Test
  // testcase tags.  Returns true if the property is valid.
  // TODO(russr): Validate attribute names are legal and human readable.
  static bool ValidateTestProperty(const std::string& xml_element,
                                   const TestProperty& test_property);

  // Adds a test part result to the list.
  void AddTestPartResult(const TestPartResult& test_part_result);

  // Returns the death test count.
  int death_test_count() const { return death_test_count_; }

  // Increments the death test count, returning the new count.
  int increment_death_test_count() { return ++death_test_count_; }

  // Clears the test part results.
  void ClearTestPartResults();

  // Clears the object.
  void Clear();

  // Protects mutable state of the property vector and of owned
  // properties, whose values may be updated.
  internal::Mutex test_properites_mutex_;

  // The vector of TestPartResults
  std::vector<TestPartResult> test_part_results_;
  // The vector of TestProperties
  std::vector<TestProperty> test_properties_;
  // Running count of death tests.
  int death_test_count_;
  // The elapsed time, in milliseconds.
  TimeInMillis elapsed_time_;

  // We disallow copying TestResult.
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestResult);
};  // class TestResult

// A TestInfo object stores the following information about a test:
//
//   Test case name
//   Test name
//   Whether the test should be run
//   A function pointer that creates the test object when invoked
//   Test result
//
// The constructor of TestInfo registers itself with the UnitTest
// singleton such that the RUN_ALL_TESTS() macro knows which tests to
// run.
class GTEST_API_ TestInfo {
 public:
  // Destructs a TestInfo object.  This function is not virtual, so
  // don't inherit from TestInfo.
  ~TestInfo();

  // Returns the test case name.
  const char* test_case_name() const { return test_case_name_.c_str(); }

  // Returns the test name.
  const char* name() const { return name_.c_str(); }

  // Returns the name of the parameter type, or NULL if this is not a typed
  // or a type-parameterized test.
  const char* type_param() const {
    if (type_param_.get() != NULL)
      return type_param_->c_str();
    return NULL;
  }

  // Returns the text representation of the value parameter, or NULL if this
  // is not a value-parameterized test.
  const char* value_param() const {
    if (value_param_.get() != NULL)
      return value_param_->c_str();
    return NULL;
  }

  // Returns the file name where this test is defined.
  const char* file() const { return location_.file.c_str(); }

  // Returns the line where this test is defined.
  int line() const { return location_.line; }

  // Returns true if this test should run, that is if the test is not
  // disabled (or it is disabled but the also_run_disabled_tests flag has
  // been specified) and its full name matches the user-specified filter.
  //
  // Google Test allows the user to filter the tests by their full names.
  // The full name of a test Bar in test case Foo is defined as
  // "Foo.Bar".  Only the tests that match the filter will run.
  //
  // A filter is a colon-separated list of glob (not regex) patterns,
  // optionally followed by a '-' and a colon-separated list of
  // negative patterns (tests to exclude).  A test is run if it
  // matches one of the positive patterns and does not match any of
  // the negative patterns.
  //
  // For example, *A*:Foo.* is a filter that matches any string that
  // contains the character 'A' or starts with "Foo.".
  bool should_run() const { return should_run_; }

  // Returns true iff this test will appear in the XML report.
  bool is_reportable() const {
    // For now, the XML report includes all tests matching the filter.
    // In the future, we may trim tests that are excluded because of
    // sharding.
    return matches_filter_;
  }

  // Returns the result of the test.
  const TestResult* result() const { return &result_; }

 private:
#if GTEST_HAS_DEATH_TEST
  friend class internal::DefaultDeathTestFactory;
#endif  // GTEST_HAS_DEATH_TEST
  friend class Test;
  friend class TestCase;
  friend class internal::UnitTestImpl;
  friend class internal::StreamingListenerTest;
  friend TestInfo* internal::MakeAndRegisterTestInfo(
      const char* test_case_name,
      const char* name,
      const char* type_param,
      const char* value_param,
      internal::CodeLocation code_location,
      internal::TypeId fixture_class_id,
      Test::SetUpTestCaseFunc set_up_tc,
      Test::TearDownTestCaseFunc tear_down_tc,
      internal::TestFactoryBase* factory);

  // Constructs a TestInfo object. The newly constructed instance assumes
  // ownership of the factory object.
  TestInfo(const std::string& test_case_name,
           const std::string& name,
           const char* a_type_param,   // NULL if not a type-parameterized test
           const char* a_value_param,  // NULL if not a value-parameterized test
           internal::CodeLocation a_code_location,
           internal::TypeId fixture_class_id,
           internal::TestFactoryBase* factory);

  // Increments the number of death tests encountered in this test so
  // far.
  int increment_death_test_count() {
    return result_.increment_death_test_count();
  }

  // Creates the test object, runs it, records its result, and then
  // deletes it.
  void Run();

  static void ClearTestResult(TestInfo* test_info) {
    test_info->result_.Clear();
  }

  // These fields are immutable properties of the test.
  const std::string test_case_name_;     // Test case name
  const std::string name_;               // Test name
  // Name of the parameter type, or NULL if this is not a typed or a
  // type-parameterized test.
  const internal::scoped_ptr<const ::std::string> type_param_;
  // Text representation of the value parameter, or NULL if this is not a
  // value-parameterized test.
  const internal::scoped_ptr<const ::std::string> value_param_;
  internal::CodeLocation location_;
  const internal::TypeId fixture_class_id_;   // ID of the test fixture class
  bool should_run_;                 // True iff this test should run
  bool is_disabled_;                // True iff this test is disabled
  bool matches_filter_;             // True if this test matches the
                                    // user-specified filter.
  internal::TestFactoryBase* const factory_;  // The factory that creates
                                              // the test object

  // This field is mutable and needs to be reset before running the
  // test for the second time.
  TestResult result_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestInfo);
};

// A test case, which consists of a vector of TestInfos.
//
// TestCase is not copyable.
class GTEST_API_ TestCase {
 public:
  // Creates a TestCase with the given name.
  //
  // TestCase does NOT have a default constructor.  Always use this
  // constructor to create a TestCase object.
  //
  // Arguments:
  //
  //   name:         name of the test case
  //   a_type_param: the name of the test's type parameter, or NULL if
  //                 this is not a type-parameterized test.
  //   set_up_tc:    pointer to the function that sets up the test case
  //   tear_down_tc: pointer to the function that tears down the test case
  TestCase(const char* name, const char* a_type_param,
           Test::SetUpTestCaseFunc set_up_tc,
           Test::TearDownTestCaseFunc tear_down_tc);

  // Destructor of TestCase.
  virtual ~TestCase();

  // Gets the name of the TestCase.
  const char* name() const { return name_.c_str(); }

  // Returns the name of the parameter type, or NULL if this is not a
  // type-parameterized test case.
  const char* type_param() const {
    if (type_param_.get() != NULL)
      return type_param_->c_str();
    return NULL;
  }

  // Returns true if any test in this test case should run.
  bool should_run() const { return should_run_; }

  // Gets the number of successful tests in this test case.
  int successful_test_count() const;

  // Gets the number of failed tests in this test case.
  int failed_test_count() const;

  // Gets the number of disabled tests that will be reported in the XML report.
  int reportable_disabled_test_count() const;

  // Gets the number of disabled tests in this test case.
  int disabled_test_count() const;

  // Gets the number of tests to be printed in the XML report.
  int reportable_test_count() const;

  // Get the number of tests in this test case that should run.
  int test_to_run_count() const;

  // Gets the number of all tests in this test case.
  int total_test_count() const;

  // Returns true iff the test case passed.
  bool Passed() const { return !Failed(); }

  // Returns true iff the test case failed.
  bool Failed() const { return failed_test_count() > 0; }

  // Returns the elapsed time, in milliseconds.
  TimeInMillis elapsed_time() const { return elapsed_time_; }

  // Returns the i-th test among all the tests. i can range from 0 to
  // total_test_count() - 1. If i is not in that range, returns NULL.
  const TestInfo* GetTestInfo(int i) const;

  // Returns the TestResult that holds test properties recorded during
  // execution of SetUpTestCase and TearDownTestCase.
  const TestResult& ad_hoc_test_result() const { return ad_hoc_test_result_; }

 private:
  friend class Test;
  friend class internal::UnitTestImpl;

  // Gets the (mutable) vector of TestInfos in this TestCase.
  std::vector<TestInfo*>& test_info_list() { return test_info_list_; }

  // Gets the (immutable) vector of TestInfos in this TestCase.
  const std::vector<TestInfo*>& test_info_list() const {
    return test_info_list_;
  }

  // Returns the i-th test among all the tests. i can range from 0 to
  // total_test_count() - 1. If i is not in that range, returns NULL.
  TestInfo* GetMutableTestInfo(int i);

  // Sets the should_run member.
  void set_should_run(bool should) { should_run_ = should; }

  // Adds a TestInfo to this test case.  Will delete the TestInfo upon
  // destruction of the TestCase object.
  void AddTestInfo(TestInfo * test_info);

  // Clears the results of all tests in this test case.
  void ClearResult();

  // Clears the results of all tests in the given test case.
  static void ClearTestCaseResult(TestCase* test_case) {
    test_case->ClearResult();
  }

  // Runs every test in this TestCase.
  void Run();

  // Runs SetUpTestCase() for this TestCase.  This wrapper is needed
  // for catching exceptions thrown from SetUpTestCase().
  void RunSetUpTestCase() { (*set_up_tc_)(); }

  // Runs TearDownTestCase() for this TestCase.  This wrapper is
  // needed for catching exceptions thrown from TearDownTestCase().
  void RunTearDownTestCase() { (*tear_down_tc_)(); }

  // Returns true iff test passed.
  static bool TestPassed(const TestInfo* test_info) {
    return test_info->should_run() && test_info->result()->Passed();
  }

  // Returns true iff test failed.
  static bool TestFailed(const TestInfo* test_info) {
    return test_info->should_run() && test_info->result()->Failed();
  }

  // Returns true iff the test is disabled and will be reported in the XML
  // report.
  static bool TestReportableDisabled(const TestInfo* test_info) {
    return test_info->is_reportable() && test_info->is_disabled_;
  }

  // Returns true iff test is disabled.
  static bool TestDisabled(const TestInfo* test_info) {
    return test_info->is_disabled_;
  }

  // Returns true iff this test will appear in the XML report.
  static bool TestReportable(const TestInfo* test_info) {
    return test_info->is_reportable();
  }

  // Returns true if the given test should run.
  static bool ShouldRunTest(const TestInfo* test_info) {
    return test_info->should_run();
  }

  // Shuffles the tests in this test case.
  void ShuffleTests(internal::Random* random);

  // Restores the test order to before the first shuffle.
  void UnshuffleTests();

  // Name of the test case.
  std::string name_;
  // Name of the parameter type, or NULL if this is not a typed or a
  // type-parameterized test.
  const internal::scoped_ptr<const ::std::string> type_param_;
  // The vector of TestInfos in their original order.  It owns the
  // elements in the vector.
  std::vector<TestInfo*> test_info_list_;
  // Provides a level of indirection for the test list to allow easy
  // shuffling and restoring the test order.  The i-th element in this
  // vector is the index of the i-th test in the shuffled test list.
  std::vector<int> test_indices_;
  // Pointer to the function that sets up the test case.
  Test::SetUpTestCaseFunc set_up_tc_;
  // Pointer to the function that tears down the test case.
  Test::TearDownTestCaseFunc tear_down_tc_;
  // True iff any test in this test case should run.
  bool should_run_;
  // Elapsed time, in milliseconds.
  TimeInMillis elapsed_time_;
  // Holds test properties recorded during execution of SetUpTestCase and
  // TearDownTestCase.
  TestResult ad_hoc_test_result_;

  // We disallow copying TestCases.
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestCase);
};

// An Environment object is capable of setting up and tearing down an
// environment.  You should subclass this to define your own
// environment(s).
//
// An Environment object does the set-up and tear-down in virtual
// methods SetUp() and TearDown() instead of the constructor and the
// destructor, as:
//
//   1. You cannot safely throw from a destructor.  This is a problem
//      as in some cases Google Test is used where exceptions are enabled, and
//      we may want to implement ASSERT_* using exceptions where they are
//      available.
//   2. You cannot use ASSERT_* directly in a constructor or
//      destructor.
class Environment {
 public:
  // The d'tor is virtual as we need to subclass Environment.
  virtual ~Environment() {}

  // Override this to define how to set up the environment.
  virtual void SetUp() {}

  // Override this to define how to tear down the environment.
  virtual void TearDown() {}
 private:
  // If you see an error about overriding the following function or
  // about it being private, you have mis-spelled SetUp() as Setup().
  struct Setup_should_be_spelled_SetUp {};
  virtual Setup_should_be_spelled_SetUp* Setup() { return NULL; }
};

// The interface for tracing execution of tests. The methods are organized in
// the order the corresponding events are fired.
class TestEventListener {
 public:
  virtual ~TestEventListener() {}

  // Fired before any test activity starts.
  virtual void OnTestProgramStart(const UnitTest& unit_test) = 0;

  // Fired before each iteration of tests starts.  There may be more than
  // one iteration if GTEST_FLAG(repeat) is set. iteration is the iteration
  // index, starting from 0.
  virtual void OnTestIterationStart(const UnitTest& unit_test,
                                    int iteration) = 0;

  // Fired before environment set-up for each iteration of tests starts.
  virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test) = 0;

  // Fired after environment set-up for each iteration of tests ends.
  virtual void OnEnvironmentsSetUpEnd(const UnitTest& unit_test) = 0;

  // Fired before the test case starts.
  virtual void OnTestCaseStart(const TestCase& test_case) = 0;

  // Fired before the test starts.
  virtual void OnTestStart(const TestInfo& test_info) = 0;

  // Fired after a failed assertion or a SUCCEED() invocation.
  virtual void OnTestPartResult(const TestPartResult& test_part_result) = 0;

  // Fired after the test ends.
  virtual void OnTestEnd(const TestInfo& test_info) = 0;

  // Fired after the test case ends.
  virtual void OnTestCaseEnd(const TestCase& test_case) = 0;

  // Fired before environment tear-down for each iteration of tests starts.
  virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test) = 0;

  // Fired after environment tear-down for each iteration of tests ends.
  virtual void OnEnvironmentsTearDownEnd(const UnitTest& unit_test) = 0;

  // Fired after each iteration of tests finishes.
  virtual void OnTestIterationEnd(const UnitTest& unit_test,
                                  int iteration) = 0;

  // Fired after all test activities have ended.
  virtual void OnTestProgramEnd(const UnitTest& unit_test) = 0;
};

// The convenience class for users who need to override just one or two
// methods and are not concerned that a possible change to a signature of
// the methods they override will not be caught during the build.  For
// comments about each method please see the definition of TestEventListener
// above.
class EmptyTestEventListener : public TestEventListener {
 public:
  virtual void OnTestProgramStart(const UnitTest& /*unit_test*/) {}
  virtual void OnTestIterationStart(const UnitTest& /*unit_test*/,
                                    int /*iteration*/) {}
  virtual void OnEnvironmentsSetUpStart(const UnitTest& /*unit_test*/) {}
  virtual void OnEnvironmentsSetUpEnd(const UnitTest& /*unit_test*/) {}
  virtual void OnTestCaseStart(const TestCase& /*test_case*/) {}
  virtual void OnTestStart(const TestInfo& /*test_info*/) {}
  virtual void OnTestPartResult(const TestPartResult& /*test_part_result*/) {}
  virtual void OnTestEnd(const TestInfo& /*test_info*/) {}
  virtual void OnTestCaseEnd(const TestCase& /*test_case*/) {}
  virtual void OnEnvironmentsTearDownStart(const UnitTest& /*unit_test*/) {}
  virtual void OnEnvironmentsTearDownEnd(const UnitTest& /*unit_test*/) {}
  virtual void OnTestIterationEnd(const UnitTest& /*unit_test*/,
                                  int /*iteration*/) {}
  virtual void OnTestProgramEnd(const UnitTest& /*unit_test*/) {}
};

// TestEventListeners lets users add listeners to track events in Google Test.
class GTEST_API_ TestEventListeners {
 public:
  TestEventListeners();
  ~TestEventListeners();

  // Appends an event listener to the end of the list. Google Test assumes
  // the ownership of the listener (i.e. it will delete the listener when
  // the test program finishes).
  void Append(TestEventListener* listener);

  // Removes the given event listener from the list and returns it.  It then
  // becomes the caller's responsibility to delete the listener. Returns
  // NULL if the listener is not found in the list.
  TestEventListener* Release(TestEventListener* listener);

  // Returns the standard listener responsible for the default console
  // output.  Can be removed from the listeners list to shut down default
  // console output.  Note that removing this object from the listener list
  // with Release transfers its ownership to the caller and makes this
  // function return NULL the next time.
  TestEventListener* default_result_printer() const {
    return default_result_printer_;
  }

  // Returns the standard listener responsible for the default XML output
  // controlled by the --gtest_output=xml flag.  Can be removed from the
  // listeners list by users who want to shut down the default XML output
  // controlled by this flag and substitute it with custom one.  Note that
  // removing this object from the listener list with Release transfers its
  // ownership to the caller and makes this function return NULL the next
  // time.
  TestEventListener* default_xml_generator() const {
    return default_xml_generator_;
  }

 private:
  friend class TestCase;
  friend class TestInfo;
  friend class internal::DefaultGlobalTestPartResultReporter;
  friend class internal::NoExecDeathTest;
  friend class internal::TestEventListenersAccessor;
  friend class internal::UnitTestImpl;

  // Returns repeater that broadcasts the TestEventListener events to all
  // subscribers.
  TestEventListener* repeater();

  // Sets the default_result_printer attribute to the provided listener.
  // The listener is also added to the listener list and previous
  // default_result_printer is removed from it and deleted. The listener can
  // also be NULL in which case it will not be added to the list. Does
  // nothing if the previous and the current listener objects are the same.
  void SetDefaultResultPrinter(TestEventListener* listener);

  // Sets the default_xml_generator attribute to the provided listener.  The
  // listener is also added to the listener list and previous
  // default_xml_generator is removed from it and deleted. The listener can
  // also be NULL in which case it will not be added to the list. Does
  // nothing if the previous and the current listener objects are the same.
  void SetDefaultXmlGenerator(TestEventListener* listener);

  // Controls whether events will be forwarded by the repeater to the
  // listeners in the list.
  bool EventForwardingEnabled() const;
  void SuppressEventForwarding();

  // The actual list of listeners.
  internal::TestEventRepeater* repeater_;
  // Listener responsible for the standard result output.
  TestEventListener* default_result_printer_;
  // Listener responsible for the creation of the XML output file.
  TestEventListener* default_xml_generator_;

  // We disallow copying TestEventListeners.
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestEventListeners);
};

// A UnitTest consists of a vector of TestCases.
//
// This is a singleton class.  The only instance of UnitTest is
// created when UnitTest::GetInstance() is first called.  This
// instance is never deleted.
//
// UnitTest is not copyable.
//
// This class is thread-safe as long as the methods are called
// according to their specification.
class GTEST_API_ UnitTest {
 public:
  // Gets the singleton UnitTest object.  The first time this method
  // is called, a UnitTest object is constructed and returned.
  // Consecutive calls will return the same object.
  static UnitTest* GetInstance();

  // Runs all tests in this UnitTest object and prints the result.
  // Returns 0 if successful, or 1 otherwise.
  //
  // This method can only be called from the main thread.
  //
  // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
  int Run() GTEST_MUST_USE_RESULT_;

  // Returns the working directory when the first TEST() or TEST_F()
  // was executed.  The UnitTest object owns the string.
  const char* original_working_dir() const;

  // Returns the TestCase object for the test that's currently running,
  // or NULL if no test is running.
  const TestCase* current_test_case() const
      GTEST_LOCK_EXCLUDED_(mutex_);

  // Returns the TestInfo object for the test that's currently running,
  // or NULL if no test is running.
  const TestInfo* current_test_info() const
      GTEST_LOCK_EXCLUDED_(mutex_);

  // Returns the random seed used at the start of the current test run.
  int random_seed() const;

#if GTEST_HAS_PARAM_TEST
  // Returns the ParameterizedTestCaseRegistry object used to keep track of
  // value-parameterized tests and instantiate and register them.
  //
  // INTERNAL IMPLEMENTATION - DO NOT USE IN A USER PROGRAM.
  internal::ParameterizedTestCaseRegistry& parameterized_test_registry()
      GTEST_LOCK_EXCLUDED_(mutex_);
#endif  // GTEST_HAS_PARAM_TEST

  // Gets the number of successful test cases.
  int successful_test_case_count() const;

  // Gets the number of failed test cases.
  int failed_test_case_count() const;

  // Gets the number of all test cases.
  int total_test_case_count() const;

  // Gets the number of all test cases that contain at least one test
  // that should run.
  int test_case_to_run_count() const;

  // Gets the number of successful tests.
  int successful_test_count() const;

  // Gets the number of failed tests.
  int failed_test_count() const;

  // Gets the number of disabled tests that will be reported in the XML report.
  int reportable_disabled_test_count() const;

  // Gets the number of disabled tests.
  int disabled_test_count() const;

  // Gets the number of tests to be printed in the XML report.
  int reportable_test_count() const;

  // Gets the number of all tests.
  int total_test_count() const;

  // Gets the number of tests that should run.
  int test_to_run_count() const;

  // Gets the time of the test program start, in ms from the start of the
  // UNIX epoch.
  TimeInMillis start_timestamp() const;

  // Gets the elapsed time, in milliseconds.
  TimeInMillis elapsed_time() const;

  // Returns true iff the unit test passed (i.e. all test cases passed).
  bool Passed() const;

  // Returns true iff the unit test failed (i.e. some test case failed
  // or something outside of all tests failed).
  bool Failed() const;

  // Gets the i-th test case among all the test cases. i can range from 0 to
  // total_test_case_count() - 1. If i is not in that range, returns NULL.
  const TestCase* GetTestCase(int i) const;

  // Returns the TestResult containing information on test failures and
  // properties logged outside of individual test cases.
  const TestResult& ad_hoc_test_result() const;

  // Returns the list of event listeners that can be used to track events
  // inside Google Test.
  TestEventListeners& listeners();

 private:
  // Registers and returns a global test environment.  When a test
  // program is run, all global test environments will be set-up in
  // the order they were registered.  After all tests in the program
  // have finished, all global test environments will be torn-down in
  // the *reverse* order they were registered.
  //
  // The UnitTest object takes ownership of the given environment.
  //
  // This method can only be called from the main thread.
  Environment* AddEnvironment(Environment* env);

  // Adds a TestPartResult to the current TestResult object.  All
  // Google Test assertion macros (e.g. ASSERT_TRUE, EXPECT_EQ, etc)
  // eventually call this to report their results.  The user code
  // should use the assertion macros instead of calling this directly.
  void AddTestPartResult(TestPartResult::Type result_type,
                         const char* file_name,
                         int line_number,
                         const std::string& message,
                         const std::string& os_stack_trace)
      GTEST_LOCK_EXCLUDED_(mutex_);

  // Adds a TestProperty to the current TestResult object when invoked from
  // inside a test, to current TestCase's ad_hoc_test_result_ when invoked
  // from SetUpTestCase or TearDownTestCase, or to the global property set
  // when invoked elsewhere.  If the result already contains a property with
  // the same key, the value will be updated.
  void RecordProperty(const std::string& key, const std::string& value);

  // Gets the i-th test case among all the test cases. i can range from 0 to
  // total_test_case_count() - 1. If i is not in that range, returns NULL.
  TestCase* GetMutableTestCase(int i);

  // Accessors for the implementation object.
  internal::UnitTestImpl* impl() { return impl_; }
  const internal::UnitTestImpl* impl() const { return impl_; }

  // These classes and funcions are friends as they need to access private
  // members of UnitTest.
  friend class Test;
  friend class internal::AssertHelper;
  friend class internal::ScopedTrace;
  friend class internal::StreamingListenerTest;
  friend class internal::UnitTestRecordPropertyTestHelper;
  friend Environment* AddGlobalTestEnvironment(Environment* env);
  friend internal::UnitTestImpl* internal::GetUnitTestImpl();
  friend void internal::ReportFailureInUnknownLocation(
      TestPartResult::Type result_type,
      const std::string& message);

  // Creates an empty UnitTest.
  UnitTest();

  // D'tor
  virtual ~UnitTest();

  // Pushes a trace defined by SCOPED_TRACE() on to the per-thread
  // Google Test trace stack.
  void PushGTestTrace(const internal::TraceInfo& trace)
      GTEST_LOCK_EXCLUDED_(mutex_);

  // Pops a trace from the per-thread Google Test trace stack.
  void PopGTestTrace()
      GTEST_LOCK_EXCLUDED_(mutex_);

  // Protects mutable state in *impl_.  This is mutable as some const
  // methods need to lock it too.
  mutable internal::Mutex mutex_;

  // Opaque implementation object.  This field is never changed once
  // the object is constructed.  We don't mark it as const here, as
  // doing so will cause a warning in the constructor of UnitTest.
  // Mutable state in *impl_ is protected by mutex_.
  internal::UnitTestImpl* impl_;

  // We disallow copying UnitTest.
  GTEST_DISALLOW_COPY_AND_ASSIGN_(UnitTest);
};

// A convenient wrapper for adding an environment for the test
// program.
//
// You should call this before RUN_ALL_TESTS() is called, probably in
// main().  If you use gtest_main, you need to call this before main()
// starts for it to take effect.  For example, you can define a global
// variable like this:
//
//   testing::Environment* const foo_env =
//       testing::AddGlobalTestEnvironment(new FooEnvironment);
//
// However, we strongly recommend you to write your own main() and
// call AddGlobalTestEnvironment() there, as relying on initialization
// of global variables makes the code harder to read and may cause
// problems when you register multiple environments from different
// translation units and the environments have dependencies among them
// (remember that the compiler doesn't guarantee the order in which
// global variables from different translation units are initialized).
inline Environment* AddGlobalTestEnvironment(Environment* env) {
  return UnitTest::GetInstance()->AddEnvironment(env);
}

// Initializes Google Test.  This must be called before calling
// RUN_ALL_TESTS().  In particular, it parses a command line for the
// flags that Google Test recognizes.  Whenever a Google Test flag is
// seen, it is removed from argv, and *argc is decremented.
//
// No value is returned.  Instead, the Google Test flag variables are
// updated.
//
// Calling the function for the second time has no user-visible effect.
GTEST_API_ void InitGoogleTest(int* argc, char** argv);

// This overloaded version can be used in Windows programs compiled in
// UNICODE mode.
GTEST_API_ void InitGoogleTest(int* argc, wchar_t** argv);

namespace internal {

// Separate the error generating code from the code path to reduce the stack
// frame size of CmpHelperEQ. This helps reduce the overhead of some sanitizers
// when calling EXPECT_* in a tight loop.
template <typename T1, typename T2>
AssertionResult CmpHelperEQFailure(const char* lhs_expression,
                                   const char* rhs_expression,
                                   const T1& lhs, const T2& rhs) {
  return EqFailure(lhs_expression,
                   rhs_expression,
                   FormatForComparisonFailureMessage(lhs, rhs),
                   FormatForComparisonFailureMessage(rhs, lhs),
                   false);
}

// The helper function for {ASSERT|EXPECT}_EQ.
template <typename T1, typename T2>
AssertionResult CmpHelperEQ(const char* lhs_expression,
                            const char* rhs_expression,
                            const T1& lhs,
                            const T2& rhs) {
GTEST_DISABLE_MSC_WARNINGS_PUSH_(4389 /* signed/unsigned mismatch */)
  if (lhs == rhs) {
    return AssertionSuccess();
  }
GTEST_DISABLE_MSC_WARNINGS_POP_()

  return CmpHelperEQFailure(lhs_expression, rhs_expression, lhs, rhs);
}

// With this overloaded version, we allow anonymous enums to be used
// in {ASSERT|EXPECT}_EQ when compiled with gcc 4, as anonymous enums
// can be implicitly cast to BiggestInt.
GTEST_API_ AssertionResult CmpHelperEQ(const char* lhs_expression,
                                       const char* rhs_expression,
                                       BiggestInt lhs,
                                       BiggestInt rhs);

// The helper class for {ASSERT|EXPECT}_EQ.  The template argument
// lhs_is_null_literal is true iff the first argument to ASSERT_EQ()
// is a null pointer literal.  The following default implementation is
// for lhs_is_null_literal being false.
template <bool lhs_is_null_literal>
class EqHelper {
 public:
  // This templatized version is for the general case.
  template <typename T1, typename T2>
  static AssertionResult Compare(const char* lhs_expression,
                                 const char* rhs_expression,
        