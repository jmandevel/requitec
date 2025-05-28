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
  requite::Scope _stub_scope = {};
  requite::Scope *_first_scope_ptr = nullptr;

  // table.cpp
  Table();
  Table(const Self &) = delete;
  Table(Self &&) = delete;
  ~Table() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Scope &getStubScope();
  [[nodiscard]] const requite::Scope &getStubScope() const;
  [[nodiscard]] bool getHasSubScopes() const;
  void addSubScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getFirstScope();
  [[nodiscard]] const requite::Scope &getFirstScope() const;
};

} // namespace requite