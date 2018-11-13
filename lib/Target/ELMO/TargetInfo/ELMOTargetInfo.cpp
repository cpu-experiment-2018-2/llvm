#include "ELMO.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheELMOTarget() {
    static Target TheELMOTarget;
    return TheELMOTarget; }
extern "C" void LLVMInitializeELMOTargetInfo() {
  RegisterTarget<Triple::elmo,
                 /*HasJIT=*/false>
      X(getTheELMOTarget(), "ELMO", "ELMO", "ELMO");
}
