
//===----------------------------------------------------------------------===//
// Registers
//===----------------------------------------------------------------------===//

class ELMOReg<bits<5> Enc, string n> : Register<n> {
  let Namespace = "ELMO";
} class ELMOGPRReg<bits<5> Enc, string n> : ELMOReg<Enc, n>;
class ELMOCond<bits<5> Enc, string n> : ELMOReg<Enc, n>;

let Namespace = "ELMO" in {
  //@ General Purpose Registers
  def ZERO : ELMOGPRReg<0, "r0">, DwarfRegNum<[ 0 ]>;
  def ONE : ELMOGPRReg<1, "r1">, DwarfRegNum<[ 1 ]>;
  def SP : ELMOGPRReg<2, "sp">, DwarfRegNum<[ 2 ]>;
  def G3 : ELMOGPRReg<3, "r3">, DwarfRegNum<[ 3 ]>;
  def G4 : ELMOGPRReg<4, "r4">, DwarfRegNum<[ 4 ]>;
  def G5 : ELMOGPRReg<5, "r5">, DwarfRegNum<[ 5 ]>;
  def G6 : ELMOGPRReg<6, "r6">, DwarfRegNum<[ 6 ]>;
  def G7 : ELMOGPRReg<7, "r7">, DwarfRegNum<[ 7 ]>;
  def G8 : ELMOGPRReg<8, "r8">, DwarfRegNum<[ 8 ]>;
  def G9 : ELMOGPRReg<9, "r9">, DwarfRegNum<[ 9 ]>;
  def G10 : ELMOGPRReg<10, "r10">, DwarfRegNum<[ 10 ]>;
  def G11 : ELMOGPRReg<11, "r11">, DwarfRegNum<[ 11 ]>;
  def G12 : ELMOGPRReg<12, "r12">, DwarfRegNum<[ 12 ]>;
  def G13 : ELMOGPRReg<13, "r13">, DwarfRegNum<[ 13 ]>;
  def G14 : ELMOGPRReg<14, "r14">, DwarfRegNum<[ 14 ]>;
  def G15 : ELMOGPRReg<15, "r15">, DwarfRegNum<[ 15 ]>;
  def G16 : ELMOGPRReg<16, "r16">, DwarfRegNum<[ 16 ]>;
  def G17 : ELMOGPRReg<17, "r17">, DwarfRegNum<[ 17 ]>;
  def G18 : ELMOGPRReg<18, "r18">, DwarfRegNum<[ 18 ]>;
  def G19 : ELMOGPRReg<19, "r19">, DwarfRegNum<[ 19 ]>;
  def G20 : ELMOGPRReg<20, "r20">, DwarfRegNum<[ 20 ]>;
  def G21 : ELMOGPRReg<21, "r21">, DwarfRegNum<[ 21 ]>;
  def G22 : ELMOGPRReg<22, "r22">, DwarfRegNum<[ 22 ]>;
  def G23 : ELMOGPRReg<23, "r23">, DwarfRegNum<[ 23 ]>;
  def G24 : ELMOGPRReg<24, "r24">, DwarfRegNum<[ 24 ]>;
  def G25 : ELMOGPRReg<25, "r25">, DwarfRegNum<[ 25 ]>;
  def G26 : ELMOGPRReg<26, "r26">, DwarfRegNum<[ 26 ]>;
  def G27 : ELMOGPRReg<27, "r27">, DwarfRegNum<[ 27 ]>;
  def G28 : ELMOGPRReg<28, "r28">, DwarfRegNum<[ 28 ]>;
  def G29 : ELMOGPRReg<29, "r29">, DwarfRegNum<[ 29 ]>;
  def G30 : ELMOGPRReg<30, "r30">, DwarfRegNum<[ 30 ]>;
  def LK : ELMOGPRReg<31, "lr">, DwarfRegNum<[ 31 ]>;
  def C : ELMOCond<32, "condreg">, DwarfRegNum<[ 32 ]>;
}

//===----------------------------------------------------------------------===//
// Register Classes
//===----------------------------------------------------------------------===//

def ELMOGRRegs : RegisterClass<"ELMO", [ i32 ], 32,
                               (add ZERO, ONE, SP, (sequence "G%u", 3, 30), LK)>;
def ELMOGRFRegs
    : RegisterClass<"ELMO", [ f32 ], 32,
                    (add ZERO, ONE, SP, (sequence "G%u", 3, 30), LK)>;

def COND : RegisterClass<"ELMO", [ i32 ], 32, (add C)>;
