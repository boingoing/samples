#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "test/TestCase.h"

// Given an array, |nums|, shift the elements in the array right |k| times.
// Elements at the end of the array wrap-around to the front when shifted.
// ex: [1,2,3,4,5,6] shifted right by 2 is [5,6,1,2,3,4]
// This is leetcode 189. Rotate Array
// https://leetcode.com/problems/rotate-array
void array_shift_right(std::vector<int>& nums, int k) {
  // Allow for shift sizes greater than nums.size()
  k = k % nums.size();

  // Do nothing if shift by 0
  if (k == 0) {
    return;
  }
  
  // Stash away the last |k| elements so we can overwrite them.
  // TODO(boingoing): Remove the need to do this by better figuring out the set
  // of required swaps.
  std::vector<int> last_k;
  last_k.assign(nums.cbegin() + nums.size() - k, nums.cend());

  // Shift all the elements to the right by one starting with element at index k
  size_t current_insert_index = nums.size() - 1;
  size_t current_source_index = nums.size() - 1 - k;
  while (current_insert_index >= k) {
    nums[current_insert_index--] = nums[current_source_index--];
  }

  // Return the previous last |k| stashed elements to the beginning of the array
  for (size_t i = 0; i < k; i++) {
    nums[i] = last_k[i];
  }
}

struct ArrayShiftRightTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums;
  int k;
};

std::vector<ArrayShiftRightTestData> array_shift_right_tests = {
  {std::vector<int>({3,3,2}), {3,2,3}, 1},
  {std::vector<int>({0,1,2,3,4,5}), {0,1,2,3,4,5}, 0},
  {std::vector<int>({5,0,1,2,3,4}), {0,1,2,3,4,5}, 1},
  {std::vector<int>({4,5,0,1,2,3}), {0,1,2,3,4,5}, 2},
  {std::vector<int>({3,4,5,0,1,2}), {0,1,2,3,4,5}, 3},
  {std::vector<int>({2,3,4,5,0,1}), {0,1,2,3,4,5}, 4},
  {std::vector<int>({1,2,3,4,5,0}), {0,1,2,3,4,5}, 5},
  {std::vector<int>({0,1,2,3,4,5}), {0,1,2,3,4,5}, 6},
  {std::vector<int>({5,0,1,2,3,4}), {0,1,2,3,4,5}, 7},
  {std::vector<int>({4,5,0,1,2,3}), {0,1,2,3,4,5}, 8},
  {std::vector<int>({3,4,5,0,1,2}), {0,1,2,3,4,5}, 9},
  {std::vector<int>({2,3,4,5,0,1}), {0,1,2,3,4,5}, 10},
  {std::vector<int>({1,2,3,4,5,0}), {0,1,2,3,4,5}, 11},
};

class ArrayShiftRightTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayShiftRightTest, tests, ArrayShiftRightTestData, array_shift_right_tests) {
  trace << std::endl << "Shifting the elements of array right " << data.k << " times:" << std::endl;
  trace.vector(data.nums);

  array_shift_right(data.nums, data.k);
  trace << "Found: " << std::endl;
  trace.vector(data.nums);
  trace << "Expected: " << std::endl;
  trace.vector(data.expected);

  assert.equal(data.nums, data.expected);
}
