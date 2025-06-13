// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>

namespace requite {

struct Context;
struct Module;

struct Maker final {
  using Self = requite::Maker;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  bool _is_ok = true;

  // maker.cpp
  Maker(requite::Context &context, requite::Module &module);
  Maker(const Self &) = delete;
  Maker(Self &&) = delete;
  ~Maker() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] bool getIsOk() const;
  void setNotOk();

  // make_user_symbols.cpp
  void makeUserSymbols();
  void makeUnorderedUserSymbols(requite::Scope &scope,
                                requite::Expression &body,
                                bool conduits_have_scopes);
  void makeUnorderedUserSymbol(requite::Scope &scope,
                               requite::Expression &expression,
                               bool conduits_have_scopes);
  void makeAscribedUnorderedUserSymbol(requite::Scope &scope,
                                       requite::Expression &expression,
                                       requite::Attributes attributes,
                                       bool conduits_have_scopes);
  void makeOrderedUserSymbols(requite::Scope &scope,
                              requite::Expression &branch);
  void makeAscribedOrderedUserSymbol(requite::Scope &scope,
                                     requite::Attributes attributes,
                                     requite::Expression &expression);
  void makeOrderedUserSymbol(requite::Scope &scope,
                             requite::Expression &expression);
  void makeScopedValues(requite::Scope &scope, requite::Expression &expression,
                        bool conduits_have_scopes);
};

} // namespace requite