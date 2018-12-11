#include "ELMO.h"
#include "ELMOInstrInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "ELMOGenInstrInfo.inc"

namespace llvm {
// Pin the vtable to this file.
void ELMOInstrInfo::anchor() {}

ELMOInstrInfo::ELMOInstrInfo(ELMOSubtarget &ST)
    : ST(ST), ELMOGenInstrInfo(ELMO::ADJCALLSTACKDOWN, ELMO::ADJCALLSTACKUP) {}
void ELMOInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator Position,
                                const DebugLoc &DL,
                                unsigned DestinationRegister,
                                unsigned SourceRegister,
                                bool KillSource) const {
  BuildMI(MBB, Position, DL, get(ELMO::ADDiu), DestinationRegister)
      .addReg(SourceRegister, getKillRegState(KillSource))
      .addImm(0);
}

unsigned ELMOInstrInfo::insertBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond, const DebugLoc &DL, int *BytesAdded) const {
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 1 || Cond.size() == 0) &&
         "ELMO branch conditions should have one component!");
  assert(!BytesAdded && "code size not handled");

  if (Cond.empty()) {
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(ELMO::JMP)).addMBB(TBB);
    return 1;
  }

  // Conditional branch
  unsigned CC = Cond[0].getImm();

  if (CC < 16)
    BuildMI(&MBB, DL, get(ELMO::BRICC)).addMBB(TBB).addImm(CC);
  else
    BuildMI(&MBB, DL, get(ELMO::BRFCC)).addMBB(TBB).addImm(CC);
  if (!FBB)
    return 1;

  BuildMI(&MBB, DL, get(ELMO::JMP)).addMBB(FBB);
  return 2;
}
void ELMOInstrInfo::loadRegFromStackSlot(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator Position,
    unsigned DestinationRegister, int FrameIndex,
    const llvm::TargetRegisterClass *RegisterClass,
    const llvm::TargetRegisterInfo *RegisterInfo) const {

  DebugLoc dl;
  if (Position != MBB.end()) {
    dl = Position->getDebugLoc();
  }
  MachineFunction *MF = MBB.getParent();
  MachineFrameInfo &MFI = MF->getFrameInfo();

  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FrameIndex),
      MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIndex),
      MFI.getObjectAlignment(FrameIndex));

  BuildMI(MBB, Position, dl, get(ELMO::LOAD))
      .addReg(DestinationRegister, RegState::Define)
      .addFrameIndex(FrameIndex)
      .addImm(0)
      .addMemOperand(MMO);
}
void ELMOInstrInfo::storeRegToStackSlot(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator Position,
    unsigned SourceRegister, bool IsKill, int FrameIndex,
    const llvm::TargetRegisterClass *RegisterClass,
    const llvm::TargetRegisterInfo *RegisterInfo) const {
  DebugLoc dl;
  if (Position != MBB.end()) {
    dl = Position->getDebugLoc();
  }
  MachineFunction *MF = MBB.getParent();
  MachineFrameInfo &MFI = MF->getFrameInfo();

  MachineMemOperand *MMO = MF->getMachineMemOperand(
      MachinePointerInfo::getFixedStack(*MF, FrameIndex),
      MachineMemOperand::MOStore, MFI.getObjectSize(FrameIndex),
      MFI.getObjectAlignment(FrameIndex));

  BuildMI(MBB, Position, dl, get(ELMO::STORE))
      .addReg(SourceRegister, getKillRegState(IsKill))
      .addFrameIndex(FrameIndex)
      .addImm(0)
      .addMemOperand(MMO);
}
}
