// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/signature.hpp>

namespace requite {

bool Signature::operator==(const requite::Signature &rhs) const {
  return this->_return_type == rhs._return_type &&
         this->_parameters == rhs._parameters;
}

bool Signature::operator!=(const requite::Signature &rhs) const {
  return this->_return_type != rhs._return_type ||
         this->_parameters != rhs._parameters;
}

bool Signature::getIsEmpty() const {
  return this->_return_type.getIsEmpty() && this->_parameters.empty();
}

std::vector<requite::SignatureParameter> &Signature::getParameters() {
  return this->_parameters;
}

const std::vector<requite::SignatureParameter> &Signature::getParameters() const {
  return this->_parameters;
}

requite::Symbol &Signature::getReturnType() { return this->_return_type; }

const requite::Symbol &Signature::getReturnType() const {
  return this->_return_type;
}

} // namespace requite