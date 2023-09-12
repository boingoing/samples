#ifndef __test_Constants_h__
#define __test_Constants_h__

#include <iostream>

enum class TestResult : uint8_t {
  Pass = 0,
  Fail,
  Skip,
  Disabled,
};

enum class TestCaseFlag : uint8_t {
  ExpectPass = 0,
  Disabled = 1,
};

#endif  // __test_Constants_h__
