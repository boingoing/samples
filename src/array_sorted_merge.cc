#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Takes two sorted arrays and merges them.
// The first array, |nums1|, must have |m| + |n| total elements where m
// are the elements to be considered part of |nums1| followed by |n| empty
// elements with value 0.
// The second array, |nums2|, must have size equal to |n|.
// The elements from |nums2| will be sorted into |nums1| using up those
// empty element slots.
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

    nums1[current_insert_point--] = current_biggest;
  }
}

struct SortedArrayMergeTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums1;
  std::vector<int> nums2;
  int m;
  int n;
};

std::vector<SortedArrayMergeTestData> sorted_array_merge_tests = {
  {std::vector<int>({1,2,2,3,5,6}), {1,2,3,0,0,0}, {2,5,6}, 3, 3},
  {std::vector<int>({1}), {0}, {1}, 0, 1},
};

class SortedArrayMergeTest : public TestCase {};

TEST_CASE_WITH_DATA(SortedArrayMergeTest, tests, SortedArrayMergeTestData, sorted_array_merge_tests) {
  trace << std::endl << "Merging two sorted arrays:" << std::endl;
  trace << "nums1 = ";
  trace.vector(data.nums1);
  trace << "nums2 = ";
  trace.vector(data.nums2);

  merge(data.nums1, data.m, data.nums2, data.n);
  trace << "sorted = ";
  trace.vector(data.nums1);

  assert.equal(data.nums1, data.expected);
}
