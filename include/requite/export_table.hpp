#pragma once

#include <requite/symbol.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringMap.h>

namespace requite {

struct Scope;

struct ExportTable final {
  using Self = requite::ExportTable;

  llvm::StringMap<requite::RootSymbol> _exported_symbol_map = {};

  // export_table.cpp
  ExportTable() = default;
  ExportTable(const Self&) = delete;
  ExportTable(Self&&) = delete;
  ~ExportTable() = default;
  Self& operator=(const Self&) = delete;
  Self& operator=(Self&&) = delete;
  [[nodiscard]] bool operator==(const Self&) const;
  [[nodiscard]] bool operator!=(const Self&) const;
  [[nodiscard]] llvm::StringMap<requite::RootSymbol> &getSymbolMap();
  [[nodiscard]] const llvm::StringMap<requite::RootSymbol> &getSymbolMap() const;

  // lookup_symbols.cpp
  [[nodiscard]]
  requite::RootSymbol lookupExportUserSymbol(llvm::StringRef name);

  // detail/export_table_symbol_map.hpp
  [[nodiscard]] inline bool getHasExportSymbolOfName(llvm::StringRef name) const;
  template <typename SymbolArg> void addExportSymbol(SymbolArg &symbol);
};

} // namespace requite

#include <requite/detail/export_table_symbol_map.hpp>