// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr requite::ProcedureType getProcedureType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::ENTRY_POINT:
    return requite::ProcedureType::ENTRY_POINT;
  case requite::Opcode::FUNCTION:
    return requite::ProcedureType::FUNCTION;
  case requite::Opcode::METHOD:
    return requite::ProcedureType::METHOD;
  case requite::Opcode::EXTENSION:
    return requite::ProcedureType::EXTENSION;
  case requite::Opcode::CONSTRUCTOR:
    return requite::ProcedureType::CONSTRUCTOR;
  case requite::Opcode::DESTRUCTOR:
    return requite::ProcedureType::DESTRUCTOR;
  default:
    break;
  }
  return requite::ProcedureType::NONE;
}

constexpr std::string_view getName(requite::ProcedureType type) {
  switch (type) {
  case requite::ProcedureType::NONE:
    return "none";
  case requite::ProcedureType::ENTRY_POINT:
    return "entry_point";
  case requite::ProcedureType::FUNCTION:
    return "function";
  case requite::ProcedureType::METHOD:
    return "method";
  case requite::ProcedureType::EXTENSION:
    return "extension";
  case requite::ProcedureType::CONSTRUCTOR:
    return "constructor";
  case requite::ProcedureType::DESTRUCTOR:
    return "destructor";
  }
  return "error";
}

constexpr bool getCanHaveName(requite::ProcedureType type) {
  switch (type) {
  case requite::ProcedureType::NONE:
    return false;
  case requite::ProcedureType::ENTRY_POINT:
    return false;
  case requite::ProcedureType::FUNCTION:
    return true;
  case requite::ProcedureType::METHOD:
    return true;
  case requite::ProcedureType::CONSTRUCTOR:
    return false;
  case requite::ProcedureType::DESTRUCTOR:
    return false;
  default:
    break;
  }
  return false;
}

} // namespace requite