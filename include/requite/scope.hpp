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
struct BaseOrTableBlock;
struct ObjectBlock;
struct LocalBlock;

struct Scope final {
  using Self = requite::Scope;

  unsigned _scope_depth = 0;
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::Table *_table_ptr;
  requite::ScopeType _type = requite::ScopeType::NONE;
  union {
    void *_nothing_ptr = nullptr;
    requite::Module *_module_ptr;
    requite::Object *_object_ptr;
    requite::Procedure *_procedure_ptr;
    requite::AnonymousFunction *_anonymous_function_ptr;
    requite::Expression *_local_statement_ptr;
    requite::Global *_global_ptr;
    requite::BaseOrTableBlock *_base_or_table_block_ptr;
    requite::ObjectBlock *_object_block_ptr;
    requite::LocalBlock *_local_block_ptr;
  };
  requite::Node *_first_node_ptr = nullptr;
  bool _has_generated_name = false;

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
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] unsigned getScopeDepth() const;
  [[nodiscard]] requite::Scope *getContainingPtr();
  [[nodiscard]] const requite::Scope *getContainingPtr() const;
  [[nodiscard]] bool getHasNodes() const;
  void addNode(requite::Node &node);
  [[nodiscard]] requite::Node &getFirstNode();
  [[nodiscard]] const requite::Node &getFirstNode() const;
  [[nodiscard]] bool getIsEmpty() const;
  [[nodiscard]] bool getHasObject() const;
  void setObject(requite::Object &object);
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] bool getHasTable() const;
  void setTable(requite::Table &table);
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] bool getHasProcedure() const;
  void setProcedure(requite::Procedure &procedure);
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] const requite::Procedure &getProcedure() const;
  [[nodiscard]] bool getHasAnonymousFunction() const;
  void setAnonymousFunction(requite::AnonymousFunction &anonymous_function);
  [[nodiscard]] requite::AnonymousFunction &getAnonymousFunction();
  [[nodiscard]] const requite::AnonymousFunction &getAnonymousFunction() const;
  [[nodiscard]] bool getHasGlobal() const;
  void setGlobal(requite::Global &variable);
  [[nodiscard]] requite::Global &getGlobal();
  [[nodiscard]] const requite::Global &getGlobal() const;
  [[nodiscard]] bool getHasBaseOrTableBlock() const;
  void setBaseOrTableBlock(requite::BaseOrTableBlock &block);
  [[nodiscard]] requite::BaseOrTableBlock &getBaseOrTableBlock();
  [[nodiscard]] const requite::BaseOrTableBlock &getBaseOrTableBlock() const;
  [[nodiscard]] bool getHasObjectBlock() const;
  void setObjectBlock(requite::ObjectBlock &block);
  [[nodiscard]] requite::ObjectBlock &getObjectBlock();
  [[nodiscard]] const requite::ObjectBlock &getObjectBlock() const;
  [[nodiscard]] bool getHasLocalBlock() const;
  void setLocalBlock(requite::LocalBlock &block);
  [[nodiscard]] requite::LocalBlock &getLocalBlock();
  [[nodiscard]] const requite::LocalBlock &getLocalBlock() const;
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
  [[nodiscard]] bool getHasGeneratedName() const;
  void setHasGeneratedName();

  // lookup_symbols.cpp
  [[nodiscard]]
  requite::RootSymbol lookupUserSymbol(llvm::StringRef name);
  [[nodiscard]] bool getHasUserSymbolOfName(llvm::StringRef name) const;

  // detail/scope_symbol_map.hpp
  template <typename SymbolArg> void addUserSymbol(SymbolArg &symbol);

  // detail/scope_subrange.hpp
  [[nodiscard]] inline std::ranges::subrange<
      requite::ContainingScopeIterator, requite::ContainingScopeIterator,
      std::ranges::subrange_kind::unsized>
  getContainingSubrange();
};

} // namespace requite

#include <requite/detail/scope_subrange.hpp>
#include <requite/detail/scope_symbol_map.hpp>
