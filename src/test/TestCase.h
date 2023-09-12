#ifndef __test_TestCase_h__
#define __test_TestCase_h__

#include <string>
#include <vector>

#include "test/Assert.h"
#include "test/Constants.h"
#include "test/Logger.h"
#include "test/TestCaseContainer.h"
#include "test/TestCaseData.h"

class TestCase {
 public:
  TestCase() : assert(trace) {}
  virtual ~TestCase() {}
  virtual TestResult run() = 0;
  virtual std::string getName() const = 0;
  virtual std::string getBaseClassName() const = 0;
  virtual std::string getFullName() const = 0;
  virtual TestCaseFlag getFlags() const = 0;

  std::string getBuffer() const {
    return trace.getBuffer();
  }

  TestResult getResult() const {
    return result_;
  }

  void setResult(TestResult result) {
    result_ = result;
  }

 private:
  TestResult result_;

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
  TestCaseFlag getFlags() const override { \
    return flags; \
  } \
  void runImpl(data_type& data); \
  TestResult run() override { \
    switch (getFlags()) { \
    case TestCaseFlag::Disabled: \
      setResult(TestResult::Disabled); \
      break; \
    case TestCaseFlag::ExpectPass: \
      setResult(TestResult::Pass); \
      for (auto& d: data_) { \
        assert.resetFailStatus(); \
        runImpl(d); \
        if (assert.isFailed()) { \
          setResult(TestResult::Fail); \
        } \
      } \
      break; \
    } \
    return getResult(); \
  } \
}; \
base_class##_##test_name base_class##_##test_name::instance_; \
void base_class##_##test_name::runImpl(data_type& data)

#define TEST_CASE_WITH_DATA_BASE(base_class, test_name, flags, data_type, data_set) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set)

#define TEST_CASE_BASE(base_class, test_name, flags) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, TestCaseData, {{}})

#define TEST_CASE(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::ExpectPass)

#define TEST_CASE_DISABLED(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::Disabled)

#define TEST_CASE_WITH_DATA(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::ExpectPass, data_type, data_set)

#define TEST_CASE_WITH_DATA_DISABLED(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::Disabled, data_type, data_set)

#endif  // __test_TestCase_h__
