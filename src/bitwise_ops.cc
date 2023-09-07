#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "bitwise_ops.h"
#include "helpers.h"

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

void test_binary_add(const std::string a, const std::string b, const std::string expected) {
  std::cout << std::endl << "Attempting to binary add " << std::quoted(a) << " and " << std::quoted(b) << "..." << std::endl;

  const auto actual = binary_add(a, b);
  std::cout << "Found: " << std::quoted(actual) << " (expected: " << std::quoted(expected) << ")" << std::endl;
}

void test_count_bits_set(uint32_t n, int expected) {
  std::cout << std::endl << "Counting count of set bits in " << n << "..." << std::endl;

  const auto actual = count_bits_set(n);
  std::cout << "Found: " << actual << " (expected: " << expected << ")" << std::endl;
}

int main_bitwise_ops() {
  test_binary_add("11", "1", "100");
  test_binary_add("1010", "1011", "10101");

  test_count_bits_set(0b00000000000000000000000000001011, 3);
  test_count_bits_set(0b00000000000000000000000010000000, 1);
  test_count_bits_set(0b11111111111111111111111111111101, 31);

  return 0;
}
