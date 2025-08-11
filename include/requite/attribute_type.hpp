// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_category.hpp>
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
  OWNING,
  PRIVATE,
  PROTECTED,
  EXPORT,
  BAKE,
  MAY_PARENT,
  PARENT,
  POSITION,
  MAY_DISCARD,
  INLINE,
  MANGLED_NAME,
  PACK,
  LABEL,
  TEMPLATE,

  _LAST
};

static constexpr unsigned ATTRIBUTE_TYPE_COUNT =
    static_cast<unsigned>(AttributeType::_LAST);

[[nodiscard]] constexpr requite::AttributeType getAttributeType(requite::Opcode opcode);
[[nodiscard]] constexpr std::string_view getName(requite::AttributeType type);
template <requite::AttributeCategory CATEGORY_PARAM>
[[nodiscard]] constexpr bool getCanBeAttributeCategory(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeTypeAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeGlobalFunctionAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberFunctionAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeLocalFunctionAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberConstructorAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberDestructorAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeGlobalObjectAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberObjectAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeLocalObjectAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberPropertyAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeGlobalGlobalAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberGlobalAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeGlobalUseAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberUseAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeGlobalUseTableAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeMemberUseTableAttribute(requite::AttributeType type);
[[nodiscard]] constexpr bool getCanBeImportAttribute(requite::AttributeType type);

} // namespace requite

#include <requite/detail/attribute_type.hpp>