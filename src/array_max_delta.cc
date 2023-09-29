//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "test/TestCase.h"

// Given an array |nums|, find the maximum delta between two
// elements ei and ej where i < j.
// This is leetcode 121. Best Time to Buy and Sell Stock
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock
int array_max_delta(std::vector<int>& nums) {
  size_t min_element_index = 0;
  //size_t max_delta_element_index = 0;
  int best_delta = 0;

  // Walk over the array elements starting from the front.
  // Track the smallest value we've seen in the array.
  // Calculate the delta between the current element and the smallest value and
  // remember the largest such delta.
  for (size_t current_index = 1; current_index < nums.size(); current_index++) {
    // If the current value is the smallest we've seen, the current delta would
    // be negative so skip the rest of the logic.
    if (nums[current_index] < nums[min_element_index]) {
      min_element_index = current_index;
      continue;
    }
    // Calculate the delta between the current value and the smallest element
    // we've seen in |nums| before the |current_index|.
    const int current_delta = nums[current_index] - nums[min_element_index];
    if (current_delta > best_delta) {
      best_delta = current_delta;
      //max_delta_element_index = current_index;
    }
  }
  
  return best_delta;
}

enum class SearchState : uint8_t {
  FindMaxValue,
  FindMinValue,
};

// Given an array |nums|, find the sum of maximum deltas between two
// elements ei and ej where i < j.
// This is leetcode 122. Best Time to Buy and Sell Stock II
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii
int array_max_delta_sum(std::vector<int>& nums) {
  if (nums.size() < 2) {
    return 0;
  }

  // Start by searching for the max value for the current window.
  SearchState state = SearchState::FindMaxValue;
  int current_max_value_index = static_cast<int>(nums.size()) - 1;
  int current_min_value_index = current_max_value_index;
  int delta_sum = 0;

  // Walk backwards over the array.
  // Search for each window creating a delta - that is, one which contains
  // a max element, ej, larger than a min element, ei, where j > i.
  // First, find the largest element we see while walking
  // backwards. This will be the largest element in the window - ej.
  // When we find a smaller element, switch to finding the smallest element.
  // This will be ei.
  // If we're looking for the smallest element but find a larger element,
  // switch to finding the next window with the current element as the max.
  for (int current_index = current_max_value_index - 1; current_index >= 0; current_index--) {
    switch (state) {
    case SearchState::FindMinValue:
      // We're looking for the min value for the current window.
      if (nums[current_index] > nums[current_min_value_index]) {
        // The current value is greater than the min value we've seen.
        // That means the window we are tracking is the current max delta.
        // Add the tracked window delta to our sum.
        delta_sum += nums[current_max_value_index] - nums[current_min_value_index];
        // Reset the window indices.
        current_max_value_index = current_index;
        current_min_value_index = current_index;
        // Change to find the max value for the next window.
        state = SearchState::FindMaxValue;
      } else {
        // Remember that the value is smaller than any we've seen in the window.
        current_min_value_index = current_index;
      }
      break;
    case SearchState::FindMaxValue:
      // We're looking for the max value for the current window.
      if (nums[current_index] < nums[current_max_value_index]) {
        // The current value is less than the max value we've seen.
        // Starting with the |current_index| as the min value, change to
        // look for the min value in this window.
        current_min_value_index = current_index;
        state = SearchState::FindMinValue;
      } else {
        // Remember that this value is larger than any we've seen in the window.
        current_max_value_index = current_index;
      }
      break;
    }
  }

  // If we were searching for the min value of the current window when we
  // walked past the beginning of |nums|, the current window is a delta we
  // should count.
  if (state == SearchState::FindMinValue) {
    delta_sum += nums[current_max_value_index] - nums[current_min_value_index];
  }

  return delta_sum;
}

struct ArrayMaxDeltaTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<ArrayMaxDeltaTestData> array_max_delta_tests = {
  {3, {3,4,2,2,3,5,2,3}},
  {0, {}},
  {0, {1}},
  {0, {1,1}},
  {2, {1,2,3}},
  {1, {2,3,1}},
  {3, {2,3,1,2,3,4}},
  {0, {3,2,1}},
};

class ArrayMaxDeltaTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayMaxDeltaTest, tests, ArrayMaxDeltaTestData, array_max_delta_tests) {
  trace << std::endl << "Searching for the max delta between two elements in array:" << std::endl;
  trace.vector(data.nums);

  const auto delta = array_max_delta(data.nums);
  assert.equal(delta, data.expected);
}

struct ArrayMaxDeltaSumTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<ArrayMaxDeltaSumTestData> array_max_delta_sum_tests = {
  {7, {7,1,5,3,6,4}},
  {2, {1,2,3}},
  {0, {}},
  {0, {1}},
  {0, {1,1}},
  {8, {10,1,2,5,2,3,6,5,4,3,2,1}},
  {15, {1,8,2,10}},
};

class ArrayMaxDeltaSumTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayMaxDeltaSumTest, tests, ArrayMaxDeltaSumTestData, array_max_delta_sum_tests) {
  trace << std::endl << "Searching for the sum of max deltas each between two elements in array:" << std::endl;
  trace.vector(data.nums);

  const auto delta = array_max_delta_sum(data.nums);
  assert.equal(delta, data.expected);
}
