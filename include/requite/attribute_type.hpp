// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <string_view>

namespace requite {

enum class AttributeType {
  NONE,
  USER,
  MUTABLE,
  CONSTANT,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  TEMPLATE,
  PRIVATE,
  PROTECTED,
  EXPORT,
  EXTERNAL,
  NOT_FINAL,
  MAY_DISCARD,
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

[[nodiscard]] constexpr bool getIsTypeAttribute(requite::AttributeType type);

} // namespace requite

#include <requite/detail/attribute_type.hpp>