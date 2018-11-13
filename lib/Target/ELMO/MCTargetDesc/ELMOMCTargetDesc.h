#ifndef LLVM_LIB_TARGET_ELMO_MCTARGETDESC_ELMOMCTARGETDESC_H
#define LLVM_LIB_TARGET_ELMO_MCTARGETDESC_ELMOMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class Triple;

Target&getTheELMOTarget();
} // End llvm namespace

// Defines symbolic names for ELMO registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "ELMOGenRegisterInfo.inc"

// Defines symbolic names for the ELMO instructions.
#define GET_INSTRINFO_ENUM
#include "ELMOGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "ELMOGenSubtargetInfo.inc"

#endif
