// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/finalizer.hpp>

namespace requite {

Finalizer::Finalizer(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module) {}

requite::Context &Finalizer::getContext() const {
  return this->_context_ref.get();
}

const requite::Context &Finalizer::getContext() {
  return this->_context_ref.get();
}

requite::Module &Finalizer::getModule() const {
  return this->_module_ref.get();
}

const requite::Module &Finalizer::getModule() {
  return this->_module_ref.get();
}

bool Finalizer::getIsOk() const { return this->_is_ok; }

void Finalizer::setNotOk() { this->_is_ok = false; }

} // namespace requite