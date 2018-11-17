#ifndef LLVM_LIB_TARGET_ELMO_ELMOTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_ELMO_ELMOTARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

class MCContext;
class TargetMachine;

class ELMOELFTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  ELMOELFTargetObjectFile() : TargetLoweringObjectFileELF() {}

  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

  const MCExpr *getTTypeGlobalReference(const GlobalValue *GV,
                                        unsigned Encoding,
                                        const TargetMachine &TM,
                                        MachineModuleInfo *MMI,
                                        MCStreamer &Streamer) const override;
};
}
#endif
