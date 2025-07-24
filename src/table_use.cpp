#include <requite/table_use.hpp>

namespace requite {

bool TableUse::operator==(const Self &rhs) const { return this == &rhs; }

bool TableUse::operator!=(const Self &rhs) const { return this != &rhs; }

bool TableUse::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void TableUse::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &TableUse::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &TableUse::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool TableUse::getHasContainingModule() const { return this->_containing_module_uptr != nullptr; }

void TableUse::setContainingModule(requite::Module &module) {
  requite::setSingleRef(this->_containing_module_uptr, module);
}

requite::Module &TableUse::getContainingModule() {
  return requite::getRef(this->_containing_module_uptr);
}

const requite::Module &TableUse::getContainingModule() const {
  return requite::getRef(this->_containing_module_uptr);
}

requite::AttributeFlags &TableUse::getAttributeFlags() {
  return this->_attributes;
}

bool TableUse::getIsExported() { return this->_is_exported; }

void TableUse::setIsExported() { this->_is_exported = true; }

bool TableUse::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void TableUse::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &TableUse::getScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &TableUse::getScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

} // namespace requite