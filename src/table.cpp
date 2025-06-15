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

} // namespace requite