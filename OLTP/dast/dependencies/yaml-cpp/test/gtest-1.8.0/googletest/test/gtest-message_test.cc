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
// Tests for the Message class.

#include "gtest/gtest-message.h"

#include "gtest/gtest.h"

namespace {

using ::testing::Message;

// Tests the testing::Message class

// Tests the default constructor.
TEST(MessageTest, DefaultConstructor) {
  const Message msg;
  EXPECT_EQ("", msg.GetString());
}

// Tests the copy constructor.
TEST(MessageTest, CopyConstructor) {
  const Message msg1("Hello");
  const Message msg2(msg1);
  EXPECT_EQ("Hello", msg2.GetString());
}

// Tests constructing a Message from a C-string.
TEST(MessageTest, ConstructsFromCString) {
  Message msg("Hello");
  EXPECT_EQ("Hello", msg.GetString());
}

// Tests streaming a float.
TEST(MessageTest, StreamsFloat) {
  const std::string s = (Message() << 1.23456F << " " << 2.34567F).GetString();
  // Both numbers should be printed with enough precision.
  EXPECT_PRED_FORMAT2(testing::IsSubstring, "1.234560", s.c_str());
  EXPECT_PRED_FORMAT2(testing::IsSubstring, " 2.345669", s.c_str());
}

// Tests streaming a double.
TEST(MessageTest, StreamsDouble) {
  const std::string s = (Message() << 1260570880.4555497 << " "
                                  << 1260572265.1954534).GetString();
  // Both numbers should be printed with enough precision.
  EXPECT_PRED_FORMAT2(testing::IsSubstring, "1260570880.45", s.c_str());
  EXPECT_PRED_FORMAT2(testing::IsSubstring, " 1260572265.19", s.c_str());
}

// Tests streaming a non-char pointer.
TEST(MessageTest, StreamsPointer) {
  int n = 0;
  int* p = &n;
  EXPECT_NE("(null)", (Message() << p).GetString());
}

// Tests streaming a NULL non-char pointer.
TEST(MessageTest, StreamsNullPointer) {
  int* p = NULL;
  EXPECT_EQ("(null)", (Message() << p).GetString());
}

// Tests streaming a C string.
TEST(MessageTest, StreamsCString) {
  EXPECT_EQ("Foo", (Message() << "Foo").GetString());
}

// Tests streaming a NULL C string.
TEST(MessageTest, StreamsNullCString) {
  char* p = NULL;
  EXPECT_EQ("(null)", (Message() << p).GetString());
}

// Tests streaming std::string.
TEST(MessageTest, StreamsString) {
  const ::std::string str("Hello");
  EXPECT_EQ("Hello", (Message() << str).GetString());
}

// Tests that we can output strings containing embedded NULs.
TEST(MessageTest, StreamsStringWithEmbeddedNUL) {
  const char char_array_with_nul[] =
      "Here's a NUL\0 and some more string";
  const ::std::string string_with_nul(char_array_with_nul,
                                      sizeof(char_array_with_nul) - 1);
  EXPECT_EQ("Here's a NUL\\0 and some more string",
            (Message() << string_with_nul).GetString());
}

// Tests streaming a NUL char.
TEST(MessageTest, StreamsNULChar) {
  EXPECT_EQ("\\0", (Message() << '\0').GetString());
}

// Tests streaming int.
TEST(MessageTest, StreamsInt) {
  EXPECT_EQ("123", (Message() << 123).GetString());
}

// Tests that basic IO manipulators (endl, ends, and flush) can be
// streamed to Message.
TEST(MessageTest, StreamsBasicIoManip) {
  EXPECT_EQ("Line 1.\nA NUL char \\0 in line 2.",
               (Message() << "Line 1." << std::endl
                         << "A NUL char " << std::ends << std::flush
                         << " in line 2.").GetString());
}

// Tests Message::GetString()
TEST(MessageTest, GetString) {
  Message msg;
  msg << 1 << " lamb";
  EXPECT_EQ("1 lamb", msg.GetString());
}

// Tests streaming a Message object to an ostream.
TEST(MessageTest, StreamsToOStream) {
  Message msg("Hello");
  ::std::stringstream ss;
  ss << msg;
  EXPECT_EQ("Hello", testing::internal::StringStreamToString(&ss));
}

// Tests that a Message object doesn't take up too much stack space.
TEST(MessageTest, DoesNotTakeUpMuchStackSpace) {
  EXPECT_LE(sizeof(Message), 16U);
}

}  // namespace
 are not
  // recording the test name.
  g_events->push_back("ListenerTest::* Test Body");
  SUCCEED();  // Triggers OnTestPartResult.
}

TEST_F(ListenerTest, DoesBar) {
  g_events->push_back("ListenerTest::* Test Body");
  SUCCEED();  // Triggers OnTestPartResult.
}

}  // namespace internal

}  // namespace testing

using ::testing::internal::EnvironmentInvocationCatcher;
using ::testing::internal::EventRecordingListener;

void VerifyResults(const std::vector<std::string>& data,
                   const char* const* expected_data,
                   size_t expected_data_size) {
  const size_t actual_size = data.size();
  // If the following assertion fails, a new entry will be appended to
  // data.  Hence we save data.size() first.
  EXPECT_EQ(expected_data_size, actual_size);

  // Compares the common prefix.
  const size_t shorter_size = expected_data_size <= actual_size ?
      expected_data_size : actual_size;
  size_t i = 0;
  for (; i < shorter_size; ++i) {
    ASSERT_STREQ(expected_data[i], data[i].c_str())
        << "at position " << i;
  }

  // Prints extra elements in the actual data.
  for (; i < actual_size; ++i) {
    printf("  Actual event #%lu: %s\n",
        static_cast<unsigned long>(i), data[i].c_str());
  }
}

int main(int argc, char **argv) {
  std::vector<std::string> events;
  g_events = &events;
  InitGoogleTest(&argc, argv);

  UnitTest::GetInstance()->listeners().Append(
      new EventRecordingListener("1st"));
  UnitTest::GetInstance()->listeners().Append(
      new EventRecordingListener("2nd"));

  AddGlobalTestEnvironment(new EnvironmentInvocationCatcher);

  GTEST_CHECK_(events.size() == 0)
      << "AddGlobalTestEnvironment should not generate any events itself.";

  ::testing::GTEST_FLAG(repeat) = 2;
  int ret_val = RUN_ALL_TESTS();

  const char* const expected_events[] = {
    "1st.OnTestProgramStart",
    "2nd.OnTestProgramStart",
    "1st.OnTestIterationStart(0)",
    "2nd.OnTestIterationStart(0)",
    "1st.OnEnvironmentsSetUpStart",
    "2nd.OnEnvironmentsSetUpStart",
    "Environment::SetUp",
    "2nd.OnEnvironmentsSetUpEnd",
    "1st.OnEnvironmentsSetUpEnd",
    "1st.OnTestCaseStart",
    "2nd.OnTestCaseStart",
    "ListenerTest::SetUpTestCase",
    "1st.OnTestStart",
    "2nd.OnTestStart",
    "ListenerTest::SetUp",
    "ListenerTest::* Test Body",
    "1st.OnTestPartResult",
    "2nd.OnTestPartResult",
    "ListenerTest::TearDown",
    "2nd.OnTestEnd",
    "1st.OnTestEnd",
    "1st.OnTestStart",
    "2nd.OnTestStart",
    "ListenerTest::SetUp",
    "ListenerTest::* Test Body",
    "1st.OnTestPartResult",
    "2nd.OnTestPartResult",
    "ListenerTest::TearDown",
    "2nd.OnTestEnd",
    "1st.OnTestEnd",
    "ListenerTest::TearDownTestCase",
    "2nd.OnTestCaseEnd",
    "1st.OnTestCaseEnd",
    "1st.OnEnvironmentsTearDownStart",
    "2nd.OnEnvironmentsTearDownStart",
    "Environment::TearDown",
    "2nd.OnEnvironmentsTearDownEnd",
    "1st.OnEnvironmentsTearDownEnd",
    "2nd.OnTestIterationEnd(0)",
    "1st.OnTestIterationEnd(0)",
    "1st.OnTestIterationStart(1)",
    "2nd.OnTestIterationStart(1)",
    "1st.OnEnvironmentsSetUpStart",
    "2nd.OnEnvironmentsSetUpStart",
    "Environment::SetUp",
    "2nd.OnEnvironmentsSetUpEnd",
    "1st.OnEnvironmentsSetUpEnd",
    "1st.OnTestCaseStart",
    "2nd.OnTestCaseStart",
    "ListenerTest::SetUpTestCase",
    "1st.OnTestStart",
    "2nd.OnTestStart",
    "ListenerTest::SetUp",
    "ListenerTest::* Test Body",
    "1st.OnTestPartResult",
    "2nd.OnTestPartResult",
    "ListenerTest::TearDown",
    "2nd.OnTestEnd",
    "1st.OnTestEnd",
    "1st.OnTestStart",
    "2nd.OnTestStart",
    "ListenerTest::SetUp",
    "ListenerTest::* Test Body",
    "1st.OnTestPartResult",
    "2nd.OnTestPartResult",
    "ListenerTest::TearDown",
    "2nd.OnTestEnd",
    "1st.OnTestEnd",
    "ListenerTest::TearDownTestCase",
    "2nd.OnTestCaseEnd",
    "1st.OnTestCaseEnd",
    "1st.OnEnvironmentsTearDownStart",
    "2nd.OnEnvironmentsTearDownStart",
    "Environment::TearDown",
    "2nd.OnEnvironmentsTearDownEnd",
    "1st.OnEnvironmentsTearDownEnd",
    "2nd.OnTestIterationEnd(1)",
    "1st.OnTestIterationEnd(1)",
    "2nd.OnTestProgramEnd",
    "1st.OnTestProgramEnd"
  };
  VerifyResults(events,
                expected_events,
                sizeof(expected_events)/sizeof(expected_events[0]));

  // We need to check manually for ad hoc test failures that happen after
  // RUN_ALL_TESTS finishes.
  if (UnitTest::GetInstance()->Failed())
    ret_val = 1;

  return ret_val;
}
