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



include "ELMOScheduling.td"
include "ELMORegisterInfo.td"
include "ELMOInstrInfo.td"
include "ELMOCallingConv.td"

class Proc1<string Name,  list<SubtargetFeature> Features>
 : Processor<Name, ELMOItineraries, Features> ;
class Proc2<string Name,  list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features> ;

/* class Proc<string Name,  list<SubtargetFeature> Features> */
/*  : Processor<Name, NoItineraries, Features>; */


def : Proc2<"generic", []>;
def : Proc1<"sched", []>;
def : ProcessorModel<"schedmodel", ELMOSchedModel, []>; 

def ELMOInstPrinter : AsmWriter {
    string AsmWriterClassName = "InstPrinter";
    bit isMcAsmWriter = 1;
}
def ELMOInstrInfo : InstrInfo;
def ELMO : Target {
  // Pull in Instruction Info:
  let InstructionSet = ELMOInstrInfo;
  let AssemblyWriters = [ELMOInstPrinter];
  let AllowRegisterRenaming = 1;
}
