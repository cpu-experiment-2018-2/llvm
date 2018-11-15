

#include "ELMO.h"
#include "ELMOInstPrinter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "asm-printer"
using namespace llvm;
#define GET_INSTRUCTION_NAME

#include "ELMOGenAsmWriter.inc"
void ELMOInstPrinter::printOperand(const MCInst *MI, int opNum,
                                   raw_ostream &OS) {}
void ELMOInstPrinter::printMemOperand(const MCInst *MI, int opNum,
                                      raw_ostream &OS, const char *Modifier) {}
void ELMOInstPrinter::printCCOperand(const MCInst *MI, int opNum,
                                     raw_ostream &OS) {}
bool ELMOInstPrinter::printGetPCX(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &OS) {}

void ELMOInstPrinter::printRegName(llvm::raw_ostream &OS,
                                   unsigned RegNo) const {
  OS << "%r" << RegNo;
}
void ELMOInstPrinter::printInst(const llvm::MCInst *MI, llvm::raw_ostream &OS,
                                llvm::StringRef Annot,
                                const llvm::MCSubtargetInfo &STI) {}