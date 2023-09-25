#ifndef __test_TestCaseContainer_h__
#define __test_TestCaseContainer_h__

#include <string>
#include <unordered_map>

#include "test/Constants.h"

class TestCase;

using TestCaseMap = std::unordered_map<std::string, std::unordered_map<std::string, TestCase*>>;

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

class TestCaseContainer {
 private:
  static TestCaseMap tests_;
  static std::string filter_;
  static TestCaseStats stats_;
  static bool verbose_;

 public:
  static void add(TestCase* tc);
  static const TestCaseStats& runOneTest(TestCase* tc);
  static void runAllTests();
  static void setFilter(const std::string& filter);
  static void enableVerbose();
};

#endif  // __test_TestCaseContainer_h__
