
#include "ELMOTargetObjectFile.h"
#include "llvm/BinaryFormat/Dwarf.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/TargetLowering.h"
using namespace llvm;

void ELMOELFTargetObjectFile::Initialize(MCContext &Ctx,
                                         const TargetMachine &TM) {
  TargetLoweringObjectFile::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);
}
const MCExpr *ELMOELFTargetObjectFile::getTTypeGlobalReference(
    const llvm::GlobalValue *GV, unsigned Encoding,
    const llvm::TargetMachine &TM, llvm::MachineModuleInfo *MMI,
    llvm::MCStreamer &Streamer) const {
  return TargetLoweringObjectFile::getTTypeGlobalReference(GV, Encoding, TM,
                                                           MMI, Streamer);
}