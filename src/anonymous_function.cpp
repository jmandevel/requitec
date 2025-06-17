// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/anonymous_function.hpp>
#include <requite/assert.hpp>

namespace requite {

AnonymousFunction::AnonymousFunction() {
  this->_scope.setAnonymousFunction(*this);
}

bool AnonymousFunction::operator==(const Self &rhs) const {
  return this == &rhs;
}

bool AnonymousFunction::operator!=(const Self &rhs) const {
  return this != &rhs;
}

requite::Scope &AnonymousFunction::getScope() { return this->_scope; }

const requite::Scope &AnonymousFunction::getScope() const {
  return this->_scope;
}

bool AnonymousFunction::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void AnonymousFunction::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &AnonymousFunction::getContaining() {
  return this->getScope().getContaining();
}
const requite::Scope &AnonymousFunction::getContaining() const {
  return this->getScope().getContaining();
}

bool AnonymousFunction::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void AnonymousFunction::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &AnonymousFunction::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &AnonymousFunction::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Signature &AnonymousFunction::getSignature() {
  return this->_signature;
}

const requite::Signature &AnonymousFunction::getSignature() const {
  return this->_signature;
}

std::vector<requite::Local *> &AnonymousFunction::getCapturedPtrs() {
  return this->_captured_ptrs;
}

const std::vector<requite::Local *> &
AnonymousFunction::AnonymousFunction::getCapturedPtrs() const {
  return this->_captured_ptrs;
}

} // namespace requite