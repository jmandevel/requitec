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

void Property::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
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

bool Property::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void Property::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &Property::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &Property::getContaining() const {
  return this->getScope().getContaining();
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

requite::SymbolStatus Property::getSymbolStatus() const {
  return this->_symbol_status;
}

void Property::incrementSymbolStatus() {
  REQUITE_ASSERT(this->_symbol_status != requite::SymbolStatus::DONE);
  this->_symbol_status = static_cast<requite::SymbolStatus>(
      static_cast<unsigned>(this->_symbol_status) + 1);
}

} // namespace requite