// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline bool Expression::getHasDataText() const {
  return requite::getHasTextData(this->getOpcode()) && !this->_data._text.getString().empty();
}

inline requite::SavedString Expression::getDataText() const {
  REQUITE_ASSERT(this->getHasDataText());
  return this->_data._text;
}

inline void Expression::setDataText(requite::SavedString text) {
  REQUITE_ASSERT(!this->getHasDataText());
  this->_data._text = text;  
}

inline void Expression::changeDataText(requite::SavedString text) {
  REQUITE_ASSERT(this->getHasDataText());
  this->_data._text = text;
}

} // namespace requite