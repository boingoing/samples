//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
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

// A single datapoint sampled from the hardware GPS indicating at |time|,
// the user traveled |distance| units.
struct Trackpoint {
  int time;
  int distance;

  friend std::ostream& operator<<(std::ostream& os, const Trackpoint& t) {
    return os << "[" << t.time << ", " << t.distance << "]";
  }
};

// A set of trackpoints which, taken together, form the data making up a
// physical activity over time.
struct Activity {
  std::string id;
  std::string date;
  std::vector<Trackpoint> trackpoints;
};

// Find the fastest segment over the course of the activity during which the
// user traveled |benchmark_distance| units.
// Returns the time taken to travel |benchmark_distance| units.
// For this implementation, the trackpoints in |a| are cumulative and ordered
// by time with each trackpoint separated by a 10 unit increment.
int fastest_segment(const Activity& a, int benchmark_distance) {
  // Default to the largest value - ideally use numerical_limits
  int current_fastest = 10000;
  // There are no gaps in the trackpoints, each one is exactly 10 distance
  // units from the next.
  // This means trackpoint[3] must have distance field set to 30.
  // The distance of the segment we're interested in is, thus,
  // (benchmark_distance / 10) trackpoints.
  const int segment_length_in_trackpoints = benchmark_distance / 10;

  // Consider each segment within the set of trackpoints.
  for (size_t i = 0; i < a.trackpoints.size() - segment_length_in_trackpoints; i++) {
    const size_t end_index = i + segment_length_in_trackpoints;
    // Time taken for this segment is the delta of time fields.
    const int segment_delta = a.trackpoints[end_index].time - a.trackpoints[i].time;

    // Lower |segment_delta| indicates higher speed of the segment because
    // all segments we're considering are the same width in distance units.
    current_fastest = std::min(segment_delta, current_fastest);
  }

  return current_fastest;
}

// Find the fastest segment over the course of the activity during which the
// user traveled |benchmark_distance| units.
// Returns the time taken to travel |benchmark_distance| units.
// For this implementation, the trackpoints in |a| are cumulative and ordered
// by time with all trackpoints located at 10 unit increments. Not all
// trackpoints are present in the activity - there may be gaps.
int fastest_segment_with_gaps(const Activity& a, int benchmark_distance) {
  int current_fastest = 10000;
  const int segment_length_in_trackpoints = benchmark_distance / 10;

  // Compute a map of expected trackpoint index (ie: trackpiont at distance
  // 20 should be at index 2) to the actual index of the trackpoint in the
  // vector. We use this to locate trackopint at distance 20 when the trackpoint
  // at distance 10 is missing, for example.
  std::map<size_t, size_t> trackpoint_index_map;
  for (size_t i = 0; i < a.trackpoints.size(); i++) {
    const auto expected_idx = a.trackpoints[i].distance / 10;
    trackpoint_index_map[expected_idx] = i;
  }

  const int max_expected_index = a.trackpoints[a.trackpoints.size() - 1].distance / 10 + 1;
  for (size_t i = 0; i < max_expected_index - segment_length_in_trackpoints; i++) {
    const auto iter = trackpoint_index_map.find(i);
    if (iter == trackpoint_index_map.cend()) {
      continue;
    }

    const auto start_actual_index = iter->second;
    const auto expected_end_index = i + segment_length_in_trackpoints;
    const auto iter2 = trackpoint_index_map.find(expected_end_index);
    if (iter2 == trackpoint_index_map.cend()) {
      continue;
    }

    const auto end_actual_index = iter2->second;
    const int segment_delta = a.trackpoints[end_actual_index].time - a.trackpoints[start_actual_index].time;
    current_fastest = std::min(segment_delta, current_fastest);
  }

  return current_fastest;
}

struct ActivityTrackpointTestData : TestCaseDataWithExpectedResult<int> {
  Activity activity;
  int benchmark_distance;
};

std::vector<ActivityTrackpointTestData> activity_trackpoint_tests = {
  {5, {"", "", {{0, 0}, {4, 10}, {9, 20}, {15, 30}, {18, 40}, {22, 50}, {23, 60}}}, 20},
};

class ActivityTrackpointTest : public TestCase {};

TEST_CASE_WITH_DATA(ActivityTrackpointTest, fastest_segment, ActivityTrackpointTestData, activity_trackpoint_tests) {
  trace << std::endl << "Finding the fastest segment of " << data.benchmark_distance << " distance units for this set of trackpoints:" << std::endl;
  trace.vector(data.activity.trackpoints);

  const auto result = fastest_segment(data.activity, data.benchmark_distance);
  trace << "Result: " << result << " (expected: " << data.expected << ")" << std::endl;
  assert.equal(result, data.expected);
}

std::vector<ActivityTrackpointTestData> activity_trackpoint_with_gaps_tests = {
  {7, {"", "", {{0, 0}, {4, 10}, {9, 20}, {18, 40}, {31, 70}, {34, 80}, {38, 90}}}, 20},
};

TEST_CASE_WITH_DATA(ActivityTrackpointTest, fastest_segment_with_gaps, ActivityTrackpointTestData, activity_trackpoint_with_gaps_tests) {
  trace << std::endl << "Finding the fastest segment of " << data.benchmark_distance << " distance units for this set of trackpoints which may contain gaps:" << std::endl;
  trace.vector(data.activity.trackpoints);

  const auto result = fastest_segment_with_gaps(data.activity, data.benchmark_distance);
  trace << "Result: " << result << " (expected: " << data.expected << ")" << std::endl;
  assert.equal(result, data.expected);
}
