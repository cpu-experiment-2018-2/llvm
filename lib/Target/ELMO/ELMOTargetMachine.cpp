#include "ELMO.h"
#include "ELMOSubtarget.h"
#include "ELMOTargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/Support/WithColor.h>

using namespace llvm;
extern "C" void LLVMInitializeELMOTarget() {
  RegisterTargetMachine<ELMOTargetMachine> X(getTheELMOTarget());
  WithColor::note() << "TargetInit\n";
}
StringRef layout = "";

ELMOTargetMachine::ELMOTargetMachine(const llvm::Target &T,
                                     const llvm::Triple &TT,
                                     llvm::StringRef CPU, llvm::StringRef FS,
                                     const llvm::TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     llvm::CodeGenOpt::Level lv, bool JIT)
    : LLVMTargetMachine(T, layout, TT, CPU, FS, Options, Reloc::Static,
                        CodeModel::Small, lv) {
  WithColor::note() << "ELMOTargetmachine was called\n";
  initAsmInfo();
}

namespace {
class ELMOPassConfig : public TargetPassConfig {
public:
  ELMOPassConfig(ELMOTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  ELMOTargetMachine &getELMOTargetMachine() const {
    return getTM<ELMOTargetMachine>();
  }

  void addIRPasses() override;

  bool addInstSelector() override;

  void addPreEmitPass() override;
};
}
TargetPassConfig *
ELMOTargetMachine::createPassConfig(llvm::PassManagerBase &PM) {
  return new ELMOPassConfig(*this, PM);
}
void ELMOPassConfig::addIRPasses() {
  // addPass(createAtomicExpandPass());
  TargetPassConfig::addIRPasses();
}
bool ELMOPassConfig::addInstSelector() {
  // addPass(createELMOIselDag(getELMOTargetMachine()));
  return false;
}
void ELMOPassConfig::addPreEmitPass() {}
