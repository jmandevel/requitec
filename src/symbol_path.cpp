#include <requite/assert.hpp>
#include <requite/object.hpp>
#include <requite/scope.hpp>
#include <requite/symbol_path.hpp>
#include <requite/table.hpp>

namespace requite {

bool SymbolPath::getIsEmpty() const {
  return !this->getHasHeadScope() && this->_body.empty();
}

bool SymbolPath::getHasHeadScope() const {
  return this->_head_scope_ptr != nullptr;
}

void SymbolPath::setHeadScope(requite::Scope &scope) {
  requite::setSingleRef(this->_head_scope_ptr, scope);
}

requite::Scope &SymbolPath::getHeadScope() {
  return requite::getRef(this->_head_scope_ptr);
}

const requite::Scope &SymbolPath::getHeadScope() const {
  return requite::getRef(this->_head_scope_ptr);
}

std::vector<llvm::StringRef> &SymbolPath::getBody() { return this->_body; }

const std::vector<llvm::StringRef> &SymbolPath::getBody() const {
  return this->_body;
}

bool SymbolPath::makeContainingPath(requite::Scope &scope) {
  if (scope.getScopeDepth() == 0) {
    return true;
    this->_body.resize(scope.getScopeDepth());
    unsigned scope_i = scope.getScopeDepth();
    for (requite::Scope &containing : scope.getContainingSubrange()) {
      if (!containing.getHasContaining()) {
        break;
      }
      switch (const requite::ScopeType type = containing.getType()) {
      case requite::ScopeType::TABLE: {
        requite::Table &table = containing.getTable();
        llvm::StringRef name = table.getName();
        this->getBody()[--scope_i] = name;
        break;
      }
      case requite::ScopeType::OBJECT: {
        requite::Object &object = containing.getObject();
        llvm::StringRef name = object.getName();
        this->getBody()[--scope_i] = name;
        break;
      }
      default:
        this->getBody().clear();
        return false;
      }
    }
  }
  return true;
}

} // namespace requite