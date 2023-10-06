//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "test/TestCaseStats.h"

#include "test/Constants.h"

void TestCaseStats::aggregate(TestResult result) {
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

const TestCaseStats& TestCaseStats::operator+=(const TestCaseStats& rhs) {
  fail_count += rhs.fail_count;
  pass_count += rhs.pass_count;
  skip_count += rhs.skip_count;
  disable_count += rhs.disable_count;
  run_count += rhs.run_count;
  return *this;
}
