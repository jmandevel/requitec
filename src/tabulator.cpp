#include <requite/assert.hpp>
#include <requite/tabulator.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>

namespace requite {

Tabulator::Tabulator(requite::Context &context,
                                 requite::Module &module)
    : _context_ref(context), _module_ref(module),
      _scope_ptr(&context.getOuterScope()) {}

requite::Context &Tabulator::getContext() {
  return this->_context_ref.get();
}

const requite::Context &Tabulator::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Tabulator::getModule() {
  return this->_module_ref.get();
}

const requite::Module &Tabulator::getModule() const {
  return this->_module_ref.get();
}

requite::Scope &Tabulator::getScope() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &Tabulator::getScope() const {
  return requite::getRef(this->_scope_ptr);
}

requite::Object &Tabulator::getObject() {
  return this->getScope().getObject();
}

const requite::Object &Tabulator::getObject() const {
  return this->getScope().getObject();
}

void Tabulator::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(scope.getContainingScope() == this->getScope());
  this->_scope_ptr = &scope;
}

void Tabulator::leaveScope() {
  this->_scope_ptr = &this->getScope().getContainingScope();
}

bool Tabulator::getIsOk() const { return this->_is_ok; }

void Tabulator::setNotOk() { this->_is_ok = false; }

} // namespace requite