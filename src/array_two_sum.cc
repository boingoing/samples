#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "array_sudoku.h"
#include "helpers.h"

// Given a set of numbers and a target value, find two elements from the set
// which sum to |target|.
// There will always be exactly one or no solution in |nums|.
// This is leetcode 1. Two Sum
// https://leetcode.com/problems/two-sum
std::vector<int> find_two_sum(std::vector<int>& nums, int target) {
  // A mapping of the value needed to sum to target and the index of the paired
  // element in |nums|.
  std::unordered_map<int, int> needed_value_to_pair_index_map;

  // Walk over the elements of |nums|.
  for (int i = 0; i < nums.size(); i++) {
    const int n = nums[i];

    // If there's a mapping for n, it maps to the index of m, which is the
    // value such that n + m == target.
    const auto& iter = needed_value_to_pair_index_map.find(n);
    if (iter != needed_value_to_pair_index_map.cend()) {
      return {iter->second, i};
    }

    // There is no mapping for m, the value which added to n equals target.
    // Add a mapping to the index of n, i.
    const int target_minus_n = target - n;
    needed_value_to_pair_index_map[target_minus_n] = i;
  }

  // No pair summing to target found... return empty vector?
  return {};
}

void test_two_sum(std::vector<int> nums, int target, std::vector<int> expected) {
  std::cout << std::endl << "Looking for two elements which sum to " << target << " in ";
  print_vector(nums);

  const auto actual = find_two_sum(nums, target);
  std::cout << "Found:" << std::endl;
  print_vector(actual);
  std::cout << "Expected:" << std::endl;
  print_vector(expected);
}

int main_two_sum() {
  test_two_sum({2,7,11,15}, 9, {0,1});
  test_two_sum({3,2,4}, 6, {1,2});
  test_two_sum({3,3}, 6, {0,1});

  return 0;
}
