// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

#include <magic_enum.hpp>

namespace requite {

constexpr requite::ProcedureType getProcedureType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::_NONE:
    return requite::ProcedureType::NONE;
  case requite::Opcode::ENTRY_POINT:
    return requite::ProcedureType::ENTRY_POINT;
  case requite::Opcode::FUNCTION:
    return requite::ProcedureType::FUNCTION;
  case requite::Opcode::METHOD:
    return requite::ProcedureType::METHOD;
  case requite::Opcode::CONSTRUCTOR:
    return requite::ProcedureType::CONSTRUCTOR;
  case requite::Opcode::DESTRUCTOR:
    return requite::ProcedureType::DESTRUCTOR;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

constexpr std::string_view getName(requite::ProcedureType type) {
  std::string_view name = magic_enum::enum_name(type);
  return name;
}

} // namespace requite