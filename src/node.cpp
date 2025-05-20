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

requite::Attributes &Node::getAttributes() { return this->_attributes; }

const requite::Attributes &Node::getAttributes() const {
  return this->_attributes;
}

std::vector<requite::Scope *> &Node::getScopePtrs() {
  return this->_scope_ptrs;
}

const std::vector<requite::Scope *> &Node::getScopePtrs() const {
  return this->_scope_ptrs;
}

} // namespace requite