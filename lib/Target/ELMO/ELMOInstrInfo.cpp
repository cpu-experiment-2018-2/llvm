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

ELMOInstrInfo::ELMOInstrInfo(ELMOSubtarget &ST) : ST(ST) {}
void ELMOInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator Position,
                                const DebugLoc &DL,
                                unsigned DestinationRegister,
                                unsigned SourceRegister,
                                bool KillSource) const {
  BuildMI(MBB, Position, DL, get(ELMO::ADD), DestinationRegister)
      .addReg(SourceRegister, getKillRegState(KillSource))
      .addReg(ELMO::ZERO);
}
}
