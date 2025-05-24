// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/node.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <vector>

namespace requite {

struct Arena;
struct Module;
struct Expression;
struct Context;
struct Expression;

struct Scope {
  using Self = requite::Scope;

  llvm::StringMap<requite::RootSymbol> _symbol_map = {};
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::Object *_object_ptr;
  requite::Table *_table_ptr = nullptr;
  requite::Procedure *_procedure_ptr = nullptr;
  requite::Expression *_expression_ptr = nullptr;
  std::vector<requite::Node> _nodes = {};

  Scope() = default;
  Scope(const Self &that) = delete;
  Scope(Self &&that) = delete;
  ~Scope() = default;
  Self &operator=(const Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;

  // scope.cpp
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(Self &&rhs) const;
  [[nodiscard]]
  llvm::StringMap<requite::RootSymbol> &getSymbolMap();
  [[nodiscard]]
  const llvm::StringMap<requite::RootSymbol> &getSymbolMap() const;
  [[nodiscard]]
  bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]]
  requite::Scope &getContainingScope();
  [[nodiscard]]
  const requite::Scope &getContainingScope() const;
  [[nodiscard]]
  std::vector<requite::Node> &getNodes();
  [[nodiscard]]
  const std::vector<requite::Node> &getNodes() const;
  [[nodiscard]]
  bool getIsEmpty() const;
  [[nodiscard]]
  bool getHasObject() const;
  void setObject(requite::Object &object);
  [[nodiscard]]
  requite::Object &getObject();
  [[nodiscard]]
  const requite::Object &getObject() const;
  [[nodiscard]]
  requite::Object *getObjectPtr();
  [[nodiscard]]
  const requite::Object *getObjectPtr() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]]
  bool getHasProcedure() const;
  [[nodiscard]]
  requite::Procedure &getProcedure();
  [[nodiscard]]
  const requite::Procedure &getProcedure() const;
  [[nodiscard]]
  requite::Procedure *getProcedurePtr();
  [[nodiscard]]
  const requite::Procedure *getProcedurePtr() const;
  [[nodiscard]]
  bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]]
  requite::Expression &getExpression();
  [[nodiscard]]
  const requite::Expression &getExpression() const;

  // lookup_symbols.cpp
  [[nodiscard]]
  requite::RootSymbol lookupInternalRootSymbol(llvm::StringRef name);

  // detail/scope_symbol.hpp
  template <typename SymbolArg> void addSymbol(SymbolArg &symbol);
};

} // namespace requite

#include <requite/detail/scope_symbol.hpp>