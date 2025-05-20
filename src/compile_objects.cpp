// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>

#include <string_view>

namespace requite {

bool Context::compileObject(requite::Module &module) {
  llvm::StringRef extension = ".o";
  llvm::SmallString<256> path;
  if (!module.getSource().makeIntermediateFilePath(path, *this, extension)) {
    return false;
  }
  std::error_code ec;
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        llvm::Twine(path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()));
    return false;
  }
  llvm::legacy::PassManager pass;
  const auto file_type = llvm::CodeGenFileType::ObjectFile;
  llvm::TargetMachine &target_machine = this->getLlvmTargetMachine();
  if (target_machine.addPassesToEmitFile(pass, fout, nullptr, file_type)) {
    this->logMessage(
        llvm::Twine("error: failed to add passes to emit file\n\tpath: ") +
        llvm::Twine(path));
    return false;
  }
  pass.run(module.getLlvmModule());
  fout.flush();
  return true;
}

} // namespace requite