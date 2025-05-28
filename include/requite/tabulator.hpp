#pragma once

#include <requite/symbol.hpp>

#include <functional>
#include <vector>

namespace requite {

struct Context;

struct Tabulator final {
  using Self = requite::Tabulator;

  std::reference_wrapper<requite::Context> _context_ref;
  std::vector<requite::RootSymbol> _unamed_symbols = {};
  bool _is_ok = true;

  // tabulator.cpp
  Tabulator(requite::Context &context);
  Tabulator(const Self &) = delete;
  Tabulator(Self &&) = delete;
  ~Tabulator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] std::vector<requite::RootSymbol> &getUnamedSymbols();
  [[nodiscard]] const std::vector<requite::RootSymbol> &getUnamedSymbols() const;
  [[nodiscard]] bool getIsOk();
  void setNotOk();

  // tabulate_user_symbols.cpp
  void tabulateUserSymbols();
  void tabulateInstantNamedSymbols();
};

} // namespace requite