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

const Instruction* GetReturnStatementFromBlock(const std::vector<Instruction*>& bb) {
  // Just assume the final instr in the block is a return...
  return bb.back();
}

// Simple dead code elimination implementation for operating on a single basic block.
// Assumes:
//   no Instruction has side-effects outside of the block
//   only one exit Instruction in the basic block
std::vector<Instruction*> GetDeadInstrs(const std::vector<Instruction*>& bb) {
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

  std::cout << std::endl << "Live instructions identified (unordered):" << std::endl;
  for (auto instr : live_instrs) {
    PrintInstruction(instr);
  }

  // All instrs in the basic block which are not in |live_instrs| ought to be dead.
  std::vector<Instruction*> dead_instrs;
  for (auto instr : bb) {
    if (live_instrs.find(instr) == live_instrs.cend()) {
      dead_instrs.push_back(instr);
    }
  }

  return dead_instrs;
}

int main() {
  // Super simple basic block like
  // a = 1 + 2
  // b = a + 3
  // c = 5
  // d = 6 + 7
  // e = d + b
  // return e

  Instruction a = {OpndType::Name, "a"};
  Instruction b = {OpndType::Name, "b"};
  Instruction c = {OpndType::Name, "c"};
  Instruction d = {OpndType::Name, "d"};
  Instruction e = {OpndType::Name, "e"};

  Instruction c1 = {OpndType::Constant, "", 1};
  Instruction c2 = {OpndType::Constant, "", 2};
  Instruction c3 = {OpndType::Constant, "", 3};
  Instruction c4 = {OpndType::Constant, "", 4};
  Instruction c5 = {OpndType::Constant, "", 5};
  Instruction c6 = {OpndType::Constant, "", 6};
  Instruction c7 = {OpndType::Constant, "", 7};

  Instruction i1 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Add, &a, &c1, &c2};
  Instruction i2 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Add, &b, &a, &c3};
  Instruction i3 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Assignment, &c, &c5, nullptr};
  Instruction i4 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Add, &d, &c6, &c7};
  Instruction i5 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Add, &e, &d, &b};
  Instruction i6 = {OpndType::Instruction, "", 0, nullptr, InstructionType::Return, nullptr, &e, nullptr};

  a.decl = &i1;
  b.decl = &i2;
  c.decl = &i3;
  d.decl = &i4;
  e.decl = &i5;

  std::vector<Instruction*> bb;
  bb.push_back(&i1);
  bb.push_back(&i2);
  bb.push_back(&i3);
  bb.push_back(&i4);
  bb.push_back(&i5);
  bb.push_back(&i6);

  std::cout << "Basic Block contains:" << std::endl;
  PrintBlock(bb);

  auto dead_instrs = GetDeadInstrs(bb);
  std::cout << std::endl << "Dead instructions identified:" << std::endl;
  PrintBlock(dead_instrs);

  return 0;
}
