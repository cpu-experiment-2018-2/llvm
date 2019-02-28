#include "ELMO.h"
#include "ELMOFrameLowering.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMORegisterInfo.h"
#include "ELMOSubtarget.h"
#define GET_REGINFO_TARGET_DESC
#include "ELMOGenRegisterInfo.inc"

namespace llvm {
ELMORegisterInfo::ELMORegisterInfo() : ELMOGenRegisterInfo(ELMO::LK){};
const MCPhysReg *
ELMORegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

const uint32_t *
ELMORegisterInfo::getCallPreservedMask(const MachineFunction &m,
                                       CallingConv::ID id) const {
  return CSR_RegMask;
}
void ELMORegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS = nullptr) const {

  // get instr
  MachineInstr &MI = *II;
  // get basic block function
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  // get instr info
  const ELMOInstrInfo *TII = MF.getSubtarget<ELMOSubtarget>().getInstrInfo();
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  unsigned FrameReg;

  int Offset =
      (getFrameLowering(MF)->getFrameIndexReference(MF, FrameIndex, FrameReg) +
       MI.getOperand(FIOperandNum + 1).getImm()) /
          4 +
      1;

  if (!isInt<32>(Offset)) {
    report_fatal_error(
        "Frame offsets outside of the signed 32-bit range not supported");
  }

  MachineBasicBlock &MBB = *MI.getParent();
  bool FrameRegIsKill = false;

  MI.getOperand(FIOperandNum)
      .ChangeToRegister(FrameReg, false, false, FrameRegIsKill);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned ELMORegisterInfo::getFrameRegister(const MachineFunction &m) const {
  return ELMO::SP;
};
bool ELMORegisterInfo::isConstantPhysReg(unsigned PhysReg) const {
  return PhysReg == ELMO::ZERO || PhysReg == ELMO::ONE;
}

BitVector ELMORegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  markSuperRegs(Reserved, ELMO::LK);
  markSuperRegs(Reserved, ELMO::SP);
  markSuperRegs(Reserved, ELMO::ONE);
  markSuperRegs(Reserved, ELMO::ZERO);

  const ELMOSubtarget &Subtarget = MF.getSubtarget<ELMOSubtarget>();
  return Reserved;
}
} // namespace llvm
