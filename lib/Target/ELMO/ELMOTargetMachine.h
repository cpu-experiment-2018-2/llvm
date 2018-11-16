#ifndef LLVM_LIB_TARGET_ELMO_ELMOTARGETMACHINE_H
#define LLVM_LIB_TARGET_ELMO_ELMOTARGETMACHINE_H

#include "ELMOInstrInfo.h"
#include "ELMOSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class ELMOTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  ELMOSubtarget Subtarget;

public:
  ELMOTargetMachine(const llvm::Target &T, const llvm::Triple &TT,
                    llvm::StringRef CPU, llvm::StringRef FS,
                    const llvm::TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    llvm::CodeGenOpt::Level lv, bool JIT);
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const ELMOSubtarget *getSubtargetImpl(const llvm::Function &) const override {
    return &Subtarget;
  }
};
}
#endif
