#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "test/TestCaseContainer.h"

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
TestResult TestCaseContainer::runOneTest(TestCase* tc) {
  if (shouldRunTest(tc, filter_)) {
    tc->run();
    if (verbose_) {
      std::cout << std::endl << tc->getFullName() << ":" << std::endl;
      std::cout << tc->getBuffer() << std::endl;
    }
  } else {
    tc->setResult(TestResult::Skip);
  }
  return tc->getResult();
}

// static
void TestCaseContainer::runAllTests() {
  for (const auto& base_name_map_pair : tests_) {
    if (shouldRunTestSuite(base_name_map_pair.second, filter_)) {
      std::cout << "Running " << base_name_map_pair.first << " tests..." << std::endl;
    }
    for (const auto& name_tc_pair : base_name_map_pair.second) {
      aggregate(runOneTest(name_tc_pair.second));
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

// static
void TestCaseContainer::aggregate(TestResult result) {
  switch (result) {
  case TestResult::Fail:
    stats_.fail_count++;
    break;
  case TestResult::Pass:
    stats_.pass_count++;
    break;
  case TestResult::Skip:
    stats_.skip_count++;
    break;
  case TestResult::Disabled:
    stats_.disable_count++;
    break;
  }
  stats_.run_count++;
}
