#include "ELMOTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/WithColor.h"

using namespace llvm;

namespace {
class ELMODAGToDAGISel : public SelectionDAGISel {
  const ELMOSubtarget *Subtarget;

public:
  explicit ELMODAGToDAGISel(ELMOTargetMachine &tm)
      : SelectionDAGISel(tm){
            // WithColor::note() << "SelectionDAGIsel inited\n";
        };
  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<ELMOSubtarget>();
    // WithColor::note() << "DAGToDAGIsel\n";
    return SelectionDAGISel::runOnMachineFunction(MF);
  }
  StringRef getPassName() const override {
    return "ELMO DAG->DAG Pattern Instruction Selection";
  }
  void Select(SDNode *N) override;

  bool SelectAddrFI(SDValue Addr, SDValue &Base);

private:
#include "ELMOGenDAGISel.inc"
};
}

void ELMODAGToDAGISel::Select(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  MVT XLenVT = MVT::i32;

  EVT VT = Node->getValueType(0);
  // Node->dump();
  SDLoc DL(Node);
  switch (Opcode) {
  case ISD::FrameIndex: {
    SDValue Imm = CurDAG->getTargetConstant(0, DL, XLenVT);
    int FI = cast<FrameIndexSDNode>(Node)->getIndex();
    SDValue TFI = CurDAG->getTargetFrameIndex(FI, VT);
    ReplaceNode(Node, CurDAG->getMachineNode(ELMO::ADDiu, DL, VT, TFI, Imm));
    return;
  }
  }

  SelectCode(Node);

  // WithColor::note() << "Select end\n";
}
FunctionPass *llvm::createELMOISelDag(ELMOTargetMachine &TM) {
  return new ELMODAGToDAGISel(TM);
}
bool ELMODAGToDAGISel::SelectAddrFI(SDValue Addr, SDValue &Base) {
  if (auto FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
    return true;
  }
  return false;
}
