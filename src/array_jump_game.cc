#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "array_jump_game.h"
#include "helpers.h"

// Given an array of numbers, treat each element as the maximum count of forward
// steps which are allowed to be made from that element.
// Return true if |nums| is solvable.
// The array is considered solvable if there is a path beginning at the first
// element and ending at the last element.
// For example, [2,0,1,4] is solvable by moving from index 0 to index 2 and
// from index 2 to index 3.
// And, [2,1,0,3] is not solvable because there is no path from index 2 - which
// is zero - to the last element, index 3.
// This is leetcode #55. Jump Game
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

void test_can_solve(std::vector<int> nums, bool expected) {
  std::cout << std::endl << "Looking to see if this jump game is solvable:" << std::endl;
  print_vector(nums);

  const auto actual = can_solve(nums);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_jump_game() {
  test_can_solve({2,3,1,1,4}, true);
  test_can_solve({3,2,1,0,4}, false);

  return 0;
}
