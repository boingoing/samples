#ifndef __test_TestCaseData_h__
#define __test_TestCaseData_h__

struct TestCaseData {
};

template <typename ExpectedType>
struct TestCaseDataWithExpectedResult {
  ExpectedType expected;
};

#endif  // __test_TestCaseData_h__
