#include "ELMOFrameLowering.h"
#include "ELMOInstrInfo.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMOSubtarget.h"
namespace llvm {
/*ELMOFrameLowering::ELMOFrameLowering(ELMOSubTarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
                           {}*/
ELMOFrameLowering::ELMOFrameLowering(ELMOSubtarget &St)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
{}

void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}

void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}

MachineBasicBlock::iterator ELMOFrameLowering::eliminateCallFramePseudoInstr(
    llvm::MachineFunction &MF, llvm::MachineBasicBlock &MBB,
    llvm::MachineBasicBlock::iterator I) const {}
bool ELMOFrameLowering::hasReservedCallFrame(
    const llvm::MachineFunction &MF) const {}
bool ELMOFrameLowering::hasFP(const MachineFunction &MF) const {}
void ELMOFrameLowering::determineCalleeSaves(llvm::MachineFunction &MF,
                                             llvm::BitVector &SavedRegs,
                                             llvm::RegScavenger *RS) const {}
int ELMOFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                              unsigned &FrameReg) const {
  return 0;
}
}
