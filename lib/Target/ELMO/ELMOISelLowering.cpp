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
  // addRegisterClass(MVT::i32, &ELMO::CONDRegClass);

  setStackPointerRegisterToSaveRestore(ELMO::SP);
  setBooleanContents(ZeroOrOneBooleanContent);
  AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i32);

  setOperationAction(ISD::BR_CC, MVT::i32, Custom);
  setOperationAction(ISD::BRCOND, MVT::Other, Expand);

  // setTargetDAGCombine(ISD::BRCOND);
  // Function alignments (log2).
  unsigned FunctionAlignment = 2;
  setMinFunctionAlignment(FunctionAlignment);
  setPrefFunctionAlignment(FunctionAlignment);

  // Effectively disable jump table generation.
  setMinimumJumpTableEntries(INT_MAX);
  computeRegisterProperties(STI.getRegisterInfo());
}
SDValue ELMOTargetLowering::LowerOperation(llvm::SDValue Op,
                                           llvm::SelectionDAG &DAG) const {

  WithColor::note() << "ISelLowering executed\n";
  switch (Op.getOpcode()) {
  default:
    report_fatal_error("unimplemented operand");
  case ISD::SELECT:
    return lowerSelect(Op, DAG);
  case ISD::BR_CC:
    return lowerBR_CC(Op, DAG);
  }
}

const char *ELMOTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case ELMOISD::Ret:
    return "ELMOISD::Ret";
  case ELMOISD::Call:
    return "ELMOISD::Call";
  case ELMOISD::BR_CC:
    return "ELMOISD::BR_CC";
  case ELMOISD::SET_FLAG:
    return "ELMOISD::SET_FLAG";
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
  // Ops.push_back(InFlag);

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

// static ELMOCC::CondCode IntCondCCodeToICC(SDValue CC, const SDLoc &DL,
//                                         SDValue &RHS, SelectionDAG &DAG) {
//   ISD::CondCode SetCCOpcode = cast<CondCodeSDNode>(CC)->get();
//
//   // For integer, only the SETEQ, SETNE, SETLT, SETLE, SETGT, SETGE, SETULT,
//   // SETULE, SETUGT, and SETUGE opcodes are used (see CodeGen/ISDOpcodes.h)
//   // and Lanai only supports integer comparisons, so only provide definitions
//   // for them.
//   switch (SetCCOpcode) {
//   case ISD::SETEQ:
//   case ISD::SETLT:
//   case ISD::SETGT:
//   case ISD::SETLE:
//   case ISD::SETGE:
//
//   case ISD::SETEQ:
//     return LPCC::ICC_EQ;
//   case ISD::SETGT:
//     if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
//       if (RHSC->getZExtValue() == 0xFFFFFFFF) {
//         // X > -1 -> X >= 0 -> is_plus(X)
//         RHS = DAG.getConstant(0, DL, RHS.getValueType());
//         return LPCC::ICC_PL;
//       }
//     return LPCC::ICC_GT;
//   case ISD::SETUGT:
//     return LPCC::ICC_UGT;
//   case ISD::SETLT:
//     if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
//       if (RHSC->getZExtValue() == 0)
//         // X < 0 -> is_minus(X)
//         return LPCC::ICC_MI;
//     return LPCC::ICC_LT;
//   case ISD::SETULT:
//     return LPCC::ICC_ULT;
//   case ISD::SETLE:
//     if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
//       if (RHSC->getZExtValue() == 0xFFFFFFFF) {
//         // X <= -1 -> X < 0 -> is_minus(X)
//         RHS = DAG.getConstant(0, DL, RHS.getValueType());
//         return LPCC::ICC_MI;
//       }
//     return LPCC::ICC_LE;
//   case ISD::SETULE:
//     return LPCC::ICC_ULE;
//   case ISD::SETGE:
//     if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
//       if (RHSC->getZExtValue() == 0)
//         // X >= 0 -> is_plus(X)
//         return LPCC::ICC_PL;
//     return LPCC::ICC_GE;
//   case ISD::SETUGE:
//     return LPCC::ICC_UGE;
//   case ISD::SETNE:
//     return LPCC::ICC_NE;
//   case ISD::SETONE:
//   case ISD::SETUNE:
//   case ISD::SETOGE:
//   case ISD::SETOLE:
//   case ISD::SETOLT:
//   case ISD::SETOGT:
//   case ISD::SETOEQ:
//   case ISD::SETUEQ:
//   case ISD::SETO:
//   case ISD::SETUO:
//     llvm_unreachable("Unsupported comparison.");
//   default:
//     llvm_unreachable("Unknown integer condition code!");
//   }
// }
SDValue ELMOTargetLowering::lowerBR_CC(SDValue Op, SelectionDAG &DAG) const {

  SDValue Chain = Op.getOperand(0);
  SDValue Cond = Op.getOperand(1);
  SDValue LHS = Op.getOperand(2);
  SDValue RHS = Op.getOperand(3);
  SDValue Dest = Op.getOperand(4);
  SDLoc DL(Op);

  ISD::CondCode CC = cast<CondCodeSDNode>(Cond)->get();
  SDValue TargetCC = DAG.getConstant(CC, DL, MVT::i32);

  SDValue Flag =
      DAG.getNode(ELMOISD::SET_FLAG, DL, MVT::Glue, LHS, RHS, TargetCC);

  return DAG.getNode(ELMOISD::BR_CC, DL, Op.getValueType(), Chain, Dest,
                     TargetCC, Flag);
}
SDValue ELMOTargetLowering::lowerSelect(SDValue Op, SelectionDAG &DAG) const {
  SDValue CondV = Op.getOperand(0);
  SDValue TrueV = Op.getOperand(1);
  SDValue FalseV = Op.getOperand(2);
  SDLoc DL(Op);
  MVT XLenVT = MVT::i32;

  // If the result type is XLenVT and CondV is the output of a SETCC node
  // which also operated on XLenVT inputs, then merge the SETCC node into the
  // lowered RISCVISD::SELECT_CC to take advantage of the integer
  // compare+branch instructions. i.e.:
  // (select (setcc lhs, rhs, cc), truev, falsev)
  // -> (riscvisd::select_cc lhs, rhs, cc, truev, falsev)
  /* if (Op.getSimpleValueType() == XLenVT && CondV.getOpcode() == ISD::SETCC &&
      CondV.getOperand(0).getSimpleValueType() == XLenVT) {
      SDValue LHS = CondV.getOperand(0);
      SDValue RHS = CondV.getOperand(1);
      auto CC = cast<CondCodeSDNode>(CondV.getOperand(2));
      ISD::CondCode CCVal = CC->get();

      normaliseSetCC(LHS, RHS, CCVal);

      SDValue TargetCC = DAG.getConstant(CCVal, DL, XLenVT);
      SDVTList VTs = DAG.getVTList(Op.getValueType(), MVT::Glue);
      SDValue Ops[] = {LHS, RHS, TargetCC, TrueV, FalseV};
      return DAG.getNode(ELMOISD::SELECT_CC, DL, VTs, Ops);
  }
   */

  // Otherwise:
  // (select condv, truev, falsev)
  // -> (riscvisd::select_cc condv, zero, setne, truev, falsev)
  SDValue Zero = DAG.getConstant(0, DL, XLenVT);
  SDValue SetNE = DAG.getConstant(ISD::SETNE, DL, XLenVT);

  SDVTList VTs = DAG.getVTList(Op.getValueType(), MVT::Glue);
  SDValue Ops[] = {CondV, Zero, SetNE, TrueV, FalseV};

  return DAG.getNode(ELMOISD::SELECT_CC, DL, VTs, Ops);
}
