#include <requite/assert.hpp>
#include <requite/local_implementor.hpp>
#include <requite/scope.hpp>

namespace requite {

LocalImplementor::LocalImplementor(requite::Context &context,
                                   requite::Global &global)
    : _context_ref(context), _global_ptr(&global) {}

LocalImplementor::LocalImplementor(requite::Context &context,
                                   requite::Procedure &procedure)
    : _context_ref(context), _procedure_ptr(&procedure) {}

requite::Context &LocalImplementor::getContext() {
  return this->_context_ref.get();
}

const requite::Context &LocalImplementor::getContext() const {
  return this->_context_ref.get();
}

bool LocalImplementor::getIsImplementingGlobal() const {
  return this->_global_ptr != nullptr;
}

requite::Global &LocalImplementor::getGlobal() {
  return requite::getRef(this->_global_ptr);
}

const requite::Global &LocalImplementor::getGlobal() const {
  return requite::getRef(this->_global_ptr);
}

bool LocalImplementor::getIsImplementingProcedure() const {
  return this->_procedure_ptr != nullptr;
}

requite::Procedure &LocalImplementor::getProcedure() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &LocalImplementor::getProcedure() const {
  return requite::getRef(this->_procedure_ptr);
}

requite::Scope &LocalImplementor::getScope() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &LocalImplementor::getScope() const {
  return requite::getRef(this->_scope_ptr);
}

void LocalImplementor::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(scope.getContaining() == this->getScope());
  this->_scope_ptr = &scope;
}

void LocalImplementor::leaveScope() {
  this->_scope_ptr = &this->getScope().getContaining();
}

} // namespace requite