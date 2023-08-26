#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "string_find_word_under_cursor.h"
#include "helpers.h"

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

void test_word_under_cursor(const std::string& buffer, size_t cursor_index, const std::string& expected) {
  std::cout << std::endl << "Looking for the word at cursor position " << cursor_index << " in string \"" << buffer << "\"" << std::endl;

  const auto actual = word_under_cursor(buffer, cursor_index);
  std::cout << "Found: \"" << actual << "\" (expected \"" << expected << "\")" << std::endl;
}

int main_word_under_cursor() {
  test_word_under_cursor("lorem ipsum something else", 2, "lorem");
  test_word_under_cursor("lorem ipsum something else", 0, "lorem");
  test_word_under_cursor("lorem ipsum something else", 4, "lorem");
  test_word_under_cursor("lorem ipsum something else", 5, "lorem");
  test_word_under_cursor("lorem ipsum something else", 6, "ipsum");
  test_word_under_cursor("lorem ipsum something else", 9, "ipsum");
  test_word_under_cursor("lorem ipsum something else", 10, "ipsum");
  test_word_under_cursor("lorem ipsum something else", 11, "ipsum");
  test_word_under_cursor("lorem ipsum something else", 12, "something");
  test_word_under_cursor("lorem ipsum something else", 15, "something");
  test_word_under_cursor("lorem ipsum something else", 21, "something");
  test_word_under_cursor("lorem ipsum something else", 22, "else");
  test_word_under_cursor("lorem ipsum something else", 26, "else");

  test_word_under_cursor("", 10, "");

  test_word_under_cursor("word1 word2   word3", 12, "");
  test_word_under_cursor("word1 word2   word3", 13, "");
  test_word_under_cursor("word1 word2   word3", 11, "word2");
  test_word_under_cursor("word1 word2   word3", 14, "word3");

  return 0;
}
