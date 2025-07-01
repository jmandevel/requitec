// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/options.hpp>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>

int main(int argc, const char **argv) {
  std::string executable_path =
      llvm::sys::fs::getMainExecutable(argv[0], reinterpret_cast<void *>(main));
  // TODO remove fake_argv when done with alpha stage of development. this is here for ease of use.
  const char *fake_argv[] = {"./requite", "--emit=ir", "test.rq", "-o",
                             "test.ir"};
  if (argc == 1) {
    llvm::cl::ParseCommandLineOptions(5, fake_argv);
  } else {
    llvm::cl::ParseCommandLineOptions(argc, argv);
  }
  requite::Context context(std::move(executable_path));
  if (!context.run()) {
    return 1;
  }
  return 0;
}
