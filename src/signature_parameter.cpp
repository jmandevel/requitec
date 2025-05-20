// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/signature_parameter.hpp>

namespace requite {

requite::Symbol &SignatureParameter::getType() { return this->_type; }

const requite::Symbol &SignatureParameter::getType() const {
  return this->_type;
}

bool SignatureParameter::operator==(const Self &rhs) const {
  return this->_name == rhs._name && this->_type == rhs._type;
}

bool SignatureParameter::operator!=(const Self &rhs) const {
  return !(*this == rhs);
}

bool SignatureParameter::getHasName() const { return !this->_name.empty(); }

void SignatureParameter::setName(std::string_view name) {
  REQUITE_ASSERT(!name.empty());
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

std::string_view SignatureParameter::getName() const {
  REQUITE_ASSERT(!this->_name.empty());
  return this->_name;
}

bool SignatureParameter::getHasDefaultValueExpression() const {
  return this->_default_value_expression_ptr != nullptr;
}

void SignatureParameter::setDefaultValueExpression(
    requite::Expression &expression) {
  requite::setSingleRef(this->_default_value_expression_ptr, expression);
}

requite::Expression &SignatureParameter::getDefaultValueExpression() {
  return requite::getRef(this->_default_value_expression_ptr);
}

const requite::Expression &
SignatureParameter::getDefaultValueExpression() const {
  return requite::getRef(this->_default_value_expression_ptr);
}

} // namespace requite