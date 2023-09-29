//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_TestCaseData_h__
#define __test_TestCaseData_h__

struct TestCaseData {
};

template <typename ExpectedType>
struct TestCaseDataWithExpectedResult {
  ExpectedType expected;
};

#endif  // __test_TestCaseData_h__
