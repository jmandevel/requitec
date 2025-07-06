// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/codeunits.hpp>
#include <requite/grouping.hpp>
#include <requite/grouping_type.hpp>
#include <requite/source_ranger.hpp>
#include <requite/unreachable.hpp>

#include <llvm/ADT/SmallVector.h>

#include <functional>

namespace requite {

struct Context;
struct File;

struct Tokenizer final {
  using Self = requite::Tokenizer;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<std::vector<requite::Token>> _tokens_ref;
  llvm::SmallVector<requite::Grouping, 128> _grouping_stack;
  requite::SourceRanger _ranger;
  bool _is_ok = true;

  // tokenizer.cpp
  Tokenizer(requite::Context &context, requite::File &file, std::vector<requite::Token> &tokens);
  Tokenizer(const Self&) = delete;
  Tokenizer(Self&&) = delete;
  ~Tokenizer() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]]
  bool getIsOk() const;
  void setNotOk();
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  requite::SourceRanger &getRanger();
  [[nodiscard]]
  const requite::SourceRanger &getRanger() const;
  [[nodiscard]]
  std::vector<requite::Token> &getTokens();
  [[nodiscard]]
  const std::vector<requite::Token> &getTokens() const;
  [[nodiscard]]
  bool getHasGrouping() const;
  [[nodiscard]]
  const requite::Grouping &getTopGrouping() const;
  void pushGrouping(requite::GroupingType grouping);
  void popGrouping();

  // tokenize_tokens.cpp
  void _tokenizeTokens();
  [[nodiscard]]
  bool tokenizeTokens();
  void tokenizeLengthToken(requite::TokenType type, unsigned length);
  void tokenizeUnmatchedLengthToken(requite::TokenType type, unsigned length);
  void tokenizeRightGrouping(requite::GroupingType grouping,
                             requite::TokenType type, unsigned length);
  void checkFinalGroupings();
  void logErrorUnmatchedRightToken(const requite::Token &token);

  // detail/tokenize_tokens.hpp
  template <bool CAN_HAVE_INTERPOLATION_PARAM, char END_QUOTE_PARAM,
            requite::TokenType TYPE_PARAM,
            requite::TokenType ERROR_UNTERMINATED_PARAM>
  void tokenizeQuotedLiteral();
};

} // namespace requite

#include <requite/detail/tokenize_tokens.hpp>