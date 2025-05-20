// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/token.hpp>

#include <llvm/ADT/StringRef.h>

#include <cstdint>

namespace requite {

struct SourceRanger final {
  using Self = requite::SourceRanger;

  llvm::StringRef::iterator _start;
  llvm::StringRef::iterator _current;
  llvm::StringRef::iterator _end;

  std::uint_fast32_t _line;
  std::uint_fast32_t _column;

  std::uint_fast32_t _sub_line;
  std::uint_fast32_t _sub_column;
  llvm::StringRef::iterator _sub_start;

  SourceRanger(llvm::StringRef text);

  [[nodiscard]]
  bool getIsDone() const;

  [[nodiscard]]
  const char &getChar(std::ptrdiff_t offset) const;

  [[nodiscard]]
  const char &getSubChar(std::ptrdiff_t offset) const;

  [[nodiscard]]
  const char &getPreviousChar(std::ptrdiff_t offset) const;

  [[nodiscard]]
  const char &getPreviousSubChar(std::ptrdiff_t offset) const;

  void incrementChar(std::ptrdiff_t offset);

  void startSubToken();

  requite::Token getSubToken(requite::TokenType type);

  requite::Token getLengthToken(requite::TokenType type,
                                std::uint_fast32_t length);

  void addLines(std::uint_fast32_t count);

  void addColumns(std::uint_fast32_t count);

  [[nodiscard]]
  bool getIsIdentifier() const;

  [[nodiscard]]
  bool getIsDecimalDigit() const;

  [[nodiscard]]
  bool getIsSymbol() const;

  [[nodiscard]]
  bool getIsNumeric() const;

  [[nodiscard]]
  bool getIsWhitespace() const;

  void skipWhitespace();
};

} // namespace requite