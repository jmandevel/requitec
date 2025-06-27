// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>

#include <llvm/Support/FileSystem.h>

namespace requite {

void Context::writeLlvmIr(llvm::StringRef output_path) {
  std::error_code ec;
  llvm::raw_fd_ostream fout(output_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tpath: ") +
        llvm::Twine(output_path) + llvm::Twine("\n\treason: ") +
        llvm::Twine(ec.message()));
    return;
  }
  std::string ir_text = this->getLlvmIrSourceText();
  fout << ir_text;
}

} // namespace requite