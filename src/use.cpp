#include <requite/use.hpp>

namespace requite {

bool Use::operator==(const Self &rhs) const { return this == &rhs; }

bool Use::operator!=(const Self &rhs) const { return this != &rhs; }

bool Use::getHasExpression() const { return this->_expression_ptr != nullptr; }

void Use::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Use::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Use::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::AttributeFlags &Use::getAttributeFlags() { return this->_attributes; }

requite::AttributeFlags Use::getAttributeFlags() const {
  return this->_attributes;
}

bool Use::getHasContainingScope() const { return this->_containing_scope_ptr != nullptr; }

void Use::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Use::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Use::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

bool Use::getHasContainingModule() const { return this->_containing_module_ptr != nullptr; }

void Use::setContainingModule(requite::Module &module) {
  requite::setSingleRef(this->_containing_module_ptr, module);
}

requite::Module &Use::getContainingModule() { return requite::getRef(this->_containing_module_ptr); }

const requite::Module &Use::getContainingModule() const {
  return requite::getRef(this->_containing_module_ptr);
}

bool Use::getHasNext() const { return this->_next_ptr; }

void Use::setNext(requite::Use &next) {
  requite::setSingleRef(this->_next_ptr, next);
}

requite::Use &Use::getNext() { return requite::getRef(this->_next_ptr); }

const requite::Use &Use::getNext() const {
  return requite::getRef(this->_next_ptr);
}

} // namespace requite