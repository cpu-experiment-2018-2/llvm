#ifndef LLVM_LIB_TARGET_ELMO_ELMOSUBTARGET_H
#define LLVM_LIB_TARGET_ELMO_ELMOSUBTARGET_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "ELMOGenSubtargetInfo.inc"

namespace llvm {
class StringRef;
class ELMOSubtarget : public ELMOGenSubtargetInfo {
  virtual void anchor(){};

public:
  ELMOSubtarget(const Triple &TT, StringRef CPU,
                               StringRef FS);
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
};
}

#endif