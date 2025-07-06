#pragma once

#include <requite/symbol.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

namespace requite {

struct Scope;

struct SymbolTable final {
  using Self = requite::SymbolTable;

  llvm::StringMap<requite::RootSymbol> _symbol_map = {};

  // export_table.cpp
  SymbolTable() = default;
  SymbolTable(const Self &) = delete;
  SymbolTable(Self &&) = delete;
  ~SymbolTable() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool operator==(const Self &) const;
  [[nodiscard]] bool operator!=(const Self &) const;
  [[nodiscard]] llvm::StringMap<requite::RootSymbol> &getSymbolMap();
  [[nodiscard]] const llvm::StringMap<requite::RootSymbol> &
  getSymbolMap() const;
  [[nodiscard]] bool getIsEmpty() const;

  // lookup_symbols.cpp
  [[nodiscard]] requite::RootSymbol lookupUserSymbol(llvm::StringRef name);
  [[nodiscard]] bool getHasUserSymbolOfName(llvm::StringRef name) const;

  // detail/table_symbol_map.hpp
  template <typename SymbolArg> void addUserSymbol(SymbolArg &symbol);
};

} // namespace requite

#include <requite/detail/table_symbol_map.hpp>