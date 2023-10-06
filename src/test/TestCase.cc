//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <string>
#include <vector>

#include "test/TestCase.h"

#include "test/Assert.h"
#include "test/Constants.h"
#include "test/Logger.h"
#include "test/TestCaseContainer.h"
#include "test/TestCaseData.h"

TestCase::TestCase() : assert(trace) {}

std::string TestCase::getBuffer() const {
  return trace.getBuffer();
}

TestResult TestCase::getResult() const {
  return result_;
}

void TestCase::setResult(TestResult result) {
  result_ = result;
}

const TestCaseStats& TestCase::getStats() const {
  return stats_;
}

TestCaseFlag TestCase::getFlags() const {
  return flags_;
}

void TestCase::setFlags(TestCaseFlag add) {
  flags_ |= add;
}

TestResult TestCase::run() {
  if ((getFlags() & TestCaseFlag::Disabled) == TestCaseFlag::Disabled) {
    setResult(TestResult::Disabled);
  } else if ((getFlags() & TestCaseFlag::Skip) == TestCaseFlag::Skip) {
    setResult(TestResult::Skip);
  } else if ((getFlags() & TestCaseFlag::ExpectPass) == TestCaseFlag::ExpectPass) {
    assert.resetFailStatus();
    runImpl();
    if (assert.isFailed()) {
      setResult(TestResult::Fail);
    } else {
      setResult(TestResult::Pass);
    }
  }
  aggregate(getResult());
  return getResult();
}

void TestCase::aggregate(TestResult result) {
  stats_.aggregate(result);
}
