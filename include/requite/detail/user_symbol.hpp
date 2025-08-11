#pragma once

namespace requite {

[[nodiscard]] inline constexpr std::string_view
getName(requite::UserSymbolType type) {
  switch (type) {
  case requite::UserSymbolType::NONE:
    return "none";
  case requite::UserSymbolType::GLOBAL:
    return "global";
  case requite::UserSymbolType::LOCAL:
    return "local";
  case requite::UserSymbolType::OBJECT:
    return "object";
  case requite::UserSymbolType::PROPERTY:
    return "property";
  case requite::UserSymbolType::PROCEDURE:
    return "procedure";
  }
  return "error";
}

} // namespace requite