#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "array_search.h"
#include "helpers.h"

// Given an array of sorted, unique numbers, |nums|, find the index within the
// array of element |target| or the index where we would insert it if it is not
// found.
// This is leetcode 35. Search Insert Position
// https://leetcode.com/problems/search-insert-position
int binary_search(std::vector<int>& nums, int target) {
  int left = 0;
  int right = nums.size();

  // Just check to see if the target is larger than the largest value in |nums|.
  if (target > nums[nums.size() - 1]) {
    return nums.size();
  }

  // Otherwise, binary search.
  while (left <= right) {
    const int current = (left + right) / 2;
    const int value = nums[current];
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

void test_binary_search(std::vector<int> nums, int target, int expected) {
  std::cout << std::endl << "Searching for the insert index of " << target << " within this array:" << std::endl;
  print_vector(nums);

  const auto actual = binary_search(nums, target);
  std::cout << "Found: " << actual << " (expected: " << expected << ")" << std::endl;
}

int main_array_search() {
  test_binary_search({1,3,5,6}, 5, 2);
  test_binary_search({1,3,5,6}, 2, 1);
  test_binary_search({1,3,5,6}, 7, 4);

  return 0;
}
