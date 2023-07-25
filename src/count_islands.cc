#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test_islands.h"

#define island_type_small char

const char water = '0';
const char land = '1';

char get_left(std::vector<std::vector<char>>& grid, size_t x, size_t y) {
  if (x == 0) {
    return water;
  }
  return grid[x-1][y];
}

char get_right(std::vector<std::vector<char>>& grid, size_t x, size_t y) {
  if (x == grid.size() - 1) {
    return water;
  }
  return grid[x+1][y];
}

char get_above(std::vector<std::vector<char>>& grid, size_t x, size_t y) {
  if (y == 0) {
    return water;
  }
  return grid[x][y-1];
}

char get_below(std::vector<std::vector<char>>& grid, size_t x, size_t y) {
  if (y == grid[0].size() - 1) {
    return water;
  }
  return grid[x][y+1];
}

int32_t island_to_int(char island_id) {
  return static_cast<int32_t>(island_id - water);
}

void print_grid(std::vector<std::vector<char>>& grid) {
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[x].size(); y++) {
      std::cout << std::setw(3) << island_to_int(grid[x][y]) << " ";
    }
    std::cout << std::endl;
  }
}

// Returns the numver of island chunks isolated.
// Equivalent chunks, that is, chunks which are connected and, thus, part of a
// larger island are indicated via |equivalent_island_map|.
void isolate_island_chunks(std::vector<std::vector<char>>& grid, std::map<char, std::unordered_set<char>>& equivalent_island_map) {
  const char first_isolated_island = land;
  // Which value should we use to mark the next found island.
  char next_island = first_isolated_island;

  // Walk over the grid one row at a time starting from the top left tile.
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[x].size(); y++) {
      if (grid[x][y] != water) {
        const auto left_tile = get_left(grid, x, y);
        const auto above_tile = get_above(grid, x, y);
        
        // We found a land tile.
        // There are several cases we can find ourselves in now.
        if (left_tile == water && above_tile == water) {
          // 1. The left tile and the above tile do not belong to an
          //    island (both are water).
          //    The current tile is the start of a new island.
          //    By default the island chunk is equivalent to itself.
          equivalent_island_map[next_island].insert(next_island);
          grid[x][y] = next_island++;
        } else if (above_tile == water) {
          // 2. Only the left tile belongs to an island (above is water).
          //    The current tile belongs to the same island.
          grid[x][y] = left_tile;
        } else if (left_tile == water) {
          // 3. Only the above tile belongs to an island (left is water).
          //    The current tile belongs to the same island.
          grid[x][y] = above_tile;
        } else if (left_tile == above_tile) {
          // 4. The left tile and above tile belong to the same island.
          //    The current tile belongs to the same island.
          grid[x][y] = left_tile;
        } else {
          // 5. The left tile and above tile belong to different islands.
          //    The two island chunks are part of the same island, connected
          //    through the current tile.
          //    The current tile belongs to either of the two island chunks.
          grid[x][y] = left_tile;
          //    We need to remember that the two chunks are part of the same
          //    island.
          //    Keep a mapping from the smaller index to the larger index - the
          //    larger one will be the effective id for the island.
          equivalent_island_map[left_tile].insert(above_tile);
          equivalent_island_map[above_tile].insert(left_tile);
        }
      }
    }
  }
}

void merge_equivalent_island_map(std::map<char, std::unordered_set<char>>& equivalent_island_map) {
  for (auto& map_pair : equivalent_island_map) {
    const auto to_id = map_pair.first;
    std::stack<char> working_set;
    for (const auto& from_id : map_pair.second) {
      working_set.push(from_id);
    }
    while (!working_set.empty()) {
      const auto& from_id = working_set.top();
      working_set.pop();
      if (to_id == from_id) {
        continue;
      }
      auto& equivalent_set = equivalent_island_map[from_id];
      for (const auto& nested_id : equivalent_set) {
        map_pair.second.insert(nested_id);
        working_set.push(nested_id);
      }
      equivalent_set.clear();
    }
  }
}

size_t count_unique_islands(std::map<char, std::unordered_set<char>>& equivalent_island_map) {
  size_t count = 0;
  for (const auto& map_pair : equivalent_island_map) {
    if (!map_pair.second.empty()) {
      count++;
    }
  }
  return count;
}

size_t count_islands(std::vector<std::vector<char>>& grid) {
  // If we locate an island multiple times as different chunks,
  // map the chunks to each other.
  std::map<char, std::unordered_set<char>> equivalent_island_map;

  isolate_island_chunks(grid, equivalent_island_map);
  merge_equivalent_island_map(equivalent_island_map);
  return count_unique_islands(equivalent_island_map);
}

void test_one_grid(std::vector<std::vector<char>> grid, size_t expected_count) {
  std::cout << "Searching for islands in grid:" << std::endl;
  print_grid(grid);

  std::cout << std::endl << "Isolating islands..." << std::endl;
  const auto count = count_islands(grid);
  std::cout << std::endl << "Found islands:" << std::endl;
  print_grid(grid);

  std::cout << std::endl << "Found " << count << " islands (expected " << expected_count << ")." << std::endl;
  std::cout << ((count == expected_count) ? "PASSED!" : "FAILED!") << std::endl;
}

int main() {
  test_one_grid(get_simple_grid(), 5);
  test_one_grid(get_medium_complexity_grid(), 2);
  test_one_grid(get_complex_grid(), 1);
  test_one_grid(get_huge_grid(), 6121);
}
