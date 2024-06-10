# Part of Project Risc 16 assembler
# Copyright (c) 2024 
# Harith Manoj (harithpub@gmail.com)
# Licensed under GNU GENERAL PUBLIC LICENSE, Version 3
# Date: 10 June 2024

include(${EXAMPLE_BUILD_DIR})

message(STATUS "CMAKE Now at Risc 16 Assembler Examples Configuration")

function(exampleRisc16Asm targetName sourceList)
    exampleBuildTemplate("risc16asm" ${targetName} ${sourceList} ${EX_DEPENDANCY})
endfunction(exampleRisc16Asm)

function(exampleRisc16AsmSingle targetName source)
    set(EX_SOURCES ${source})
    exampleRisc16Asm(${targetName} ${EX_SOURCES})
endfunction(exampleRisc16AsmSingle)

set(
    EX_DEPENDANCY 
    genAsmLib
)

