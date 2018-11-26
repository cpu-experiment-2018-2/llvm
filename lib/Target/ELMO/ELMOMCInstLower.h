#ifndef LLVM_LIB_TARGET_ELMO_ELMOINSTLOWER_H
#define LLVM_LIB_TARGET_ELMO_ELMOINSTLOWER_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MCSymbol;
class MachineInstr;
class MachineModuleInfoMachO;
class MachineOperand;
class Mangler;

// LanaiMCInstLower - This class is used to lower an MachineInstr
// into an MCInst.
class LLVM_LIBRARY_VISIBILITY ELMOMCInstLower {
  MCContext &Ctx;

  AsmPrinter &Printer;

public:
  ELMOMCInstLower(MCContext &CTX, AsmPrinter &AP) : Ctx(CTX), Printer(AP) {}
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  MCOperand LowerOperand(const MachineOperand &MO, unsigned offset) const;

  // MCOperand LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym)
  // const;

  // MCSymbol *GetGlobalAddressSymbol(const MachineOperand &MO) const;
  // MCSymbol *GetBlockAddressSymbol(const MachineOperand &MO) const;
  // MCSymbol *GetExternalSymbolSymbol(const MachineOperand &MO) const;
  // MCSymbol *GetJumpTableSymbol(const MachineOperand &MO) const;
  // MCSymbol *GetConstantPoolIndexSymbol(const MachineOperand &MO) const;
};
} // namespace llvm
#endif
