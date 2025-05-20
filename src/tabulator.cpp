// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/tabulator.hpp>

namespace requite {

Tabulator::Tabulator(
    requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module) {}

bool Tabulator::getIsOk() const { return this->_is_ok; }

void Tabulator::setNotOk() { this->_is_ok = false; }

requite::Context &Tabulator::getContext() {
  return this->_context_ref.get();
}

const requite::Context &Tabulator::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Tabulator::getModule() {
  return this->_module_ref.get();
}

const requite::Module &Tabulator::getModule() const {
  return this->_module_ref.get();
}

} // namespace requite