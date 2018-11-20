
def SDT_ELMORet : SDTypeProfile < 0, 1, [SDTCisInt<0>]>;
def ELMORet : SDNode < "ELMOISD::Ret", SDT_ELMORet,
    [SDNPHasChain, SDNPOptInGlue,SDNPVariadic]>;
def SDT_ELMOCall : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def ELMOCall : SDNode < "ELMOISD::Call", SDT_ELMOCall,
    [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,SDNPVariadic]>;


class InstELMO<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "ELMO";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  let Size = 4;
}
class ALUInst<bits<6> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstELMO<outs, ins, asmstr, pattern> {
  let Inst{31-26} = opcode;
}

// Return
class RetInst<bits<6> op, string asmstr>:
  ALUInst<op, (outs), (ins ELMOGRRegs:$rc),
     asmstr, [(ELMORet ELMOGRRegs:$rc)] > {
  let isBranch=1;
  let isTerminator=1;
  let isBarrier=1;
  let isReturn=1;
}


class BinOp<bits<6> opcode, string opstr, SDNode opnode> 
  :ALUInst<opcode, (outs ELMOGRRegs:$dst),
                   (ins ELMOGRRegs:$src1, ELMOGRRegs:$src2),
                   !strconcat(opstr, " $dst, $src1, $src2"),
                   [(set i32:$dst, (opnode i32:$src1, i32:$src2))]> {
    bits<5> dst;
    bits<5> src1;
    bits<5> src2;
    let Inst{25-21} = dst;
    let Inst{20-16} = src1;
    let Inst{15-11} = src2;
}
def ADD  : BinOp<0b000000, "add", add>;
def SUB  : BinOp<0b000001, "sub", sub>;
def BLR  : RetInst<0b000010, "blr">; 


class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
   : InstELMO<outs, ins, asmstr, pattern> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}
