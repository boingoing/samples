//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <algorithm>
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
#define TileTypeVisited '2'

int largest_enclosed_space_helper(Grid& grid, size_t x, size_t y) {
  // Outside grid
  if (x < 0 || x >= grid.size() || y < 0 || y >= grid[x].size()) {
    return -1;
  }
  // Already visited or a land tile
  if (grid[x][y] != TileTypeWater) {
    return 0;
  }

  grid[x][y] = TileTypeVisited;

  const auto tile_down = largest_enclosed_space_helper(grid, x + 1, y);
  const auto tile_up = largest_enclosed_space_helper(grid, x - 1, y);
  const auto tile_right = largest_enclosed_space_helper(grid, x, y + 1);
  const auto tile_left = largest_enclosed_space_helper(grid, x, y - 1);

  if (tile_down < 0 || tile_up < 0 || tile_right < 0 || tile_left < 0) {
    return -1;
  }

  return 1 + tile_down + tile_up + tile_right + tile_left;
}

// Given a grid filled with '0' or '1' characters, determine the size of the
// largest 4-connected set of '0' tiles which are fully enclosed by '1' tiles.
int largest_enclosed_space(Grid& grid) {
  int largest = 0;
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[0].size(); y++) {
      const auto cur = largest_enclosed_space_helper(grid, x, y);
      largest = std::max(cur, largest);
    }
  }
  return largest;
}

struct LargestEnclosedSpaceTestData : TestCaseDataWithExpectedResult<int> {
  Grid grid;
};

std::vector<LargestEnclosedSpaceTestData> largest_enclosed_space_tests = {
  {380, get_huge_grid()},
  {0, get_simple_grid()},
  {2, get_medium_complexity_grid()},
  {3, get_complex_grid()},

  {0, {
    {'1','0','1'},
    {'0','1','1'},
    {'1','1','0'},
    {'0','0','1'},
    {'0','1','1'}}},

  {3, {
    {'0', '0', '0', '0', '0'},
    {'0', '1', '1', '1', '0'},
    {'1', '0', '0', '1', '0'},
    {'1', '1', '0', '1', '0'},
    {'1', '1', '1', '1', '0'}}},

  {2, {
    {'1', '1', '1', '1'},
    {'1', '0', '0', '1'},
    {'1', '1', '1', '1'}}},

  {0, {
    {'0', '0', '0', '0', '0'},
    {'0', '1', '1', '1', '0'},
    {'1', '0', '0', '0', '0'},
    {'1', '1', '0', '1', '0'},
    {'1', '1', '1', '1', '0'}}},
};

class LargestEnclosedSpaceTest : public TestCase {};

TEST_CASE_WITH_DATA(LargestEnclosedSpaceTest, tests, LargestEnclosedSpaceTestData, largest_enclosed_space_tests) {
  trace << std::endl << "Finding largest enclosed space in grid:" << std::endl;
  trace.grid(data.grid);

  const auto count = largest_enclosed_space(data.grid);
  trace << "Found: " << count << " (expected: " << data.expected << ")" << std::endl;
  assert.equal(count, data.expected);
}
