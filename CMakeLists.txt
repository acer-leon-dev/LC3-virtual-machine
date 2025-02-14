cmake_minimum_required (VERSION 3.31)

project(lc3_virtual_machine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDEDIRS
    "include"
)
set(PRECOMPILEDHEADERS
    "include/LC3VM/Precompiled.h"
)
set(SOURCES
    "src/vm.cpp"
    "src/Bit.cpp"
    "src/LC3.cpp"
    "src/Memory.cpp"
    "src/Operations.cpp"
    "src/Platform.cpp"
    "src/RegisterFile.cpp"
    "src/Traps.cpp"
)

add_executable(lc3_vm ${SOURCES})
target_include_directories(
    lc3_vm 
    PUBLIC 
        ${INCLUDEDIRS})
target_precompile_headers(
    lc3_vm 
    PUBLIC 
        ${PRECOMPILEDHEADERS})
target_compile_options(
    lc3_vm 
    PUBLIC 
        -g -lstdc++exp -Wall -Wextra -Wno-unused)
target_link_libraries(
    lc3_vm 
        stdc++exp)
set_target_properties(lc3_vm 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_SOURCE_DIR}/bin
)