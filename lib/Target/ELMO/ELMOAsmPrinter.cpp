#include "ELMO.h"
#include "ELMOInstrInfo.h"
//#include "ELMOMCInstLower.h"
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
  // void printOperand(const MachineInstr *MI, int opNum, raw_ostream &OS);
  // void printMemOperand(const MachineInstr *MI, int opNum, raw_ostream &OS,
  //                      const char *Modifier = nullptr);

  // void EmitFunctionBodyStart() override;
  void EmitInstruction(const MachineInstr *MI) override;

  // static const char *getRegisterName(unsigned RegNo) {
  //   return ELMOInstPrinter::getRegisterName(RegNo);
  // }

  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       unsigned AsmVariant, const char *ExtraCode,
                       raw_ostream &O) override;
  // bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,
  //                            unsigned AsmVariant, const char *ExtraCode,
  //                            raw_ostream &O) override;
  StringRef getPassName() const override { return "ELMO ASM PRINTER"; }
};
}
// void ELMOAsmPrinter::printOperand(const MachineInstr *MI, int opNum,
//                                   raw_ostream &O) {
//   const MachineOperand &MO = MI->getOperand(opNum);
//   switch (MO.getType()) {
//   case MachineOperand::MO_Register:
//     O << ELMOInstPrinter::getRegisterName(MO.getReg());
//     break;
//
//   case MachineOperand::MO_Immediate:
//     O << MO.getImm();
//     break;
//
//   case MachineOperand::MO_MachineBasicBlock:
//     O << *MO.getMBB()->getSymbol();
//     break;
//
//   case MachineOperand::MO_GlobalAddress:
//     O << *getSymbol(MO.getGlobal());
//     break;
//
//   case MachineOperand::MO_BlockAddress: {
//     MCSymbol *BA = GetBlockAddressSymbol(MO.getBlockAddress());
//     O << BA->getName();
//     break;
//   }
//
//   case MachineOperand::MO_ExternalSymbol:
//     O << *GetExternalSymbolSymbol(MO.getSymbolName());
//     break;
//
//   case MachineOperand::MO_JumpTableIndex:
//     O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber() << '_'
//       << MO.getIndex();
//     break;
//
//   case MachineOperand::MO_ConstantPoolIndex:
//     O << MAI->getPrivateGlobalPrefix() << "CPI" << getFunctionNumber() << '_'
//       << MO.getIndex();
//     return;
//   default:
//     llvm_unreachable("<unknown operand type>");
//   }
// }

// void ELMOAsmPrinter::printMemOperand(const MachineInstr *MI, int opNum,
//                                      raw_ostream &OS, const char *Modifier)
//                                      {}
// void ELMOAsmPrinter::EmitFunctionBodyStart() {}
void ELMOAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  // ELMOMCInstLower MCInstLowering(OutContext, *this);
  MCSubtargetInfo STI = getSubtargetInfo();
  MI->dump();
  MCInst TmpInst;
  LowerELMOMachineInstrToMCInst(MI, TmpInst, *this);
  OutStreamer->EmitInstruction(TmpInst, STI);
}
bool ELMOAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     unsigned AsmVariant, const char *ExtraCode,
                                     raw_ostream &OS) {
  if (AsmVariant != 0)
    report_fatal_error("There are no defined alternate asm variants");

  // First try the generic code, which knows about modifiers like 'c' and 'n'.
  if (!AsmPrinter::PrintAsmOperand(MI, OpNo, AsmVariant, ExtraCode, OS))
    return false;

  if (!ExtraCode) {
    const MachineOperand &MO = MI->getOperand(OpNo);
    switch (MO.getType()) {
    case MachineOperand::MO_Immediate:
      OS << MO.getImm();
      return false;
    case MachineOperand::MO_Register:
      OS << "%" << ELMOInstPrinter::getRegisterName(MO.getReg());
      return false;
    default:
      break;
    }
  }
  return true;
}
// bool ELMOAsmPrinter::PrintAsmMemoryOperand(const MachineInstr *MI,
//                                            unsigned OpNo, unsigned
//                                            AsmVariant,
//                                            const char *ExtraCode,
//                                            raw_ostream &O) {}
//
extern "C" void LLVMInitializeELMOAsmPrinter() {
  RegisterAsmPrinter<ELMOAsmPrinter> X(getTheELMOTarget());
}
