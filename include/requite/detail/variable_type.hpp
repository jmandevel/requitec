// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requtite {

[[nodiscard]] constexpr std::string_view getName(requite::VariableType type) {
  switch (type) {
  case requite::VariableType::NONE:
    return "NONE";
  case requite::VariableType::LOCAL:
    return "LOCAL";
  case requite::VariableType::GLOBAL:
    return "GLOBAL";
  case requite::VariableType::PROPERTY:
    return "PROPERTY";
  }
  return "_ERROR";
}

constexpr requite::VariableType getVariableType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::LOCAL:
    return requite::VariableType::LOCAL;
  case requite::Opcode::GLOBAL:
    return requite::VariableType::GLOBAL;
  case requite::Opcode::PROPERTY:
    return requite::VariableType::PROPERTY;
  default:
    return requite::VariableType::NONE;
  }
}

constexpr requite::Opcode getOpcode(requite::VariableType type) {
  switch (type) {
  case requite::VariableType::LOCAL:
    return requite::Opcode::LOCAL;
  case requite::VariableType::GLOBAL:
    return requite::Opcode::GLOBAL;
  case requite::VariableType::PROPERTY:
    return requite::Opcode::PROPERTY;
  default:
    return requite::Opcode::_ERROR;
  }
}

} // namespace requtite