#include "ELMOFrameLowering.h"
#include "ELMOInstrInfo.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMOSubtarget.h"
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/Support/WithColor.h>
namespace llvm {
/*ELMOFrameLowering::ELMOFrameLowering(ELMOSubTarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
                           {}*/
ELMOFrameLowering::ELMOFrameLowering(ELMOSubtarget &St)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 1, 0,
                          1) // 怪しいから治す
{}

void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {

  WithColor::note() << "Emit epilogue\n";
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  /*
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  uint64_t StackSize = 4;
  if (!StackSize) {
    return;
  }
  unsigned StackReg = ELMO::SP;
  */

  unsigned OffsetReg = 0;
  // if (OffsetReg) {
  //   BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDi), StackReg)
  //       .addReg(StackReg)
  //       .addReg(OffsetReg)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // } else {
  //   BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDi), StackReg)
  //       .addReg(StackReg)
  //       .addImm(StackSize)
  //       .setMIFlag(MachineInstr::FrameSetup);
  // }
}

void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  WithColor::note() << "Emit prologue\n";
}
/*
MachineBasicBlock::iterator ELMOFrameLowering::eliminateCallFramePseudoInstr(
    llvm::MachineFunction &MF, llvm::MachineBasicBlock &MBB,
    llvm::MachineBasicBlock::iterator I) const {
  return I;
}*/
bool ELMOFrameLowering::hasReservedCallFrame(
    const llvm::MachineFunction &MF) const {
  const MachineFrameInfo *MFI = &MF.getFrameInfo();

  // Reserve call frame if the size of the maximum call frame fits into 16-bit
  //   // immediate field and there are no variable sized objects on the stack.
  //     // Make sure the second register scavenger spill slot can be accessed
  //     with one
  //       // instruction.
  return isInt<16>(MFI->getMaxCallFrameSize() + getStackAlignment()) &&
         !MFI->hasVarSizedObjects();
}
bool ELMOFrameLowering::hasFP(const MachineFunction &MF) const { return true; }
void ELMOFrameLowering::determineCalleeSaves(llvm::MachineFunction &MF,
                                             llvm::BitVector &SavedRegs,
                                             llvm::RegScavenger *RS) const {}
int ELMOFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                              unsigned &FrameReg) const {
  return 0;
}
}
