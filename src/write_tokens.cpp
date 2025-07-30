// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/literal_text.hpp>
#include <requite/token.hpp>

#include "llvm/Support/FormatVariadic.h"
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

#include <memory>

namespace requite {

bool Context::writeTokens(requite::Module &module,
                            std::vector<requite::Token> &tokens, llvm::StringRef out_path) {
  llvm::SmallString<64> str_buffer_a;
  llvm::raw_svector_ostream str_buffer_a_ostream(str_buffer_a);
  for (const requite::Token &token : tokens) {
    llvm::StringRef text = token.getSourceText();
    std::string csv_value_text =
        requite::getLiteralValue(text);
    str_buffer_a_ostream << llvm::formatv(
        "{0},{1},{2},{3},{4}\n", token.getLine(), token.getColumn(),
        token.getSourceTextLength(), requite::getName(token.getType()),
        csv_value_text);
  }
  std::error_code ec;
  llvm::raw_fd_ostream fout(out_path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open output file for writing\n\tPath: ") +
        llvm::Twine(out_path) + llvm::Twine("\n\tReason: ") +
        llvm::Twine(ec.message()) + "\n");
    return false;
  }
  fout << str_buffer_a_ostream.str();
  return true;
}

} // namespace requite