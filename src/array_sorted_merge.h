#ifndef __array_sorted_merge_h__
#define __array_sorted_merge_h__

#include <cstddef>
#include <cstdint>
#include <vector>

// Takes two sorted arrays and merges them.
// |nums1| must have |n| elements at the back of the vector with value 0.
// The elements from |nums2| will be sorted into |nums1| using up those
// extra element slots. 
void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n);

void test_merge(std::vector<int> nums1, int m, std::vector<int> nums2, int n, std::vector<int> expected);

int main_merge();

#endif  // __array_sorted_merge_h__
