#pragma once

#include <requite/symbol.hpp>

#include <llvm/IR/Value.h>

#include <functional>
#include <string_view>
#include <vector>

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

  // build.cpp
 [[nodiscard]] bool buildEntryPoint(requite::Procedure& entry_point);

 llvm::BasicBlock &createLlvmBlock(std::string_view name);
  void setCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  void changeCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  [[nodiscard]] bool getHasLlvmBlock() const;
  [[nodiscard]] llvm::BasicBlock &getLlvmBlock();
  [[nodiscard]] const llvm::BasicBlock &getLlvmBlock() const;

  [[nodiscard]] bool buildStatement(requite::Expression& statement);
  [[nodiscard]] bool buildStatementAssign(requite::Expression& statement);
  [[nodiscard]] bool buildStatementSwap(requite::Expression& statement);
  [[nodiscard]] bool buildStatementCall(requite::Expression& statement);
  [[nodiscard]] bool buildStatementDestroy(requite::Expression& statement);
  [[nodiscard]] bool buildStatementReturn(requite::Expression& statement);
  [[nodiscard]] bool buildStatementBreak(requite::Expression& statement);
  [[nodiscard]] bool buildStatementContinue(requite::Expression& statement);
  [[nodiscard]] bool buildStatementFallthrough(requite::Expression& statement);
  [[nodiscard]] bool buildStatementExit(requite::Expression& statement);
  [[nodiscard]] bool buildStatementGoto(requite::Expression& statement);
  [[nodiscard]] bool buildStatementLocal(requite::Expression& statement);
  [[nodiscard]] bool buildStatementGlobal(requite::Expression& statement);
  [[nodiscard]] bool buildStatementIf(requite::Expression& statement);
  [[nodiscard]] bool buildStatementElseIf(requite::Expression& statement);
  [[nodiscard]] bool buildStatementElse(requite::Expression& statement);
  [[nodiscard]] bool buildStatementSwitch(requite::Expression& statement);
  [[nodiscard]] bool buildStatementFor(requite::Expression& statement);
  [[nodiscard]] bool buildStatementWhile(requite::Expression& statement);
  [[nodiscard]] bool buildStatementDoWhile(requite::Expression& statement);
  [[nodiscard]] bool buildStatementForEach(requite::Expression& statement);
  [[nodiscard]] bool buildStatementLoop(requite::Expression& statement);
  [[nodiscard]] bool buildStatementScope(requite::Expression& statement);
  [[nodiscard]] bool buildStatementAssert(requite::Expression& statement);
  [[nodiscard]] bool buildStatementTry(requite::Expression& statement);
  [[nodiscard]] bool buildStatementCatch(requite::Expression& statement);
  [[nodiscard]] bool buildStatementThrow(requite::Expression& statement);

  [[nodiscard]] llvm::Value* buildValue(requite::Expression& expression, const requite::Symbol& expected_type);
  [[nodiscard]] llvm::Value* buildValue__IntegerLiteral(requite::Expression& expression, const requite::Symbol& expected_type);
};

} // namespace requite