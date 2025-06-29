// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/attribute_flags.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/table.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>
#include <vector>

namespace requite {

struct Expression;
struct Procedure;
struct UnorderedVariable;

struct Object final {
  using Self = requite::Object;

  std::string _name = {};
  requite::Expression* _expression_ptr = nullptr;
  requite::Scope _scope = {};
  std::string _mangled_name = {};
  requite::Procedure *_destructor_ptr = nullptr;
  requite::Procedure *_first_constructor_ptr = nullptr;
  requite::Module *_module_ptr = nullptr;
  requite::AttributeFlags _attributes = {};
  std::vector<requite::UnorderedVariable *> _property_ptrs = {};

  // object.cpp
  Object();
  Object(const Self &) = delete;
  Object(Self &&) = delete;
  ~Object() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  void setAttributeFlags(requite::AttributeFlags attributes);
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
  [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
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
  void setModule(requite::Module &module);
  [[nodiscard]] bool getHasModule() const;
  [[nodiscard]] requite::Module &getModule();
  [[nodiscard]] const requite::Module &getModule() const;
};

} // namespace requite
