#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

struct LargestNumericPalindromeTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string str;
};

std::vector<LargestNumericPalindromeTestData> largest_numeric_palindrome_tests = {
  {"898", "9888"},
  {"100001", "001010"},
  {"777", "777"},
  {"", ""},
  {"5", "5"},
  {"5", "12345"},
  {"909", "909"},
  {"0", "0"},
  {"0", "0000"},
  {"0", "00000"},
  {"1", "000010"},
  {"95259", "5259190"},
  {"95059", "55990"},
  {"7449447", "444947137"},
  {"9", "00009"},
};

class LargestNumericPalindromeTest : public TestCase {
 protected:

  // Given a string containing only digits (0-9), construct the largest possible
  // number which is also a palindrome using only those digits.
  // This is leetcode 2384. Largest Palindromic Number
  // https://leetcode.com/problems/largest-palindromic-number/
  std::string largest_palindrome(const std::string& str) {
    // Just a quick mapping from integer digit to the char letter.
    constexpr std::array<char, 10> digit_to_char = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    // Count the occurence of each digit within |str|.
    std::vector<int> digit_counters(10);
    for (const auto& l : str) {
      const int digit = l - '0';
      digit_counters[digit]++;
    }
    trace << "Found these digit counts:" << std::endl;
    trace.vector(digit_counters);

    // Track if we should add a middle digit.
    // We'll add the largest digit with an odd number of occurences.
    bool use_middle_digit = false;
    int middle_digit = 0;
    // Keep a list representing half of the palindrome.
    // The opposite side of the palindrome is just this list in reverse.
    std::list<int> half_palindrome;

    // Attempt to add digits to the palindrome starting with the most
    // significant digit and moving down the number.
    // At each slot of the palindrome, consider the largest available
    // digit we have with more than two occurences.
    // The largest digit with an odd number of occurences will be used
    // as the middle digit.
    for (int current_digit = 9; current_digit >= 0; current_digit--) {
      auto& counter = digit_counters[current_digit];
      while (counter >= 2) {
        // If we don't have any slots taken up in the palindrome, yet,
        // don't add zero digits to it.
        if (half_palindrome.empty() && current_digit == 0) {
          break;
        }
        // Consume 2 of these digits to form the left and right side
        // in the palindrome.
        counter -= 2;
        half_palindrome.push_back(current_digit);
      }
      // If this is the largest digit we've seen with an odd occurence
      // count, use it as the middle digit.
      if (!use_middle_digit && counter > 0) {
        counter--;
        use_middle_digit = true;
        middle_digit = current_digit;
      }
    }

    std::string result;

    // Add the left half of the palindrome digits.
    for (auto iter = half_palindrome.cbegin(); iter != half_palindrome.cend(); iter++) {
      const auto digit = *iter;
      result.push_back(digit_to_char[digit]);
    }

    // If we need to, add the middle digit.
    if (use_middle_digit) {
      result.push_back(digit_to_char[middle_digit]);
    }

    // Reverse the set of digits and add them to form the right half of the palindrome.
    for (auto iter = half_palindrome.crbegin(); iter != half_palindrome.crend(); iter++) {
      const auto digit = *iter;
      result.push_back(digit_to_char[digit]);
    }

    return result;
  }
};

TEST_CASE_WITH_DATA(LargestNumericPalindromeTest, tests, LargestNumericPalindromeTestData, largest_numeric_palindrome_tests) {
  trace << std::endl << "Attempting to construct the largest palindromatic number from digits in this string: " << std::quoted(data.str) << std::endl;

  const auto actual = largest_palindrome(data.str);
  trace << "Found: " << std::quoted(actual) << std::endl;
  assert.equal(actual, data.expected);
}
