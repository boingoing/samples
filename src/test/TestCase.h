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
  TestCase() : assert(trace) {}
  virtual ~TestCase() {}
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

  TestResult run() {
    switch (getFlags()) {
    case TestCaseFlag::Disabled:
      setResult(TestResult::Disabled);
      break;
    case TestCaseFlag::ExpectPass:
      setResult(TestResult::Pass);
      runImpl();
      if (assert.isFailed()) {
        setResult(TestResult::Fail);
      }
      break;
    }
    return getResult();
  }

 protected:
  virtual void runImpl() = 0;

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
  void runImpl() override; \
  void runImpl(data_type& data); \
}; \
base_class##_##test_name base_class##_##test_name::instance_; \

#define TEST_CASE_WITH_DATA_BASE(base_class, test_name, flags, data_type, data_set) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set) \
void base_class##_##test_name::runImpl() { \
  for (auto& d: data_) { \
    assert.resetFailStatus(); \
    runImpl(d); \
    if (assert.isFailed()) { \
      setResult(TestResult::Fail); \
    } \
  } \
} \
void base_class##_##test_name::runImpl(data_type& data)

#define TEST_CASE_BASE(base_class, test_name, flags) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, TestCaseData, {{}}) \
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
