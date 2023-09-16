#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

struct FixUnbalancedParensTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string str;
};

std::vector<FixUnbalancedParensTestData> fix_unbalanced_parens_tests = {
  {"", ")("},
  {"()", "(()"},
  {"", ")"},
  {"", "("},
  {"()()(())", "()())((((())"},
};

class FixUnbalancedParensTest : public TestCase {
 protected:

  std::string fix_unbalanced_string(const std::string& str) {
    int count_open_parens = 0;
    std::string result;
    std::set<size_t> deleted_indices;

    // Walk over the string and keep the count of open parens.
    for (size_t i = 0; i < str.size(); i++) {
      if (str[i] == '(') {
        // This open paren needs a subsequent close paren, track the count.
        count_open_parens++;
      } else {
        if (count_open_parens == 0) {
          // Currently balanced, another close paren will make it unbalanced.
          // Delete this close paren.
          deleted_indices.insert(i);
        } else {
          // This close paren closes the previous open paren.
          count_open_parens--;
        }
      }
    }

    // If there are open parens which were not closed, we can just walk
    // backwards over the string and remove open parens until the string
    // is balanced.
    for (size_t i = str.size(); i != 0 && count_open_parens > 0; i--) {
      if (str[i-1] == '(') {
        deleted_indices.insert(i-1);
        count_open_parens--;
      }
    }

    // Construct result string by copying indices out of |str| and skipping
    // those indices which were marked deleted.
    for (size_t i = 0; i < str.size(); i++) {
      if (deleted_indices.count(i) == 0) {
        result.push_back(str[i]);
      }
    }

    return result;
  }
};

TEST_CASE_WITH_DATA(FixUnbalancedParensTest, tests, FixUnbalancedParensTestData, fix_unbalanced_parens_tests) {
  trace << std::endl << "Attempting to balance parens in: " << std::quoted(data.str) << std::endl;

  const auto actual = fix_unbalanced_string(data.str);
  assert.equal(actual, data.expected);
}
