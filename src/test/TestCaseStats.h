//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_TestCaseStats_h__
#define __test_TestCaseStats_h__

#include <cstddef>

#include "test/Constants.h"

struct TestCaseStats {
  size_t run_count;
  size_t pass_count;
  size_t fail_count;
  size_t skip_count;
  size_t disable_count;

  void aggregate(TestResult result);
  const TestCaseStats& operator+=(const TestCaseStats& rhs);
};

#endif  // __test_TestCaseStats_h__
