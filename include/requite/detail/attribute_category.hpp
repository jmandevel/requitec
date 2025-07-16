#pragma once

namespace requite {

constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category) {
  switch (category) {
  case requite::AttributeCategory::TYPE:
    return " attribute is not type attribute";
  case requite::AttributeCategory::FUNCTION:
    return " attribute is not function attribute";
  case requite::AttributeCategory::MEMBER_FUNCTION:
    return " attribute is not member function attribute";
  case requite::AttributeCategory::MEMBER_METHOD:
    return " attribute is not member method attribute";
  case requite::AttributeCategory::EXTENSION:
    return " attribute is not extension attribute";
  case requite::AttributeCategory::MEMBER_EXTENSION:
    return " attribute is not member extension attribute";
  case requite::AttributeCategory::MEMBER_CONSTRUCTOR:
    return " attribute is not member constructor attribute";
  case requite::AttributeCategory::MEMBER_DESTRUCTOR:
    return " attribute is not member destructor attribute";
  case requite::AttributeCategory::OBJECT:
    return " attribute is not object attribute";
  case requite::AttributeCategory::MEMBER_OBJECT:
    return " attribute is not member object attribute";
  case requite::AttributeCategory::MEMBER_PROPERTY:
    return " attribute is not member property attribute";
  case requite::AttributeCategory::ALIAS:
    return " attribute is not alias attribute";
  case requite::AttributeCategory::MEMBER_ALIAS:
    return " attribute is not member alias attribute";
  case requite::AttributeCategory::GLOBAL:
    return " attribute is not global attribute";
  case requite::AttributeCategory::MEMBER_GLOBAL:
    return " attribute is not member global attribute";
  case requite::AttributeCategory::USE:
    return " attribute is not use attribute";
  case requite::AttributeCategory::MEMBER_USE:
    return " attribute is not member use attribute";
  case requite::AttributeCategory::IMPORT:
    return " attribute is not import attribute";
  case requite::AttributeCategory::MEMBER_IMPORT:
    return " attribute is not member import attribute";
  case requite::AttributeCategory::MATTE_LOCAL_STATEMENT:
    return " attribute is not matte local statement attribute";
  }
  return " attribute is not valid";
}

} // namespace requite