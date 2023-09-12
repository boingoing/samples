#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>

#include "helpers.h"
#include "test/TestCase.h"

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

struct CanConstructTestData : TestCaseDataWithExpectedResult<bool> {
  std::string note;
  std::string letters;
};

std::vector<CanConstructTestData> can_construct_tests = {
  {false, "a", "b"},
  {false, "aa", "ab"},
  {true, "aa", "aab"},
};

class CanConstructTest : public TestCase {};

TEST_CASE_WITH_DATA(CanConstructTest, tests, CanConstructTestData, can_construct_tests) {
  trace << std::endl << "Looking to see if \"" << data.note << "\" can be constructed using letters from \"" << data.letters << "\"" << std::endl;

  const auto actual = can_construct(data.note, data.letters);
  assert.equal(actual, data.expected);
}
