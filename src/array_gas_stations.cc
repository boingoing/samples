#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

using std::vector;

// We can start from a station if it provides more gas than it costs to travel to the next station.
int get_next_potential_starting_station(const vector<int>& gas, const vector<int>& cost, int starting_station) {
  while (starting_station < gas.size()) {
    if (gas[starting_station] >= cost[starting_station]) {
      return starting_station;
    }
    starting_station++;
  }

  // Didn't find a valid station to start from.
  return -1;
}

// Can the circuit be completed if we start at |starting_station|?
bool can_complete_circuit(const vector<int>& gas, const vector<int>& cost, int starting_station) {
  // Gas tank initially empty.
  int current_tank = 0;
  // Initial station is the one we should return to, to complete the circuit.
  int current_station = starting_station;
  // Try and complete the circuit.
  do {
    // Fill up gas from the current station.
    current_tank += gas[current_station];
    // Costs |cost[current_station]| units of gas to move to the next station.
    current_tank -= cost[current_station];

    // If tank would become negative to travel this segment, we can't make the circuit.
    if (current_tank < 0) {
      return false;
    }

    // Wrap back around if we run off the right edge.
    current_station = (current_station + 1) % gas.size();
  } while (current_station != starting_station);

  // We successfully traveled the circuit.
  return true;
}

// Determine if there is a circuit among a set of gas stations by which we may
// travel from a starting station clockwise until we reach the same starting
// station again.
// If there is such a circuit, return the starting station index.
// The set of stations is defined by a list of how much gas is available
// for refueling, |gas|, and a list of how many units of gas is required to
// drive to the next station, |cost|.
// Where |gas[i]| is the gas available at station |i| and |cost[i]| is the
// cost in units of gas required to drive between station |i| and |i+1|.
// This is leetcode 134. Gas Station
// https://leetcode.com/problems/gas-station
int can_complete_circuit(vector<int>& gas, vector<int>& cost) {
  // Find initial starting station.
  int starting_station = get_next_potential_starting_station(gas, cost, 0);

  while (starting_station != -1) {
    if (can_complete_circuit(gas, cost, starting_station)) {
      return starting_station;
    }

    // We weren't able to complete a circuit starting at |starting_station|.
    // Find next potential starting station.
    starting_station = get_next_potential_starting_station(gas, cost, starting_station + 1);
  }

  return -1;
}

struct GasStationCircuitTestData : TestCaseDataWithExpectedResult<int> {
  std::vector<int> gas;
  std::vector<int> cost;
};

std::vector<GasStationCircuitTestData> gas_station_circuit_tests = {
  {3, {1,2,3,4,5}, {3,4,5,1,2}},
  {-1, {2,3,4}, {3,4,3}},
  {4, {5,1,2,3,4}, {4,4,1,5,1}},
  {0, {2}, {2}},
  {0, {2,3}, {2,3}},
};

class GasStationCircuitTest : public TestCase {};

TEST_CASE_WITH_DATA(GasStationCircuitTest, tests, GasStationCircuitTestData, gas_station_circuit_tests) {
  trace << std::endl << "Looking to see if we can complete a circuit among gas stations with" << std::endl;
  trace << "gas = ";
  trace.vector(data.gas);
  trace << "cost = ";
  trace.vector(data.cost);

  const auto actual = can_complete_circuit(data.gas, data.cost);
  assert.equal(actual, data.expected);
}
