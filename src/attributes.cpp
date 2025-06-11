// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/attributes.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>

#include <cstddef>

namespace requite {

Attributes::Attributes(llvm::ArrayRef<requite::AttributeType> attributes) {
  for (requite::AttributeType attribute : attributes) {
    REQUITE_ASSERT(!this->getHasAttribute(attribute));
    this->addAttribute(attribute);
  }
}

requite::MakeAttributesResult
Attributes::makeAttributes(requite::Context &context,
                           requite::Expression &first_attribute) {
  requite::MakeAttributesResult result;
  result.attributes.setFirstExpression(first_attribute);
  for (requite::Expression &attribute : first_attribute.getHorizontalSubrange()) {
    const requite::Opcode opcode = attribute.getOpcode();
    const requite::AttributeType type = requite::getAttributeType(opcode);
    result.attributes.addAttribute(type);
  }
  return result;
}

void Attributes::logErrorDuplicateAttribute(requite::Context &context,
                                            requite::Expression &branch,
                                            requite::AttributeType type) {
  context.logSourceMessage(branch, requite::LogType::ERROR,
                           llvm::Twine("duplicate attribute found of type") +
                               requite::getName(type) + "");
}

void Attributes::clear() {
  this->_flags.reset();
}

bool Attributes::getHasAnyAttribute() const {
  return this->_flags.any();
}

void Attributes::addAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(!this->getHasAttribute(type));
  this->_flags.set(static_cast<std::size_t>(type));
}

void Attributes::removeAttribute(requite::AttributeType type) {
  REQUITE_ASSERT(this->getHasAttribute(type));
  this->_flags.reset(static_cast<std::size_t>(type));
}

bool Attributes::getHasAttribute(requite::AttributeType type) const {
  return this->_flags.test(static_cast<std::size_t>(type));
}

bool Attributes::operator==(const requite::Attributes &other) const {
  return this->_flags == other._flags;
}

bool Attributes::operator!=(const requite::Attributes &other) const {
  return !(*this == other);
}

bool Attributes::getHasFirstExpression() const {
  return this->_first_expression_ptr != nullptr;
}

void Attributes::setFirstExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_first_expression_ptr, expression);
}

requite::Expression &Attributes::getFirstExpression() {
  return requite::getRef(this->_first_expression_ptr);
}

const requite::Expression &Attributes::getFirstExpression() const {
  return requite::getRef(this->_first_expression_ptr);
}

} // namespace requite