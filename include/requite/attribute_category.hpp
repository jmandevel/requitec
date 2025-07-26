#pragma once

#include <string_view>

namespace requite {

enum class AttributeCategory {
  TYPE,
  GLOBAL_FUNCTION,
  MEMBER_FUNCTION,
  LOCAL_FUNCTION,
  MEMBER_CONSTRUCTOR,
  MEMBER_DESTRUCTOR,
  GLOBAL_OBJECT,
  MEMBER_OBJECT,
  LOCAL_OBJECT,
  MEMBER_PROPERTY,
  GLOBAL_ALIAS,
  MEMBER_ALIAS,
  LOCAL_ALIAS,
  GLOBAL_GLOBAL,
  MEMBER_GLOBAL,
  GLOBAL_USE,
  MEMBER_USE,
  GLOBAL_USE_TABLE,
  MEMBER_USE_TABLE,
  IMPORT,
  LOCAL_BLOCK
};

// NOTE: the error message is meant to be concatinated after the name of the
// attribute type, obtainable with requite::getName(requite::AttributeType)
[[nodiscard]] constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category);

} // namespace requite

#include <requite/detail/attribute_category.hpp>