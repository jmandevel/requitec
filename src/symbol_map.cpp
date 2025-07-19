#include <requite/table.hpp>
#include <requite/scope.hpp>

namespace requite {

void Scope::addTable(requite::Table &table) {
  REQUITE_ASSERT(table.getContainingScope() == *this);
  llvm::StringRef name = table.getName();
  REQUITE_ASSERT(!name.empty());
  requite::RootSymbol root;
  root.setAsUser(table);
  requite::LookupTableEntry &entry = this->_symbol_map[name];
  entry.addTable(root);
}

}