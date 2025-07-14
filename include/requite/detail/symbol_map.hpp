#pragma once

namespace requite {

template <typename SymbolArg> void Scope::addUserSymbol(SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getContaining() == *this);
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(root);
}

template <typename SymbolArg>
void Scope::addUserSymbol(SymbolArg &symbol, requite::Use &use) {
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(root, use);
}

template <typename SymbolArg>
void Scope::addUserSymbol(SymbolArg &symbol, requite::Import &import) {
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(root, import);
}

} // namespace requite