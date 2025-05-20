# SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(Catch2)

add_executable(requite_tests "")

add_subdirectory(src)

target_link_libraries(
    requite_tests
    PRIVATE
        Catch2::Catch2WithMain
        librequite
)

include(CTest)
include(Catch)
catch_discover_tests(
  requite_tests
  TEST_PREFIX "unittests."
  REPORTER junit
  OUTPUT_DIR ${CMAKE_BINARY_DIR}
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
)