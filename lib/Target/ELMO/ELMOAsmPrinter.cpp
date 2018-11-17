#include "ELMO.h"
#include "ELMOInstrInfo.h"
#include "ELMOTargetMachine.h"
#include "InstPrinter/ELMOInstPrinter.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {
class ELMOAsmPrinter : public AsmPrinter {
public:
  explicit ELMOAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}
  void printOperand(const MachineInstr *MI, int opNum, raw_ostream &OS);
  void printMemOperand(const MachineInstr *MI, int opNum, raw_ostream &OS,
                       const char *Modifier = nullptr);

  void EmitFunctionBodyStart() override;
  void EmitInstruction(const MachineInstr *MI) override;

  static const char *getRegisterName(unsigned RegNo) {
    return ELMOInstPrinter::getRegisterName(RegNo);
  }

  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       unsigned AsmVariant, const char *ExtraCode,
                       raw_ostream &O) override;
  bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,
                             unsigned AsmVariant, const char *ExtraCode,
                             raw_ostream &O) override;
};
}
void ELMOAsmPrinter::printOperand(const MachineInstr *MI, int opNum,
                                  raw_ostream &OS) {}
void ELMOAsmPrinter::printMemOperand(const MachineInstr *MI, int opNum,
                                     raw_ostream &OS, const char *Modifier) {}
void ELMOAsmPrinter::EmitFunctionBodyStart() {}
void ELMOAsmPrinter::EmitInstruction(const MachineInstr *MI) {}
bool ELMOAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     unsigned AsmVariant, const char *ExtraCode,
                                     raw_ostream &O) {}
bool ELMOAsmPrinter::PrintAsmMemoryOperand(const MachineInstr *MI,
                                           unsigned OpNo, unsigned AsmVariant,
                                           const char *ExtraCode,
                                           raw_ostream &O) {}

extern "C" void LLVMInitializeELMOAsmPrinter() {
  RegisterAsmPrinter<ELMOAsmPrinter> X(getTheELMOTarget());
}