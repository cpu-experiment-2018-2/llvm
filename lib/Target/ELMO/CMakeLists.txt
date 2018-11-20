
set(LLVM_TARGET_DEFINITIONS ELMO.td)

tablegen(LLVM PPCGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM ELMOGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM ELMOGenCallingConv.inc -gen-callingconv)
tablegen(LLVM ELMOGenDAGISel.inc -gen-dag-isel)
#tablegen(LLVM ELMOGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM ELMOGenFastISel.inc -gen-fast-isel)
tablegen(LLVM ELMOGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM ELMOGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM ELMOGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM ELMOGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(ELMOCommonTableGen)

add_llvm_target(ELMOCodeGen
  # ELMOBoolRetToInt.cpp
  ELMOAsmPrinter.cpp
  # ELMOBranchSelector.cpp
  # ELMOBranchCoalescing.cpp
  # ELMOCCState.cpp
  # ELMOCTRLoops.cpp
  # ELMOHazardRecognizers.cpp
  ELMOInstrInfo.cpp
  ELMOISelDAGToDAG.cpp
  ELMOISelLowering.cpp
  # ELMOEarlyReturn.cpp
  # ELMOFastISel.cpp
  ELMOFrameLowering.cpp
  # ELMOLoopPreIncPrep.cpp
  ELMOMCInstLower.cpp
  ELMOMachineFunctionInfo.cpp
  # ELMOMIPeephole.cpp
  ELMORegisterInfo.cpp
  # ELMOQPXLoadSplat.cpp
  ELMOSubtarget.cpp
  ELMOTargetMachine.cpp
  ELMOTargetObjectFile.cpp
  # ELMOTargetTransformInfo.cpp
  # ELMOTOCRegDeps.cpp
  # ELMOTLSDynamicCall.cpp
  # ELMOVSXCopy.cpp
  # ELMOReduceCRLogicals.cpp
  # ELMOVSXFMAMutate.cpp
  # ELMOVSXSwapRemoval.cpp
  # ELMOExpandISEL.cpp
  # ELMOPreEmitPeephole.cpp
  )

# add_subdirectory(AsmParser)
# add_subdirectory(Disassembler)
add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
