//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

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

struct SortedArrayMerge3TestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums1;
  std::vector<int> nums2;
  std::vector<int> nums3;
};

std::vector<SortedArrayMerge3TestData> sorted_array_merge3_tests = {
  {std::vector<int>({1,1,2,2,3,5,6,7}), {1,2,3}, {2,5,6}, {1,7}},
  {std::vector<int>({1,2,2,3,3}), {1}, {2,3}, {2,3}},
  {std::vector<int>({1}), {1}, {}, {}},
  {std::vector<int>({}), {}, {}, {}},
};

class SortedArrayMerge3Test : public TestCase {
 protected:

  // Merge 3 sorted int arrays into a single array.
  std::vector<int> merge3(std::vector<int>& nums1, std::vector<int>& nums2, std::vector<int>& nums3) {
    std::vector<int> result;

    // Keep iterators and flags for each source array to track the
    // current index we should take from each array and remember
    // when we've exhausted the elements from the array.
    auto iter1 = nums1.cbegin();
    auto iter2 = nums2.cbegin();
    auto iter3 = nums3.cbegin();

    auto n1_done = iter1 == nums1.cend();
    auto n2_done = iter2 == nums2.cend();
    auto n3_done = iter3 == nums3.cend();

    // Use a big dumb set of conditionals to figure out which source
    // array has the next smallest element.
    while (!n1_done || !n2_done || !n3_done) {
      int cur;
      if (n1_done) {
        if (n2_done) {
          // nums1 and nums2 at end. Take from nums3.
          cur = *iter3;
          iter3++;
        } else {
          if (n3_done) {
            // nums1 and nums3 at end. Take from nums2.
            cur = *iter2;
            iter2++;
          } else {
            // nums1 at end. Take min between nums2 and nums3.
            if (*iter2 < *iter3) {
              cur = *iter2;
              iter2++;
            } else {
              cur = *iter3;
              iter3++;
            }
          }
        }
      } else {
        if (n2_done) {
          if (n3_done) {
            // nums2 and nums3 at end. Take from nums1.
            cur = *iter1;
            iter1++;
          } else {
            // nums2 at end. Take min between nums1 and nums3.
            if (*iter1 < *iter3) {
              cur = *iter1;
              iter1++;
            } else {
              cur = *iter3;
              iter3++;
            }
          }
        } else {
          if (n3_done) {
            // nums3 at end. Take min between nums1 and nums2.
            if (*iter1 < *iter2) {
              cur = *iter1;
              iter1++;
            } else {
              cur = *iter2;
              iter2++;
            }
          } else {
            // No vectors at end. Take min between all three.
            if (*iter1 < *iter2) {
              if (*iter1 < *iter3) {
                cur = *iter1;
                iter1++;
              } else {
                cur = *iter3;
                iter3++;
              }
            } else {
              if (*iter2 < *iter3) {
                cur = *iter2;
                iter2++;
              } else {
                cur = *iter3;
                iter3++;
              }
            }
          }
        }
      }

      // It might be preferable to pre-allocate the result vector but
      // this is probably not the point of the excercise.
      result.push_back(cur);

      // We moved one of the iterators forward, just reset all the flags.
      n1_done = iter1 == nums1.cend();
      n2_done = iter2 == nums2.cend();
      n3_done = iter3 == nums3.cend();
    }
    return result;
  }
};

TEST_CASE_WITH_DATA(SortedArrayMerge3Test, tests, SortedArrayMerge3TestData, sorted_array_merge3_tests) {
  trace << std::endl << "Merging three sorted arrays:" << std::endl;
  trace << "nums1 = ";
  trace.vector(data.nums1);
  trace << "nums2 = ";
  trace.vector(data.nums2);
  trace << "nums3 = ";
  trace.vector(data.nums3);

  const auto result = merge3(data.nums1, data.nums2, data.nums3);
  trace << "sorted = ";
  trace.vector(result);

  assert.equal(result, data.expected);
}
