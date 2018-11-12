#include "ELMO.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheELMOTarget;
Target &llvm::getTheELMOTarget() { return TheELMOTarget; }
extern "C" void LLVMInitializeELMOTargetInfo() {
  RegisterTarget<Triple::elmo,
                 /*HasJIT=*/false>
      X(TheELMOTarget, "ELMO", "ELMO", "ELMO");
}
