#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "test/TestCase.h"

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

struct TwoSumTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums;
  int target;
};

std::vector<TwoSumTestData> two_sum_tests = {
  {std::vector<int>({0,1}), {2,7,11,15}, 9},
  {std::vector<int>({1,2}), {3,2,4}, 6},
  {std::vector<int>({0,1}), {3,3}, 6},
};

class TwoSumTest : public TestCase {};

TEST_CASE_WITH_DATA(TwoSumTest, tests, TwoSumTestData, two_sum_tests) {
  trace << std::endl << "Looking for two elements which sum to " << data.target << " in ";
  trace.vector(data.nums);

  const auto actual = find_two_sum(data.nums, data.target);
  assert.equivalent(actual, data.expected);
}
