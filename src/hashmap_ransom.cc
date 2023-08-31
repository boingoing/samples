#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>

#include "hashmap_ransom.h"
#include "helpers.h"

// Given a string, |note|, determine if it can be constructed entirely using
// the letters given by a second string, |letters|.
// This is leetcode 383. Ransom Note
// https://leetcode.com/problems/ransom-note
bool can_construct(std::string& note, std::string& letters) {
  // A mapping from letter to the count of occurences of that letter within
  // |letters|.
  std::unordered_map<char, int> letters_map;

  // Construct the letter mapping.
  // ex: "aab" produces a map with these entries:
  //     'a' => 2
  //     'b' => 1
  for (const auto& letter : letters) {
    letters_map[letter]++;
  }

  // Remove a counter for each letter in the note.
  for (const auto& letter : note) {
    // If there aren't enough counters for |letter|,
    // |note| cannot be constructed.
    if (--letters_map[letter] < 0) {
      return false;
    }
  }

  // All letters were found.
  return true;
}

void test_can_construct(std::string note, std::string letters, bool expected) {
  std::cout << std::endl << "Looking to see if \"" << note << "\" can be constructed using letters from \"" << letters << "\"" << std::endl;

  const auto actual = can_construct(note, letters);
  std::cout << "Found: " << actual << " (expected " << expected << ")" << std::endl;
}

int main_ransom() {
  test_can_construct("a", "b", false);
  test_can_construct("aa", "ab", false);
  test_can_construct("aa", "aab", true);

  return 0;
}
