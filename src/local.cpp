// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/local.hpp>

namespace requite {

bool Local::operator==(const Self &rhs) const { return this == &rhs; }

bool Local::operator!=(const Self &rhs) const { return this != &rhs; }

bool Local::getHasName() const { return !this->_name.empty(); }

void Local::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

llvm::StringRef Local::getName() const { return this->_name; }

bool Local::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Local::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Local::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Local::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &Local::getDataType() { return this->_data_type; }

const requite::Symbol &Local::getDataType() const { return this->_data_type; }

bool Local::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void Local::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Local::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Local::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

bool Local::getHasLlvmAllocaPtr() const {
  return this->_llvm_alloca_ptr != nullptr;
}

void Local::setLlvmAllocaPtr(llvm::AllocaInst *llvm_alloca) {
  this->_llvm_alloca_ptr = llvm_alloca;
}

llvm::AllocaInst *Local::getLlvmAllocaPtr() { return this->_llvm_alloca_ptr; }

const llvm::AllocaInst *Local::getLlvmAllocaPtr() const {
  return this->_llvm_alloca_ptr;
}

bool Local::getHasNext() const { return this->_next_ptr != nullptr; }

void Local::setNext(requite::Local &local) {
  requite::setSingleRef(this->_next_ptr, local);
}

requite::Local &Local::getNext() { return requite::getRef(this->_next_ptr); }

const requite::Local &Local::getNext() const {
  return requite::getRef(this->_next_ptr);
}

} // namespace requite