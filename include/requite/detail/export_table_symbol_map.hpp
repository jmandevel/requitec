#pragma once

#include <requite/assert.hpp>

namespace requite {

inline bool ExportTable::getHasExportSymbolOfName(llvm::StringRef name) const {
  REQUITE_ASSERT(!name.empty());
  llvm::StringMapConstIterator<requite::RootSymbol> it =
      this->getSymbolMap().find(name);
  if (it != this->getSymbolMap().end()) {
    return true;
  }
  return false;
}

template <typename SymbolArg> void ExportTable::addExportSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getHasName());
  REQUITE_ASSERT(!this->getHasExportSymbolOfName(symbol.getName()));
  REQUITE_ASSERT(!symbol.getHasContaining());
  symbol.setContaining(*this);
  this->getSymbolMap().insert(
      std::pair<llvm::StringRef, requite::RootSymbol>(
          symbol.getName(), requite::RootSymbol::makeUser(symbol)));
}

}