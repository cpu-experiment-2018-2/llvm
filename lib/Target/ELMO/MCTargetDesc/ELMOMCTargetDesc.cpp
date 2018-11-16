
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

#include <llvm/Support/WithColor.h>
#include "InstPrinter/ELMOInstPrinter.h"
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
    WithColor::note() << "MCAsmInfoAllocater\n";

    MCAsmInfo* MAI = new ELMOMCAsmInfo(TT);

    return MAI;
}
static MCInstrInfo* createELMOMCInstrInfo(){
 MCInstrInfo *X = new MCInstrInfo();
 InitELMOMCInstrInfo(X);
 return X;
}
static MCRegisterInfo *createELMOMCRegisterInfo(const Triple & /*TT*/) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitELMOMCRegisterInfo(X, 0, 0, 0,0 );
    return X;
}

static MCInstPrinter *createELMOMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
    return new ELMOInstPrinter(MAI, MII, MRI);
}

static MCSubtargetInfo *
createELMOMCSubtargetInfo(const Triple&TT,StringRef CPU,StringRef FS){
return createELMOMCSubtargetInfoImpl(TT,CPU,FS);
}
extern "C" void LLVMInitializeELMOTargetMC() {
    WithColor::note() << "InitTargetMC\n";

    RegisterMCAsmInfoFn X(getTheELMOTarget(),createELMOMCAsmInfo);

// Register the MC instruction info.
TargetRegistry::RegisterMCInstrInfo(getTheELMOTarget(),
        createELMOMCInstrInfo);

// Register the MC register info.
TargetRegistry::RegisterMCRegInfo(getTheELMOTarget(),
        createELMOMCRegisterInfo);
TargetRegistry::RegisterMCInstPrinter(getTheELMOTarget(), createELMOMCInstPrinter);

TargetRegistry::RegisterMCSubtargetInfo(getTheELMOTarget(),createELMOMCSubtargetInfo );
}
