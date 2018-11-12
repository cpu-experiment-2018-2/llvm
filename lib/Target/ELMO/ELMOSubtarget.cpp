#include "ELMOSubtarget.h"
#include "ELMO.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_SUBTARGETINFO_TARGET_DESC
//#define GET_SUBTARGETINFO_CTOR
//#include "ELMOGenSubtargetInfo.inc"

using namespace llvm;


ELMOSubtarget::ELMOSubtarget(const Triple &TT, StringRef CPU,
                             StringRef FS)
    : ELMOGenSubtargetInfo(TT, CPU, FS) {
  std::string CPUName = "generic";

  // Parse features string.
  ParseSubtargetFeatures(CPUName, FS);
}
