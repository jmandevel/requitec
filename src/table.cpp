// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

bool Table::getHasName() const { return !this->_name.empty(); }

void Table::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Table::getName() const { return this->_name; }

requite::Scope &Table::getScope() { return this->_scope; }

const requite::Scope &Table::getScope() const { return this->_scope; }

bool Table::getHasContainingScope() const {
  return this->getScope().getHasContainingScope();
}

void Table::setContainingScope(requite::Scope &scope) {
  this->getScope().setContainingScope(scope);
}

requite::Scope &Table::getContainingScope() {
  return this->getScope().getContainingScope();
}

const requite::Scope &Table::getContainingScope() const {
  return this->getScope().getContainingScope();
}

bool Table::getHasExpression() const {
  return this->getScope().getHasExpression();
}

void Table::setExpression(requite::Expression &expression) {
  this->getScope().setExpression(expression);
}

requite::Expression &Table::getExpression() {
  return this->getScope().getExpression();
}

const requite::Expression &Table::getExpression() const {
  return this->getScope().getExpression();
}

} // namespace requite