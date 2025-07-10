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

llvm::StringRef Table::getName() const { return this->_name; }

requite::SymbolStatus Table::getSymbolStatus() const {
  return this->_symbol_status;
}

void Table::incrementSymbolStatus() {
  REQUITE_ASSERT(this->_symbol_status != requite::SymbolStatus::DONE);
  this->_symbol_status = static_cast<requite::SymbolStatus>(
      static_cast<unsigned>(this->_symbol_status) + 1);
}

} // namespace requite