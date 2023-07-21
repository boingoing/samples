#include "compiler_structs.h"

#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

void PrintInstruction(const Instruction* instr) {
  switch (instr->opnd_type) {
  case OpndType::Name:
    std::cout << instr->name;
    break;
  case OpndType::Constant:
    std::cout << instr->constant_value;
    break;
  case OpndType::Temp:
    std::cout << "#Temp(" << instr->temp_number << ")";
    break;
  case OpndType::Instruction:
    std::cout << "(" << instr << "): ";
    switch (instr->instr_type) {
    case InstructionType::Add:
      PrintInstruction(instr->dest);
      std::cout << " = ";
      PrintInstruction(instr->src1);
      std::cout << " + ";
      PrintInstruction(instr->src2);
      break;
    case InstructionType::Assignment:
      PrintInstruction(instr->dest);
      std::cout << " = ";
      PrintInstruction(instr->src1);
      break;
    case InstructionType::Return:
      std::cout << "return ";
      PrintInstruction(instr->src1);
      break;
    default:
      // Unhandled instruction
      assert(false);
    }
    std::cout << std::endl;
    break;
  }
}

void PrintBlock(const std::vector<Instruction>& bb) {
  for (auto& instr : bb) {
    if (instr.opnd_type == OpndType::Instruction) {
      PrintInstruction(&instr);
    }
  }
}

// Super simple basic block like
// a = 1 + 2
// b = a + 3
// c = 5
// d = a
// e = d + b
// f = c + d
// return e
void BuildSimpleBasicBlock(BasicBlock& bb) {
  bb.names.reserve(100);
  bb.names.push_back({OpndType::Name, "a"});
  bb.names.push_back({OpndType::Name, "b"});
  bb.names.push_back({OpndType::Name, "c"});
  bb.names.push_back({OpndType::Name, "d"});
  bb.names.push_back({OpndType::Name, "e"});
  bb.names.push_back({OpndType::Name, "f"});

  bb.consts.reserve(100);
  bb.consts.push_back({OpndType::Constant, "", 1});
  bb.consts.push_back({OpndType::Constant, "", 2});
  bb.consts.push_back({OpndType::Constant, "", 3});
  bb.consts.push_back({OpndType::Constant, "", 4});
  bb.consts.push_back({OpndType::Constant, "", 5});
  bb.consts.push_back({OpndType::Constant, "", 6});
  bb.consts.push_back({OpndType::Constant, "", 7});

  bb.instrs.reserve(100);
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Add, &bb.names[0], &bb.consts[0], &bb.consts[1]});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Add, &bb.names[1], &bb.names[0], &bb.consts[2]});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Assignment, &bb.names[2], &bb.consts[4], nullptr});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Assignment, &bb.names[3], &bb.names[0], nullptr});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Add, &bb.names[4], &bb.names[3], &bb.names[1]});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Add, &bb.names[5], &bb.names[2], &bb.names[3]});
  bb.instrs.push_back({OpndType::Instruction, "", 0, 0, nullptr, InstructionType::Return, nullptr, &bb.names[4], nullptr});

  bb.names[0].decl = &bb.instrs[0];
  bb.names[1].decl = &bb.instrs[1];
  bb.names[2].decl = &bb.instrs[2];
  bb.names[3].decl = &bb.instrs[3];
  bb.names[4].decl = &bb.instrs[4];
  bb.names[5].decl = &bb.instrs[5];
}
