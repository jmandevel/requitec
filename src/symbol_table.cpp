#include <requite/symbol_table.hpp>

namespace requite {

bool SymbolTable::operator==(const Self &rhs) const { return this == &rhs; }

bool SymbolTable::operator!=(const Self &rhs) const { return this == &rhs; }

llvm::StringMap<requite::RootSymbol> &SymbolTable::getSymbolMap() {
  return this->_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &SymbolTable::getSymbolMap() const {
  return this->_symbol_map;
}

bool SymbolTable::getIsEmpty() const { return this->_symbol_map.empty(); }

} // namespace requite