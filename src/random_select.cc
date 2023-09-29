//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

#include "test/TestCase.h"

std::random_device device;
std::mt19937 engine(device());
std::uniform_real_distribution<> real_distribution(0.0, 1.0);
std::uniform_int_distribution<> int_distribution(0, 99);

// Get a random floating point in the range [0,1)
double get_unit_rand() {
  return real_distribution(engine);
}

size_t get_index_rand() {
  return int_distribution(engine);
}

// Given a set of names and their relative probabilities, randomly choose
// one name according to a uniform distribution where each name has weighted
// probability of being chosen.
// The |probs| parameter is the set of names and their weighted probabilities.
// The weighted probabilities of all names in the set should sum to 1.0.
// Ex: { {"a", 0.35}, {"b", 0.25}, {"c", 0.21}, {"d", 0.19} }
// This algorithm runs in O(n) where n is the number of entries in |probs|.
const std::string& dist(const std::vector<std::pair<float, std::string>>& probs) {
  const auto r = get_unit_rand();
  float sum = 0.0;
  for (const auto& p : probs) {
    sum += p.first;
    if (sum >= r) {
      return p.second;
    }
  }
  return probs.back().second;
}

// Process the set of weighted probabilities into a vector where each
// name in |probs| is stored in the vector n times where n is the weighted
// probability (0,1) for the name multiplied by 100.
std::vector<std::string> process_probs(const std::vector<std::pair<float, std::string>>& probs) {
  std::vector<std::string> processed_probs;
  for (const auto& p : probs) {
    const size_t count = static_cast<size_t>(std::floor(p.first * 100));
    for (size_t i = 0; i < count; i++) {
      processed_probs.push_back(p.second);
    }
  }
  return processed_probs;
}

// This is a faster version of dist.
// It requires pre-processing the weighted probabilities into an intermediate
// data structure via process_probs.
// This algorithm runs in O(1) time at the expense of the extra memory required
// for the processed weighted probability data structure.
const std::string& dist_fast(const std::vector<std::string>& processed_probs) {
  const auto r = get_index_rand();
  return processed_probs[r];
}

struct UniformRandomSelectWeightedProbabilityTestData : TestCaseData {
  std::vector<std::pair<float, std::string>> probs;
  size_t select_count;

  UniformRandomSelectWeightedProbabilityTestData(std::vector<std::pair<float, std::string>> probs, size_t select_count) : probs(probs), select_count(select_count) {}
};

std::vector<UniformRandomSelectWeightedProbabilityTestData> uniform_random_select_weighted_probability_tests = {
  {{{0.35f, "a"}, {0.25f, "b"}, {0.21f, "c"}, {0.19f, "d"}}, 10000},
};

class UniformRandomSelectWeightedProbabilityTest : public TestCase {};

TEST_CASE_WITH_DATA(UniformRandomSelectWeightedProbabilityTest, tests, UniformRandomSelectWeightedProbabilityTestData, uniform_random_select_weighted_probability_tests) {
  trace << std::endl << "Uniform-randomly choosing " << data.select_count << " names using this set of weighted probabilities:" << std::endl;
  trace << "[ ";
  for (const auto& p : data.probs) {
    trace << "{ " << p.second << ": " << p.first << " } ";
  }
  trace << "]" << std::endl;

  std::unordered_map<std::string, size_t> counters;
  for (size_t i = 0; i < data.select_count; i++) {
    const auto& n = dist(data.probs);
    counters[n]++;
  }

  trace << "Found: " << std::endl;
  for (const auto& p : data.probs) {
    trace << p.second << ": " << counters[p.second] << " (" << p.first << ")" << std::endl;
  }

  // TODO: Verify distribution is uniform and weighted correctly.
}

class UniformRandomSelectWeightedProbabilityFastTest : public TestCase {};

TEST_CASE_WITH_DATA(UniformRandomSelectWeightedProbabilityFastTest, tests, UniformRandomSelectWeightedProbabilityTestData, uniform_random_select_weighted_probability_tests) {
  trace << std::endl << "Uniform-randomly and quickly choosing " << data.select_count << " names using this set of weighted probabilities:" << std::endl;
  trace << "[ ";
  for (const auto& p : data.probs) {
    trace << "{ " << p.second << ": " << p.first << " } ";
  }
  trace << "]" << std::endl;

  const auto processed_probs = process_probs(data.probs);
  std::unordered_map<std::string, size_t> counters;
  for (size_t i = 0; i < data.select_count; i++) {
    const auto& n = dist_fast(processed_probs);
    counters[n]++;
  }

  trace << "Found: " << std::endl;
  for (const auto& p : data.probs) {
    trace << p.second << ": " << counters[p.second] << " (" << p.first << ")" << std::endl;
  }

  // TODO: Verify distribution is uniform and weighted correctly.
}
