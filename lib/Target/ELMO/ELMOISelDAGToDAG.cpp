#include "ELMOTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

using namespace llvm;

namespace {
class ELMODAGToDAGISel : public SelectionDAGISel {
  const ELMOSubtarget *Subtarget;

public:
  explicit ELMODAGToDAGISel(ELMOTargetMachine &tm) : SelectionDAGISel(tm){};
  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<ELMOSubtarget>();
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

void ELMODAGToDAGISel::Select(SDNode *N) { SelectCode(N); }
