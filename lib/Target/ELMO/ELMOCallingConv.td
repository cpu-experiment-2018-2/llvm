def CC_ELMO : CallingConv <[
    CCIfType<[i32,f32],CCAssignToReg<[
    G3,G4,G5,G6,G7,G8,G9,G10,G11,G12,G13,G14,G15,G16,G17,G18,G19,G20
    ]>>
]>;

def RetCC_ELMO : CallingConv<[
    CCIfType<[i32,f32], CCAssignToReg<[G3]>>
]>;

/*
def CSR : CalleeSavedRegs<(add)> {
  let OtherPreserved = (add FP, SP, LK);
}
*/
def CSR : CalleeSavedRegs<(add  LK)> ;
