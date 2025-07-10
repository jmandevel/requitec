// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol.hpp>
#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace llvm {

struct AllocaInst;

}

namespace requite {

struct Expression;
struct Node;
struct Scope;

struct Local final {
  using Self = requite::Local;

  std::string _name = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::Symbol _data_type = {};
  requite::Scope *_containing_scope_ptr = nullptr;
  llvm::AllocaInst *_llvm_alloca_ptr = nullptr;
  requite::Local * _next_ptr = nullptr;
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;
  bool _has_generated_name = false;

  // local.cpp
  Local() = default;
  Local(const Self &) = delete;
  Local(Self &&) = delete;
  ~Local() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool operator==(const Self &) const;
  [[nodiscard]] bool operator!=(const Self &) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] requite::Symbol &getDataType();
  [[nodiscard]] const requite::Symbol &getDataType() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] bool getHasLlvmAllocaPtr() const;
  void setLlvmAllocaPtr(llvm::AllocaInst *llvm_alloca);
  [[nodiscard]] llvm::AllocaInst *getLlvmAllocaPtr();
  [[nodiscard]] const llvm::AllocaInst *getLlvmAllocaPtr() const;
  [[nodiscard]] bool getHasNext() const;
  void setNext(requite::Local& local);
  [[nodiscard]] requite::Local& getNext();
  [[nodiscard]] const requite::Local& getNext() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasGeneratedName() const;
  void setHasGeneratedName();
};

} // namespace requite