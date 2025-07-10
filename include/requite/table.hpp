// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol_status.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <string>
#include <set>

namespace requite {

struct Table final {
  using Self = Table;

  std::string _name = {};
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;
  llvm::StringMap<requite::RootSymbol> _symbol_map = {};

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
  [[nodiscard]] llvm::StringMap<requite::RootSymbol> &getSymbolMap();
  [[nodiscard]] const llvm::StringMap<requite::RootSymbol> &
  getSymbolMap() const;
  bool getIsEmpty() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();

  // lookup_symbols.cpp
  [[nodiscard]] requite::RootSymbol lookupUserSymbol(llvm::StringRef name);
  [[nodiscard]] bool getHasUserSymbolOfName(llvm::StringRef name) const;

  // detail/table_symbol_map.hpp
  template <typename SymbolArg> void addUserSymbol(SymbolArg &symbol);
};

} // namespace requite

#include <requite/detail/table_symbol_map.hpp>