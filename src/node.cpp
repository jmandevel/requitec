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

bool Node::getHasNext() const { return this->_next_ptr != nullptr; }

void Node::setNext(requite::Node &node) {
  requite::setSingleRef(this->_next_ptr, node);
}

requite::Node &Node::getNext() { return requite::getRef(this->_next_ptr); }

const requite::Node &Node::getNext() const {
  return requite::getRef(this->_next_ptr);
}

requite::SymbolStatus Node::getSymbolStatus() const {
  return this->_symbol_status;
}

void Node::incrementSymbolStatus() {
  REQUITE_ASSERT(this->_symbol_status != requite::SymbolStatus::DONE);
  this->_symbol_status = static_cast<requite::SymbolStatus>(
      static_cast<unsigned>(this->_symbol_status) + 1);
}

bool Node::getHasGeneratedNameOrValue() const {
  return this->_has_generated_name_or_value;
}

void Node::setHasGeneratedNameOrValue() {
  REQUITE_ASSERT(!this->_has_generated_name_or_value);
  this->_has_generated_name_or_value = true;
}

} // namespace requite