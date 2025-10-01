cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(requitec)

add_library(requite STATIC "")

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(LLVM_LIBS support core irreader mc mca mcdisassembler mcjit mcparser X86CodeGen X86Info X86Desc TargetParser X86)

target_link_libraries(
    requite
    PUBLIC
        ${LLVM_LIBS}
)   

target_include_directories(
    requite
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

add_executable(requitec "")

target_link_libraries(
    requitec
    PRIVATE
        requite   
)

add_subdirectory(src)