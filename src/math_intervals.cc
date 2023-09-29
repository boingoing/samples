//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "test/TestCase.h"

struct Interval {
  float left;
  float right;

  bool operator<(const Interval& rhs) const {
    return left < rhs.left;
  }

  friend std::ostream& operator<<(std::ostream& os, const Interval& i) {
    return os << "[" << i.left << ", " << i.right << "]";
  }
};

// Holds a set of intervals and may be used to determine if
// values are within this set of intervals.
class IntervalSet {
 private:
  std::vector<Interval> intervals_;

 public:
  const std::vector<Interval>& getIntervals() const {
    return intervals_;
  }

  // Add |interval| to the set of intervals we track.
  // If |interval| collides with an existing interval in our set,
  // the two will be coalesced.
  void addInterval(const Interval& interval) {
    Interval new_interval = interval;
    auto iter = intervals_.begin();

    // Walk over the set of intervals sorted by the left edge.
    while (iter != intervals_.end()) {
      const auto& i = *iter;
      // New interval begins before |i| begins.
      if (new_interval.left < i.left) {
        // New interval ends after |i| ends.
        if (new_interval.right > i.right) {
          // The new interval subsumes |i|, let's erase |i|.
          iter = intervals_.erase(iter);
          // The new interval may cross into further intervals, too.
          continue;
        }
        // New interval ends within |i|, let's coalesce the two.
        new_interval.right = i.right;
        // Erase the old interval, |i|.
        intervals_.erase(iter);
        // |i| is the furthest-left interval which the new interval starts
        // to the left of. If the new interval ends within |i|, we know that
        // there should be no other interval collisions with the new interval.
        break;
      }

      // New interval begins after |i| ends.
      if (new_interval.left > i.right) {
        // Just look at intervals further to the right.
        iter++;
        continue;
      }

      // New interval begins after |i| begins and ends after |i| ends.
      if (new_interval.right > i.right) {
        // Let's coalesce the two.
        new_interval.left = i.left;
        // Erase the old interval, |i|.
        iter = intervals_.erase(iter);
        // The new interval may cross into further intervals, too.
        continue;
      }

      // New interval is completely within |i|.
      // No need to change our interval set.
      return;
    }

    intervals_.push_back(new_interval);
    std::sort(intervals_.begin(), intervals_.end());
  }

  // Returns true if |x| is contained by any intervals in our set
  // of intervals.
  bool isInSet(float x) const {
    for (const auto& i : intervals_) {
      // |x| is outside the interval, to the left.
      if (x < i.left) {
        // The intervals are sorted by left edge so there
        // is no interval beginning further to the left of |i|.
        return false;
      }
      // |x| is outside the interval, to the right.
      if (x > i.right) {
        // |x| might be within an interval further to the right.
        continue;
      }
      // |x| is inside this interval.
      // x >= i.left && i.right >= x
      return true;
    }

    // Looked through all intervals and didn't find a containing one.
    return false;
  }
};

struct IntervalSetTestData : TestCaseDataWithExpectedResult<bool> {
  std::vector<Interval> intervals;
  float x;
};

std::vector<IntervalSetTestData> interval_set_tests = {
  {true, {{-1,5},{3,8},{12,15}}, 4},
  {false, {{-1,5},{3,8},{12,15}}, 9},
  {true, {{-1,5},{3,8},{12,15}}, 13},
  {true, {{0,5},{2,10}}, 8},
  {false, {{0,5},{2,10}}, 12},
  {true, {{0,5},{-5,2}}, 4},
  {false, {{0,5},{-5,2}}, -6},
  {true, {{0,5},{2,4}}, 1},
  {false, {{0,5},{2,4}}, 6},
  {true, {{0,5},{-2,6}}, 3},
  {false, {{0,5},{-2,6}}, 7},
  {true, {{0,5},{7,10},{3,12}}, 11},
  {false, {{0,5},{7,10},{3,12}}, -2},
  {true, {{0,5},{7,10},{3,9}}, 8},
  {false, {{0,5},{7,10},{3,9}}, 12},
  {true, {{0,5},{7,10},{6,9}}, 8},
  {false, {{0,5},{7,10},{6,9}}, 5.5},
};

class IntervalSetTest : public TestCase {};

TEST_CASE_WITH_DATA(IntervalSetTest, tests, IntervalSetTestData, interval_set_tests) {
  trace << std::endl << "Determining if " << data.x << " is in the set of intervals:" << std::endl;
  trace.vector(data.intervals);

  IntervalSet set;
  for (const auto& i : data.intervals) {
    trace << "Before adding interval " << i << " to the set, it contains these intervals:" << std::endl;
    trace.vector(set.getIntervals());
    set.addInterval(i);
    trace << "After adding interval " << i << " to the set, it contains these intervals:" << std::endl;
    trace.vector(set.getIntervals());
  }

  const auto result = set.isInSet(data.x);
  trace << "Result: " << result << " (expected: " << data.expected << ")" << std::endl;
  assert.equal(result, data.expected);
}
