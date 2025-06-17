#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/table_path.hpp>

namespace requite {

const std::vector<llvm::StringRef> &TablePath::getTables() const {
  return this->_tables;
}

bool TablePath::getIsAbsolute() const { return this->_is_absolute; }

void TablePath::buildPath(requite::Scope &scope) {
  this->_tables.clear();
  this->_is_absolute = true;
  if (scope.getType() == requite::ScopeType::MODULE) {
    return;
  }
  this->_tables.resize(scope.getScopeDepth());
  unsigned path_i = scope.getScopeDepth();
  for (requite::Scope &containing_scope : scope.getContainingSubrange()) {
    if (containing_scope.getType() == requite::ScopeType::MODULE) {
      break;
    }
    if (containing_scope.getType() != requite::ScopeType::TABLE) {
      this->_is_absolute = false;
      this->_tables.clear();
      break;
    }
    requite::Table &table = scope.getTable();
    llvm::StringRef name = table.getName();
    this->_tables[path_i--] = name;
  }
}

} // namespace requite