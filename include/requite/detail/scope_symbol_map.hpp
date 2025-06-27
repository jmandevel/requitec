// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/export_table.hpp>

namespace requite {

inline bool Scope::getHasInternalSymbolOfName(llvm::StringRef name) const {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapConstIterator<requite::RootSymbol> it =
      this->getInternalSymbolMap().find(name);
  if (it != this->_internal_symbol_map.end()) {
    return true;
  }
  return false;
}

template <typename SymbolArg> void Scope::addInternalSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getHasName());
  REQUITE_ASSERT(!this->getHasInternalSymbolOfName(symbol.getName()));
  REQUITE_ASSERT(!symbol.getHasContaining());
  symbol.setContaining(*this);
  this->getInternalSymbolMap().insert(
      std::pair<llvm::StringRef, requite::RootSymbol>(
          symbol.getName(), requite::RootSymbol::makeUser(symbol)));
}

inline bool Scope::getHasExportSymbolOfName(llvm::StringRef name) const {
  REQUITE_ASSERT(!name.empty());
  const requite::ExportTable &export_table = this->getExportTable();
  if (export_table.getHasExportSymbolOfName(name)) {
    return true;
  }
  return false;
}


template <typename SymbolArg> void Scope::addExportSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getHasName());
  REQUITE_ASSERT(!this->getHasExportSymbolOfName(symbol.getName()));
  REQUITE_ASSERT(!symbol.getHasContaining());
  symbol.setContaining(*this);
  requite::ExportTable &export_table = this->getExportTable();
  export_table.getSymbolMap().insert(
      std::pair<llvm::StringRef, requite::RootSymbol>(
          symbol.getName(), requite::RootSymbol::makeUser(symbol)));
}

inline bool Scope::getHasSymbolOfName(llvm::StringRef name) const {
  REQUITE_ASSERT(!name.empty());
  if (this->getHasExportTable()) {
    const requite::ExportTable &export_table = this->getExportTable();
    if (export_table.getHasExportSymbolOfName(name)) {
      return true;
    }
  }
  if (this->getHasInternalSymbolOfName(name)) {
    return true;
  }
  return false;
}

} // namespace requite