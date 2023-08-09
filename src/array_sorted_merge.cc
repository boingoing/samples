#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "array_sorted_merge.h"
#include "helpers.h"

// Takes two sorted arrays and merges them.
// |nums1| must have |n| elements at the back of the vector with value 0.
// The elements from |nums2| will be sorted into |nums1| using up those
// extra element slots.
// This is leetcode 88. Merge Sorted Array
// https://leetcode.com/problems/merge-sorted-array/
void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
  // Start at the back of nums1, move the bigger elements from nums1 and nums2 to the back.
  int current_insert_point = m + n - 1;
  int current_nums1_index = m - 1;
  int current_nums2_index = n - 1;

  while (current_insert_point >= 0) {
    int current_biggest;

    if (current_nums1_index < 0) {
      // No more elements to take from nums1
      current_biggest = nums2[current_nums2_index--];
    } else if (current_nums2_index < 0) {
      // No more elements to take from nums2
      current_biggest = nums1[current_nums1_index--];
    } else if (nums1[current_nums1_index] > nums2[current_nums2_index]) {
      // Last element in nums1 is larger than the last one in nums2
      current_biggest = nums1[current_nums1_index--];
    } else {
      // Last element in nums2 is larger than the last one in nums1
      current_biggest = nums2[current_nums2_index--];
    }

    nums1[current_insert_point] = current_biggest;
    current_insert_point--;
  }
}

void test_merge(std::vector<int> nums1, int m, std::vector<int> nums2, int n, std::vector<int> expected) {
  std::cout << std::endl << "Merging two sorted arrays:" << std::endl;
  std::cout << "nums1 = ";
  print_vector(nums1);
  std::cout << "nums2 = ";
  print_vector(nums2);

  merge(nums1, m, nums2, n);
  std::cout << "sorted = ";
  print_vector(nums1);
}

int main_merge() {
  test_merge({1,2,3,0,0,0}, 3, {2,5,6}, 3, {1,2,2,3,4,5});
  test_merge({0}, 0, {1}, 1, {1});
  return 0;
}
