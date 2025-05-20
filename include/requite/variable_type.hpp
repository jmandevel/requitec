// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <string_view>

namespace requite {

enum class VariableType { NONE, LOCAL, GLOBAL, PROPERTY };

[[nodiscard]] constexpr std::string_view getName(requite::VariableType type);

[[nodiscard]] constexpr requite::VariableType
getVariableType(requite::Opcode opcode);

[[nodiscard]] constexpr requite::Opcode getOpcode(requite::VariableType type);

} // namespace requite

#include <requite/detail/variable_type.hpp>