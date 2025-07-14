#include <requite/assert.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

UserSymbol::UserSymbol(requite::RootSymbol root)
    : _origin(requite::UserSymbolOrigin::INTERNAL), _symbol(root) {}

UserSymbol::UserSymbol(requite::RootSymbol root, requite::Use &use)
    : _origin(requite::UserSymbolOrigin::USED), _symbol(root), _use_ptr(&use) {}

UserSymbol::UserSymbol(requite::RootSymbol root, requite::Import &import)
    : _origin(requite::UserSymbolOrigin::IMPORTED), _symbol(root),
      _import_ptr(&import) {}

requite::UserSymbolOrigin UserSymbol::getOrigin() const { return this->_origin; }

requite::RootSymbol &UserSymbol::getRoot() { return this->_symbol; }

const requite::RootSymbol &UserSymbol::getRoot() const { return this->_symbol; }

requite::Use &UserSymbol::getUse() { return requite::getRef(this->_use_ptr); }

const requite::Use &UserSymbol::getUse() const {
  return requite::getRef(this->_use_ptr);
}

requite::Import &UserSymbol::getImport() {
  return requite::getRef(this->_import_ptr);
}

const requite::Import &UserSymbol::getImport() const {
  return requite::getRef(this->_import_ptr);
}

} // namespace requite