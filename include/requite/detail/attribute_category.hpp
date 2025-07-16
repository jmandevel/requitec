#pragma once

namespace requite {

constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category) {
  switch (category) {
  case requite::AttributeCategory::TYPE:
    return " is not type attribute";
  case requite::AttributeCategory::FUNCTION:
    return " is not function attribute";
  case requite::AttributeCategory::MEMBER_FUNCTION:
    return " is not member function attribute";
  case requite::AttributeCategory::MEMBER_METHOD:
    return " is not member method attribute";
  case requite::AttributeCategory::EXTENSION:
    return " attribute is not extension attribute";
  case requite::AttributeCategory::MEMBER_EXTENSION:
    return " is not member extension attribute";
  case requite::AttributeCategory::MEMBER_CONSTRUCTOR:
    return " is not member constructor attribute";
  case requite::AttributeCategory::MEMBER_DESTRUCTOR:
    return " is not member destructor attribute";
  case requite::AttributeCategory::OBJECT:
    return " is not object attribute";
  case requite::AttributeCategory::MEMBER_OBJECT:
    return " is not member object attribute";
  case requite::AttributeCategory::MEMBER_PROPERTY:
    return " is not member property attribute";
  case requite::AttributeCategory::ALIAS:
    return " is not alias attribute";
  case requite::AttributeCategory::MEMBER_ALIAS:
    return " is not member alias attribute";
  case requite::AttributeCategory::GLOBAL:
    return " is not global attribute";
  case requite::AttributeCategory::MEMBER_GLOBAL:
    return " is not member global attribute";
  case requite::AttributeCategory::USE:
    return " is not use attribute";
  case requite::AttributeCategory::MEMBER_USE:
    return " is not member use attribute";
  case requite::AttributeCategory::IMPORT:
    return " is not import attribute";
  case requite::AttributeCategory::MEMBER_IMPORT:
    return " is not member import attribute";
  case requite::AttributeCategory::BLOCK:
    return " is not block attribute";
  }
  return " is not valid attribute";
}

} // namespace requite