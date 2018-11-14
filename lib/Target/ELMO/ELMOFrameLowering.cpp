#include "ELMOFrameLowering.h"

namespace llvm {
ELMOFrameLowering::ELMOFrameLowering(const llvm::ELMOSubTarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
{}
void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     llvm::MachineBasicBlock &MBB) const {}
void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     llvm::MachineBasicBlock &MBB) const {}
}