// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/context.hpp>
#include <requite/situation.hpp>

#include <llvm/IR/BasicBlock.h>

#include <functional>
#include <memory>

namespace requite {

struct Scope;

struct Builder final {
  using Self = requite::Builder;

  std::reference_wrapper<requite::Context> _context_ref;
  std::reference_wrapper<requite::Module> _module_ref;
  llvm::BasicBlock *_current_llvm_block_ptr = nullptr;
  requite::Scope* _current_scope_ptr = nullptr;
  bool _is_ok = true;

  // builder.cpp
  explicit Builder(requite::Context &context, requite::Module &module);
  Builder(const Self &) = delete;
  Builder(Self &&) = default;
  ~Builder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]]
  requite::Context &getContext();
  [[nodiscard]]
  const requite::Context &getContext() const;
  [[nodiscard]]
  requite::Module &getModule();
  [[nodiscard]]
  const requite::Module &getModule() const;
  [[nodiscard]]
  bool getHasCurrentLlvmBlock() const;
  void setCurrentLlvmBlock(llvm::BasicBlock &block);
  void changeCurrentLlvmBlock(llvm::BasicBlock &block);
  void clearCurrentLlvmBlock();
  [[nodiscard]]
  llvm::BasicBlock &getCurrentLlvmBlock();
  [[nodiscard]]
  const llvm::BasicBlock &getCurrentLlvmBlock() const;
  [[nodiscard]]
  bool getIsOk() const;
  void setNotOk();

  // build_ir.cpp
  void buildModule();
  bool buildEntryPoint(requite::Procedure &procedure);
  bool buildStatement(requite::Expression &statement);
  bool buildStatementExit(requite::Expression &expression);
  llvm::Value *buildValue(requite::Expression &expression,
                          const requite::Symbol &expected_type);
  llvm::Value *buildValue_IntegerLiteral(requite::Expression &expression,
                                        const requite::Symbol &expected_type);
};

} // namespace requite