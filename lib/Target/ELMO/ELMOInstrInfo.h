#ifndef LLVM_LIB_TARGET_ELMO_ELMOINSTRINFO_H
#define LLVM_LIB_TARGET_ELMO_ELMOINSTRINFO_H

#include "ELMORegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "ELMOGenInstrInfo.inc"

namespace llvm {
class ELMOSubtarget;

class ELMOInstrInfo : public ELMOGenInstrInfo {
  const ELMORegisterInfo RI;
  const ELMOSubtarget &ST;

  virtual void anchor();

public:
  ELMOInstrInfo(ELMOSubtarget &ST);

  const ELMORegisterInfo &getRegisterInfo() const { return RI; }

  unsigned insertBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond, const DebugLoc &DL, int *BytesAdded) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
                   const DebugLoc &DL, unsigned DestinationRegister,
                   unsigned SourceRegister, bool KillSource) const override;
  void
  storeRegToStackSlot(MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator Position,
                      unsigned SourceRegister, bool IsKill, int FrameIndex,
                      const TargetRegisterClass *RegisterClass,
                      const TargetRegisterInfo *RegisterInfo) const override;

  void
  loadRegFromStackSlot(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator Position,
                       unsigned DestinationRegister, int FrameIndex,
                       const TargetRegisterClass *RegisterClass,
                       const TargetRegisterInfo *RegisterInfo) const override;
};
} // end namespace llvm

#endif
