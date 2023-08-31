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

void test_are_isomorphic(std::string s, std::string t, bool expected) {
  std::cout << std::endl << "Looking to see if \"" << s << "\" and \"" << t << "\" are isomorphic" << std::endl;

  const auto actual = are_isomorphic(s, t);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_hashmap_strings() {
  test_are_isomorphic("a", "b", true);
  test_are_isomorphic("egg", "add", true);
  test_are_isomorphic("foo", "bar", false);
  test_are_isomorphic("paper", "title", true);
  test_are_isomorphic("badc", "baba", false);

  return 0;
}
