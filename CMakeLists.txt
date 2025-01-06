cmake_minimum_required (VERSION 3.31)

project(lc3_virtual_machine LANGUAGES CXX)
set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)

set(SOURCES
    "src/vm.cpp"
)
set(INCLUDEDIRS
)

add_executable(lc3-vm ${SOURCES})
set_target_properties(lc3-vm 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_SOURCE_DIR}/bin
)