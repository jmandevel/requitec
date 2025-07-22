#pragma once

#include <string_view>

namespace requite {

enum class AttributeCategory {
  TYPE,
  FUNCTION,
  MEMBER_FUNCTION,
  MEMBER_METHOD,
  EXTENSION,
  MEMBER_EXTENSION,
  MEMBER_CONSTRUCTOR,
  MEMBER_DESTRUCTOR,
  OBJECT,
  MEMBER_OBJECT,
  MEMBER_PROPERTY,
  ALIAS,
  MEMBER_ALIAS,
  GLOBAL,
  MEMBER_GLOBAL,
  USE,
  PULL,
  MEMBER_USE,
  IMPORT,
  BLOCK
};

// NOTE: the error message is meant to be concatinated after the name of the
// attribute type, obtainable with requite::getName(requite::AttributeType)
[[nodiscard]] constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category);

} // namespace requite

#include <requite/detail/attribute_category.hpp>