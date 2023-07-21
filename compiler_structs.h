#ifndef __compiler_structs_h__
#define __compiler_structs_h__

#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

enum class OpndType {
  Constant,
  Name,
  Instruction,
};

struct Instruction;

struct Opnd {
  OpndType opnd_type;
  std::string name;
  uint32_t constant_value;
  // Assume only one decl statement
  Instruction* decl;
};

enum class InstructionType {
  Add,
  Assignment,
  Return,
};

struct Instruction : public Opnd {
  InstructionType instr_type;
  Instruction* dest;
  Instruction* src1;
  Instruction* src2;
};

void PrintInstruction(const Instruction* instr) {
  switch (instr->opnd_type) {
  case OpndType::Name:
    std::cout << instr->name;
    break;
  case OpndType::Constant:
    std::cout << instr->constant_value;
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
    }
    std::cout << std::endl;
  }
}

void PrintBlock(const std::vector<Instruction*>& bb) {
  for (auto instr : bb) {
    PrintInstruction(instr);
  }
}

#endif  // __compiler_structs_h__
