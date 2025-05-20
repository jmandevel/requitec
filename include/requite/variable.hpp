// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attributes.hpp>
#include <requite/variable_type.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Node;
struct Scope;

struct Variable final {
  using Self = requite::Variable;

  std::string _name = {};
  requite::VariableType _type = requite::VariableType::NONE;
  requite::Expression *_expression_ptr = nullptr;
  requite::Attributes _attributes = {};
  requite::Symbol _data_type = {};
  requite::Scope *_containing_scope_ptr = nullptr;

  // variable.cpp
  [[nodiscard]]
  bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]]
  llvm::StringRef getName() const;
  void setType(requite::VariableType type);
  [[nodiscard]]
  requite::VariableType getType() const;
  [[nodiscard]]
  requite::Attributes &getAttributes();
  [[nodiscard]]
  const requite::Attributes &getAttributes() const;
  [[nodiscard]]
  bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]]
  requite::Expression &getExpression();
  [[nodiscard]]
  const requite::Expression &getExpression() const;
  [[nodiscard]]
  requite::Symbol &getDataType();
  [[nodiscard]]
  const requite::Symbol& getDataType() const;
  [[nodiscard]]
  bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]]
  requite::Scope &getContainingScope();
  [[nodiscard]]
  const requite::Scope &getContainingScope() const;
};

} // namespace requite