//===-- Sample.td - Describe the Sample Target Machine ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This is the top level entry point for the Sample target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;
def : Proc<"generic", []>;
include "ELMORegisterInfo.td"
include "ELMOInstrInfo.td"
include "ELMOCallingConv.td"

def ELMOInstrInfo : InstrInfo;
def ELMO : Target {
  // Pull in Instruction Info:
  let InstructionSet = ELMOInstrInfo;
  let AssemblyParsers  = [];
  let AssemblyWriters = [];
  let AllowRegisterRenaming = 1;
}


