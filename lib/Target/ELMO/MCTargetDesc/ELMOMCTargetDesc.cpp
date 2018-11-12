
//===-- ELMOMCTargetDesc.cpp - ELMO Target Descriptions -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides ELMO specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/ELMOMCTargetDesc.h"
#include "ELMOMCAsmInfo.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "ELMOGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "ELMOGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "ELMOGenRegisterInfo.inc"

static MCAsmInfo * createELMOMCAsmInfo(const MCRegisterInfo &MRI, const Triple & TT){
    MCAsmInfo* MAI = new ELMOMCAsmInfo(TT);
    return MAI;
}
extern "C" void LLVMInitializeELMOTargetMC() {
    RegisterMCAsmInfoFn X(getTheELMOTarget(),createELMOMCAsmInfo);
}
