// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/variable.hpp>

namespace requite {

bool Variable::getHasName() const { return !this->_name.empty(); }

void Variable::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

void Variable::setType(requite::VariableType type) {
  REQUITE_ASSERT(this->_type == requite::VariableType::NONE);
  this->_type = type;
}

requite::VariableType Variable::getType() const { return this->_type; }

llvm::StringRef Variable::getName() const { return this->_name; }

void Variable::setAttributes(requite::Attributes attributes) {
  this->_attributes = attributes;
}

requite::Attributes &Variable::getAttributes() {
  return this->_attributes;
}

const requite::Attributes &Variable::getAttributes() const {
  return this->_attributes;
}

bool Variable::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Variable::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Variable::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Variable::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &Variable::getDataType() { return this->_data_type; }

const requite::Symbol &Variable::getDataType() const {
  return this->_data_type;
}

bool Variable::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void Variable::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Variable::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Variable::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

} // namespace requite