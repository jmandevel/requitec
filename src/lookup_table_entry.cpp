#include <requite/lookup_table_entry.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

void LookupTableEntry::addTable(requite::RootSymbol root) {
  REQUITE_ASSERT(root.getType() == requite::RootSymbolType::TABLE);
  this->_symbols.emplace_back(root);
}

void LookupTableEntry::addSymbol(requite::Module &module,
                                 requite::RootSymbol root) {
  REQUITE_ASSERT(!root.getIsNone());
  this->_symbols.emplace_back(module, root);
}

void LookupTableEntry::addSymbol(requite::Module &module,
                                 requite::RootSymbol root, requite::Use &use) {
  REQUITE_ASSERT(!root.getIsNone());
  this->_symbols.emplace_back(module, root, use);
}

void LookupTableEntry::addSymbol(requite::Module &module,
                                 requite::RootSymbol root,
                                 requite::Import &import) {
  REQUITE_ASSERT(!root.getIsNone());
  this->_symbols.emplace_back(module, root, import);
}

llvm::SmallVector<requite::UserSymbol, 1> &LookupTableEntry::getUserSymbols() {
  REQUITE_ASSERT(!this->_symbols.empty());
  return this->_symbols;
}

const llvm::SmallVector<requite::UserSymbol, 1> &
LookupTableEntry::getUserSymbols() const {
  REQUITE_ASSERT(!this->_symbols.empty());
  return this->_symbols;
}

bool LookupTableEntry::getIsAmbiguous() const {
  REQUITE_ASSERT(!this->_symbols.empty());
  if (this->_symbols.size() == 1) {
    return false;
  }
  for (const requite::UserSymbol &user : this->_symbols) {
    const requite::RootSymbol &root = user.getRoot();
    switch (const requite::RootSymbolType type = root.getType()) {
    case requite::RootSymbolType::PROCEDURE:
      continue;
    default:
      return true;
    }
  }
  return false;
}

} // namespace requite