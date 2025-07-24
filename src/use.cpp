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

bool Use::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void Use::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Use::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Use::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

requite::UserSymbol &Use::getUserSymbol() {
  return this->_user_symbol;
}

const requite::UserSymbol &Use::getUserSymbol() const {
  return this->_user_symbol;
}

bool Use::getHasContainingModule() const {
  return this->getUserSymbol().getHasContainingModule();
}

void Use::setContainingModule(requite::Module &module) {
  this->getUserSymbol().setContainingModule(module);
}

requite::Module &Use::getContainingModule() {
  return this->getUserSymbol().getContainingModule();
}

const requite::Module &Use::getContainingModule() const {
  return this->getUserSymbol().getContainingModule();
}

} // namespace requite