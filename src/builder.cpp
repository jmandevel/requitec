#include <requite/builder.hpp>
#include <requite/scope.hpp>

namespace requite {

Builder::Builder(requite::Context &context) : _context_ref(context) {}

requite::Context &Builder::getContext() { return this->_context_ref.get(); }

const requite::Context &Builder::getContext() const {
  return this->_context_ref.get();
}

bool Builder::getHasScope() const {
  return this->_current_scope_ptr != nullptr;
}

void Builder::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(this->_current_scope_ptr != nullptr);
  this->_current_scope_ptr = &scope;
}

void Builder::exitScope() {
  this->_current_scope_ptr = &this->getScope().getContaining();
}

requite::Scope &Builder::getScope() {
  return requite::getRef(this->_current_scope_ptr);
}

const requite::Scope &Builder::getScope() const {
  return requite::getRef(this->_current_scope_ptr);
}

} // namespace requite