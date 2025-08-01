// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>
#include <requite/token_type.hpp>

#include <functional>

namespace requite {

struct Expression;
struct Parser;
struct Token;

struct PrecedenceParser final {
  using Self = requite::PrecedenceParser;

  std::reference_wrapper<requite::Parser> _parser_ref;
  // the outermost operation that is returned at the end of the precedence
  requite::Expression *_outer_ptr = nullptr;
  // the current operation that is being filled with branches
  requite::Expression *_operation_ptr = nullptr;
  // the most recent parsed branch, which is not yet appended because it might
  // need to nest inside the next operation
  requite::Expression *_recent_ptr = nullptr;
  // the last branch that was appended to the operation
  requite::Expression *_last_ptr = nullptr;

  // precedence_parser.cpp
  PrecedenceParser(requite::Parser& parser);
  PrecedenceParser(const Self &) = delete;
  PrecedenceParser(Self &&) = delete;
  ~PrecedenceParser() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Parser& getParser();
  [[nodiscard]] const requite::Parser& getParser() const;

  // parse.cpp
  void parseDoubleUnary(requite::Opcode opcode);
  void parseUnary(requite::Opcode opcode);
  void parseBinary(requite::Opcode opcode);
  void parseBinaryCombination(requite::Opcode opcode);
  void parseNary(requite::Opcode opcode);
  void parseNestingNary(requite::Opcode opcode);
  void parseShortRangeBranch(const requite::Token &token, requite::Opcode opcode);
  void parseAttribute();
  void parseUnaryAttribute(requite::Opcode opcode);
  void parseAscribe(const requite::Token& token);
  void parseCallOrSignature();
  void parseCallOrSignatureImplicitCallee();
  void parseSpecialization();
  void appendBranch(requite::Expression &branch);
  void setRecent(requite::Expression &branch);
  void appendRecent();
  [[nodiscard]] bool getHasOuter() const;
  [[nodiscard]] const requite::Expression &getOuter() const;
  [[nodiscard]] requite::Expression &getOuter();
  [[nodiscard]] bool getHasOperation() const;
  [[nodiscard]] const requite::Expression &getOperation() const;
  [[nodiscard]] requite::Expression &getOperation();
  [[nodiscard]] bool getHasRecent() const;
  [[nodiscard]] const requite::Expression &getRecent() const;
  [[nodiscard]] requite::Expression &getRecent();
  [[nodiscard]] bool getHasLast() const;
  [[nodiscard]] const requite::Expression &getLast() const;
  [[nodiscard]] requite::Expression &getLast();
  [[nodiscard]] bool getHasUnary() const;
  [[nodiscard]] const requite::Expression &getUnary() const;
  [[nodiscard]] requite::Expression &getUnary();
};

} // namespace requite