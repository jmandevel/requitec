#pragma once

namespace requite {

constexpr std::string_view getDescription(requite::AttributeCategory category) {
  using namespace requite;
  using C = AttributeCategory;
  switch (category) {
  case C::TYPE:
    return "type attribute";
  case C::GLOBAL_FUNCTION:
    return "global function attribute";
  case C::MEMBER_FUNCTION:
    return "member function attribute";
  case C::LOCAL_FUNCTION:
    return "local function attribute";
  case C::MEMBER_CONSTRUCTOR:
    return "member constructor attribute";
  case C::MEMBER_DESTRUCTOR:
    return "member destructor attribute";
  case C::GLOBAL_OBJECT:
    return "global object attribute";
  case C::MEMBER_OBJECT:
    return "member object attribute";
  case C::LOCAL_OBJECT:
    return "local object attribute";
  case C::MEMBER_PROPERTY:
    return "member property attribute";
  case C::GLOBAL_GLOBAL:
    return "global global attribute";
  case C::MEMBER_GLOBAL:
    return "member global attribute";
  case C::GLOBAL_USE:
    return "global use attribute";
  case C::MEMBER_USE:
    return "member use attribute";
  case C::GLOBAL_USE_TABLE:
    return "global use table attribute";
  case C::MEMBER_USE_TABLE:
    return "member use table attribute";
  case C::IMPORT:
    return "import attribute";
  }
  return "valid attribute";
}

} // namespace requite