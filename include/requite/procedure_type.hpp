// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <string_view>

namespace requite {

enum class ProcedureType {
  NONE,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  CONSTRUCTOR,
  DESTRUCTOR
};

[[nodiscard]] constexpr requite::ProcedureType
getProcedureType(requite::Opcode opcode);

[[nodiscard]] constexpr std::string_view getName(requite::ProcedureType type);

} // namespace requite

#include <requite/detail/procedure_type.hpp>