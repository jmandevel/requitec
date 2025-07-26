#include <requite/use_table.hpp>

namespace requite {

bool UseTable::operator==(const Self &rhs) const { return this == &rhs; }

bool UseTable::operator!=(const Self &rhs) const { return this != &rhs; }

bool UseTable::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void UseTable::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &UseTable::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &UseTable::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool UseTable::getHasContainingModule() const { return this->_containing_module_uptr != nullptr; }

void UseTable::setContainingModule(requite::Module &module) {
  requite::setSingleRef(this->_containing_module_uptr, module);
}

requite::Module &UseTable::getContainingModule() {
  return requite::getRef(this->_containing_module_uptr);
}

const requite::Module &UseTable::getContainingModule() const {
  return requite::getRef(this->_containing_module_uptr);
}

requite::AttributeFlags &UseTable::getAttributeFlags() {
  return this->_attributes;
}

bool UseTable::getIsExported() { return this->_is_exported; }

void UseTable::setIsExported() { this->_is_exported = true; }

bool UseTable::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void UseTable::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &UseTable::getScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &UseTable::getScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

} // namespace requite