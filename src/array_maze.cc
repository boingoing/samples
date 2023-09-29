//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "test/TestCase.h"

#define VERBOSE 0

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
bool find_shortest_path_helper(Maze& maze, Coord& end, Coord current, Path& path) {
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
  // If any of them reaches the end, return the shortest found path.
  bool found_path = false;
  // Store the shortest path if we found any valid ones.
  Path shortest_path;
  // Nested calls modify path, so create a copy.
  Path temp_path = path;
  // Nested calls also modify maze, create a copy of it, too.
  Maze temp_maze = maze;
  if (find_shortest_path_helper(temp_maze, end, {x,y+1}, temp_path)) {
    if (!found_path || temp_path.size() < shortest_path.size()) {
      shortest_path = temp_path;
    }
    found_path = true;
  }
  temp_path = path;
  temp_maze = maze;
  if (find_shortest_path_helper(temp_maze, end, {x,y-1}, temp_path)) {
    if (!found_path || temp_path.size() < shortest_path.size()) {
      shortest_path = temp_path;
    }
    found_path = true;
  }
  temp_path = path;
  temp_maze = maze;
  if (find_shortest_path_helper(temp_maze, end, {x+1,y}, temp_path)) {
    if (!found_path || temp_path.size() < shortest_path.size()) {
      shortest_path = temp_path;
    }
    found_path = true;
  }
  temp_path = path;
  temp_maze = maze;
  if (find_shortest_path_helper(temp_maze, end, {x-1,y}, temp_path)) {
    if (!found_path || temp_path.size() < shortest_path.size()) {
      shortest_path = temp_path;
    }
    found_path = true;
  }

  if (found_path) {
    path = shortest_path;
    return true;
  }

  // There is not a path from current to the end point.
  // Remove current from the path, search in a different direction.
  path.pop_back();
  return false;
}

// Given a maze, starting, and ending coordinates, find the shortest path from
// |start| to |end|.
// If there is no path, returns an empy vector.
Path find_shortest_path(Maze& maze, Coord& start, Coord& end) {
  Path path;

  // Begin recursive search at |start|.
  if (find_shortest_path_helper(maze, end, start, path)) {
    return path;
  }

  return {};
}

struct MazeFindPathTestData : TestCaseDataWithExpectedResult<Path> {
  Maze maze;
  Coord start;
  Coord end;
};

std::vector<MazeFindPathTestData> maze_find_path_tests = {
  {Path({
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
    {4, 6}}),
    {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6}},
  {Path({}), {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6}},
};

class MazeFindPathTest : public TestCase {};

TEST_CASE_WITH_DATA(MazeFindPathTest, tests, MazeFindPathTestData, maze_find_path_tests) {
  trace << std::endl << "Searching for a path through this maze:" << std::endl;
  trace.grid(data.maze);

  const auto actual = find_path(data.maze, data.start, data.end);
  trace << "Found: " << std::endl;
  for (const auto& coord : actual) {
    trace << "(" << coord.first << ", " << coord.second << ") => ";
  }
  trace << std::endl << "Expected: " << std::endl;
  for (const auto& coord : data.expected) {
    trace << "(" << coord.first << ", " << coord.second << ") => ";
  }
  trace << std::endl;

  // TODO: Assert path found is correct.
}

struct MazeFindShortestPathTestData : TestCaseDataWithExpectedResult<Path> {
  Maze maze;
  Coord start;
  Coord end;
};

std::vector<MazeFindShortestPathTestData> maze_find_shortest_path_tests = {
  {Path({
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
    {4, 6}}),
    {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6}},
  {Path({}), {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 0, 0, 0, 0}},
    {0, 0},
    {4, 6}},
};

class MazeFindShortestPathTest : public TestCase {};

TEST_CASE_WITH_DATA(MazeFindShortestPathTest, tests, MazeFindShortestPathTestData, maze_find_shortest_path_tests) {
  trace << std::endl << "Searching for the shortest path through this maze:" << std::endl;
  trace.grid(data.maze);

  const auto actual = find_shortest_path(data.maze, data.start, data.end);
  trace << "Found: " << std::endl;
  for (const auto& coord : actual) {
    trace << "(" << coord.first << ", " << coord.second << ") => ";
  }
  trace << std::endl << "Expected: " << std::endl;
  for (const auto& coord : data.expected) {
    trace << "(" << coord.first << ", " << coord.second << ") => ";
  }
  trace << std::endl;

  // TODO: Assert path found is correct.
}
