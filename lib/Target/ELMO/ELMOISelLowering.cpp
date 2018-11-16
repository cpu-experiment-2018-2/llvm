#include "ELMOISelLowering.h"
#include "ELMO.h"
#include "ELMORegisterInfo.h"
#include "ELMOTargetMachine.h"
#include "ELMOTargetObjectFile.h"

using namespace llvm;
ELMOTargetLowering::ELMOTargetLowering(const TargetMachine &TM,
                                       const ELMOSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  addRegisterClass(MVT::i32, &ELMO::ELMOGRRegsRegClass);
  computeRegisterProperties(STI.getRegisterInfo());
}
SDValue ELMOTargetLowering::LowerOperation(llvm::SDValue Op,
                                           llvm::SelectionDAG &DAG) const {
  return Op;
}