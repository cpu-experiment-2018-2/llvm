#include "llvm/Target/TargetMachine.h"
#include "ELMOSubtarget.h"
namespace llvm {
class ELMOTargetMachine : public LLVMTargetMachine {
    public:
    ELMOTargetMachine(const llvm::Target &T,  const llvm::Triple &TT,
            llvm::StringRef CPU, llvm::StringRef FS, const llvm::TargetOptions &Options,
    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM ,
    llvm::CodeGenOpt::Level lv,bool JIT);
};
}
