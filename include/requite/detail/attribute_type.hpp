// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/situation.hpp>
#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

#include <cstdint>

namespace requite {

constexpr requite::AttributeType getAttributeType(requite::Opcode opcode) {
  using namespace requite;
  using A = AttributeType;
  using O = Opcode;
  REQUITE_ASSERT(getCanBeAttribute(opcode));
  switch (opcode) {
  case O::MUTABLE:
    return A::MUTABLE;
  case O::CONSTANT:
    return A::CONSTANT;
  case O::VOLATILE:
    return A::VOLATILE;
  case O::ATOMIC:
    return A::ATOMIC;
  case O::NULL_TERMINATED:
    return A::NULL_TERMINATED;
  case O::OWNING:
    return A::OWNING;
  case O::PRIVATE:
    return A::PRIVATE;
  case O::PROTECTED:
    return A::PROTECTED;
  case O::EXPORT:
    return A::EXPORT;
  case O::BAKE:
    return A::BAKE;
  case O::MAY_PARENT:
    return A::MAY_PARENT;
  case O::PARENT:
    return A::PARENT;
  case O::POSITION:
    return A::POSITION;
  case O::MAY_DISCARD:
    return A::MAY_DISCARD;
  case O::INLINE:
    return A::INLINE;
  case O::MANGLED_NAME:
    return A::MANGLED_NAME;
  case O::PACK:
    return A::PACK;
  case O::USER:
    return A::USER;
  case O::TEMPLATE:
    return A::TEMPLATE;
  case O::LABEL:
    return A::LABEL;
  default:
    break;
  }
  return A::NONE;
}

constexpr std::string_view getName(requite::AttributeType type) {
  using namespace requite;
  using A = AttributeType;
  switch (type) {
  case A::NONE:
    return "none";
  case A::USER:
    return "user";
  case A::MUTABLE:
    return "mutable";
  case A::CONSTANT:
    return "constant";
  case A::VOLATILE:
    return "volatile";
  case A::ATOMIC:
    return "atomic";
  case A::NULL_TERMINATED:
    return "null_terminated";
  case A::OWNING:
    return "owning";
  case A::PRIVATE:
    return "private";
  case A::PROTECTED:
    return "protected";
  case A::EXPORT:
    return "export";
  case A::BAKE: 
    return "bake";
  case A::MAY_PARENT:
    return "may_parent";
  case A::PARENT:
    return "parent";
  case A::POSITION:
    return "position";
  case A::MAY_DISCARD:
    return "may_discard";
  case A::INLINE:
    return "inline";
  case A::MANGLED_NAME:
    return "mangled_name";
  case A::PACK:
    return "pack";
  case A::LABEL:
    return "label";
  case A::TEMPLATE:
    return "template";
  case A::_LAST:
    break;
  }
  return "none";
}

namespace _attribute {
enum _AttributeFlags : std::uint32_t {
  _NONE = 0,
  _TYPE = requite::getBit(31),
  _GLOBAL_FUNCTION = requite::getBit(30),
  _MEMBER_FUNCTION = requite::getBit(29),
  _LOCAL_FUNCTION = requite::getBit(28),
  _MEMBER_CONSTRUCTOR = requite::getBit(27),
  _MEMBER_DESTRUCTOR = requite::getBit(16),
  _GLOBAL_OBJECT = requite::getBit(25),
  _MEMBER_OBJECT = requite::getBit(24),
  _LOCAL_OBJECT = requite::getBit(23),
  _MEMBER_PROPERTY = requite::getBit(22),
  _GLOBAL_GLOBAL = requite::getBit(21),
  _MEMBER_GLOBAL = requite::getBit(20),
  _GLOBAL_USE = requite::getBit(19),
  _MEMBER_USE = requite::getBit(18),
  _LOCAL_USE = requite::getBit(17),
  _GLOBAL_USE_TABLE = requite::getBit(16),
  _MEMBER_USE_TABLE = requite::getBit(15),
  _IMPORT = requite::getBit(14)
};
}

[[nodiscard]] constexpr requite::_attribute::_AttributeFlags
operator|(requite::_attribute::_AttributeFlags lhs,
          requite::_attribute::_AttributeFlags rhs) {
  using Underlying =
      std::underlying_type_t<requite::_attribute::_AttributeFlags>;
  return static_cast<requite::_attribute::_AttributeFlags>(
      static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

[[nodiscard]] constexpr requite::_attribute::_AttributeFlags
_getFlags(requite::AttributeType type) {
  using namespace requite;
  using namespace requite::_attribute;
  using A = AttributeType;
  switch (type) {
  case A::NONE:
    return _NONE;
  case A::USER:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR | _GLOBAL_OBJECT |
           _MEMBER_OBJECT | _LOCAL_OBJECT | _MEMBER_PROPERTY | _GLOBAL_GLOBAL | _MEMBER_GLOBAL | _MEMBER_USE |
           _IMPORT;
  case A::MUTABLE:
    return _TYPE;
  case A::CONSTANT:
    return _TYPE;
  case A::VOLATILE:
    return _TYPE;
  case A::ATOMIC:
    return _TYPE;
  case A::NULL_TERMINATED:
    return _TYPE;
  case A::OWNING:
    return _TYPE;
  case A::PRIVATE:
    return _MEMBER_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR |
           _MEMBER_PROPERTY | _MEMBER_GLOBAL | _MEMBER_USE |
           _MEMBER_USE_TABLE;
  case A::PROTECTED:
    return _MEMBER_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR |
           _MEMBER_PROPERTY | _MEMBER_GLOBAL | _MEMBER_USE |
           _MEMBER_USE_TABLE;
  case A::EXPORT:
    return _GLOBAL_FUNCTION | _GLOBAL_OBJECT | _GLOBAL_GLOBAL |
           _GLOBAL_USE | _IMPORT | _GLOBAL_USE_TABLE;
  case A::BAKE:
    return _NONE;
  case A::MAY_PARENT:
    return _GLOBAL_OBJECT | _MEMBER_OBJECT | _LOCAL_OBJECT;
  case A::PARENT:
    return _NONE;
  case A::POSITION:
    return _NONE;
  case A::MAY_DISCARD:
    return _TYPE;
  case A::INLINE:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _LOCAL_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR;
  case A::MANGLED_NAME:
    return _NONE;
  case A::PACK:
    return _NONE;
  case A::LABEL:
    return _NONE;
  case A::TEMPLATE:
    return _NONE; // TODO
  case A::_LAST:
    break;
  }
  return _NONE;
}

[[nodiscard]] constexpr bool
_getHasFlags(requite::AttributeType type,
             requite::_attribute::_AttributeFlags flags) {
  requite::_attribute::_AttributeFlags attribute_flags =
      requite::_getFlags(type);
  const bool has_flags = (attribute_flags & flags) == flags;
  return has_flags;
}

template <requite::AttributeCategory CATEGORY_PARAM>
constexpr bool getCanBeAttributeCategory(requite::AttributeType type) {
  if constexpr (CATEGORY_PARAM == requite::AttributeCategory::TYPE) {
    return requite::getCanBeNarrowAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::GLOBAL_FUNCTION) {
    return requite::getCanBeGlobalFunctionAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_FUNCTION) {
    return requite::getCanBeMemberFunctionAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::LOCAL_FUNCTION) {
    return requite::getCanBeLocalFunctionAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_CONSTRUCTOR) {
    return requite::getCanBeMemberConstructorAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_DESTRUCTOR) {
    return requite::getCanBeMemberDestructorAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::GLOBAL_OBJECT) {
    return requite::getCanBeGlobalObjectAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_OBJECT) {
    return requite::getCanBeMemberObjectAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::LOCAL_OBJECT) {
    return requite::getCanBeLocalObjectAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_PROPERTY) {
    return requite::getCanBeMemberPropertyAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::GLOBAL_GLOBAL) {
    return requite::getCanBeGlobalGlobalAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_GLOBAL) {
    return requite::getCanBeMemberGlobalAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::GLOBAL_USE) {
    return requite::getCanBeGlobalUseAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_USE) {
    return requite::getCanBeMemberUseAttribute(type);

  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::GLOBAL_USE_TABLE) {
    return requite::getCanBeGlobalUseTableAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_USE_TABLE) {
    return requite::getCanBeMemberUseTableAttribute(type);

  } else if constexpr (CATEGORY_PARAM == requite::AttributeCategory::IMPORT) {
    return requite::getCanBeImportAttribute(type);
  } else {
    static_assert(false, "invalid category");
  }
}

constexpr bool getCanBeNarrowAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_TYPE);
  return has_flags;
}

constexpr bool getCanBeGlobalFunctionAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_FUNCTION);
  return has_flags;
}

constexpr bool getCanBeMemberFunctionAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_FUNCTION);
  return has_flags;
}

constexpr bool getCanBeLocalFunctionAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_LOCAL_FUNCTION);
  return has_flags;
}

constexpr bool getCanBeMemberConstructorAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_CONSTRUCTOR);
  return has_flags;
}

constexpr bool getCanBeMemberDestructorAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_DESTRUCTOR);
  return has_flags;
}

constexpr bool getCanBeGlobalObjectAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_OBJECT);
  return has_flags;
}

constexpr bool getCanBeMemberObjectAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_OBJECT);
  return has_flags;
}

constexpr bool getCanBeLocalObjectAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_LOCAL_OBJECT);
  return has_flags;
}

constexpr bool getCanBeMemberPropertyAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_PROPERTY);
  return has_flags;
}

constexpr bool getCanBeGlobalGlobalAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_GLOBAL);
  return has_flags;
}

constexpr bool getCanBeMemberGlobalAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_GLOBAL);
  return has_flags;
}

constexpr bool getCanBeGlobalUseAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_USE);
  return has_flags;
}

constexpr bool getCanBeMemberUseAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_USE);
  return has_flags;
}

constexpr bool getCanBeGlobalUseTableAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_USE_TABLE);
  return has_flags;
}

constexpr bool getCanBeMemberUseTableAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_USE_TABLE);
  return has_flags;
}

constexpr bool getCanBeImportAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_IMPORT);
  return has_flags;
}

} // namespace requite