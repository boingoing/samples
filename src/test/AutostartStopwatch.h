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
