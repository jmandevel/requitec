#pragma once

#include <string_view>

namespace requite {

enum class AttributeCategory {
  TYPE,
  FUNCTION,
  MEMBER_FUNCTION,
  LOCAL_FUNCTION,
  MEMBER_METHOD,
  EXTENSION,
  MEMBER_EXTENSION,
  LOCAL_EXTENSION,
  MEMBER_CONSTRUCTOR,
  MEMBER_DESTRUCTOR,
  OBJECT,
  MEMBER_OBJECT,
  LOCAL_OBJECT,
  MEMBER_PROPERTY,
  ALIAS,
  MEMBER_ALIAS,
  LOCAL_ALIAS,
  GLOBAL,
  MEMBER_GLOBAL,
  LOCAL_GLOBAL,
  USE,
  MEMBER_USE,
  LOCAL_USE,
  TABLE_ALIAS,
  MEMBER_TABLE_ALIAS,
  LOCAL_TABLE_ALIAS,
  TABLE_USE,
  MEMBER_TABLE_USE,
  LOCAL_TABLE_USE,
  IMPORT,
  BLOCK
};

// NOTE: the error message is meant to be concatinated after the name of the
// attribute type, obtainable with requite::getName(requite::AttributeType)
[[nodiscard]] constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category);

} // namespace requite

#include <requite/detail/attribute_category.hpp>