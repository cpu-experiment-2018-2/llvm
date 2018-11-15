#ifndef LLVM_LIB_TARGET_ELMO_ELMOISELLOWERING_H
#define LLVM_LIB_TARGET_ELMO_ELMOISELLOWERING_H

#include "ELMO.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class ELMOSubtarget;

class ELMOTargetLowering : public TargetLowering {
  const ELMOSubtarget *Subtarget;

public:
  ELMOTargetLowering(const TargetMachine &TM, const ELMOSubtarget &STI);
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
};
}
#endif