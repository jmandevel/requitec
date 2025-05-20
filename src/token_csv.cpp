// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/csv.hpp>
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

void Context::writeTokenCsv(requite::Source &source,
                            std::vector<requite::Token> &tokens) {
  llvm::SmallString<64> str_buffer_a;
  llvm::SmallString<64> str_buffer_b;
  llvm::raw_svector_ostream str_buffer_a_ostream(str_buffer_a);
  for (const requite::Token &token : tokens) {
    str_buffer_b.clear();
    llvm::StringRef text = token.getSourceText();
    llvm::StringRef csv_value_text =
        requite::getCsvValueText(str_buffer_b, text);
    str_buffer_a_ostream << llvm::formatv(
        "{0},{1},{2},{3},{4}\n", token.getLine(), token.getColumn(),
        token.getSourceTextLength(), requite::getName(token.getType()),
        csv_value_text);
  }
  llvm::SmallString<256> path;
  std::error_code ec;
  if (!source.makeIntermediateFilePath(path, *this, ".tokens.csv")) {
    return;
  }
  llvm::raw_fd_ostream fout(path, ec, llvm::sys::fs::OF_Text);
  if (ec) {
    this->logMessage(
        llvm::Twine(
            "error: failed to open intermediate file for writing\n\tPath: ") +
        llvm::Twine(path) + llvm::Twine("\n\tReason: ") +
        llvm::Twine(ec.message()));
    return;
  }
  fout << str_buffer_a_ostream.str();
}

} // namespace requite