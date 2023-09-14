#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "helpers.h"
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
