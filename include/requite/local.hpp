// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol.hpp>

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
  void setLlvmAllocaPtr(llvm::AllocaInst *llvm_alloca);
  [[nodiscard]] llvm::AllocaInst *getLlvmAllocaPtr();
  [[nodiscard]] const llvm::AllocaInst *getLlvmAllocaPtr() const;
};

} // namespace requite