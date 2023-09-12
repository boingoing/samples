#ifndef __test_Logger_h__
#define __test_Logger_h__

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Logger {
 private:
  std::stringstream buffer_;

 public:
  std::string getBuffer() const {
    return buffer_.str();
  }

  template <typename T>
  Logger& operator<<(const T& t) {
    buffer_ << t;
    return *this;
  }

  Logger& operator<<(std::ostream& (*f)(std::ostream&)) {
    f(buffer_);
    return *this;
  }

  Logger& operator<<(std::ostream& (*f)(std::ios&)) {
    f(buffer_);
    return *this;
  }

  Logger& operator<<(std::ostream& (*f)(std::ios_base&)) {
    f(buffer_);
    return *this;
  }

  template<typename K, typename V>
  void map(const std::unordered_map<K,V>& m) {
    bool is_first = true;
    buffer_ << "[ ";
    for (const auto& p : m) {
      buffer_ << (is_first ? "" : ", ") << "\"" << p.first << "\" => \"" << p.second << "\"";
      is_first = false;
    }
    buffer_ << " ]" << std::endl;
  }

  template<typename T>
  void vector(const std::vector<T>& v) {
    bool is_first = true;
    buffer_ << "[ ";
    for (const auto& value : v) {
      buffer_ << (is_first ? "" : ", ") << value;
      is_first = false;
    }
    buffer_ << " ]" << std::endl;
  }

  template<typename T>
  void grid(const std::vector<std::vector<T>>& grid) {
    for (size_t x = 0; x < grid.size(); x++) {
      for (size_t y = 0; y < grid[x].size(); y++) {
        buffer_ << std::setw(3) << grid[x][y] << " ";
      }
      buffer_ << std::endl;
    }
  }
};

#endif  // __test_Logger_h__
