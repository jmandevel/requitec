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

bool Table::getHasSubScopes() const {
  return this->_first_scope_ptr != nullptr;
}

void Table::addSubScope(requite::Scope &scope) {
  scope._next_ptr = this->_first_scope_ptr;
  this->_first_scope_ptr = &scope;
}

requite::Scope &Table::getFirstScope() {
  return requite::getRef(this->_first_scope_ptr);
}

const requite::Scope &Table::getFirstScope() const {
  return requite::getRef(this->_first_scope_ptr);
}

} // namespace requite