// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {
struct Label final {
  using Self = requite::Label;

  std::string _name = {};
  requite::Expression* _expression_ptr = nullptr;

  // label.cpp
  Label() = default;
  explicit Label(const Self &that) = default;
  explicit Label(Self &&that) = default;
  explicit Label(requite::Attributes attributes);
  ~Label() = default;
  Self& operator=(const Self &rhs) = default;
  Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
};

} // namespace requite