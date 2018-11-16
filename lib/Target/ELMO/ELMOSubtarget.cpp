
#include "ELMOSubtarget.h"
#include "ELMO.h"
#include "llvm/Support/TargetRegistry.h"

#define DEBUG_TYPE "elmo-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "ELMOGenSubtargetInfo.inc"

using namespace llvm;

ELMOSubtarget &ELMOSubtarget::init(StringRef CPU, StringRef FS) {
  return *this;
}

ELMOSubtarget::ELMOSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                             const TargetMachine &TM)
    : ELMOGenSubtargetInfo(TT, CPU, FS), InstrInfo(init(CPU, FS)),
      FrameLowering(*this), TLInfo(TM, *this) {

  std::string CPUName = "generic";

  // Parse features string.
  ParseSubtargetFeatures(CPUName, FS);
}
