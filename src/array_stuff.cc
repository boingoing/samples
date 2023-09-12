#include <string>
#include <vector>

#include "test/TestCaseContainer.h"

#include "helpers.h"

int main(int argc, const char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  if (args.size() > 1) {
    TestCaseContainer::setFilter(args[1]);
  }
  TestCaseContainer::runAllTests();

  return 0;
}
