#pragma once

#include <requite/table.hpp>

namespace requite {

template <typename SymbolArg> void Scope::addUserSymbol(SymbolArg &symbol) {
  return this->getTable().addUserSymbol(symbol);
}

} // namespace requite