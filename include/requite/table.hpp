// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol_status.hpp>

#include <llvm/ADT/StringMap.h>

#include <string>

namespace requite {

struct Table final {
  using Self = Table;

  std::string _name = {};
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;

  // table.cpp
  Table() = default;
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
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
};

} // namespace requite