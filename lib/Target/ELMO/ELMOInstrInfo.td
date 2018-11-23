include "ELMOInstrFormats.td"

def IICLoad   : InstrItinClass;
def IICStore  : InstrItinClass;
def IIBranch  : InstrItinClass;
def IIAlu  : InstrItinClass;



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
    [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,SDNPVariadic]>;


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
}

defm : BrcondPats<ELMOGRRegs, JEQ, JNE, JLT, JGT, JLE, JGE, CMP, ZERO>;

/* let isReturn = 1,isCodeGenOnly = 1, hasCtrlDep=1, hasCtrlDep = 1 in  */
/* def BLR : JumpFR<0x000010, "blr", ELMOGRRegs> ; */
/*  */


let isReturn = 1,isBranch = 1, isTerminator=1, isBarrier = 1 in  
def BLR : FI < 0b000000, (outs), (ins ELMOGRRegs:$rc), "blr",  [(ELMORet ELMOGRRegs:$rc)] , IIBranch> ;

