// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>
#include <requite/scope.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

requite::RootSymbol Table::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapIterator<requite::RootSymbol> it = this->getSymbolMap().find(name);
  if (it == this->getSymbolMap().end()) {
    return requite::RootSymbol();
  }
  return it->second;
}

inline bool Table::getHasUserSymbolOfName(llvm::StringRef name) const {
  return this->getSymbolMap().contains(name);
}

requite::RootSymbol Scope::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  requite::Table &table = this->getTable();
  return table.lookupUserSymbol(name);
}

bool Scope::getHasUserSymbolOfName(llvm::StringRef name) const {
    return this->getTable().getHasUserSymbolOfName(name);
}

} // namespace requite