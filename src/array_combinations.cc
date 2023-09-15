#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

// Mapping of integer phone number digit to the set of letters which that
// digit can represent.
const std::vector<std::vector<char>> number_to_letter_map = {
    {},
    {},
    {'a','b','c'},
    {'d','e','f'},
    {'g','h','i'},
    {'j','k','l'},
    {'m','n','o'},
    {'p','q','r','s'},
    {'t','u','v'},
    {'w','x','y','z'}
};

// For each string in |existing_strings|, insert copies with each letter in |new_chars| appended
// and return a new vector including all the copies.
std::vector<std::string> explode(const std::vector<std::string>& existing_strings, const std::vector<char>& new_chars) {
  // TODO: Don't create a new vector
  std::vector<std::string> new_strings;
  for (const auto& letter : new_chars) {
    if (existing_strings.size() == 0) {
      new_strings.push_back(std::string() + letter);
    } else {
      for (const auto& str : existing_strings) {
        new_strings.push_back(str + letter);
      }
    }
  }
  return new_strings;
}

// Given a phone number, |digits|, return all letter combinations which
// can be made out of the digits of the number.
// This is leetcode 17. Letter Combinations of a Phone Number
// https://leetcode.com/problems/letter-combinations-of-a-phone-number
std::vector<std::string> phone_number_letters(std::string& digits) {
  std::vector<std::string> results;
  for (const auto& c : digits) {
    // Convert digit letter into int.
    const int n = c - '0';
    results = explode(results, number_to_letter_map[n]);
  }
  return results;
}

struct PhoneNumberLetterCombinationsTestData : TestCaseDataWithExpectedResult<std::vector<std::string>> {
  std::string digits;
};

std::vector<PhoneNumberLetterCombinationsTestData> phone_number_letters_tests = {
  {std::vector<std::string>({"ad", "bd", "cd", "ae", "be", "ce", "af", "bf", "cf"}), "23"},
  {{}, "1"},
};

class PhoneNumberLetterCombinationsTest : public TestCase {};

TEST_CASE_WITH_DATA(PhoneNumberLetterCombinationsTest, tests, PhoneNumberLetterCombinationsTestData, phone_number_letters_tests) {
  trace << std::endl << "Finding letter combinations of phone number " << data.digits << std::endl;

  const auto actual = phone_number_letters(data.digits);
  trace << "Found: " << std::endl;
  trace.vector(actual);
  trace << "Expected: " << std::endl;
  trace.vector(data.expected);
  assert.equivalent(actual, data.expected);
}
