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

const Instruction* GetReturnStatementFromBlock(const BasicBlock& bb) {
  // Just assume the final instr in the block is a return...
  return &bb.instrs.back();
}

// Simple dead code elimination implementation for operating on a single basic block.
// Assumes:
//   no Instruction has side-effects outside of the block
//   only one exit Instruction in the basic block
unsigned int EliminateDeadInstrs(BasicBlock& bb) {
  std::unordered_set<const Instruction*> live_instrs;
  std::stack<const Instruction*> working_set;

  // Whatever the return statement reaches back and touches should be considered live.
  const auto* return_instr = GetReturnStatementFromBlock(bb);
  working_set.push(return_instr);

  // Walk upwards through all reachable instructions and remember them - these are live.
  while (!working_set.empty()) {
    const auto* instr = working_set.top();
    working_set.pop();

    // Skip empty Opnds because we don't enforce pushing valid ones based on the instr.
    if (instr == nullptr) {
      continue;
    }

    // Walk up to the sources for this instr.
    working_set.push(instr->src1);
    working_set.push(instr->src2);
    working_set.push(instr->decl);
  
    // Track live instrs (no duplicates in unordered_set).
    // Skip tracking name and constant Opnds.
    if (instr->opnd_type == OpndType::Instruction) {
      live_instrs.insert(instr);
    }
  }

  unsigned int change_count = 0;
  // All instrs in the basic block which are not in |live_instrs| ought to be dead.
  for (auto& instr : bb.instrs) {
    if (instr.opnd_type == OpndType::Instruction && live_instrs.find(&instr) == live_instrs.cend()) {
      std::cout << std::endl << "Removing dead instruction:" << std::endl;
      PrintInstruction(&instr);

      // We don't have a good way to erase Instructions from the BasicBlock so let's just make this instr ignored.
      instr.opnd_type = OpndType::Temp;
    }
  }

  return change_count;
}

int dead_code_main() {
  BasicBlock bb;
  BuildSimpleBasicBlock(bb);

  std::cout << "Basic block contains:" << std::endl;
  PrintBlock(bb.instrs);

  EliminateDeadInstrs(bb);
  std::cout << std::endl << "Basic block after dead instructions eliminated:" << std::endl;
  PrintBlock(bb.instrs);

  return 0;
}
