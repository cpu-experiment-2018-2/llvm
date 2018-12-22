//=-LanaiSchedule.td - Lanai Scheduling Definitions --*- tablegen -*-=========//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

def ALU_FU  : FuncUnit;
def FPU_FU : FuncUnit;

def IICLoad   : InstrItinClass;
def IICStore  : InstrItinClass;
def IIBranch  : InstrItinClass;
def IIAlu  : InstrItinClass;
def IIFpu  : InstrItinClass;



/* def ELMOItineraries : ProcessorItineraries<[ALU_FU,ALU_FU,FPU_FU,FPU_FU],[],[ */
/*   InstrItinData<IICLoad,   [InstrStage<3, [FPU_FU]>]>, */
/*   InstrItinData<IICStore,   [InstrStage<1, [FPU_FU]>]>, */
/*   InstrItinData<IIBranch, [InstrStage<3, [FPU_FU]>]>, */
/*   InstrItinData<IIAlu, [InstrStage<3, [FPU_FU]>]>, */
/*   InstrItinData<IIFpu,  [InstrStage<5, [FPU_FU]>]> */
/* ]>; */
/*  */
def ELMOItineraries : ProcessorItineraries<[ALU_FU,FPU_FU],[],[
  InstrItinData<IICLoad,   [InstrStage<1, [FPU_FU]>], [4,1]>,
  InstrItinData<IICStore,   [InstrStage<1, [FPU_FU]>], [0,1]>,
  InstrItinData<IIBranch, [InstrStage<1, [FPU_FU]>],[2,1]>, 
  InstrItinData<IIAlu, [InstrStage<1, [FPU_FU]>], [2,1]>,
  InstrItinData<IIFpu,  [InstrStage<1, [FPU_FU]>], [1,4]>
]>;

/*  */
def ELMOSchedModel : SchedMachineModel {
    let MicroOpBufferSize = 0;
    let IssueWidth = 2;
    let LoadLatency = 4;
    let MispredictPenalty = 0;
    let CompleteModel = 0;
}

def ALU : ProcResource<2> { let BufferSize = 0; }
def FPU : ProcResource<2> { let BufferSize = 0; }

def WriteALU4  : SchedWrite;
def WriteALU3  : SchedWrite;
def WriteALU2  : SchedWrite;
def WriteALU1  : SchedWrite;
def WriteALU0  : SchedWrite;

def WriteFPU4  : SchedWrite;
def WriteFPU3  : SchedWrite;
def WriteFPU6  : SchedWrite;



let SchedModel = ELMOSchedModel in {
  def : WriteRes<WriteALU4, [ALU]>   { let Latency = 5; }
  def : WriteRes<WriteALU3, [ALU]>   { let Latency = 4; }
  def : WriteRes<WriteALU2, [ALU]>   { let Latency = 3; }
  def : WriteRes<WriteALU1, [ALU]>   { let Latency = 2; }
  def : WriteRes<WriteALU0, [ALU]>   { let Latency = 1; }
  def : WriteRes<WriteFPU3, [FPU]>   { let Latency = 4; }
  def : WriteRes<WriteFPU4, [FPU]>   { let Latency = 5; }
  def : WriteRes<WriteFPU6, [FPU]>   { let Latency = 7; }
  def : InstRW<[WriteALU2],(instrs COPY)>;
}
