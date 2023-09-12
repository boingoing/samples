#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "helpers.h"
#include "test/TestCase.h"

// Create a mapping of values from |nums| to the count of occurences of that
// value in the array.
void get_element_count_map(const std::vector<int>& nums, std::map<int, int>& element_count_map) {
  for (const auto& n : nums) {
    element_count_map[n]++;
  }
}

// Given an array |nums|, return the majority element.
// Majority element is defined as the element which occurs more than
// nums.size() / 2 times in the array.
// Assumes a majority element is present in |nums|.
// This is leetcode 169. Majority Element
// https://leetcode.com/problems/majority-element
int array_majority_element(std::vector<int>& nums) {
  std::map<int, int> element_count_map;
  get_element_count_map(nums, element_count_map);
  
  const size_t floor = nums.size() / 2;
  for (const auto& p : element_count_map) {
    if (p.second > floor) {
      return p.first;
    }
  }

  // Error?
  return 0;
}

// Given an array |nums|, return the set of majority elements.
// Majority element is defined as any element which occurs more than
// nums.size() / 3 times in the array.
// This is leetcode 229. Majority Element II
// https://leetcode.com/problems/majority-element-ii/
std::vector<int> array_majority_elements(std::vector<int>& nums) {
  std::map<int, int> element_count_map;
  get_element_count_map(nums, element_count_map);

  std::vector<int> return_elements;
  const size_t floor = nums.size() / 3;
  for (const auto& p : element_count_map) {
    if (p.second > floor) {
      return_elements.push_back(p.first);
    }
  }

  return return_elements;
}

struct ArrayMajorityElementTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> nums;
};

std::vector<ArrayMajorityElementTestData> array_majority_element_tests = {
  {3, {3,2,3}},
  {2, {2,2,1,1,1,2,2}},
  {1, {1}},
};

class ArrayMajorityElementTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayMajorityElementTest, tests, ArrayMajorityElementTestData, array_majority_element_tests) {
  trace << std::endl << "Searching for the majority element (occurance > n/2) in array:" << std::endl;
  trace.vector(data.nums);

  const auto e = array_majority_element(data.nums);
  assert.equal(e, data.expected);
}

struct ArrayMajorityElementsTestData : TestCaseDataWithExpectedResult<std::vector<int>> {
  std::vector<int> nums;
};

std::vector<ArrayMajorityElementsTestData> array_majority_elements_tests = {
  {std::vector<int>({3}), {3,2,3}},
  {std::vector<int>({1,2}), {1,2}},
  {std::vector<int>({1}), {1}},
  {{}, {}},
};

class ArrayMajorityElementsTest : public TestCase {};

TEST_CASE_WITH_DATA(ArrayMajorityElementsTest, tests, ArrayMajorityElementsTestData, array_majority_elements_tests) {
  trace << std::endl << "Searching for the majority elements (occurance > n/3) in array:" << std::endl;
  trace.vector(data.nums);

  const auto elements = array_majority_elements(data.nums);
  trace << "Found: " << std::endl;
  trace.vector(elements);
  trace << "Expected: " << std::endl;
  trace.vector(data.expected);
  assert.equal(elements, data.expected);
}
