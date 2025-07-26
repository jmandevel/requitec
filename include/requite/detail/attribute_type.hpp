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
  case requite::Opcode::OWNING:
    return requite::AttributeType::OWNING;
  case requite::Opcode::TEMPLATE:
    return requite::AttributeType::TEMPLATE;
  case requite::Opcode::PRIVATE:
    return requite::AttributeType::PRIVATE;
  case requite::Opcode::PROTECTED:
    return requite::AttributeType::PROTECTED;
  case requite::Opcode::EXPORT:
    return requite::AttributeType::EXPORT;
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
    return "user defined attribute";
  case requite::AttributeType::MUTABLE:
    return "mutable";
  case requite::AttributeType::CONSTANT:
    return "constant";
  case requite::AttributeType::VOLATILE:
    return "volatile";
  case requite::AttributeType::ATOMIC:
    return "atomic";
  case requite::AttributeType::NULL_TERMINATED:
    return "null_terminated";
  case requite::AttributeType::OWNING:
    return "owning";
  case requite::AttributeType::TEMPLATE:
    return "template";
  case requite::AttributeType::PRIVATE:
    return "private";
  case requite::AttributeType::PROTECTED:
    return "protected";
  case requite::AttributeType::EXPORT:
    return "export";
  case requite::AttributeType::NOT_FINAL:
    return "not_final";
  case requite::AttributeType::MAY_DISCARD:
    return "may_discard";
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

namespace _attribute {
enum _AttributeFlags : std::uint32_t {
  _NONE = 0,
  _TYPE = requite::getBit(0),
  _GLOBAL_FUNCTION = requite::getBit(1),
  _MEMBER_FUNCTION = requite::getBit(2),
  _LOCAL_FUNCTION = requite::getBit(3),
  _MEMBER_CONSTRUCTOR = requite::getBit(4),
  _MEMBER_DESTRUCTOR = requite::getBit(5),
  _GLOBAL_OBJECT = requite::getBit(6),
  _MEMBER_OBJECT = requite::getBit(7),
  _LOCAL_OBJECT = requite::getBit(8),
  _MEMBER_PROPERTY = requite::getBit(9),
  _GLOBAL_ALIAS = requite::getBit(10),
  _MEMBER_ALIAS = requite::getBit(11),
  _LOCAL_ALIAS = requite::getBit(12),
  _GLOBAL_GLOBAL = requite::getBit(13),
  _MEMBER_GLOBAL = requite::getBit(14),
  _GLOBAL_USE = requite::getBit(15),
  _MEMBER_USE = requite::getBit(16),
  _LOCAL_USE = requite::getBit(17),
  _GLOBAL_USE_TABLE = requite::getBit(18),
  _MEMBER_USE_TABLE = requite::getBit(19),
  _IMPORT = requite::getBit(20),
  _LOCAL_BLOCK = requite::getBit(21)
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
  switch (type) {
  case AttributeType::NONE:
    return _NONE;
  case AttributeType::USER:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR | _GLOBAL_OBJECT |
           _MEMBER_OBJECT | _LOCAL_OBJECT | _MEMBER_PROPERTY | _GLOBAL_ALIAS |
           _MEMBER_ALIAS | _GLOBAL_GLOBAL | _MEMBER_GLOBAL | _MEMBER_USE |
           _IMPORT;
  case AttributeType::MUTABLE:
    return _TYPE;
  case AttributeType::CONSTANT:
    return _TYPE;
  case AttributeType::VOLATILE:
    return _TYPE;
  case AttributeType::ATOMIC:
    return _TYPE;
  case AttributeType::NULL_TERMINATED:
    return _TYPE;
  case AttributeType::OWNING:
    return _TYPE;
  case AttributeType::TEMPLATE:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _MEMBER_CONSTRUCTOR | _GLOBAL_OBJECT | _MEMBER_OBJECT |
           _GLOBAL_ALIAS | _MEMBER_ALIAS | _LOCAL_ALIAS | _GLOBAL_GLOBAL |
           _MEMBER_GLOBAL;
  case AttributeType::PRIVATE:
    return _MEMBER_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR |
           _MEMBER_PROPERTY | _MEMBER_ALIAS | _MEMBER_GLOBAL | _MEMBER_USE |
           _MEMBER_USE_TABLE;
  case AttributeType::PROTECTED:
    return _MEMBER_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR |
           _MEMBER_PROPERTY | _MEMBER_ALIAS | _MEMBER_GLOBAL | _MEMBER_USE |
           _MEMBER_USE_TABLE;
  case AttributeType::EXPORT:
    return _GLOBAL_FUNCTION | _GLOBAL_OBJECT | _GLOBAL_ALIAS | _GLOBAL_GLOBAL |
           _GLOBAL_USE | _IMPORT | _GLOBAL_USE_TABLE;
  case AttributeType::NOT_FINAL:
    return _GLOBAL_OBJECT | _MEMBER_OBJECT | _LOCAL_OBJECT;
  case AttributeType::MAY_DISCARD:
    return _TYPE;
  case AttributeType::INLINE:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _LOCAL_FUNCTION | _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR;
  case AttributeType::MANGLED_NAME:
    return _GLOBAL_FUNCTION | _MEMBER_FUNCTION | _LOCAL_FUNCTION |
           _MEMBER_CONSTRUCTOR | _MEMBER_DESTRUCTOR;
  case AttributeType::LABEL:
    return _LOCAL_BLOCK;
  case AttributeType::PACK:
    return _GLOBAL_OBJECT | _MEMBER_OBJECT | _LOCAL_OBJECT;
  case AttributeType::_LAST:
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
    return requite::getCanBeTypeAttribute(type);
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
                       requite::AttributeCategory::GLOBAL_ALIAS) {
    return requite::getCanBeGlobalAliasAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::MEMBER_ALIAS) {
    return requite::getCanBeMemberAliasAttribute(type);
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::LOCAL_ALIAS) {
    return requite::getCanBeLocalAliasAttribute(type);
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
  } else if constexpr (CATEGORY_PARAM ==
                       requite::AttributeCategory::LOCAL_BLOCK) {
    return requite::getCanBeLocalBlockAttribute(type);
  } else {
    static_assert(false, "invalid category");
  }
}

constexpr bool getCanBeTypeAttribute(requite::AttributeType type) {
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

constexpr bool getCanBeGlobalAliasAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_GLOBAL_ALIAS);
  return has_flags;
}

constexpr bool getCanBeMemberAliasAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_MEMBER_ALIAS);
  return has_flags;
}

constexpr bool getCanBeLocalAliasAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_LOCAL_ALIAS);
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

constexpr bool getCanBeLocalBlockAttribute(requite::AttributeType type) {
  const bool has_flags =
      requite::_getHasFlags(type, requite::_attribute::_LOCAL_BLOCK);
  return has_flags;
}

} // namespace requite