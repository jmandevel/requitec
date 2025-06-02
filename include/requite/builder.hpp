#pragma once

#include <requite/expression_ranger.hpp>

#include <functional>
#include <string_view>

namespace llvm {

struct BasicBlock;

}

namespace requite {

struct Context;
struct Scope;
struct Procedure;
struct AnonymousFunction;
struct Variable;
struct Symbol;
struct Expression;
struct Value;

// NOTE:
//  the Builder should be designed with these tips in mind:
//
//  https://llvm.org/docs/Frontend/PerformanceTips.html

struct Builder final {
  using Self = requite::Builder;

  std::reference_wrapper<requite::Context> _context_ref;
  llvm::BasicBlock *_current_llvm_block_ptr = nullptr;
  requite::Scope *_current_scope_ptr = nullptr;
  requite::ExpressionRanger _ranger = {};

  // builder.cpp
  Builder(requite::Context &context);
  Builder(const Self &) = delete;
  Builder(Self &&) = delete;
  ~Builder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Context &getContext();
  [[nodiscard]] const requite::Context &getContext() const;
  [[nodiscard]] bool getHasScope() const;
  void setScope(requite::Scope &scope);
  void enterScope(requite::Scope &scope);
  void exitScope();
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::ExpressionRanger &getRanger();
  [[nociscard]] const requite::ExpressionRanger &getRanger() const;

  // build_user_symbols.cpp
  [[nodiscard]] bool buildUserSymbol(requite::Procedure &procedure);
  [[nodiscard]] bool
  buildUserSymbol(requite::AnonymousFunction &anonymous_function);
  [[nodiscard]] bool buildUserSymbol(requite::Variable &variable);

  // builder_blocks.cpp
  llvm::BasicBlock &createLlvmBlock(std::string_view name);
  void setCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  void changeCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  [[nodiscard]] bool getHasLlvmBlock() const;
  [[nodiscard]] llvm::BasicBlock &getLlvmBlock();
  [[nodiscard]] const llvm::BasicBlock &getLlvmBlock() const;

  // build_statements.cpp
  [[nodiscard]] bool buildStatement();
  [[nodiscard]] bool buildStatementAssign();
  [[nodiscard]] bool buildStatementSwap();
  [[nodiscard]] bool buildStatementCall();
  [[nodiscard]] bool buildStatementDestroy();
  [[nodiscard]] bool buildStatementReturn();
  [[nodiscard]] bool buildStatementBreak();
  [[nodiscard]] bool buildStatementContinue();
  [[nodiscard]] bool buildStatementFallthrough();
  [[nodiscard]] bool buildStatementExit();
  [[nodiscard]] bool buildStatementGoto();
  [[nodiscard]] bool buildStatementLocal();
  [[nodiscard]] bool buildStatementGlobal();
  [[nodiscard]] bool buildStatementIf();
  [[nodiscard]] bool buildStatementElseIf();
  [[nodiscard]] bool buildStatementElse();
  [[nodiscard]] bool buildStatementSwitch();
  [[nodiscard]] bool buildStatementFor();
  [[nodiscard]] bool buildStatementWhile();
  [[nodiscard]] bool buildStatementDoWhile();
  [[nodiscard]] bool buildStatementForEach();
  [[nodiscard]] bool buildStatementLoop();
  [[nodiscard]] bool buildStatementScope();
  [[nodiscard]] bool buildStatementAssert();
  [[nodiscard]] bool buildStatementTry();
  [[nodiscard]] bool buildStatementCatch();
  [[nodiscard]] bool buildStatementThrow();
};

} // namespace requite