#ifndef __array_remove_element_h__
#define __array_remove_element_h__

#include <cstddef>
#include <cstdint>
#include <vector>

// Remove all elements matching |val| from the |nums| vector.
// Remaining elements may be returned in any order.
// The length of the |nums| vector does not change.
// The number of removed elements is returned.
int remove(std::vector<int>& nums, int val);

void test_remove(std::vector<int> nums, int val, std::vector<int> expected_nums, int expected_count);

int main_remove();

#endif  // __array_remove_element_h__
