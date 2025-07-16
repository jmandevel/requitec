#pragma once

namespace requite {

template <requite::UserSymbolType TYPE_PARAM>
bool getIsValidUserSymbolAttribute(requite::ScopeType scope_type,
                                   requite::AttributeType attribute_type) {
  return true; // TODO
}

[[nodiscard]] inline constexpr std::string_view
getName(requite::UserSymbolType type) {
  switch (type) {
  case requite::UserSymbolType::MODULE:
    return "module";
  case requite::UserSymbolType::ALIAS:
    return "alias";
  case requite::UserSymbolType::GLOBAL:
    return "global";
  case requite::UserSymbolType::IMPORT:
    return "import";
  case requite::UserSymbolType::LABEL:
    return "label";
  case requite::UserSymbolType::LOCAL:
    return "local";
  case requite::UserSymbolType::OBJECT:
    return "object";
  case requite::UserSymbolType::TABLE:
    return "table";
  case requite::UserSymbolType::PROPERTY:
    return "property";
  case requite::UserSymbolType::USE:
    return "use";
  case requite::UserSymbolType::FUNCTION:
    return "function";
  case requite::UserSymbolType::METHOD:
    return "method";
  case requite::UserSymbolType::EXTENSION:
    return "extension";
  case requite::UserSymbolType::CONSTRUCTOR:
    return "constructor";
  case requite::UserSymbolType::DESTRUCTOR:
    return "destructor";
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite