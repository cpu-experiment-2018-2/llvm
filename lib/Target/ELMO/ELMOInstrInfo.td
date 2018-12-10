include "ELMOInstrFormats.td"

def IICLoad   : InstrItinClass;
def IICStore  : InstrItinClass;
def IIBranch  : InstrItinClass;
def IIAlu  : InstrItinClass;

def SDT_ELMOCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>,
                                            SDTCisVT<1, i32>]>;
def SDT_ELMOCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32>]>;
def CallSeqStart     : SDNode<"ISD::CALLSEQ_START", SDT_ELMOCallSeqStart,
                              [SDNPHasChain, SDNPOutGlue]>;
def CallSeqEnd       : SDNode<"ISD::CALLSEQ_END", SDT_ELMOCallSeqEnd,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;




// sparcのcmpiccに対応
def SDT_ELMOSetFlagInt      : SDTypeProfile<0,  2, [SDTCisInt<0>, SDTCisSameAs<0, 1>]>;
def ELMOSetFlagInt     : SDNode<"ELMOISD::SET_FLAGI", SDT_ELMOSetFlagInt,
                              [SDNPOutGlue]>;

// sparcのcmpfccに対応
def SDT_ELMOSetFlagFP  :
SDTypeProfile<0, 2, [SDTCisFP<0>, SDTCisSameAs<0, 1>]>;
def ELMOSetFlagFP     : SDNode<"ELMOISD::SET_FLAGF", SDT_ELMOSetFlagFP,
                              [SDNPOutGlue]>;



def SDT_ELMOSelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                SDTCisSameAs<1, 2>,
                                                SDTCisVT<3, i32>
                                                 ]>;
def ELMOselecticc : SDNode<"ELMOISD::SELECT_ICC", SDT_ELMOSelectCC, [SDNPInGlue]>;
def ELMOselectfcc : SDNode<"ELMOISD::SELECT_FCC", SDT_ELMOSelectCC, [SDNPInGlue]>;

let Uses = [C], usesCustomInserter = 1 in {
  def SELECT_CC_Int_ICC
   : Pseudo<(outs ELMOGRRegs:$dst), (ins ELMOGRRegs:$T, ELMOGRRegs:$F, i32imm:$Cond),
            "# SELECT_CC_Int_ICC PSEUDO!",
            [(set i32:$dst, (ELMOselecticc i32:$T, i32:$F, imm:$Cond))],IIAlu>;
  def SELECT_CC_FP_ICC
   : Pseudo<(outs ELMOGRFRegs:$dst), (ins ELMOGRFRegs:$T, ELMOGRFRegs:$F, i32imm:$Cond),
            "# SELECT_CC_FP_ICC PSEUDO!",
            [(set f32:$dst, (ELMOselecticc f32:$T, f32:$F, imm:$Cond))], IIAlu>;
  def SELECT_CC_Int_FCC
   : Pseudo<(outs ELMOGRRegs:$dst), (ins ELMOGRRegs:$T, ELMOGRRegs:$F, i32imm:$Cond),
            "# SELECT_CC_Int_ICC PSEUDO!",
            [(set i32:$dst, (ELMOselectfcc i32:$T, i32:$F, imm:$Cond))],IIAlu>;
  def SELECT_CC_FP_FCC
   : Pseudo<(outs ELMOGRFRegs:$dst), (ins ELMOGRFRegs:$T, ELMOGRFRegs:$F, i32imm:$Cond),
            "# SELECT_CC_FP_ICC PSEUDO!",
            [(set f32:$dst, (ELMOselectfcc f32:$T, f32:$F, imm:$Cond))], IIAlu>;

}

def SDT_ELMORet : SDTypeProfile < 0, 1, [SDTCisInt<0>]>;
def SDT_ELMOBrCC : SDTypeProfile<0,  2, [SDTCisVT<0, OtherVT>,
                                                  SDTCisVT<1, i32>]>;
def SDT_ELMOJmpLink      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;
def SDT_ELMOCall : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;

def ELMOJmpLink : SDNode<"ELMOISD::JmpLink",SDT_ELMOJmpLink,
                         [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,
                                                   SDNPVariadic]>;

def ELMORet : SDNode < "ELMOISD::Ret", SDT_ELMORet,
    [SDNPHasChain, SDNPOptInGlue,SDNPVariadic]>; 

def ELMORet2 : SDNode < "ELMOISD::Ret", SDTNone,
    [SDNPHasChain, SDNPOptInGlue,SDNPVariadic]>; 

def ELMObricc       : SDNode<"ELMOISD::BRICC", SDT_ELMOBrCC,
                              [SDNPHasChain, SDNPInGlue]>;
def ELMObrfcc        : SDNode<"ELMOISD::BRFCC", SDT_ELMOBrCC,
                              [SDNPHasChain, SDNPInGlue]>;


def ELMOCall : SDNode < "ELMOISD::Call", SDT_ELMOCall,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                               SDNPVariadic]>;


def CCOp : Operand<i32> {
    let PrintMethod = "printCCOperand";
}

def Im: Operand<i32>;
def Imf: Operand<f32>;


def imm32SExt16  : Operand<i32>, ImmLeaf<i32, [{
      // imm32SExt16 predicate - True if the i32 immediate fits in a 16-bit
        // sign extended field.  Used by instructions like 'addi'.
     return (int32_t)Imm == (short)Imm;
}]>;

def imm32plus  : Operand<i32>, ImmLeaf<i32, [{return Imm>=0;}]>;
def imm32  : Operand<i32>, ImmLeaf<i32, [{return true;}]>;
def imm32f  : Operand<i32>, ImmLeaf<f32, [{return true;}]>;


def brtarget    : Operand<OtherVT> {
  let EncoderMethod = "getBranchTargetOpValue";
  let OperandType = "OPERAND_PCREL";
  let DecoderMethod = "DecodeBranchTarget";
}





class ArithLogicX<bits<6> op, string instr_asm, SDNode OpNode,
                  RegisterClass InR,RegisterClass OutR> :
  FL<op, (outs OutR:$ra), (ins InR:$rb),
     !strconcat(instr_asm, "\t$ra, $rb"),
     [(set OutR:$ra, (OpNode InR:$rb))], IIAlu> {
  let isReMaterializable = 1;
}
class ArithLogicI<bits<6> op, string instr_asm, SDNode OpNode,
                  Operand Od, PatLeaf imm_type, RegisterClass RC> :
  FL<op, (outs RC:$ra), (ins RC:$rb, Od:$imm16),
     !strconcat(instr_asm, "\t$ra, $rb, $imm16"),
     [(set RC:$ra, (OpNode RC:$rb, imm_type:$imm16))], IIAlu> {
  let isReMaterializable = 1;
}

class ArithLogicR<bits<6> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>:
  FA<op, (outs RC:$ra), (ins RC:$rb, RC:$rc),
     !strconcat(instr_asm, "\t$ra, $rb, $rc"),
     [(set RC:$ra, (OpNode RC:$rb, RC:$rc))], itin> {
  let isCommutable = isComm;	// e.g. add rb rc =  add rc rb
  let isReMaterializable = 1;
}




def simm16      : Operand<i32> {
  let DecoderMethod= "DecodeSimm16";
}
def immSExt16  : PatLeaf<(imm), [{ return isInt<16>(N->getSExtValue());}]>;
def ADDiu   : ArithLogicI<0x09, "addi", add, simm16, immSExt16, ELMOGRRegs>;
def SUBiu   : ArithLogicI<0x09, "subi", sub, simm16, immSExt16, ELMOGRRegs>;
def ShiftR  : ArithLogicI<0x09, "srawi", shl, simm16, immSExt16, ELMOGRRegs>;
def ShifrLA  : ArithLogicI<0x09, "slawi", sra , simm16, immSExt16, ELMOGRRegs>;
def ShifrLL  : ArithLogicI<0x09, "slawi", srl , simm16, immSExt16, ELMOGRRegs>;
def FTOI  : ArithLogicX<0x09, "ftoi", fp_to_sint, ELMOGRFRegs, ELMOGRRegs>;
def ITOF  : ArithLogicX<0x09, "itof", sint_to_fp, ELMOGRRegs, ELMOGRFRegs>;
def SQRT  : ArithLogicX<0x09, "fsqrt", fsqrt, ELMOGRFRegs, ELMOGRFRegs>;






def mem : Operand<i32> {
      let PrintMethod = "printMemOperand";
      let MIOperandInfo = (ops ELMOGRRegs, i16imm);
      let EncoderMethod = "getMemEncoding";
}


def ADD  : ArithLogicR<0b000000, "add", add, IIAlu, ELMOGRRegs, 1>;
def SUB  : ArithLogicR<0b000001, "sub", sub, IIAlu, ELMOGRRegs, 0>;
def AND  : ArithLogicR<0b000000, "and", and, IIAlu, ELMOGRRegs, 1>;
def OR  : ArithLogicR<0b000001, "or", or , IIAlu, ELMOGRRegs, 1>;


def FADD  : ArithLogicR<0b000000, "fadd", fadd, IIAlu, ELMOGRFRegs, 1>;
def FSUB  : ArithLogicR<0b000001, "fsub", fsub, IIAlu, ELMOGRFRegs, 0>;
def FMUL  : ArithLogicR<0b000000, "fmul", fmul, IIAlu, ELMOGRFRegs, 1>;
def FDIV  : ArithLogicR<0b000001, "fdiv", fdiv, IIAlu, ELMOGRFRegs, 0>;


/* def BLR  : RetInst<0b000010, "blr">;  */
let mayLoad  = 1, hasSideEffects = 1 in
{
def LOAD  : FL<0b000011, (outs ELMOGRRegs:$rd), (ins ELMOGRRegs:$rs1,simm16:$imm16),
    "load $rd, $rs1, $imm16", [], IICLoad
>;  

def LOADF  : FL<0b000011, (outs ELMOGRFRegs:$rd), (ins ELMOGRRegs:$rs1,simm16:$imm16),
    "load $rd, $rs1, $imm16", [], IICLoad
>; 
}
let mayStore = 1 in
{
def STOREF  : FL<0b000011, (outs ), (ins ELMOGRFRegs:$rd, ELMOGRRegs:$rs1,simm16:$imm16),
    "store $rd, $rs1, $imm16", [], IICStore
>;  

def STORE  : FL<0b000011, (outs ), (ins ELMOGRRegs:$rd, ELMOGRRegs:$rs1,simm16:$imm16),
    "store $rd, $rs1, $imm16", [], IICStore
>;  
}




/* class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern> */
/*    : InstELMO<outs, ins, asmstr, pattern> { */
/*   let isCodeGenOnly = 1; */
/*   let isPseudo = 1; */
/* } */




// Unconditional branch, such as JMP
class UncondBranch<bits<6> op, string instr_asm>:
  FJ<op, (outs), (ins brtarget:$addr),
             !strconcat(instr_asm, "\t$addr"), [(br bb:$addr)], IIBranch> {
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let hasDelaySlot = 0;
  let DecoderMethod = "DecodeJumpRelativeTarget";
}
class JumpFR<bits<6> op, string instr_asm, RegisterClass RC>:
  FL<op, (outs), (ins RC:$ra),
     !strconcat(instr_asm, "\t$ra"), [(brind RC:$ra)], IIBranch> {
  let rb = 0;
  let imm16 = 0;
}


def JMP : UncondBranch<0b000000, "jump">;

/* class CBranch<bits<6> op, string instr_asm, RegisterClass RC, */
/*                    list<Register> UseRegs>: */
/*   FJ<op, (outs), (ins RC:$ra, brtarget:$addr), */
/*              !strconcat(instr_asm, "\t$ra, $addr"), */
/*              [], IIBranch> { */
/*   let isBranch = 1; */
/*   let isTerminator = 1; */
/*   let hasDelaySlot = 0; */
/*   let DecoderMethod = "DecodeBranchTarget"; */
/* } */
/*  */
/* class CmpInstr<bits<6> op, string instr_asm,  */
/*                InstrItinClass itin, RegisterClass RC, RegisterClass RD,  */
/*                bit isComm = 0>: */
/*   FA<op, (outs RD:$rc), (ins RC:$ra, RC:$rb), */
/*      !strconcat(instr_asm, "\t$rc, $ra, $rb"), [], itin> { */
/*   let rc = 0; */
/*   let isCommutable = isComm; */
/* } */
/*  */
/* def JEQ     : CBranch<0x10, "jeq", COND, [G29]>; */
/* def JNE     : CBranch<0x11, "jne", COND, [G29]>; */
/* def JLT     : CBranch<0x12, "jlt", COND, [G29]>; */
/* def JGT     : CBranch<0x13, "jgt", COND, [G29]>; */
/* def JLE     : CBranch<0x14, "jle", COND, [G29]>; */
/* def JGE     : CBranch<0x15, "jge", COND, [G29]>; */
/* def CMP     : CmpInstr<0x16, "cmpd",IIBranch, COND,ELMOGRRegs, 0>; */
/*  */
/*  */
/*  */
/* // brcond patterns */
/* multiclass BrcondPats<RegisterClass RC, Instruction JEQOp, Instruction JNEOp,  */
/* Instruction JLTOp, Instruction JGTOp, Instruction JLEOp, Instruction JGEOp,  */
/* Instruction CMPOp, Register ZEROReg> { */
/* def : Pat<(brcond (i32 (seteq RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setueq RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setne RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setune RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setlt RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setult RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setgt RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setugt RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setle RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JLEOp (CMPOp RC:$rhs, RC:$lhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setule RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JLEOp (CMPOp RC:$rhs, RC:$lhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setge RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond (i32 (setuge RC:$lhs, RC:$rhs)), bb:$dst), */
/*           (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>; */
/* def : Pat<(brcond RC:$cond, bb:$dst), */
/*           (JNEOp (CMPOp RC:$cond, ZEROReg), bb:$dst)>; */
/* def : Pat<(setcc RC:$lhs, RC:$rhs, SETEQ), */
/*             (CMP RC:$lhs, RC:$rhs)>; */
/*  */
/* } */
/*  */
/* defm : BrcondPats<ELMOGRRegs, JEQ, JNE, JLT, JGT, JLE, JGE, CMP, ZERO>; */

/* let isReturn = 1,isCodeGenOnly = 1, hasCtrlDep=1, hasCtrlDep = 1 in  */
/* def BLR : JumpFR<0x000010, "blr", ELMOGRRegs> ; */



let isBranch = 1, isBarrier = 1, isTerminator = 1, hasDelaySlot = 1 , Uses = [C] in {
    def BRICC : FJ<0b000111,(outs), (ins brtarget:$addr, CCOp:$DDDI),
                      "b$DDDI\t$addr",
                      [(ELMObricc bb:$addr, imm:$DDDI)],
	IIBranch
>;
    def BRFCC : FJ<0b000111,(outs), (ins brtarget:$addr, CCOp:$DDDI),
                      "b$DDDI\t$addr",
                      [(ELMObrfcc bb:$addr, imm:$DDDI)],
	IIBranch
>;

}



let isReturn = 1,isBranch = 1, isTerminator=1, isBarrier = 1 in  
{
def BLR : FI < 0b000000, (outs), (ins ELMOGRRegs:$rc), "blr",  [(ELMORet ELMOGRRegs:$rc)] , IIBranch> ;

def PseudoBLR : FI < 0b000000, (outs), (ins), "blr",  [(ELMORet2)] , IIBranch> ;
}
/* multiclass SetltPats<RegisterClass RC> { */
/*   def : Pat<(setlt RC:$lhs, RC:$rhs), */
/*             (CMP RC:$lhs, RC:$rhs)>; */
/* //          (XORi (ANDi (SHR (CMP RC:$lhs, RC:$rhs), (LUi 0x8000), 31), 1), 1)>; */
/*   def : Pat<(setult RC:$lhs, RC:$rhs), */
/*             (CMP RC:$lhs, RC:$rhs)>; */
/* } */
/*  */
/* defm : SetltPats<ELMOGRRegs>; */
/*  */
/* multiclass SeteqPats<RegisterClass RC> { */
/*   def : Pat<(seteq RC:$lhs, RC:$rhs), */
/*             (CMP RC:$lhs, RC:$rhs)>; */
/*   def : Pat<(setcc RC:$lhs, RC:$rhs, SETEQ), */
/*             (CMP RC:$lhs, RC:$rhs)>; */
/* } */
/*  */
/* defm : SeteqPats<ELMOGRRegs>; */
/*  */

/* let usesCustomInserter = 1 in */
/* class SelectCC_rrirr<RegisterClass valty, RegisterClass cmpty,string asmstr> */
/*     : Pseudo<(outs valty:$dst), */
/*              (ins cmpty:$lhs, cmpty:$rhs, */
/*               valty:$truev, valty:$falsev, CCOp:$imm), */
/*               asmstr, */
/*              [(set valty:$dst, (SelectCC cmpty:$lhs, cmpty:$rhs,imm:$imm, valty:$truev, valty:$falsev))] */
/*               ,IIBranch>; */
/*  */
/* def Select_GPR_Using_CC_GPR : SelectCC_rrirr<ELMOGRRegs, ELMOGRRegs,"select">; */
/*  */





def psimm16  : PatLeaf<(imm), [{ return isInt<16>(N->getSExtValue()); }]>;
let Defs = [C] in {
def CMPD : FJ<0b000000, (outs), (ins ELMOGRRegs:$Rs1, ELMOGRRegs:$Rs2),
                      "cmpd $Rs1,$Rs2",
                      [(ELMOSetFlagInt i32:$Rs1, i32 :$Rs2)],
                      IIBranch
                      >;

def CMPF : FJ<0b000000, (outs), (ins ELMOGRFRegs:$Rs1, ELMOGRFRegs:$Rs2),
                      "cmpf $Rs1,$Rs2",
                      [(ELMOSetFlagFP f32 :$Rs1, f32 :$Rs2)],
                      IIBranch
                      >;

def CMPDI : FJ<0b000000, (outs), (ins ELMOGRRegs:$Rs1, Im:$imm),
                      "cmpdi $Rs1,$imm",
                      [(ELMOSetFlagInt i32:$Rs1, (i32 psimm16:$imm))],
                      IIBranch
                      >;
}
/* def : Pat<(setcc ELMOGRFRegs:$lhs, ELMOGRFRegs:$rhs, SETOGE), */
/*             (CMPF ELMOGRFRegs:$lhs, ELMOGRFRegs:$rhs)>; */





let Defs = [SP], Uses = [SP] in {  
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                "#ADJCALLSTACKDOWN $amt1 $amt2",
                                [(CallSeqStart timm:$amt1, timm:$amt2)], IIBranch>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                "#ADJCALLSTACKUP $amt1 $amt2",
                                [(CallSeqEnd timm:$amt1, timm:$amt2)],IIBranch>;
  }  

/* def SELECT : Pseudo<(outs ELMOGRRegs:$Rd), */
/* 						  (ins ELMOGRRegs:$Rs1, ELMOGRRegs:$Rs2, CCOp:$DDDI), */
/*                           "select", */
/* 						  [(set (i32 ELMOGRRegs:$Rd), */
/* 						   (ELMOSelectCC (i32 ELMOGRRegs:$Rs1), (i32 ELMOGRRegs:$Rs2), */
/* 										  (imm:$DDDI)))], */
/* 	IIBranch */
/* >;  */
def bare_symbol : Operand<OtherVT> {
}
let isCall = 1, hasDelaySlot = 1, isCodeGenOnly = 1,  Defs = [LK] in { 
/* def CALL : Pseudo<(outs), (ins brtarget:$addr), "bl $addr", [], IIBranch>; */
def PseudoCALL : Pseudo<(outs), (ins Im:$func),
                        "bl $func",
                        [(ELMOCall tglobaladdr:$func)], IIBranch>;
def CALLR : Pseudo<(outs), (ins ELMOGRRegs:$Rs1), "blrr $Rs1", [(ELMOCall ELMOGRRegs:$Rs1)],IIBranch>;


} 
let isReMaterializable = 1, isAsCheapAsAMove = 1, isMoveImm = 1 in {
  def LI  : FJ<14, (outs ELMOGRRegs:$rD), (ins Im:$imm),
                       "li $rD, $imm", 
                       [(set i32:$rD, imm32SExt16:$imm)], IIAlu>;

  def LWI  : FJ<14, (outs ELMOGRRegs:$rD), (ins Im:$imm),
                       "liw $rD, $imm", 
                       [(set i32:$rD, imm32:$imm)], IIAlu>;

  def LWIF  : FJ<14, (outs ELMOGRFRegs:$rD), (ins Imf:$imm),
                       "liw $rD, $imm", 
                       [(set f32:$rD, fpimm:$imm)], IIAlu>;
}
class FJJ<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin> : ELMOInst<outs, ins, asmstr, pattern, itin> {
  let Opcode = op;
         }

def FR : RegisterOperand<ELMOGRFRegs>;

/* def LWIF  : FJJ<14, (outs FR:$rD), (ins Imf:$imm), */
/*                        "liw $rD,$imm",  */
/*                        [(set f32:$rD, imm32f:$imm)], IIAlu>; */


// from sparcinstrinfo.td line 247
class ICC_VAL<int N> : PatLeaf<(i32 N)>;
def ICC_NE  : ICC_VAL< 9>;  // Not Equal
def ICC_E   : ICC_VAL< 1>;  // Equal
def ICC_G   : ICC_VAL<10>;  // Greater
def ICC_LE  : ICC_VAL< 2>;  // Less or Equal
def ICC_GE  : ICC_VAL<11>;  // Greater or Equal
def ICC_L   : ICC_VAL< 3>;  // Less
def ICC_GU  : ICC_VAL<12>;  // Greater Unsigned
def ICC_LEU : ICC_VAL< 4>;  // Less or Equal Unsigned
def ICC_CC  : ICC_VAL<13>;  // Carry Clear/Great or Equal Unsigned
def ICC_CS  : ICC_VAL< 5>;  // Carry Set/Less Unsigned
def ICC_POS : ICC_VAL<14>;  // Positive
def ICC_NEG : ICC_VAL< 6>;  // Negative
def ICC_VC  : ICC_VAL<15>;  // Overflow Clear
def ICC_VS  : ICC_VAL< 7>;  // Overflow Set

class FCC_VAL<int N> : PatLeaf<(i32 N)>;
def FCC_U   : FCC_VAL<23>;  // Unordered
def FCC_G   : FCC_VAL<22>;  // Greater
def FCC_UG  : FCC_VAL<21>;  // Unordered or Greater
def FCC_L   : FCC_VAL<20>;  // Less
def FCC_UL  : FCC_VAL<19>;  // Unordered or Less
def FCC_LG  : FCC_VAL<18>;  // Less or Greater
def FCC_NE  : FCC_VAL<17>;  // Not Equal
def FCC_E   : FCC_VAL<25>;  // Equal
def FCC_UE  : FCC_VAL<26>;  // Unordered or Equal
def FCC_GE  : FCC_VAL<27>;  // Greater or Equal
def FCC_UGE : FCC_VAL<28>;  // Unordered or Greater or Equal
def FCC_LE  : FCC_VAL<29>;  // Less or Equal
def FCC_ULE : FCC_VAL<30>;  // Unordered or Less or Equal
def FCC_O   : FCC_VAL<31>;  // Ordered


def IsOrAdd: PatFrag<(ops node:$A, node:$B), (or node:$A, node:$B), [{
  return isOrEquivalentToAdd(N);
}]>;


def AddrFI : ComplexPattern<iPTR, 1, "SelectAddrFI", [ frameindex ], []>;
multiclass LdPat<PatFrag LoadOp, ELMOInst Inst> {
def:Pat<(LoadOp ELMOGRRegs : $rs1), (Inst ELMOGRRegs : $rs1, 0)>;
def:Pat<(LoadOp(add ELMOGRRegs
              : $rs1, immSExt16
              : $imm16)),
      (Inst ELMOGRRegs
       : $rs1, immSExt16
       : $imm16)>;

def: Pat<(LoadOp AddrFI : $rs1), (Inst AddrFI : $rs1, 0)>;
def: Pat<(LoadOp(add AddrFI
              : $rs1, immSExt16
              : $imm12)),
      (Inst AddrFI
       : $rs1, immSExt16
       : $imm12)>;
def: Pat<(LoadOp(IsOrAdd AddrFI
              : $rs1, immSExt16
              : $imm12)),
      (Inst AddrFI
       : $rs1, immSExt16
       : $imm12)>;
}

multiclass StPat<PatFrag StoreOp, ELMOInst Inst, RegisterClass Reg> {
def: Pat<(StoreOp Reg
       : $rs1, ELMOGRRegs
       : $rs2),
      (Inst Reg
       : $rs1, ELMOGRRegs
       : $rs2, 0)>;
def: Pat<(StoreOp Reg
       : $rs1, (add ELMOGRRegs
                : $rs2, immSExt16
                : $imm16)),
      (Inst Reg
       : $rs1, ELMOGRRegs
       : $rs2, immSExt16
       : $imm16)>;
}
defm : LdPat<load, LOAD>;
defm : LdPat<load, LOADF>;

defm : StPat<store, STORE, ELMOGRRegs>;
defm : StPat<store, STOREF, ELMOGRFRegs>;

