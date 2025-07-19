#pragma once

namespace requite {

template <typename SymbolArg>
void Scope::addUserSymbol(requite::Module &module, SymbolArg &symbol) {
  REQUITE_ASSERT(symbol.getContainingScope() == *this);
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(module, root);
}

template <typename SymbolArg>
void Scope::addUserSymbol(requite::Module &module, SymbolArg &symbol,
                          requite::Use &use) {
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(module, root, use);
}

template <typename SymbolArg>
void Scope::addUserSymbol(requite::Module &module, SymbolArg &symbol,
                          requite::Import &import) {
  llvm::StringRef name = symbol.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(symbol);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addSymbol(module, root, import);
}

} // namespace requite