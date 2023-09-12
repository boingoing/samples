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
};

class TestCaseContainer {
 private:
  static TestCaseMap tests_;
  static std::string filter_;
  static TestCaseStats stats_;
  static bool verbose_;

 public:
  static void add(TestCase* tc);
  static TestResult runOneTest(TestCase* tc);
  static void runAllTests();
  static void aggregate(TestResult result);
  static void setFilter(const std::string& filter);
  static void enableVerbose();
};

#endif  // __test_TestCaseContainer_h__
