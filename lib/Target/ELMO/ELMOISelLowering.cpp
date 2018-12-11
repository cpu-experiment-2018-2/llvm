#include "ELMO.h"
#include "ELMOISelLowering.h"
#include "ELMOMachineFunctionInfo.h"
#include "ELMORegisterInfo.h"
#include "ELMOTargetMachine.h"
#include "ELMOTargetObjectFile.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include <llvm/CodeGen/CallingConvLower.h>
#include <llvm/Support/WithColor.h>

using namespace llvm;
#include "ELMOGenCallingConv.inc"
#include "ELMOMachineFunctionInfo.h"

static ELMOCC::CondCodes CondCCodeToICC(ISD::CondCode CC) {
  switch (CC) {
  default:
    llvm_unreachable("unknown condition code");
  // 符号無しとかはここでlowerしてもよいかもね
  case ISD::SETEQ:
    return ELMOCC::ICC_E;
  case ISD::SETNE:
    return ELMOCC::ICC_NE;
  case ISD::SETLT:
    return ELMOCC::ICC_L;
  case ISD::SETGT:
    return ELMOCC::ICC_G;
  case ISD::SETLE:
    return ELMOCC::ICC_LE;
  case ISD::SETGE:
    return ELMOCC::ICC_GE;
  case ISD::SETOEQ:
    return ELMOCC::FCC_E;
  case ISD::SETO:
    // いいんか
    return ELMOCC::FCC_E;
  case ISD::SETONE:
    return ELMOCC::FCC_NE;
  case ISD::SETOLT:
    return ELMOCC::FCC_L;
  case ISD::SETOGT:
    return ELMOCC::FCC_G;
  case ISD::SETOLE:
    return ELMOCC::FCC_LE;
  case ISD::SETOGE:
    return ELMOCC::FCC_GE;

  case ISD::SETUEQ:
    return ELMOCC::FCC_E;
  case ISD::SETUNE:
    return ELMOCC::FCC_NE;
  case ISD::SETULT:
    return ELMOCC::FCC_L;
  case ISD::SETUGT:
    return ELMOCC::FCC_G;
  case ISD::SETULE:
    return ELMOCC::FCC_LE;
  case ISD::SETUGE:
    return ELMOCC::FCC_GE;
  }
}

ELMOTargetLowering::ELMOTargetLowering(const TargetMachine &TM,
                                       const ELMOSubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  addRegisterClass(MVT::i32, &ELMO::ELMOGRRegsRegClass);
  addRegisterClass(MVT::f32, &ELMO::ELMOGRFRegsRegClass);

  // addRegisterClass(MVT::i32, &ELMO::CONDRegClass);

  setStackPointerRegisterToSaveRestore(ELMO::SP);
  setBooleanContents(ZeroOrOneBooleanContent);
  AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i32);
  setOperationAction(ISD::BR_CC, MVT::i32, Custom);
  setOperationAction(ISD::BR_CC, MVT::f32, Custom);
  // setOperationAction(ISD::ConstantPool, MVT::i32, Custom);

  // AddPromotedToType(ISD::ConstantFP, MVT::f32, MVT::i32);
  // for (auto CC : FPCCToExtend)
  //   setCondCodeAction(CC, MVT::f32, Expand);
  // for (auto Op : FPOpToExtend)
  //   setOperationAction(Op, MVT::f32, Expand);

  setOperationAction(ISD::BRCOND, MVT::Other, Expand);
  setOperationAction(ISD::SELECT_CC, MVT::i32, Custom);
  setOperationAction(ISD::SELECT_CC, MVT::f32, Custom);
  setOperationAction(ISD::BITCAST, MVT::i32, Expand);
  setOperationAction(ISD::BITCAST, MVT::f32, Expand);
  setOperationAction(ISD::SELECT, MVT::i32, Expand);
  setOperationAction(ISD::SELECT, MVT::f32, Expand);
  setOperationAction(ISD::SETCC, MVT::i32, Expand);
  setOperationAction(ISD::SETCC, MVT::f32, Expand);
  setOperationAction(ISD::ConstantFP, MVT::f32, Legal);
  // setOperationAction(ISD::STACKSAVE, MVT::Other, Expand);
  // setOperationAction(ISD::STACKRESTORE, MVT::Other, Expand);
  //
  setOperationAction(ISD::XOR, MVT::i32, Expand);
  ISD::CondCode FPCCToExtend[] = {ISD::SETO};
  for (auto Op : FPCCToExtend)
    setOperationAction(Op, MVT::f32, Expand);

  // setOperationAction(ISD::ConstantFP, MVT::f32, Custom);
  for (auto N : {ISD::EXTLOAD, ISD::SEXTLOAD, ISD::ZEXTLOAD}) {
    setLoadExtAction(N, MVT::i32, MVT::i1, Promote);
  }

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
  case ISD::SELECT_CC:
    return lowerSelect_CC(Op, DAG);
  case ISD::BR_CC:
    return lowerBR_CC(Op, DAG);
  case ISD::ConstantPool:
    return lowerConstantPool(Op, DAG);
  case ISD::ConstantFP:
    return lowerConstantFP(Op, DAG);
    // case ISD::FRAMEADDR:
    //   return lowerFRAMEADDR(Op, DAG);
  }
}

const char *ELMOTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case ELMOISD::Ret:
    return "ELMOISD::Ret";
  case ELMOISD::Call:
    return "ELMOISD::Call";
  case ELMOISD::BRICC:
    return "ELMOISD::BRICC";
  case ELMOISD::BRFCC:
    return "ELMOISD::BRFCC";
  case ELMOISD::SELECT_ICC:
    return "ELMOISD::SELECT_ICC";
  case ELMOISD::SELECT_FCC:
    return "ELMOISD::SELECT_FCC";
  case ELMOISD::SET_FLAGI:
    return "ELMOISD::SET_FLAGI";
  case ELMOISD::SET_FLAGF:
    return "ELMOISD::SET_FLAGF";

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
  isTailCall = false;

  CallingConv::ID CallConv = CLI.CallConv;
  bool isVarArg = CLI.IsVarArg;
  MachineFunction &MF = DAG.getMachineFunction();
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
  Ops.push_back(Callee);

  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }
  if (!isTailCall) {
    const TargetRegisterInfo *TRI = Subtarget->getRegisterInfo();
    const uint32_t *Mask = TRI->getCallPreservedMask(MF, CallConv);
    Ops.push_back(DAG.getRegisterMask(Mask));
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

SDValue ELMOTargetLowering::lowerBR_CC(SDValue Op, SelectionDAG &DAG) const {

  SDValue Chain = Op.getOperand(0);
  SDValue Cond = Op.getOperand(1);
  SDValue LHS = Op.getOperand(2);
  SDValue RHS = Op.getOperand(3);
  SDValue Dest = Op.getOperand(4);
  SDLoc DL(Op);

  ISD::CondCode CC = cast<CondCodeSDNode>(Cond)->get();
  unsigned cc = ~0u;
  DAG.dump();
  cc = CondCCodeToICC(CC);

  bool swap_condition = false;
  bool swap_cont = true;

  // switch (cc) {
  // case ELMOCC::ICC_G:
  //   cc = ELMOCC::ICC_L;
  //   swap_condition = false;
  //   break;
  // case ELMOCC::ICC_GE:
  //   cc = ELMOCC::ICC_LE;
  //   swap_condition = false;
  //   break;
  // case ELMOCC::ICC_NE:
  //   break;
  // defuault:
  //   break;
  // }

  SDValue TargetCC = DAG.getConstant(cc, DL, MVT::i32);
  SDValue Flag = DAG.getNode(
      LHS.getValueType() == MVT::i32 ? ELMOISD::SET_FLAGI : ELMOISD::SET_FLAGF,
      DL, MVT::Glue, LHS, RHS);

  return DAG.getNode(LHS.getValueType() == MVT::i32 ? ELMOISD::BRICC
                                                    : ELMOISD::BRFCC,
                     DL, MVT::Other, Chain, Dest, TargetCC, Flag);
}
SDValue ELMOTargetLowering::lowerSelect_CC(SDValue Op,
                                           SelectionDAG &DAG) const {
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(4))->get();
  SDValue TrueVal = Op.getOperand(2);
  SDValue FalseVal = Op.getOperand(3);
  SDLoc dl(Op);
  unsigned Opc, SPCC = ~0U;

  // If this is a select_cc of a "setcc", and if the setcc got lowered into
  // an CMP[IF]CC/SELECT_[IF]CC pair, find the original compared values.

  SDValue CompareFlag;
  if (LHS.getValueType() == MVT::i32) {
    CompareFlag = DAG.getNode(ELMOISD::SET_FLAGI, dl, MVT::Glue, LHS, RHS);
    Opc = ELMOISD::SELECT_ICC;
    SPCC = CondCCodeToICC(CC);
  } else {
    CompareFlag = DAG.getNode(ELMOISD::SET_FLAGF, dl, MVT::Glue, LHS, RHS);
    Opc = ELMOISD::SELECT_FCC;
    SPCC = CondCCodeToICC(CC);
  }
  return DAG.getNode(Opc, dl, TrueVal.getValueType(), TrueVal, FalseVal,
                     DAG.getConstant(SPCC, dl, MVT::i32), CompareFlag);
}
SDValue ELMOTargetLowering::lowerConstantFP(SDValue Op,
                                            SelectionDAG &DAG) const {
  // auto CurDAG = DAG;
  // auto N = cast<ConstantFPSDNode>(Op);
  // APInt x = N->getValueAPF().bitcastToAPInt();
  // SDValue CPAV = CurDAG->getTargetConstant(x, DL, EVT(MVT::i32));
  // CPAV->dump();
  // ReplaceNode(Node, CurDAG->getMachineNode(ELMO::LWI, DL, EVT(MVT::i32),
  // CPAV));
  // CurDAG->dump();

  // SDLoc DL(Op);
  // EVT Ty = Op.getValueType();
  // dbgs() << "lower constantFP";
  //
  // ConstantFPSDNode *N = cast<ConstantFPSDNode>(Op);
  // float y = N->getConstantFPValue()->getValueAPF().convertToFloat();
  // int z = *(int *)(&z);
  // SDValue L = DAG.getTargetConstant(z, DL, EVT(MVT::i32));
  // SDValue M = SDValue(DAG.getMachineNode(ELMO::LWI, DL, EVT(MVT::i32), L),
  // 0);
  // M->dump();
  return Op;
}
SDValue ELMOTargetLowering::lowerConstantPool(SDValue Op,
                                              SelectionDAG &DAG) const {
  SDLoc DL(Op);
  EVT Ty = Op.getValueType();
  ConstantPoolSDNode *N = cast<ConstantPoolSDNode>(Op);
  const Constant *CPA = N->getConstVal();
  dbgs() << "lower constant";

  if (!isPositionIndependent()) {
    SDValue CPAV = DAG.getTargetConstantPool(CPA, Ty);
    SDValue M = SDValue(DAG.getMachineNode(ELMO::LWI, DL, Ty, CPAV), 0);
    return M;
  } else {
    report_fatal_error("Unable to lowerConstantPool");
  }
}
MachineBasicBlock *ELMOTargetLowering::expandSelectCC(MachineInstr &MI,
                                                      MachineBasicBlock *BB,
                                                      unsigned s) const {
  const TargetInstrInfo &TII = *Subtarget->getInstrInfo();
  DebugLoc dl = MI.getDebugLoc();
  unsigned CC = (ELMOCC::CondCodes)MI.getOperand(3).getImm();

  // To "insert" a SELECT_CC instruction, we actually have to insert the
  // triangle control-flow pattern. The incoming instruction knows the
  // destination vreg to set, the condition code register to branch on, the
  // true/false values to select between, and the condition code for the branch.
  //
  // We produce the following control flow:
  //     ThisMBB
  //     |  \
  //     |  IfFalseMBB
  //     | /
  //    SinkMBB
  const BasicBlock *LLVM_BB = BB->getBasicBlock();
  MachineFunction::iterator It = ++BB->getIterator();

  MachineBasicBlock *ThisMBB = BB;
  MachineFunction *F = BB->getParent();
  MachineBasicBlock *IfFalseMBB = F->CreateMachineBasicBlock(LLVM_BB);
  MachineBasicBlock *SinkMBB = F->CreateMachineBasicBlock(LLVM_BB);
  F->insert(It, IfFalseMBB);
  F->insert(It, SinkMBB);

  // Transfer the remainder of ThisMBB and its successor edges to SinkMBB.
  SinkMBB->splice(SinkMBB->begin(), ThisMBB,
                  std::next(MachineBasicBlock::iterator(MI)), ThisMBB->end());
  SinkMBB->transferSuccessorsAndUpdatePHIs(ThisMBB);

  // Set the new successors for ThisMBB.
  ThisMBB->addSuccessor(IfFalseMBB);
  ThisMBB->addSuccessor(SinkMBB);

  BuildMI(ThisMBB, dl, TII.get(s)).addMBB(SinkMBB).addImm(CC);

  // IfFalseMBB just falls through to SinkMBB.
  IfFalseMBB->addSuccessor(SinkMBB);

  // %Result = phi [ %TrueValue, ThisMBB ], [ %FalseValue, IfFalseMBB ]
  BuildMI(*SinkMBB, SinkMBB->begin(), dl, TII.get(ELMO::PHI),
          MI.getOperand(0).getReg())
      .addReg(MI.getOperand(1).getReg())
      .addMBB(ThisMBB)
      .addReg(MI.getOperand(2).getReg())
      .addMBB(IfFalseMBB);

  MI.eraseFromParent(); // The pseudo instruction is gone now.
  return SinkMBB;
}
MachineBasicBlock *
ELMOTargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                                MachineBasicBlock *BB) const {
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("unknown select_cc");
  case ELMO::SELECT_CC_Int_ICC:
  case ELMO::SELECT_CC_FP_ICC:
    return expandSelectCC(MI, BB, ELMO::BRICC);
  case ELMO::SELECT_CC_Int_FCC:
  case ELMO::SELECT_CC_FP_FCC:
    return expandSelectCC(MI, BB, ELMO::BRFCC);
  }
}
