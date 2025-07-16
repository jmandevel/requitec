// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/global.hpp>

namespace requite {

bool Global::operator==(const Self &rhs) const { return this == &rhs; }

bool Global::operator!=(const Self &rhs) const { return this != &rhs; }

bool Global::getHasName() const { return !this->_name.empty(); }

void Global::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

void Global::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
}

requite::AttributeFlags &Global::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &Global::getAttributeFlags() const {
  return this->_attributes;
}

llvm::StringRef Global::getName() const { return this->_name; }

bool Global::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Global::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Global::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Global::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &Global::getDataType() { return this->_data_type; }

const requite::Symbol &Global::getDataType() const { return this->_data_type; }

bool Global::getHasContaining() const {
  return this->_containing_ptr != nullptr;
}

void Global::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_ptr, scope);
}

requite::Scope &Global::getContaining() {
  return requite::getRef(this->_containing_ptr);
}

const requite::Scope &Global::getContaining() const {
  return requite::getRef(this->_containing_ptr);
}

bool Global::getHasNext() const { return this->_next_ptr != nullptr; }

void Global::setNext(requite::Global &global) {
  requite::setSingleRef(this->_next_ptr, global);
}

requite::Global &Global::getNext() { return requite::getRef(this->_next_ptr); }

const requite::Global &Global::getNext() const {
  return requite::getRef(this->_next_ptr);
}

} // namespace requite