// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol.hpp>

#include <string>
#include <string_view>

namespace requite {

struct Expression;

struct SignatureParameter final {
  using Self = requite::SignatureParameter;

  std::string _name = {};
  requite::Symbol _type = {};
  requite::Expression *_default_value_expression_ptr = nullptr;

  // signature_parameter.cpp
  SignatureParameter() = default;
  explicit SignatureParameter(const Self& that) = default;
  explicit SignatureParameter(Self&& that) = default;
  ~SignatureParameter() = default;
  [[nodiscard]] Self& operator=(const Self& rhs) = default;
  [[nodiscard]] Self& operator=(Self&& rhs) = default;
  [[nodiscard]] requite::Symbol& getType();
  [[nodiscard]] const requite::Symbol& getType() const;
  [[nodiscard]] bool operator==(const Self& rhs) const; 
  [[nodiscard]] bool operator!=(const Self& rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(std::string_view name);
  [[nodiscard]] std::string_view getName() const;
  [[nodiscard]] bool getHasDefaultValueExpression() const;
  void setDefaultValueExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getDefaultValueExpression();
  [[nodiscard]] const requite::Expression &getDefaultValueExpression() const;
};

} // namespace requite