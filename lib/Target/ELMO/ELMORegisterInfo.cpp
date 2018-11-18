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
  return CSR_SaveList;
}

const uint32_t *
ELMORegisterInfo::getCallPreservedMask(const MachineFunction &m,
                                       CallingConv::ID id) const {
  return CSR_RegMask;
}
void ELMORegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS = nullptr) const {}

unsigned ELMORegisterInfo::getFrameRegister(const MachineFunction &m) const {
  return ELMO::FP;
};

BitVector ELMORegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const ELMOSubtarget &Subtarget = MF.getSubtarget<ELMOSubtarget>();
  return Reserved;
}
}
