// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/maker.hpp>
#include <requite/module.hpp>

namespace requite {

Maker::Maker(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module) {}

requite::Context &Maker::getContext() { return this->_context_ref.get(); }

const requite::Context &Maker::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Maker::getModule() { return this->_module_ref.get(); }

const requite::Module &Maker::getModule() const {
  return this->_module_ref.get();
}

bool Maker::getIsOk() const { return this->_is_ok; }

void Maker::setNotOk() { this->_is_ok = false; }

} // namespace requite