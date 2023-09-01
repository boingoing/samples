#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "array_maze.h"
#include "helpers.h"

#define VERBOSE 1

typedef std::vector<std::vector<int>> Maze;
typedef std::pair<int,int> Coord;
typedef std::vector<Coord> Path;
const int tile_empty = 0;
const int tile_wall = 1;

// Recursively search for a path from |current| to |end|.
// When called, |path| does not include |current|.
// We push |current| onto |path| and then recursively search the neighbors
// of |current| if |current| is a valid search node.
// If none of the neighbors of |current| has a path to |end|, we pop |current|
// off of |path|.
// A node of the maze is valid to search if the bounds of the node are within
// the maze, if the value of the node is empty (0), and if we have not visited
// the search node, yet.
// Returns true when |path| reaches |end| => |current| == |end| and false
// otherwise.
bool find_path_helper(Maze& maze, Coord& end, Coord current, Path& path) {
  int x = current.first;
  int y = current.second;

#if VERBOSE
  std::cout << "Checking (" << current.first << ", " << current.second << ")...";
#endif

  if (x < 0 || x >= maze.size()) {
#if VERBOSE
  std::cout << " out of bounds in x direction. Skipping." << std::endl;
#endif
    return false;
  }
  if (y < 0 || y >= maze[x].size()) {
#if VERBOSE
  std::cout << " out of bounds in y direction. Skipping." << std::endl;
#endif
    return false;
  }
  if (maze[x][y] == tile_wall) {
#if VERBOSE
  std::cout << " tile value = " << maze[x][y] << ". Skipping." << std::endl;
#endif
    return false;
  }

  // Visit node
  maze[x][y] = tile_wall;
  path.push_back({x,y});

  // Found end point
  if (x == end.first && y == end.second) {
#if VERBOSE
  std::cout << " found end node. Stopping Recursion." << std::endl;
#endif
    return true;
  }

#if VERBOSE
  std::cout << " unvisited node. Recursing to neighbors." << std::endl;
#endif
  // Search to the right, left, up, and down.
  // If any of them reaches the end, return success.
  if (find_path_helper(maze, end, {x,y+1}, path)) {
    return true;
  }
  if (find_path_helper(maze, end, {x,y-1}, path)) {
    return true;
  }
  if (find_path_helper(maze, end, {x+1,y}, path)) {
    return true;
  }
  if (find_path_helper(maze, end, {x-1,y}, path)) {
    return true;
  }

  // There is not a path from current to the end point.
  // Remove current from the path, search in a different direction.
  path.pop_back();
  return false;
}

// Given a maze, starting, and ending coordinates, find a path from |start| to
// |end|.
// If there is no path, returns an empy vector.
Path find_path(Maze& maze, Coord& start, Coord& end) {
  Path path;

  // Begin recursive search at |start|.
  if (find_path_helper(maze, end, start, path)) {
    return path;
  }

  return {};
}

void test_find_path(Maze maze, Coord start, Coord end, Path expected) {
  std::cout << std::endl << "Searching for a path through this maze:" << std::endl;
  for (const auto& row : maze) {
    print_vector(row);
  }

  const auto actual = find_path(maze, start, end);
  std::cout << "Found: " << std::endl;
  for (const auto& coord : actual) {
    std::cout << "(" << coord.first << ", " << coord.second << ") => ";
  }
  std::cout << std::endl << "Expected: " << std::endl;
  for (const auto& coord : expected) {
    std::cout << "(" << coord.first << ", " << coord.second << ") => ";
  }
  std::cout << std::endl;
}

int main_maze() {
  test_find_path({
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6},
    {
      {0, 0},
      {0, 1},
      {0, 2},
      {0, 3},
      {1, 3},
      {2, 3},
      {3, 3},
      {4, 3},
      {4, 4},
      {4, 5},
      {4, 6}});
  test_find_path({
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6},
    {});

  return 0;
}
