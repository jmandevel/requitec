#include <requite/builder.hpp>
#include <requite/scope.hpp>

namespace requite {

Builder::Builder(requite::Context &context)
    : _context_ref(context) {}

requite::Context &Builder::getContext() { return this->_context_ref.get(); }

const requite::Context &Builder::getContext() const {
  return this->_context_ref.get();
}

bool Builder::getHasScope() const {
  return this->_current_scope_ptr != nullptr;
}

void Builder::setScope(requite::Scope &scope) {
  requite::setSingleRef(this->_current_scope_ptr, scope);
}

void Builder::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(this->getScope() == scope.getContaining());
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

bool Builder::getHasProcedure() const {
  return this->_procedure_ptr != nullptr;
}

requite::Procedure &Builder::getProcedure() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &Builder::getProcedure() const {
  return requite::getRef(this->_procedure_ptr);
}

} // namespace requite