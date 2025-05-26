// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/context.hpp>
#include <requite/precedence_parser.hpp>
#include <requite/module.hpp>
#include <requite/token.hpp>
#include <requite/token_type.hpp>

#include <llvm/ADT/SmallVector.h>

#include <array>
#include <functional>
#include <vector>
#include <string>

namespace requite {

struct Parser final {
  std::reference_wrapper<requite::Module> _module_ref;
  std::reference_wrapper<requite::Context> _context_ref;
  std::vector<requite::Token>::const_iterator _it;
  std::vector<requite::Token>::const_iterator _end;
  bool _is_ok;

  Parser(requite::Context &context, requite::Module &module, std::vector<requite::Token>& tokens);

  [[nodiscard]]
  bool getIsOk();
  void setNotOk();

  void logErrorBinaryNoLValue(const requite::Token &token);

  void logErrorHornedNoFirstBranch(const requite::Token &token);

  void logErrorFoundErrorToken(const requite::Token &token);

  void logErrorUnexpectedToken(const requite::Token &token);

  void logErrorInvalidOperatorSpacing(const requite::Token &token);

  [[nodiscard]]
  std::string getText(llvm::StringRef log_message_type_text,
                          const requite::Token &token,
                          llvm::StringRef source_text);

  [[nodiscard]]
  requite::Module &getModule();

  [[nodiscard]]
  const requite::Module &getModule() const;

  [[nodiscard]]
  requite::Context &getContext();

  [[nodiscard]]
  const requite::Context &getContext() const;

  [[nodiscard]]
  bool getIsDone() const;
  [[nodiscard]]
  const requite::Token &getToken() const;

  [[nodiscard]]
  const requite::Token &getPreviousToken() const;

  [[nodiscard]]
  const requite::Token &getNextToken();

  void incrementToken(std::size_t offset);

  [[nodiscard]]
  bool getIsToken(requite::TokenType type) const;

  [[nodiscard]]
  bool parseExpressions();

  [[nodiscard]]
  requite::Expression &parseExpression();

  [[nodiscard]]
  requite::Expression &parsePrecedence11();

  [[nodiscard]]
  requite::Expression &parsePrecedence10();

  [[nodiscard]]
  requite::Expression &parsePrecedence9();

  [[nodiscard]]
  requite::Expression &parsePrecedence8();

  [[nodiscard]]
  requite::Expression &parsePrecedence7();

  [[nodiscard]]
  requite::Expression &parsePrecedence6();

  [[nodiscard]]
  requite::Expression &parsePrecedence5();

  [[nodiscard]]
  requite::Expression &parsePrecedence4();

  [[nodiscard]]
  requite::Expression &parsePrecedence3();

  [[nodiscard]]
  requite::Expression &parsePrecedence2();

  [[nodiscard]]
  requite::Expression &parsePrecedence1();

  [[nodiscard]]
  requite::Expression &parsePrecedence0();

  [[nodiscard]]
  requite::Expression *parseBranches(const requite::Token &left_token,
                                     requite::TokenType end);

  [[nodiscard]]
  requite::Expression *
  parseOperationBranches(const requite::Token &left_token,
                         const requite::Token &opcode_token);

  [[nodiscard]]
  requite::Expression &parseMacroBranches(const requite::Token &left_token);

  [[nodiscard]]
  requite::Opcode parseOpcode();

  [[nodiscard]]
  requite::Expression &parseBracketExpression();

  // branch0(branch1 branch2)
  // when this is called, branch0 was already parsed.
  [[nodiscard]]
  requite::Expression &parseHorned(requite::Expression &first,
                                   requite::Opcode opcode,
                                   requite::TokenType end);

  // (branch0 branch1 branch2)
  [[nodiscard]]
  requite::Expression &parseCloven(requite::Opcode opcode,
                                   requite::TokenType end);

  [[nodiscard]]
  requite::Expression &parsePostUnary(requite::Expression &first,
                                      requite::Opcode opcode);

  [[nodiscard]]
  requite::Expression &parseIdentifierLiteral();

  [[nodiscard]]
  requite::Expression &parseIdentify();

  [[nodiscard]]
  requite::Expression &parseSituationalInferenceOrIndeterminate();

  [[nodiscard]]
  requite::Expression &parseIntegerLiteral();

  [[nodiscard]]
  requite::Expression &parseRealLiteral();

  [[nodiscard]]
  requite::Expression &parseStringLiteral();

  [[nodiscard]]
  requite::Expression &parseCodeunitLiteral();

  [[nodiscard]]
  requite::Expression &parseInterpolatedString();
};

} // namespace requite