// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

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

llvm::StringMap<requite::RootSymbol> &Table::getSymbolMap() {
  return this->_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &Table::getSymbolMap() const {
  return this->_symbol_map;
}

bool Table::getIsEmpty() const { return this->_symbol_map.empty(); }

} // namespace requite