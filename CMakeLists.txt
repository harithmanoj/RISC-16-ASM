# Part of Project Risc 16 assembler
# Copyright (c) 2024 
# Harith Manoj (harithpub@gmail.com)
# Licensed under GNU GENERAL PUBLIC LICENSE, Version 3
# Date: 10 June 2024


cmake_minimum_required(VERSION 3.10)

option(RISC_16_ASM_BUILD_TEST "Should Build tests along with library" OFF)
option(RISC_16_ASM_BUILD_EXAMPLE "Should Build examples along with library" OFF)

set(RISC_16_ASM_VERSION 0.0.1)
set(RISC_16_ASM_BUILD_TYPE alpha)

if(
    RISC_16_ASM_BUILD_TEST
    AND RISC_16_ASM_BUILD_EXAMPLE
    AND RISC_16_ASM_BUILD_CPP_TESTER_EXT
)
    set(RISC_16_ASM_BUILD_CONFIG "ALL")
else()
    set(RISC_16_ASM_BUILD_CONFIG "EXE")
    
    if(RISC_16_ASM_BUILD_TEST)
        # Test are build
        string(APPEND RISC_16_ASM_BUILD_CONFIG "_TEST")
    endif(RISC_16_ASM_BUILD_TEST)

    if(RISC_16_ASM_BUILD_EXAMPLE)
        # Example are build
        string(APPEND RISC_16_ASM_BUILD_CONFIG "_EX")
    endif(RISC_16_ASM_BUILD_EXAMPLE)
    
endif(
    RISC_16_ASM_BUILD_TEST
    AND RISC_16_ASM_BUILD_EXAMPLE
    AND RISC_16_ASM_BUILD_CPP_TESTER_EXT
)

message(STATUS "CMAKE Now at Risc 16 Assembler Building Variant:        ${RISC_16_ASM_BUILD_CONFIG}")
message(STATUS "    Configuring for version:                            ${RISC_16_ASM_VERSION}.${RISC_16_ASM_BUILD_TYPE}")

project(risc16asm VERSION ${RISC_16_ASM_VERSION} DESCRIPTION "C++ Program for assembling risc 16 asm code.")

file(WRITE "${CMAKE_BINARY_DIR}/risc16asm.version" "${RISC_16_ASM_VERSION}.${RISC_16_ASM_BUILD_TYPE}")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "    Configuring Test for EasyCppTester")

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED True)

    if(RISC_16_ASM_BUILD_TEST)
        enable_testing()
    endif(RISC_16_ASM_BUILD_TEST)
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)


if(CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "   Standard minimum of c++20")
endif()

set (
    SOURCE_LIST
    src/asm.cpp
)

add_executable(risc16asm ${SOURCE_LIST})

set_target_properties(risc16asm PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
    risc16asm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
    risc16asm
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

if(RISC_16_ASM_BUILD_CPP_TESTER_EXT OR RISC_16_ASM_BUILD_TEST)
    
    if(TARGET EasyCppTesterLib)
        message(STATUS "    EasyCppTesterLib found already added")
    else()
        message(STATUS "    EasyCppTesterLib not found, using add_subdirectory")
        add_subdirectory(EasyCppTester)
        
    endif(TARGET EasyCppTesterLib)

    target_link_libraries(risc16asm PUBLIC EasyCppTesterLib)
endif(RISC_16_ASM_BUILD_CPP_TESTER_EXT OR RISC_16_ASM_BUILD_TEST)


set_target_properties(risc16asm PROPERTIES VERSION ${RISC_16_ASM_VERSION})

file(GLOB_RECURSE TEST_BUILD_DIR "testBuild.cmake")
file(GLOB_RECURSE EXAMPLE_BUILD_DIR "exampleBuild.cmake")

if(RISC_16_ASM_BUILD_EXAMPLE)
    add_subdirectory(example)    
endif(RISC_16_ASM_BUILD_EXAMPLE)

if(RISC_16_ASM_BUILD_TEST)
    add_subdirectory(test)    
endif(RISC_16_ASM_BUILD_TEST)