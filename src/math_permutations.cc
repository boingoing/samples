//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "test/TestCase.h"

struct NextLargestDigitPermutationTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> digits;
};

std::vector<NextLargestDigitPermutationTestData> next_largest_digit_permutation_tests = {
  {std::vector<int>({9,2,0,1}), {9,2,1,0}},
  {std::vector<int>({9,1,2,0}), {9,2,0,1}},
  {std::vector<int>({9,1,0,2}), {9,1,2,0}},
  {std::vector<int>({9,0,2,1}), {9,1,0,2}},
  {std::vector<int>({9,0,1,2}), {9,0,2,1}},
  {std::vector<int>({2,9,1,0}), {9,0,1,2}},
  {std::vector<int>({2,9,0,1}), {2,9,1,0}},
  {std::vector<int>({1,9,0,2}), {1,9,2,0}},
};

class NextLargestDigitPermutationTest : public TestCase {
 protected:

  template <bool maximize>
  void next_permutation_helper(std::vector<int>& result, int most_significant_digit) {
    if (most_significant_digit >= result.size() - 1) {
      return;
    }

    // Start from the rear.
    int start_index = result.size() - 1;
    int next_digit = result[start_index];
    int current_index = start_index - 1;

    // Find initial split - the smallest digit which needs to change.
    while (current_index >= most_significant_digit) {
      const auto current_digit = result[current_index];
      if ((!maximize && current_digit > next_digit) ||
          (maximize && next_digit > current_digit)) {
        // Split at current_index.
        // Swap digits[start_index] and digits[current_index] as it's the
        // largest digit we saw before current_digit.
        const auto temp = result[current_index];
        result[current_index] = result[start_index];
        result[start_index] = temp;

        // Now we need to recursively solve the remaining digits.
        return next_permutation_helper<true>(result, current_index + 1);
      }
      next_digit = current_digit;
      current_index--;
    }
  }

  // Given a set of |digits| representing a number (ex: {2,3,1} => 231), return
  // the largest permutation of those digits which is less than |digits|.
  // ex: 231 -> 213
  std::vector<int> next_permutation(const std::vector<int>& digits) {
    std::vector<int> result = digits;

    next_permutation_helper<false>(result, 0);

    return result;
  }
};

TEST_CASE_WITH_DATA(NextLargestDigitPermutationTest, tests, NextLargestDigitPermutationTestData, next_largest_digit_permutation_tests) {
  trace << std::endl << "Finding the next largest digit permutation for:" << std::endl;
  trace.vector(data.digits);

  const auto next = next_permutation(data.digits);
  trace << "Result:" << std::endl;
  trace.vector(next);
  trace << "Expected:" << std::endl;
  trace.vector(data.expected);

  assert.equal(next, data.expected);
}
