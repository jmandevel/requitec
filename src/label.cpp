// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attribute_flags.hpp>
#include <requite/label.hpp>

namespace requite {

bool Label::operator==(const Self &rhs) const { return this == &rhs; }

bool Label::operator!=(const Self &rhs) const { return this != &rhs; }

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

void Label::setHasDependentName() {
  REQUITE_ASSERT(this->_dependent_name == false);
  this->_dependent_name = true;
}

bool Label::getHasDependentName() const { return this->_dependent_name; }

bool Label::getHasAttributeExpression() const {
  return this->_attribute_expression_ptr != nullptr;
}

void Label::setAttributeExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_attribute_expression_ptr, expression);
}

requite::Expression &Label::getAttributeExpression() {
  return requite::getRef(this->_attribute_expression_ptr);
}

const requite::Expression &Label::getAttributeExpression() const {
  return requite::getRef(this->_attribute_expression_ptr);
}

bool Label::getHasStatementExpression() const {
  return this->_statement_expression_ptr != nullptr;
}

void Label::setStatementExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_statement_expression_ptr, expression);
}

requite::Expression &Label::getStatementExpression() {
  return requite::getRef(this->_statement_expression_ptr);
}

const requite::Expression &Label::getStatementExpression() const {
  return requite::getRef(this->_statement_expression_ptr);
}

bool Label::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void Label::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Label::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Label::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

} // namespace requite