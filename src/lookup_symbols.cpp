// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/symbol_table.hpp>
#include <requite/scope.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

requite::RootSymbol SymbolTable::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapIterator<requite::RootSymbol> it = this->getSymbolMap().find(name);
  if (it == this->getSymbolMap().end()) {
    return requite::RootSymbol();
  }
  return it->second;
}

inline bool SymbolTable::getHasUserSymbolOfName(llvm::StringRef name) const {
  return this->getSymbolMap().contains(name);
}

requite::RootSymbol Scope::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  requite::SymbolTable &table = this->getSymbolTable();
  return table.lookupUserSymbol(name);
}

bool Scope::getHasUserSymbolOfName(llvm::StringRef name) const {
    return this->getSymbolTable().getHasUserSymbolOfName(name);
}

} // namespace requite