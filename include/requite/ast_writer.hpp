// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

#include <functional>
#include <string>

namespace requite {

struct Module;
struct Context;
struct Expression;

struct AstWriter final {
  std::string _buffer;
  llvm::raw_string_ostream _ostream;
  std::reference_wrapper<Context> _context_ref;
  unsigned _indentation;

  // write_ast.cpp
  AstWriter(requite::Context &context);
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  llvm::raw_string_ostream &getOstream();
  void writeAst(const requite::Module &module, llvm::Twine sub_extension);
  void writeIndentation();
  void addIndentation();
  void removeIndentation();
  void writeExpression(const requite::Expression &expression);
  void writeExpressionLocationComment(const requite::Expression &expression);
};

struct AstWriterIndentLock final {
  using Self = requite::AstWriterIndentLock;

  std::reference_wrapper<requite::AstWriter> _writer;

  // write_ast.cpp
  AstWriterIndentLock(AstWriter &writer);
  AstWriterIndentLock(const Self &) = delete;
  AstWriterIndentLock(Self &&) = delete;
  ~AstWriterIndentLock();

  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace requite