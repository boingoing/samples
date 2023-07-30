#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "array_majority_element.h"
#include "helpers.h"


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

void test_array_majority_element(std::vector<int> nums, int expected_element) {
  std::cout << std::endl << "Searching for the majority element (occurance > n/2) in array:" << std::endl;
  print_vector(nums);

  const auto e = array_majority_element(nums);
  std::cout << "Found: " << e << std::endl << "Expected: " << expected_element << std::endl;
}

void test_array_majority_elements(std::vector<int> nums, std::vector<int> expected_elements) {
  std::cout << std::endl << "Searching for the majority elements (occurance > n/3) in array:" << std::endl;
  print_vector(nums);

  const auto elements = array_majority_elements(nums);
  std::cout << "Found: " << std::endl;
  print_vector(elements);
  std::cout << "Expected: " << std::endl;
  print_vector(expected_elements);
}

int main_array_majority_element() {
  test_array_majority_element({3,2,3}, 3);
  test_array_majority_element({2,2,1,1,1,2,2}, 2);
  test_array_majority_element({1}, 1);

  test_array_majority_elements({3,2,3}, {3});
  test_array_majority_elements({1,2}, {1,2});
  test_array_majority_elements({1}, {1});
  test_array_majority_elements({}, {});

  return 0;
}
