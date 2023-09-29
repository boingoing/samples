//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "compiler_structs.h"
#include "constant_fold.h"
#include "constant_prop.h"
#include "dead_code.h"

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
  BasicBlock bb;
  BuildSimpleBasicBlock(bb);

  std::cout << "Basic block contains:" << std::endl;
  PrintBlock(bb.instrs);

  unsigned int change_count = 0;
  do {
    change_count = PropConstants(bb);
    std::cout << std::endl << "Basic block after constants propagated:" << std::endl;
    PrintBlock(bb.instrs);
  
    change_count += FoldConstants(bb);
    std::cout << std::endl << "Basic block after constants folded:" << std::endl;
    PrintBlock(bb.instrs);

    change_count += EliminateDeadInstrs(bb);
    std::cout << std::endl << "Basic block after dead instructions eliminated:" << std::endl;
    PrintBlock(bb.instrs);
  } while (change_count > 0);

  return 0;
}
