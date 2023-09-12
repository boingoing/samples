#include <string>
#include <vector>

#include "test/TestCaseContainer.h"

#include "helpers.h"

int main(int argc, const char** argv) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  for (auto iter = args.cbegin(); iter != args.cend(); iter++) {
    const auto& arg = *iter;
    if (arg == "--verbose") {
      TestCaseContainer::enableVerbose();
    } else if (arg == "--filter") {
      // If there are no more arguments, break out of the loop.
      if (++iter == args.cend()) {
        break;
      }
      TestCaseContainer::setFilter(*iter);
    }
  }

  TestCaseContainer::runAllTests();

  return 0;
}
