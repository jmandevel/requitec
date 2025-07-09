#include <requite/assert.hpp>
#include <requite/contextualizer0.hpp>
#include <requite/module.hpp>
#include <requite/scope.hpp>

namespace requite {

Contextualizer0::Contextualizer0(requite::Context &context,
                                 requite::Module &module)
    : _context_ref(context), _module_ref(module),
      _scope_ptr(&module.getScope()) {}

requite::Context &Contextualizer0::getContext() {
  return this->_context_ref.get();
}

const requite::Context &Contextualizer0::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Contextualizer0::getModule() {
  return this->_module_ref.get();
}

const requite::Module &Contextualizer0::getModule() const {
  return this->_module_ref.get();
}

requite::Scope &Contextualizer0::getScope() {
  return requite::getRef(this->_scope_ptr);
}

const requite::Scope &Contextualizer0::getScope() const {
  return requite::getRef(this->_scope_ptr);
}

requite::Object &Contextualizer0::getObject() {
  return this->getScope().getObject();
}

const requite::Object &Contextualizer0::getObject() const {
  return this->getScope().getObject();
}

void Contextualizer0::enterScope(requite::Scope &scope) {
  REQUITE_ASSERT(this->getScope().getContaining() == scope);
  this->_scope_ptr = &scope;
}

void Contextualizer0::leaveScope() {
  this->_scope_ptr = &this->getScope().getContaining();
}

bool Contextualizer0::getIsOk() const { return this->_is_ok; }

void Contextualizer0::setNotOk() { this->_is_ok = false; }

} // namespace requite