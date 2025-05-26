// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <string_view>

namespace requite {

enum class AttributeType {
  NONE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  TEMPLATE,
  PRIVATE,
  PROTECTED,
  EXPORT,
  MAY_THROW,
  THROWS,
  EXTERNAL,
  C,
  POSITIONAL_ARGUMENTS,
  NOT_FINAL,
  MAY_DISCARD,
  NO_SHORT_CIRCUIT,
  NO_REMAINDER,
  INLINE,
  MANGLED_NAME,
  LABEL,
  PACK,

  _LAST
};

static constexpr unsigned ATTRIBUTE_TYPE_COUNT =
    static_cast<unsigned>(AttributeType::_LAST);

[[nodiscard]] constexpr requite::AttributeType getAttributeType(requite::Opcode opcode);

[[nodiscard]] constexpr std::string_view getName(requite::AttributeType type);

} // namespace requite

#include <requite/detail/attribute_type.hpp>