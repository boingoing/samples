#ifndef __test_helpers_h__
#define __test_helpers_h__

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class TestResult : uint8_t {
  Pass = 0,
  Fail,
  Skip,
  Disabled,
};

enum class TestCaseFlag : uint8_t {
  Execute = 0,
  Disabled = 1,
};

class Assert {
 private:
  bool assertFailed_ = false;

 public:
  bool getAssertFailed() {
    return assertFailed_;
  }

  void resetAssertStatus() {
    assertFailed_ = false;
  }

  template <typename T>
  void assertEqual(const T& actual, const T& expected) {
    if (actual != expected) {
      assertFailed_ = true;
      std::cerr << "Assert failed. Found: \"" << actual << "\" (expected \"" << expected << "\")" << std::endl;
    }
  }

  void assertFail() {
    assertFailed_ = true;
  }
};

class TestLogger {
 private:
  std::stringstream buffer_;

 public:
  template <typename T>
  TestLogger& operator<<(const T& t) {
    buffer_ << t;
    return *this;
  }

  TestLogger& operator<<(std::ostream& (*f)(std::ostream &)) {
    f(buffer_);
    return *this;
  }

  TestLogger& operator<<(std::ostream& (*f)(std::ios &)) {
    f(buffer_);
    return *this;
  }

  TestLogger& operator<<(std::ostream& (*f)(std::ios_base &)) {
    f(buffer_);
    return *this;
  }
};

class TestCase : public Assert {
 public:
  virtual ~TestCase() {}
  virtual TestResult run() = 0;
  virtual std::string getName() = 0;
  virtual std::string getBaseClass() = 0;
  virtual TestCaseFlag getFlags() = 0;

 protected:
  TestLogger trace;
};

struct TestCaseData {
};

template <typename ExpectedType>
struct TestCaseDataWithExpectedResult {
  ExpectedType expected;
};

using TestCaseMap = std::unordered_map<std::string, std::unordered_map<std::string, TestCase*>>;

class TestCaseContainer {
 private:
  static TestCaseMap tests_;

 public:
  static void add(TestCase* tc) {
    tests_[tc->getBaseClass()][tc->getName()] = tc;
  }

  static void runOneTest(TestCase* tc) {
    const auto result = tc->run();
  }

  static void runAllTests() {
    for (const auto& base_name_map_pair : tests_) {
      for (const auto& name_tc_pair : base_name_map_pair.second) {
        runOneTest(name_tc_pair.second);
      }
    }
  }
};

#define TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set) \
class base_class##_##test_name : public base_class { \
 private: \
  static base_class##_##test_name instance_; \
  std::vector<data_type> data_ = data_set; \
 public: \
  const std::string name_ = #base_class "_" #test_name; \
  base_class##_##test_name() { \
    TestCaseContainer::add(this); \
  } \
  ~base_class##_##test_name() = default; \
  std::string getName() override { \
    return #test_name; \
  } \
  std::string getBaseClass() override { \
    return #base_class; \
  } \
  TestCaseFlag getFlags() override { \
    return flags; \
  } \
  void runImpl(data_type& data); \
  void runImpl(); \
  TestResult run() override { \
    TestResult result = TestResult::Pass; \
    for (auto& d: data_) { \
      resetAssertStatus(); \
      runImpl(d); \
      if (getAssertFailed()) { \
        result = TestResult::Fail; \
      } \
    } \
    return result; \
  } \
}; \
base_class##_##test_name base_class##_##test_name::instance_;

#define TEST_CASE_WITH_DATA_BASE(base_class, test_name, flags, data_type, data_set) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, data_type, data_set) \
void base_class##_##test_name::runImpl() {} \
void base_class##_##test_name::runImpl(data_type& data)

#define TEST_CASE_BASE(base_class, test_name, flags) \
TEST_CASE_COMMON_BASE(base_class, test_name, flags, TestCaseData, {}) \
void base_class##_##test_name::runImpl(TestCaseData& data) {} \
void base_class##_##test_name::runImpl()

#define TEST_CASE(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::Execute)

#define TEST_CASE_DISABLED(base_class, test_name) \
TEST_CASE_BASE(base_class, test_name, TestCaseFlag::Disabled)

#define TEST_CASE_WITH_DATA(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::Execute, data_type, data_set)

#define TEST_CASE_WITH_DATA_DISABLED(base_class, test_name, data_type, data_set) \
TEST_CASE_WITH_DATA_BASE(base_class, test_name, TestCaseFlag::Disabled, data_type, data_set)

#endif  // __test_helpers_h__
