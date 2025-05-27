// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Scope;
struct Expression;

struct Label final {
  using Self = requite::Label;

  std::string _name = {};
  bool _dependent_name = false;
  requite::Expression *_attribute_expression_ptr = nullptr;
  requite::Expression *_statement_expression_ptr = nullptr;
  requite::Scope *_containing_scope_ptr = nullptr;

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
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
};

} // namespace requite