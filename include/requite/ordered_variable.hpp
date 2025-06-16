// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/variable_type.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Node;
struct Scope;

struct OrderedVariable final {
  using Self = requite::OrderedVariable;

  std::string _name = {};
  requite::VariableType _type = requite::VariableType::NONE;
  requite::Expression *_expression_ptr = nullptr;
  requite::Symbol _data_type = {};
  requite::Scope *_containing_scope_ptr = nullptr;

  // ordered_variable.cpp
  OrderedVariable() = default;
  OrderedVariable(const Self&) = delete;
  OrderedVariable(Self&&) = delete;
  ~OrderedVariable() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  void setType(requite::VariableType type);
  [[nodiscard]] requite::VariableType getType() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] requite::Symbol &getDataType();
  [[nodiscard]] const requite::Symbol& getDataType() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
};

} // namespace requite