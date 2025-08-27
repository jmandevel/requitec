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
  PrecedenceParser() = default;
  PrecedenceParser(const Self &) = delete;
  PrecedenceParser(Self &&) = delete;
  ~PrecedenceParser() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  // parse.cpp
  void parseDoubleUnary(const requite::Token& token, requite::Opcode opcode);
  void parseUnary(const requite::Token& token, requite::Opcode opcode);
  void parseAscribe(const requite::Token &token, requite::Opcode opcode);
  void parseBinary(const requite::Token& token, requite::Opcode opcode);
  void parseNary(const requite::Token &token, requite::Opcode opcode);
  void parseNestingNary(const requite::Token &token, requite::Opcode opcode);
  void parseShortRangeBranch(const requite::Token &token,
                             requite::Opcode opcode,
                             requite::Expression &rvalue);
  void appendBranch(requite::Expression &branch);
  void appendUnaryAttribute(const requite::Token &token, requite::Opcode opcode);
  void setRecent(requite::Expression &branch);
  void setOnlyRecent(requite::Expression &branch);
  void appendRecent();
  void setOuterOperation(requite::Expression &expression);
  [[nodiscard]] const requite::Expression &getOuter() const;
  [[nodiscard]] bool getHasOuter() const;
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