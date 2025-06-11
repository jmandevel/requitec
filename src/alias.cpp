// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/assert.hpp>
#include <requite/expression.hpp>

namespace requite {

void Alias::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Alias::getName() const { return this->_name; }

bool Alias::getHasName() const { return !this->_name.empty(); }

void Alias::setHasDependentName() {
  REQUITE_ASSERT(this->_dependent_name == false);
  this->_dependent_name = true;
}

bool Alias::getHasDependentName() const { return this->_dependent_name; }

void Alias::setExpression(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Alias::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Alias::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

void Alias::setAttributes(requite::Attributes attributes) {
  this->_attributes = attributes;
}

requite::Attributes &Alias::getAttributes() {
  return this->_attributes;
}

const requite::Attributes &Alias::getAttributes() const {
  return this->_attributes;
}

bool Alias::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void Alias::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Alias::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Alias::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

requite::Symbol &Alias::getSymbol() { return this->_symbol; }

const requite::Symbol &Alias::getSymbol() const { return this->_symbol; }

} // namespace requite
