// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/options.hpp>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

namespace requite::options {

bool parseOptions(int argc, const char **argv) {
  const char *fake_argv[] = {"./requite",      "test.rq",
                             "--token-csv",    "--single-thread-execution",
                             "--parse-ast",    "--name-intermediate-opcodes",
                             "--situated-ast", "--tabulation-tron",
                             "--llvm-ir"};
  llvm::cl::ParseCommandLineOptions(sizeof(fake_argv) / sizeof(const char *),
                                    fake_argv);
  bool is_ok = true;
  if (requite::options::MODULE_FILES.empty()) {
    llvm::errs() << "error: you must specify at least one module file.\n";
    llvm::errs() << "usage: " << argv[0] << " <module-file>\n";
    is_ok = false;
  }
  for (llvm::StringRef module_path : requite::options::MODULE_FILES) {
    if (!module_path.ends_with(".rq")) {
      llvm::errs() << "error: requite module source filename must end in "
                      "\".rq\" extension.\n";
      llvm::errs() << "file: " << module_path << "\n";
      is_ok = false;
    }
  }
  return is_ok;
}

} // namespace requite::options