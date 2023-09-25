#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "test/TestCaseContainer.h"

void print_help() {
  std::cout << "Usage: runner [--filter str] [--verbose]" << std::endl << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--filter str";
  std::cout << "Filter and only execute test cases with names matching str" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--help";
  std::cout << "Display this usage information" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--verbose";
  std::cout << "Enable verbose tracing" << std::endl;
}

int main(int argc, const char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  std::cout << "Simple unit test runner" << std::endl << std::endl;

  for (auto iter = args.cbegin() + 1; iter != args.cend(); iter++) {
    const auto& arg = *iter;
    if (arg == "--verbose") {
      TestCaseContainer::enableVerbose();
    } else if (arg == "--filter") {
      // If there are no more arguments, break out of the loop.
      if (++iter == args.cend()) {
        break;
      }
      TestCaseContainer::setFilter(*iter);
    } else if (arg == "--help") {
      print_help();
      return 0;
    } else {
      std::cout << "Unknown option: " << std::quoted(arg) << std::endl;
      print_help();
      return -1;
    }
  }

  TestCaseContainer::runAllTests();

  return 0;
}
