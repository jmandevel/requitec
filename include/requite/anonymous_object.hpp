// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol.hpp>
#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>
#include <vector>

namespace requite {

struct Expression;

struct AnonymousProperty final {
  using Self = requite::AnonymousProperty;

  std::string _name = {};
  requite::Symbol _symbol = {};
  requite::Expression *_default_value_expression_ptr = nullptr;
  requite::SymbolStatus _status = requite::SymbolStatus::EXPAND_NAME;

  // anonymous_property.cpp
  AnonymousProperty() = default;
  AnonymousProperty(llvm::StringRef name, const requite::Symbol &symbol);
  explicit AnonymousProperty(const Self &that) = default;
  explicit AnonymousProperty(requite::AnonymousProperty &&that) = default;
  ~AnonymousProperty() = default;
  [[nodiscard]] Self &operator=(const Self &rhs) = default;
  [[nodiscard]] Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Symbol &getSymbol();
  [[nodiscard]] const requite::Symbol &getSymbol() const;
  [[nodiscard]] bool getHasDefaultValueExpression() const;
  void setDefaultValueExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getDefaultValueExpression();
  [[nodiscard]] const requite::Expression &getDefaultValueExpression() const;
};

struct AnonymousObject final {
  using Self = requite::AnonymousObject;
  std::vector<requite::AnonymousProperty> _properties;

  // anonymous_object.cpp
  AnonymousObject() = default;
  explicit AnonymousObject(const Self &that) = default;
  explicit AnonymousObject(Self &&that) = default;
  ~AnonymousObject() = default;
  [[nodiscard]] Self &operator=(const Self &rhs) = default;
  [[nodiscard]] Self &operator=(Self &rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] std::vector<requite::AnonymousProperty> &getProperties();
  [[nodiscard]] const std::vector<requite::AnonymousProperty> &
  getProperties() const;
};

} // namespace requite