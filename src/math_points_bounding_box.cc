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

struct Point {
  int x;
  int y;

  bool operator<(const Point& rhs) const {
    return x < rhs.x;
  }

  bool operator==(const Point& rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};

struct BoundingBox {
  Point bottom_left;
  Point top_right;

  bool operator==(const BoundingBox& rhs) const {
    return bottom_left == rhs.bottom_left && top_right == rhs.top_right;
  }

  bool operator!=(const BoundingBox& rhs) const {
    return !(*this == rhs);
  }

  friend std::ostream& operator<<(std::ostream& os, const BoundingBox& bb) {
    return os << "[" << bb.bottom_left << ", " << bb.top_right << "]";
  }
};

// Compute the bounding rectangle which encloses all points in a given
// set, |points|, which is not sorted in any way.
// Note: This is an oriented bounding box.
BoundingBox get_bounding_box(const std::vector<Point>& points) {
  if (points.size() < 1) {
    return {};
  }

  // Default to the single point at index 0.
  BoundingBox bb = {{points[0].x, points[0].y}, {points[0].x, points[0].y}};

  // Walk the array to find the extents.
  for (const auto& point : points) {
      bb.bottom_left.x = std::min(bb.bottom_left.x, point.x);
      bb.bottom_left.y = std::min(bb.bottom_left.y, point.y);
      bb.top_right.x = std::max(bb.top_right.x, point.x);
      bb.top_right.y = std::max(bb.top_right.y, point.y);
  }

  return bb;
}

struct BoundingBoxTestData : TestCaseDataWithExpectedResult<BoundingBox> {
  std::vector<Point> points;
};

std::vector<BoundingBoxTestData> bounding_box_tests = {
  {BoundingBox({{5, 1}, {15, 5}}), {{5, 2}, {6, 3}, {7, 4}, {9, 2}, {10, 5}, {11, 1}, {12, 4}, {14, 4}, {15, 2}}},
  {BoundingBox({}), {}},
  {BoundingBox({{5, 2}, {5, 2}}), {{5, 2}}},
  {BoundingBox({{5, 2}, {15, 3}}), {{5, 2}, {15, 3}}},
  {BoundingBox({{5, 2}, {15, 3}}), {{15, 3}, {5, 2}}},
};

class BoundingBoxTest : public TestCase {};

TEST_CASE_WITH_DATA(BoundingBoxTest, tests, BoundingBoxTestData, bounding_box_tests) {
  trace << std::endl << "Computing the bounding box for these points:" << std::endl;
  trace.vector(data.points);

  const auto result = get_bounding_box(data.points);
  trace << "Result: " << result << " (expected: " << data.expected << ")" << std::endl;
  assert.equal(result, data.expected);
}
