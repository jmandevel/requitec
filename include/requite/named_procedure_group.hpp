// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/procedure.hpp>
#include <requite/procedure_type.hpp>
#include <requite/symbol.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Object;
struct Module;

struct NamedProcedureGroup final {
  using Self = requite::NamedProcedureGroup;

  requite::ProcedureType _type = requite::ProcedureType::NONE;
  std::string _name = {};
  requite::Object *_object_ptr = nullptr;
  requite::Procedure *_first_procedure_ptr = nullptr;
  requite::Module *_module_ptr = nullptr;
  bool _is_exported = false;

  // named_procedure_group.cpp
  NamedProcedureGroup() = default;
  NamedProcedureGroup(Self &that) = delete;
  NamedProcedureGroup(Self &&that) = default;
  ~NamedProcedureGroup() = default;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  void setType(requite::ProcedureType type);
  [[nodiscard]]
  requite::ProcedureType getType() const;
  [[nodiscard]]
  llvm::StringRef getName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]]
  bool getHasName() const;
  [[nodiscard]]
  bool getHasProcedures() const;
  [[nodiscard]]
  requite::Procedure &getFirstProcedure();
  [[nodiscard]]
  const requite::Procedure &getFirstProcedure() const;
  void setModule(requite::Module &module);
  [[nodiscard]]
  bool getHasModule() const;
  [[nodiscard]]
  requite::Module &getModule();
  [[nodiscard]]
  const requite::Module &getModule() const;
  void setAsExported();
  [[nodiscard]]
  bool getIsExported() const;
  void addProcedure(requite::Procedure &procedure);
  void setObject(requite::Object &object);
  [[nodiscard]]
  bool getHasObject() const;
  [[nodiscard]]
  requite::Object &getObject();
  [[nodiscard]]
  const requite::Object &getObject() const;
  [[nodiscard]]
  requite::Object *getObjectPtr();
  [[nodiscard]]
  const requite::Object *getObjectPtr() const;
};

} // namespace requite