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
  void start();
  void stop();
  double elapsed() const;
};

#endif  // __test_Stopwatch_h__
