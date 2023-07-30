#include <vector>

#ifndef __helpers_h__
#define __helpers_h__

template<typename t>
void print_vector(const std::vector<t>& v) {
  bool is_first = true;
  std::cout << "[ ";
  for (const auto& value : v) {
    std::cout << (is_first ? "" : ", ") << value;
    is_first = false;
  }
  std::cout << " ]" << std::endl;
}

#endif  // __helpers_h__
