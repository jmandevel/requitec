cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(requitec)

add_library(requite STATIC "")

find_package(LLVM REQUIRED CONFIG)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(LLVM_LIBS support core irreader mc mca mcdisassembler mcjit mcparser X86CodeGen X86Info X86Desc TargetParser X86)

target_link_libraries(
    requite
    PUBLIC
        ${LLVM_LIBS}
)   

target_compile_options(requite PRIVATE    
    # warnings as errors (all configs)
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-switch-enum -Wno-extra-semi-stmt -Wno-unsafe-buffer-usage -Wno-global-constructors -Wno-exit-time-destroys -Wno-main>

    # no exceptions on release builds
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/EHs-c- /D_HAS_EXCEPTIONS=0>
    $<$<AND:$<CONFIG:Release>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fno-exceptions>
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