class ELMOInst<dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin> : Instruction {
  field bits<32> Inst;

  let Namespace = "ELMO";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  let Size = 4;
  let Itinerary = itin;
  bits<6> Opcode;
  let Inst{31-26} = Opcode;
}

class FA<bits<6> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      ELMOInst<outs, ins, asmstr, pattern, itin>
{
  bits<5>  ra;
  bits<5>  rb;
  bits<5>  rc;

  let Opcode = op;

  let Inst{25-21} = ra;
  let Inst{20-16} = rb;
  let Inst{15-11} = rc;
}

//===----------------------------------------------------------------------===//
// Format L instruction class in Cpu0 : <|opcode|ra|rb|cx|>
//===----------------------------------------------------------------------===//

class FL<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: ELMOInst<outs, ins, asmstr, pattern, itin>
{
  bits<5>  ra;
  bits<5>  rb;
  bits<16> imm16;

  let Opcode = op;

  let Inst{25-21} = ra;
  let Inst{20-16} = rb;
  let Inst{15-0}  = imm16;
}

//===----------------------------------------------------------------------===//
// Format J instruction class in ELMO : <|opcode|address|>
//===----------------------------------------------------------------------===//

class FJ<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: ELMOInst<outs, ins, asmstr, pattern, itin>
{
  bits<26> addr;

  let Opcode = op;

  let Inst{25-0} = addr;
}

class FI<bits<6> op, dag outs, dag ins, string asmstr, list<dag> pattern,
         InstrItinClass itin>: ELMOInst<outs, ins, asmstr, pattern, itin>
{
  let Opcode = op;
}





/* class ItinInst<bits<6> opcode, dag outs, dag ins, string asmstr,list<dag> pattern, InstrItinClass itin>: InstELMO<outs, ins, asmstr, pattern,itin> { */
/*     let Inst{31-26} = opcode; */
/* } */
/*  */
/* class ALUInst<bits<6> opcode, dag outs, dag ins, string asmstr, list<dag> pattern> */
/*     : InstELMO<outs, ins, asmstr, pattern> { */
/*   let Inst{31-26} = opcode; */
/* } */
/*  */
/*  */
/*  */
/*  */
/* class FJ<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern, */
/* InstrItinClass itin>: ELMOInst<outs, ins, asmstr, pattern, itin> */
/* { */
/*    bits<24> addr; */
/*  */
/*    let Opcode = op; */
/*  */
/*    et Inst{23-0} = addr; */
/* } */
/*  */
/* // Return */
/* class RetInst<bits<6> op, string asmstr>: */
/*   ALUInst<op, (outs), (ins ELMOGRRegs:$rc), */
/*      asmstr, [(ELMORet ELMOGRRegs:$rc)] > { */
/*   let isBranch=1; */
/*   let isTerminator=1; */
/*   let isBarrier=1; */
/*   let isReturn=1; */
/* } */
/*  */
/* class DForm<bits<6> opcode, string opstr, SDNode opnode >  */
/* :ALUInst<opcode, (outs ELMOGRRegs:$dst), */
/*                    (ins ELMOGRRegs:$src1, mem:$imm), */
/*                    !strconcat(opstr, " $dst, $src, $imm"), */
/*                    [(set i32:$dst, (opnode i32:$src, imm:$imm))]> { */
/*     bits<5> dst; */
/*     bits<5> src; */
/*     bits<16> imm; */
/*     let Inst{15-0} = imm; */
/*     let Inst{20-16} = src; */
/*     let Inst{25-21} = dst; */
/*     let DecoderMethod = "DecodeMoveTarget"; */
/* } */
/*  */
/* class XForm<bits<6> opcode, string opstr, SDNode opnode > : ALUInst<opcode, (outs ELMOGRRegs:$dst), */
/*                    (ins ELMOGRRegs:$src1, ELMOGRRegs:$src2), */
/*                    !strconcat(opstr, " $dst, $src1, $src2"), */
/*                    [(set i32:$dst, (opnode i32:$src, i32:$src2))]> { */
/*  */
/*     bits<5> dst; */
/*     bits<5> src1; */

/*     bits<5> src2; */
/*  */
/*     let Inst{15-11} = src2; */
/*     let Inst{20-16} = src1; */
/*     let Inst{25-21} = dst; */
/* } */
/*  */
/*  */
/*  */
/*  */
/*  */
/* class BinOp<bits<6> opcode, string opstr, SDNode opnode>  */
/*   :ALUInst<opcode, (outs ELMOGRRegs:$dst), */
/*                    (ins ELMOGRRegs:$src1, ELMOGRRegs:$src2), */
/*                    !strconcat(opstr, " $dst, $src1, $src2"), */
/*                    [(set i32:$dst, (opnode i32:$src1, i32:$src2))]> { */
/*     bits<5> dst; */
/*     bits<5> src1; */
/*     bits<5> src2; */
/*     let Inst{25-21} = dst; */
/*     let Inst{20-16} = src1; */
/*     let Inst{15-11} = src2; */
/* } */
class Pseudo<dag outs, dag ins, string opcodestr,list<dag> pattern, InstrItinClass itin >
    : ELMOInst<outs, ins, opcodestr,  pattern, itin> {
          let isPseudo = 1;
          let isCodeGenOnly = 1;
    }

