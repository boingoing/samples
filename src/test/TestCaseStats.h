//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_TestCaseStats_h__
#define __test_TestCaseStats_h__

#include "test/Constants.h"

struct TestCaseStats {
  size_t run_count;
  size_t pass_count;
  size_t fail_count;
  size_t skip_count;
  size_t disable_count;

  void aggregate(TestResult result) {
    switch (result) {
    case TestResult::Fail:
      fail_count++;
      break;
    case TestResult::Pass:
      pass_count++;
      break;
    case TestResult::Skip:
      skip_count++;
      break;
    case TestResult::Disabled:
      disable_count++;
      break;
    }
    run_count++;
  }

  const TestCaseStats& operator+=(const TestCaseStats& rhs) {
    fail_count += rhs.fail_count;
    pass_count += rhs.pass_count;
    skip_count += rhs.skip_count;
    disable_count += rhs.disable_count;
    run_count += rhs.run_count;
    return *this;
  }
};

#endif  // __test_TestCaseStats_h__
