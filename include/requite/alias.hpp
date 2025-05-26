// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attributes.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Module;
struct Object;
struct Scope;

struct Alias final {
  std::string _name = {};
  requite::Module *_module_ptr = nullptr;
  requite::Object *_object_ptr = nullptr;
  requite::Scope *_containing_scope_ptr = nullptr;
  requite::Expression *_expression_ptr = nullptr;
  requite::Attributes _attributes = {};
  requite::Symbol _symbol = {};

  // alias.cpp
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasName() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasAttributes() const;
  void setAttributes(requite::Attributes attributes);
  [[nodiscard]] requite::Attributes &getAttributes();
  [[nodiscard]] const requite::Attributes &getAttributes() const;
  void setModule(requite::Module &module);
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
  void setObject(requite::Object &object);
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] requite::Object *getObjectPtr();
  [[nodiscard]] const requite::Object *getObjectPtr() const;
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
  [[nodiscard]] requite::Symbol &getSymbol();
  [[nodiscard]] const requite::Symbol &getSymbol() const;
};

} // namespace requite