#include <requite/assert.hpp>
#include <requite/implementor.hpp>
#include <requite/scope.hpp>

namespace requite {

Implementor::Implementor(requite::Context &context,
                                   requite::Global &global)
    : _context_ref(context), _global_ptr(&global) {}

Implementor::Implementor(requite::Context &context,
                                   requite::Procedure &procedure)
    : _context_ref(context), _procedure_ptr(&procedure) {}

requite::Context &Implementor::getContext() {
  return this->_context_ref.get();
}

const requite::Context &Implementor::getContext() const {
  return this->_context_ref.get();
}

bool Implementor::getIsImplementingGlobal() const {
  return this->_global_ptr != nullptr;
}

requite::Global &Implementor::getGlobal() {
  return requite::getRef(this->_global_ptr);
}

const requite::Global &Implementor::getGlobal() const {
  return requite::getRef(this->_global_ptr);
}

bool Implementor::getIsImplementingProcedure() const {
  return this->_procedure_ptr != nullptr;
}

requite::Procedure &Implementor::getProcedure() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &Implementor::getProcedure() const {
  return requite::getRef(this->_procedure_ptr);
}

requite::Scope &Implementor::getScope() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &Implementor::getScope() const {
  return requite::getRef(this->_scope_ptr);
}

void Implementor::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(scope.getContaining() == this->getScope());
  this->_scope_ptr = &scope;
}

void Implementor::leaveScope() {
  this->_scope_ptr = &this->getScope().getContaining();
}

} // namespace requite