#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>

#include "hashmap_strings.h"
#include "helpers.h"

// Given two strings, |s| and |t|, determine if they are isomorphic strings.
// This is leetcode 205. Isomorphic Strings
// https://leetcode.com/problems/isomorphic-strings
bool are_isomorphic(std::string& s, std::string& t) {
  if (s.size() != t.size()) {
    return false;
  }

  // Mapping of letters in |s| to counterparts in |t|.
  std::unordered_map<char, char> s_to_t_map;
  // Mapping of letters in |t| to counterparts in |s|.
  std::unordered_map<char, char> t_to_s_map;

  // Walk over the two strings, taking one letter at a time.
  for (size_t i = 0; i < s.size(); i++) {
    const auto s_letter = s[i];
    const auto t_letter = t[i];
    const auto& s_to_t_iter = s_to_t_map.find(s_letter);

    if (s_to_t_iter == s_to_t_map.cend()) {
      const auto& t_to_s_iter = t_to_s_map.find(t_letter);

      if (t_to_s_iter == t_to_s_map.cend()) {
        // If there is no mapping from the letter in |s| and there is no
        // mapping to the letter in |t|, add mappings between the letter
        // in |s| and the letter in |t|.
        s_to_t_map[s_letter] = t_letter;
        t_to_s_map[t_letter] = s_letter;
      } else {
        // If there is no mapping from the letter in |s| but there is a mapping
        // to the letter in |t|, that means that some previous letter in |s|
        // already mapped to the letter in |t|.
        // The strings cannot be isomorphic, then.
        return false;
      }
    } else {
      // If there is already a mapping from the letter in |s|, it must be to
      // the same letter in |t| or the strings are not isomorphic.
      if (s_to_t_iter->second != t_letter) {
        return false;
      }
    }
  }

  return true;
}

// Given a |pattern| and a string, |str|, determine if the words in |str|
// follow the pattern.
// A pattern is defined as a set of letters, where each letter would map to
// a word in the set of words, |str|.
// ex: pattern = "aba", str = "foo bar foo"
// ex: pattern = "aabb", str = "foo foo bar bar"
// Words in |str| are separated by a single space, there are no other whitespace
// characters found in |str|.
// This is leetcode 290. Word Pattern
// https://leetcode.com/problems/word-pattern
bool words_follow_pattern(std::string& pattern, std::string& str) {
  size_t str_index = 0;

  // Mapping of a letter from |pattern| to a word in |str|.
  std::unordered_map<char, std::string> letter_to_word_map;
  // Mapping of a word in |str| to a letter in |pattern|.
  std::unordered_map<std::string, char> word_to_letter_map;

  // Walk over the pattern and words, taking one letter and one word at a time.
  for (const auto& letter : pattern) {
    // If we've reached the end of |str|, there are more letters in pattern than
    // words.
    if (str_index >= str.size()) {
      return false;
    }

    // First find out what the current word is - just find the next space char.
    size_t word_end_index = str_index;
    while (word_end_index < str.size() && str[word_end_index] != ' ') {
      word_end_index++;
    }
    const auto& word = str.substr(str_index, word_end_index - str_index);
    str_index = word_end_index + 1;

    const auto& letter_to_word_iter = letter_to_word_map.find(letter);
    if (letter_to_word_iter == letter_to_word_map.cend()) {
      const auto& word_to_letter_iter = word_to_letter_map.find(word);
      if (word_to_letter_iter == word_to_letter_map.cend()) {
        // No mapping letter <==> word
        letter_to_word_map[letter] = word;
        word_to_letter_map[word] = letter;
      } else {
        // Mapping from word to a different letter.
        return false;
      }
    } else {
      if (letter_to_word_iter->second != word) {
        // Mapping from letter to a different word.
        return false;
      }
    }
  }

  // If the current index is not at the end of |str|, then we have more
  // words than we had letters in the pattern.
  return str_index >= str.size();
}

void test_are_isomorphic(std::string s, std::string t, bool expected) {
  std::cout << std::endl << "Looking to see if \"" << s << "\" and \"" << t << "\" are isomorphic" << std::endl;

  const auto actual = are_isomorphic(s, t);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

void test_words_follow_pattern(std::string pattern, std::string str, bool expected) {
  std::cout << std::endl << "Looking to see if \"" << str << "\" follows the pattern \"" << pattern << "\"" << std::endl;

  const auto actual = words_follow_pattern(pattern, str);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_hashmap_strings() {
  test_are_isomorphic("a", "b", true);
  test_are_isomorphic("egg", "add", true);
  test_are_isomorphic("foo", "bar", false);
  test_are_isomorphic("paper", "title", true);
  test_are_isomorphic("badc", "baba", false);

  test_words_follow_pattern("aba", "foo bar foo", true);
  test_words_follow_pattern("aba", "foo bar bar", false);
  test_words_follow_pattern("aabb", "foo foo bar bar", true);
  test_words_follow_pattern("abba", "dog cat cat dog", true);
  test_words_follow_pattern("abba", "dog cat cat fish", false);
  test_words_follow_pattern("aaaa", "dog cat cat dog", false);
  test_words_follow_pattern("abba", "foo foo foo foo", false);
  test_words_follow_pattern("badc", "foo bar foo baz", false);
  test_words_follow_pattern("aa", "foo foo foo", false);
  test_words_follow_pattern("aa", "foo foo bar", false);
  test_words_follow_pattern("aaaa", "foo", false);

  return 0;
}
