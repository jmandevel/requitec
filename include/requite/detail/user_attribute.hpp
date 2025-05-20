// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>

namespace requite {

bool UserAttribute::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

requite::Expression &UserAttribute::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &UserAttribute::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

void UserAttribute::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

bool UserAttribute::getHasNext() const {
  return this->_next_user_attribute_ptr != nullptr;
}

requite::UserAttribute &UserAttribute::getNext() {
  return requite::getRef(this->_next_user_attribute_ptr);
}

const requite::UserAttribute &UserAttribute::getNext() const {
  return requite::getRef(this->_next_user_attribute_ptr);
}

} // namespace requite