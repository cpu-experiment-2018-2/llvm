#ifndef LLVM_LIB_TARGET_ELMO_ELMOFRAMELOWERING_H
#define LLVM_LIB_TARGET_ELMO_ELMOFRAMELOWERING_H

#include "ELMO.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class ELMOSubtarget;
class ELMOFrameLowering : public TargetFrameLowering {
  ELMOSubtarget &STI;

public:
  ELMOFrameLowering(ELMOSubtarget &);
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  bool hasFP(const MachineFunction &MF) const override;
  // void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
  //                           RegScavenger *RS = nullptr) const override;

  // int getFrameIndexReference(const MachineFunction &MF, int FI,
  //                            unsigned &FrameReg) const override;
  //
  /// targetHandlesStackFrameRounding - Returns true if the target is
  /// responsible for rounding up the stack frame (probably at emitPrologue
  /// time).
  bool targetHandlesStackFrameRounding() const override { return true; }
  void adjustReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                 const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                 int64_t Val, MachineInstr::MIFlag Flag) const;
};
}
#endif
