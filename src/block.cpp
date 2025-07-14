#include <requite/assert.hpp>
#include <requite/block.hpp>

namespace requite {

requite::Scope &Block::getScope() { return this->_scope; }

const requite::Scope &Block::getScope() const { return this->_scope; }

bool Block::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Block::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Block::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Block::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool Block::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void Block::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &Block::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &Block::getContaining() const {
  return this->getScope().getContaining();
}

} // namespace requite