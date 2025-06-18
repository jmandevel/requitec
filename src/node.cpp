// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/node.hpp>

namespace requite {

void Node::setType(requite::NodeType type) {
  REQUITE_ASSERT(this->_type == requite::NodeType::NONE);
  REQUITE_ASSERT(type != requite::NodeType::NONE);
  this->_type = type;
}

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

void Node::setAttributeFlags(requite::AttributeFlags attributes) {
  REQUITE_ASSERT(!attributes.getHasAnyAttribute());
  this->_attributes = attributes;
}

requite::AttributeFlags &Node::getAttributeFlags() { return this->_attributes; }

const requite::AttributeFlags &Node::getAttributeFlags() const {
  return this->_attributes;
}

} // namespace requite