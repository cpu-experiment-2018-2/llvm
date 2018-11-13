#include <llvm/ADT/Triple.h>
#include <llvm/Support/WithColor.h>
#include "ELMOMCAsmInfo.h"

namespace llvm{
    Triple h;
    void ELMOMCAsmInfo::anchor() {}
    ELMOMCAsmInfo::ELMOMCAsmInfo(const llvm::Triple &TheTriple) {
        h = TheTriple;
        if(TheTriple.getArch()==Triple::elmo)
        IsLittleEndian = false;

        WithColor::note() << "MCAsmInfo constructer" ;
    }
}