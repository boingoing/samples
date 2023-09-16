#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "test/TestCase.h"

struct DistributeChickenTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  int count;
};

std::vector<DistributeChickenTestData> distribute_chicken_tests = {
  {std::vector<int>({2,1,0}), 29},
  {std::vector<int>({1,0,1}), 13},
  {std::vector<int>({1,0,0}), 12},
  {std::vector<int>({1,0,0}), 11},
  {std::vector<int>({1,0,0}), 10},
  {std::vector<int>({0,1,1}), 9},
  {std::vector<int>({0,1,1}), 8},
  {std::vector<int>({0,1,1}), 7},
  {std::vector<int>({0,1,0}), 6},
  {std::vector<int>({0,1,0}), 5},
  {std::vector<int>({0,1,0}), 4},
  {std::vector<int>({0,0,1}), 3},
  {std::vector<int>({0,0,1}), 2},
  {std::vector<int>({0,0,1}), 1},
};

class DistributeChickenTest : public TestCase {
 protected:

  // Given a number of chicken for an order, |count|, distribute the chicken
  // into boxes - L (12x), M (6x), S (3x) - such that the customer always
  // receives at least as many chicken as ordered and the number of extra
  // chicken is minimized and then the number of boxes is minimized.
  std::vector<int> distribute(int count) {
    std::vector<int> boxes;

    // Align count to a multiple of 3 - round up.
    count += (3 - (count % 3)) % 3;

    int cur = count / 12;
    boxes.push_back(cur);
    count -= cur * 12;

    cur = count / 6;
    boxes.push_back(cur);
    count -= cur * 6;

    cur = count / 3;
    boxes.push_back(cur);
    count -= cur * 3;

    return boxes;
  }
};

TEST_CASE_WITH_DATA(DistributeChickenTest, tests, DistributeChickenTestData, distribute_chicken_tests) {
  trace << std::endl << "Finding best distribution of boxes for " << data.count << " chickens:" << std::endl;

  const auto boxes = distribute(data.count);
  trace << "Resulting boxes:" << std::endl;
  trace.vector(boxes);
  trace << "Expected boxes:" << std::endl;
  trace.vector(data.expected);

  assert.equal(boxes, data.expected);
}
