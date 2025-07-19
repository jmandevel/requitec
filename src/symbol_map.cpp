#include <requite/table.hpp>
#include <requite/scope.hpp>

namespace requite {

  bool Scope::getHasTable(llvm::StringRef name) const {
    const llvm::StringMap<requite::LookupTableEntry>::const_iterator entry_it = this->_symbol_map.find(name);
    if (entry_it == this->_symbol_map.end()) {
      return false;
    }
    const requite::LookupTableEntry& entry = entry_it->getValue();
    return entry.getHasTable();
  }

void Scope::addTable(requite::Table &table) {
  REQUITE_ASSERT(table.getContainingScope() == *this);
  llvm::StringRef name = table.getName();
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.setTable(table);
}

}