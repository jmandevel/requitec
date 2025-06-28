// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/file.hpp>
#include <requite/scope.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <memory>
#include <string>
#include <vector>

namespace requite {

struct Context;
struct Expression;
struct ExportTable;

struct Module final {
  using Self = requite::Module;

  std::string _name = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::Scope _scope = {};
  requite::File _file = {};
  requite::ExportTable *_export_tble_ptr = nullptr;
  requite::Procedure *_entry_point_ptr = nullptr;

  Module();
  Module(Self &that) = delete;
  Module(Self &&that) = delete;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  ~Module() = default;

  // module_symbols.cpp

  // module.cpp
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::File &getFile();
  [[nodiscard]] const requite::File &getFile() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &
  replaceExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &popExpression();
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] llvm::StringRef getPath() const;
  [[nodiscard]] llvm::StringRef getIdentifier() const;
  [[nodiscard]] llvm::StringRef getText() const;
  [[nodiscard]] const char *getTextPtr() const;
  [[nodiscard]] std::uint_fast32_t getBufferI() const;
  [[nodiscard]] bool getHasEntryPoint() const;
  void addEntryPoint(requite::Procedure &entry_point);
  [[nodiscard]] requite::Procedure &getEntryPoint();
  [[nodiscard]] const requite::Procedure &getEntryPoint() const;
};

} // namespace requite