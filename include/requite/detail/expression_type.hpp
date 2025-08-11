// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/situation.hpp>

namespace requite {

inline bool Expression::getIsConverging() const {
  return requite::getIsConverging(this->getOpcode());
}

inline bool Expression::getIsInternal() const {
  return requite::getIsInternal(this->getOpcode());
}

inline bool Expression::getCanHaveNoSemicolon() const {
  if (this->getOpcode() == requite::Opcode::_ASCRIBE) {
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