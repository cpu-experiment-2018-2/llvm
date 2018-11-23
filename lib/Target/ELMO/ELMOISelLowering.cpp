#include "ELMO.h"
#include "ELMOISelLowering.h"
#include "ELMORegisterInfo.h"
#include "ELMOTargetMachine.h"
#include "ELMOTargetObjectFile.h"
#include <llvm/CodeGen/CallingConvLower.h>
#include <llvm/Support/WithColor.h>

using namespace llvm;
#include "ELMOGenCallingConv.inc"
#include "ELMOMachineFunctionInfo.h"

ELMOTargetLowering::ELMOTargetLowering(const TargetMachine &TM,
                                       const ELMOSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  addRegisterClass(MVT::i32, &ELMO::ELMOGRRegsRegClass);

  computeRegisterProperties(STI.getRegisterInfo());
  setStackPointerRegisterToSaveRestore(ELMO::SP);
  setBooleanContents(ZeroOrOneBooleanContent);
  setBooleanVectorContents(ZeroOrNegativeOneBooleanContent);
  // AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i32);

  setOperationAction(ISD::BR_CC, MVT::i32, Expand);

  //  setTargetDAGCombine(ISD::BR_CC);
  for (MVT VT : MVT::integer_valuetypes()) {
    setLoadExtAction(ISD::EXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1, Promote);
  }
  setMinFunctionAlignment(2);
}
SDValue ELMOTargetLowering::LowerOperation(llvm::SDValue Op,
                                           llvm::SelectionDAG &DAG) const {

  WithColor::note() << "ISelLowering executed\n";
  return Op;
}

const char *ELMOTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case ELMOISD::Ret:
    return "ELMOISD::Ret";
  default:
    return nullptr;
  }
}

SDValue ELMOTargetLowering::LowerReturn(
    llvm::SDValue Chain, llvm::CallingConv::ID CallConv, bool isVarArg,
    const llvm::SmallVectorImpl<llvm::ISD::OutputArg> &Outs,
    const llvm::SmallVectorImpl<SDValue> &OutVals, const llvm::SDLoc &dl,
    llvm::SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_ELMO);
  SDValue Flag;

  SmallVector<SDValue, 4> RetOps(1, Chain);
  for (unsigned i = 0; i != RVLocs.size(); i++) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }
  RetOps[0] = Chain;
  if (Flag.getNode()) {
    RetOps.push_back(Flag);
  }
  return DAG.getNode(ELMOISD::Ret, dl, MVT::Other, RetOps);
}

SDValue ELMOTargetLowering::LowerCallResult(
    llvm::SDValue Chain, SDValue InFlag, llvm::CallingConv::ID CallConv,
    bool isVarArg, const llvm::SmallVectorImpl<ISD::InputArg> &Ins,
    const llvm::SDLoc &dl, llvm::SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallResult(Ins, RetCC_ELMO);
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    Chain = DAG.getCopyFromReg(Chain, dl, RVLocs[i].getLocReg(),
                               RVLocs[i].getValVT(), InFlag)
                .getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }
  return Chain;
}

SDValue ELMOTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &dl = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  bool &isTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool isVarArg = CLI.IsVarArg;

  // Sparc target does not yet support tail call optimization.

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_ELMO);

  // Get the size of the outgoing arguments stack space requirement.
  unsigned NumBytes = CCInfo.getNextStackOffset();

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, dl);

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;

  for (unsigned i = 0, j = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[i];
    ISD::ArgFlagsTy Flags = Outs[i].Flags;
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      llvm_unreachable("MemLoc are not supproted");
    }
  }
  SDValue InFlag;
  for (unsigned i = 0, e = RegsToPass.size(); i != e; i++) {
    Chain = DAG.getCopyToReg(Chain, dl, RegsToPass[i].first,
                             RegsToPass[i].second, InFlag);
    InFlag = Chain.getValue(1);
  }
  GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee);
  Callee = DAG.getTargetGlobalAddress(G->getGlobal(), dl, MVT::i32);

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(InFlag);

  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }
  if (InFlag.getNode())
    Ops.push_back(InFlag);

  Chain = DAG.getNode(ELMOISD::Call, dl, NodeTys, Ops);
  InFlag = Chain.getValue(1);
  Chain = DAG.getCALLSEQ_END(
      Chain,
      DAG.getConstant(NumBytes, dl, getPointerTy(DAG.getDataLayout()), true),
      DAG.getConstant(0, dl, getPointerTy(DAG.getDataLayout()), true), InFlag,
      dl);
  InFlag = Chain.getValue(1);

  return LowerCallResult(Chain, InFlag, CallConv, isVarArg, Ins, dl, DAG,
                         InVals);
}

SDValue ELMOTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_ELMO);

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    if (VA.isRegLoc()) {
      EVT RegVT = VA.getLocVT();
      const TargetRegisterClass *RC = &ELMO::ELMOGRRegsRegClass;
      if (VA.getLocInfo() != CCValAssign::Full) {
        llvm_unreachable_internal("not supported yet");
      }
      unsigned VReg = RegInfo.createVirtualRegister(RC);
      RegInfo.addLiveIn(VA.getLocReg(), VReg);
      SDValue ArgValue = DAG.getCopyFromReg(Chain, dl, VReg, RegVT);
      InVals.push_back(ArgValue);
    } else {
      assert(VA.isMemLoc());
      unsigned ObjSize = VA.getLocVT().getSizeInBits() / 8;
      int FI = MFI.CreateFixedObject(ObjSize, VA.getLocMemOffset(), true);

      SDValue FIN = DAG.getFrameIndex(FI, MVT::i32);
      InVals.push_back(DAG.getLoad(VA.getLocVT(), dl, Chain, FIN,
                                   MachinePointerInfo::getFixedStack(MF, FI)));
    }
  }
  return Chain;
}
