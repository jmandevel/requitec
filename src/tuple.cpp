// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/tuple.hpp>

namespace requite {

bool Tuple::operator==(const requite::Tuple &rhs) const {
  return this->_element_types == rhs._element_types;
}

bool Tuple::operator!=(const requite::Tuple &rhs) const {
  return this->_element_types != rhs._element_types;
}

std::vector<requite::Symbol> &Tuple::getElementTypes() {
  return this->_element_types;
}

const std::vector<requite::Symbol> &Tuple::getElementTypes() const {
  return this->_element_types;
}

} // namespace requite