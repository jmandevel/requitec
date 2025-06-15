// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/scope.hpp>

#include <llvm/ADT/StringMap.h>

#include <string>

namespace requite {

struct Table final {
  using Self = Table;

  std::string _name = {};
  requite::Scope _scope = {};

  // table.cpp
  Table() = default;
  Table(const Self &) = delete;
  Table(Self &&) = delete;
  ~Table() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] unsigned getScopeDepth() const;
  [[nodiscard]] requite::Scope *getContainingPtr();
  [[nodiscard]] const requite::Scope *getContainingPtr() const;
};

} // namespace requite