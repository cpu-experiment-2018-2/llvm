
#ifndef LLVM_LIB_TARGET_ELMO_ELMO_H
#define LLVM_LIB_TARGET_ELMO_ELMO_H

#include "MCTargetDesc/ELMOMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class ELMOTargetMachine;
class FunctionPass;

FunctionPass *createELMOISelDag(ELMOTargetMachine &TM);
} // end namespace llvm;

#endif
