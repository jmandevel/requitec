#pragma once

#include <requite/symbol.hpp>
#include <requite/temporary.hpp>

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
  std::vector<requite::Temporary> _temporary_list = {};

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

  /*
    "build" functions are used to generate IR with LLVM. There are 4
     main categories of generate functions:

     1. statement functions - generate a statement that does not return
        anything. These are always for top level operations of a scope.
     2. value expression functions - generate an expression that returns
        result as llvm::Value*. These are mainly used for non-aggregate
        types.
     3. store expression functions - generate an expression that stores the
        result in a memory location passed in as an llvm::Value*. these are
        mainly used for aggregate types.
     4. symbol builders - generate all code associated with an unordered symbol
    in base, table, or object scope.

     Some opcodes have different functions bellow associated with them that
     belong to a different category. The appropriate builder function must
     be choosen based on the opcode and expected return.

     The reason for the separetion between value expressions and store
     expressions is performance of generated IR:

     https://llvm.org/docs/Frontend/PerformanceTips.html#avoid-creating-values-of-aggregate-type
   */

  // build.cpp
  [[nodiscard]] bool buildSymbolEntryPoint(requite::Procedure &entry_point);

  llvm::BasicBlock &createLlvmBlock(std::string_view name);
  void setCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  void changeCurrentLlvmBlock(llvm::BasicBlock &llvm_block);
  [[nodiscard]] bool getHasLlvmBlock() const;
  [[nodiscard]] llvm::BasicBlock &getLlvmBlock();
  [[nodiscard]] const llvm::BasicBlock &getLlvmBlock() const;

  [[nodiscard]] bool buildStatement(requite::Expression &statement);
  [[nodiscard]] bool buildStatementExit(requite::Expression &statement);
  [[nodiscard]] bool buildStatement_Local(requite::Expression &statement);

  [[nodiscard]] llvm::Value *buildValue(requite::Expression &expression,
                                        const requite::Symbol &expected_type);
  [[nodiscard]] llvm::Value *storeValue(requite::Expression &expression,
                                        const requite::Symbol &expected_type,
                                        llvm::Value *location_ptr);
  [[nodiscard]] llvm::Value *
  buildValue__IntegerLiteral(requite::Expression &expression,
                             const requite::Symbol &expected_type);
  llvm::Value *
  buildValue_Add(requite::Expression& expression,
                                const requite::Symbol &expected_type);
};

} // namespace requite