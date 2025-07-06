#include <requite/situator.hpp>

namespace requite {

Situator::Situator(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module), _is_ok(true) {}

requite::Context &Situator::getContext() { return this->_context_ref.get(); }

const requite::Context &Situator::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Situator::getModule() { return this->_module_ref.get(); }

const requite::Module &Situator::getModule() const {
  return this->_module_ref.get();
}

void Situator::setNotOk() { this->_is_ok = false; }

bool Situator::getIsOk() const { return this->_is_ok; }

void Situator::setIsOk() { this->_is_ok = true; }

}