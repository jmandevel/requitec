// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

namespace requite {

Context::Context(std::string &&executable_path)
    : _executable_path(std::move(executable_path)) {}

bool Context::getIsOk() const { return this->_is_ok; }

void Context::setNotOk() { this->_is_ok = false; }

} // namespace requite