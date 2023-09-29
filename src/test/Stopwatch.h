//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_Stopwatch_h__
#define __test_Stopwatch_h__

#include <chrono>

class Stopwatch {
 private:
  std::chrono::high_resolution_clock::time_point start_ = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point stop_ = std::chrono::high_resolution_clock::now();

  bool is_running_ = false;

 public:
  void start() {
    start_ = std::chrono::high_resolution_clock::now();
    is_running_ = true;
  }

  void stop() {
    stop_ = std::chrono::high_resolution_clock::now();
    is_running_ = false;
  }

  double elapsed() {
    std::chrono::nanoseconds duration;
    if (is_running_) {
      duration = std::chrono::high_resolution_clock::now() - start_;
    } else {
      duration = stop_ - start_;
    }
    return static_cast<double>(duration.count()) / 1e9;
  }
};

#endif  // __test_Stopwatch_h__
