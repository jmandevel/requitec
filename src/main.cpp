// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/options.hpp>

#include <llvm/Support/FileSystem.h>

int main(int argc, const char **argv) {
  std::string executable_path =
      llvm::sys::fs::getMainExecutable(argv[0], reinterpret_cast<void *>(main));
  if (!requite::options::parseOptions(argc, argv)) {
    return 1;
  }
  requite::Context context(std::move(executable_path));
  if (!context.run()) {
    return 1;
  }
  return 0;
}
