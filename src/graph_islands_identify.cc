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

// A non-recursive solution to the "count islands" problem which also
// identifies the islands in the grid.
// The algorithm which isolates the island chunks is iterative and runs
// in O(n) time where n is the count of tiles in the grid.
// After isolating the chunks, we then need to combine equivalent chunks
// - that is chunks which are actually part of the same larger island -
// in order to find the actual count of islands and full shape of each
// island.

#define VERBOSE_DEBUG 0

#define island_type_small char
#define island_type_large uint32_t

const island_type_small water = '0';
const island_type_small land = '1';

template<typename island_type>
island_type get_left(std::vector<std::vector<island_type>>& grid, size_t x, size_t y) {
  if (x == 0) {
    return water;
  }
  return grid[x-1][y];
}

template<typename island_type>
island_type get_above(std::vector<std::vector<island_type>>& grid, size_t x, size_t y) {
  if (y == 0) {
    return water;
  }
  return grid[x][y-1];
}

template<typename island_type>
int32_t island_to_int(island_type island_id) {
  return static_cast<int32_t>(island_id - water);
}

template<typename island_type>
void print_equivalent_map(std::map<island_type, std::unordered_set<island_type>>& equivalent_island_map) {
  for (const auto& map_pair : equivalent_island_map) {
    std::cout << island_to_int(map_pair.first) << " -> [ ";
    for (const auto& eq_id : map_pair.second) {
      std::cout << island_to_int(eq_id) << ", ";
    }
    std::cout << "]" << std::endl;
  }
}

template<typename island_type>
void print_grid(std::vector<std::vector<island_type>>& grid) {
  for (size_t x = 0; x < grid.size(); x++) {
    for (size_t y = 0; y < grid[x].size(); y++) {
      std::cout << std::setw(3) << island_to_int(grid[x][y]) << " ";
    }
    std::cout << std::endl;
  }
}

// Modifies the grid to mark each identified island chunk with a unique id.
// Equivalent chunks, that is, chunks with different ids which are connected
// and, thus, part of a larger island are indicated via |equivalent_island_map|.
template<typename island_type>
void isolate_island_chunks(std::vector<std::vector<island_type>>& grid,
    std::map<island_type, std::unordered_set<island_type>>& equivalent_island_map) {
  const island_type first_isolated_island = land;
  // Which value should we use to mark the next found island.
  island_type next_island = first_isolated_island;

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
          equivalent_island_map[left_tile].insert(above_tile);
          equivalent_island_map[above_tile].insert(left_tile);
        }
      }
    }
  }
}

// The equivalent island map is a set of island chunks which are part of the
// same, connected island but have different island ids.
// During island chunk isolation, we inserted mappings between equivalent chunks
// but we need to process that set of mappings so that we can know which chunks
// make up each unique island.
template<typename island_type>
void merge_equivalent_island_map(std::map<island_type, std::unordered_set<island_type>>& equivalent_island_map) {
  // We have all the equivalent mappings discovered by walking the grid.
  // But these are all just mappings between two island chunks at the point
  // where they connect.
  // Effectively, these act as a list allowing us to find all the island chunks
  // belonging to each island.
  // Consider this grid:
  // [ [ "1", "0", "1", "1", "0", "1" ],
  //   [ "1", "0", "0", "1", "1", "1" ],
  //   [ "1", "1", "1", "1", "0", "0" ] ]
  // We would isolate that into these island chunks:
  // [ [ "1", "0", "2", "2",  "0", "3"  ],
  //   [ "1", "0", "0", "2",  "2", "2*" ],
  //   [ "1", "1", "1", "1*", "0", "0"  ] ]
  // While isolating, we would identify two places where island chunks connect:
  // Tile marked "1*" connects chunk 1 and chunk 2.
  // Tile marked "2*" connects chunk 2 and chunk 3.
  // The equivalent island map would look like this:
  // "1" -> [ "1", "2" ]
  // "2" -> [ "2", "1" ]
  // "3" -> [ "3", "2" ]
  // In reality, there is only one island in this grid but there is no direct
  // mapping from "1" -> "3" however we can discover this mapping by walking
  // the map and merging entries down.
  // When we're done, the equivalent map will look like this:
  // "1" -> [ "1", "2", "3" ]
  // "2" -> []
  // "3" -> []
  for (auto& map_pair : equivalent_island_map) {
    // To which island chunk does the set of mappings belong?
    const auto to_id = map_pair.first;
    // Push all the equivalent island chunks of |to_id| onto a stack.
    std::stack<island_type> working_set;
    for (const auto& from_id : map_pair.second) {
      working_set.push(from_id);
    }
    // Process entries off of the stack until it's empty.
    while (!working_set.empty()) {
      const auto& from_id = working_set.top();
      working_set.pop();
      // Skip any mappings back to |to_id|, that would cause a loop.
      if (to_id == from_id) {
        continue;
      }
      // Get the set of equivalent island chunks for |from_id|.
      auto& equivalent_set = equivalent_island_map[from_id];
      for (const auto& nested_id : equivalent_set) {
        // And for each chunk id, add it into the equivalent set for |to_id|.
        map_pair.second.insert(nested_id);
        // Push the id onto the stack so we will recursively walk the
        // equivalent set for that id.
        working_set.push(nested_id);
      }
      // We know that |from_id| is part of the same island as |to_id| and we
      // added all the equivalent chunk ids from |from_id| to the set for
      // |to_id|.
      // Now, delete the equivalent mappings for |from_id| so we don't further
      // process them.
      // Note: This will remove the self-equivalence mapping ("2" -> [ "2" ])
      //       too. This is how we will identify unique islands - they will not
      //       have an empty equivalence map.
      equivalent_set.clear();
    }
  }
}

// Search the equivalent island map for island chunks which do not have an empty
// equivalence map. These are the unique islands.
template<typename island_type>
size_t count_unique_islands(std::map<island_type, std::unordered_set<island_type>>& equivalent_island_map) {
  size_t count = 0;
  for (const auto& map_pair : equivalent_island_map) {
    if (!map_pair.second.empty()) {
      count++;
    }
  }
  return count;
}

// Count the number of islands located in a grid of '0' and '1' characters.
// Only consider two tiles of the grid connected if they are 4-connected,
// via left, right, above, or below.
// Perform island chunk isolation, merge the equivalent island map, and return
// the count of unique islands.
template<typename island_type>
size_t count_islands(std::vector<std::vector<island_type>>& grid) {
  // If we locate an island multiple times as different chunks,
  // map the chunks to each other.
  std::map<island_type, std::unordered_set<island_type>> equivalent_island_map;

#if VERBOSE_DEBUG
  std::cout << std::endl << "Searching for islands in grid:" << std::endl;
  print_grid(grid);
#endif

  isolate_island_chunks(grid, equivalent_island_map);

#if VERBOSE_DEBUG
  std::cout << std::endl << "Found island chunks:" << std::endl;
  print_grid(grid);

  std::cout << std::endl << "Equivalent island mapping:" << std::endl;
  print_equivalent_map(equivalent_island_map);
#endif

  merge_equivalent_island_map(equivalent_island_map);

#if VERBOSE_DEBUG
  std::cout << std::endl << "Merged equivalent island mapping:" << std::endl;
  print_equivalent_map(equivalent_island_map);
#endif

  return count_unique_islands(equivalent_island_map);
}

// If there are going to be more island chunks than can be held in a char (island_type_small), expand
// the elements of the grid vector.
std::vector<std::vector<island_type_large>> convert_grid_to_large_id(std::vector<std::vector<island_type_small>>& grid) {
  std::vector<std::vector<island_type_large>> large_grid;

  large_grid.resize(grid.size());
  auto iter = large_grid.begin();
  for (const auto& row : grid) {
    (*iter).assign(row.begin(), row.end());
    iter++;
  }

  return large_grid;
}

struct IdentifyIslandsTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<std::vector<char>> grid;
};

std::vector<IdentifyIslandsTestData> identify_islands_tests = {
  {5, get_simple_grid()},
  {2, get_medium_complexity_grid()},
  {1, get_complex_grid()},
};

class IdentifyIslandsTest : public TestCase {
 public:
  template<typename island_type>
  void testOneGrid(std::vector<std::vector<island_type>> grid, size_t expected_count) {
    const auto count = count_islands(grid);
    assert.equal(count, expected_count);
  }
};

TEST_CASE_WITH_DATA(IdentifyIslandsTest, tests, IdentifyIslandsTestData, identify_islands_tests) {
  testOneGrid(data.grid, data.expected);
}

TEST_CASE(IdentifyIslandsTest, huge_grid) {
  auto huge_grid = get_huge_grid();
  testOneGrid(convert_grid_to_large_id(huge_grid), 6121);
}
