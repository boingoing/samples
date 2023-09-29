//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "test/TestCaseContainer.h"

#include "test/AutostartStopwatch.h"
#include "test/Constants.h"
#include "test/TestCase.h"

TestCaseMap TestCaseContainer::tests_;
std::string TestCaseContainer::filter_;
TestCaseStats TestCaseContainer::stats_;
bool TestCaseContainer::verbose_ = false;

namespace {

bool shouldRunTest(const TestCase* tc, const std::string& filter) {
  return filter == "" || tc->getFullName().find(filter) != std::string::npos;
}

bool shouldRunTestSuite(const std::unordered_map<std::string, TestCase*>& suite, const std::string& filter) {
  for (const auto& tc_pair : suite) {
    if (shouldRunTest(tc_pair.second, filter)) {
      return true;
    }
  }
  return false;
}

}  // namespace

// static
void TestCaseContainer::add(TestCase* tc) {
  tests_[tc->getBaseClassName()][tc->getName()] = tc;
}

// static
const TestCaseStats& TestCaseContainer::runOneTest(TestCase* tc) {
  const auto should_skip = !shouldRunTest(tc, filter_);
  if (should_skip) {
    tc->setFlags(TestCaseFlag::Skip);
  }
  AutostartStopwatch timer;
  tc->run();
  if (!should_skip && verbose_) {
    std::cout << std::endl << tc->getFullName() << ":" << std::endl;
    std::cout << tc->getBuffer() << std::endl;
    std::cout << "Time taken: " << timer.elapsed() << "ns" << std::endl;
  }
  return tc->getStats();
}

// static
void TestCaseContainer::runAllTests() {
  for (const auto& base_name_map_pair : tests_) {
    if (shouldRunTestSuite(base_name_map_pair.second, filter_)) {
      std::cout << "Running " << base_name_map_pair.first << " tests..." << std::endl;
    }
    for (const auto& name_tc_pair : base_name_map_pair.second) {
      stats_ += runOneTest(name_tc_pair.second);
    }
  }

  if (stats_.fail_count > 0) {
    std::cout << std::endl << "Failed test cases: " << std::endl;
    for (const auto& base_name_map_pair : tests_) {
      for (const auto& name_tc_pair : base_name_map_pair.second) {
        if (name_tc_pair.second->getResult() == TestResult::Fail) {
          std::cout << std::endl << name_tc_pair.second->getFullName() << ":" << std::endl;
          std::cout << name_tc_pair.second->getBuffer();
        }
      }
    }
  }

  std::cout << std::endl << "Total tests: " << stats_.run_count << std::endl;
  std::cout << "Passed tests: " << stats_.pass_count << std::endl;
  std::cout << "Failed tests: " << stats_.fail_count << std::endl;
  std::cout << "Disabled tests: " << stats_.disable_count << std::endl;
  std::cout << "Skipped tests: " << stats_.skip_count << std::endl;
}

// static
void TestCaseContainer::setFilter(const std::string& filter) {
  filter_ = filter;
}

// static
void TestCaseContainer::enableVerbose() {
  verbose_ = true;
}
