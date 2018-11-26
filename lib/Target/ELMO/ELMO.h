
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

#endif
