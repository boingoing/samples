#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "test/TestCase.h"

struct Building {
  int left;
  int right;
  int height;

  friend std::ostream& operator<<(std::ostream& os, const Building& b) {
    return os << "(" << b.left << ", " << b.right << ", " << b.height << ")";
  }
};

struct Point {
  int x;
  int y;

  bool operator==(const Point& rhs) const {
    return rhs.x == x && rhs.y == y;
  }

  bool operator!=(const Point& rhs) const {
    return !(*this == rhs);
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};

struct Edge {
  int x;
  int height;
  bool is_left;

  bool operator<(const Edge& rhs) const {
    return x < rhs.x;
  }
};

struct CitySkylineTestData : TestCaseDataWithExpectedResult<std::vector<Point>> {
  std::vector<Building> buildings;
};

std::vector<CitySkylineTestData> city_skyline_tests = {
  {std::vector<Point>({
      {2,0},
      {2,5},
      {4,5},
      {4,10},
      {10,10},
      {10,3},
      {12,3},
      {12,0},
      {15,0},
      {15,10},
      {20,10},
      {20,0},
      {25,0},
      {25,5},
      {26,5},
      {26,10},
      {27,10},
      {27,5},
      {28,5},
      {28,15},
      {29,15},
      {29,5},
      {30,5},
      {30,0},
      }), {
      {2,6,5},
      {4,10,10},
      {8,12,3},
      {15,20,10},
      {16,18,5},
      {25,30,5},
      {26,27,10},
      {28,29,15}}},
};

class CitySkylineTest : public TestCase {
 protected:

  // Given a set of buildings, |buildings|, determine the set of points
  // which make up the skyline created by the buildings.
  std::vector<Point> find_skyline(const std::vector<Building>& buildings) {
    // Split each building into a pair of edges, a left and right edge.
    std::vector<Edge> edges;
    for (const auto& b : buildings) {
      edges.push_back({b.left, b.height, true});
      edges.push_back({b.right, b.height, false});
    }

    // Sort the edges by their x-coordinate so we can traverse the set of them
    // from left to right.
    std::sort(edges.begin(), edges.end());

    // Keep an ordered set of building heights for which we've seen the left
    // but not the right edge. These define the current height of the skyline
    // and where intersections with subsequent edges can occur.
    std::multiset<int> in_progress_buildings;

    // The set of points which make up the skyline.
    std::vector<Point> skyline_points;

    // Walk from left to right over each building edge and insert points into
    // |skyline_points| for each building corner and at the intersection of
    // visible buildings.
    for (const auto& e : edges) {
      if (e.is_left) {
        // Each left edge represents a building we haven't seen before.
        // If this building is the tallest among buildings we have seen only the
        // left edge of (haven't yet seen the right edge, yet) that means the
        // left side of this building is at least partially visible.
        // If we've previously seen the left edge of a taller building and
        // haven't yet seen that buildings right edge, the left edge of this new
        // building isn't visible - it's occluded by taller buildings.

        // Get the current largest height value in our ordered set (or 0 if the
        // set it empty).
        // Do this before we modify the set in case the current building is
        // tallest.
        const auto& iter = in_progress_buildings.upper_bound(0);
        const auto& current_height =
            iter == in_progress_buildings.cend() ? 0 : *iter;

        // Remember that we saw the left edge of a building in case this
        // building becomes (or already is) the tallest one in the skyline.
        in_progress_buildings.insert(e.height);

        // If this edge is visible above other buildings.
        if (e.height > current_height) {
          // The point where this edge intersects with either a shorter building
          // or the ground.
          skyline_points.push_back({e.x, current_height});
          // The top-left corner of the building this edge belongs to.
          skyline_points.push_back({e.x, e.height});
        }
      } else {
        // Each right edge represents a building we previously saw the left
        // edge of.
        // If this building was the tallest among buildings we have seen only
        // the left edge of (haven't yet seen the right edge, yet) that means
        // the right side of the building is at least partially visible.
        // If we've previously seen the left edge of a taller building and
        // haven't yet seen that buildings right edge, the right edge of this
        // new building isn't visible - it's occluded by that taller building.

        // Find an element in the set of buildings for which we saw a left
        // edge but haven't yet seen a right edge for which the height of that
        // element is the height of this edge.
        // TODO: We should assert that the element is found (!= cend()).
        const auto& iter = in_progress_buildings.find(e.height);
        // Remove that element from the set.
        // Note: It doesn't matter which element we remove if there are
        //       multiple buildings at the same height.
        in_progress_buildings.erase(iter);

        // Get the current largest height value in our ordered set (or 0 if the
        // set it empty).
        // Do this after modifying the set so we get the next tallest building
        // if the above removed one was previously tallest.
        const auto& iter2 = in_progress_buildings.upper_bound(0);
        const auto& current_height =
            iter2 == in_progress_buildings.cend() ? 0 : *iter2;

        // If this edge is visible above other buildings.
        if (e.height > current_height) {
          // The top-right corner of the building this edge belongs to.
          skyline_points.push_back({e.x, e.height});
          // The point where this edge intersects with either a building or
          // the ground.
          skyline_points.push_back({e.x, current_height});
        }
      }
    }

    return skyline_points;
  }
};

TEST_CASE_WITH_DATA(CitySkylineTest, tests, CitySkylineTestData, city_skyline_tests) {
  trace << std::endl << "Calculating the set of points representing the skyline of these buildings:" << std::endl;
  trace.vector(data.buildings);

  const auto actual = find_skyline(data.buildings);
  trace << "Found:" << std::endl;
  trace.vector(actual);
  trace << "Expected:" << std::endl;
  trace.vector(data.expected);
  assert.equal(actual, data.expected);
}
