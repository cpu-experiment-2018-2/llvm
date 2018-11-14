#ifndef LLVM_LIB_TARGET_ELMO_ELMOREGISTERINFO_H
#define LLVM_LIB_TARGET_ELMO_ELMOREGISTERINFO_H

#include "ELMO.h"

#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "ELMOGenRegisterInfo.inc"

namespace llvm {
struct ELMORegisterInfo : public ELMOGenRegisterInfo {
  ELMORegisterInfo();
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *) const override;
  const uint32_t *getCallPreservedMask(const MachineFunction &,
                                       CallingConv::ID) const override;
  BitVector getReservedRegs(const MachineFunction &) const override;
  void eliminateFrameIndex(MachineBasicBlock::iterator, int, unsigned,
                           RegScavenger *) const override;
  unsigned getFrameRegister(const MachineFunction &) const override;
};
}
#endif
