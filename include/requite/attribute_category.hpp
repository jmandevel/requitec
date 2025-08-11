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
  GLOBAL_GLOBAL,
  MEMBER_GLOBAL,
  GLOBAL_USE,
  MEMBER_USE,
  GLOBAL_USE_TABLE,
  MEMBER_USE_TABLE,
  IMPORT
};

[[nodiscard]] constexpr std::string_view
getDescription(requite::AttributeCategory category);

} // namespace requite

#include <requite/detail/attribute_category.hpp>