// #include "ELMOMCInstLower.h"
//
// #include "llvm/ADT/SmallString.h"
// #include "llvm/CodeGen/AsmPrinter.h"
// #include "llvm/CodeGen/MachineBasicBlock.h"
// #include "llvm/CodeGen/MachineInstr.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/MC/MCAsmInfo.h"
// #include "llvm/MC/MCContext.h"
// #include "llvm/MC/MCExpr.h"
// #include "llvm/MC/MCInst.h"
// #include "llvm/Support/ErrorHandling.h"
// #include "llvm/Support/raw_ostream.h"
//
// using namespace llvm;
//
// MCSymbol *
// ELMOMCInstLower::GetGlobalAddressSymbol(const MachineOperand &MO) const {
//   return Printer.getSymbol(MO.getGlobal());
// }
//
// MCSymbol *
// ELMOMCInstLower::GetBlockAddressSymbol(const MachineOperand &MO) const {
//   return Printer.GetBlockAddressSymbol(MO.getBlockAddress());
// }
//
// MCSymbol *
// ELMOMCInstLower::GetExternalSymbolSymbol(const MachineOperand &MO) const {
//   return Printer.GetExternalSymbolSymbol(MO.getSymbolName());
// }
//
// MCSymbol *ELMOMCInstLower::GetJumpTableSymbol(const MachineOperand &MO) const
// {
//   SmallString<256> Name;
//   raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "JTI"
//                             << Printer.getFunctionNumber() << '_'
//                             << MO.getIndex();
//   // Create a symbol for the name.
//   return Ctx.getOrCreateSymbol(Name.str());
// }
//
// MCSymbol *
// ELMOMCInstLower::GetConstantPoolIndexSymbol(const MachineOperand &MO) const {
//   SmallString<256> Name;
//   raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "CPI"
//                             << Printer.getFunctionNumber() << '_'
//                             << MO.getIndex();
//   // Create a symbol for the name.
//   return Ctx.getOrCreateSymbol(Name.str());
// }
//
// MCOperand ELMOMCInstLower::LowerSymbolOperand(const MachineOperand &MO,
//                                               MCSymbol *Sym) const {
//   // ELMOMCExpr::VariantKind Kind;
//
//   // switch (MO.getTargetFlags()) {
//   // case ELMOII::MO_NO_FLAG:
//   //   Kind = ELMOMCExpr::VK_ELMO_None;
//   //   break;
//   // case ELMOII::MO_ABS_HI:
//   //   Kind = ELMOMCExpr::VK_ELMO_ABS_HI;
//   //   break;
//   // case ELMOII::MO_ABS_LO:
//   //   Kind = ELMOMCExpr::VK_ELMO_ABS_LO;
//   //   break;
//   // default:
//   //   llvm_unreachable("Unknown target flag on GV operand");
//   // }
//
//   const MCExpr *Expr =
//       MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);
//   // if (!MO.isJTI() && MO.getOffset())
//   //   Expr = MCBinaryExpr::createAdd(
//   //       Expr, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
//   // Expr = ELMOMCExpr::create(Kind, Expr, Ctx);
//   return MCOperand::createExpr(Expr);
// }
//
// void ELMOMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
//   OutMI.setOpcode(MI->getOpcode());
//
//   for (unsigned I = 0, E = MI->getNumOperands(); I != E; ++I) {
//     const MachineOperand &MO = MI->getOperand(I);
//
//     MCOperand MCOp;
//     if (MCOp.isValid()) {
//       OutMI.addOperand(MCOp);
//     }
//   }
//   //   switch (MO.getType()) {
//   //   case MachineOperand::MO_Register:
//   //     // Ignore all implicit register operands.
//   //     if (MO.isImplicit())
//   //       continue;
//   //     MCOp = MCOperand::createReg(MO.getReg());
//   //     break;
//   //   case MachineOperand::MO_Immediate:
//   //     MCOp = MCOperand::createImm(MO.getImm());
//   //     break;
//   //   // case MachineOperand::MO_MachineBasicBlock:
//   //   //   MCOp = MCOperand::createExpr(
//   //   //       MCSymbolRefExpr::create(MO.getMBB()->getSymbol(), Ctx));
//   //   //   break;
//   //   case MachineOperand::MO_RegisterMask:
//   //     continue;
//   //   case MachineOperand::MO_GlobalAddress:
//   //     MCOp = LowerSymbolOperand(MO, GetGlobalAddressSymbol(MO));
//   //     break;
//   //   case MachineOperand::MO_BlockAddress:
//   //     MCOp = LowerSymbolOperand(MO, GetBlockAddressSymbol(MO));
//   //     break;
//   //   case MachineOperand::MO_ExternalSymbol:
//   //     MCOp = LowerSymbolOperand(MO, GetExternalSymbolSymbol(MO));
//   //     break;
//   //   case MachineOperand::MO_JumpTableIndex:
//   //     MCOp = LowerSymbolOperand(MO, GetJumpTableSymbol(MO));
//   //     break;
//   //   case MachineOperand::MO_ConstantPoolIndex:
//   //     MCOp = LowerSymbolOperand(MO, GetConstantPoolIndexSymbol(MO));
//   //     break;
//   //   default:
//   //     MI->print(errs());
//   //     llvm_unreachable("unknown operand type");
//   //   }
//   //
//   //   OutMI.addOperand(MCOp);
//   // }
// }
//===-- Cpu0MCInstLower.cpp - Convert Cpu0 MachineInstr to MCInst ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower Cpu0 MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "ELMOMCInstLower.h"
// #include "MCTargetDesc/Cpu0BaseInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

// ELMOMCInstLower::ELMOMCInstLower(ELMOAsmPrinter &asmprinter)
//   : AsmPrinter(asmprinter) {}

// void ELMOMCInstLower::Initialize(MCContext* C,AsmPrinter &AP):  {
//   Ctx = C;
// }

static void CreateMCInst(MCInst &Inst, unsigned Opc, const MCOperand &Opnd0,
                         const MCOperand &Opnd1,
                         const MCOperand &Opnd2 = MCOperand()) {
  Inst.setOpcode(Opc);
  Inst.addOperand(Opnd0);
  Inst.addOperand(Opnd1);
  if (Opnd2.isValid())
    Inst.addOperand(Opnd2);
}

//@LowerOperand {
MCOperand ELMOMCInstLower::LowerOperand(const MachineOperand &MO,
                                        unsigned offset) const {

  switch (MO.getType()) {
  //@2
  default:
    MO.dump();
    llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm() + offset);
  case MachineOperand::MO_RegisterMask:
    break;
  case MachineOperand::MO_MachineBasicBlock:
    return MCOperand::createExpr(
        MCSymbolRefExpr::create(MO.getMBB()->getSymbol(), Ctx));
  }

  return MCOperand();
}

void ELMOMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO, 0);

    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}
