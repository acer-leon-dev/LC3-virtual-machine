cmake_minimum_required (VERSION 3.31)

project(lc3_virtual_machine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDEDIRS
    "include"
)
set(SOURCES
    "src/vm.cpp"
    "src/vm_operations.cpp"
    "src/vm_base.cpp"
)

add_executable(lc3-vm ${SOURCES})
target_include_directories(lc3-vm PUBLIC ${INCLUDEDIRS})
target_compile_options(lc3-vm PUBLIC -g -lstdc++exp -Wall -Wextra -Wno-unused)
target_link_libraries(lc3-vm stdc++exp)
set_target_properties(lc3-vm 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_SOURCE_DIR}/bin
)