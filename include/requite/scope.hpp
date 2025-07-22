// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/containing_scope_iterator.hpp>
#include <requite/lookup_table_entry.hpp>
#include <requite/lookup_table_result.hpp>
#include <requite/scope_type.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <ranges>

namespace requite {

struct Module;
struct Expression;
struct Context;
struct Expression;
struct AnonymousFunction;
struct Procedure;
struct Table;
struct Object;
struct Import;
struct Use;
struct Import;
struct Block;
struct Pull;

struct Scope final {
  using Self = requite::Scope;

  unsigned _scope_depth = 0;
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::ScopeType _type = requite::ScopeType::NONE;
  llvm::StringMap<requite::LookupTableEntry> _symbol_map = {};
  requite::Pull* _first_pull_ptr = nullptr;
  union {
    void *_nothing_ptr = nullptr;
    requite::Module *_module_ptr;
    requite::Object *_object_ptr;
    requite::Table *_table_ptr;
    requite::Procedure *_procedure_ptr;
    requite::AnonymousFunction *_anonymous_function_ptr;
    requite::Expression *_local_statement_ptr;
    requite::Block *_block_ptr;
  };

  // scope.cpp
  Scope() = default;
  Scope(const Self &that) = delete;
  Scope(Self &&that) = delete;
  ~Scope() = default;
  Self &operator=(const Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(Self &&rhs) const;
  [[nodiscard]] const llvm::StringMap<requite::LookupTableEntry>
  getLookupTable() const;
  [[nodiscard]] bool getHasModule() const;
  void setModule(requite::Module &module);
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::ScopeType getType() const;
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
  [[nodiscard]] unsigned getScopeDepth() const;
  [[nodiscard]] requite::Scope *getContainingPtr();
  [[nodiscard]] const requite::Scope *getContainingPtr() const;
  [[nodiscard]] bool getIsEmpty() const;
  [[nodiscard]] bool getIsObject() const;
  void setObject(requite::Object &object);
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] bool getIsTable() const;
  void setTable(requite::Table &table);
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] bool getIsProcedure() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] bool getIsAnonymousFunction() const;
  void setAnonymousFunction(requite::AnonymousFunction &anonymous_function);
  [[nodiscard]] requite::AnonymousFunction &getAnonymousFunction();
  [[nodiscard]] const requite::AnonymousFunction &getAnonymousFunction() const;
  [[nodiscard]] bool getIsBlock() const;
  void setBlock(requite::Block &block);
  [[nodiscard]] requite::Block &getBlock();
  [[nodiscard]] const requite::Block &getBlock() const;

  // lookup_symbols.cpp
  [[nodiscard]] requite::LookupTableEntry &
  lookupUserSymbol(llvm::StringRef name);
  [[nodiscard]] bool getHasUserSymbolOfName(llvm::StringRef name) const;

  // detail/add_user_symbol.hpp
  template <typename SymbolArg>
  void addUserSymbol(SymbolArg &symbol, requite::Module &module);

  // symbol_map.cpp
  [[nodiscard]] bool getHasTable(llvm::StringRef name) const;
  void addTable(requite::Table &table);

  // detail/scope_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ContainingScopeIterator, requite::ContainingScopeIterator,
      std::ranges::subrange_kind::unsized>
  getContainingSubrange();
};

} // namespace requite

#include <requite/detail/scope_add_user_symbol.hpp>
#include <requite/detail/scope_subrange.hpp>
