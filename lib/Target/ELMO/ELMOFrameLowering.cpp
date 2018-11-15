#include "ELMOFrameLowering.h"
#include "ELMOInstrInfo.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMOSubtarget.h"
namespace llvm {
ELMOFrameLowering::ELMOFrameLowering(const ELMOSubTarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
{}
void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}
void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {}
}