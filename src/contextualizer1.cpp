#include <requite/assert.hpp>
#include <requite/contextualizer1.hpp>
#include <requite/scope.hpp>

namespace requite {

Contextualizer1::Contextualizer1(requite::Context &context,
                                   requite::Global &global)
    : _context_ref(context), _global_ptr(&global) {}

Contextualizer1::Contextualizer1(requite::Context &context,
                                   requite::Procedure &procedure)
    : _context_ref(context), _procedure_ptr(&procedure) {}

requite::Context &Contextualizer1::getContext() {
  return this->_context_ref.get();
}

const requite::Context &Contextualizer1::getContext() const {
  return this->_context_ref.get();
}

bool Contextualizer1::getIsImplementingGlobal() const {
  return this->_global_ptr != nullptr;
}

requite::Global &Contextualizer1::getGlobal() {
  return requite::getRef(this->_global_ptr);
}

const requite::Global &Contextualizer1::getGlobal() const {
  return requite::getRef(this->_global_ptr);
}

bool Contextualizer1::getIsImplementingProcedure() const {
  return this->_procedure_ptr != nullptr;
}

requite::Procedure &Contextualizer1::getProcedure() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &Contextualizer1::getProcedure() const {
  return requite::getRef(this->_procedure_ptr);
}

requite::Scope &Contextualizer1::getScope() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &Contextualizer1::getScope() const {
  return requite::getRef(this->_scope_ptr);
}

void Contextualizer1::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(scope.getContaining() == this->getScope());
  this->_scope_ptr = &scope;
}

void Contextualizer1::leaveScope() {
  this->_scope_ptr = &this->getScope().getContaining();
}

} // namespace requite