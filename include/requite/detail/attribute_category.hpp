#pragma once

namespace requite {

constexpr std::string_view
getErrorMessageEnding(requite::AttributeCategory category) {
  switch (category) {
  case requite::AttributeCategory::TYPE:
    return " is not type attribute";
  case requite::AttributeCategory::GLOBAL_FUNCTION:
    return " is not global function attribute";
  case requite::AttributeCategory::MEMBER_FUNCTION:
    return " is not member function attribute";
  case requite::AttributeCategory::LOCAL_FUNCTION:
    return " is not local function attribute";
  case requite::AttributeCategory::MEMBER_CONSTRUCTOR:
    return " is not member constructor attribute";
  case requite::AttributeCategory::MEMBER_DESTRUCTOR:
    return " is not member destructor attribute";
  case requite::AttributeCategory::GLOBAL_OBJECT:
    return " is not global object attribute";
  case requite::AttributeCategory::MEMBER_OBJECT:
    return " is not member object attribute";
  case requite::AttributeCategory::LOCAL_OBJECT:
    return " is not local object attribute";
  case requite::AttributeCategory::MEMBER_PROPERTY:
    return " is not member property attribute";
  case requite::AttributeCategory::GLOBAL_ALIAS:
    return " is not global alias attribute";
  case requite::AttributeCategory::MEMBER_ALIAS:
    return " is not member alias attribute";
  case requite::AttributeCategory::LOCAL_ALIAS:
    return " is not local alias attribute";
  case requite::AttributeCategory::GLOBAL_GLOBAL:
    return " is not global global attribute";
  case requite::AttributeCategory::MEMBER_GLOBAL:
    return " is not member global attribute";
  case requite::AttributeCategory::GLOBAL_USE:
    return " is not global use attribute";
  case requite::AttributeCategory::MEMBER_USE:
    return " is not member use attribute";
  case requite::AttributeCategory::GLOBAL_TABLE_ALIAS:
    return " is not global table alias attribute";
  case requite::AttributeCategory::MEMBER_TABLE_ALIAS:
    return " is not member table alias attribute";
  case requite::AttributeCategory::GLOBAL_TABLE_USE:
    return " is not global table use attribute";
  case requite::AttributeCategory::MEMBER_TABLE_USE:
    return " is not member table use attribute";
  case requite::AttributeCategory::IMPORT:
    return " is not import attribute";
  case requite::AttributeCategory::LOCAL_BLOCK:
    return " is not local block attribute";
  }
  return " is not valid attribute";
}

} // namespace requite