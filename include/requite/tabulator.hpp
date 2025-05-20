// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_type.hpp>

#include <functional>

namespace requite {

struct Context;
struct Module;
struct Expression;
struct Scope;
struct Object;
struct Attributes;
struct Procedure;
struct Table;
struct AnonymousFunction;

struct Tabulator final {
  using Self = requite::Tabulator;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  bool _is_ok = true;

  // tabulator.cpp
  Tabulator(requite::Context &context, requite::Module &module);
  Tabulator(Self &&that) = delete;
  Tabulator(const Self &that) = delete;
  ~Tabulator() = default;
  Self &operator=(Self &&that) = delete;
  Self &operator=(const Self &that) = delete;
  [[nodiscard]]
  bool getIsOk() const;
  void setNotOk();
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  requite::Module &getModule();
  [[nodiscard]]
  const requite::Module &getModule() const;

  // tabulate_symbols.cpp
  void tabulateModule();
  void searchForUnorderedSymbolExpression(requite::Scope &scope,
                                          requite::Attributes attributes,
                                          requite::Expression &expression);
  void tabulateUnorderedAscribe(requite::Scope &scope,
                                requite::Expression &expression);
  void tabulateUnorderedEntryPoint(requite::Scope &scope,
                                   requite::Attributes attributes,
                                   requite::Expression &expression);
  void tabulateUnorderedNamedProcedure(requite::Scope &scope,
                                       requite::Attributes attributes,
                                       requite::Expression &expression);
  void tabulateUnorderedConstructor(requite::Scope &scope,
                                    requite::Attributes attributes,
                                    requite::Expression &expression);
  void tabulateUnorderedDestructor(requite::Scope &scope,
                                   requite::Attributes attributes,
                                   requite::Expression &expression);
  void tabulateUnorderedObject(requite::Scope &scope,
                               requite::Attributes attributes,
                               requite::Expression &expression);
  void tabulateUnorderedTable(requite::Scope &scope,
                              requite::Expression &expression);
  void tabulateUnorderedAlias(requite::Scope &scope,
                              requite::Attributes attributes,
                              requite::Expression &expression);
  void tabulateUnorderedGlobal(requite::Scope &scope,
                               requite::Attributes attributes,
                               requite::Expression &expression);
  void tabulateUnorderedProperty(requite::Scope &scope,
                                 requite::Attributes attributes,
                                 requite::Expression &expression);
  void tabulateUnorderedScope(requite::Scope &scope,
                              requite::Expression &expression);
  void tabulateUnorderedImport(requite::Scope &scope,
                               requite::Attributes attributes,
                               requite::Expression &expression);
  void tabulateUnorderedUse(requite::Scope &scope,
                            requite::Attributes attributes,
                            requite::Expression &expression);
  void searchForLocalMatteStatement(requite::Scope &scope,
                                    requite::Attributes attributes,
                                    requite::Expression &expression);
  void tabulateLocalLocal(requite::Scope &scope, requite::Attributes attributes,
                          requite::Expression &expression);
  void tabulateLocalScopedStatement(requite::Scope &scope,
                                    requite::Expression &expression);
  void tabulateLocalScopedStatementSkipFirst(requite::Scope &scope,
                                             requite::Expression &expression);
  void tabulateLocalAscribe(requite::Scope &scope,
                            requite::Expression &expression);
  void searchForScopedValue(requite::Scope &scope,
                            requite::Expression &expression);
  void tabulateConduit(requite::Scope &scope, requite::Expression &expression);
  void tabulateScopedValueAnonymousFunction(requite::Scope &scope,
                                            requite::Expression &expression);
  [[nodiscard]]
  requite::Table *getOrMakeTableOfName(requite::Scope &scope,
                                       requite::Expression &expression,
                                       requite::Expression &name_expression);
  [[nodiscard]]
  requite::Table *getOrMakeTableAtPath(requite::Scope &scope,
                                       requite::Expression &expression,
                                       requite::Expression &path_expression);
};

} // namespace requite