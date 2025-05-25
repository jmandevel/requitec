// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attributes.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/table.hpp>

#include <llvm/ADT/StringRef.h>

#include <vector>

namespace requite {

struct Expression;
struct Procedure;
struct Variable;

struct Object final {
  requite::Table _table = {};
  llvm::StringRef _mangled_name = {};
  requite::Procedure *_destructor_ptr = nullptr;
  requite::Procedure *_first_constructor_ptr = nullptr;
  requite::Expression *_ascribe_ptr = nullptr;
  requite::Module *_module_ptr = nullptr;
  requite::Attributes _attributes = {};
  std::vector<requite::Variable *> _property_ptrs = {};

  // object.cpp
  [[nodiscard]]
  bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] requite::Scope& getScope();
  [[nodiscard]] const requite::Scope& getScope() const;
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope& scope);
  [[nodiscard]] requite::Scope& getContainingScope();
  [[nodiscard]] const requite::Scope& getContainingScope() const;
  void setMangledName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getMangledName() const;
  [[nodiscard]] bool getHasMangledName() const;
  void setDestructor(requite::Procedure &destructor);
  [[nodiscard]] bool getHasDestructor() const;
  [[nodiscard]] requite::Procedure &getDestructor();
  [[nodiscard]] const requite::Procedure &getDestructor() const;
  void addConstructor(requite::Procedure &constructor);
  [[nodiscard]] bool getHasConstructor() const;
  [[nodiscard]] requite::Procedure &getFirstConstructor();
  [[nodiscard]] const requite::Procedure &getFirstConstructor() const;
  void setAscribe(requite::Expression &ascribe);
  [[nodiscard]] bool getHasAscribe() const;
  [[nodiscard]] requite::Expression &getAscribe();
  [[nodiscard]] const requite::Expression &getAscribe() const;
  void setModule(requite::Module &module);
  [[nodiscard]] bool getHasModule() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  [[nodiscard]] requite::Attributes &getAttributes();
};

} // namespace requite
