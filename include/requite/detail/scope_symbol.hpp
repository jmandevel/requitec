// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>

namespace requite {

inline bool Scope::getHasSymbolOfName(llvm::StringRef name) {
  llvm::StringMapIterator<requite::RootSymbol> it =
      this->_symbol_map.find(name);
  if (it != this->_symbol_map.end()) {
    return true;
  }
  return false;
}

template <typename SymbolArg> void Scope::addSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getHasName());
  REQUITE_ASSERT(!this->getHasSymbolOfName(symbol.getName()));
  this->_symbol_map.insert(std::pair<llvm::StringRef, requite::RootSymbol>(
      symbol.getName(), requite::RootSymbol::makeUser(symbol)));
}

} // namespace requite