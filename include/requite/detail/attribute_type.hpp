// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>
#include <requite/situation.hpp>

namespace requite {

constexpr requite::AttributeType getAttributeType(requite::Opcode opcode) {
  REQUITE_ASSERT(requite::getCanBeMatteValueSituation(opcode));
  switch (opcode) {
  case requite::Opcode::MUTABLE:
    return requite::AttributeType::MUTABLE;
  case requite::Opcode::CONSTANT:
    return requite::AttributeType::CONSTANT;
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
  case requite::Opcode::EXTERNAL:
    return requite::AttributeType::EXTERNAL;
  case requite::Opcode::NOT_FINAL:
    return requite::AttributeType::NOT_FINAL;
  case requite::Opcode::MAY_DISCARD:
    return requite::AttributeType::MAY_DISCARD;
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
  return requite::AttributeType::USER;
}

constexpr std::string_view getName(requite::AttributeType type) {
  switch (type) {
  case requite::AttributeType::NONE:
    return "none";
  case requite::AttributeType::USER:
    return "user defined";
  case requite::AttributeType::MUTABLE:
    return "bulitin mutable";
  case requite::AttributeType::CONSTANT:
    return "builtin constant";
  case requite::AttributeType::VOLATILE:
    return "builtin volatile";
  case requite::AttributeType::ATOMIC:
    return "builtin atomic";
  case requite::AttributeType::NULL_TERMINATED:
    return "builtin null_terminated";
  case requite::AttributeType::TEMPLATE:
    return "builtin template";
  case requite::AttributeType::PRIVATE:
    return "builtin private";
  case requite::AttributeType::PROTECTED:
    return "builtin protected";
  case requite::AttributeType::EXPORT:
    return "builtin export";
  case requite::AttributeType::EXTERNAL:
    return "builtin external";
  case requite::AttributeType::NOT_FINAL:
    return "builtin not_final";
  case requite::AttributeType::MAY_DISCARD:
    return "builtin may_discard";
  case requite::AttributeType::INLINE:
    return "builtin inline";
  case requite::AttributeType::MANGLED_NAME:
    return "builtin mangled_name";
  case requite::AttributeType::LABEL:
    return "builtin label";
  case requite::AttributeType::PACK:
    return "builtin pack";
  case requite::AttributeType::_LAST:
    break;
  }
  return "unknown";
}

constexpr bool getIsTypeAttribute(requite::AttributeType type) {
  switch (type) {
  case requite::AttributeType::NONE:
    return false;
  case requite::AttributeType::USER:
    return false;
  case requite::AttributeType::MUTABLE:
    return true;
  case requite::AttributeType::CONSTANT:
    return true;
  case requite::AttributeType::VOLATILE:
    return true;
  case requite::AttributeType::NULL_TERMINATED:
    return true;
  case requite::AttributeType::TEMPLATE:
    return false;
  case requite::AttributeType::PRIVATE:
    return false;
  case requite::AttributeType::PROTECTED:
    return false;
  case requite::AttributeType::EXPORT:
    return false;
  case requite::AttributeType::EXTERNAL:
    return false;
  case requite::AttributeType::NOT_FINAL:
    return false;
  case requite::AttributeType::MAY_DISCARD:
    return false;
  case requite::AttributeType::INLINE:
    return false;
  case requite::AttributeType::MANGLED_NAME:
    return false;
  case requite::AttributeType::LABEL:
    return false;
  case requite::AttributeType::PACK:
    return false;
  default:
    break;
  }
  return false;
}

} // namespace requite