// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/node.hpp>
#include <requite/scope_type.hpp>
#include <requite/symbol.hpp>
#include <requite/containing_scope_iterator.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <vector>
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

struct Scope final {
  using Self = requite::Scope;

  requite::Module* _module_ptr = nullptr;
  llvm::StringMap<requite::RootSymbol> _symbol_map = {};
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::Scope *_next_ptr = nullptr;
  requite::ScopeType _type = requite::ScopeType::NONE;
  union {
    void *_nothing_ptr = nullptr;
    requite::Object *_object_ptr;
    requite::Table *_table_ptr;
    requite::Procedure *_procedure_ptr;
    requite::AnonymousFunction *_anonymous_function_ptr;
  };
  requite::Expression *_expression_ptr = nullptr;
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
  void setType(requite::ScopeType type);
  [[nodiscard]] requite::ScopeType getType() const;
  [[nodiscard]] llvm::StringMap<requite::RootSymbol> &getSymbolMap();
  [[nodiscard]] const llvm::StringMap<requite::RootSymbol> &
  getSymbolMap() const;
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
[[nodiscard]] requite::Scope *getContainingScopePtr();
  [[nodiscard]] const requite::Scope *getContainingScopePtr() const;
  [[nodiscard]] std::vector<requite::Node> &getNodes();
  [[nodiscard]] const std::vector<requite::Node> &getNodes() const;
  [[nodiscard]] bool getIsEmpty() const;
  [[nodiscard]] bool getHasObject() const;
  void setObject(requite::Object &object);
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] bool getHasTable() const;
  void setTable(requite::Table &table);
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] bool getHasProcedure() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] bool getHasAnonymousFunction() const;
  void setAnonymousFunction(requite::AnonymousFunction &anonymous_function);
  [[nodiscard]] requite::AnonymousFunction &getAnonymousFunction();
  [[nodiscard]] const requite::AnonymousFunction &getAnonymousFunction() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &
  replaceExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &popExpression();
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasNext() const;
  void setNext(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getNext();
  [[nodiscard]] const requite::Scope &getNext() const;

  // lookup_symbols.cpp
  [[nodiscard]]
  requite::RootSymbol lookupInternalRootSymbol(llvm::StringRef name);

  // detail/scope_symbol.hpp
  template <typename SymbolArg> void addSymbol(SymbolArg &symbol);

  // detail/scope_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ContainingScopeIterator, requite::ContainingScopeIterator,
      std::ranges::subrange_kind::unsized>
  getContainingSubrange();
};

} // namespace requite

#include <requite/detail/scope_subrange.hpp>
#include <requite/detail/scope_symbol.hpp>