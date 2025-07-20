#pragma once

namespace requite {

template <typename SymbolArg>
void Scope::addUserSymbol(SymbolArg &symbol, requite::Module &module) {
  llvm::StringRef name = symbol.getName();
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  for (requite::UserSymbol &user : entry.getUserSymbols()) {
    if (user == symbol) {
        return;
    }
  }
  entry.getUserSymbols().emplace_back(symbol, module);
}

} // namespace requite