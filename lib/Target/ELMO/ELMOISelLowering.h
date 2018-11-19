#ifndef LLVM_LIB_TARGET_ELMO_ELMOISELLOWERING_H
#define LLVM_LIB_TARGET_ELMO_ELMOISELLOWERING_H

#include "ELMO.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace ELMOISD {
enum NodeType { FIRST_NUMBER = ISD::BUILTIN_OP_END, Call, Ret };
}
class ELMOSubtarget;
class ELMOFunctionInfo;

class ELMOTargetLowering : public TargetLowering {
  const ELMOSubtarget *Subtarget;

public:
  ELMOTargetLowering(const TargetMachine &TM, const ELMOSubtarget &STI);
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  const char *getTargetNodeName(unsigned Opcode) const override;

  SDValue LowerCall(CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;
  SDValue LowerCallResult(llvm::SDValue Chain, SDValue InFlag,
                          llvm::CallingConv::ID CallConv, bool isVarArg,
                          const llvm::SmallVectorImpl<ISD::InputArg> &Ins,
                          const llvm::SDLoc &dl, llvm::SelectionDAG &DAG,
                          SmallVectorImpl<SDValue> &InVals) const;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool isVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;
};
}
#endif
