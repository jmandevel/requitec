// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/unordered_variable.hpp>

namespace requite {

bool UnorderedVariable::getHasName() const { return !this->_name.empty(); }

void UnorderedVariable::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

void UnorderedVariable::setType(requite::VariableType type) {
  REQUITE_ASSERT(this->_type == requite::VariableType::NONE);
  this->_type = type;
}

requite::VariableType UnorderedVariable::getType() const { return this->_type; }

void UnorderedVariable::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
}

requite::AttributeFlags &UnorderedVariable::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &UnorderedVariable::getAttributeFlags() const {
  return this->_attributes;
}

llvm::StringRef UnorderedVariable::getName() const { return this->_name; }

bool UnorderedVariable::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void UnorderedVariable::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &UnorderedVariable::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &UnorderedVariable::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &UnorderedVariable::getDataType() { return this->_data_type; }

const requite::Symbol &UnorderedVariable::getDataType() const {
  return this->_data_type;
}

requite::Scope &UnorderedVariable::getScope() { return this->_scope; }

const requite::Scope &UnorderedVariable::getScope() const {
  return this->_scope;
}

bool UnorderedVariable::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void UnorderedVariable::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &UnorderedVariable::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &UnorderedVariable::getContaining() const {
  return this->getScope().getContaining();
}

} // namespace requite