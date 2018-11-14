#ifndef LLVM_LIB_TARGET_ELMO_ELMOFRAMELOWERING_H
#define LLVM_LIB_TARGET_ELMO_ELMOFRAMELOWERING_H

#include "ELMO.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class ELMOSubTarget;
class ELMOFrameLowering : public TargetFrameLowering {
public:
  explicit ELMOFrameLowering(const ELMOSubTarget &ST);
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  bool hasFP(const MachineFunction &MF) const override;
  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;

  int getFrameIndexReference(const MachineFunction &MF, int FI,
                             unsigned &FrameReg) const override;

  /// targetHandlesStackFrameRounding - Returns true if the target is
  /// responsible for rounding up the stack frame (probably at emitPrologue
  /// time).
  bool targetHandlesStackFrameRounding() const override { return true; }
};
}
#endif
