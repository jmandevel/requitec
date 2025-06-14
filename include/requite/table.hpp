// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/scope.hpp>
#include <requite/scope_iterator.hpp>

#include <llvm/ADT/StringMap.h>

#include <string>

namespace requite {

struct Table final {
  using Self = Table;

  std::string _name = {};
  requite::Scope _stub_scope = {};

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
  void addScope(requite::Scope& scope);

    // detail/table_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ScopeIterator, requite::ScopeIterator,
      std::ranges::subrange_kind::unsized>
  getScopeSubrange();
};

} // namespace requite

#include <requite/detail/table_subrange.hpp>