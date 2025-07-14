// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/user_symbol.hpp>
#include <requite/scope.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <string>
#include <set>

namespace requite {

struct Table final {
  using Self = Table;

  std::string _name = {};
  requite::Scope _scope = {};

  // table.cpp
  Table();
  Table(const Self &) = delete;
  Table(Self &&) = delete;
  ~Table() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool operator==(const Self&) const;
  [[nodiscard]] bool operator!=(const Self&) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Scope& getScope();
  [[nodiscard]] const requite::Scope& getScope() const;
  bool getIsEmpty() const;
};

} // namespace requite