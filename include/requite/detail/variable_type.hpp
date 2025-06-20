// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

[[nodiscard]] constexpr std::string_view getName(requite::VariableType type) {
  switch (type) {
  case requite::VariableType::NONE:
    return "NONE";
  case requite::VariableType::LOCAL:
    return "LOCAL";
  case requite::VariableType::GLOBAL:
    return "GLOBAL";
  case requite::VariableType::STASH:
    return "STASH";
  case requite::VariableType::PROPERTY:
    return "PROPERTY";
  case requite::VariableType::CONSTANT:
    return "CONSTANT";
  }
  return "ERROR";
}

constexpr requite::VariableType getVariableType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::_LOCAL:
    return requite::VariableType::LOCAL;
  case requite::Opcode::GLOBAL:
    return requite::VariableType::GLOBAL;
  case requite::Opcode::STASH:
    return requite::VariableType::STASH;
  case requite::Opcode::PROPERTY:
    return requite::VariableType::PROPERTY;
  case requite::Opcode::CONSTANT:
    return requite::VariableType::CONSTANT;
  default:
    break;
  }
  return requite::VariableType::NONE;
}

constexpr requite::Opcode getOpcode(requite::VariableType type) {
  switch (type) {
  case requite::VariableType::LOCAL:
    return requite::Opcode::_LOCAL;
  case requite::VariableType::GLOBAL:
    return requite::Opcode::GLOBAL;
  case requite::VariableType::PROPERTY:
    return requite::Opcode::PROPERTY;
  case requite::VariableType::CONSTANT:
    return requite::Opcode::CONSTANT;
  default:
    break;
}
return requite::Opcode::__ERROR;
}

} // namespace requtite