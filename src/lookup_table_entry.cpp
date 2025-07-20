#include <requite/lookup_table_entry.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

bool LookupTableEntry::getHasTable() const {
  return this->_table_ptr != nullptr;
}

void LookupTableEntry::setTable(requite::Table &table) {
  requite::setSingleRef(this->_table_ptr, table);
}

requite::Table &LookupTableEntry::getTable() {
  return requite::getRef(this->_table_ptr);
}

const requite::Table &LookupTableEntry::getTable() const {
  return requite::getRef(this->_table_ptr);
}

llvm::SmallVector<requite::UserSymbol, 1> &LookupTableEntry::getUserSymbols() {
  return this->_symbols;
}

const llvm::SmallVector<requite::UserSymbol, 1> &
LookupTableEntry::getUserSymbols() const {
  return this->_symbols;
}

bool LookupTableEntry::getIsAmbiguous() const {
  return true; // TODO
}

} // namespace requite