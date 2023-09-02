#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "hashmap_happy_number.h"
#include "helpers.h"

// Given a number, |n|, determine if it is a happy number.
// A happy number is defined as one which may be decomposed into digits,
// the sum of the squares of those digits decomposed into digits, etc,
// recursively, until the sum settles at 1.
// ex: 19
//       => 1^2 + 9^2 = 82
//       => 8^2 + 2^2 = 68
//       => 6^2 + 8^2 = 100
//       => 1^2 + 0^2 + 0^2 = 1
// This is leetcode 202. Happy Number
// https://leetcode.com/problems/happy-number
bool is_happy(int n) {
  // The set of values we've already reached.
  std::unordered_set<int> visited_number_set;

  // Until n is settled to 1, consider the sum of the squares of it's digits.
  while (n != 1) {
    // If we've already seen |n|, then the values loop and never settle
    // to 1.
    const auto& iter = visited_number_set.find(n);
    if (iter != visited_number_set.cend()) {
      return false;
    }
    visited_number_set.insert(n);

    // Calculate the sum of the squares of the digits of |n|.
    int sum_of_factors = 0;
    while (n != 0) {
      int factor = n % 10;
      n /= 10;
      sum_of_factors += factor * factor;
    }
    n = sum_of_factors;
  }

  // n == 1, value passed to us is happy.
  return true;
}

void test_is_happy(int n, bool expected) {
  std::cout << std::endl << "Determining if " << n << " is a happy number" << std::endl;

  const auto actual = is_happy(n);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_happy_number() {
  test_is_happy(19, true);
  test_is_happy(2, false);

  return 0;
}