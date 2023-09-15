#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Only detect space characters as white space.
// TODO: We ought to check for other whitespace.
bool is_character(char letter) {
  return letter != ' ';
}

// Given a string |buffer|, find the word within |buffer| at |cursor_index|.
// A word is defined to be at |cursor_index| if |cursor_index| >= word start index
// and |cursor_index| <= word end index.
// If there isn't a word at |cursor_index|, returns empty string.
std::string word_under_cursor(const std::string& buffer, size_t cursor_index) {
  // First look forward in |buffer| to see where the end of the word is.
  size_t right_index = cursor_index;
  while (right_index < buffer.length() && is_character(buffer[right_index])) {
    right_index++;
  }
  // Now look backwards to see where the word begins.
  size_t left_index = cursor_index;
  while (left_index > 0 && left_index <= buffer.length() && is_character(buffer[left_index - 1])) {
    left_index--;
  }
  // If we didn't find any non-whitespace characters.
  if (right_index == left_index) {
    return "";
  }
  return buffer.substr(left_index, right_index - left_index);
}

struct WordUnderCursorTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string buffer;
  size_t cursor_index;
};

std::vector<WordUnderCursorTestData> word_under_cursor_tests = {
    {"lorem", "lorem ipsum something else", 0},
    {"lorem", "lorem ipsum something else", 2},
    {"lorem", "lorem ipsum something else", 4},
    {"lorem", "lorem ipsum something else", 5},
    {"ipsum", "lorem ipsum something else", 6},
    {"ipsum", "lorem ipsum something else", 9},
    {"ipsum", "lorem ipsum something else", 10},
    {"ipsum", "lorem ipsum something else", 11},
    {"something", "lorem ipsum something else", 12},
    {"something", "lorem ipsum something else", 15},
    {"something", "lorem ipsum something else", 21},
    {"else", "lorem ipsum something else", 22},
    {"else", "lorem ipsum something else", 26},

    {"", "", 10},

    {"", "word1 word2   word3", 12},
    {"", "word1 word2   word3", 13},
    {"word2", "word1 word2   word3", 11},
    {"word3", "word1 word2   word3", 14},
};

class WordUnderCursorTest : public TestCase {};

TEST_CASE_WITH_DATA(WordUnderCursorTest, tests, WordUnderCursorTestData, word_under_cursor_tests) {
  trace << std::endl << "Looking for the word at cursor position " << data.cursor_index << " in string \"" << data.buffer << "\"" << std::endl;

  const auto actual = word_under_cursor(data.buffer, data.cursor_index);
  assert.equal(actual, data.expected);
}
