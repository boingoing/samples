//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "test/Logger.h"

std::string Logger::getBuffer() const {
  return buffer_.str();
}

Logger& Logger::operator<<(std::ostream& (*f)(std::ostream&)) {
  f(buffer_);
  return *this;
}

Logger& Logger::operator<<(std::ostream& (*f)(std::ios&)) {
  f(buffer_);
  return *this;
}

Logger& Logger::operator<<(std::ostream& (*f)(std::ios_base&)) {
  f(buffer_);
  return *this;
}
