#ifndef LLVM_LIB_TARGET_ELMO_MCTARGETDESC_ELMOMCASMINFO_H
#define LLVM_LIB_TARGET_ELMO_MCTARGETDESC_ELMOMCASMINFO_H

#include <llvm/MC/MCAsmInfoELF.h>
namespace llvm{
    class  Triple;
    class ELMOMCAsmInfo : public MCAsmInfoELF{
        void anchor() override;
    public:
        explicit ELMOMCAsmInfo (const Triple & TheTriple);
    };


}


#endif