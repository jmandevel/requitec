// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/alias.hpp>
#include <requite/anonymous_function.hpp>
#include <requite/file.hpp>
#include <requite/label.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/variable.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Module.h>

#include <memory>
#include <string>
#include <vector>

namespace requite {

struct Context;

struct Module final {
  using Self = requite::Module;

  requite::Table _table = {};
  requite::File _file = {};
  std::unique_ptr<llvm::Module> _llvm_module_uptr = nullptr;
  std::vector<std::unique_ptr<requite::Scope>> _scope_uptrs = {};
  std::vector<std::unique_ptr<requite::Table>> _table_uptrs = {};
  std::vector<std::unique_ptr<requite::Object>> _object_uptrs = {};
  std::vector<std::unique_ptr<requite::NamedProcedureGroup>>
      _named_procedure_group_uptrs = {};
  std::vector<std::unique_ptr<requite::Procedure>> _procedure_uptrs = {};
  std::vector<std::unique_ptr<requite::Alias>> _alias_uptrs = {};
  std::vector<std::unique_ptr<requite::Variable>> _variable_uptrs = {};
  std::vector<std::unique_ptr<requite::AnonymousFunction>>
      _anonymous_function_uptrs = {};
  std::vector<std::unique_ptr<requite::Label>> _label_uptrs = {};
  requite::Procedure *_entry_point_ptr = nullptr;

  Module() = default;
  Module(Self &that) = delete;
  Module(Self &&that) = delete;
  Self &operator=(Self &rhs) = delete;
  Self &operator=(Self &&rhs) = delete;
  ~Module() = default;

  // module_symbols.cpp
  [[nodiscard]] requite::Scope &makeScope();
  [[nodiscard]] requite::Table &makeTable();
  [[nodiscard]] requite::Object &makeObject();
  [[nodiscard]] requite::NamedProcedureGroup &makeNamedProcedureGroup();
  [[nodiscard]] requite::Procedure &makeProcedure();
  [[nodiscard]] requite::Alias &makeAlias();
  [[nodiscard]] requite::Variable &makeVariable();
  [[nodiscard]] requite::AnonymousFunction &makeAnonymousFunction();
  [[nodiscard]] requite::Label &makeLabel();
  [[nodiscard]] std::vector<std::unique_ptr<requite::Scope>> &getScopeUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Scope>> &
  getScopeUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Table>> &getTableUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Table>> &
  getTableUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Object>> &getObjectUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Object>> &
  getObjectUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
  getNamedProcedureGroupUptrs();
  [[nodiscard]] const std::vector<
      std::unique_ptr<requite::NamedProcedureGroup>> &
  getNamedProcedureGroupUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Procedure>> &
  getProcedureUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Procedure>> &
  getProcedureUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Alias>> &getAliasUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Alias>> &
  getAliasUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Variable>> &
  getVariableUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Variable>> &
  getVariableUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::AnonymousFunction>> &
  getAnonymousFunctionUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::AnonymousFunction>> &
  getAnonymousFunctionUptrs() const;
  [[nodiscard]] std::vector<std::unique_ptr<requite::Label>> &getLabelUptrs();
  [[nodiscard]] const std::vector<std::unique_ptr<requite::Label>> &
  getLabelUptrs() const;

  // module.cpp
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]] bool getHasName() const;
  void setName(llvm::StringRef name);
  [[nodiscard]] llvm::StringRef getName() const;
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] requite::File &getFile();
  [[nodiscard]] const requite::File &getFile() const;
  [[nodiscard]] bool getHasEntryPoint() const;
  void setEntryPoint(requite::Procedure &procedure);
  [[nodiscard]] requite::Procedure &getEntryPoint();
  [[nodiscard]] const requite::Procedure &getEntryPoint() const;
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

  /// llvm_module.cpp
  void initializeLlvmModule(requite::Context &context);
  [[nodiscard]] bool getIsLlvmModuleInitialized() const;
  void terminateLlvmModule();
  [[nodiscard]] llvm::Module &getLlvmModule();
  [[nodiscard]] const llvm::Module &getLlvmModule() const;
  [[nodiscard]] std::string getLlvmIrSourceText() const;
};

} // namespace requite