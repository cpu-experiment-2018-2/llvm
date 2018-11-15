#include "ELMOISelLowering.h"
#include "ELMORegisterInfo.h"
#include "ELMOTargetMachine.h"
#include "ELMOTargetObjectFile.h"

using namespace llvm;
ELMOTargetLowering::ELMOTargetLowering(const TargetMachine &TM,
                                       const ELMOSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  computeRegisterProperties(STI.getRegisterInfo());
}