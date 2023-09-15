#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Given an array of numbers, treat each element as the maximum count of forward
// steps which are allowed to be made from that element.
// Return true if |nums| is solvable.
// The array is considered solvable if there is a path beginning at the first
// element and ending at the last element.
// For example, [2,0,1,4] is solvable by moving from index 0 to index 2 and
// from index 2 to index 3.
// And, [2,1,0,3] is not solvable because there is no path from index 2 - which
// is zero - to the last element, index 3.
// This is leetcode 55. Jump Game
// https://leetcode.com/problems/jump-game
bool can_solve(std::vector<int>& nums) {
  if (nums.size() < 2) {
    return true;
  }

  // The count of steps we need to make in order to reach the current next
  // position.
  size_t current_steps = 0;

  // Walk backwards over the array and search for a from-element with value
  // great enough to walk to the next position - starting with the last index.
  for (int index = nums.size() - 2; index >= 0; index--) {
    // Jumping from |index| to the next position.
    // We need to increment the number of steps required.
    current_steps++;
    // If the value at |index| is larger than |current_steps|, we can jump
    // to the next position from this index.
    if (nums[index] >= current_steps) {
      // Reset the steps counter so we can find out which index we need to jump
      // from to reach the current index.
      current_steps = 0;
    }
  }

  // If we are able to jump from the first index, the game itself is solvable.
  return current_steps == 0;
}

// Find the minimum number of jumps needed to move from the first element to the
// last element in |nums|.
// This is leetcode 45. Jump Game II
// https://leetcode.com/problems/jump-game-ii
int min_jumps(std::vector<int>& nums) {
  int jump_count = 0;
  size_t current_index = 0;
  // Walk forward over the elements in |nums|.
  // For each element, jump to the potential next position which is able to
  // reach the farthest forward in the array.
  while (current_index != nums.size() - 1) {
    size_t max_jump_value = 0;
    size_t max_jump_index = 0;
    for (size_t i = 0; i < nums[current_index]; i++) {
      size_t potential_target_index = current_index + i + 1;
      size_t jump_value = nums[potential_target_index] + potential_target_index;
      if (potential_target_index == nums.size() - 1) {
        return jump_count + 1;
      }
      if (jump_value >= max_jump_value) {
        max_jump_value = jump_value;
        max_jump_index = potential_target_index;
      }
    }
    jump_count++;
    current_index = max_jump_index;
  }
  return jump_count;
}

struct JumpGameCanSolveTestData : TestCaseDataWithExpectedResult<bool> {
  std::vector<int> nums;
};

std::vector<JumpGameCanSolveTestData> jump_game_can_solve_tests = {
  {true, {2,3,1,1,4}},
  {false, {3,2,1,0,4}},
};

class JumpGameCanSolveTest : public TestCase {};

TEST_CASE_WITH_DATA(JumpGameCanSolveTest, tests, JumpGameCanSolveTestData, jump_game_can_solve_tests) {
  trace << std::endl << "Looking to see if this jump game is solvable:" << std::endl;
  trace.vector(data.nums);

  const auto actual = can_solve(data.nums);
  assert.equal(actual, data.expected);
}

struct JumpGameMinJumpsTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<JumpGameMinJumpsTestData> jump_game_min_jumps_tests = {
  {2, {5,3,1,1,1,1,4}},
  {2, {2,3,1,1,4}},
  {2, {2,3,0,1,4}},
  {2, {1,2,3}},
};

class JumpGameMinJumpsTest : public TestCase {};

TEST_CASE_WITH_DATA(JumpGameMinJumpsTest, tests, JumpGameMinJumpsTestData, jump_game_min_jumps_tests) {
  trace << std::endl << "Finding the minimum number of jumps to solve this jump game:" << std::endl;
  trace.vector(data.nums);

  const auto actual = min_jumps(data.nums);
  assert.equal(actual, data.expected);
}
