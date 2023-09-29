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

// Given a sorted array |nums|, remove duplicates from it.
// Remaining values are left in the same order.
// Return the number of unique values in |nums|.
// This is leetcode 26. Remove Duplicates from Sorted Array
// https://leetcode.com/problems/remove-duplicates-from-sorted-array
size_t sorted_array_remove_all_duplicates(std::vector<int>& nums) {
  // Track previous starting at the first element.
  size_t previous_index = 0;
  // Consider duplicates starting with the second element - it's not possible
  // for the first element alone to be a duplicate.
  size_t current_index = 1;
  // Which index we would swap into |current_index| when we determine that
  // current is a duplicate and needs to be replaced with the next larger value
  // in |nums|.
  size_t swap_index = 1;
  // Count duplicate values found.
  size_t duplicate_count = 0;
  // Walk forward over the array keeping track of the current index we want to
  // check for duplicates against, the previous index (probably don't need as
  // it's always current_index - 1 ?), and the index we've walked forward to
  // and swapped from.
  while (current_index < nums.size()) {
    // If |nums[previous_index]| == |nums[current_index]|, the element at
    // |current_index| is a duplicate. It was duplicated in the source array.
    // If |nums[previous_index]| > |nums[current_index]|, the element at
    // |current_index| is a duplicate. The element at |previous_index| was
    // also a duplicate (the same value, probably) and was already swapped with
    // a larger value from further right in the array (that's why it is larger
    // now).
    if (nums[previous_index] >= nums[current_index]) {
      duplicate_count++;
      // Walk |swap_index| forward until we either reach a larger element or
      // the end of the array.
      while(swap_index < nums.size() && nums[previous_index] >= nums[swap_index]) {
        swap_index++;
      }
      // If we didn't reach the end of the array, the element at |swap_index|
      // is larger than the element at |current_index|. Swap it into place.
      if (swap_index < nums.size()) {
        nums[current_index] = nums[swap_index];
      } else {
        // No more non-duplicate elements exist in |nums|. We know the count of
        // unique elements. It's |current_index|.
        return current_index;
      }
    }
    current_index++;
    previous_index++;
  }

  return nums.size() - duplicate_count;
}

// Given a sorted array |nums|, remove duplicates from it, leaving at-most
// 2 duplicates of each value.
// Remaining values are left in the same order.
// Return the number of elements in |nums|.
// This is leetcode 80. Remove Duplicates from Sorted Array II
// https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/
size_t sorted_array_remove_all_but_2_duplicates(std::vector<int>& nums) {
  size_t previous_previous_index = 0;
  size_t previous_index = 1;
  size_t current_index = 2;
  size_t swap_index = 2;
  // Count duplicate values found.
  size_t duplicate_count = 0;
  while (current_index < nums.size()) {
    if ((nums[previous_previous_index] == nums[previous_index] && nums[previous_index] == nums[current_index]) ||
        (nums[previous_index] > nums[current_index])) {
      // If we are looking at a value we swapped from further in the array, this isn't a real duplicate.
      if (swap_index > previous_index) {
        duplicate_count++;
      }
      while(swap_index < nums.size() && nums[previous_previous_index] >= nums[swap_index] && nums[previous_index] >= nums[swap_index]) {
        swap_index++;
      }
      if (swap_index < nums.size()) {
        nums[current_index] = nums[swap_index];
      } else {
        // No more non-duplicate elements...
        return current_index;
      }
    }
    current_index++;
    previous_index++;
    previous_previous_index++;
  }

  return nums.size() - duplicate_count;
}

struct ArrayRemoveAllDuplicatesTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums;
};

std::vector<ArrayRemoveAllDuplicatesTestData> array_remove_all_duplicates_tests = {
  {std::vector<int>({1,2,3}), {1,2,2,3,3}},
  {std::vector<int>({1,2}), {1,1,2}},
  {std::vector<int>({1}), {1}},
  {std::vector<int>({1,2,3,4}), {1,2,2,3,4}},
  {std::vector<int>({1}), {1,1,1,1,1,1}},
  {std::vector<int>({}), {}},
  {std::vector<int>({1,3}), {1,1,1,3}},
  {std::vector<int>({1,3}), {1,1,1,3,3,3}},
  {std::vector<int>({1,3,4}), {1,1,1,3,3,3,4}},
  {std::vector<int>({1,3,4}), {1,1,1,3,3,3,4,4,4}},
  {std::vector<int>({1,2,3,4}), {1,2,3,4}},
};

class ArrayRemoveAllDuplicatesTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayRemoveAllDuplicatesTest, tests, ArrayRemoveAllDuplicatesTestData, array_remove_all_duplicates_tests) {
  trace << std::endl << "Removing duplicate elements from array:" << std::endl;
  trace.vector(data.nums);

  const auto count = sorted_array_remove_all_duplicates(data.nums);
  data.nums.resize(count);
  trace << "Resulting array:" << std::endl;
  trace.vector(data.nums);

  assert.equal(data.nums, data.expected);
}

struct ArrayRemoveAllButTwoDuplicatesTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums;
};

std::vector<ArrayRemoveAllButTwoDuplicatesTestData> array_remove_all_but_2_duplicates_tests = {
  {std::vector<int>({1,2,2,3,3}), {1,2,2,3,3}},
  {std::vector<int>({1}), {1}},
  {std::vector<int>({}), {}},
  {std::vector<int>({1,1,2,2,3,3}), {1,1,2,2,3,3}},
  {std::vector<int>({1,1,2,2,3,3}), {1,1,1,1,2,2,2,2,3,3,3,3}},
  {std::vector<int>({1,2,3}), {1,2,3}},
  {std::vector<int>({1,1}), {1,1,1,1,1,1}},
  {std::vector<int>({1,1,2,2,3}), {1,1,1,2,2,3}},
};

class ArrayRemoveAllButTwoDuplicatesTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayRemoveAllButTwoDuplicatesTest, tests, ArrayRemoveAllButTwoDuplicatesTestData, array_remove_all_but_2_duplicates_tests) {
  trace << std::endl << "Removing all but 2 duplicate elements from array:" << std::endl;
  trace.vector(data.nums);

  const auto count = sorted_array_remove_all_but_2_duplicates(data.nums);
  data.nums.resize(count);
  trace << "Resulting array:" << std::endl;
  trace.vector(data.nums);

  assert.equal(data.nums, data.expected);
}
