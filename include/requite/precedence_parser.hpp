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
  requite::Expression *_outer_ptr = nullptr;
  requite::Expression *_operation_ptr = nullptr;
  requite::Expression *_last_ptr = nullptr;

  void parseUnary(requite::Parser &parser, requite::Opcode opcode);
  void parseBinary(requite::Parser &parser, requite::Opcode opcode);
  void parseBinaryNesting(requite::Parser &parser, requite::Opcode opcode);
  void parseBinaryCombination(requite::Parser &parser, requite::Opcode opcode);
  void parseNary(requite::Parser &parser, requite::Opcode opcode);
  void parseNestedNary(requite::Parser &parser,
                                 requite::Opcode opcode);
  void parseAttribute(requite::Parser &parser, requite::Opcode opcode);
  void parseHorned(requite::Parser &parser, requite::Opcode opcode,
                   requite::TokenType right_token);
  void appendBranch(requite::Expression &branch);
  [[nodiscard]] bool getHasOuter() const;
  [[nodiscard]] const requite::Expression &getOuter() const;
  [[nodiscard]] requite::Expression &getOuter();
  [[nodiscard]] bool getHasOperation() const;
  [[nodiscard]] const requite::Expression &getOperation() const;
  [[nodiscard]] requite::Expression &getOperation();
  [[nodiscard]] bool getHasLast() const;
  [[nodiscard]] const requite::Expression &getLast() const;
  [[nodiscard]] requite::Expression &getLast();
  [[nodiscard]] bool getHasUnary() const;
  [[nodiscard]] const requite::Expression &getUnary() const;
  [[nodiscard]] requite::Expression &getUnary();
};

} // namespace requite