// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr requite::AttributeType getAttributeType(requite::Opcode opcode)
{
  switch (opcode) {
    case requite::Opcode::VOLATILE:
      return requite::AttributeType::VOLATILE;
    case requite::Opcode::ATOMIC:
      return requite::AttributeType::ATOMIC;
    case requite::Opcode::NULL_TERMINATED:
      return requite::AttributeType::NULL_TERMINATED;
    case requite::Opcode::TEMPLATE:
      return requite::AttributeType::TEMPLATE;
    case requite::Opcode::PRIVATE:
      return requite::AttributeType::PRIVATE;
    case requite::Opcode::PROTECTED:
      return requite::AttributeType::PROTECTED;
    case requite::Opcode::EXPORT:
      return requite::AttributeType::EXPORT;
    case requite::Opcode::MAY_THROW:
      return requite::AttributeType::MAY_THROW;
    case requite::Opcode::EXTERNAL:
      return requite::AttributeType::EXTERNAL;
    case requite::Opcode::C:
      return requite::AttributeType::C;
    case requite::Opcode::NOT_FINAL:
      return requite::AttributeType::NOT_FINAL;
    case requite::Opcode::MAY_DISCARD:
      return requite::AttributeType::MAY_DISCARD;
    case requite::Opcode::NO_SHORT_CIRCUIT:
      return requite::AttributeType::NO_SHORT_CIRCUIT;
    case requite::Opcode::NO_REMAINDER:
      return requite::AttributeType::NO_REMAINDER;
    case requite::Opcode::INLINE:
      return requite::AttributeType::INLINE;
    case requite::Opcode::MANGLED_NAME:
      return requite::AttributeType::MANGLED_NAME;
    case requite::Opcode::LABEL:
      return requite::AttributeType::LABEL;
    case requite::Opcode::PACK:
      return requite::AttributeType::PACK;
    default:
      break;
  }
  return requite::AttributeType::NONE;
}


constexpr std::string_view getName(requite::AttributeType type) {
  switch (type) {
  case requite::AttributeType::NONE:
    return "none";
  case requite::AttributeType::VOLATILE:
    return "volatile";
  case requite::AttributeType::ATOMIC:
    return "atomic";
  case requite::AttributeType::NULL_TERMINATED:
    return "null_terminated";
  case requite::AttributeType::TEMPLATE:
    return "template";
  case requite::AttributeType::PRIVATE:
    return "private";
  case requite::AttributeType::PROTECTED:
    return "protected";
  case requite::AttributeType::EXPORT:
    return "export";
  case requite::AttributeType::MAY_THROW:
    return "may_throw";
  case requite::AttributeType::EXTERNAL:
    return "external";
  case requite::AttributeType::C:
    return "c";
  case requite::AttributeType::NOT_FINAL:
    return "not_final";
  case requite::AttributeType::MAY_DISCARD:
    return "may_discard";
  case requite::AttributeType::NO_SHORT_CIRCUIT:
    return "no_short_circuit";
  case requite::AttributeType::NO_REMAINDER:
    return "no_remainder";
  case requite::AttributeType::INLINE:
    return "inline";
  case requite::AttributeType::MANGLED_NAME:
    return "mangled_name";
  case requite::AttributeType::LABEL:
    return "label";
  case requite::AttributeType::PACK:
    return "pack";
  case requite::AttributeType::_LAST:
    break;
  }
  return "unknown";
}

} // namespace requite