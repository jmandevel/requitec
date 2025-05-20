// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/resolver.hpp>

namespace requite {

Resolver::Resolver(requite::Context &context) : _context_ref(context) {}

requite::Context &Resolver::getContext() { return this->_context_ref.get(); }

const requite::Context &Resolver::getContext() const {
  return this->_context_ref.get();
}

bool Resolver::getIsOk() const { return this->_is_ok; }

void Resolver::setNotOk() { this->_is_ok = false; }

} // namespace requite