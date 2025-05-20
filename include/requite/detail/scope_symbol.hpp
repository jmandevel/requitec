// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>

namespace requite {

template <typename SymbolArg> void Scope::addSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getHasName());
  REQUITE_ASSERT(this->lookupInternalRootSymbol(symbol.getName()).getIsNone());
  this->_symbol_map.insert(std::pair<llvm::StringRef, requite::RootSymbol>(
      symbol.getName(), requite::RootSymbol::makeUser(symbol)));
}

} // namespace requite