#include "compiler_structs.h"

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

bool CanReplaceSrcByConstant(Instruction* src) {
  return src->opnd_type == OpndType::Name
    && src->decl
    && src->decl->opnd_type == OpndType::Instruction
    && src->decl->instr_type == InstructionType::Assignment
    && src->decl->src1->opnd_type == OpndType::Constant;
}

// Simple constant propagation implementation for operating on a single basic block.
// Returns >0 if there were any changes applied to the block.
// Assumes:
//   no Instruction has side-effects outside of the block
unsigned int PropConstants(BasicBlock& bb) {
  unsigned int change_count = 0;

  for (auto& instr : bb.instrs) {
    if (instr.opnd_type == OpndType::Instruction) {
      switch (instr.instr_type) {
      case InstructionType::Add:
        if (CanReplaceSrcByConstant(instr.src2)) {
          std::cout << std::endl << "Constant propagating instruction:" << std::endl;
          PrintInstruction(&instr);
          instr.src2 = instr.src2->decl->src1;
          std::cout << "Into:" << std::endl;
          PrintInstruction(&instr);
          // Count the change we made
          change_count++;
        }
        // Intentional fall-thru
      case InstructionType::Assignment:
      case InstructionType::Return:
        // Replace src1 declared by assignment of constant by the constant itself
        if (CanReplaceSrcByConstant(instr.src1)) {
          std::cout << std::endl << "Constant propagating instruction:" << std::endl;
          PrintInstruction(&instr);
          instr.src1 = instr.src1->decl->src1;
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

int constant_prop_main() {
  BasicBlock bb;
  BuildSimpleBasicBlock(bb);

  std::cout << "Basic block contains:" << std::endl;
  PrintBlock(bb.instrs);

  PropConstants(bb);
  std::cout << std::endl << "Basic block after constants propagated:" << std::endl;
  PrintBlock(bb.instrs);

  return 0;
}
