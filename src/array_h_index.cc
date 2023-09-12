#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "helpers.h"
#include "test/TestCase.h"

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

struct ArrayHIndexTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> citations;
};

std::vector<ArrayHIndexTestData> array_h_index_tests = {
  {2, {1,2,3}},
  {1, {1,2,1}},
  {3, {3,0,6,1,5}},
  {1, {100}},
  {2, {100, 15}},
  {2, {100, 15, 0}},
  {3, {100,15,4,0,0,0}},
  {3, {1,7,9,4}},
};

class ArrayHIndexTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayHIndexTest, tests, ArrayHIndexTestData, array_h_index_tests) {
  trace << std::endl << "Finding the H-index for these citations:" << std::endl;
  trace.vector(data.citations);

  const auto actual = h_index(data.citations);
  assert.equal(actual, data.expected);
}
