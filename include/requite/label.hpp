// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>

#include <string>

namespace requite {

struct Scope;
struct Expression;

struct Label final {
  using Self = requite::Label;

  std::string _name = {};
  requite::Expression *_attribute_expression_ptr = nullptr;
  requite::Expression *_statement_expression_ptr = nullptr;
  requite::Scope *_containing_scope_ptr = nullptr;
  llvm::BasicBlock* _llvm_block_ptr = nullptr;
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;
  bool _has_generated_name = false;

  // label.cpp
  Label() = default;
  Label(const Self &) = delete;
  Label(Self &&) = delete;
  ~Label() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasAttributeExpression() const;
  void setAttributeExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getAttributeExpression();
  [[nodiscard]] const requite::Expression &getAttributeExpression() const;
  [[nodiscard]] bool getHasStatementExpression() const;
  void setStatementExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getStatementExpression();
  [[nodiscard]] const requite::Expression &getStatementExpression() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  void setLlvmBlockPtr(llvm::BasicBlock* llvm_block_ptr);
  [[nodiscard]] llvm::BasicBlock* getLllvmBlockPtr();
  [[nodiscard]] const llvm::BasicBlock* getLlvmBlockPtr() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasGeneratedName() const;
  void setHasGeneratedName();
};

} // namespace requite