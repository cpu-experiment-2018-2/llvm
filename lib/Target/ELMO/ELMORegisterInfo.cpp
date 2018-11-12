#include "ELMO.h"
#include "ELMOFrameLowering.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMORegisterInfo.h"
#include "ELMOSubtarget.h"
#define GET_REGINFO_TARGET_DESC
#include "ELMOGenRegisterInfo.inc"

namespace llvm {
ELMORegisterInfo::ELMORegisterInfo() : ELMOGenRegisterInfo(0){};
const MCPhysReg *
ELMORegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return (uint16_t *)0;
}

const uint32_t *
ELMORegisterInfo::getCallPreservedMask(const MachineFunction &m,
                                       CallingConv::ID id) const {
  return (uint32_t *)0;
}
void ELMORegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS = nullptr) const {}

unsigned ELMORegisterInfo::getFrameRegister(const MachineFunction &m) const {
  return ELMO::FP;
};
}
