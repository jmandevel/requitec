#include <requite/table_alias.hpp>

namespace requite {

bool TableAlias::operator==(const Self &rhs) const { return this == &rhs; }

bool TableAlias::operator!=(const Self &rhs) const { return this != &rhs; }

bool TableAlias::getHasName() const { return !this->_name.empty(); }

void TableAlias::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name;
}

llvm::StringRef TableAlias::getName() const {
  REQUITE_ASSERT(this->getHasName());
  return this->_name;
}

bool TableAlias::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void TableAlias::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &TableAlias::getExpresssion() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &TableAlias::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool TableAlias::getHasAliasedTable() const {
  return this->_aliased_table_ptr != nullptr;
}

void TableAlias::setAliasedTable(requite::Table &table) {
  requite::setSingleRef(this->_aliased_table_ptr, table);
}

requite::Table &TableAlias::getAliasedTable() {
  return requite::getRef(this->_aliased_table_ptr);
}

const requite::Table &TableAlias::getAliasedTable() const {
  return requite::getRef(this->_aliased_table_ptr);
}

bool TableAlias::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void TableAlias::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &TableAlias::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &TableAlias::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

bool TableAlias::getHasContainingModule() const {
  return this->_containing_module_ptr != nullptr;
}

void TableAlias::setContainingModule(requite::Module &module) {
  requite::setSingleRef(this->_containing_module_ptr, module);
}

requite::Module &TableAlias::getContainingModule() {
  return requite::getRef(this->_containing_module_ptr);
}

const requite::Module &TableAlias::getContainingModule() const {
  return requite::getRef(this->_containing_module_ptr);
}

requite::AttributeFlags &TableAlias::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &TableAlias::getAttributeFlags() const {
  return this->_attributes;
}

} // namespace requite