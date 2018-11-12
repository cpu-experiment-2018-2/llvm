#ifndef  LLVM_LIB_TARGET_ELMO_ELMOMACHINEFUNCTIONINFO_H
#define  LLVM_LIB_TARGET_ELMO_ELMOMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"
namespace llvm{
    class ELMOMachineFunctionInfo: public MachineFunctionInfo{
        MachineFunction&MF;
    public:
        explicit ELMOMachineFunctionInfo(MachineFunction&MF) : MF(MF){}
        virtual  void anchor();
    };
}
#endif
