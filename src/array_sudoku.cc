#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

std::vector<bool> is_number_found_set;

// Unmark all numbers in the found set.
void init_found_set() {
  is_number_found_set.resize(9);
  is_number_found_set.assign(9, false);
}

// Mark that |num| was found.
// Returns false when |num| is invalid.
bool set_found_number(int num) {
  // Validate |num|.
  // TODO: This will validate the same num multiple times,
  //       split it out so that it's only checked once.
  if (num < 1 || num > 9) {
    return false;
  }
  is_number_found_set[num - 1] = true;
  return true;
}

// Returns true if every number [1,9] was marked as found in the found set.
bool were_all_numbers_found() {
  // Return false if any number is not marked as found.
  for (const auto& b : is_number_found_set) {
    if (!b) {
      return false;
    }
  }

  // All numbers accounted for.
  return true;
}

// Determine if the 3x3 sub-grid with upper-left coordinates (x,y)
// is valid.
// The sub-grid is valid if it contains exactly one each of every number in the
// range [1,9]
bool is_sub_grid_valid(const std::vector<std::vector<int>>& grid, size_t x, size_t y) {
  init_found_set();

  // Walk over all elements of the sub-grid and mark off each found number.
  for (size_t i = x; i < x + 3; i++) {
    for (size_t j = y; j < y + 3; j++) {
      if (!set_found_number(grid[i][j])) {
        return false;
      }
    }
  }

  return were_all_numbers_found();
}

// Determine if a sudoku puzzle grid is valid.
// Validity is only defined here as:
// - Every tile value is in the range [1,9]
// - Every 3x3 sub-grid contains exactly one each of every number in the range [1,9]
// - Every row and column contains exactly one each of every number in the range [1,9]
bool is_grid_valid(std::vector<std::vector<int>>& grid) {
  // Check each 3x3 sub-grid to make sure they include every number in the range [1,9]
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      if (!is_sub_grid_valid(grid, i * 3, j * 3)) {
        return false;
      }
    }
  }

  // Check to make sure each row has every number in the range [1,9]
  for (size_t i = 0; i < 9; i++) {
    init_found_set();
    for (size_t j = 0; j < 9; j++) {
      if (!set_found_number(grid[i][j])) {
        return false;
      }
    }
    // Some number was missing from the row.
    if (!were_all_numbers_found()) {
      return false;
    }
  }

  // Check to make sure each column has every number in the range [1,9]
  for (size_t i = 0; i < 9; i++) {
    init_found_set();
    for (size_t j = 0; j < 9; j++) {
      if (!set_found_number(grid[j][i])) {
        return false;
      }
    }
    // Some number was missing from the column.
    if (!were_all_numbers_found()) {
      return false;
    }
  }

  // All sub-grids, rows, and columns have expected set of numbers.
  return true;
}

struct SudokuGridValidatorTestData : TestCaseDataWithExpectedResult<bool> {
  std::vector<std::vector<int>> grid;
};

std::vector<SudokuGridValidatorTestData> sudoku_grid_validator_tests = {
  {false, {
    {8,9,3,5,4,6,7,1,2},
    {2,6,4,9,7,1,8,5,3},
    {7,5,1,3,2,8,6,4,9},
    {1,3,6,14,5,9,2,7,8},
    {5,2,8,6,1,7,3,9,4},
    {4,7,9,8,3,2,0,6,1},
    {3,1,7,2,6,4,9,8,5},
    {9,4,5,7,8,3,1,2,6},
    {6,8,2,1,9,5,4,3,7}}},
  {false, {
    {8,9,3,5,4,6,7,1,2},
    {2,9,4,9,7,1,8,5,3},
    {7,5,1,3,2,8,6,4,9},
    {1,3,6,4,5,9,2,7,8},
    {5,2,8,6,1,7,3,9,4},
    {4,7,9,8,3,2,5,6,1},
    {3,1,7,2,6,4,9,8,5},
    {9,4,5,7,4,3,1,2,6},
    {6,8,2,1,9,5,4,3,7}}},
  {false, {
    {8,9,3,5,4,6,7,1,2},
    {2,6,4,9,7,1,8,5,3},
    {7,5,1,3,2,8,6,4,9},
    {1,3,6,4,5,9,2,7,8},
    {5,2,8,6,1,7,3,9,4},
    {4,7,9,8,3,2,5,6,1},
    {3,1,7,2,6,4,9,8,5},
    {4,9,5,7,8,3,1,2,6},
    {6,8,2,1,9,5,4,3,7}}},
  {false, {
    {1,2,3,4,5,6,7,8,9},
    {3,1,2,6,4,5,9,7,8},
    {2,3,1,5,6,4,8,9,7},
    {7,8,9,1,2,3,4,5,6},
    {9,7,8,3,1,2,6,4,5},
    {8,9,7,2,3,1,5,6,4},
    {4,5,6,7,8,9,1,2,3},
    {6,4,5,9,7,8,3,1,2},
    {5,6,4,8,9,7,2,3,1}}},
  {true, {
    {8,9,3,5,4,6,7,1,2},
    {2,6,4,9,7,1,8,5,3},
    {7,5,1,3,2,8,6,4,9},
    {1,3,6,4,5,9,2,7,8},
    {5,2,8,6,1,7,3,9,4},
    {4,7,9,8,3,2,5,6,1},
    {3,1,7,2,6,4,9,8,5},
    {9,4,5,7,8,3,1,2,6},
    {6,8,2,1,9,5,4,3,7}}},
};

class SudokuGridValidatorTest : public TestCase {};

TEST_CASE_WITH_DATA(SudokuGridValidatorTest, tests, SudokuGridValidatorTestData, sudoku_grid_validator_tests) {
  trace << std::endl << "Looking to see if this sudoku grid is valid:" << std::endl;
  trace.grid(data.grid);

  const auto actual = is_grid_valid(data.grid);
  assert.equal(actual, data.expected);
}
