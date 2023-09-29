//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "compiler_structs.h"

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

// Simple constant folding implementation for operating on a single basic block.
// Returns >0 if there were any changes applied to the block.
// Assumes:
//   no Instruction has side-effects outside of the block
unsigned int FoldConstants(BasicBlock& bb) {
  unsigned int change_count = 0;

  for (auto& instr : bb.instrs) {
    if (instr.opnd_type == OpndType::Instruction) {
      switch (instr.instr_type) {
      // Replace add of two constants via assignment of constant
      case InstructionType::Add:
        if (instr.src1->opnd_type == OpndType::Constant && instr.src2->opnd_type == OpndType::Constant) {
          std::cout << std::endl << "Constant folding instruction:" << std::endl;
          PrintInstruction(&instr);
          // Insert a new constant which is the sum of the two opnds
          bb.consts.push_back({OpndType::Constant, "", instr.src1->constant_value + instr.src2->constant_value});
          // Assign that new constant value to the dest of the Add and convert it into an assignment
          instr.instr_type = InstructionType::Assignment;
          instr.src1 = &bb.consts.back();
          instr.src2 = nullptr;
          std::cout << "Into:" << std::endl;
          PrintInstruction(&instr);
          // Count the change we made
          change_count++;
        }
        break;
      default:
        break;
      }
    }
  }

  return change_count;
}

int constant_fold_main() {
  BasicBlock bb;
  BuildSimpleBasicBlock(bb);

  std::cout << "Basic block contains:" << std::endl;
  PrintBlock(bb.instrs);

  FoldConstants(bb);
  std::cout << std::endl << "Basic block after constants folded:" << std::endl;
  PrintBlock(bb.instrs);

  return 0;
}
