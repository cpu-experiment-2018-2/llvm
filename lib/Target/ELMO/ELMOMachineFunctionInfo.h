#ifndef LLVM_LIB_TARGET_ELMO_ELMOMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_ELMO_ELMOMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"
namespace llvm {
class ELMOMachineFunctionInfo : public MachineFunctionInfo {
  // MachineFunction &MF;

public:
  ELMOMachineFunctionInfo() {}
  explicit ELMOMachineFunctionInfo(MachineFunction &MF) {}
  virtual void anchor();
};
}
#endif
