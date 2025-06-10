// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

namespace requite {

struct Expression;
struct Parser;

struct PrecedenceParser final {

  requite::Expression *_outer_ptr = nullptr;
  requite::Expression *_last_branch_ptr = nullptr;

  [[nodiscard]]
  requite::Expression &parseUnary(requite::Parser &parser,
                                  requite::Opcode opcode,
                                  requite::Expression *unary_ptr);

  void parseBinary(requite::Parser &parser, requite::Opcode opcode);

  void parseNary(requite::Parser &parser, requite::Opcode opcode);

  void parseAttribute(requite::Parser &parser, requite::Opcode opcode,
                      bool new_ascribe_needed, requite::Expression *unary_ptr);

  void setInitialOuter(requite::Expression &outer);

  void appendBranch(requite::Expression &branch);

  [[nodiscard]]
  bool getHasOuter() const;

  [[nodiscard]]
  requite::Opcode getOuterOpcode() const;

  [[nodiscard]]
  const requite::Expression& getOuter() const;

  [[nodiscard]]
  requite::Expression& getOuter();
};

} // namespace requite