// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>

namespace requite {

Context::Context(std::string &&executable_path)
    : _executable_path(std::move(executable_path)) {}

} // namespace requite