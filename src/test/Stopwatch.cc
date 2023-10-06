//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <chrono>

#include "test/Stopwatch.h"

void Stopwatch::start() {
  start_ = std::chrono::high_resolution_clock::now();
  is_running_ = true;
}

void Stopwatch::stop() {
  stop_ = std::chrono::high_resolution_clock::now();
  is_running_ = false;
}

double Stopwatch::elapsed() const {
  std::chrono::nanoseconds duration;
  if (is_running_) {
    duration = std::chrono::high_resolution_clock::now() - start_;
  } else {
    duration = stop_ - start_;
  }
  return static_cast<double>(duration.count()) / 1e9;
}
