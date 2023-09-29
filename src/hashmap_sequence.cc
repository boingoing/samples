//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

struct LongestConsecutiveSequenceTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<LongestConsecutiveSequenceTestData> longest_consecutive_sequence_tests = {
  {4, {100,4,200,1,3,2}},
  {9, {0,3,7,2,5,8,4,6,0,1}},
  {3, {1,2,0,1}},
};

class LongestConsecutiveSequenceTest : public TestCase {
 protected:

  // Given an array of numbers, |nums|, find the length of the longest
  // consecutive sequence of numbers found at any positions in the array.
  // ex: [1,2,3] => 3
  //     [3,8,1,7,2] => 3
  // This is leetcode 128. Longest Consecutive Sequence
  // https://leetcode.com/problems/longest-consecutive-sequence
  int longest_consecutive_sequence(std::vector<int>& nums) {
    std::unordered_map<int, int> value_to_length_map;
    int longest = 0;

    for (const auto& n : nums) {
      if (value_to_length_map[n] > 0) {
        continue;
      }

      const auto& prev = value_to_length_map[n - 1];
      const auto& next = value_to_length_map[n + 1];
      int new_sequence_length = 1;

      if (prev == 0) {
        if (next == 0) {
          // First element of a sequence with length 1.
        } else {
          // New element extending a sequence to the left.
          // ie: 3,4,5 + 2

          // Length of the new sequence is the length of the sequence
          // to the right + 1.
          new_sequence_length = next + 1;
          // Update the last element in the sequence, too.
          value_to_length_map[n + next] = new_sequence_length;
        }
      } else {
        if (next == 0) {
          // New element extending a sequence to the right.
          // ie: 3,4,5 + 6

          // Length of the new sequence is the length of the sequence
          // to the left + 1.
          new_sequence_length = prev + 1;
          // Update the first element in the sequence, too.
          value_to_length_map[n - prev] = new_sequence_length;
        } else {
          // New element joining two sequences.
          // ie: [1,2,3] [5,6,7] + 4

          // Length of the new sequence is the length of the sequence
          // to the right + length of the sequence to the left + 1.
          new_sequence_length = next + prev + 1;
          // Update the first and last elements in the sequence, too.
          value_to_length_map[n + next] = new_sequence_length;
          value_to_length_map[n - prev] = new_sequence_length;
        }
      }

      value_to_length_map[n] = new_sequence_length;
      longest = std::max(longest, new_sequence_length);
    }

    return longest;
  }
};

TEST_CASE_WITH_DATA(LongestConsecutiveSequenceTest, tests, LongestConsecutiveSequenceTestData, longest_consecutive_sequence_tests) {
  trace << std::endl << "Finding the length of the longest consecutive sequence in this array:" << std::endl;
  trace.vector(data.nums);

  const auto actual = longest_consecutive_sequence(data.nums);
  assert.equal(actual, data.expected);
}

struct SmallestMissingPositiveIntegerTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<SmallestMissingPositiveIntegerTestData> smallest_missing_positive_integer_tests = {
  {5, {1,3,6,4,1,2}},
  {4, {1,2,3}},
  {4, {3,2,1}},
  {1, {2}},
  {1, {}},
  {1, {0}},
  {1, {-1}},
  {2, {1}},
  {1, {100}},
  {2, {1,1,1,1,1,1}},
  {4, {1,2,3,5,6,7}},
  {5, {-1,0,1,2,3,4,200}},
  {5, {-1,0,1,2,3,4}},
  {1, {-1,-2,-3}},
};

// Given an array of integers, return the smallest positive integer not found
// in the array.
// ex: [4,5,6] => 1
//     [1,2,3] => 4
class SmallestMissingPositiveIntegerTest : public TestCase {
 protected:

  int smallest_missing_positive_integer_obvious(std::vector<int> nums) {
    // Track numbers seen while traversing |nums|.
    std::set<int> seen_n;
    // The smallest number we haven't seen.
    int current_smallest = 1;

    for (const auto& n : nums) {
      seen_n.insert(n);

      // We just saw the number we're currently tracking as the smallest one
      // we haven't yet seen. Find the next smallest number which we haven't
      // yet seen.
      if (n == current_smallest) {
        // Start at the next integer and keep searching forward until the number
        // is not in our set of numbers we've seen.
        current_smallest = n+1;
        while (seen_n.count(current_smallest) > 0) {
          current_smallest++;
        }
      }
    }

    return current_smallest;
  }

  int smallest_missing_positive_integer_sorting(std::vector<int> &nums) {
    // Zero-size vector, just use 1.
    if (nums.size() == 0) {
      return 1;
    }
    // Vector of a single element. If it's 1, we need to return 2. Otherwise 1.
    if (nums.size() == 1) {
      if (nums[0] == 1) {
        return 2;
      }
      return 1;
    }

    // Sort the elements of the vector.
    std::sort(nums.begin(), nums.end());

    // If the largest element is negative or zero, use 1.
    if (nums.back() < 1) {
      return 1;
    }

    // Start by assuming the smallest missing number is 1.
    int current_smallest = 1;
    // Walk forward over the array.
    for (const auto& cur : nums) {
      // We might have many elements < 1 which we can just ignore.
      if (cur < current_smallest) {
        continue;
      }
      // The first number to be larger than the smallest integer we're tracking
      // indicates there's a gap between the previous at least one integer wide.
      if (cur > current_smallest) {
        return current_smallest;
      }
      // When cur == current_smallest the next possible smallest missing integer
      // is cur + 1, the next integer.
      current_smallest = cur + 1;
    }
    return current_smallest;
  }
};

TEST_CASE_WITH_DATA(SmallestMissingPositiveIntegerTest, obvious_test, SmallestMissingPositiveIntegerTestData, smallest_missing_positive_integer_tests) {
  trace << std::endl << "Finding the smallest missing positive integer (via hashmap) in this array:" << std::endl;
  trace.vector(data.nums);

  const auto actual = smallest_missing_positive_integer_obvious(data.nums);
  assert.equal(actual, data.expected);
}

TEST_CASE_WITH_DATA(SmallestMissingPositiveIntegerTest, sorted_test, SmallestMissingPositiveIntegerTestData, smallest_missing_positive_integer_tests) {
  trace << std::endl << "Finding the smallest missing positive integer (via sorting) in this array:" << std::endl;
  trace.vector(data.nums);

  const auto actual = smallest_missing_positive_integer_sorting(data.nums);
  assert.equal(actual, data.expected);
}
