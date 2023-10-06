//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <string>

#include "test/Assert.h"
#include "test/Logger.h"

Assert::Assert(Logger& trace) : trace_(trace) {}

bool Assert::isFailed() {
  return is_failed_;
}

void Assert::resetFailStatus() {
  is_failed_ = false;
}

void Assert::fail() {
  is_failed_ = true;
}
