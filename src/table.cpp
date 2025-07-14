// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

Table::Table() { this->getScope().setTable(*this); }

bool Table::operator==(const Self &rhs) const { return this == &rhs; }

bool Table::operator!=(const Self &rhs) const { return this != &rhs; }

bool Table::getHasName() const { return !this->_name.empty(); }

void Table::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Table::getName() const {
  REQUITE_ASSERT(!this->_name.empty());
  return this->_name;
}

requite::Scope &Table::getScope() { return this->_scope; }

const requite::Scope &Table::getScope() const { return this->_scope; }

bool Table::getIsEmpty() const { return this->getScope().getIsEmpty(); }

bool Table::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Table::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Table::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Table::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool Table::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void Table::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &Table::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &Table::getContaining() const {
  return this->getScope().getContaining();
}

} // namespace requite