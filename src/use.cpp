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

bool Use::getHasScope() const { return this->_scope_ptr != nullptr; }
void Use::setScope(requite::Scope &scope) {
  requite::setSingleRef(this->_scope_ptr, scope);
}

requite::Scope &Use::getScope() { return requite::getRef(this->_scope_ptr); }

const requite::Scope &Use::getScope() const {
  return requite::getRef(this->_scope_ptr);
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