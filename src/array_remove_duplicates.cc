#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "array_remove_duplicates.h"
#include "helpers.h"

// Given a sorted array |nums|, remove duplicates from it.
// Remaining values are left in the same order.
// Return the number of unique values in |nums|.
int sorted_array_remove_all_duplicates(std::vector<int>& nums) {
  size_t previous_index = 0;
  size_t current_index = 1;
  size_t swap_index = 1;
  // Count duplicate values found.
  int duplicate_count = 0;
  while (current_index < nums.size()) {
    if (nums[previous_index] >= nums[current_index]) {
      duplicate_count++;
      while(swap_index < nums.size() && nums[previous_index] >= nums[swap_index]) {
        swap_index++;
      }
      if (swap_index < nums.size()) {
        nums[current_index] = nums[swap_index];
      } else {
        // No more non-duplicate elements...
        return current_index;
      }
    }
    current_index++;
    previous_index++;
  }

  return nums.size() - duplicate_count;
}

void test_remove_all_duplicates(std::vector<int> nums, std::vector<int> expected_nums, int expected_count) {
  std::cout << std::endl << "Removing duplicate elements from array:" << std::endl;
  print_vector(nums);

  const auto count = sorted_array_remove_all_duplicates(nums);
  nums.resize(count);
  std::cout << "Remaining " << count << " elements:" << std::endl;
  print_vector(nums);
  std::cout << "Expected " << expected_count << " elements:" << std::endl;
  print_vector(expected_nums);
}

int main_remove_duplicates() {
  test_remove_all_duplicates({1,2,2,3,3}, {1,2,3}, 3);
  test_remove_all_duplicates({1,1,2}, {1,2}, 2);
  test_remove_all_duplicates({1}, {1}, 1);
  test_remove_all_duplicates({1,2,2,3,4}, {1,2,3,4}, 4);
  test_remove_all_duplicates({1,1,1,1,1,1}, {1}, 1);
  test_remove_all_duplicates({}, {}, 0);
  test_remove_all_duplicates({1,1,1,3}, {1,3}, 2);
  test_remove_all_duplicates({1,1,1,3,3,3}, {1,3}, 2);
  test_remove_all_duplicates({1,1,1,3,3,3,4}, {1,3,4}, 3);
  test_remove_all_duplicates({1,1,1,3,3,3,4,4,4}, {1,3,4}, 3);
  test_remove_all_duplicates({1,2,3,4}, {1,2,3,4}, 4);

  return 0;
}
