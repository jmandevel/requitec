// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/SmallPtrSet.h>

#include <functional>

namespace requite {

struct Context;
struct Module;
struct Scope;
struct Object;
struct Attributes;
struct Expression;
struct Procedure;
struct Global;
struct Local;
struct AnonymousFunction;


struct LocalTabulator final {
  using Self = requite::LocalTabulator;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  llvm::SmallPtrSet<requite::Global *, 4> _found_global_ptrs = {};
  bool _is_ok = true;

  // local_tabulator.cpp
  LocalTabulator(requite::Context &context, requite::Module &module);
  LocalTabulator(const requite::LocalTabulator &) = delete;
  LocalTabulator(requite::LocalTabulator &&) = delete;
  ~LocalTabulator() = default;
  LocalTabulator &operator=(const requite::LocalTabulator &) = delete;
  LocalTabulator &operator=(requite::LocalTabulator &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] bool getIsOk() const;
  void setNotOk();

  // tabulate_locals.cpp
  void tabulateLocals();
  void tabulateLocalMatteStatement(requite::Scope &scope,
                                   requite::Expression &expression);
  void tabulateLocal(requite::Scope &scope, requite::Expression &expression);
  void tabulateProcedureScope(requite::Procedure &procedure);
  void tabulateEntryPointScope(requite::Procedure &procedure);
  void tabulateFunctionScope(requite::Procedure &procedure);
  void tabulateMethodScope(requite::Procedure &procedure);
  void tabulateConstructorScope(requite::Procedure &procedure);
  void tabulateDestructorScope(requite::Procedure &procedure);
  void tabulateIfScope(requite::Scope &containing_scope,
                       requite::Expression &expression);
  void tabulateElseIfScope(requite::Scope &containing_scope,
                           requite::Expression &expression);
  void tabulateElseScope(requite::Scope &containing_scope,
                         requite::Expression &expression);
  void tabulateSwitchScope(requite::Scope &containing_scope,
                           requite::Expression &expression);
  void tabulateCaseScope(requite::Scope &containing_scope,
                         requite::Expression &expression);
  void tabulateDefaultCaseScope(requite::Scope &containing_scope,
                                requite::Expression &expression);
  void tabulateForScope(requite::Scope &containing_scope,
                        requite::Expression &expression);
  void tabulateWhileScope(requite::Scope &containing_scope,
                          requite::Expression &expression);
  void tabulateDoWhileScope(requite::Scope &containing_scope,
                            requite::Expression &expression);
  void tabulateForEachScope(requite::Scope &containing_scope,
                            requite::Expression &expression);
  void tabulateLoopScope(requite::Scope &containing_scope,
                         requite::Expression &expression);
  void tabulateScopeScope(requite::Scope &containing_scope,
                          requite::Expression &expression);
  void tabulateTryScope(requite::Scope &containing_scope,
                        requite::Expression &expression);
  void tabulateCatchScope(requite::Scope &containing_scope,
                          requite::Expression &expression);
  void tabulateInlineScopeScope(requite::InlineScope &inline_scope);
  void tabulateAnonymousFunctionScope(
      requite::AnonymousFunction &anonymous_function);
};

} // namespace requite