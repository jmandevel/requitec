#pragma once

#include <llvm/ADT/StringRef.h>

#include <functional>

namespace requite {

struct Context;
struct Global;
struct Procedure;
struct Scope;
struct Expression;
struct Symbol;
struct Local;

struct Contextualizer1 final {
  using Self = requite::Contextualizer1;

  std::reference_wrapper<requite::Context> _context_ref;
  requite::Global *_global_ptr = nullptr;
  requite::Procedure *_procedure_ptr = nullptr;
  requite::Scope *_scope_ptr = nullptr;

  // contextualizer0.cpp
  Contextualizer1(requite::Context &context, requite::Global &global);
  Contextualizer1(requite::Context &context, requite::Procedure &procedure);
  Contextualizer1(const Self &) = delete;
  Contextualizer1(Self &&) = delete;
  ~Contextualizer1() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] bool getIsImplementingGlobal() const;
  [[nodiscard]] requite::Global &getGlobal();
  [[nodiscard]] const requite::Global &getGlobal() const;
  [[nodiscard]] bool getIsImplementingProcedure() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  void enterScope(requite::Scope &scope);
  void leaveScope();

  // tabulate.cpp
  [[nodiscard]] bool tabulate_Local(requite::Local*& out_local_ptr, llvm::StringRef name, requite::Expression &statement);

  // implement.cpp
  // [[nodiscard]] bool implementProcedureSignature(requite::Expression&
  // expression);
  [[nodiscard]] bool implementLocalScope(requite::Expression &first_statement);
  // [[nodiscard]] bool implementGlobalValue(requite::Expression& expression);
  [[nodiscard]] bool implement_Initialize(requite::Expression &statement);
  [[nodiscard]] bool implement_Assign(requite::Expression &statement);
  [[nodiscard]] bool implementSwap(requite::Expression &statement);
  [[nodiscard]] bool
  implement_StructuredBinding(requite::Expression &statement);
  [[nodiscard]] bool implement_Ignore(requite::Expression &statement);
  [[nodiscard]] bool implement_Call(requite::Expression &statement);
  [[nodiscard]] bool implement_DestroyValue(requite::Expression &statement);
  [[nodiscard]] bool implement_DropValue(requite::Expression &statement);
  [[nodiscard]] bool implementReturn(requite::Expression &statement);
  [[nodiscard]] bool implementBreak(requite::Expression &statement);
  [[nodiscard]] bool implementContinue(requite::Expression &statement);
  [[nodiscard]] bool implementFallthrough(requite::Expression &statement);
  [[nodiscard]] bool implementExit(requite::Expression &statement);
  [[nodiscard]] bool implementGoto(requite::Expression &statement);
  [[nodiscard]] bool implementAlias(requite::Expression &statement);
  [[nodiscard]] bool implementIf(requite::Expression &statement);
  [[nodiscard]] bool implementElseIf(requite::Expression &statement);
  [[nodiscard]] bool implementElse(requite::Expression &statement);
  [[nodiscard]] bool implementSwitch(requite::Expression &statement);
  [[nodiscard]] bool implementCase(requite::Expression &statement);
  [[nodiscard]] bool implementFor(requite::Expression &statement);
  [[nodiscard]] bool implementWhile(requite::Expression &statement);
  [[nodiscard]] bool implementDoWhile(requite::Expression &statement);
  [[nodiscard]] bool implementForEach(requite::Expression &statement);
  [[nodiscard]] bool implementLoop(requite::Expression &statement);
  [[nodiscard]] bool implementScope(requite::Expression &statement);
  [[nodiscard]] bool implementImport(requite::Expression &statement);
  [[nodiscard]] bool implementUse(requite::Expression &statement);
  [[nodiscard]] bool implementAssert(requite::Expression &statement);
  [[nodiscard]] bool implementUnreachable(requite::Expression &statement);

  // resolve.cpp
  [[nodiscard]] bool resolveTypeOfValue(requite::Symbol &out_symbol,
                                        requite::Expression &symbol_expression,
                                        requite::Expression &value_expression);
  [[nodiscard]] bool
  inferenceTypeOfValue(requite::Symbol &out_symbol,
                       requite::Expression &value_expression);
  [[nodiscard]] bool
  inferenceTypeOfNaryArithmeticValue(requite::Symbol &out_symbol,
                                     requite::Expression &expression);
};

} // namespace requite