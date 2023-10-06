//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_TestCase_h__
#define __test_TestCase_h__

#include <string>
#include <vector>

#include "test/Assert.h"
#include "test/Constants.h"
#include "test/Logger.h"
#include "test/TestCaseContainer.h"
#include "test/TestCaseData.h"

// On some platforms, "assert" is defined as a macro which will break our
// attempt to use the name for a member of TestCase below. Undef it just
// as a workaround. This isn't a serious project.
#ifdef assert
#undef assert
#endif

class TestCase {
 public:
  TestCase();
  virtual ~TestCase() = default;

  virtual std::string getName() const = 0;
  virtual std::string getBaseClassName() const = 0;
  virtual std::string getFullName() const = 0;
  virtual size_t getTestCaseCount() const = 0;

  std::string getBuffer() const;
  TestResult getResult() const;
  void setResult(TestResult result);
  const TestCaseStats& getStats() const;
  TestCaseFlag getFlags() const;
  void setFlags(TestCaseFlag add);

  TestResult run();

 protected:
  virtual void runImpl() = 0;

  void aggregate(TestResult result);

 private:
  TestResult result_;
  TestCaseStats stats_;
  TestCaseFlag flags_;

 protected:
  Logger trace;
  Assert assert;
};

#define TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set) \
class base_class##_##test_name : public base_class { \
 private: \
  static base_class##_##test_name instance_; \
  std::vector<data_type> data_ = data_set; \
 public: \
  base_class##_##test_name() { \
    TestCaseContainer::add(this); \
    setFlags(flags); \
  } \
  ~base_class##_##test_name() = default; \
  std::string getName() const override { \
    return #test_name; \
  } \
  std::string getBaseClassName() const override { \
    return #base_class; \
  } \
  std::string getFullName() const override { \
    return #base_class "_" #test_name; \
  } \
  size_t getTestCaseCount() const override; \
  void runImpl() override; \
  void runImpl(data_type& data); \
}; \
base_class##_##test_name base_class##_##test_name::instance_;

#define TEST_CASE_WITH_DATA_BASE(base_class, test_name, flags, data_type, data_set) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set) \
size_t base_class##_##test_name::getTestCaseCount() const { \
  return data_.size(); \
} \
void base_class##_##test_name::runImpl() { \
  bool did_any_fail = false; \
  for (auto& d: data_) { \
    assert.resetFailStatus(); \
    runImpl(d); \
    if (assert.isFailed()) { \
      did_any_fail = true; \
      aggregate(TestResult::Fail); \
    } else { \
      aggregate(TestResult::Pass); \
    } \
  } \
  if (did_any_fail) { \
    assert.fail(); \
  } \
} \
void base_class##_##test_name::runImpl(data_type& data)

#define TEST_CASE_BASE(base_class, test_name, flags) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, TestCaseData, {{}}) \
size_t base_class##_##test_name::getTestCaseCount() const { \
  return 1; \
} \
void base_class##_##test_name::runImpl(TestCaseData& data) { } \
void base_class##_##test_name::runImpl()

#define TEST_CASE(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::ExpectPass)

#define TEST_CASE_DISABLED(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::Disabled)

#define TEST_CASE_WITH_DATA(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::ExpectPass, data_type, data_set)

#define TEST_CASE_WITH_DATA_DISABLED(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::Disabled, data_type, data_set)

#endif  // __test_TestCase_h__
