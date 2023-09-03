#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "array_combinations.h"
#include "helpers.h"

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

void test_phone_number_letters(std::string digits, std::vector<std::string> expected) {
  std::cout << std::endl << "Finding letter combinations of phone number " << digits << std::endl;

  const auto actual = phone_number_letters(digits);
  std::cout << "Found: " << std::endl;
  print_vector(actual);
  std::cout << "Expected: " << std::endl;
  print_vector(expected);
}

int main_combinations() {
  test_phone_number_letters("1", {});
  test_phone_number_letters("23", {"ad", "bd", "cd", "ae", "be", "ce", "af", "df", "cf"});

  return 0;
}
