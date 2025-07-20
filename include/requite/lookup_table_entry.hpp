#pragma once

#include <requite/user_symbol.hpp>

#include <llvm/ADT/SmallVector.h>

namespace requite {

struct Table;

struct LookupTableEntry final {
  using Self = requite::LookupTableEntry;

  requite::Table* _table_ptr = nullptr;
  llvm::SmallVector<requite::UserSymbol, 1> _symbols = {};

  // lookup_table_entry.cpp
  LookupTableEntry() = default;
  LookupTableEntry(const Self &) = default;
  LookupTableEntry(Self &&) = default;
  ~LookupTableEntry() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] bool getHasTable() const;
  void setTable(requite::Table& table);
  [[nodiscard]] requite::Table& getTable();
  [[nodiscard]] const requite::Table& getTable() const;
  [[nodiscard]] llvm::SmallVector<requite::UserSymbol, 1> &getUserSymbols();
  [[nodiscard]] const llvm::SmallVector<requite::UserSymbol, 1> &
  getUserSymbols() const;
  [[nodiscard]] bool getIsAmbiguous() const;

  // detail/entry_add_user_symbol.hpp
  template<typename UserParam>
  void addSymbol(UserParam& user);
};

} // namespace requite

#include <requite/detail/entry_add_user_symbol.hpp>