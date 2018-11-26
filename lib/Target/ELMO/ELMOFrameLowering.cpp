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
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 1, 0),
      STI(St) // 怪しいから治す
{}

void ELMOFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {

  WithColor::note() << "Emit epilogue\n";
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  uint64_t StackSize = MF.getFrameInfo().getStackSize();

  if (!StackSize) {
    WithColor::note() << "Emit epilogue end\n";

    return;
  }
  unsigned StackReg = ELMO::SP;

  unsigned OffsetReg = 0;
  /*  if (OffsetReg) {
      BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADD), StackReg)
          .addReg(StackReg)
          .addReg(OffsetReg)
          .setMIFlag(MachineInstr::FrameSetup);
    } else {
    */
  BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDiu), StackReg)
      .addReg(StackReg)
      .addImm(StackSize)
      .setMIFlag(MachineInstr::FrameSetup);

  WithColor::note() << "Emit epilogue end\n";
  /*
}
*/
}

void ELMOFrameLowering::emitPrologue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  WithColor::note() << "Emit prologue\n";
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc dl = MBBI->getDebugLoc();

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();
  if (!StackSize) {
    WithColor::note() << "Emit prologue end\n";

    return;
  }

  // Adjust stack.
  BuildMI(MBB, MBBI, dl, TII.get(ELMO::ADDiu), ELMO::SP)
      .addReg(ELMO::SP)
      .addImm(StackSize);
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
  // unsigned SPReg = ELMO::SP;
  WithColor::note() << "PSEUDO ELIMMINATE\n";
  // DebugLoc DL = MI->getDebugLoc();
  //
  // if (!hasReservedCallFrame(MF)) {
  //   // If space has not been reserved for a call frame, ADJCALLSTACKDOWN and
  //   // ADJCALLSTACKUP must be converted to instructions manipulating the
  //   stack
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
