#include <requite/assert.hpp>
#include <requite/local_block.hpp>

namespace requite {

requite::Scope &LocalBlock::getScope() { return this->_scope; }

const requite::Scope &LocalBlock::getScope() const {
  return this->_scope;
}

bool LocalBlock::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void LocalBlock::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &LocalBlock::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &LocalBlock::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool LocalBlock::getHasLocal() const {
  return this->_first_local_ptr != nullptr;
}

void LocalBlock::addLocal(requite::Local &local) {
  requite::setSingleRef(this->_first_local_ptr, local);
}
requite::Local &LocalBlock::getFirstLocal() {
  return requite::getRef(this->_first_local_ptr);
}

const requite::Local &LocalBlock::getFirstLocal() const {
  return requite::getRef(this->_first_local_ptr);
}

requite::AttributeFlags &LocalBlock::getAttributeFlags() {
  return this->_attribute_flags;
}

} // namespace requtite