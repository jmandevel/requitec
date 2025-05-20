# SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.29.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(requite)

add_library(librequite STATIC "")

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(LLVM_LIBS support core irreader mc mca mcdisassembler mcjit mcparser X86CodeGen X86Info X86Desc TargetParser X86)

find_package(ICU COMPONENTS data)
find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(
    librequite
    PUBLIC
        magic_enum::magic_enum
        ${LLVM_LIBS}
        ICU::data
)

target_include_directories(
    librequite
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"   
)

add_executable(requite "")

target_link_libraries(
    requite
    PRIVATE
        librequite
)

add_subdirectory(src)

add_subdirectory(test)

