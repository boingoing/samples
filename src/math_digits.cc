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

struct AddOneTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> digits;
};

std::vector<AddOneTestData> add_one_tests = {
  {std::vector<int>({1,2,3,5}), {1,2,3,4}},
  {std::vector<int>({}), {}},
  {std::vector<int>({1}), {0}},
  {std::vector<int>({0,0,2,0}), {0,0,1,9}},
  {std::vector<int>({1,0}), {9}},
  {std::vector<int>({2,0,0}), {1,9,9}},
  {std::vector<int>({1,0,0,0}), {9,9,9}},
  {std::vector<int>({0,1}), {0,0}},
};

class AddOneTest : public TestCase {
 protected:

  // Given an array, |digits|, where each element is numeric in the range [0-9],
  // the array taken together representing an integer, return the array of
  // digits representing the next integer.
  // The most significant digit is at index 0, the least significant digit is at
  // the end of the digits array.
  // ie: [1,2,3] -> [1,2,4]
  std::vector<int> add_one(const std::vector<int>& digits) {
    // What should an empty set of digits return? Empty set?
    if (digits.size() == 0) {
      return {};
    }

    // Initially copy the digits into result - we don't modify |digits|.
    std::vector<int> result = digits;

    // Walk backwards over the digits.
    // That is, from least significant to most significant digit.
    // Find the point where we need to increment a digit and do so.
    for (size_t i = result.size(); i > 0; i--) {
      const auto index = i - 1;

      // If the current digit is not a 9, we can just increment this digit
      // and break out of the loop - this is the single add.
      if (result[index] != 9) {
        result[index]++;
        break;
      }

      // The current digit is a 9, replace it with a 0 and carry forward to
      // the next most significant digit.
      result[index] = 0;

      // If we reached a 9 at the most significant digit, we'll need to
      // add a new digit to the front of the number.
      if (index == 0) {
        // The new digit must be a 1 because it was implicitly a 0 before.
        result.insert(result.begin(), 1);
        // Technically shouldn't need a break here as index is 0 so the loop
        // will end.
        break;
      }
    }

    return result;
  }
};

TEST_CASE_WITH_DATA(AddOneTest, tests, AddOneTestData, add_one_tests) {
  trace << std::endl << "Adding one to the number formed by these digits:" << std::endl;
  trace.vector(data.digits);

  const auto result = add_one(data.digits);
  trace << "Result:" << std::endl;
  trace.vector(result);
  trace << "Expected:" << std::endl;
  trace.vector(data.expected);

  assert.equal(result, data.expected);
}
