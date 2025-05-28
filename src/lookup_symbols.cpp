// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/scope.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

requite::RootSymbol Scope::lookupInternalRootSymbol(llvm::StringRef name) {
  llvm::StringMapIterator<requite::RootSymbol> it =
      this->_symbol_map.find(name);
  if (it != this->_symbol_map.end()) {
    return requite::RootSymbol(it->second);
  }
  return requite::RootSymbol();
}

} // namespace requite