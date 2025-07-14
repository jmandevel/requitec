// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/scope.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

requite::LookupTableEntry &Scope::lookupUserSymbol(llvm::StringRef name) {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapIterator<requite::LookupTableEntry> it = this->_symbol_map.find(name);
  REQUITE_ASSERT (it != this->_symbol_map.end());
  return it->second;
}

bool Scope::getHasUserSymbolOfName(llvm::StringRef name) const {
  return this->_symbol_map.contains(name);
}

} // namespace requite