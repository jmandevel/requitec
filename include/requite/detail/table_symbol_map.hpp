#pragma once

#include <requite/assert.hpp>

namespace requite {

template <typename SymbolArg> void Table::addUserSymbol(SymbolArg &symbol) {
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  REQUITE_ASSERT(!this->getHasUserSymbolOfName(name));
  requite::RootSymbol root;
  root.setAsUser(symbol);
  this->getSymbolMap().insert(
      std::pair<llvm::StringRef, requite::RootSymbol>(name, root));
}

} // namespace requite