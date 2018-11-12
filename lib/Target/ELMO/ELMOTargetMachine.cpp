#include <llvm/Support/WithColor.h>
#include "ELMO.h"
#include "ELMOTargetMachine.h"
#include "llvm/Support/TargetRegistry.h"
#include "ELMOSubtarget.h"
namespace llvm {
extern "C" void LLVMInitializeELMOTarget() {
    RegisterTargetMachine<ELMOTargetMachine> X(getTheELMOTarget());
    WithColor::note() << "TargetInit\n";
}
    StringRef layout = "";
    ELMOTargetMachine::ELMOTargetMachine(const llvm::Target &T,  const llvm::Triple &TT,
                                         llvm::StringRef CPU, llvm::StringRef FS, const llvm::TargetOptions &Options,
                                         Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM ,
                                         llvm::CodeGenOpt::Level lv,bool JIT):
            LLVMTargetMachine(T, layout, TT,CPU,FS,Options, Reloc::Static, CodeModel::Small, lv){
        WithColor::note() << "ELMOTargetmachine was called\n" ;
    }

}
