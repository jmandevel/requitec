#include <requite/global_tabulator.hpp>
#include <requite/assert.hpp>
#include <requite/scope.hpp>
#include <requite/module.hpp>

namespace requite {

GlobalTabulator::GlobalTabulator(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module), _scope_ptr(&module.getScope()) {}

requite::Context &GlobalTabulator::getContext() {
    return this->_context_ref.get();
}

const requite::Context &GlobalTabulator::getContext() const {
    return this->_context_ref.get();
}

requite::Module &GlobalTabulator::getModule() {
    return this->_module_ref.get();
}

const requite::Module &GlobalTabulator::getModule() const {
    return this->_module_ref.get();
}

requite::Scope &GlobalTabulator::getScope() {
    return requite::getRef(this->_scope_ptr);
}

const requite::Scope &GlobalTabulator::getScope() const {
    return requite::getRef(this->_scope_ptr);
}

void GlobalTabulator::enterScope(requite::Scope &scope) {
    REQUITE_ASSERT(this->getScope().getContaining() == scope);
    this->_scope_ptr = &scope;
}

void GlobalTabulator::leaveScope() {
    this->_scope_ptr = &this->getScope().getContaining();
}

} // namespace requite