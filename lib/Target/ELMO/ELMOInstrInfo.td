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



def SDT_ELMOSetFlag      : SDTypeProfile<0,  2, [SDTCisSameAs<0, 1>]>;
def ELMOSetFlag     : SDNode<"ELMOISD::SET_FLAG", SDT_ELMOSetFlag,
                              [SDNPOutGlue]>;
def SDT_ELMOSelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                 SDTCisSameAs<1, 2>
                                                 ]>;
def ELMOSelectCC     : SDNode<"ELMOISD::SELECT_CC", SDT_ELMOSelectCC,
                          [SDNPInGlue]>;
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
def ELMOBrCC        : SDNode<"ELMOISD::BR_CC", SDT_ELMOBrCC,
                              [SDNPHasChain, SDNPInGlue]>;


def ELMOCall : SDNode < "ELMOISD::Call", SDT_ELMOCall,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                               SDNPVariadic]>;


def CCOp : Operand<i32> {
  let PrintMethod = "printCCOperand";
}

def brtarget    : Operand<OtherVT> {
  let EncoderMethod = "getBranchTargetOpValue";
  let OperandType = "OPERAND_PCREL";
  let DecoderMethod = "DecodeBranchTarget";
}



//  Arithmetic and logical instructions with 2 register operands.
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
def immSExt16  : PatLeaf<(imm), [{ return isInt<16>(N->getSExtValue()); }]>;
def ADDiu   : ArithLogicI<0x09, "addi", add, simm16, immSExt16, ELMOGRRegs>;
def SUBiu   : ArithLogicI<0x09, "subi", sub, simm16, immSExt16, ELMOGRRegs>;

def mem : Operand<i32> {
      let PrintMethod = "printMemOperand";
      let MIOperandInfo = (ops ELMOGRRegs, i16imm);
      let EncoderMethod = "getMemEncoding";
}


def ADD  : ArithLogicR<0b000000, "add", add, IIAlu, ELMOGRRegs, 1>;
def SUB  : ArithLogicR<0b000001, "sub", sub, IIAlu, ELMOGRRegs, 0>;
/* def BLR  : RetInst<0b000010, "blr">;  */
/* def LOAD  : DForm<0b000011, "load", load>;  */


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

class CBranch<bits<6> op, string instr_asm, RegisterClass RC,
                   list<Register> UseRegs>:
  FJ<op, (outs), (ins RC:$ra, brtarget:$addr),
             !strconcat(instr_asm, "\t$ra, $addr"),
             [], IIBranch> {
  let isBranch = 1;
  let isTerminator = 1;
  let hasDelaySlot = 0;
  let DecoderMethod = "DecodeBranchTarget";
}

class CmpInstr<bits<6> op, string instr_asm, 
               InstrItinClass itin, RegisterClass RC, RegisterClass RD, 
               bit isComm = 0>:
  FA<op, (outs RD:$rc), (ins RC:$ra, RC:$rb),
     !strconcat(instr_asm, "\t$rc, $ra, $rb"), [], itin> {
  let rc = 0;
  let isCommutable = isComm;
}

def JEQ     : CBranch<0x10, "jeq", COND, [G29]>;
def JNE     : CBranch<0x11, "jne", COND, [G29]>;
def JLT     : CBranch<0x12, "jlt", COND, [G29]>;
def JGT     : CBranch<0x13, "jgt", COND, [G29]>;
def JLE     : CBranch<0x14, "jle", COND, [G29]>;
def JGE     : CBranch<0x15, "jge", COND, [G29]>;
def CMP     : CmpInstr<0x16, "cmpd",IIBranch, COND,ELMOGRRegs, 0>;



// brcond patterns
multiclass BrcondPats<RegisterClass RC, Instruction JEQOp, Instruction JNEOp, 
Instruction JLTOp, Instruction JGTOp, Instruction JLEOp, Instruction JGEOp, 
Instruction CMPOp, Register ZEROReg> {
def : Pat<(brcond (i32 (seteq RC:$lhs, RC:$rhs)), bb:$dst),
          (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setueq RC:$lhs, RC:$rhs)), bb:$dst),
          (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setne RC:$lhs, RC:$rhs)), bb:$dst),
          (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setune RC:$lhs, RC:$rhs)), bb:$dst),
          (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setlt RC:$lhs, RC:$rhs)), bb:$dst),
          (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setult RC:$lhs, RC:$rhs)), bb:$dst),
          (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setgt RC:$lhs, RC:$rhs)), bb:$dst),
          (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setugt RC:$lhs, RC:$rhs)), bb:$dst),
          (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setle RC:$lhs, RC:$rhs)), bb:$dst),
          (JLEOp (CMPOp RC:$rhs, RC:$lhs), bb:$dst)>;
def : Pat<(brcond (i32 (setule RC:$lhs, RC:$rhs)), bb:$dst),
          (JLEOp (CMPOp RC:$rhs, RC:$lhs), bb:$dst)>;
def : Pat<(brcond (i32 (setge RC:$lhs, RC:$rhs)), bb:$dst),
          (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i32 (setuge RC:$lhs, RC:$rhs)), bb:$dst),
          (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond RC:$cond, bb:$dst),
          (JNEOp (CMPOp RC:$cond, ZEROReg), bb:$dst)>;
def : Pat<(setcc RC:$lhs, RC:$rhs, SETEQ),
            (CMP RC:$lhs, RC:$rhs)>;

}

defm : BrcondPats<ELMOGRRegs, JEQ, JNE, JLT, JGT, JLE, JGE, CMP, ZERO>;

/* let isReturn = 1,isCodeGenOnly = 1, hasCtrlDep=1, hasCtrlDep = 1 in  */
/* def BLR : JumpFR<0x000010, "blr", ELMOGRRegs> ; */
/*  */


let isBranch = 1, isBarrier = 1, isTerminator = 1, hasDelaySlot = 1 in {
  let Uses = [G29] in
    def BRCC : FJ<0b000111,(outs), (ins brtarget:$addr, CCOp:$DDDI),
                      "b$DDDI\t$addr",
                      [(ELMOBrCC bb:$addr, imm:$DDDI)],
	IIBranch
>;
}



let isReturn = 1,isBranch = 1, isTerminator=1, isBarrier = 1 in  
def BLR : FI < 0b000000, (outs), (ins ELMOGRRegs:$rc), "blr",  [(ELMORet ELMOGRRegs:$rc)] , IIBranch> ;
multiclass SetltPats<RegisterClass RC> {
  def : Pat<(setlt RC:$lhs, RC:$rhs),
            (CMP RC:$lhs, RC:$rhs)>;
//          (XORi (ANDi (SHR (CMP RC:$lhs, RC:$rhs), (LUi 0x8000), 31), 1), 1)>;
  def : Pat<(setult RC:$lhs, RC:$rhs),
            (CMP RC:$lhs, RC:$rhs)>;
}

defm : SetltPats<ELMOGRRegs>;

multiclass SeteqPats<RegisterClass RC> {
  def : Pat<(seteq RC:$lhs, RC:$rhs),
            (CMP RC:$lhs, RC:$rhs)>;
  def : Pat<(setcc RC:$lhs, RC:$rhs, SETEQ),
            (CMP RC:$lhs, RC:$rhs)>;
}

defm : SeteqPats<ELMOGRRegs>;

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

def SET_FLAG : FJ<0b000000, (outs), (ins ELMOGRRegs:$Rs1, ELMOGRRegs:$Rs2),
                      "cmp",
                      [(ELMOSetFlag (i32 ELMOGRRegs:$Rs1), (i32 ELMOGRRegs:$Rs2))],
                      IIBranch
                      >;


/* let Defs = [SP], Uses = [SP] in { */
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                "#ADJCALLSTACKDOWN $amt1 $amt2",
                                [(CallSeqStart timm:$amt1, timm:$amt2)], IIBranch>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                "#ADJCALLSTACKUP $amt1 $amt2",
                                [(CallSeqEnd timm:$amt1, timm:$amt2)],IIBranch>;
/* } */

def SELECT : Pseudo<(outs ELMOGRRegs:$Rd),
						  (ins ELMOGRRegs:$Rs1, ELMOGRRegs:$Rs2, CCOp:$DDDI),
                          "select",
						  [(set (i32 ELMOGRRegs:$Rd),
						   (ELMOSelectCC (i32 ELMOGRRegs:$Rs1), (i32 ELMOGRRegs:$Rs2),
										  (imm:$DDDI)))],
	IIBranch
>; 
let isCall = 1, hasDelaySlot = 1, isCodeGenOnly = 1, Uses = [SP] in {
/*     Defs = [RCA] in { */
def CALL : Pseudo<(outs), (ins brtarget:$addr), "", [], IIBranch>;
def CALLR : Pseudo<(outs), (ins ELMOGRRegs:$Rs1), "", [(ELMOCall ELMOGRRegs:$Rs1)],IIBranch>;
} 

