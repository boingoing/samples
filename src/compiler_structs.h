//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __compiler_structs_h__
#define __compiler_structs_h__

#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

struct Instruction;

enum class OpndType {
  Constant,
  Name,
  Temp,
  Instruction,
};

struct Opnd {
  OpndType opnd_type;
  std::string name;
  uint32_t constant_value;
  uint32_t temp_number;
  // Assume only one decl statement
  Instruction* decl;
};

enum class InstructionType {
  Invalid = 0,
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

struct BasicBlock {
  std::vector<Instruction> names;
  std::vector<Instruction> consts;
  std::vector<Instruction> instrs;
};

void PrintInstruction(const Instruction* instr);

void PrintBlock(const std::vector<Instruction>& bb);

// Super simple basic block like
// a = 1 + 2
// b = a + 3
// c = 5
// d = a
// e = d + b
// f = c + d
// return e
void BuildSimpleBasicBlock(BasicBlock& bb);

#endif  // __compiler_structs_h__
