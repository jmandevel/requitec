// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/token_spacing.hpp>
#include <requite/token_type.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

namespace requite {

struct Context;
struct Source;

struct Token final {
  requite::TokenType _type;
  requite::TokenSpacing _spacing;
  unsigned _line;
  unsigned _column;
  const char *_source_text_ptr;
  unsigned _source_text_length;

  Token() = default;

  Token(requite::TokenType type, unsigned line, unsigned column,
        const char *source_text_ptr, unsigned source_text_length,
        requite::TokenSpacing spacing);

  [[nodiscard]]
  requite::TokenType getType() const;

  [[nodiscard]]
  unsigned getLine() const;

  [[nodiscard]]
  unsigned getColumn() const;

  [[nodiscard]]
  requite::TokenSpacing getSpacing() const;

  [[nodiscard]]
  bool getHasBinaryOperatorSpacing() const;

  [[nodiscard]]
  bool getHasUnaryOperatorSpacing() const;

  [[nodiscard]]
  bool getHasInvalidOperatorSpacing() const;

  [[nodiscard]]
  llvm::StringRef getSourceText() const;

  [[nodiscard]]
  const char *getSourceTextPtr() const;

  [[nodiscard]]
  unsigned getSourceTextLength() const;

  void dropFrontAndBack();

  void dropFront();

  void dropBack();

  void setUnmatched();

  [[nodiscard]] llvm::SMLoc getSourceStart() const;
  [[nodiscard]] llvm::SMLoc getSourceEnd() const;
  [[nodiscard]] llvm::SMRange getSourceRange() const;
};

} // namespace requite