// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>
#include <requite/token_type.hpp>

namespace requite {

struct Expression;
struct Parser;

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

  PrecedenceParser() = default;
  PrecedenceParser(const Self &) = delete;
  PrecedenceParser(Self &&) = delete;
  ~PrecedenceParser() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  // parse.cpp
  void parseDoubleUnary(requite::Parser &parser, requite::Opcode opcode);
  void parseUnary(requite::Parser &parser, requite::Opcode opcode);
  void parseBinary(requite::Parser &parser, requite::Opcode opcode);
  void parseBinaryCombination(requite::Parser &parser, requite::Opcode opcode);
  void parseNary(requite::Parser &parser, requite::Opcode opcode);
  void parseNestedNary(requite::Parser &parser, requite::Opcode opcode);
  void parseAttribute(requite::Parser &parser, requite::Opcode opcode);
  void parseHorned(requite::Parser &parser, requite::Opcode opcode,
                   requite::TokenType right_type);
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