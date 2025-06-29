// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/ordered_variable.hpp>

namespace requite {

bool OrderedVariable::getHasName() const { return !this->_name.empty(); }

void OrderedVariable::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

void OrderedVariable::setType(requite::VariableType type) {
  REQUITE_ASSERT(this->_type == requite::VariableType::NONE);
  this->_type = type;
}

requite::VariableType OrderedVariable::getType() const { return this->_type; }

llvm::StringRef OrderedVariable::getName() const { return this->_name; }

bool OrderedVariable::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void OrderedVariable::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &OrderedVariable::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &OrderedVariable::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &OrderedVariable::getDataType() { return this->_data_type; }

const requite::Symbol &OrderedVariable::getDataType() const {
  return this->_data_type;
}

bool OrderedVariable::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void OrderedVariable::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &OrderedVariable::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &OrderedVariable::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

void OrderedVariable::setLlvmAllocaPtr(llvm::AllocaInst *llvm_alloca) {
  this->_llvm_alloca_ptr = llvm_alloca;
}

llvm::AllocaInst *OrderedVariable::getLlvmAllocaPtr() {
  return this->_llvm_alloca_ptr;
}

const llvm::AllocaInst *OrderedVariable::getLlvmAllocaPtr() const {
  return this->_llvm_alloca_ptr;
}

} // namespace requite