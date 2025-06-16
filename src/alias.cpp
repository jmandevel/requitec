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

void Alias::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
}

requite::AttributeFlags &Alias::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &Alias::getAttributeFlags() const {
  return this->_attributes;
}

bool Alias::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void Alias::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Alias::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Alias::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

requite::Symbol &Alias::getSymbol() { return this->_symbol; }

const requite::Symbol &Alias::getSymbol() const { return this->_symbol; }

} // namespace requite
