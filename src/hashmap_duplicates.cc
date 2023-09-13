#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "helpers.h"
#include "test/TestCase.h"

struct ContainsDuplicateTestData : TestCaseDataWithExpectedResult<bool> {
  std::vector<int> nums;
  int k;
};

std::vector<ContainsDuplicateTestData> contains_duplicate_tests = {
  {true, {1,2,3,1}, 3},
  {true, {1,0,1,1}, 1},
  {false, {1,2,3,1,2,3}, 2},
};

class ContainsDuplicateTest : public TestCase {
 protected:

  // Given an array of integers, |nums|, determine if there are two elements
  // in the array with the same value which are located at most |k| apart.
  // That is, two indices i and j where nums[i] == nums[j] and abs(i-j) <= k.
  // This is leetcode 219. Contains Duplicate II
  // https://leetcode.com/problems/contains-duplicate-ii
  bool contains_duplicate(std::vector<int>& nums, int k) {
    // Keep a map of element values to their set of indices in |nums|.
    std::unordered_map<int, std::unordered_set<size_t>> element_to_index_map;

    // Walk over |nums|.
    for (size_t i = 0; i < nums.size(); i++) {
      const auto n = nums[i];
      // Walk over the set of indices which hold the same value as |nums[i]|.
      // This set does not include |i|, yet.
      for (const auto& j : element_to_index_map[n]) {
        const auto delta = i >= j ? i - j : j - i;
        // If abs(i-j) <= k, we've met all the requirements.
        if (delta <= k) {
          return true;
        }
      }
      // We walked over the set of indices but didn't find a matching one within
      // |k|. Insert |i| into the set and continue.
      element_to_index_map[n].insert(i);
    }

    // Didn't find any matching indices meeting the requirements.
    return false;
  }
};

TEST_CASE_WITH_DATA(ContainsDuplicateTest, tests, ContainsDuplicateTestData, contains_duplicate_tests) {
  trace << std::endl << "Looking to see if there are duplicates in this array:" << std::endl;
  trace.vector(data.nums);

  const auto actual = contains_duplicate(data.nums, data.k);
  assert.equal(actual, data.expected);
}
