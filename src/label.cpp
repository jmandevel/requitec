// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attributes.hpp>
#include <requite/label.hpp>

namespace requite {

bool Label::operator==(const Self &rhs) const {
  return this == &rhs;
}

bool Label::operator!=(const Self &rhs) const {
  return this != &rhs;
}

bool Label::getHasName() const { return !this->_name.empty(); }

void Label::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Label::getName() const {
  REQUITE_ASSERT(this->_name.empty());
  return this->_name;
}

bool Label::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Label::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Label::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Label::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

} // namespace requite