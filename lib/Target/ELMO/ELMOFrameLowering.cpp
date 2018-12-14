#include "ELMOFrameLowering.h"
#include "ELMOInstrInfo.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMOSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/Support/WithColor.h>
namespace llvm {
/*ELMOFrameLowering::ELMOFrameLowering(ELMOSubTarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 0, 0,
                          0) // 怪しいから治す
                           {}*/
ELMOFrameLowering::ELMOFrameLowering(ELMOSubtarget &St)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 1, 0),
      STI(St) // 怪しいから治す
{}
// int ELMOFrameLowering::getFrameIndexReference(const MachineFunction &MF, int
// FI,
//                                               unsigned &FrameReg) const {
//   const MachineFrameInfo &MFI = MF.getFrameInfo();
//   const TargetRegisterInfo *RI = MF.getSubtarget().getRegisterInfo();
//   const auto *RVFI = MF.getInfo<ELMOMachineFunctionInfo>();
//
//   // Callee-saved registers should be referenced relative to the stack
//   // pointer (positive offset), otherwise use the frame pointer (negative
//   // offset).
//   const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
//   int MinCSFI = 0;
//   int MaxCSFI = -1;
//
//   int Offset = MFI.getObjectOffset(FI) - getOffsetOfLocalArea() +
//                MFI.getOffsetAdjustment();
//
//   if (CSI.size()) {
//     MinCSFI = CSI[0].getFrameIdx();
//     MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
//   }
//
//   if (FI >= MinCSFI && FI <= MaxCSFI) {
//     FrameReg = RISCV::X2;
//     Offset += MF.getFrameInfo().getStackSize();
//   } else {
//     FrameReg = RI->getFrameRegister(MF);
//     if (hasFP(MF))
//       Offset += RVFI->getVarArgsSaveSize();
//     else
//       Offset += MF.getFrameInfo().getStackSize();
//   }
//   return Offset;
// }
void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {

  WithColor::note() << "Emit epilogue\n";
  MBB.dump();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  auto MFI = MF.getFrameInfo();
  unsigned StackReg = ELMO::SP;

  if (MFI.hasStackObjects()) {
    BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDiu), ELMO::SP)
        .addReg(ELMO::FP)
        .addImm(0);
    BuildMI(MBB, MBBI, dl, TII.get(ELMO::LOAD), ELMO::FP)
        .addReg(ELMO::FP)
        .addImm(0);
  }

  WithColor::note() << "Emit epilogue end\n";
  /*
}
*/
}

void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  WithColor::note() << "Emit prologue\n";
  MBB.dump();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  MachineBasicBlock::iterator MBBI = MBB.begin();

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc dl = MBBI->getDebugLoc();

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  if (MFI.hasStackObjects()) {
    BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDiu), ELMO::FP)
        .addReg(ELMO::SP)
        .addImm(0);
    if (StackSize) {
      // アホなので治したい
      StackSize += 4;
      // Adjust stack.
      BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDiu), ELMO::SP)
          .addReg(ELMO::SP)
          //.addImm(StackSize)
          .addImm(StackSize / 4)
          .setMIFlag(MachineInstr::FrameSetup);
    }
    //
    BuildMI(MBB, MBBI, dl, TII.get(ELMO::STORE), ELMO::FP)
        .addReg(ELMO::SP)
        .addImm(0);
  }

  WithColor::note() << "Emit prologue end\n";
}
void ELMOFrameLowering::adjustReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MBBI,
                                  const DebugLoc &DL, unsigned DestReg,
                                  unsigned SrcReg, int64_t Val,
                                  MachineInstr::MIFlag Flag) const {
  MachineRegisterInfo &MRI = MBB.getParent()->getRegInfo();
  const TargetInstrInfo *TII = STI.getInstrInfo();

  if (DestReg == SrcReg && Val == 0)
    return;
  if (isInt<16>(Val)) {
    BuildMI(MBB, MBBI, DL, TII->get(ELMO::ADDiu), DestReg)
        .addReg(SrcReg)
        .addImm(Val)
        .setMIFlag(Flag);
  } else {
    report_fatal_error("adjustReg cannot yet handle adjustments >16 bits");
  }
}
bool ELMOFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}
MachineBasicBlock::iterator ELMOFrameLowering::eliminateCallFramePseudoInstr(
    llvm::MachineFunction &MF, llvm::MachineBasicBlock &MBB,
    llvm::MachineBasicBlock::iterator MI) const {
  unsigned SPReg = ELMO::SP;
  WithColor::note() << "PSEUDO ELIMMINATE\n";
  // DebugLoc DL = MI->getDebugLoc();
  //
  // if (!hasReservedCallFrame(MF)) {
  //   // If space has not been reserved for a call frame, ADJCALLSTACKDOWN and
  //   // ADJCALLSTACKUP must be converted to instructions manipulating the
  //   // pointer. This is necessary when there is a variable length stack
  //   // allocation (e.g. alloca), which means it's not possible to allocate
  //   // space for outgoing arguments from within the function prologue.
  //   int64_t Amount = MI->getOperand(0).getImm();
  //
  //   if (Amount != 0) {
  //     // Ensure the stack remains aligned after adjustment.
  //     Amount = alignSPAdjust(Amount);
  //
  //     if (MI->getOpcode() == ELMO::ADJCALLSTACKDOWN)
  //       Amount = -Amount;
  //
  //     adjustReg(MBB, MI, DL, SPReg, SPReg, Amount, MachineInstr::NoFlags);
  //   }
  // }

  return MBB.erase(MI);
}

bool ELMOFrameLowering::hasFP(const MachineFunction &MF) const { return true; }
// }
}
