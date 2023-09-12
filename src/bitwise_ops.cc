#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "helpers.h"
#include "test/TestCase.h"

// Given two binary numbers encoded as strings, return their sum,
// also encoded as a string.
// This is leetcode 67. Add Binary
// https://leetcode.com/problems/add-binary
std::string binary_add(const std::string& a, const std::string& b) {
  const auto* a_cur = a.c_str() + (a.length()-1);
  const auto* b_cur = b.c_str() + (b.length()-1);
  std::string result;
  result.resize(std::max(a.length(), b.length()) + 1);
  auto result_cur = result.length();
  bool carry = false;

  // Walk backwards, adding each digit.
  while (result_cur > 0) {
    bool cur = carry;
    carry = false;

    // While there are still digits in |a|.
    if (a_cur >= a.c_str()) {
      if (*a_cur == '1') {
        carry = cur;
        cur = !cur;
      }
      // Consume digit.
      a_cur--;
    }

    // While there are still digits in |b|.
    if (b_cur >= b.c_str()) {
      if (*b_cur == '1') {
        if (cur) {
          carry = true;
        }
        cur = !cur;
      }
      // Consume digit.
      b_cur--;
    }

    // Result digit.
    result[--result_cur] = cur ? '1' : '0';
  }

  // Remove a leading zero.
  if (result[0] == '0') {
    result.erase(0, 1);
  }

  return result;
}

// Count the number of bits set in a 32-bit int.
// This is leetcode 191. Number of 1 Bits
// https://leetcode.com/problems/number-of-1-bits
int count_bits_set(uint32_t n) {
  int count = 0;

  while (n != 0) {
    count += n & 0b1;
    n = n >> 1;
  }

  return count;
}

struct BinaryStringAddTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string a;
  std::string b;
};

std::vector<BinaryStringAddTestData> binary_string_add_tests = {
  {"100", "11", "1"},
  {"10101", "1010", "1011"},
};

class BinaryStringAddTest : public TestCase {};

TEST_CASE_WITH_DATA(BinaryStringAddTest, tests, BinaryStringAddTestData, binary_string_add_tests) {
  trace << std::endl << "Attempting to binary add " << std::quoted(data.a) << " and " << std::quoted(data.b) << "..." << std::endl;

  const auto actual = binary_add(data.a, data.b);
  assert.equal(actual, data.expected);
}

struct CountBitsSetTestData : TestCaseDataWithExpectedResult<int> {
  uint32_t n;
};

std::vector<CountBitsSetTestData> count_bits_set_tests = {
  {3, 0b00000000000000000000000000001011},
  {1, 0b00000000000000000000000010000000},
  {31, 0b11111111111111111111111111111101},
};

class CountBitsSetTest : public TestCase {};

TEST_CASE_WITH_DATA(CountBitsSetTest, tests, CountBitsSetTestData, count_bits_set_tests) {
  trace << std::endl << "Counting count of set bits in " << data.n << "..." << std::endl;

  const auto actual = count_bits_set(data.n);
  assert.equal(actual, data.expected);
}
