#pragma once

#include <requite/attribute_type.hpp>
#include <requite/scope_type.hpp>
#include <requite/symbol.hpp>

#include <string_view>

namespace requite {

struct Use;
struct Import;

enum class UserSymbolType {
  MODULE,
  ALIAS,
  GLOBAL,
  IMPORT,
  LABEL,
  LOCAL,
  OBJECT,
  TABLE,
  PROPERTY,
  USE,
  FUNCTION,
  METHOD,
  EXTENSION,
  CONSTRUCTOR,
  DESTRUCTOR
};

enum class UserSymbolOrigin { INTERNAL, USED, IMPORTED };

struct UserSymbol final {
  using Self = UserSymbol;

  requite::RootSymbol _symbol;
  requite::UserSymbolOrigin _origin;
  union {
    void *_nothing_ptr = nullptr;
    requite::Use *_use_ptr;
    requite::Import *_import_ptr;
  };

  // user_symbol.cpp
  UserSymbol(requite::RootSymbol root);
  UserSymbol(requite::RootSymbol root, requite::Use &use);
  UserSymbol(requite::RootSymbol root, requite::Import &import);
  UserSymbol(const Self &) = default;
  UserSymbol(Self &&) = default;
  ~UserSymbol() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] requite::UserSymbolOrigin getOrigin() const;
  [[nodiscard]] requite::RootSymbol &getRoot();
  [[nodiscard]] const requite::RootSymbol &getRoot() const;
  [[nodiscard]] requite::Use &getUse();
  [[nodiscard]] const requite::Use &getUse() const;
  [[nodiscard]] requite::Import &getImport();
  [[nodiscard]] const requite::Import &getImport() const;
};

// detail/user_symbol.hpp
template <requite::UserSymbolType TYPE_PARAM>
[[nodiscard]] bool
getIsValidUserSymbolAttribute(requite::ScopeType scope_type,
                         requite::AttributeType attribute_type);
[[nodiscard]] inline constexpr std::string_view getName(requite::UserSymbolType type);

} // namespace requite

#include <requite/detail/user_symbol.hpp>