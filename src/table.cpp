// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

Table::Table() {
  this->getStubScope().setType(requite::ScopeType::TABLE);
  this->getStubScope().setTable(*this);
}

bool Table::getHasName() const { return !this->_name.empty(); }

void Table::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Table::getName() const { return this->_name; }

requite::Scope &Table::getStubScope() { return this->_stub_scope; }

const requite::Scope &Table::getStubScope() const { return this->_stub_scope; }

void Table::addScope(requite::Scope& scope) {
  REQUITE_ASSERT(scope.getType() == requite::ScopeType::TABLE);
  requite::Scope* previous_next_ptr = this->getStubScope().getNextPtr();
  this->_stub_scope._next_ptr = &scope;
  REQUITE_ASSERT(!scope.getHasNext());
  scope._next_ptr = previous_next_ptr;
}

} // namespace requite