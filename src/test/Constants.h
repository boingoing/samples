//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_Constants_h__
#define __test_Constants_h__

#include <cstdint>

enum class TestResult : uint8_t {
  Pass = 0,
  Fail,
  Skip,
  Disabled,
};

enum class TestCaseFlag : uint8_t {
  ExpectPass = 0,
  Disabled = 1 << 0,
  Skip = 1 << 1,
};

TestCaseFlag operator|(const TestCaseFlag& lhs, const TestCaseFlag& rhs);
TestCaseFlag& operator|=(TestCaseFlag& lhs, const TestCaseFlag& rhs);
TestCaseFlag operator&(const TestCaseFlag& lhs, const TestCaseFlag& rhs);

#endif  // __test_Constants_h__
