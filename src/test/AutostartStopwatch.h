//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __test_AutostartStopwatch_h__
#define __test_AutostartStopwatch_h__

#include <chrono>

#include "test/Stopwatch.h"

class AutostartStopwatch : public Stopwatch {
 public:
  AutostartStopwatch() : Stopwatch() {
    start();
  }
};

#endif  // __test_AutostartStopwatch_h__
