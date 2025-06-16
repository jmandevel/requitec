// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

Table::Table() { this->_scope.setTable(*this); }

bool Table::getHasName() const { return !this->_name.empty(); }

void Table::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Table::getName() const { return this->_name; }

requite::Scope &Table::getScope() { return this->_scope; }

const requite::Scope &Table::getScope() const { return this->_scope; }

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

unsigned Table::getScopeDepth() const {
  return this->getScope().getScopeDepth();
}

requite::Scope *Table::getContainingPtr() {
  return this->getScope().getContainingPtr();
}

const requite::Scope *Table::getContainingPtr() const {
  return this->getScope().getContainingPtr();
}

} // namespace requite