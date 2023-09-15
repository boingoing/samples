#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "test/TestCase.h"

#define VERBOSE 0

// Given a set of ratings where ratings[i] is the rating for individual i,
// allocate the minimum number of rewards to each individual such that:
// 1. Every individual has at least one reward and
// 2. An individual with a higher rating is allocated more rewards than
//    either individual to the left or right of itself
// Return the minimum total number of rewards that can be allocated.
// This is leetcode 135. Candy
// https://leetcode.com/problems/candy
size_t allocate_min_rewards(std::vector<int>& ratings) {
  if (ratings.size() < 2) {
    return ratings.size();
  }

  size_t min_rewards = ratings.size();
  size_t cur_less_than_streak = 0;
  size_t cur_greater_than_streak = 0;
  size_t tracked_greater_than_streak = 0;
  int prev = ratings[0];

  // Walk over the array and calculate the delta to the total rewards caused
  // by each element. Key strategy is to track the streaks of elements which
  // are less than / greater than the previous element. If we know how large
  // each streak is, we can use the size of the streak to tell us how many
  // rewards we need to allocate for all elements within the streak naturally.
  // [5,9,10,15] is an increasing streak of 4 elements so we know that we
  // need to distribute rewards in an increasing order such as [1,2,3,4].
  // The same is true of a decreasing streak, say [15,10,9,5], which we know
  // needs to distribute rewards in a decreasing order such as [4,3,2,1].
  // The complications arise when the current streak ends.
  // When a decreasing streak ends because the next element is greater than
  // the final element in the streak, we don't have much to do. Just start
  // an increasing streak beginning with the next element.
  // When an increasing streak ends because the next element is less than or
  // equal to the final element in the streak, we need to know which streak is
  // longer - the increasing streak leading up to here or the decreasing streak
  // beginning from here. Because the maximal element belongs to one streak or
  // the other we don't want to double-count it but we need to make sure to
  // count it as part of the correct streak.
  // 
  // To visualize, here is an example:
  // ratings = [2,3,4,5,3,2,4]
  // minimum reward per element = [1,2,3,4,2,1,2]
  // delta per element = [+0,+1,+2,+3,+1,+0,+1]
  // total rewards = sum of deltas + count of elements
  //               = 8 + 7 = 15
  // iterative steps:
  // 2 < 3 => first element of increasing streak
  //          increasing streak length = 1, decreasing streak length = 0
  // 3 < 4 => second element of increasing streak, total rewards += 1
  //          increasing streak length = 2, decreasing streak length = 0
  // 4 < 5 => third element of increasing streak, total rewards += 2
  //          increasing streak length = 3, decreasing streak length = 0
  // 5 > 3 => first element of decreasing streak
  //          change in streak direction, 5 is a local maxima
  //          - track the increasing streak length so we can reference it
  //            when the subsequent decreasing streak ends
  //          saved increasing streak length = 3
  //          increasing streak length = 0, decreasing streak length = 1
  // 3 > 2 => second element of decreasing streak, total rewards += 1
  //          saved increasing streak length = 3
  //          increasing streak length = 0, decreasing streak length = 2
  // 2 < 4 => first element of increasing streak
  //          change in streak direction, 2 is a local minima
  //          - determine which delta to apply to the previous maxima
  //          - saved increasing streak (3) is larger than
  //            current decreasing streak length (2) so the maxima element
  //            belongs to the increasing streak
  //          - total rewards += 3
  //          increasing streak length = 1, decreasing streak length = 0
  // cleanup:
  // - the last element was part of an increasing streak
  // - add delta to rewards
  // - total rewards += 1
  for (size_t i = 1; i < ratings.size(); i++) {
    const int cur = ratings[i];
    size_t rewards_delta = 0;
    if (prev > cur) {
      if (cur_greater_than_streak > 0) {
        tracked_greater_than_streak = cur_greater_than_streak;
        cur_less_than_streak = 0;
        cur_greater_than_streak = 0;
      }
      rewards_delta = cur_less_than_streak++;
    } else if (prev < cur) {
      if (cur_less_than_streak > 0) {
        if (tracked_greater_than_streak > 0) {
          rewards_delta = std::max(tracked_greater_than_streak, cur_less_than_streak);
          tracked_greater_than_streak = 0;
        } else {
          rewards_delta = cur_less_than_streak;
        }
        cur_greater_than_streak = 0;
        cur_less_than_streak = 0;
      } else {
        rewards_delta = cur_greater_than_streak;
      }
      cur_greater_than_streak++;
    } else {
      // cur == prev
      if (tracked_greater_than_streak > 0) {
        rewards_delta = std::max(tracked_greater_than_streak, cur_less_than_streak);
        cur_less_than_streak = 0;
        tracked_greater_than_streak = 0;
      }
      rewards_delta += cur_less_than_streak + cur_greater_than_streak;
      cur_less_than_streak = 0;
      cur_greater_than_streak = 0;
    }
#if VERBOSE
    std::cout << prev << ((prev > cur) ? " > " : ((prev < cur) ? " < " : " == ")) << cur << " : g = " << cur_greater_than_streak << ", l = " << cur_less_than_streak << ", t = " << tracked_greater_than_streak << ", m += " << rewards_delta << std::endl;
#endif
    min_rewards += rewards_delta;
    prev = cur;
  }

  size_t rewards_delta = 0;
  if (tracked_greater_than_streak > 0) {
    rewards_delta += std::max(tracked_greater_than_streak, cur_less_than_streak);
  } else {
    rewards_delta += cur_less_than_streak;
  }
  rewards_delta += cur_greater_than_streak;

#if VERBOSE
    std::cout << "cleanup: g = " << cur_greater_than_streak << ", l = " << cur_less_than_streak << ", t = " << tracked_greater_than_streak << ", m += " << rewards_delta << std::endl;
#endif

  return min_rewards + rewards_delta;
}

struct AllocateMinRewardsTestData : TestCaseDataWithExpectedResult<size_t> {
  std::vector<int> ratings;
};

std::vector<AllocateMinRewardsTestData> allocate_min_rewards_tests = {
  {5, {1,0,2}},
  {4, {1,2,2}},
  {15, {4,3,2,0,1,5}},
  {15, {5,1,0,2,3,4}},
  {4, {3,3,3,3}},
  {6, {2,4,2,4}},
  {7, {2,4,2,4,2}},
  {8, {4,2,4,2,4}},
  {6, {4,2,4,2}},
  {7, {1,3,2,2,1}},
  {7, {1,6,10,8}},
  {9, {1,6,10,8,7}},
  {13, {1,6,10,8,7,3}},
  {18, {1,6,10,8,7,3,2}},
  {6, {3,2,1}},
  {6, {1,2,3}},
  {47, {1,2,3,5,4,3,2,1,4,3,2,1,3,2,1,1,2,3,4}},
  {15, {2,3,4,5,3,2,4}},
};

class AllocateMinRewardsTest : public TestCase {};

TEST_CASE_WITH_DATA(AllocateMinRewardsTest, tests, AllocateMinRewardsTestData, allocate_min_rewards_tests) {
  trace << std::endl << "Finding minimum number of rewards to allocate with ratings: ";
  trace.vector(data.ratings);

  const auto actual = allocate_min_rewards(data.ratings);
  assert.equal(actual, data.expected);
}
