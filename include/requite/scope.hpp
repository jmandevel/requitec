// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/containing_scope_iterator.hpp>
#include <requite/node.hpp>
#include <requite/scope_type.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <ranges>
#include <vector>

namespace requite {

struct Module;
struct Expression;
struct Context;
struct Expression;
struct AnonymousFunction;
struct Procedure;
struct Table;
struct Object;
struct ExportTable;

struct Scope final {
  using Self = requite::Scope;

  unsigned _scope_depth = 0;
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::ExportTable *_export_table_ptr = nullptr;
  llvm::StringMap<requite::RootSymbol> _internal_symbol_map = {};
  requite::ScopeType _type = requite::ScopeType::NONE;
  union {
    void *_nothing_ptr = nullptr;
    requite::Module *_module_ptr;
    requite::Object *_object_ptr;
    requite::Table *_table_ptr;
    requite::Procedure *_procedure_ptr;
    requite::AnonymousFunction *_anonymous_function_ptr;
    requite::Expression *_local_statement_ptr;
    requite::UnorderedVariable *_unordered_variable_ptr;
  };
  std::vector<requite::Node> _nodes = {};

  // scope.cpp
  Scope() = default;
  Scope(const Self &that) = delete;
  Scope(Self &&that) = delete;
  ~Scope() = default;
  Self &operator=(const Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(Self &&rhs) const;
  [[nodiscard]] bool getHasModule() const;
  void setModule(requite::Module &module);
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::ScopeType getType() const;
  [[nodiscard]] llvm::StringMap<requite::RootSymbol> &getInternalSymbolMap();
  [[nodiscard]] const llvm::StringMap<requite::RootSymbol> &
  getInternalSymbolMap() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] unsigned getScopeDepth() const;
  [[nodiscard]] requite::Scope *getContainingPtr();
  [[nodiscard]] const requite::Scope *getContainingPtr() const;
  [[nodiscard]] std::vector<requite::Node> &getNodes();
  [[nodiscard]] const std::vector<requite::Node> &getNodes() const;
  [[nodiscard]] bool getHasExportTable() const;
  void setExportTable(requite::ExportTable& table);
  [[nodiscard]] requite::ExportTable& getExportTable();
  [[nodiscard]] const requite::ExportTable& getExportTable() const;
  [[nodiscard]] bool getIsEmpty() const;
  void setObject(requite::Object &object);
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  void setTable(requite::Table &table);
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  void setAnonymousFunction(requite::AnonymousFunction &anonymous_function);
  [[nodiscard]] requite::AnonymousFunction &getAnonymousFunction();
  [[nodiscard]] const requite::AnonymousFunction &getAnonymousFunction() const;
  void setLocalStatement(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getLocalStatement();
  [[nodiscard]] const requite::Expression &getLocalStatement() const;
  void setUnorderedVariable(requite::UnorderedVariable &variable);
  [[nodiscard]] requite::UnorderedVariable &getUnorderedVariable();
  [[nodiscard]] const requite::UnorderedVariable &getUnorderedVariable() const;

  // lookup_symbols.cpp
  [[nodiscard]]
  requite::RootSymbol lookupInternalUserSymbol(llvm::StringRef name);
  [[nodiscard]]
  requite::RootSymbol lookupExportUserSymbol(llvm::StringRef name);
  [[nodiscard]]
  requite::RootSymbol lookupUserSymbol(llvm::StringRef name);

  // detail/scope_symbol_map.hpp
  [[nodiscard]] inline bool getHasInternalSymbolOfName(llvm::StringRef name) const;
  template <typename SymbolArg> void addInternalSymbol(SymbolArg &symbol);
  [[nodiscard]] inline bool getHasExportSymbolOfName(llvm::StringRef name) const;
  template <typename SymbolArg> void addExportSymbol(SymbolArg &symbol);
  [[nodiscard]] inline bool getHasSymbolOfName(llvm::StringRef name) const;
  
  // detail/scope_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ContainingScopeIterator, requite::ContainingScopeIterator,
      std::ranges::subrange_kind::unsized>
  getContainingSubrange();
};

} // namespace requite

#include <requite/detail/scope_subrange.hpp>
#include <requite/detail/scope_symbol_map.hpp>