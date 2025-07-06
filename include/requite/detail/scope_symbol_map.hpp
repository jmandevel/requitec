#pragma once

#include <requite/symbol_table.hpp>

namespace requite {

template <typename SymbolArg> void Scope::addUserSymbol(SymbolArg &symbol) {
  return this->getSymbolTable().addUserSymbol(symbol);
}

} // namespace requite