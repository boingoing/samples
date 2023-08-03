#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "array_shift.h"
#include "helpers.h"

// Given an array, |nums|, shift the elements in the array right |k| times.
// Elements at the end of the array wrap-around to the front when shifted.
// ex: [1,2,3,4,5,6] shifted right by 2 is [5,6,1,2,3,4]
void array_shift_right(std::vector<int>& nums, int k) {
  // Allow for shift sizes greater than nums.size()
  k = k % nums.size();

  // Do nothing if shift by 0
  if (k == 0) {
    return;
  }
  
  // Stash away the last |k| elements so we can overwrite them.
  // TODO(boingoing): Remove the need to do this by better figuring out the set
  // of required swaps.
  std::vector<int> last_k;
  last_k.assign(nums.cbegin() + nums.size() - k, nums.cend());

  // Shift all the elements to the right by one starting with element at index k
  size_t current_insert_index = nums.size() - 1;
  size_t current_source_index = nums.size() - 1 - k;
  while (current_insert_index >= k) {
    nums[current_insert_index--] = nums[current_source_index--];
  }

  // Return the previous last |k| stashed elements to the beginning of the array
  for (size_t i = 0; i < k; i++) {
    nums[i] = last_k[i];
  }
}

void test_array_shift_right(std::vector<int> nums, int k, std::vector<int> expected) {
  std::cout << std::endl << "Shifting the elements of array right " << k << " times:" << std::endl;
  print_vector(nums);

  array_shift_right(nums, k);
  std::cout << "Found: " << std::endl;
  print_vector(nums);
  std::cout << "Expected: " << std::endl;
  print_vector(expected);
}

int main_array_shift() {
  test_array_shift_right({3,2,3}, 1, {3,3,2});
  test_array_shift_right({0,1,2,3,4,5}, 0, {0,1,2,3,4,5});
  test_array_shift_right({0,1,2,3,4,5}, 1, {5,0,1,2,3,4});
  test_array_shift_right({0,1,2,3,4,5}, 2, {4,5,0,1,2,3});
  test_array_shift_right({0,1,2,3,4,5}, 3, {3,4,5,0,1,2});
  test_array_shift_right({0,1,2,3,4,5}, 4, {2,3,4,5,0,1});
  test_array_shift_right({0,1,2,3,4,5}, 5, {1,2,3,4,5,0});
  test_array_shift_right({0,1,2,3,4,5}, 6, {0,1,2,3,4,5});
  test_array_shift_right({0,1,2,3,4,5}, 7, {5,0,1,2,3,4});
  test_array_shift_right({0,1,2,3,4,5}, 8, {4,5,0,1,2,3});
  test_array_shift_right({0,1,2,3,4,5}, 9, {3,4,5,0,1,2});
  test_array_shift_right({0,1,2,3,4,5}, 10, {2,3,4,5,0,1});
  test_array_shift_right({0,1,2,3,4,5}, 11, {1,2,3,4,5,0});

  return 0;
}
