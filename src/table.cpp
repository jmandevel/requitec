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

} // namespace requite