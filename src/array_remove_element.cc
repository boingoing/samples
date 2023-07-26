#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "array_remove_element.h"
#include "helpers.h"

// Remove all elements matching |val| from the |nums| vector.
// Remaining elements may be returned in any order.
// The length of the |nums| vector does not change.
// The number of remaining elements is returned.
int remove(std::vector<int>& nums, int val) {
  // Start from the back and look at the elements of |nums| in reverse order.
  // When we find an element matching |val|, swap it with the last element in
  // |nums| which does not match |val| (or nothing if there are no such
  // elements).
  // Initially past the end of the vector - no good index identified.
  int current_good_index = static_cast<int>(nums.size());
  // Initially the last element in the vector.
  int current_index = current_good_index - 1;
  // Count of elements with value |val|.
  int count_val = 0;

  while (current_index >= 0) {
    if (nums[current_index] == val) {
      // Remove value at current_index from nums by swapping it with the value
      // at current_good_index.
      // If current_good_index isn't a valid index, that means we haven't yet
      // found a value in nums which is not |val|.
      if (current_good_index < nums.size()) {
        nums[current_index] = nums[current_good_index];
        // The next smaller index must also be good so we can just move our
        // current good index down one.
        // This is the case because, either it was already a good index or
        // we already moved a good value there to replace a bad index in a
        // previous step.
        current_good_index--;
      }

      count_val++;
    } else if (current_good_index == nums.size()) {
      // This is the first good index we've found.
      current_good_index = current_index;
    }

    current_index--;
  }

  return nums.size() - count_val;
}

void test_remove(std::vector<int> nums, int val, std::vector<int> expected_nums, int expected_count) {
  std::cout << std::endl << "Removing all elements of value " << val << " from array:" << std::endl;
  print_vector(nums);

  const auto count = remove(nums, val);
  nums.resize(count);
  std::cout << "Remaining " << count << " elements (expected " << expected_count << ")" << std::endl;
  std::cout << "Resulting array:" << std::endl;
  print_vector(nums);
}

int main_remove() {
  test_remove({3,2,2,3}, 3, {2,2}, 2);
  test_remove({0,1,2,2,3,0,4,2}, 2, {0,1,0,4,3}, 5);
  return 0;
}
