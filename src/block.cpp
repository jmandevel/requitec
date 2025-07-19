#include <requite/assert.hpp>
#include <requite/block.hpp>

namespace requite {

requite::BlockType Block::getType() {
  return this->_type;
}

void Block::setType(requite::BlockType type) {
  REQUITE_ASSERT(this->_type == requite::BlockType::NONE);
  this->_type = type;
}

Block::Block() { this->getScope().setBlock(*this); }

requite::Scope &Block::getScope() { return this->_scope; }

const requite::Scope &Block::getScope() const { return this->_scope; }

bool Block::getHasContainingScope() const {
  return this->getScope().getHasContainingScope();
}

void Block::setContainingScope(requite::Scope &scope) {
  this->getScope().setContainingScope(scope);
}

requite::Scope &Block::getContainingScope() {
  return this->getScope().getContainingScope();
}

const requite::Scope &Block::getContainingScope() const {
  return this->getScope().getContainingScope();
}

bool Block::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Block::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

const requite::Expression &Block::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Expression &Block::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

requite::AttributeFlags &Block::getAttributeFlags() {
  return this->_attributes;
}

requite::AttributeFlags Block::getAttributeFlags() const {
  return this->_attributes;
}

} // namespace requite