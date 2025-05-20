// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/anonymous_object.hpp>
#include <requite/assert.hpp>
#include <requite/symbol.hpp>

namespace requite {

AnonymousProperty::AnonymousProperty(llvm::StringRef name,
                                     const requite::Symbol &symbol)
    : _name(name), _symbol(symbol) {}

bool AnonymousProperty::operator==(const Self &rhs) const {
  return this->getName() == rhs.getName() &&
         this->getSymbol() == rhs.getSymbol();
}

bool AnonymousProperty::operator!=(const Self &rhs) const {
  return !(*this == rhs);
}

bool AnonymousProperty::getHasName() const { return !this->_name.empty(); }

void AnonymousProperty::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef AnonymousProperty::getName() const { return this->_name; }

requite::Symbol &AnonymousProperty::getSymbol() { return this->_symbol; }

const requite::Symbol &AnonymousProperty::getSymbol() const {
  return this->_symbol;
}

bool AnonymousProperty::getHasDefaultValueExpression() const {
  return this->_default_value_expression_ptr != nullptr;
}

void AnonymousProperty::setDefaultValueExpression(
    requite::Expression &expression) {
  requite::setSingleRef(this->_default_value_expression_ptr, expression);
}

requite::Expression &AnonymousProperty::getDefaultValueExpression() {
  return requite::getRef(this->_default_value_expression_ptr);
}

const requite::Expression &
AnonymousProperty::getDefaultValueExpression() const {
  return requite::getRef(this->_default_value_expression_ptr);
}

} // namespace requite