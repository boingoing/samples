//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_TestCaseContainer_h__
#define __test_TestCaseContainer_h__

#include <string>
#include <unordered_map>

#include "test/Constants.h"
#include "test/TestCaseStats.h"

class TestCase;

using TestCaseMap = std::unordered_map<std::string, std::unordered_map<std::string, TestCase*>>;

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
