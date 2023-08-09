#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "array_h_index.h"
#include "helpers.h"

// Given an array of citation counts, |citations|, calculate the h-index
// for the author. The h-index is the largest value, n, such that there
// are n values found in the array |citations| which are >= n.
// This is leetcode 274. H-Index
// https://leetcode.com/problems/h-index
int h_index(std::vector<int>& citations) {
  // Map from citation count (ie: citations[i]) to the number of occurences
  // of that count in |citations|.
  // ex: [3,3,2,9] -> {9 => 1, 3 => 2, 2 => 1}
  std::map<int, int, std::greater<int>> h_map;

  // Create the above map.
  for (const auto& c : citations) {
    h_map[c]++;
  }

  // Keep track of the numbers of elements from |citations| we've accumulated.
  int element_sum = 0;

  // Walk the map in decreasing key order.
  for (const auto& map_pair : h_map) {
    // Skip 0 values.
    if (map_pair.first == 0) {
      continue;
    }
    // If the current key is smaller than the accumulated sum, we have n values
    // greater than the key where n is the sum. We should use the sum itself as
    // the h-index.
    if (element_sum > map_pair.first) {
      return element_sum;
    }
    // Increment the accumulated sum by the count of mappings.
    element_sum += map_pair.second;
    // If the accumulated sum is now greater than the key, the key is our
    // h-index.
    if (element_sum >= map_pair.first) {
      return map_pair.first;
    }
  }

  // We didn't find any value in the map which itself is the h-index.
  // That means the count of values in |citations| with value > 0 is not larger
  // than the value of any individual citation value > 0.
  // ex: [100,101]
  // In that case, we have two values > 0 but not 100 or 101 values >= 100.
  // So the largest h-value we can return is 2, which is the accumulated sum.
  return element_sum;
}

void test_h_index(std::vector<int> citations, int expected) {
  std::cout << std::endl << "Finding the H-index for these citations:" << std::endl;
  print_vector(citations);

  const auto actual = h_index(citations);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_h_index() {
  test_h_index({1,2,3}, 2);
  test_h_index({1,2,1}, 1);
  test_h_index({3,0,6,1,5}, 3);
  test_h_index({100}, 1);
  test_h_index({100, 15}, 2);
  test_h_index({100, 15, 0}, 2);
  test_h_index({100,15,4,0,0,0}, 3);
  test_h_index({1,7,9,4}, 3);

  return 0;
}
