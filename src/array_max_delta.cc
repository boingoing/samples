#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "array_max_delta.h"
#include "helpers.h"

// Given an array |nums|, find the maximum delta between two
// elements ei and ej where i < j.
int array_max_delta(std::vector<int>& nums) {
  size_t min_element_index = 0;
  //size_t max_delta_element_index = 0;
  int best_delta = 0;

  // Walk over the array elements starting from the front.
  // Track the smallest value we've seen in the array.
  // Calculate the delta between the current element and the smallest value and
  // remember the largest such delta.
  for (size_t current_index = 1; current_index < nums.size(); current_index++) {
    // If the current value is the smallest we've seen, the current delta would
    // be negative so skip the rest of the logic.
    if (nums[current_index] < nums[min_element_index]) {
      min_element_index = current_index;
      continue;
    }
    // Calculate the delta between the current value and the smallest element
    // we've seen in |nums| before the |current_index|.
    const int current_delta = nums[current_index] - nums[min_element_index];
    if (current_delta > best_delta) {
      best_delta = current_delta;
      //max_delta_element_index = current_index;
    }
  }
  
  return best_delta;
}

void test_array_max_delta(std::vector<int> nums, int expected_delta) {
  std::cout << std::endl << "Searching for the max delta between two elements in array:" << std::endl;
  print_vector(nums);

  const auto delta = array_max_delta(nums);
  std::cout << "Found: " << delta << " (expected: " << expected_delta << ")" << std::endl;
}

int main_max_delta() {
  test_array_max_delta({3,4,2,2,3,5,2,3}, 3);
  test_array_max_delta({}, 0);
  test_array_max_delta({1}, 0);
  test_array_max_delta({1,1}, 0);
  test_array_max_delta({1,2,3}, 2);
  test_array_max_delta({2,3,1}, 1);
  test_array_max_delta({2,3,1,2,3,4}, 3);
  test_array_max_delta({3,2,1}, 0);

  return 0;
}
