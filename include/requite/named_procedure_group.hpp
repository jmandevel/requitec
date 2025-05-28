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

  std::string _name = {};
  requite::Scope* _containing_scope_ptr = nullptr;
  requite::Procedure *_first_ptr = nullptr;

  // named_procedure_group.cpp
  NamedProcedureGroup() = default;
  NamedProcedureGroup(Self &that) = delete;
  NamedProcedureGroup(Self &&that) = default;
  ~NamedProcedureGroup() = default;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] llvm::StringRef getName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] bool getHasName() const;
  [[nodiscard]] bool getHasProcedures() const;
  [[nodiscard]] requite::Procedure &getFirstProcedure();
  [[nodiscard]] const requite::Procedure &getFirstProcedure() const;
  void addProcedure(requite::Procedure &procedure);
  [[nodiscard]] bool getHasContainingScope() const;
  void setContainingScope(requite::Scope &scope);
  [[nodiscard]] requite::Scope &getContainingScope();
  [[nodiscard]] const requite::Scope &getContainingScope() const;
};

} // namespace requite