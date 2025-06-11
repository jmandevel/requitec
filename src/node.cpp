// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/node.hpp>

namespace requite {

void Node::setType(requite::NodeType type) { this->_type = type; }

requite::NodeType Node::getType() const { return this->_type; }

bool Node::getHasExpression() const { return this->_expression_ptr != nullptr; }

void Node::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Node::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Node::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool Node::getHasAttributes() const {
  return this->_attributes.getHasFirstExpression();
}

void Node::setAttributes(requite::Attributes attributes) {
  REQUITE_ASSERT(!this->_attributes.getHasFirstExpression());
  REQUITE_ASSERT(attributes.getHasFirstExpression());
  this->_attributes = attributes;
}

requite::Attributes &Node::getAttributes() {
  REQUITE_ASSERT(this->_attributes.getHasFirstExpression());
  return this->_attributes;
}

const requite::Attributes &Node::getAttributes() const {
  REQUITE_ASSERT(this->_attributes.getHasFirstExpression());
  return this->_attributes;
}

} // namespace requite