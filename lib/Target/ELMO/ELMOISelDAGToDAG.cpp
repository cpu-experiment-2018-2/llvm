#include "ELMOTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/WithColor.h"

using namespace llvm;

namespace {
class ELMODAGToDAGISel : public SelectionDAGISel {
  const ELMOSubtarget *Subtarget;

public:
  explicit ELMODAGToDAGISel(ELMOTargetMachine &tm) : SelectionDAGISel(tm) {
    WithColor::note() << "SelectionDAGIsel inited\n";
  };
  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<ELMOSubtarget>();
    WithColor::note() << "DAGToDAGIsel\n";
    return SelectionDAGISel::runOnMachineFunction(MF);
  }
  StringRef getPassName() const override {
    return "ELMO DAG->DAG Pattern Instruction Selection";
  }
  void Select(SDNode *N) override;

private:
#include "ELMOGenDAGISel.inc"
};
}

void ELMODAGToDAGISel::Select(SDNode *N) {
  WithColor::note() << "Selected\n";
  SelectCode(N);
}
FunctionPass *llvm::createELMOISelDag(ELMOTargetMachine &TM) {
  return new ELMODAGToDAGISel(TM);
}
