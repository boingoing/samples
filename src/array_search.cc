#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "test/TestCase.h"

// Given an array of sorted, unique numbers, |nums|, find the index within the
// array of element |target| or the index where we would insert it if it is not
// found.
// This is leetcode 35. Search Insert Position
// https://leetcode.com/problems/search-insert-position
size_t binary_search(std::vector<int>& nums, int target) {
  size_t left = 0;
  size_t right = nums.size();

  // Just check to see if the target is larger than the largest value in |nums|.
  if (target > nums[nums.size() - 1]) {
    return nums.size();
  }

  // Otherwise, binary search.
  while (left <= right) {
    const size_t current = (left + right) / 2;
    const size_t value = nums[current];
    if (target < value) {
      right = current - 1;
    } else if (target > value) {
      left = current + 1;
    } else {
      return current;
    }
  }

  return left;
}

struct ArraySearchTestData : TestCaseDataWithExpectedResult<size_t> {
  std::vector<int> nums;
  int target;
};

std::vector<ArraySearchTestData> array_search_tests = {
  {2, {1,3,5,6}, 5},
  {1, {1,3,5,6}, 2},
  {4, {1,3,5,6}, 7},
};

class ArraySearchTest : public TestCase {};

TEST_CASE_WITH_DATA(ArraySearchTest, tests, ArraySearchTestData, array_search_tests) {
  trace << std::endl << "Searching for the insert index of " << data.target << " within this array:" << std::endl;
  trace.vector(data.nums);

  const auto actual = binary_search(data.nums, data.target);
  assert.equal(actual, data.expected);
}
