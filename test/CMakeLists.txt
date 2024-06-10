# Part of Project Risc 16 assembler
# Copyright (c) 2024 
# Harith Manoj (harithpub@gmail.com)
# Licensed under GNU GENERAL PUBLIC LICENSE, Version 3
# Date: 10 June 2024

include(${TEST_BUILD_DIR})

message(STATUS "CMAKE Now at Risc 16 Assember Tests Configuration")

set(
    TEST_DEPENDANCY 
    genAsmLib
    EasyCppTesterLib
)

function(unitTestRisc16Asm targetName)
    unitTestTemplate("risc16asm" ${targetName} ${TEST_SOURCES} ${TEST_DEPENDANCY})
endfunction(unitTestRisc16Asm )

