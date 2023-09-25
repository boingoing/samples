#include <type_traits>

#include "test/Constants.h"

TestCaseFlag operator|(const TestCaseFlag& lhs, const TestCaseFlag& rhs) {
  using T = std::underlying_type<TestCaseFlag>::type;
  return static_cast<TestCaseFlag>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

TestCaseFlag& operator|=(TestCaseFlag& lhs, const TestCaseFlag& rhs) {
  lhs = lhs | rhs;
  return lhs;
}

TestCaseFlag operator&(const TestCaseFlag& lhs, const TestCaseFlag& rhs) {
  using T = std::underlying_type<TestCaseFlag>::type;
  return static_cast<TestCaseFlag>(static_cast<T>(lhs) & static_cast<T>(rhs));
}
