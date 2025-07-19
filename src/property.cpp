// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/property.hpp>

namespace requite {

bool Property::operator==(const Self &rhs) const { return this == &rhs; }

bool Property::operator!=(const Self &rhs) const { return this != &rhs; }

bool Property::getHasName() const { return !this->_name.empty(); }

void Property::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

requite::AttributeFlags &Property::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &Property::getAttributeFlags() const {
  return this->_attributes;
}

llvm::StringRef Property::getName() const { return this->_name; }

bool Property::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Property::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Property::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Property::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Symbol &Property::getDataType() { return this->_data_type; }

const requite::Symbol &Property::getDataType() const {
  return this->_data_type;
}

requite::Scope &Property::getScope() { return this->_scope; }

const requite::Scope &Property::getScope() const { return this->_scope; }

bool Property::getHasContainingScope() const {
  return this->getScope().getHasContainingScope();
}

void Property::setContainingScope(requite::Scope &scope) {
  this->getScope().setContainingScope(scope);
}

requite::Scope &Property::getContainingScope() {
  return this->getScope().getContainingScope();
}

const requite::Scope &Property::getContainingScope() const {
  return this->getScope().getContainingScope();
}

bool Property::getHasNext() const { return this->_next_ptr != nullptr; }

void Property::setNext(requite::Property &property) {
  requite::setSingleRef(this->_next_ptr, property);
}

requite::Property &Property::getNext() {
  return requite::getRef(this->_next_ptr);
}

const requite::Property &Property::getNext() const {
  return requite::getRef(this->_next_ptr);
}

} // namespace requite