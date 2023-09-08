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

void test_largest_island(Grid grid, int expected) {
  std::cout << std::endl << "Finding largest island in grid:" << std::endl;
  print_grid(grid);

  const auto count = largest_island(grid);
  std::cout << std::endl << "Found " << count << " (expected " << expected << ")." << std::endl;
}

int main_largest_island() {
  test_largest_island(get_huge_grid(), 288);
  test_largest_island(get_simple_grid(), 16);
  test_largest_island(get_medium_complexity_grid(), 68);
  test_largest_island(get_complex_grid(), 79);

  test_largest_island({
      {'1','0','1'},
      {'0','1','1'},
      {'1','1','0'},
      {'0','0','1'},
      {'0','1','1'}}, 5);

  return 0;
}
