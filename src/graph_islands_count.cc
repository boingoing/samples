#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "graph_islands_count.h"
#include "helpers.h"
#include "test_islands.h"

// Grid holding only 0 or 1 values.
// 0 => water
// 1 => land
typedef std::vector<std::vector<char>> Grid;

#define TileTypeLand '1'
#define TileTypeWater '0'

bool count_islands_helper(Grid& grid, size_t x, size_t y) {
  if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size()) {
    return false;
  }
  if (grid[x][y] != TileTypeLand) {
    return false;
  }

  grid[x][y] = TileTypeWater;

  count_islands_helper(grid, x + 1, y);
  count_islands_helper(grid, x - 1, y);
  count_islands_helper(grid, x, y + 1);
  count_islands_helper(grid, x, y - 1);

  return true;
}

// Given a grid filled with '0' or '1' characters, count the
// total number of "islands" in the grid.
// An island is defined as a set of grid tiles with value '1'
// which are 4-connected - that is (connected via up, down, left,
// or right).
// This is leetcode 200. Number of Islands
// https://leetcode.com/problems/number-of-islands
int count_islands(Grid& grid) {
  int count = 0;
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[0].size(); y++) {
      if (count_islands_helper(grid, x, y)) {
        count++;
      }
    }
  }
  return count;
}

void test_count_islands(Grid grid, int expected) {
  std::cout << std::endl << "Counting islands in grid:" << std::endl;
  print_grid(grid);

  const auto count = count_islands(grid);
  std::cout << std::endl << "Found " << count << " islands (expected " << expected << ")." << std::endl;
}

int main_count_islands() {
  test_count_islands(get_huge_grid(), 6121);
  test_count_islands(get_simple_grid(), 5);
  test_count_islands(get_medium_complexity_grid(), 2);
  test_count_islands(get_complex_grid(), 1);

  return 0;
}
