// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/scope.hpp>

#include <string>

namespace requite {

struct Table final {
  std::string _name = {};
  requite::Scope _scope = {};

  // table.cpp
  [[nodiscard]]
  bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]]
  llvm::StringRef getName() const;
  [[nodiscard]]
  requite::Scope &getScope();
  [[nodiscard]]
  const requite::Scope &getScope() const;
};

} // namespace requite