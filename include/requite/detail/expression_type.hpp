// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/situation.hpp>

namespace requite {

inline bool Expression::getIsNone() const {
  return this->getOpcode() == requite::Opcode::__NONE;
}

inline bool Expression::getIsOperation() const {
  return static_cast<std::uint32_t>(this->getOpcode()) >
         static_cast<std::uint32_t>(requite::Opcode::__IDENTIFIER_LITERAL);
}

inline bool Expression::getIsInteger() const {
  return this->getOpcode() == requite::Opcode::__INTEGER_LITERAL;
}

inline bool Expression::getIsFractional() const {
  return this->getOpcode() == requite::Opcode::__FRACTIONAL_LITERAL;
}

inline bool Expression::getIsNumeric() const {
  return this->getIsInteger() || this->getIsFractional();
}

inline bool Expression::getIsString() const {
  return this->getOpcode() == requite::Opcode::__STRING_LITERAL;
}

inline bool Expression::getIsCodeunit() const {
  return this->getOpcode() == requite::Opcode::__CODEUNIT_LITERAL;
}

inline bool Expression::getIsIdentifier() const {
  return this->getOpcode() == requite::Opcode::__IDENTIFIER_LITERAL;
}

inline bool Expression::getIsConverging() const {
  return requite::getIsConverging(this->getOpcode());
}

inline bool Expression::getIsInternalUseOnly() const {
  return requite::getIsInternalUseOnly(this->getOpcode());
}

inline bool Expression::getCanHaveNoSemicolon() const {
  if (this->getOpcode() == requite::Opcode::_ASCRIBE_LAST_BRANCH) {
    return this->getLastBranch().getCanHaveNoSemicolon();
  }
  else if (this->getOpcode() == requite::Opcode::_ASCRIBE_FIRST_BRANCH) {
    return this->getBranch().getCanHaveNoSemicolon();
  }
  return requite::getBranchCanHaveNoSemicolon(this->getOpcode());
}

inline requite::Opcode Expression::getOpcode() const { return this->_opcode; }

inline void Expression::setOpcode(requite::Opcode opcode) {
  REQUITE_ASSERT(this->_opcode == requite::Opcode::__NONE);
  this->_opcode = opcode;
}

inline void Expression::changeOpcode(requite::Opcode opcode) {
  REQUITE_ASSERT(this->_opcode != requite::Opcode::__NONE);
  this->_opcode = opcode;
}

} // namespace requite