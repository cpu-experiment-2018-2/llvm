#ifndef LLVM_LIB_TARGET_ELMO_ELMOSUBTARGET_H
#define LLVM_LIB_TARGET_ELMO_ELMOSUBTARGET_H

#include "ELMOFrameLowering.h"
#include "ELMOISelLowering.h"
#include "ELMOInstrInfo.h"
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
  ELMOInstrInfo InstrInfo;
  ELMOTargetLowering TLInfo;
  SelectionDAGTargetInfo TSInfo;
  ELMOFrameLowering FrameLowering;

public:
  ELMOSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                const TargetMachine &TM);
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);
  ELMOSubtarget &init(StringRef CPU, StringRef FS);
  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const ELMORegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const ELMOTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }
  const ELMOInstrInfo *getInstrInfo() const override { return &InstrInfo; }
};
}

#endif
