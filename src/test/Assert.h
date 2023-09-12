#ifndef __test_Assert_h__
#define __test_Assert_h__

#include <string>

#include "test/Logger.h"

class Assert {
 private:
  bool is_failed_ = false;
  Logger& trace_;

 public:
  Assert(Logger& trace) : trace_(trace) {}

  bool isFailed() {
    return is_failed_;
  }

  void resetFailStatus() {
    is_failed_ = false;
  }

  void fail() {
    is_failed_ = true;
  }

  template <typename T>
  void equal(const T& actual, const T& expected) {
    if (actual != expected) {
      fail();
      trace_ << "Assert failed. Found: \"" << actual << "\" (expected \"" << expected << "\")" << std::endl;
    }
  }

  // Two vectors include the same elements in the same order.
  template <typename T>
  void equal(const std::vector<T>& set1, const std::vector<T>& set2) {
    equal(set1.size(), set2.size());
    for (size_t i = 0; i < set1.size(); i++) {
      equal(set1[i], set2[i]);
    }
  }

  // A container holds all the elements of a set in any order.
  template <typename Container>
  void contains(const Container& container, const Container& set) {
    Container container_copy = container;
    for (const auto& e : set) {
      bool found = false;
      for (auto& iter = container_copy.cbegin(); iter != container_copy.cend(); iter++) {
        if (*iter == e) {
          found = true;
          container_copy.erase(iter);
          break;
        }
      }
      if (!found) {
        fail();
        trace_ << "Assert failed. Item \"" << e << "\" not found in expected container" << std::endl;
      }
    }
  }

  // Two containers include the same elements in any order.
  template <typename Container>
  void equivalent(const Container& set1, const Container& set2) {
    contains(set1, set2);
    contains(set2, set1);
  }
};

#endif  // __test_Assert_h__
