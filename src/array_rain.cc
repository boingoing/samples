#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Given a height map for some terrain, |height|, calculate how many units of
// rain water would be trapped between each element of the map.
// This is leetcode 42. Trapping Rain Water
// https://leetcode.com/problems/trapping-rain-water
int trapped_rain_water(std::vector<int>& height) {
  int total_trapped_blocks = 0;
  // Figure out how many levels we need to search.
  // Also we could probably continue going up until there's only one or no more
  // indices with height greater than |current_height|.
  const int max_height = *std::max_element(height.cbegin(), height.cend());

  // Calculate the trapped blocks one level of the height-map at a
  // time starting from the bottom.
  for (int current_height = 1; current_height <= max_height; current_height++) {
    // Walk from left to right at the current level and count up trapped
    // blocks.
    // A block is trapped if the height at its index is less than |current_height|
    // and there are indices with height more than |current_height| to the right
    // and left of the block.
    // Start at index 0 and walk right.
    // If we see a filled block (index at least at current_height), remember that
    // and keep track of how many empty blocks there are at this level while
    // walking until we reach another filled block on the right or the end of the
    // vector.
    bool saw_filled_block = false;
    int maybe_trapped_blocks = 0;
    for (const auto& h : height) {
      // If the current block is filled
      if (h >= current_height) {
        // If we already saw a filled block, this could be the right-bound
        // of a trapped region. Blocks we identified as maybe trapped should
        // be counted into the total.
        if (saw_filled_block) {
          total_trapped_blocks += maybe_trapped_blocks;
          // Reset our maybe counter, those blocks are counted now.
          maybe_trapped_blocks = 0;
        }
        saw_filled_block = true;
      } else {
        // If the current block is non-filled and we already saw a filled
        // block at this height, then the current block might be trapped
        // if we later see another filled block.
        if (saw_filled_block) {
          maybe_trapped_blocks++;
        }
      }
    }
  }

  return total_trapped_blocks;
}

struct RainWaterTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> height;
};

std::vector<RainWaterTestData> rain_water_tests = {
  {6, {0,1,0,2,1,0,1,3,2,1,2,1}},
  {9, {4,2,0,3,2,5}},
};

class RainWaterTest : public TestCase {};

TEST_CASE_WITH_DATA(RainWaterTest, tests, RainWaterTestData, rain_water_tests) {
  trace << std::endl << "Calculating how much rain water can be trapped by this height map:" << std::endl;
  trace.vector(data.height);

  const auto actual = trapped_rain_water(data.height);
  assert.equal(actual, data.expected);
}
