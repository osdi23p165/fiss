// Copyright 2008, Google Inc.
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
// Author: keith.ray@gmail.com (Keith Ray)
//
// gtest_xml_outfile1_test_ writes some xml via TestProperty used by
// gtest_xml_outfiles_test.py

#include "gtest/gtest.h"

class PropertyOne : public testing::Test {
 protected:
  virtual void SetUp() {
    RecordProperty("SetUpProp", 1);
  }
  virtual void TearDown() {
    RecordProperty("TearDownProp", 1);
  }
};

TEST_F(PropertyOne, TestSomeProperties) {
  RecordProperty("TestSomeProperty", 1);
}
   || testing::GTEST_FLAG(break_on_failure)
      || testing::GTEST_FLAG(catch_exceptions)
      || testing::GTEST_FLAG(color) != "unknown"
      || testing::GTEST_FLAG(filter) != "unknown"
      || testing::GTEST_FLAG(list_tests)
      || testing::GTEST_FLAG(output) != "unknown"
      || testing::GTEST_FLAG(print_time)
      || testing::GTEST_FLAG(random_seed)
      || testing::GTEST_FLAG(repeat) > 0
      || testing::GTEST_FLAG(show_internal_stack_frames)
      || testing::GTEST_FLAG(shuffle)
      || testing::GTEST_FLAG(stack_trace_depth) > 0
      || testing::GTEST_FLAG(stream_result_to) != "unknown"
      || testing::GTEST_FLAG(throw_on_failure);
  EXPECT_TRUE(dummy || !dummy);  // Suppresses warning that dummy is unused.
}

#include <limits.h>  // For INT_MAX.
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <map>
#include <vector>
#include <ostream>

#include "gtest/gtest-spi.h"

// Indicates that this translation unit is part of Google Test's
// implementation.  It must come before gtest-internal-inl.h is
// included, or there will be a compiler error.  This trick is to
// prevent a user from accidentally including gtest-internal-inl.h in
// his code.
#define GTEST_IMPLEMENTATION_ 1
#include "src/gtest-internal-inl.h"
#undef GTEST_IMPLEMENTATION_

namespace testing {
namespace internal {

#if GTEST_CAN_STREAM_RESULTS_

class StreamingListenerTest : public Test {
 public:
  class FakeSocketWriter : public StreamingListener::AbstractSocketWriter {
   public:
    // Sends a string to the socket.
    virtual void Send(const string& message) { output_ += message; }

    string output_;
  };

  StreamingListenerTest()
      : fake_sock_writer_(new FakeSocketWriter),
        streamer_(fake_sock_writer_),
        test_info_obj_("FooTest", "Bar", NULL, NULL,
                       CodeLocation(__FILE__, __LINE__), 0, NULL) {}

 protected:
  string* output() { return &(fake_sock_writer_->output_); }

  FakeSocketWriter* const fake_sock_writer_;
  StreamingListener streamer_;
  UnitTest unit_test_;
  TestInfo test_info_obj_;  // The name test_info_ was taken by testing::Test.
};

TEST_F(StreamingListenerTest, OnTestProgramEnd) {
  *output() = "";
  streamer_.OnTestProgramEnd(unit_test_);
  EXPECT_EQ("event=TestProgramEnd&passed=1\n", *output());
}

TEST_F(StreamingListenerTest, OnTestIterationEnd) {
  *output() = "";
  streamer_.OnTestIterationEnd(unit_test_, 42);
  EXPECT_EQ("event=TestIterationEnd&passed=1&elapsed_time=0ms\n", *output());
}

TEST_F(StreamingListenerTest, OnTestCaseStart) {
  *output() = "";
  streamer_.OnTestCaseStart(TestCase("FooTest", "Bar", NULL, NULL));
  EXPECT_EQ("event=TestCaseStart&name=FooTest\n", *output());
}

TEST_F(StreamingListenerTest, OnTestCaseEnd) {
  *output() = "";
  streamer_.OnTestCaseEnd(TestCase("FooTest", "Bar", NULL, NULL));
  EXPECT_EQ("event=TestCaseEnd&passed=1&elapsed_time=0ms\n", *output());
}

TEST_F(StreamingListenerTest, OnTestStart) {
  *output() = "";
  streamer_.OnTestStart(test_info_obj_);
  EXPECT_EQ("event=TestStart&name=Bar\n", *output());
}

TEST_F(StreamingListenerTest, OnTestEnd) {
  *output() = "";
  streamer_.OnTestEnd(test_info_obj_);
  EXPECT_EQ("event=TestEnd&passed=1&elapsed_time=0ms\n", *output());
}

TEST_F(StreamingListenerTest, OnTestPartResult) {
  *output() = "";
  streamer_.OnTestPartResult(TestPartResult(
      TestPartResult::kFatalFailure, "foo.cc", 42, "failed=\n&%"));

  // Meta characters in the failure message should be properly escaped.
  EXPECT_EQ(
      "event=TestPartResult&file=foo.cc&line=42&message=failed%3D%0A%26%25\n",
      *output());
}

#endif  // GTEST_CAN_STREAM_RESULTS_

// Provides access to otherwise private parts of the TestEventListeners class
// that are needed to test it.
class TestEventListenersAccessor {
 public:
  static TestEventListener* GetRepeater(TestEventListeners* listeners) {
    return listeners->repeater();
  }

  static void SetDefaultResultPrinter(TestEventListeners* listeners,
                                      TestEventListener* listener) {
    listeners->SetDefaultResultPrinter(listener);
  }
  static void SetDefaultXmlGenerator(TestEventListeners* listeners,
                                     TestEventListener* listener) {
    listeners->SetDefaultXmlGenerator(listener);
  }

  static bool EventForwardingEnabled(const TestEventListeners& listeners) {
    return listeners.EventForwardingEnabled();
  }

  static void SuppressEventForwarding(TestEventListeners* listeners) {
    listeners->SuppressEventForwarding();
  }
};

class UnitTestRecordPropertyTestHelper : public Test {
 protected:
  UnitTestRecordPropertyTestHelper() {}

  // Forwards to UnitTest::RecordProperty() to bypass access controls.
  void UnitTestRecordProperty(const char* key, const std::string& value) {
    unit_test_.RecordProperty(key, value);
  }

  UnitTest unit_test_;
};

}  // namespace internal
}  // namespace testing

using testing::AssertionFailure;
using testing::AssertionResult;
using testing::AssertionSuccess;
using testing::DoubleLE;
using testing::EmptyTestEventListener;
using testing::Environment;
using testing::FloatLE;
using testing::GTEST_FLAG(also_run_disabled_tests);
using testing::GTEST_FLAG(break_on_failure);
using testing::GTEST_FLAG(catch_exceptions);
using testing::GTEST_FLAG(color);
using testing::GTEST_FLAG(death_test_use_fork);
using testing::GTEST_FLAG(filter);
using testing::GTEST_FLAG(list_tests);
using testing::GTEST_FLAG(output);
using testing::GTEST_FLAG(print_time);
using testing::GTEST_FLAG(random_seed);
using testing::GTEST_FLAG(repeat);
using testing::GTEST_FLAG(show_internal_stack_frames);
using testing::GTEST_FLAG(shuffle);
using testing::GTEST_FLAG(stack_trace_depth);
using testing::GTEST_FLAG(stream_result_to);
using testing::GTEST_FLAG(throw_on_failure);
using testing::IsNotSubstring;
using testing::IsSubstring;
using testing::Message;
using testing::ScopedFakeTestPartResultReporter;
using testing::StaticAssertTypeEq;
using testing::Test;
using testing::TestCase;
using testing::TestEventListeners;
using testing::TestInfo;
using testing::TestPartResult;
using testing::TestPartResultArray;
using testing::TestProperty;
using testing::TestResult;
using testing::TimeInMillis;
using testing::UnitTest;
using testing::internal::AddReference;
using testing::internal::AlwaysFalse;
using testing::internal::AlwaysTrue;
using testing::internal::AppendUserMessage;
using testing::internal::ArrayAwareFind;
using testing::internal::ArrayEq;
using testing::internal::CodePointToUtf8;
using testing::internal::CompileAssertTypesEqual;
using testing::internal::CopyArray;
using testing::internal::CountIf;
using testing::internal::EqFailure;
using testing::internal::FloatingPoint;
using testing::internal::ForEach;
using testing::internal::FormatEpochTimeInMillisAsIso8601;
using testing::internal::FormatTimeInMillisAsSeconds;
using testing::internal::GTestFlagSaver;
using testing::internal::GetCurrentOsStackTraceExceptTop;
using testing::internal::GetElementOr;
using testing::internal::GetNextRandomSeed;
using testing::internal::GetRandomSeedFromFlag;
using testing::internal::GetTestTypeId;
using testing::internal::GetTimeInMillis;
using testing::internal::GetTypeId;
using testing::internal::GetUnitTestImpl;
using testing::internal::ImplicitlyConvertible;
using testing::internal::Int32;
using testing::internal::Int32FromEnvOrDie;
using testing::internal::IsAProtocolMessage;
using testing::internal::IsContainer;
using testing::internal::IsContainerTest;
using testing::internal::IsNotContainer;
using testing::internal::NativeArray;
using testing::internal::ParseInt32Flag;
using testing::internal::RelationToSourceCopy;
using testing::internal::RelationToSourceReference;
using testing::internal::RemoveConst;
using testing::internal::RemoveReference;
using testing::internal::ShouldRunTestOnShard;
using testing::internal::ShouldShard;
using testing::internal::ShouldUseColor;
using testing::internal::Shuffle;
using testing::internal::ShuffleRange;
using testing::internal::SkipPrefix;
using testing::internal::StreamableToString;
using testing::internal::String;
using testing::internal::TestEventListenersAccessor;
using testing::internal::TestResultAccessor;
using testing::internal::UInt32;
using testing::internal::WideStringToUtf8;
using testing::internal::edit_distance::CalculateOptimalEdits;
using testing::internal::edit_distance::CreateUnifiedDiff;
using testing::internal::edit_distance::EditType;
using testing::internal::kMaxRandomSeed;
using testing::internal::kTestTypeIdInGoogleTest;
using testing::kMaxStackTraceDepth;

#if GTEST_HAS_STREAM_REDIRECTION
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;
#endif

#if GTEST_IS_THREADSAFE
using testing::internal::ThreadWithParam;
#endif

class TestingVector : public std::vector<int> {
};

::std::ostream& operator<<(::std::ostream& os,
                           const TestingVector& vector) {
  os << "{ ";
  for (size_t i = 0; i < vector.size(); i++) {
    os << vector[i] << " ";
  }
  os << "}";
  return os;
}

// This line tests that we can define tests in an unnamed namespace.
namespace {

TEST(GetRandomSeedFromFlagTest, HandlesZero) {
  const int seed = GetRandomSeedFromFlag(0);
  EXPECT_LE(1, seed);
  EXPECT_LE(seed, static_cast<int>(kMaxRandomSeed));
}

TEST(GetRandomSeedFromFlagTest, PreservesValidSeed) {
  EXPECT_EQ(1, GetRandomSeedFromFlag(1));
  EXPECT_EQ(2, GetRandomSeedFromFlag(2));
  EXPECT_EQ(kMaxRandomSeed - 1, GetRandomSeedFromFlag(kMaxRandomSeed - 1));
  EXPECT_EQ(static_cast<int>(kMaxRandomSeed),
            GetRandomSeedFromFlag(kMaxRandomSeed));
}

TEST(GetRandomSeedFromFlagTest, NormalizesInvalidSeed) {
  const int seed1 = GetRandomSeedFromFlag(-1);
  EXPECT_LE(1, seed1);
  EXPECT_LE(seed1, static_cast<int>(kMaxRandomSeed));

  const int seed2 = GetRandomSeedFromFlag(kMaxRandomSeed + 1);
  EXPECT_LE(1, seed2);
  EXPECT_LE(seed2, static_cast<int>(kMaxRandomSeed));
}

TEST(GetNextRandomSeedTest, WorksForValidInput) {
  EXPECT_EQ(2, GetNextRandomSeed(1));
  EXPECT_EQ(3, GetNextRandomSeed(2));
  EXPECT_EQ(static_cast<int>(kMaxRandomSeed),
            GetNextRandomSeed(kMaxRandomSeed - 1));
  EXPECT_EQ(1, GetNextRandomSeed(kMaxRandomSeed));

  // We deliberately don't test GetNextRandomSeed() with invalid
  // inputs, as that requires death tests, which are expensive.  This
  // is fine as GetNextRandomSeed() is internal and has a
  // straightforward definition.
}

static void ClearCurrentTestPartResults() {
  TestResultAccessor::ClearTestPartResults(
      GetUnitTestImpl()->current_test_result());
}

// Tests GetTypeId.

TEST(GetTypeIdTest, ReturnsSameValueForSameType) {
  EXPECT_EQ(GetTypeId<int>(), GetTypeId<int>());
  EXPECT_EQ(GetTypeId<Test>(), GetTypeId<Test>());
}

class SubClassOfTest : public Test {};
class AnotherSubClassOfTest : public Test {};

TEST(GetTypeIdTest, ReturnsDifferentValuesForDifferentTypes) {
  EXPECT_NE(GetTypeId<int>(), GetTypeId<const int>());
  EXPECT_NE(GetTypeId<int>(), GetTypeId<char>());
  EXPECT_NE(GetTypeId<int>(), GetTestTypeId());
  EXPECT_NE(GetTypeId<SubClassOfTest>(), GetTestTypeId());
  EXPECT_NE(GetTypeId<AnotherSubClassOfTest>(), GetTestTypeId());
  EXPECT_NE(GetTypeId<AnotherSubClassOfTest>(), GetTypeId<SubClassOfTest>());
}

// Verifies that GetTestTypeId() returns the same value, no matter it
// is called from inside Google Test or outside of it.
TEST(GetTestTypeIdTest, ReturnsTheSameValueInsideOrOutsideOfGoogleTest) {
  EXPECT_EQ(kTestTypeIdInGoogleTest, GetTestTypeId());
}

// Tests FormatTimeInMillisAsSeconds().

TEST(FormatTimeInMillisAsSecondsTest, FormatsZero) {
  EXPECT_EQ("0", FormatTimeInMillisAsSeconds(0));
}

TEST(FormatTimeInMillisAsSecondsTest, FormatsPositiveNumber) {
  EXPECT_EQ("0.003", FormatTimeInMillisAsSeconds(3));
  EXPECT_EQ("0.01", FormatTimeInMillisAsSeconds(10));
  EXPECT_EQ("0.2", FormatTimeInMillisAsSeconds(200));
  EXPECT_EQ("1.2", FormatTimeInMillisAsSeconds(1200));
  EXPECT_EQ("3", FormatTimeInMillisAsSeconds(3000));
}

TEST(FormatTimeInMillisAsSecondsTest, FormatsNegativeNumber) {
  EXPECT_EQ("-0.003", FormatTimeInMillisAsSeconds(-3));
  EXPECT_EQ("-0.01", FormatTimeInMillisAsSeconds(-10));
  EXPECT_EQ("-0.2", FormatTimeInMillisAsSeconds(-200));
  EXPECT_EQ("-1.2", FormatTimeInMillisAsSeconds(-1200));
  EXPECT_EQ("-3", FormatTimeInMillisAsSeconds(-3000));
}

// Tests FormatEpochTimeInMillisAsIso8601().  The correctness of conversion
// for particular dates below was verified in Python using
// datetime.datetime.fromutctimestamp(<timetamp>/1000).

// FormatEpochTimeInMillisAsIso8601 depends on the current timezone, so we
// have to set up a particular timezone to obtain predictable results.
class FormatEpochTimeInMillisAsIso8601Test : public Test {
 public:
  // On Cygwin, GCC doesn't allow unqualified integer literals to exceed
  // 32 bits, even when 64-bit integer types are available.  We have to
  // force the constants to have a 64-bit type here.
  static const TimeInMillis kMillisPerSec = 1000;

 private:
  virtual void SetUp() {
    saved_tz_ = NULL;

    GTEST_DISABLE_MSC_WARNINGS_PUSH_(4996 /* getenv, strdup: deprecated */)
    if (getenv("TZ"))
      saved_tz_ = strdup(getenv("TZ"));
    GTEST_DISABLE_MSC_WARNINGS_POP_()

    // Set up the time zone for FormatEpochTimeInMillisAsIso8601 to use.  We
    // cannot use the local time zone because the function's output depends
    // on the time zone.
    SetTimeZone("UTC+00");
  }

  virtual void TearDown() {
    SetTimeZone(saved_tz_);
    free(const_cast<char*>(saved_tz_));
    saved_tz_ = NULL;
  }

  static void SetTimeZone(const char* time_zone) {
    // tzset() distinguishes between the TZ variable being present and empty
    // and not being present, so we have to consider the case of time_zone
    // being NULL.
#if _MSC_VER
    // ...Unless it's MSVC, whose standard library's _putenv doesn't
    // distinguish between an empty and a missing variable.
    const std::string env_var =
        std::string("TZ=") + (time_zone ? time_zone : "");
    _putenv(env_var.c_str());
    GTEST_DISABLE_MSC_WARNINGS_PUSH_(4996 /* deprecated function */)
    tzset();
    GTEST_DISABLE_MSC_WARNINGS_POP_()
#else
    if (time_zone) {
      setenv(("TZ"), time_zone, 1);
    } else {
      unsetenv("TZ");
    }
    tzset();
#endif
  }

  const char* saved_tz_;
};

const TimeInMillis FormatEpochTimeInMillisAsIso8601Test::kMillisPerSec;

TEST_F(FormatEpochTimeInMillisAsIso8601Test, PrintsTwoDigitSegments) {
  EXPECT_EQ("2011-10-31T18:52:42",
            FormatEpochTimeInMillisAsIso8601(1320087162 * kMillisPerSec));
}

TEST_F(FormatEpochTimeInMillisAsIso8601Test, MillisecondsDoNotAffectResult) {
  EXPECT_EQ(
      "2011-10-31T18:52:42",
      FormatEpochTimeInMillisAsIso8601(1320087162 * kMillisPerSec + 234));
}

TEST_F(FormatEpochTimeInMillisAsIso8601Test, PrintsLeadingZeroes) {
  EXPECT_EQ("2011-09-03T05:07:02",
            FormatEpochTimeInMillisAsIso8601(1315026422 * kMillisPerSec));
}

TEST_F(FormatEpochTimeInMillisAsIso8601Test, Prints24HourTime) {
  EXPECT_EQ("2011-09-28T17:08:22",
            FormatEpochTimeInMillisAsIso8601(1317229702 * kMillisPerSec));
}

TEST_F(FormatEpochTimeInMillisAsIso8601Test, PrintsEpochStart) {
  EXPECT_EQ("1970-01-01T00:00:00", FormatEpochTimeInMillisAsIso8601(0));
}

#if GTEST_CAN_COMPARE_NULL

# ifdef __BORLANDC__
// Silences warnings: "Condition is always true", "Unreachable code"
#  pragma option push -w-ccc -w-rch
# endif

// Tests that GTEST_IS_NULL_LITERAL_(x) is true when x is a null
// pointer literal.
TEST(NullLiteralTest, IsTrueForNullLiterals) {
  EXPECT_TRUE(GTEST_IS_NULL_LITERAL_(NULL));
  EXPECT_TRUE(GTEST_IS_NULL_LITERAL_(0));
  EXPECT_TRUE(GTEST_IS_NULL_LITERAL_(0U));
  EXPECT_TRUE(GTEST_IS_NULL_LITERAL_(0L));
}

// Tests that GTEST_IS_NULL_LITERAL_(x) is false when x is not a null
// pointer literal.
TEST(NullLiteralTest, IsFalseForNonNullLiterals) {
  EXPECT_FALSE(GTEST_IS_NULL_LITERAL_(1));
  EXPECT_FALSE(GTEST_IS_NULL_LITERAL_(0.0));
  EXPECT_FALSE(GTEST_IS_NULL_LITERAL_('a'));
  EXPECT_FALSE(GTEST_IS_NULL_LITERAL_(static_cast<void*>(NULL)));
}

# ifdef __BORLANDC__
// Restores warnings after previous "#pragma option push" suppressed them.
#  pragma option pop
# endif

#endif  // GTEST_CAN_COMPARE_NULL
//
// Tests CodePointToUtf8().

// Tests that the NUL character L'\0' is encoded correctly.
TEST(CodePointToUtf8Test, CanEncodeNul) {
  EXPECT_EQ("", CodePointToUtf8(L'\0'));
}

// Tests that ASCII characters are encoded correctly.
TEST(CodePointToUtf8Test, CanEncodeAscii) {
  EXPECT_EQ("a", CodePointToUtf8(L'a'));
  EXPECT_EQ("Z", CodePointToUtf8(L'Z'));
  EXPECT_EQ("&", CodePointToUtf8(L'&'));
  EXPECT_EQ("\x7F", CodePointToUtf8(L'\x7F'));
}

// Tests that Unicode code-points that have 8 to 11 bits are encoded
// as 110xxxxx 10xxxxxx.
TEST(CodePointToUtf8Test, CanEncode8To11Bits) {
  // 000 1101 0011 => 110-00011 10-010011
  EXPECT_EQ("\xC3\x93", CodePointToUtf8(L'\xD3'));

  // 101 0111 0110 => 110-10101 10-110110
  // Some compilers (e.g., GCC on MinGW) cannot handle non-ASCII codepoints
  // in wide strings and wide chars. In order to accomodate them, we have to
  // introduce such character constants as integers.
  EXPECT_EQ("\xD5\xB6",
            CodePointToUtf8(static_cast<wchar_t>(0x576)));
}

// Tests that Unicode code-points that have 12 to 16 bits are encoded
// as 1110xxxx 10xxxxxx 10xxxxxx.
TEST(CodePointToUtf8Test, CanEncode12To16Bits) {
  // 0000 1000 1101 0011 => 1110-0000 10-100011 10-010011
  EXPECT_EQ("\xE0\xA3\x93",
            CodePointToUtf8(static_cast<wchar_t>(0x8D3)));

  // 1100 0111 0100 1101 => 1110-1100 10-011101 10-001101
  EXPECT_EQ("\xEC\x9D\x8D",
            CodePointToUtf8(static_cast<wchar_t>(0xC74D)));
}

#if !GTEST_WIDE_STRING_USES_UTF16_
// Tests in this group require a wchar_t to hold > 16 bits, and thus
// are skipped on Windows, Cygwin, and Symbian, where a wchar_t is
// 16-bit wide. This code may not compile on those systems.

// Tests that Unicode code-points that have 17 to 21 bits are encoded
// as 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx.
TEST(CodePointToUtf8Test, CanEncode17To21Bits) {
  // 0 0001 0000 1000 1101 0011 => 11110-000 10-010000 10-100011 10-010011
  EXPECT_EQ("\xF0\x90\xA3\x93", CodePointToUtf8(L'\x108D3'));

  // 0 0001 0000 0100 0000 0000 => 11110-000 10-010000 10-010000 10-000000
  EXPECT_EQ("\xF0\x90\x90\x80", CodePointToUtf8(L'\x10400'));

  // 1 0000 1000 0110 0011 0100 => 11110-100 10-001000 10-011000 10-110100
  EXPECT_EQ("\xF4\x88\x98\xB4", CodePointToUtf8(L'\x108634'));
}

// Tests that encoding an invalid code-point generates the expected result.
TEST(CodePointToUtf8Test, CanEncodeInvalidCodePoint) {
  EXPECT_EQ("(Invalid Unicode 0x1234ABCD)", CodePointToUtf8(L'\x1234ABCD'));
}

#endif  // !GTEST_WIDE_STRING_USES_UTF16_

// Tests WideStringToUtf8().

// Tests that the NUL character L'\0' is encoded correctly.
TEST(WideStringToUtf8Test, CanEncodeNul) {
  EXPECT_STREQ("", WideStringToUtf8(L"", 0).c_str());
  EXPECT_STREQ("", WideStringToUtf8(L"", -1).c_str());
}

// Tests that ASCII strings are encoded correctly.
TEST(WideStringToUtf8Test, CanEncodeAscii) {
  EXPECT_STREQ("a", WideStringToUtf8(L"a", 1).c_str());
  EXPECT_STREQ("ab", WideStringToUtf8(L"ab", 2).c_str());
  EXPECT_STREQ("a", WideStringToUtf8(L"a", -1).c_str());
  EXPECT_STREQ("ab", WideStringToUtf8(L"ab", -1).c_str());
}

// Tests that Unicode code-points that have 8 to 11 bits are encoded
// as 110xxxxx 10xxxxxx.
TEST(WideStringToUtf8Test, CanEncode8To11Bits) {
  // 000 1101 0011 => 110-00011 10-010011
  EXPECT_STREQ("\xC3\x93", WideStringToUtf8(L"\xD3", 1).c_str());
  EXPECT_STREQ("\xC3\x93", WideStringToUtf8(L"\xD3", -1).c_str());

  // 101 0111 0110 => 110-10101 10-110110
  const wchar_t s[] = { 0x576, '\0' };
  EXPECT_STREQ("\xD5\xB6", WideStringToUtf8(s, 1).c_str());
  EXPECT_STREQ("\xD5\xB6", WideStringToUtf8(s, -1).c_str());
}

// Tests that Unicode code-points that have 12 to 16 bits are encoded
// as 1110xxxx 10xxxxxx 10xxxxxx.
TEST(WideStringToUtf8Test, CanEncode12To16Bits) {
  // 0000 1000 1101 0011 => 1110-0000 10-100011 10-010011
  const wchar_t s1[] = { 0x8D3, '\0' };
  EXPECT_STREQ("\xE0\xA3\x93", WideStringToUtf8(s1, 1).c_str());
  EXPECT_STREQ("\xE0\xA3\x93", WideStringToUtf8(s1, -1).c_str());

  // 1100 0111 0100 1101 => 1110-1100 10-011101 10-001101
  const wchar_t s2[] = { 0xC74D, '\0' };
  EXPECT_STREQ("\xEC\x9D\x8D", WideStringToUtf8(s2, 1).c_str());
  EXPECT_STREQ("\xEC\x9D\x8D", WideStringToUtf8(s2, -1).c_str());
}

// Tests that the conversion stops when the function encounters \0 character.
TEST(WideStringToUtf8Test, StopsOnNulCharacter) {
  EXPECT_STREQ("ABC", WideStringToUtf8(L"ABC\0XYZ", 100).c_str());
}

// Tests that the conversion stops when the function reaches the limit
// specified by the 'length' parameter.
TEST(WideStringToUtf8Test, StopsWhenLengthLimitReached) {
  EXPECT_STREQ("ABC", WideStringToUtf8(L"ABCDEF", 3).c_str());
}

#if !GTEST_WIDE_STRING_USES_UTF16_
// Tests that Unicode code-points that have 17 to 21 bits are encoded
// as 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx. This code may not compile
// on the systems using UTF-16 encoding.
TEST(WideStringToUtf8Test, CanEncode17To21Bits) {
  // 0 0001 0000 1000 1101 0011 => 11110-000 10-010000 10-100011 10-010011
  EXPECT_STREQ("\xF0\x90\xA3\x93", WideStringToUtf8(L"\x108D3", 1).c_str());
  EXPECT_STREQ("\xF0\x90\xA3\x93", WideStringToUtf8(L"\x108D3", -1).c_str());

  // 1 0000 1000 0110 0011 0100 => 11110-100 10-001000 10-011000 10-110100
  EXPECT_STREQ("\xF4\x88\x98\xB4", WideStringToUtf8(L"\x108634", 1).c_str());
  EXPECT_STREQ("\xF4\x88\x98\xB4", WideStringToUtf8(L"\x108634", -1).c_str());
}

// Tests that encoding an invalid code-point generates the expected result.
TEST(WideStringToUtf8Test, CanEncodeInvalidCodePoint) {
  EXPECT_STREQ("(Invalid Unicode 0xABCDFF)",
               WideStringToUtf8(L"\xABCDFF", -1).c_str());
}
#else  // !GTEST_WIDE_STRING_USES_UTF16_
// Tests that surrogate pairs are encoded correctly on the systems using
// UTF-16 encoding in the wide strings.
TEST(WideStringToUtf8Test, CanEncodeValidUtf16SUrrogatePairs) {
  const wchar_t s[] = { 0xD801, 0xDC00, '\0' };
  EXPECT_STREQ("\xF0\x90\x90\x80", WideStringToUtf8(s, -1).c_str());
}

// Tests that encoding an invalid UTF-16 surrogate pair
// generates the expected result.
TEST(WideStringToUtf8Test, CanEncodeInvalidUtf16SurrogatePair) {
  // Leading surrogate is at the end of the string.
  const wchar_t s1[] = { 0xD800, '\0' };
  EXPECT_STREQ("\xED\xA0\x80", WideStringToUtf8(s1, -1).c_str());
  // Leading surrogate is not followed by the trailing surrogate.
  const wchar_t s2[] = { 0xD800, 'M', '\0' };
  EXPECT_STREQ("\xED\xA0\x80M", WideStringToUtf8(s2, -1).c_str());
  // Trailing surrogate appearas without a leading surrogate.
  const wchar_t s3[] = { 0xDC00, 'P', 'Q', 'R', '\0' };
  EXPECT_STREQ("\xED\xB0\x80PQR", WideStringToUtf8(s3, -1).c_str());
}
#endif  // !GTEST_WIDE_STRING_USES_UTF16_

// Tests that codepoint concatenation works correctly.
#if !GTEST_WIDE_STRING_USES_UTF16_
TEST(WideStringToUtf8Test, ConcatenatesCodepointsCorrectly) {
  const wchar_t s[] = { 0x108634, 0xC74D, '\n', 0x576, 0x8D3, 0x108634, '\0'};
  EXPECT_STREQ(
      "\xF4\x88\x98\xB4"
          "\xEC\x9D\x8D"
          "\n"
          "\xD5\xB6"
          "\xE0\xA3\x93"
          "\xF4\x88\x98\xB4",
      WideStringToUtf8(s, -1).c_str());
}
#else
TEST(WideStringToUtf8Test, ConcatenatesCodepointsCorrectly) {
  const wchar_t s[] = { 0xC74D, '\n', 0x576, 0x8D3, '\0'};
  EXPECT_STREQ(
      "\xEC\x9D\x8D" "\n" "\xD5\xB6" "\xE0\xA3\x93",
      WideStringToUtf8(s, -1).c_str());
}
#endif  // !GTEST_WIDE_STRING_USES_UTF16_

// Tests the Random class.

TEST(RandomDeathTest, GeneratesCrashesOnInvalidRange) {
  testing::internal::Random random(42);
  EXPECT_DEATH_IF_SUPPORTED(
      random.Generate(0),
      "Cannot generate a number in the range \\[0, 0\\)");
  EXPECT_DEATH_IF_SUPPORTED(
      random.Generate(testing::internal::Random::kMaxRange + 1),
      "Generation of a number in \\[0, 2147483649\\) was requested, "
      "but this can only generate numbers in \\[0, 2147483648\\)");
}

TEST(RandomTest, GeneratesNumbersWithinRange) {
  const UInt32 kRange = 10000;
  testing::internal::Random random(12345);
  for (int i = 0; i < 10; i++) {
    EXPECT_LT(random.Generate(kRange), kRange) << " for iteration " << i;
  }

  testing::internal::Random random2(testing::internal::Random::kMaxRange);
  for (int i = 0; i < 10; i++) {
    EXPECT_LT(random2.Generate(kRange), kRange) << " for iteration " << i;
  }
}

TEST(RandomTest, RepeatsWhenReseeded) {
  const int kSeed = 123;
  const int kArraySize = 10;
  const UInt32 kRange = 10000;
  UInt32 values[kArraySize];

  testing::internal::Random random(kSeed);
  for (int i = 0; i < kArraySize; i++) {
    values[i] = random.Generate(kRange);
  }

  random.Reseed(kSeed);
  for (int i = 0; i < kArraySize; i++) {
    EXPECT_EQ(values[i], random.Generate(kRange)) << " for iteration " << i;
  }
}

// Tests STL container utilities.

// Tests CountIf().

static bool IsPositive(int n) { return n > 0; }

TEST(ContainerUtilityTest, CountIf) {
  std::vector<int> v;
  EXPECT_EQ(0, CountIf(v, IsPositive));  // Works for an empty container.

  v.push_back(-1);
  v.push_back(0);
  EXPECT_EQ(0, CountIf(v, IsPositive));  // Works when no value satisfies.

  v.push_back(2);
  v.push_back(-10);
  v.push_back(10);
  EXPECT_EQ(2, CountIf(v, IsPositive));
}

// Tests ForEach().

static int g_sum = 0;
static void Accumulate(int n) { g_sum += n; }

TEST(ContainerUtilityTest, ForEach) {
  std::vector<int> v;
  g_sum = 0;
  ForEach(v, Accumulate);
  EXPECT_EQ(0, g_sum);  // Works for an empty container;

  g_sum = 0;
  v.push_back(1);
  ForEach(v, Accumulate);
  EXPECT_EQ(1, g_sum);  // Works for a container with one element.

  g_sum = 0;
  v.push_back(20);
  v.push_back(300);
  ForEach(v, Accumulate);
  EXPECT_EQ(321, g_sum);
}

// Tests GetElementOr().
TEST(ContainerUtilityTest, GetElementOr) {
  std::vector<char> a;
  EXPECT_EQ('x', GetElementOr(a, 0, 'x'));

  a.push_back('a');
  a.push_back('b');
  EXPECT_EQ('a', GetElementOr(a, 0, 'x'));
  EXPECT_EQ('b', GetElementOr(a, 1, 'x'));
  EXPECT_EQ('x', GetElementOr(a, -2, 'x'));
  EXPECT_EQ('x', GetElementOr(a, 2, 'x'));
}

TEST(ContainerUtilityDeathTest, ShuffleRange) {
  std::vector<int> a;
  a.push_back(0);
  a.push_back(1);
  a.push_back(2);
  testing::internal::Random random(1);

  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, -1, 1, &a),
      "Invalid shuffle range start -1: must be in range \\[0, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 4, 4, &a),
      "Invalid shuffle range start 4: must be in range \\[0, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 3, 2, &a),
      "Invalid shuffle range finish 2: must be in range \\[3, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 3, 4, &a),
      "Invalid shuffle range finish 4: must be in range \\[3, 3\\]");
}

class VectorShuffleTest : public Test {
 protected:
  static const int kVectorSize = 20;

  VectorShuffleTest() : random_(1) {
    for (int i = 0; i < kVectorSize; i++) {
      vector_.push_back(i);
    }
  }

  static bool VectorIsCorrupt(const TestingVector& vector) {
    if (kVectorSize != static_cast<int>(vector.size())) {
      return true;
    }

    bool found_in_vector[kVectorSize] = { false };
    for (size_t i = 0; i < vector.size(); i++) {
      const int e = vector[i];
      if (e < 0 || e >= kVectorSize || found_in_vector[e]) {
        return true;
      }
      found_in_vector[e] = true;
    }

    // Vector size is correct, elements' range is correct, no
    // duplicate elements.  Therefore no corruption has occurred.
    return false;
  }

  static bool VectorIsNotCorrupt(const TestingVector& vector) {
    return !VectorIsCorrupt(vector);
  }

  static bool RangeIsShuffled(const TestingVector& vector, int begin, int end) {
    for (int i = begin; i < end; i++) {
      if (i != vector[i]) {
        return true;
      }
    }
    return false;
  }

  static bool RangeIsUnshuffled(
      const TestingVector& vector, int begin, int end) {
    return !RangeIsShuffled(vector, begin, end);
  }

  static bool VectorIsShuffled(const TestingVector& vector) {
    return RangeIsShuffled(vector, 0, static_cast<int>(vector.size()));
  }

  static bool VectorIsUnshuffled(const TestingVector& vector) {
    return !VectorIsShuffled(vector);
  }

  testing::internal::Random random_;
  TestingVector vector_;
};  // class VectorShuffleTest

const int VectorShuffleTest::kVectorSize;

TEST_F(VectorShuffleTest, HandlesEmptyRange) {
  // Tests an empty range at the beginning...
  ShuffleRange(&random_, 0, 0, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);

  // ...in the middle...
  ShuffleRange(&random_, kVectorSize/2, kVectorSize/2, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);

  // ...at the end...
  ShuffleRange(&random_, kVectorSize - 1, kVectorSize - 1, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);

  // ...and past the end.
  ShuffleRange(&random_, kVectorSize, kVectorSize, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);
}

TEST_F(VectorShuffleTest, HandlesRangeOfSizeOne) {
  // Tests a size one range at the beginning...
  ShuffleRange(&random_, 0, 1, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);

  // ...in the middle...
  ShuffleRange(&random_, kVectorSize/2, kVectorSize/2 + 1, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);

  // ...and at the end.
  ShuffleRange(&random_, kVectorSize - 1, kVectorSize, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsUnshuffled, vector_);
}

// Because we use our own random number generator and a fixed seed,
// we can guarantee that the following "random" tests will succeed.

TEST_F(VectorShuffleTest, ShufflesEntireVector) {
  Shuffle(&random_, &vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  EXPECT_FALSE(VectorIsUnshuffled(vector_)) << vector_;

  // Tests the first and last elements in particular to ensure that
  // there are no off-by-one problems in our shuffle algorithm.
  EXPECT_NE(0, vector_[0]);
  EXPECT_NE(kVectorSize - 1, vector_[kVectorSize - 1]);
}

TEST_F(VectorShuffleTest, ShufflesStartOfVector) {
  const int kRangeSize = kVectorSize/2;

  ShuffleRange(&random_, 0, kRangeSize, &vector_);

  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  EXPECT_PRED3(RangeIsShuffled, vector_, 0, kRangeSize);
  EXPECT_PRED3(RangeIsUnshuffled, vector_, kRangeSize, kVectorSize);
}

TEST_F(VectorShuffleTest, ShufflesEndOfVector) {
  const int kRangeSize = kVectorSize / 2;
  ShuffleRange(&random_, kRangeSize, kVectorSize, &vector_);

  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  EXPECT_PRED3(RangeIsUnshuffled, vector_, 0, kRangeSize);
  EXPECT_PRED3(RangeIsShuffled, vector_, kRangeSize, kVectorSize);
}

TEST_F(VectorShuffleTest, ShufflesMiddleOfVector) {
  int kRangeSize = kVectorSize/3;
  ShuffleRange(&random_, kRangeSize, 2*kRangeSize, &vector_);

  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  EXPECT_PRED3(RangeIsUnshuffled, vector_, 0, kRangeSize);
  EXPECT_PRED3(RangeIsShuffled, vector_, kRangeSize, 2*kRangeSize);
  EXPECT_PRED3(RangeIsUnshuffled, vector_, 2*kRangeSize, kVectorSize);
}

TEST_F(VectorShuffleTest, ShufflesRepeatably) {
  TestingVector vector2;
  for (int i = 0; i < kVectorSize; i++) {
    vector2.push_back(i);
  }

  random_.Reseed(1234);
  Shuffle(&random_, &vector_);
  random_.Reseed(1234);
  Shuffle(&random_, &vector2);

  ASSERT_PRED1(VectorIsNotCorrupt, vector_);
  ASSERT_PRED1(VectorIsNotCorrupt, vector2);

  for (int i = 0; i < kVectorSize; i++) {
    EXPECT_EQ(vector_[i], vector2[i]) << " where i is " << i;
  }
}

// Tests the size of the AssertHelper class.

TEST(AssertHelperTest, AssertHelperIsSmall) {
  // To avoid breaking clients that use lots of assertions in one
  // function, we cannot grow the size of AssertHelper.
  EXPECT_LE(sizeof(testing::internal::AssertHelper), sizeof(void*));
}

// Tests String::EndsWithCaseInsensitive().
TEST(StringTest, EndsWithCaseInsensitive) {
  EXPECT_TRUE(String::EndsWithCaseInsensitive("foobar", "BAR"));
  EXPECT_TRUE(String::EndsWithCaseInsensitive("foobaR", "bar"));
  EXPECT_TRUE(String::EndsWithCaseInsensitive("foobar", ""));
  EXPECT_TRUE(String::EndsWithCaseInsensitive("", ""));

  EXPECT_FALSE(String::EndsWithCaseInsensitive("Foobar", "foo"));
  EXPECT_FALSE(String::EndsWithCaseInsensitive("foobar", "Foo"));
  EXPECT_FALSE(String::EndsWithCaseInsensitive("", "foo"));
}

// C++Builder's preprocessor is buggy; it fails to expand macros that
// appear in macro parameters after wide char literals.  Provide an alias
// for NULL as a workaround.
static const wchar_t* const kNull = NULL;

// Tests String::CaseInsensitiveWideCStringEquals
TEST(StringTest, CaseInsensitiveWideCStringEquals) {
  EXPECT_TRUE(String::CaseInsensitiveWideCStringEquals(NULL, NULL));
  EXPECT_FALSE(String::CaseInsensitiveWideCStringEquals(kNull, L""));
  EXPECT_FALSE(String::CaseInsensitiveWideCStringEquals(L"", kNull));
  EXPECT_FALSE(String::CaseInsensitiveWideCStringEquals(kNull, L"foobar"));
  EXPECT_FALSE(String::CaseInsensitiveWideCStringEquals(L"foobar", kNull));
  EXPECT_TRUE(String::CaseInsensitiveWideCStringEquals(L"foobar", L"foobar"));
  EXPECT_TRUE(String::CaseInsensitiveWideCStringEquals(L"foobar", L"FOOBAR"));
  EXPECT_TRUE(String::CaseInsensitiveWideCStringEquals(L"FOOBAR", L"foobar"));
}

#if GTEST_OS_WINDOWS

// Tests String::ShowWideCString().
TEST(StringTest, ShowWideCString) {
  EXPECT_STREQ("(null)",
               String::ShowWideCString(NULL).c_str());
  EXPECT_STREQ("", String::ShowWideCString(L"").c_str());
  EXPECT_STREQ("foo", String::ShowWideCString(L"foo").c_str());
}

# if GTEST_OS_WINDOWS_MOBILE
TEST(StringTest, AnsiAndUtf16Null) {
  EXPECT_EQ(NULL, String::AnsiToUtf16(NULL));
  EXPECT_EQ(NULL, String::Utf16ToAnsi(NULL));
}

TEST(StringTest, AnsiAndUtf16ConvertBasic) {
  const char* ansi = String::Utf16ToAnsi(L"str");
  EXPECT_STREQ("str", ansi);
  delete [] ansi;
  const WCHAR* utf16 = String::AnsiToUtf16("str");
  EXPECT_EQ(0, wcsncmp(L"str", utf16, 3));
  delete [] utf16;
}

TEST(StringTest, AnsiAndUtf16ConvertPathChars) {
  const char* ansi = String::Utf16ToAnsi(L".:\\ \"*?");
  EXPECT_STREQ(".:\\ \"*?", ansi);
  delete [] ansi;
  const WCHAR* utf16 = String::AnsiToUtf16(".:\\ \"*?");
  EXPECT_EQ(0, wcsncmp(L".:\\ \"*?", utf16, 3));
  delete [] utf16;
}
# endif  // GTEST_OS_WINDOWS_MOBILE

#endif  // GTEST_OS_WINDOWS

// Tests TestProperty construction.
TEST(TestPropertyTest, StringValue) {
  TestProperty property("key", "1");
  EXPECT_STREQ("key", property.key());
  EXPECT_STREQ("1", property.value());
}

// Tests TestProperty replacing a value.
TEST(TestPropertyTest, ReplaceStringValue) {
  TestProperty property("key", "1");
  EXPECT_STREQ("1", property.value());
  property.SetValue("2");
  EXPECT_STREQ("2", property.value());
}

// AddFatalFailure() and AddNonfatalFailure() must be stand-alone
// functions (i.e. their definitions cannot be inlined at the call
// sites), or C++Builder won't compile the code.
static void AddFatalFailure() {
  FAIL() << "Expected fatal failure.";
}

static void AddNonfatalFailure() {
  ADD_FAILURE() << "Expected non-fatal failure.";
}

class ScopedFakeTestPartResultReporterTest : public Test {
 public:  // Must be public and not protected due to a bug in g++ 3.4.2.
  enum FailureMode {
    FATAL_FAILURE,
    NONFATAL_FAILURE
  };
  static void AddFailure(FailureMode failure) {
    if (failure == FATAL_FAILURE) {
      AddFatalFailure();
    } else {
      AddNonfatalFailure();
    }
  }
};

// Tests that ScopedFakeTestPartResultReporter intercepts test
// failures.
TEST_F(ScopedFakeTestPartResultReporterTest, InterceptsTestFailures) {
  TestPartResultArray results;
  {
    ScopedFakeTestPartResultReporter reporter(
        ScopedFakeTestPartResultReporter::INTERCEPT_ONLY_CURRENT_THREAD,
        &results);
    AddFailure(NONFATAL_FAILURE);
    AddFailure(FATAL_FAILURE);
  }

  EXPECT_EQ(2, results.size());
  EXPECT_TRUE(results.GetTestPartResult(0).nonfatally_failed());
  EXPECT_TRUE(results.GetTestPartResult(1).fatally_failed());
}

TEST_F(ScopedFakeTestPartResultReporterTest, DeprecatedConstructor) {
  TestPartResultArray results;
  {
    // Tests, that the deprecated constructor still works.
    ScopedFakeTestPartResultReporter reporter(&results);
    AddFailure(NONFATAL_FAILURE);
  }
  EXPECT_EQ(1, results.size());
}

#if GTEST_IS_THREADSAFE

class ScopedFakeTestPartResultReporterWithThreadsTest
  : public ScopedFakeTestPartResultReporterTest {
 protected:
  static void AddFailureInOtherThread(FailureMode failure) {
    ThreadWithParam<FailureMode> thread(&AddFailure, failure, NULL);
    thread.Join();
  }
};

TEST_F(ScopedFakeTestPartResultReporterWithThreadsTest,
       InterceptsTestFailuresInAllThreads) {
  TestPartResultArray results;
  {
    ScopedFakeTestPartResultReporter reporter(
        ScopedFakeTestPartResultReporter::INTERCEPT_ALL_THREADS, &results);
    AddFailure(NONFATAL_FAILURE);
    AddFailure(FATAL_FAILURE);
    AddFailureInOtherThread(NONFATAL_FAILURE);
    AddFailureInOtherThread(FATAL_FAILURE);
  }

  EXPECT_EQ(4, results.size());
  EXPECT_TRUE(results.GetTestPartResult(0).nonfatally_failed());
  EXPECT_TRUE(results.GetTestPartResult(1).fatally_failed());
  EXPECT_TRUE(results.GetTestPartResult(2).nonfatally_failed());
  EXPECT_TRUE(results.GetTestPartResult(3).fatally_failed());
}

#endif  // GTEST_IS_THREADSAFE

// Tests EXPECT_FATAL_FAILURE{,ON_ALL_THREADS}.  Makes sure that they
// work even if the failure is generated in a called function rather than
// the current context.

typedef ScopedFakeTestPartResultReporterTest ExpectFatalFailureTest;

TEST_F(ExpectFatalFailureTest, CatchesFatalFaliure) {
  EXPECT_FATAL_FAILURE(AddFatalFailure(), "Expected fatal failure.");
}

#if GTEST_HAS_GLOBAL_STRING
TEST_F(ExpectFatalFailureTest, AcceptsStringObject) {
  EXPECT_FATAL_FAILURE(AddFatalFailure(), ::string("Expected fatal failure."));
}
#endif

TEST_F(ExpectFatalFailureTest, AcceptsStdStringObject) {
  EXPECT_FATAL_FAILURE(AddFatalFailure(),
                       ::std::string("Expected fatal failure."));
}

TEST_F(ExpectFatalFailureTest, CatchesFatalFailureOnAllThreads) {
  // We have another test below to verify that the macro catches fatal
  // failures generated on another thread.
  EXPECT_FATAL_FAILURE_ON_ALL_THREADS(AddFatalFailure(),
                                      "Expected fatal failure.");
}

#ifdef __BORLANDC__
// Silences warnings: "Condition is always true"
# pragma option push -w-ccc
#endif

// Tests that EXPECT_FATAL_FAILURE() can be used in a non-void
// function even when the statement in it contains ASSERT_*.

int NonVoidFunction() {
  EXPECT_FATAL_FAILURE(ASSERT_TRUE(false), "");
  EXPECT_FATAL_FAILURE_ON_ALL_THREADS(FAIL(), "");
  return 0;
}

TEST_F(ExpectFatalFailureTest, CanBeUsedInNonVoidFunction) {
  NonVoidFunction();
}

// Tests that EXPECT_FATAL_FAILURE(statement, ...) doesn't abort the
// current function even though 'statement' generates a fatal failure.

void DoesNotAbortHelper(bool* aborted) {
  EXPECT_FATAL_FAILURE(ASSERT_TRUE(false), "");
  EXPECT_FATAL_FAILURE_ON_ALL_THREADS(FAIL(), "");

  *aborted = false;
}

#ifdef __BORLANDC__
// Restores warnings after previous "#pragma option push" suppressed them.
# pragma option pop
#endif

TEST_F(ExpectFatalFailureTest, DoesNotAbort) {
  bool aborted = true;
  DoesNotAbortHelper(&aborted);
  EXPECT_FALSE(aborted);
}

// Tests that the EXPECT_FATAL_FAILURE{,_ON_ALL_THREADS} accepts a
// statement that contains a macro which expands to code containing an
// unprotected comma.

static int global_var = 0;
#define GTEST_USE_UNPROTECTED_COMMA_ global_var++, global_var++

TEST_F(ExpectFatalFailureTest, AcceptsMacroThatExpandsToUnprotectedComma) {
#ifndef __BORLANDC__
  // ICE's in C++Builder.
  EXPECT_FATAL_FAILURE({
    GTEST_USE_UNPROTECTED_COMMA_;
    AddFatalFailure();
  }, "");
#endif

  EXPECT_FATAL_FAILURE_ON_ALL_THREADS({
    GTEST_USE_UNPROTECTED_COMMA_;
    AddFatalFailure();
  }, "");
}

// Tests EXPECT_NONFATAL_FAILURE{,ON_ALL_THREADS}.

typedef ScopedFakeTestPartResultReporterTest ExpectNonfatalFailureTest;

TEST_F(ExpectNonfatalFailureTest, CatchesNonfatalFailure) {
  EXPECT_NONFATAL_FAILURE(AddNonfatalFailure(),
                          "Expected non-fatal failure.");
}

#if GTEST_HAS_GLOBAL_STRING
TEST_F(ExpectNonfatalFailureTest, AcceptsStringObject) {
  EXPECT_NONFATAL_FAILURE(AddNonfatalFailure(),
                          ::string("Expected non-fatal failure."));
}
#endif

TEST_F(ExpectNonfatalFailureTest, AcceptsStdStringObject) {
  EXPECT_NONFATAL_FAILURE(AddNonfatalFailure(),
                          ::std::string("Expected non-fatal failure."));
}

TEST_F(ExpectNonfatalFailureTest, CatchesNonfatalFailureOnAllThreads) {
  // We have another test below to verify that the macro catches
  // non-fatal failures generated on another thread.
  EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS(AddNonfatalFailure(),
                                         "Expected non-fatal failure.");
}

// Tests that the EXPECT_NONFATAL_FAILURE{,_ON_ALL_THREADS} accepts a
// statement that contains a macro which expands to code containing an
// unprotected comma.
TEST_F(ExpectNonfatalFailureTest, AcceptsMacroThatExpandsToUnprotectedComma) {
  EXPECT_NONFATAL_FAILURE({
    GTEST_USE_UNPROTECTED_COMMA_;
    AddNonfatalFailure();
  }, "");

  EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS({
    GTEST_USE_UNPROTECTED_COMMA_;
    AddNonfatalFailure();
  }, "");
}

#if GTEST_IS_THREADSAFE

typedef ScopedFakeTestPartResultReporterWithThreadsTest
    ExpectFailureWithThreadsTest;

TEST_F(ExpectFailureWithThreadsTest, ExpectFatalFailureOnAllThreads) {
  EXPECT_FATAL_FAILURE_ON_ALL_THREADS(AddFailureInOtherThread(FATAL_FAILURE),
                                      "Expected fatal failure.");
}

TEST_F(ExpectFailureWithThreadsTest, ExpectNonFatalFailureOnAllThreads) {
  EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS(
      AddFailureInOtherThread(NONFATAL_FAILURE), "Expected non-fatal failure.");
}

#endif  // GTEST_IS_THREADSAFE

// Tests the TestProperty class.

TEST(TestPropertyTest, ConstructorWorks) {
  const TestProperty property("key", "value");
  EXPECT_STREQ("key", property.key());
  EXPECT_STREQ("value", property.value());
}

TEST(TestPropertyTest, SetValue) {
  TestProperty property("key", "value_1");
  EXPECT_STREQ("key", property.key());
  property.SetValue("value_2");
  EXPECT_STREQ("key", property.key());
  EXPECT_STREQ("value_2", property.value());
}

// Tests the TestResult class

// The test fixture for testing TestResult.
class TestResultTest : public Test {
 protected:
  typedef std::vector<TestPartResult> TPRVector;

  // We make use of 2 TestPartResult objects,
  TestPartResult * pr1, * pr2;

  // ... and 3 TestResult objects.
  TestResult * r0, * r1, * r2;

  virtual void SetUp() {
    // pr1 is for success.
    pr1 = new TestPartResult(TestPartResult::kSuccess,
                             "foo/bar.cc",
                             10,
                             "Success!");

    // pr2 is for fatal failure.
    pr2 = new TestPartResult(TestPartResult::kFatalFailure,
                             "foo/bar.cc",
                             -1,  // This line number means "unknown"
                             "Failure!");

    // Creates the TestResult objects.
    r0 = new TestResult();
    r1 = new TestResult();
    r2 = new TestResult();

    // In order to test TestResult, we need to modify its internal
    // state, in particular the TestPartResult vector it holds.
    // test_part_results() returns a const reference to this vector.
    // We cast it to a non-const object s.t. it can be modified (yes,
    // this is a hack).
    TPRVector* results1 = const_cast<TPRVector*>(
        &TestResultAccessor::test_part_results(*r1));
    TPRVector* results2 = const_cast<TPRVector*>(
        &TestResultAccessor::test_part_results(*r2));

    // r0 is an empty TestResult.

    // r1 contains a single SUCCESS TestPartResult.
    results1->push_back(*pr1);

    // r2 contains a SUCCESS, and a FAILURE.
    results2->push_back(*pr1);
    results2->push_back(*pr2);
  }

  virtual void TearDown() {
    delete pr1;
    delete pr2;

    delete r0;
    delete r1;
    delete r2;
  }

  // Helper that compares two two TestPartResults.
  static void CompareTestPartResult(const TestPartResult& expected,
                                    const TestPartResult& actual) {
    EXPECT_EQ(expected.type(), actual.type());
    EXPECT_STREQ(expected.file_name(), actual.file_name());
    EXPECT_EQ(expected.line_number(), actual.line_number());
    EXPECT_STREQ(expected.summary(), actual.summary());
    EXPECT_STREQ(expected.message(), actual.message());
    EXPECT_EQ(expected.passed(), actual.passed());
    EXPECT_EQ(expected.failed(), actual.failed());
    EXPECT_EQ(expected.nonfatally_failed(), actual.nonfatally_failed());
    EXPECT_EQ(expected.fatally_failed(), actual.fatally_failed());
  }
};

// Tests TestResult::total_part_count().
TEST_F(TestResultTest, total_part_count) {
  ASSERT_EQ(0, r0->total_part_count());
  ASSERT_EQ(1, r1->total_part_count());
  ASSERT_EQ(2, r2->total_part_count());
}

// Tests TestResult::Passed().
TEST_F(TestResultTest, Passed) {
  ASSERT_TRUE(r0->Passed());
  ASSERT_TRUE(r1->Passed());
  ASSERT_FALSE(r2->Passed());
}

// Tests TestResult::Failed().
TEST_F(TestResultTest, Failed) {
  ASSERT_FALSE(r0->Failed());
  ASSERT_FALSE(r1->Failed());
  ASSERT_TRUE(r2->Failed());
}

// Tests TestResult::GetTestPartResult().

typedef TestResultTest TestResultDeathTest;

TEST_F(TestResultDeathTest, GetTestPartResult) {
  CompareTestPartResult(*pr1, r2->GetTestPartResult(0));
  CompareTestPartResult(*pr2, r2->GetTestPartResult(1));
  EXPECT_DEATH_IF_SUPPORTED(r2->GetTestPartResult(2), "");
  EXPECT_DEATH_IF_SUPPORTED(r2->GetTestPartResult(-1), "");
}

// Tests TestResult has no properties when none are added.
TEST(TestResultPropertyTest, NoPropertiesFoundWhenNoneAreAdded) {
  TestResult test_result;
  ASSERT_EQ(0, test_result.test_property_count());
}

// Tests TestResult has the expected property when added.
TEST(TestResultPropertyTest, OnePropertyFoundWhenAdded) {
  TestResult test_result;
  TestProperty property("key_1", "1");
  TestResultAccessor::RecordProperty(&test_result, "testcase", property);
  ASSERT_EQ(1, test_result.test_property_count());
  const TestProperty& actual_property = test_result.GetTestProperty(0);
  EXPECT_STREQ("key_1", actual_property.key());
  EXPECT_STREQ("1", actual_property.value());
}

// Tests TestResult has multiple properties when added.
TEST(TestResultPropertyTest, MultiplePropertiesFoundWhenAdded) {
  TestResult test_result;
  TestProperty property_1("key_1", "1");
  TestProperty property_2("key_2", "2");
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_1);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_2);
  ASSERT_EQ(2, test_result.test_property_count());
  const TestProperty& actual_property_1 = test_result.GetTestProperty(0);
  EXPECT_STREQ("key_1", actual_property_1.key());
  EXPECT_STREQ("1", actual_property_1.value());

  const TestProperty& actual_property_2 = test_result.GetTestProperty(1);
  EXPECT_STREQ("key_2", actual_property_2.key());
  EXPECT_STREQ("2", actual_property_2.value());
}

// Tests TestResult::RecordProperty() overrides values for duplicate keys.
TEST(TestResultPropertyTest, OverridesValuesForDuplicateKeys) {
  TestResult test_result;
  TestProperty property_1_1("key_1", "1");
  TestProperty property_2_1("key_2", "2");
  TestProperty property_1_2("key_1", "12");
  TestProperty property_2_2("key_2", "22");
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_1_1);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_2_1);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_1_2);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_2_2);

  ASSERT_EQ(2, test_result.test_property_count());
  const TestProperty& actual_property_1 = test_result.GetTestProperty(0);
  EXPECT_STREQ("key_1", actual_property_1.key());
  EXPECT_STREQ("12", actual_property_1.value());

  const TestProperty& actual_property_2 = test_result.GetTestProperty(1);
  EXPECT_STREQ("key_2", actual_property_2.key());
  EXPECT_STREQ("22", actual_property_2.value());
}

// Tests TestResult::GetTestProperty().
TEST(TestResultPropertyTest, GetTestProperty) {
  TestResult test_result;
  TestProperty property_1("key_1", "1");
  TestProperty property_2("key_2", "2");
  TestProperty property_3("key_3", "3");
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_1);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_2);
  TestResultAccessor::RecordProperty(&test_result, "testcase", property_3);

  const TestProperty& fetched_property_1 = test_result.GetTestProperty(0);
  const TestProperty& fetched_property_2 = test_result.GetTestProperty(1);
  const TestProperty& fetched_property_3 = test_result.GetTestProperty(2);

  EXPECT_STREQ("key_1", fetched_property_1.key());
  EXPECT_STREQ("1", fetched_property_1.value());

  EXPECT_STREQ("key_2", fetched_property_2.key());
  EXPECT_STREQ("2", fetched_property_2.value());

  EXPECT_STREQ("key_3", fetched_property_3.key());
  EXPECT_STREQ("3", fetched_property_3.value());

  EXPECT_DEATH_IF_SUPPORTED(test_result.GetTestProperty(3), "");
  EXPECT_DEATH_IF_SUPPORTED(test_result.GetTestProperty(-1), "");
}

// Tests the Test class.
//
// It's difficult to test every public method of this class (we are
// already stretching the limit of Google Test by using it to test itself!).
// Fortunately, we don't have to do that, as we are already testing
// the functionalities of the Test class extensively by using Google Test
// alone.
//
// Therefore, this section only contains one test.

// Tests that GTestFlagSaver works on Windows and Mac.

class GTestFlagSaverTest : public Test {
 protected:
  // Saves the Google Test flags such that we can restore them later, and
  // then sets them to their default values.  This will be called
  // before the first test in this test case is run.
  static void SetUpTestCase() {
    saver_ = new GTestFlagSaver;

    GTEST_FLAG(also_run_disabled_tests) = false;
    GTEST_FLAG(break_on_failure) = false;
    GTEST_FLAG(catch_exceptions) = false;
    GTEST_FLAG(death_test_use_fork) = false;
    GTEST_FLAG(color) = "auto";
    GTEST_FLAG(filter) = "";
    GTEST_FLAG(list_tests) = false;
    GTEST_FLAG(output) = "";
    GTEST_FLAG(print_time) = true;
    GTEST_FLAG(random_seed) = 0;
    GTEST_FLAG(repeat) = 1;
    GTEST_FLAG(shuffle) = false;
    GTEST_FLAG(stack_trace_depth) = kMaxStackTraceDepth;
    GTEST_FLAG(stream_result_to) = "";
    GTEST_FLAG(throw_on_failure) = false;
  }

  // Restores the Google Test flags that the tests have modified.  This will
  // be called after the last test in this test case is run.
  static void TearDownTestCase() {
    delete saver_;
    saver_ = NULL;
  }

  // Verifies that the Google Test flags have their default values, and then
  // modifies each of them.
  void VerifyAndModifyFlags() {
    EXPECT_FALSE(GTEST_FLAG(also_run_disabled_tests));
    EXPECT_FALSE(GTEST_FLAG(break_on_failure));
    EXPECT_FALSE(GTEST_FLAG(catch_exceptions));
    EXPECT_STREQ("auto", GTEST_FLAG(color).c_str());
    EXPECT_FALSE(GTEST_FLAG(death_test_use_fork));
    EXPECT_STREQ("", GTEST_FLAG(filter).c_str());
    EXPECT_FALSE(GTEST_FLAG(list_tests));
    EXPECT_STREQ("", GTEST_FLAG(output).c_str());
    EXPECT_TRUE(GTEST_FLAG(print_time));
    EXPECT_EQ(0, GTEST_FLAG(random_seed));
    EXPECT_EQ(1, GTEST_FLAG(repeat));
    EXPECT_FALSE(GTEST_FLAG(shuffle));
    EXPECT_EQ(kMaxStackTraceDepth, GTEST_FLAG(stack_trace_depth));
    EXPECT_STREQ("", GTEST_FLAG(stream_result_to).c_str());
    EXPECT_FALSE(GTEST_FLAG(throw_on_failure));

    GTEST_FLAG(also_run_disabled_tests) = true;
    GTEST_FLAG(break_on_failure) = true;
    GTEST_FLAG(catch_exceptions) = true;
    GTEST_FLAG(color) = "no";
    GTEST_FLAG(death_test_use_fork) = true;
    GTEST_FLAG(filter) = "abc";
    GTEST_FLAG(list_tests) = true;
    GTEST_FLAG(output) = "xml:foo.xml";
    GTEST_FLAG(print_time) = false;
    GTEST_FLAG(random_seed) = 1;
    GTEST_FLAG(repeat) = 100;
    GTEST_FLAG(shuffle) = true;
    GTEST_FLAG(stack_trace_depth) = 1;
    GTEST_FLAG(stream_result_to) = "localhost:1234";
    GTEST_FLAG(throw_on_failure) = true;
  }

 private:
  // For saving Google Test flags during this test case.
  static GTestFlagSaver* saver_;
};

GTestFlagSaver* GTestFlagSaverTest::saver_ = NULL;

// Google Test doesn't guarantee the order of tests.  The following two
// tests are designed to work regardless of their order.

// Modifies the Google Test flags in the test body.
TEST_F(GTestFlagSaverTest, ModifyGTestFlags) {
  VerifyAndModifyFlags();
}

// Verifies that the Google Test flags in the body of the previous test were
// restored to their original values.
TEST_F(GTestFlagSaverTest, VerifyGTestFlags) {
  VerifyAndModifyFlags();
}

// Sets an environment variable with the given name to the given
// value.  If the value argument is "", unsets the environment
// variable.  The caller must ensure that both arguments are not NULL.
static void SetEnv(const char* name, const char* value) {
#if GTEST_OS_WINDOWS_MOBILE
  // Environment variables are not supported on Windows CE.
  return;
#elif defined(__BORLANDC__) || defined(__SunOS_5_8) || defined(__SunOS_5_9)
  // C++Builder's putenv only stores a pointer to its parameter; we have to
  // ensure that the string remains valid as long as it might be needed.
  // We use an std::map to do so.
  static std::map<std::string, std::string*> added_env;

  // Because putenv stores a pointer to the string buffer, we can't delete the
  // previous string (if present) until after it's replaced.
  std::string *prev_env = NULL;
  if (added_env.find(name) != added_env.end()) {
    prev_env = added_env[name];
  }
  added_env[name] = new std::string(
      (Message() << name << "=" << value).GetString());

  // The standard signature of putenv accepts a 'char*' argument. Other
  // implementations, like C++Builder's, accept a 'const char*'.
  // We cast away the 'const' since that would work for both variants.
  putenv(const_cast<char*>(added_env[name]->c_str()));
  delete prev_env;
#elif GTEST_OS_WINDOWS  // If we are on Windows proper.
  _putenv((Message() << name << "=" << value).GetString().c_str());
#else
  if (*value == '\0') {
    unsetenv(name);
  } else {
    setenv(name, value, 1);
  }
#endif  // GTEST_OS_WINDOWS_MOBILE
}

#if !GTEST_OS_WINDOWS_MOBILE
// Environment variables are not supported on Windows CE.

using testing::internal::Int32FromGTestEnv;

// Tests Int32FromGTestEnv().

// Tests that Int32FromGTestEnv() returns the default value when the
// environment variable is not set.
TEST(Int32FromGTestEnvTest, ReturnsDefaultWhenVariableIsNotSet) {
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "");
  EXPECT_EQ(10, Int32FromGTestEnv("temp", 10));
}

# if !defined(GTEST_GET_INT32_FROM_ENV_)

// Tests that Int32FromGTestEnv() returns the default value when the
// environment variable overflows as an Int32.
TEST(Int32FromGTestEnvTest, ReturnsDefaultWhenValueOverflows) {
  printf("(expecting 2 warnings)\n");

  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "12345678987654321");
  EXPECT_EQ(20, Int32FromGTestEnv("temp", 20));

  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "-12345678987654321");
  EXPECT_EQ(30, Int32FromGTestEnv("temp", 30));
}

// Tests that Int32FromGTestEnv() returns the default value when the
// environment variable does not represent a valid decimal integer.
TEST(Int32FromGTestEnvTest, ReturnsDefaultWhenValueIsInvalid) {
  printf("(expecting 2 warnings)\n");

  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "A1");
  EXPECT_EQ(40, Int32FromGTestEnv("temp", 40));

  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "12X");
  EXPECT_EQ(50, Int32FromGTestEnv("temp", 50));
}

# endif  // !defined(GTEST_GET_INT32_FROM_ENV_)

// Tests that Int32FromGTestEnv() parses and returns the value of the
// environment variable when it represents a valid decimal integer in
// the range of an Int32.
TEST(Int32FromGTestEnvTest, ParsesAndReturnsValidValue) {
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "123");
  EXPECT_EQ(123, Int32FromGTestEnv("temp", 0));

  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "TEMP", "-321");
  EXPECT_EQ(-321, Int32FromGTestEnv("temp", 0));
}
#endif  // !GTEST_OS_WINDOWS_MOBILE

// Tests ParseInt32Flag().

// Tests that ParseInt32Flag() returns false and doesn't change the
// output value when the flag has wrong format
TEST(ParseInt32FlagTest, ReturnsFalseForInvalidFlag) {
  Int32 value = 123;
  EXPECT_FALSE(ParseInt32Flag("--a=100", "b", &value));
  EXPECT_EQ(123, value);

  EXPECT_FALSE(ParseInt32Flag("a=100", "a", &value));
  EXPECT_EQ(123, value);
}

// Tests that ParseInt32Flag() returns false and doesn't change the
// output value when the flag overflows as an Int32.
TEST(ParseInt32FlagTest, ReturnsDefaultWhenValueOverflows) {
  printf("(expecting 2 warnings)\n");

  Int32 value = 123;
  EXPECT_FALSE(ParseInt32Flag("--abc=12345678987654321", "abc", &value));
  EXPECT_EQ(123, value);

  EXPECT_FALSE(ParseInt32Flag("--abc=-12345678987654321", "abc", &value));
  EXPECT_EQ(123, value);
}

// Tests that ParseInt32Flag() returns false and doesn't change the
// output value when the flag does not represent a valid decimal
// integer.
TEST(ParseInt32FlagTest, ReturnsDefaultWhenValueIsInvalid) {
  printf("(expecting 2 warnings)\n");

  Int32 value = 123;
  EXPECT_FALSE(ParseInt32Flag("--abc=A1", "abc", &value));
  EXPECT_EQ(123, value);

  EXPECT_FALSE(ParseInt32Flag("--abc=12X", "abc", &value));
  EXPECT_EQ(123, value);
}

// Tests that ParseInt32Flag() parses the value of the flag and
// returns true when the flag represents a valid decimal integer in
// the range of an Int32.
TEST(ParseInt32FlagTest, ParsesAndReturnsValidValue) {
  Int32 value = 123;
  EXPECT_TRUE(ParseInt32Flag("--" GTEST_FLAG_PREFIX_ "abc=456", "abc", &value));
  EXPECT_EQ(456, value);

  EXPECT_TRUE(ParseInt32Flag("--" GTEST_FLAG_PREFIX_ "abc=-789",
                             "abc", &value));
  EXPECT_EQ(-789, value);
}

// Tests that Int32FromEnvOrDie() parses the value of the var or
// returns the correct default.
// Environment variables are not supported on Windows CE.
#if !GTEST_OS_WINDOWS_MOBILE
TEST(Int32FromEnvOrDieTest, ParsesAndReturnsValidValue) {
  EXPECT_EQ(333, Int32FromEnvOrDie(GTEST_FLAG_PREFIX_UPPER_ "UnsetVar", 333));
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "UnsetVar", "123");
  EXPECT_EQ(123, Int32FromEnvOrDie(GTEST_FLAG_PREFIX_UPPER_ "UnsetVar", 333));
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "UnsetVar", "-123");
  EXPECT_EQ(-123, Int32FromEnvOrDie(GTEST_FLAG_PREFIX_UPPER_ "UnsetVar", 333));
}
#endif  // !GTEST_OS_WINDOWS_MOBILE

// Tests that Int32FromEnvOrDie() aborts with an error message
// if the variable is not an Int32.
TEST(Int32FromEnvOrDieDeathTest, AbortsOnFailure) {
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "VAR", "xxx");
  EXPECT_DEATH_IF_SUPPORTED(
      Int32FromEnvOrDie(GTEST_FLAG_PREFIX_UPPER_ "VAR", 123),
      ".*");
}

// Tests that Int32FromEnvOrDie() aborts with an error message
// if the variable cannot be represnted by an Int32.
TEST(Int32FromEnvOrDieDeathTest, AbortsOnInt32Overflow) {
  SetEnv(GTEST_FLAG_PREFIX_UPPER_ "VAR", "1234567891234567891234");
  EXPECT_DEATH_IF_SUPPORTED(
      Int32FromEnvOrDie(GTEST_FLAG_PREFIX_UPPER_ "VAR", 123),
      ".*");
}

// Tests that ShouldRunTestOnShard() selects all tests
// where there is 1 shard.
TEST(ShouldRunTestOnShardTest, IsPartitionWhenThereIsOneShard) {
  EXPECT_TRUE(ShouldRunTestOnShard(1, 0, 0));
  EXPECT_TRUE(ShouldRunTestOnShard(1, 0, 1));
  EXPECT_TRUE(ShouldRunTestOnShard(1, 0, 2));
  EXPECT_TRUE(ShouldRunTestOnShard(1, 0, 3));
  EXPECT_TRUE(ShouldRunTestOnShard(1, 0, 4));
}

class ShouldShardTest : public testing::Test {
 protected:
  virtual void SetUp() {
    index_var_ = GTEST_FLAG_PREFIX_UPPER_ "INDEX";
    total_var_ = GTEST_FLAG_PREFIX_UPPER_ "TOTAL";
  }

  virtual void TearDown() {
    SetEnv(index_var_, "");
    SetEnv(total_var_, "");
  }

  const char* index_var_;
  const char* total_var_;
};

// Tests that sharding is disabled if neither of the environment variables
// are set.
TEST_F(ShouldShardTest, ReturnsFalseWhenNeitherEnvVarIsSet) {
  SetEnv(index_var_, "");
  SetEnv(total_var_, "");

  EXPECT_FALSE(ShouldShard(total_var_, index_var_, false));
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, true));
}

// Tests that sharding is not enabled if total_shards  == 1.
TEST_F(ShouldShardTest, ReturnsFalseWhenTotalShardIsOne) {
  SetEnv(index_var_, "0");
  SetEnv(total_var_, "1");
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, false));
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, true));
}

// Tests that sharding is enabled if total_shards > 1 and
// we are not in a death test subprocess.
// Environment variables are not supported on Windows CE.
#if !GTEST_OS_WINDOWS_MOBILE
TEST_F(ShouldShardTest, WorksWhenShardEnvVarsAreValid) {
  SetEnv(index_var_, "4");
  SetEnv(total_var_, "22");
  EXPECT_TRUE(ShouldShard(total_var_, index_var_, false));
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, true));

  SetEnv(index_var_, "8");
  SetEnv(total_var_, "9");
  EXPECT_TRUE(ShouldShard(total_var_, index_var_, false));
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, true));

  SetEnv(index_var_, "0");
  SetEnv(total_var_, "9");
  EXPECT_TRUE(ShouldShard(total_var_, index_var_, false));
  EXPECT_FALSE(ShouldShard(total_var_, index_var_, true));
}
#endif  // !GTEST_OS_WINDOWS_MOBILE

// Tests that we exit in error if the sharding values are not valid.

typedef ShouldShardTest ShouldShardDeathTest;

TEST_F(ShouldShardDeathTest, AbortsWhenShardingEnvVarsAreInvalid) {
  SetEnv(index_var_, "4");
  SetEnv(total_var_, "4");
  EXPECT_DEATH_IF_SUPPORTED(ShouldShard(total_var_, index_var_, false), ".*");

  SetEnv(index_var_, "4");
  SetEnv(total_var_, "-2");
  EXPECT_DEATH_IF_SUPPORTED(ShouldShard(total_var_, index_var_, false), ".*");

  SetEnv(index_var_, "5");
  SetEnv(total_var_, "");
  EXPECT_DEATH_IF_SUPPORTED(ShouldShard(total_var_, index_var_, false), ".*");

  SetEnv(index_var_, "");
  SetEnv(total_var_, "5");
  EXPECT_DEATH_IF_SUPPORTED(ShouldShard(total_var_, index_var_, false), ".*");
}

// Tests that ShouldRunTestOnShard is a partition when 5
// shards are used.
TEST(ShouldRunTestOnShardTest, IsPartitionWhenThereAreFiveShards) {
  // Choose an arbitrary number of tests and shards.
  const int num_tests = 17;
  const int num_shards = 5;

  // Check partitioning: each test should be on exactly 1 shard.
  for (int test_id = 0; test_id < num_tests; test_id++) {
    int prev_selected_shard_index = -1;
    for (int shard_index = 0; shard_index < num_shards; shard_index++) {
      if (ShouldRunTestOnShard(num_shards, shard_index, test_id)) {
        if (prev_selected_shard_index < 0) {
          prev_selected_shard_index = shard_index;
        } else {
          ADD_FAILURE() << "Shard " << prev_selected_shard_index << " and "
            << shard_index << " are both selected to run test " << test_id;
        }
      }
    }
  }

  // Check balance: This is not required by the sharding protocol, but is a
  // desirable property for performance.
  for (int shard_index = 0; shard_index < num_shards; shard_index++) {
    int num_tests_on_shard = 0;
    for (int test_id = 0; test_id < num_tests; test_id++) {
      num_tests_on_shard +=
        ShouldRunTestOnShard(num_shards, shard_index, test_id);
    }
    EXPECT_GE(num_tests_on_shard, num_tests / num_shards);
  }
}

// For the same reason we are not explicitly testing everything in the
// Test class, there are no separate tests for the following classes
// (except for some trivial cases):
//
//   TestCase, UnitTest, UnitTestResultPrinter.
//
// Similarly, there are no separate tests for the following macros:
//
//   TEST, TEST_F, RUN_ALL_TESTS

TEST(UnitTestTest, CanGetOriginalWorkingDir) {
  ASSERT_TRUE(UnitTest::GetInstance()->original_working_dir() != NULL);
  EXPECT_STRNE(UnitTest::GetInstance()->original_working_dir(), "");
}

TEST(UnitTestTest, ReturnsPlausibleTimestamp) {
  EXPECT_LT(0, UnitTest::GetInstance()->start_timestamp());
  EXPECT_LE(UnitTest::GetInstance()->start_timestamp(), GetTimeInMillis());
}

// When a property using a reserved key is supplied to this function, it
// tests that a non-fatal failure is added, a fatal failure is not added,
// and that the property is not recorded.
void ExpectNonFatalFailureRecordingPropertyWithReservedKey(
    const TestResult& test_result, const char* key) {
  EXPECT_NONFATAL_FAILURE(Test::RecordProperty(key, "1"), "Reserved key");
  ASSERT_EQ(0, test_result.test_property_count()) << "Property for key '" << key
                                                  << "' recorded unexpectedly.";
}

void ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
    const char* key) {
  const TestInfo* test_info = UnitTest::GetInstance()->current_test_info();
  ASSERT_TRUE(test_info != NULL);
  ExpectNonFatalFailureRecordingPropertyWithReservedKey(*test_info->result(),
                                                        key);
}

void ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
    const char* key) {
  const TestCase* test_case = UnitTest::GetInstance()->current_test_case();
  ASSERT_TRUE(test_case != NULL);
  ExpectNonFatalFailureRecordingPropertyWithReservedKey(
      test_case->ad_hoc_test_result(), key);
}

void ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
    const char* key) {
  ExpectNonFatalFailureRecordingPropertyWithReservedKey(
      UnitTest::GetInstance()->ad_hoc_test_result(), key);
}

// Tests that property recording functions in UnitTest outside of tests
// functions correcly.  Creating a separate instance of UnitTest ensures it
// is in a state similar to the UnitTest's singleton's between tests.
class UnitTestRecordPropertyTest :
    public testing::internal::UnitTestRecordPropertyTestHelper {
 public:
  static void SetUpTestCase() {
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "disabled");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "errors");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "failures");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "name");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "tests");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTestCase(
        "time");

    Test::RecordProperty("test_case_key_1", "1");
    const TestCase* test_case = UnitTest::GetInstance()->current_test_case();
    ASSERT_TRUE(test_case != NULL);

    ASSERT_EQ(1, test_case->ad_hoc_test_result().test_property_count());
    EXPECT_STREQ("test_case_key_1",
                 test_case->ad_hoc_test_result().GetTestProperty(0).key());
    EXPECT_STREQ("1",
                 test_case->ad_hoc_test_result().GetTestProperty(0).value());
  }
};

// Tests TestResult has the expected property when added.
TEST_F(UnitTestRecordPropertyTest, OnePropertyFoundWhenAdded) {
  UnitTestRecordProperty("key_1", "1");

  ASSERT_EQ(1, unit_test_.ad_hoc_test_result().test_property_count());

  EXPECT_STREQ("key_1",
               unit_test_.ad_hoc_test_result().GetTestProperty(0).key());
  EXPECT_STREQ("1",
               unit_test_.ad_hoc_test_result().GetTestProperty(0).value());
}

// Tests TestResult has multiple properties when added.
TEST_F(UnitTestRecordPropertyTest, MultiplePropertiesFoundWhenAdded) {
  UnitTestRecordProperty("key_1", "1");
  UnitTestRecordProperty("key_2", "2");

  ASSERT_EQ(2, unit_test_.ad_hoc_test_result().test_property_count());

  EXPECT_STREQ("key_1",
               unit_test_.ad_hoc_test_result().GetTestProperty(0).key());
  EXPECT_STREQ("1", unit_test_.ad_hoc_test_result().GetTestProperty(0).value());

  EXPECT_STREQ("key_2",
               unit_test_.ad_hoc_test_result().GetTestProperty(1).key());
  EXPECT_STREQ("2", unit_test_.ad_hoc_test_result().GetTestProperty(1).value());
}

// Tests TestResult::RecordProperty() overrides values for duplicate keys.
TEST_F(UnitTestRecordPropertyTest, OverridesValuesForDuplicateKeys) {
  UnitTestRecordProperty("key_1", "1");
  UnitTestRecordProperty("key_2", "2");
  UnitTestRecordProperty("key_1", "12");
  UnitTestRecordProperty("key_2", "22");

  ASSERT_EQ(2, unit_test_.ad_hoc_test_result().test_property_count());

  EXPECT_STREQ("key_1",
               unit_test_.ad_hoc_test_result().GetTestProperty(0).key());
  EXPECT_STREQ("12",
               unit_test_.ad_hoc_test_result().GetTestProperty(0).value());

  EXPECT_STREQ("key_2",
               unit_test_.ad_hoc_test_result().GetTestProperty(1).key());
  EXPECT_STREQ("22",
               unit_test_.ad_hoc_test_result().GetTestProperty(1).value());
}

TEST_F(UnitTestRecordPropertyTest,
       AddFailureInsideTestsWhenUsingTestCaseReservedKeys) {
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "name");
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "value_param");
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "type_param");
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "status");
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "time");
  ExpectNonFatalFailureRecordingPropertyWithReservedKeyForCurrentTest(
      "classname");
}

TEST_F(UnitTestRecordPropertyTest,
       AddRecordWithReservedKeysGeneratesCorrectPropertyList) {
  EXPECT_NONFATAL_FAILURE(
      Test::RecordProperty("name", "1"),
      "'classname', 'name', 'status', 'time', 'type_param', and 'value_param'"
      " are reserved");
}

class UnitTestRecordPropertyTestEnvironment : public Environment {
 public:
  virtual void TearDown() {
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "tests");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "failures");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "disabled");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "errors");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "name");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "timestamp");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "time");
    ExpectNonFatalFailureRecordingPropertyWithReservedKeyOutsideOfTestCase(
        "random_seed");
  }
};

// This will test property recording outside of any test or test case.
static Environment* record_property_env =
    AddGlobalTestEnvironment(new UnitTestRecordPropertyTestEnvironment);

// This group of tests is for predicate assertions (ASSERT_PRED*, etc)
// of various arities.  They do not attempt to be exhaustive.  Rather,
// view them as smoke tests that can be easily reviewed and verified.
// A more complete set of tests for predicate assertions can be found
// in gtest_pred_impl_unittest.cc.

// First, some predicates and predicate-formatters needed by the tests.

// Returns true iff the argument is an even number.
bool IsEven(int n) {
  return (n % 2) == 0;
}

// A functor that returns true iff the argument is an even number.
struct IsEvenFunctor {
  bool operator()(int n) { return IsEven(n); }
};

// A predicate-formatter function that asserts the argument is an even
// number.
AssertionResult AssertIsEven(const char* expr, int n) {
  if (IsEven(n)) {
    return AssertionSuccess();
  }

  Message msg;
  msg << expr << " evaluates to " << n << ", which is not even.";
  return AssertionFailure(msg);
}

// A predicate function that returns AssertionResult for use in
// EXPECT/ASSERT_TRUE/FALSE.
AssertionResult ResultIsEven(int n) {
  if (IsEven(n))
    return AssertionSuccess() << n << " is even";
  else
    return AssertionFailure() << n << " is odd";
}

// A predicate function that returns AssertionResult but gives no
// explanation why it succeeds. Needed for testing that
// EXPECT/ASSERT_FALSE handles such functions correctly.
AssertionResult ResultIsEvenNoExplanation(int n) {
  if (IsEven(n))
    return AssertionSuccess();
  else
    return AssertionFailure() << n << " is odd";
}

// A predicate-formatter functor that asserts the argument is an even
// number.
struct AssertIsEvenFunctor {
  AssertionResult operator()(const char* expr, int n) {
    return AssertIsEven(expr, n);
  }
};

// Returns true iff the sum of the arguments is an even number.
bool SumIsEven2(int n1, int n2) {
  return IsEven(n1 + n2);
}

// A functor that returns true iff the sum of the arguments is an even
// number.
struct SumIsEven3Functor {
  bool operator()(int n1, int n2, int n3) {
    return IsEven(n1 + n2 + n3);
  }
};

// A predicate-formatter function that asserts the sum of the
// arguments is an even number.
AssertionResult AssertSumIsEven4(
    const char* e1, const char* e2, const char* e3, const char* e4,
    int n1, int n2, int n3, int n4) {
  const int sum = n1 + n2 + n3 + n4;
  if (IsEven(sum)) {
    return AssertionSuccess();
  }

  Message msg;
  msg << e1 << " + " << e2 << " + " << e3 << " + " << e4
      << " (" << n1 << " + " << n2 << " + " << n3 << " + " << n4
      << ") evaluates to " << sum << ", which is not even.";
  return AssertionFailure(msg);
}

// A predicate-formatter functor that asserts the sum of the arguments
// is an even number.
struct AssertSumIsEven5Functor {
  AssertionResult operator()(
      const char* e1, const char* e2, const char* e3, const char* e4,
      const char* e5, int n1, int n2, int n3, int n4, int n5) {
    const int sum = n1 + n2 + n3 + n4 + n5;
    if (IsEven(sum)) {
      return AssertionSuccess();
    }

    Message msg;
    msg << e1 << " + " << e2 << " + " << e3 << " + " << e4 << " + " << e5
        << " ("
        << n1 << " + " << n2 << " + " << n3 << " + " << n4 << " + " << n5
        << ") evaluates to " << sum << ", which is not even.";
    return AssertionFailure(msg);
  }
};


// Tests unary predicate assertions.

// Tests unary predicate assertions that don't use a custom formatter.
TEST(Pred1Test, WithoutFormat) {
  // Success cases.
  EXPECT_PRED1(IsEvenFunctor(), 2) << "This failure is UNEXPECTED!";
  ASSERT_PRED1(IsEven, 4);

  // Failure cases.
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED1(IsEven, 5) << "This failure is expected.";
  }, "This failure is expected.");
  EXPECT_FATAL_FAILURE(ASSERT_PRED1(IsEvenFunctor(), 5),
                       "evaluates to false");
}

// Tests unary predicate assertions that use a custom formatter.
TEST(Pred1Test, WithFormat) {
  // Success cases.
  EXPECT_PRED_FORMAT1(AssertIsEven, 2);
  ASSERT_PRED_FORMAT1(AssertIsEvenFunctor(), 4)
    << "This failure is UNEXPECTED!";

  // Failure cases.
  const int n = 5;
  EXPECT_NONFATAL_FAILURE(EXPECT_PRED_FORMAT1(AssertIsEvenFunctor(), n),
                          "n evaluates to 5, which is not even.");
  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_PRED_FORMAT1(AssertIsEven, 5) << "This failure is expected.";
  }, "This failure is expected.");
}

// Tests that unary predicate assertions evaluates their arguments
// exactly once.
TEST(Pred1Test, SingleEvaluationOnFailure) {
  // A success case.
  static int n = 0;
  EXPECT_PRED1(IsEven, n++);
  EXPECT_EQ(1, n) << "The argument is not evaluated exactly once.";

  // A failure case.
  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_PRED_FORMAT1(AssertIsEvenFunctor(), n++)
        << "This failure is expected.";
  }, "This failure is expected.");
  EXPECT_EQ(2, n) << "The argument is not evaluated exactly once.";
}


// Tests predicate assertions whose arity is >= 2.

// Tests predicate assertions that don't use a custom formatter.
TEST(PredTest, WithoutFormat) {
  // Success cases.
  ASSERT_PRED2(SumIsEven2, 2, 4) << "This failure is UNEXPECTED!";
  EXPECT_PRED3(SumIsEven3Functor(), 4, 6, 8);

  // Failure cases.
  const int n1 = 1;
  const int n2 = 2;
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED2(SumIsEven2, n1, n2) << "This failure is expected.";
  }, "This failure is expected.");
  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_PRED3(SumIsEven3Functor(), 1, 2, 4);
  }, "evaluates to false");
}

// Tests predicate assertions that use a custom formatter.
TEST(PredTest, WithFormat) {
  // Success cases.
  ASSERT_PRED_FORMAT4(AssertSumIsEven4, 4, 6, 8, 10) <<
    "This failure is UNEXPECTED!";
  EXPECT_PRED_FORMAT5(AssertSumIsEven5Functor(), 2, 4, 6, 8, 10);

  // Failure cases.
  const int n1 = 1;
  const int n2 = 2;
  const int n3 = 4;
  const int n4 = 6;
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED_FORMAT4(AssertSumIsEven4, n1, n2, n3, n4);
  }, "evaluates to 13, which is not even.");
  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_PRED_FORMAT5(AssertSumIsEven5Functor(), 1, 2, 4, 6, 8)
        << "This failure is expected.";
  }, "This failure is expected.");
}

// Tests that predicate assertions evaluates their arguments
// exactly once.
TEST(PredTest, SingleEvaluationOnFailure) {
  // A success case.
  int n1 = 0;
  int n2 = 0;
  EXPECT_PRED2(SumIsEven2, n1++, n2++);
  EXPECT_EQ(1, n1) << "Argument 1 is not evaluated exactly once.";
  EXPECT_EQ(1, n2) << "Argument 2 is not evaluated exactly once.";

  // Another success case.
  n1 = n2 = 0;
  int n3 = 0;
  int n4 = 0;
  int n5 = 0;
  ASSERT_PRED_FORMAT5(AssertSumIsEven5Functor(),
                      n1++, n2++, n3++, n4++, n5++)
                        << "This failure is UNEXPECTED!";
  EXPECT_EQ(1, n1) << "Argument 1 is not evaluated exactly once.";
  EXPECT_EQ(1, n2) << "Argument 2 is not evaluated exactly once.";
  EXPECT_EQ(1, n3) << "Argument 3 is not evaluated exactly once.";
  EXPECT_EQ(1, n4) << "Argument 4 is not evaluated exactly once.";
  EXPECT_EQ(1, n5) << "Argument 5 is not evaluated exactly once.";

  // A failure case.
  n1 = n2 = n3 = 0;
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED3(SumIsEven3Functor(), ++n1, n2++, n3++)
        << "This failure is expected.";
  }, "This failure is expected.");
  EXPECT_EQ(1, n1) << "Argument 1 is not evaluated exactly once.";
  EXPECT_EQ(1, n2) << "Argument 2 is not evaluated exactly once.";
  EXPECT_EQ(1, n3) << "Argument 3 is not evaluated exactly once.";

  // Another failure case.
  n1 = n2 = n3 = n4 = 0;
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED_FORMAT4(AssertSumIsEven4, ++n1, n2++, n3++, n4++);
  }, "evaluates to 1, which is not even.");
  EXPECT_EQ(1, n1) << "Argument 1 is not evaluated exactly once.";
  EXPECT_EQ(1, n2) << "Argument 2 is not evaluated exactly once.";
  EXPECT_EQ(1, n3) << "Argument 3 is not evaluated exactly once.";
  EXPECT_EQ(1, n4) << "Argument 4 is not evaluated exactly once.";
}


// Some helper functions for testing using overloaded/template
// functions with ASSERT_PREDn and EXPECT_PREDn.

bool IsPositive(double x) {
  return x > 0;
}

template <typename T>
bool IsNegative(T x) {
  return x < 0;
}

template <typename T1, typename T2>
bool GreaterThan(T1 x1, T2 x2) {
  return x1 > x2;
}

// Tests that overloaded functions can be used in *_PRED* as long as
// their types are explicitly specified.
TEST(PredicateAssertionTest, AcceptsOverloadedFunction) {
  // C++Builder requires C-style casts rather than static_cast.
  EXPECT_PRED1((bool (*)(int))(IsPositive), 5);  // NOLINT
  ASSERT_PRED1((bool (*)(double))(IsPositive), 6.0);  // NOLINT
}

// Tests that template functions can be used in *_PRED* as long as
// their types are explicitly specified.
TEST(PredicateAssertionTest, AcceptsTemplateFunction) {
  EXPECT_PRED1(IsNegative<int>, -5);
  // Makes sure that we can handle templates with more than one
  // parameter.
  ASSERT_PRED2((GreaterThan<int, int>), 5, 0);
}


// Some helper functions for testing using overloaded/template
// functions with ASSERT_PRED_FORMATn and EXPECT_PRED_FORMATn.

AssertionResult IsPositiveFormat(const char* /* expr */, int n) {
  return n > 0 ? AssertionSuccess() :
      AssertionFailure(Message() << "Failure");
}

AssertionResult IsPositiveFormat(const char* /* expr */, double x) {
  return x > 0 ? AssertionSuccess() :
      AssertionFailure(Message() << "Failure");
}

template <typename T>
AssertionResult IsNegativeFormat(const char* /* expr */, T x) {
  return x < 0 ? AssertionSuccess() :
      AssertionFailure(Message() << "Failure");
}

template <typename T1, typename T2>
AssertionResult EqualsFormat(const char* /* expr1 */, const char* /* expr2 */,
                             const T1& x1, const T2& x2) {
  return x1 == x2 ? AssertionSuccess() :
      AssertionFailure(Message() << "Failure");
}

// Tests that overloaded functions can be used in *_PRED_FORMAT*
// without explicitly specifying their types.
TEST(PredicateFormatAssertionTest, AcceptsOverloadedFunction) {
  EXPECT_PRED_FORMAT1(IsPositiveFormat, 5);
  ASSERT_PRED_FORMAT1(IsPositiveFormat, 6.0);
}

// Tests that template functions can be used in *_PRED_FORMAT* without
// explicitly specifying their types.
TEST(PredicateFormatAssertionTest, AcceptsTemplateFunction) {
  EXPECT_PRED_FORMAT1(IsNegativeFormat, -5);
  ASSERT_PRED_FORMAT2(EqualsFormat, 3, 3);
}


// Tests string assertions.

// Tests ASSERT_STREQ with non-NULL arguments.
TEST(StringAssertionTest, ASSERT_STREQ) {
  const char * const p1 = "good";
  ASSERT_STREQ(p1, p1);

  // Let p2 have the same content as p1, but be at a different address.
  const char p2[] = "good";
  ASSERT_STREQ(p1, p2);

  EXPECT_FATAL_FAILURE(ASSERT_STREQ("bad", "good"),
                       "Expected: \"bad\"");
}

// Tests ASSERT_STREQ with NULL arguments.
TEST(StringAssertionTest, ASSERT_STREQ_Null) {
  ASSERT_STREQ(static_cast<const char *>(NULL), NULL);
  EXPECT_FATAL_FAILURE(ASSERT_STREQ(NULL, "non-null"),
                       "non-null");
}

// Tests ASSERT_STREQ with NULL arguments.
TEST(StringAssertionTest, ASSERT_STREQ_Null2) {
  EXPECT_FATAL_FAILURE(ASSERT_STREQ("non-null", NULL),
                       "non-null");
}

// Tests ASSERT_STRNE.
TEST(StringAssertionTest, ASSERT_STRNE) {
  ASSERT_STRNE("hi", "Hi");
  ASSERT_STRNE("Hi", NULL);
  ASSERT_STRNE(NULL, "Hi");
  ASSERT_STRNE("", NULL);
  ASSERT_STRNE(NULL, "");
  ASSERT_STRNE("", "Hi");
  ASSERT_STRNE("Hi", "");
  EXPECT_FATAL_FAILURE(ASSERT_STRNE("Hi", "Hi"),
                       "\"Hi\" vs \"Hi\"");
}

// Tests ASSERT_STRCASEEQ.
TEST(StringAssertionTest, ASSERT_STRCASEEQ) {
  ASSERT_STRCASEEQ("hi", "Hi");
  ASSERT_STRCASEEQ(static_cast<const char *>(NULL), NULL);

  ASSERT_STRCASEEQ("", "");
  EXPECT_FATAL_FAILURE(ASSERT_STRCASEEQ("Hi", "hi2"),
                       "Ignoring case");
}

// Tests ASSERT_STRCASENE.
TEST(StringAssertionTest, ASSERT_STRCASENE) {
  ASSERT_STRCASENE("hi1", "Hi2");
  ASSERT_STRCASENE("Hi", NULL);
  ASSERT_STRCASENE(NULL, "Hi");
  ASSERT_STRCASENE("", NULL);
  ASSERT_STRCASENE(NULL, "");
  ASSERT_STRCASENE("", "Hi");
  ASSERT_STRCASENE("Hi", "");
  EXPECT_FATAL_FAILURE(ASSERT_STRCASENE("Hi", "hi"),
                       "(ignoring case)");
}

// Tests *_STREQ on wide strings.
TEST(StringAssertionTest, STREQ_Wide) {
  // NULL strings.
  ASSERT_STREQ(static_cast<const wchar_t *>(NULL), NULL);

  // Empty strings.
  ASSERT_STREQ(L"", L"");

  // Non-null vs NULL.
  EXPECT_NONFATAL_FAILURE(EXPECT_STREQ(L"non-null", NULL),
                          "non-null");

  // Equal strings.
  EXPECT_STREQ(L"Hi", L"Hi");

  // Unequal strings.
  EXPECT_NONFATAL_FAILURE(EXPECT_STREQ(L"abc", L"Abc"),
                          "Abc");

  // Strings containing wide characters.
  EXPECT_NONFATAL_FAILURE(EXPECT_STREQ(L"abc\x8119", L"abc\x8120"),
                          "abc");

  // The streaming variation.
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_STREQ(L"abc\x8119", L"abc\x8121") << "Expected failure";
  }, "Expected failure");
}

// Tests *_STRNE on wide strings.
TEST(StringAssertionTest, STRNE_Wide) {
  // NULL strings.
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_STRNE(static_cast<const wchar_t *>(NULL), NULL);
  }, "");

  // Empty strings.
  EXPECT_NONFATAL_FAILURE(EXPECT_STRNE(L"", L""),
                          "L\"\"");

  // Non-null vs NULL.
  ASSERT_STRNE(L"non-null", NULL);

  // Equal strings.
  EXPECT_NONFATAL_FAILURE(EXPECT_STRNE(L"Hi", L"Hi"),
                          "L\"Hi\"");

  // Unequal strings.
  EXPECT_STRNE(L"abc", L"Abc");

  // Strings containing wide characters.
  EXPECT_NONFATAL_FAILURE(EXPECT_STRNE(L"abc\x8119", L"abc\x8119"),
                          "abc");

  // The streaming variation.
  ASSERT_STRNE(L"abc\x8119", L"abc\x8120") << "This shouldn't happen";
}

// Tests for ::testing::IsSubstring().

// Tests that IsSubstring() returns the correct result when the input
// argument type is const char*.
TEST(IsSubstringTest, ReturnsCorrectResultForCString) {
  EXPECT_FALSE(IsSubstring("", "", NULL, "a"));
  EXPECT_FALSE(IsSubstring("", "", "b", NULL));
  EXPECT_FALSE(IsSubstring("", "", "needle", "haystack"));

  EXPECT_TRUE(IsSubstring("", "", static_cast<const char*>(NULL), NULL));
  EXPECT_TRUE(IsSubstring("", "", "needle", "two needles"));
}

// Tests that IsSubstring() returns the correct result when the input
// argument type is const wchar_t*.
TEST(IsSubstringTest, ReturnsCorrectResultForWideCString) {
  EXPECT_FALSE(IsSubstring("", "", kNull, L"a"));
  EXPECT_FALSE(IsSubstring("", "", L"b", kNull));
  EXPECT_FALSE(IsSubstring("", "", L"needle", L"haystack"));

  EXPECT_TRUE(IsSubstring("", "", static_cast<const wchar_t*>(NULL), NULL));
  EXPECT_TRUE(IsSubstring("", "", L"needle", L"two needles"));
}

// Tests that IsSubstring() generates the correct message when the input
// argument type is const char*.
TEST(IsSubstringTest, GeneratesCorrectMessageForCString) {
  EXPECT_STREQ("Value of: needle_expr\n"
               "  Actual: \"needle\"\n"
               "Expected: a substring of haystack_expr\n"
               "Which is: \"haystack\"",
               IsSubstring("needle_expr", "haystack_expr",
                           "needle", "haystack").failure_message());
}

// Tests that IsSubstring returns the correct result when the input
// argument type is ::std::string.
TEST(IsSubstringTest, ReturnsCorrectResultsForStdString) {
  EXPECT_TRUE(IsSubstring("", "", std::string("hello"), "ahellob"));
  EXPECT_FALSE(IsSubstring("", "", "hello", std::string("world")));
}

#if GTEST_HAS_STD_WSTRING
// Tests that IsSubstring returns the correct result when the input
// argument type is ::std::wstring.
TEST(IsSubstringTest, ReturnsCorrectResultForStdWstring) {
  EXPECT_TRUE(IsSubstring("", "", ::std::wstring(L"needle"), L"two needles"));
  EXPECT_FALSE(IsSubstring("", "", L"needle", ::std::wstring(L"haystack")));
}

// Tests that IsSubstring() generates the correct message when the input
// argument type is ::std::wstring.
TEST(IsSubstringTest, GeneratesCorrectMessageForWstring) {
  EXPECT_STREQ("Value of: needle_expr\n"
               "  Actual: L\"needle\"\n"
               "Expected: a substring of haystack_expr\n"
               "Which is: L\"haystack\"",
               IsSubstring(
                   "needle_expr", "haystack_expr",
                   ::std::wstring(L"needle"), L"haystack").failure_message());
}

#endif  // GTEST_HAS_STD_WSTRING

// Tests for ::testing::IsNotSubstring().

// Tests that IsNotSubstring() returns the correct result when the input
// argument type is const char*.
TEST(IsNotSubstringTest, ReturnsCorrectResultForCString) {
  EXPECT_TRUE(IsNotSubstring("", "", "needle", "haystack"));
  EXPECT_FALSE(IsNotSubstring("", "", "needle", "two needles"));
}

// Tests that IsNotSubstring() returns the correct result when the input
// argument type is const wchar_t*.
TEST(IsNotSubstringTest, ReturnsCorrectResultForWideCString) {
  EXPECT_TRUE(IsNotSubstring("", "", L"needle", L"haystack"));
  EXPECT_FALSE(IsNotSubstring("", "", L"needle", L"two needles"));
}

// Tests that IsNotSubstring() generates the correct message when the input
// argument type is const wchar_t*.
TEST(IsNotSubstringTest, GeneratesCorrectMessageForWideCString) {
  EXPECT_STREQ("Value of: needle_expr\n"
               "  Actual: L\"needle\"\n"
               "Expected: not a substring of haystack_expr\n"
               "Which is: L\"two needles\"",
               IsNotSubstring(
                   "needle_expr", "haystack_expr",
                   L"needle", L"two needles").failure_message());
}

// Tests that IsNotSubstring returns the correct result when the input
// argument type is ::std::string.
TEST(IsNotSubstringTest, ReturnsCorrectResultsForStdString) {
  EXPECT_FALSE(IsNotSubstring("", "", std::string("hello"), "ahellob"));
  EXPECT_TRUE(IsNotSubstring("", "", "hello", std::string("world")));
}

// Tests that IsNotSubstring() generates the correct message when the input
// argument type is ::std::string.
TEST(IsNotSubstringTest, GeneratesCorrectMessageForStdString) {
  EXPECT_STREQ("Value of: needle_expr\n"
               "  Actual: \"needle\"\n"
               "Expected: not a substring of haystack_expr\n"
               "Which is: \"two needles\"",
               IsNotSubstring(
                   "needle_expr", "haystack_expr",
                   ::std::string("needle"), "two needles").failure_message());
}

#if GTEST_HAS_STD_WSTRING

// Tests that IsNotSubstring returns the correct result when the input
// argument type is ::std::wstring.
TEST(IsNotSubstringTest, ReturnsCorrectResultForStdWstring) {
  EXPECT_FALSE(
      IsNotSubstring("", "", ::std::wstring(L"needle"), L"two needles"));
  EXPECT_TRUE(IsNotSubstring("", "", L"needle", ::std::wstring(L"haystack")));
}

#endif  // GTEST_HAS_STD_WSTRING

// Tests floating-point assertions.

template <typename RawType>
class FloatingPointTest : public Test {
 protected:
  // Pre-calculated numbers to be used by the tests.
  struct TestValues {
    RawType close_to_positive_zero;
    RawType close_to_negative_zero;
    RawType further_from_negative_zero;

    RawType close_to_one;
    RawType further_from_one;

    RawType infinity;
    RawType close_to_infinity;
    RawType further_from_infinity;

    RawType nan1;
    RawType nan2;
  };

  typedef typename testing::internal::FloatingPoint<RawType> Floating;
  typedef typename Floating::Bits Bits;

  virtual void SetUp() {
    const size_t max_ulps = Floating::kMaxUlps;

    // The bits that represent 0.0.
    const Bits zero_bits = Floating(0).bits();

    // Makes some numbers close to 0.0.
    values_.close_to_positive_zero = Floating::ReinterpretBits(
        zero_bits + max_ulps/2);
    values_.close_to_negative_zero = -Floating::ReinterpretBits(
        zero_bits + max_ulps - max_ulps/2);
    values_.further_from_negative_zero = -Floating::ReinterpretBits(
        zero_bits + max_ulps + 1 - max_ulps/2);

    // The bits that represent 1.0.
    const Bits one_bits = Floating(1).bits();

    // Makes some numbers close to 1.0.
    values_.close_to_one = Floating::ReinterpretBits(one_bits + max_ulps);
    values_.further_from_one = Floating::ReinterpretBits(
        one_bits + max_ulps + 1);

    // +infinity.
    values_.infinity = Floating::Infinity();

    // The bits that represent +infinity.
    const Bits infinity_bits = Floating(values_.infinity).bits();

    // Makes some numbers close to infinity.
    values_.close_to_infinity = Floating::ReinterpretBits(
        infinity_bits - max_ulps);
    values_.further_from_infinity = Floating::ReinterpretBits(
        infinity_bits - max_ulps - 1);

    // Makes some NAN's.  Sets the most significant bit of the fraction so that
    // our NaN's are quiet; trying to process a signaling NaN would raise an
    // exception if our environment enables floating point exceptions.
    values_.nan1 = Floating::ReinterpretBits(Floating::kExponentBitMask
        | (static_cast<Bits>(1) << (Floating::kFractionBitCount - 1)) | 1);
    values_.nan2 = Floating::ReinterpretBits(Floating::kExponentBitMask
        | (static_cast<Bits>(1) << (Floating::kFractionBitCount - 1)) | 200);
  }

  void TestSize() {
    EXPECT_EQ(sizeof(RawType), sizeof(Bits));
  }

  static TestValues values_;
};

template <typename RawType>
typename FloatingPointTest<RawType>::TestValues
    FloatingPointTest<RawType>::values_;

// Instantiates FloatingPointTest for testing *_FLOAT_EQ.
typedef FloatingPointTest<float> FloatTest;

// Tests that the size of Float::Bits matches the size of float.
TEST_F(FloatTest, Size) {
  TestSize();
}

// Tests comparing with +0 and -0.
TEST_F(FloatTest, Zeros) {
  EXPECT_FLOAT_EQ(0.0, -0.0);
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(-0.0, 1.0),
                          "1.0");
  EXPECT_FATAL_FAILURE(ASSERT_FLOAT_EQ(0.0, 1.5),
                       "1.5");
}

// Tests comparing numbers close to 0.
//
// This ensures that *_FLOAT_EQ handles the sign correctly and no
// overflow occurs when comparing numbers whose absolute value is very
// small.
TEST_F(FloatTest, AlmostZeros) {
  // In C++Builder, names within local classes (such as used by
  // EXPECT_FATAL_FAILURE) cannot be resolved against static members of the
  // scoping class.  Use a static local alias as a workaround.
  // We use the assignment syntax since some compilers, like Sun Studio,
  // don't allow initializing references using construction syntax
  // (parentheses).
  static const FloatTest::TestValues& v = this->values_;

  EXPECT_FLOAT_EQ(0.0, v.close_to_positive_zero);
  EXPECT_FLOAT_EQ(-0.0, v.close_to_negative_zero);
  EXPECT_FLOAT_EQ(v.close_to_positive_zero, v.close_to_negative_zero);

  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_FLOAT_EQ(v.close_to_positive_zero,
                    v.further_from_negative_zero);
  }, "v.further_from_negative_zero");
}

// Tests comparing numbers close to each other.
TEST_F(FloatTest, SmallDiff) {
  EXPECT_FLOAT_EQ(1.0, values_.close_to_one);
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(1.0, values_.further_from_one),
                          "values_.further_from_one");
}

// Tests comparing numbers far apart.
TEST_F(FloatTest, LargeDiff) {
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(2.5, 3.0),
                          "3.0");
}

// Tests comparing with infinity.
//
// This ensures that no overflow occurs when comparing numbers whose
// absolute value is very large.
TEST_F(FloatTest, Infinity) {
  EXPECT_FLOAT_EQ(values_.infinity, values_.close_to_infinity);
  EXPECT_FLOAT_EQ(-values_.infinity, -values_.close_to_infinity);
#if !GTEST_OS_SYMBIAN
  // Nokia's STLport crashes if we try to output infinity or NaN.
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(values_.infinity, -values_.infinity),
                          "-values_.infinity");

  // This is interesting as the representations of infinity and nan1
  // are only 1 DLP apart.
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(values_.infinity, values_.nan1),
                          "values_.nan1");
#endif  // !GTEST_OS_SYMBIAN
}

// Tests that comparing with NAN always returns false.
TEST_F(FloatTest, NaN) {
#if !GTEST_OS_SYMBIAN
// Nokia's STLport crashes if we try to output infinity or NaN.

  // In C++Builder, names within local classes (such as used by
  // EXPECT_FATAL_FAILURE) cannot be resolved against static members of the
  // scoping class.  Use a static local alias as a workaround.
  // We use the assignment syntax since some compilers, like Sun Studio,
  // don't allow initializing references using construction syntax
  // (parentheses).
  static const FloatTest::TestValues& v = this->values_;

  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(v.nan1, v.nan1),
                          "v.nan1");
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(v.nan1, v.nan2),
                          "v.nan2");
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(1.0, v.nan1),
                          "v.nan1");

  EXPECT_FATAL_FAILURE(ASSERT_FLOAT_EQ(v.nan1, v.infinity),
                       "v.infinity");
#endif  // !GTEST_OS_SYMBIAN
}

// Tests that *_FLOAT_EQ are reflexive.
TEST_F(FloatTest, Reflexive) {
  EXPECT_FLOAT_EQ(0.0, 0.0);
  EXPECT_FLOAT_EQ(1.0, 1.0);
  ASSERT_FLOAT_EQ(values_.infinity, values_.infinity);
}

// Tests that *_FLOAT_EQ are commutative.
TEST_F(FloatTest, Commutative) {
  // We already tested EXPECT_FLOAT_EQ(1.0, values_.close_to_one).
  EXPECT_FLOAT_EQ(values_.close_to_one, 1.0);

  // We already tested EXPECT_FLOAT_EQ(1.0, values_.further_from_one).
  EXPECT_NONFATAL_FAILURE(EXPECT_FLOAT_EQ(values_.further_from_one, 1.0),
                          "1.0");
}

// Tests EXPECT_NEAR.
TEST_F(FloatTest, EXPECT_NEAR) {
  EXPECT_NEAR(-1.0f, -1.1f, 0.2f);
  EXPECT_NEAR(2.0f, 3.0f, 1.0f);
  EXPECT_NONFATAL_FAILURE(EXPECT_NEAR(1.0f,1.5f, 0.25f),  // NOLINT
                          "The difference between 1.0f and 1.5f is 0.5, "
                          "which exceeds 0.25f");
  // To work around a bug in gcc 2.95.0, there is intentionally no
  // space after the first comma in the previous line.
}

// Tests ASSERT_NEAR.
TEST_F(FloatTest, ASSERT_NEAR) {
  ASSERT_NEAR(-1.0f, -1.1f, 0.2f);
  ASSERT_NEAR(2.0f, 3.0f, 1.0f);
  EXPECT_FATAL_FAILURE(ASSERT_NEAR(1.0f,1.5f, 0.25f),  // NOLINT
                       "The difference between 1.0f and 1.5f is 0.5, "
                       "which exceeds 0.25f");
  // To work around a bug in gcc 2.95.0, there is intentionally no
  // space after the first comma in the previous line.
}

// Tests the cases where FloatLE() should succeed.
TEST_F(FloatTest, FloatLESucceeds) {
  EXPECT_PRED_FORMAT2(FloatLE, 1.0f, 2.0f);  // When val1 < val2,
  ASSERT_PRED_FORMAT2(FloatLE, 1.0f, 1.0f);  // val1 == val2,

  // or when val1 is greater than, but almost equals to, val2.
  EXPECT_PRED_FORMAT2(FloatLE, values_.close_to_positive_zero, 0.0f);
}

// Tests the cases where FloatLE() should fail.
TEST_F(FloatTest, FloatLEFails) {
  // When val1 is greater than val2 by a large margin,
  EXPECT_NONFATAL_FAILURE(EXPECT_PRED_FORMAT2(FloatLE, 2.0f, 1.0f),
                          "(2.0f) <= (1.0f)");

  // or by a small yet non-negligible margin,
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED_FORMAT2(FloatLE, values_.further_from_one, 1.0f);
  }, "(values_.further_from_one) <= (1.0f)");

#if !GTEST_OS_SYMBIAN && !defined(__BORLANDC__)
  // Nokia's STLport crashes if we try to output infinity or NaN.
  // C++Builder gives bad results for ordered comparisons involving NaNs
  // due to compiler bugs.
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED_FORMAT2(FloatLE, values_.nan1, values_.infinity);
  }, "(values_.nan1) <= (values_.infinity)");
  EXPECT_NONFATAL_FAILURE({  // NOLINT
    EXPECT_PRED_FORMAT2(FloatLE, -values_.infinity, values_.nan1);
  }, "(-values_.infinity) <= (values_.nan1)");
  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_PRED_FORMAT2(FloatLE, values_.nan1, values_.nan1);
  }, "(values_.nan1) <= (values_.nan1)");
#endif  // !GTEST_OS_SYMBIAN && !defined(__BORLANDC__)
}

// Instantiates FloatingPointTest for testing *_DOUBLE_EQ.
typedef FloatingPointTest<double> DoubleTest;

// Tests that the size of Double::Bits matches the size of double.
TEST_F(DoubleTest, Size) {
  TestSize();
}

// Tests comparing with +0 and -0.
TEST_F(DoubleTest, Zeros) {
  EXPECT_DOUBLE_EQ(0.0, -0.0);
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(-0.0, 1.0),
                          "1.0");
  EXPECT_FATAL_FAILURE(ASSERT_DOUBLE_EQ(0.0, 1.0),
                       "1.0");
}

// Tests comparing numbers close to 0.
//
// This ensures that *_DOUBLE_EQ handles the sign correctly and no
// overflow occurs when comparing numbers whose absolute value is very
// small.
TEST_F(DoubleTest, AlmostZeros) {
  // In C++Builder, names within local classes (such as used by
  // EXPECT_FATAL_FAILURE) cannot be resolved against static members of the
  // scoping class.  Use a static local alias as a workaround.
  // We use the assignment syntax since some compilers, like Sun Studio,
  // don't allow initializing references using construction syntax
  // (parentheses).
  static const DoubleTest::TestValues& v = this->values_;

  EXPECT_DOUBLE_EQ(0.0, v.close_to_positive_zero);
  EXPECT_DOUBLE_EQ(-0.0, v.close_to_negative_zero);
  EXPECT_DOUBLE_EQ(v.close_to_positive_zero, v.close_to_negative_zero);

  EXPECT_FATAL_FAILURE({  // NOLINT
    ASSERT_DOUBLE_EQ(v.close_to_positive_zero,
                     v.further_from_negative_zero);
  }, "v.further_from_negative_zero");
}

// Tests comparing numbers close to each other.
TEST_F(DoubleTest, SmallDiff) {
  EXPECT_DOUBLE_EQ(1.0, values_.close_to_one);
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(1.0, values_.further_from_one),
                          "values_.further_from_one");
}

// Tests comparing numbers far apart.
TEST_F(DoubleTest, LargeDiff) {
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(2.0, 3.0),
                          "3.0");
}

// Tests comparing with infinity.
//
// This ensures that no overflow occurs when comparing numbers whose
// absolute value is very large.
TEST_F(DoubleTest, Infinity) {
  EXPECT_DOUBLE_EQ(values_.infinity, values_.close_to_infinity);
  EXPECT_DOUBLE_EQ(-values_.infinity, -values_.close_to_infinity);
#if !GTEST_OS_SYMBIAN
  // Nokia's STLport crashes if we try to output infinity or NaN.
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(values_.infinity, -values_.infinity),
                          "-values_.infinity");

  // This is interesting as the representations of infinity_ and nan1_
  // are only 1 DLP apart.
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(values_.infinity, values_.nan1),
                          "values_.nan1");
#endif  // !GTEST_OS_SYMBIAN
}

// Tests that comparing with NAN always returns false.
TEST_F(DoubleTest, NaN) {
#if !GTEST_OS_SYMBIAN
  // In C++Builder, names within local classes (such as used by
  // EXPECT_FATAL_FAILURE) cannot be resolved against static members of the
  // scoping class.  Use a static local alias as a workaround.
  // We use the assignment syntax since some compilers, like Sun Studio,
  // don't allow initializing references using construction syntax
  // (parentheses).
  static const DoubleTest::TestValues& v = this->values_;

  // Nokia's STLport crashes if we try to output infinity or NaN.
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(v.nan1, v.nan1),
                          "v.nan1");
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(v.nan1, v.nan2), "v.nan2");
  EXPECT_NONFATAL_FAILURE(EXPECT_DOUBLE_EQ(1.0, v.nan1), "v.nan1");
  EXPECT_FATAL_FAILURE(ASSERT_DOUBLE_EQ(v.nan1, v.infinity),
                       "v.infinity");
#endif  // !GTEST_OS_SYMBIAN
}

// Tests that *_DOUBLE_EQ are reflexive.
TEST_F(DoubleTest, Reflexive) {
  EXPECT_DOUBLE_EQ(0.0, 0.0);
  EXPECT_D