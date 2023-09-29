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

#include "test_islands.h"
#include "test/TestCase.h"

// Grid holding only 0 or 1 values.
// 0 => water
// 1 => land
typedef std::vector<std::vector<char>> Grid;

#define TileTypeLand '1'
#define TileTypeWater '0'

int largest_island_helper(Grid& grid, size_t x, size_t y) {
  if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) {
    return 0;
  }
  if (grid[x][y] != TileTypeLand) {
    return 0;
  }

  grid[x][y] = TileTypeWater;

  return 1
      + largest_island_helper(grid, x + 1, y)
      + largest_island_helper(grid, x - 1, y)
      + largest_island_helper(grid, x, y + 1)
      + largest_island_helper(grid, x, y - 1);
}

// Given a grid filled with '0' or '1' characters, find the
// largest "island" in the grid.
// An island is defined as a set of grid tiles with value '1'
// which are 4-connected - that is (connected via up, down, left,
// or right).
int largest_island(Grid& grid) {
  int largest = 0;
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[0].size(); y++) {
      const auto cur = largest_island_helper(grid, x, y);
      if (cur > largest) {
        largest = cur;
      }
    }
  }
  return largest;
}

struct LargestIslandTestData : TestCaseDataWithExpectedResult<int> {
  Grid grid;
};

std::vector<LargestIslandTestData> largest_island_tests = {
  {288, get_huge_grid()},
  {16, get_simple_grid()},
  {68, get_medium_complexity_grid()},
  {79, get_complex_grid()},

  {5, {
    {'1','0','1'},
    {'0','1','1'},
    {'1','1','0'},
    {'0','0','1'},
    {'0','1','1'}}},
};

class LargestIslandTest : public TestCase {};

TEST_CASE_WITH_DATA(LargestIslandTest, tests, LargestIslandTestData, largest_island_tests) {
  trace << std::endl << "Finding largest island in grid:" << std::endl;
  trace.grid(data.grid);

  const auto count = largest_island(data.grid);
  assert.equal(count, data.expected);
}
