#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

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

void isolate_island_chunks(std::vector<std::vector<char>>& grid, std::map<char, std::unordered_set<char>>& equivalent_island_map) {
  
}

size_t count_islands(std::vector<std::vector<char>>& grid) {
  const char first_isolated_island = land;
  // Which value should we use to mark the next found island.
  char next_island = first_isolated_island;
  // If we locate an island multiple times as different chunks,
  // map the chunks to each other.
  std::map<char, std::unordered_set<char>> equivalent_island_map;

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
          //    We need to remember that the two chunks are part of the same island.
          //    Keep a mapping from the smaller index to the larger index - the larger
          //    one will be the effective id for the island.
          equivalent_island_map[left_tile].insert(above_tile);
          equivalent_island_map[above_tile].insert(left_tile);
        }
      }
    }
  }

  size_t count = next_island - first_isolated_island;

  // Each island we isolated above may be built out of chunks each with a different index.
  // We don't want to count such an island more than once, though, so remove all the
  // chunks which are equivalent.
  // We kept an entry in the equivalent map for each such chunk so we just need to subtract
  // one for each entries in the equivalent map.
  for (const auto& map_pair : equivalent_island_map) {
    const auto bucket_size = map_pair.second.size();

    if (bucket_size != 0) {
      size_t non_doubled_entry_count = 0;

      std::cout << "Island " << island_to_int(map_pair.first) << " is equivalent to " << bucket_size << " other islands. ( ";
      for (const auto& entry : map_pair.second) {
        if (map_pair.first < entry) {
          non_doubled_entry_count++;
        }
        std::cout << island_to_int(entry) << " ";
      }
      std::cout << ")" << std::endl;

      count -= bucket_size;
    } 
  }

  return count;
}

std::vector<std::vector<char>> get_simple_grid() {
  return {
    {'1', '1', '0', '0', '1', '0'},
    {'1', '1', '0', '0', '1', '1'},
    {'0', '1', '0', '1', '1', '0'},
    {'0', '1', '1', '0', '0', '1'},
    {'0', '0', '0', '0', '0', '1'},
    {'0', '1', '0', '1', '0', '1'},
    {'1', '1', '0', '1', '0', '1'},
    {'0', '1', '1', '1', '0', '1'},
    {'0', '0', '0', '1', '1', '1'},
    {'1', '1', '0', '0', '0', '0'},
    {'0', '1', '1', '0', '0', '0'},
    {'0', '0', '1', '1', '0', '0'},
    {'0', '0', '0', '1', '1', '0'},
    {'0', '0', '0', '0', '1', '1'},
    {'0', '0', '0', '0', '0', '0'},
    {'0', '0', '0', '0', '1', '1'},
    {'0', '0', '0', '1', '1', '0'},
    {'0', '0', '1', '1', '0', '0'},
    {'0', '1', '1', '0', '0', '0'},
    {'1', '1', '0', '0', '0', '0'},
  };
}

std::vector<std::vector<char>> get_complex_grid() {
  return {
    {'1','1','1','1','1','0','1','1','1','1'},
    {'0','1','1','0','1','1','1','0','1','1'},
    {'1','0','1','0','1','1','0','1','0','1'},
    {'1','0','1','1','0','1','1','1','1','1'},
    {'1','1','0','0','1','1','1','1','1','1'},
    {'1','1','0','1','1','1','1','1','1','1'},
    {'1','1','1','1','1','1','1','1','0','1'},
    {'0','1','1','0','1','1','1','1','1','0'},
    {'1','1','0','1','1','0','1','1','1','1'},
    {'0','1','1','1','1','1','0','1','1','1'},
  };
}

std::vector<std::vector<char>> get_medium_complexity_grid() {
  return {
    {'1','1','1','1','1','0','1','1','1','1'},
    {'1','0','1','0','1','1','1','1','1','1'},
    {'0','1','1','1','0','1','1','1','1','1'},
    {'1','1','0','1','1','0','0','0','0','1'},
    {'1','0','1','0','1','0','0','1','0','1'},
    {'1','0','0','1','1','1','0','1','0','0'},
    {'0','0','1','0','0','1','1','1','1','0'},
    {'1','0','1','1','1','0','0','1','1','1'},
    {'1','1','1','1','1','1','1','1','0','1'},
    {'1','0','1','1','1','1','1','1','1','0'},
  };
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
  test_one_grid(get_medium_complexity_grid(), 1);
  test_one_grid(get_complex_grid(), 1);
}
