
#ifndef LLVM_LIB_TARGET_ELMO_ELMO_H
#define LLVM_LIB_TARGET_ELMO_ELMO_H

#include "MCTargetDesc/ELMOMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class ELMOTargetMachine;
class FunctionPass;
class AsmPrinter;
class MCInst;
class MachineInstr;

FunctionPass *createELMOISelDag(ELMOTargetMachine &TM);
void LowerELMOMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                   AsmPrinter &AP);

} // end namespace llvm;

namespace llvm {
namespace ELMOCC {
enum CondCodes {
  ICC_A = 8,    // Always
  ICC_N = 0,    // Never
  ICC_NE = 9,   // Not Equal
  ICC_E = 1,    // Equal
  ICC_G = 10,   // Greater
  ICC_LE = 2,   // Less or Equal
  ICC_GE = 11,  // Greater or Equal
  ICC_L = 3,    // Less
  ICC_GU = 12,  // Greater Unsigned
  ICC_LEU = 4,  // Less or Equal Unsigned
  ICC_CC = 13,  // Carry Clear/Great or Equal Unsigned
  ICC_CS = 5,   // Carry Set/Less Unsigned
  ICC_POS = 14, // Positive
  ICC_NEG = 6,  // Negative
  ICC_VC = 15,  // Overflow Clear
  ICC_VS = 7,   // Overflow Set

  FCC_A = 8 + 16,    // Always
  FCC_N = 0 + 16,    // Never
  FCC_U = 7 + 16,    // Unordered
  FCC_G = 6 + 16,    // Greater
  FCC_UG = 5 + 16,   // Unordered or Greater
  FCC_L = 4 + 16,    // Less
  FCC_UL = 3 + 16,   // Unordered or Less
  FCC_LG = 2 + 16,   // Less or Greater
  FCC_NE = 1 + 16,   // Not Equal
  FCC_E = 9 + 16,    // Equal
  FCC_UE = 10 + 16,  // Unordered or Equal
  FCC_GE = 11 + 16,  // Greater or Equal
  FCC_UGE = 12 + 16, // Unordered or Greater or Equal
  FCC_LE = 13 + 16,  // Less or Equal
  FCC_ULE = 14 + 16, // Unordered or Less or Equal
  FCC_O = 15 + 16,   // Ordered

};
}

inline static const char *ELMOCondCodeToString(ELMOCC::CondCodes CC) {
  switch (CC) {
  case ELMOCC::ICC_A:
    return "a";
  case ELMOCC::ICC_N:
    return "n";
  case ELMOCC::ICC_NE:
    return "ne";
  case ELMOCC::ICC_E:
    return "eq";
  case ELMOCC::ICC_G:
    return "g";
  case ELMOCC::ICC_LE:
    return "le";
  case ELMOCC::ICC_GE:
    return "ge";
  case ELMOCC::ICC_L:
    return "lt";
  case ELMOCC::ICC_GU:
    return "gu";
  case ELMOCC::ICC_LEU:
    return "leu";
  case ELMOCC::ICC_CC:
    return "cc";
  case ELMOCC::ICC_CS:
    return "cs";
  case ELMOCC::ICC_POS:
    return "pos";
  case ELMOCC::ICC_NEG:
    return "neg";
  case ELMOCC::ICC_VC:
    return "vc";
  case ELMOCC::ICC_VS:
    return "vs";
  case ELMOCC::FCC_A:
    return "a";
  case ELMOCC::FCC_N:
    return "n";
  case ELMOCC::FCC_U:
    return "u";
  case ELMOCC::FCC_G:
    return "g";
  case ELMOCC::FCC_UG:
    return "ug";
  case ELMOCC::FCC_L:
    return "lt";
  case ELMOCC::FCC_UL:
    return "ul";
  case ELMOCC::FCC_LG:
    return "lg";
  case ELMOCC::FCC_NE:
    return "ne";
  case ELMOCC::FCC_E:
    return "eq";
  case ELMOCC::FCC_UE:
    return "ue";
  case ELMOCC::FCC_GE:
    return "ge";
  case ELMOCC::FCC_UGE:
    return "uge";
  case ELMOCC::FCC_LE:
    return "le";
  case ELMOCC::FCC_ULE:
    return "ule";
  case ELMOCC::FCC_O:
    return "o";
  }
  llvm_unreachable("Invalid cond code");
}
}

#endif
