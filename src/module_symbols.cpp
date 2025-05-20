// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/variable.hpp>
#include <requite/module.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/assert.hpp>

namespace requite {

requite::Scope &Module::makeScope() {
  std::unique_ptr<requite::Scope> &scope_uptr =
      this->_scopes.emplace_back(std::make_unique<requite::Scope>());
  return requite::getRef(scope_uptr);
}

requite::Table &Module::makeTable() {
  std::unique_ptr<requite::Table> &table_uptr =
      this->_tables.emplace_back(std::make_unique<requite::Table>());
  return requite::getRef(table_uptr);
}

requite::Object &Module::makeObject() {
  std::unique_ptr<requite::Object> &object_uptr =
      this->_objects.emplace_back(std::make_unique<requite::Object>());
  return requite::getRef(object_uptr);
}

requite::NamedProcedureGroup &Module::makeNamedProcedureGroup() {
  std::unique_ptr<requite::NamedProcedureGroup> &procedure_uptr =
      this->_named_procedure_groups.emplace_back(std::make_unique<requite::NamedProcedureGroup>());
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Module::makeProcedure() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedures.emplace_back(std::make_unique<requite::Procedure>());
  return requite::getRef(procedure_uptr);
}

requite::Alias &Module::makeAlias() {
  std::unique_ptr<requite::Alias> &alias_uptr =
      this->_aliases.emplace_back(std::make_unique<requite::Alias>());
  return requite::getRef(alias_uptr);
}

requite::Variable &Module::makeVariable() {
  std::unique_ptr<requite::Variable> &variable_uptr =
      this->_variables.emplace_back(std::make_unique<requite::Variable>());
  return requite::getRef(variable_uptr);
}

requite::AnonymousFunction &Module::makeAnonymousFunction() {
  std::unique_ptr<requite::AnonymousFunction> &anonymous_function_uptr =
      this->_anonymous_functions.emplace_back(
          std::make_unique<requite::AnonymousFunction>());
  return requite::getRef(anonymous_function_uptr);
}

requite::Label& Module::makeLabel()
{
  std::unique_ptr<requite::Label> &label_uptr =
      this->_labels.emplace_back(std::make_unique<requite::Label>());
  return requite::getRef(label_uptr);
}

std::vector<std::unique_ptr<requite::Scope>> &Module::getScopes() {
  return this->_scopes;
}

const std::vector<std::unique_ptr<requite::Scope>> &Module::getScopes() const {
  return this->_scopes;
}

std::vector<std::unique_ptr<requite::Table>> &Module::getTables() {
  return this->_tables;
}

const std::vector<std::unique_ptr<requite::Table>> &Module::getTables() const {
  return this->_tables;
}

std::vector<std::unique_ptr<requite::Object>> &Module::getObjects() {
  return this->_objects;
}

const std::vector<std::unique_ptr<requite::Object>> &
Module::getObjects() const {
  return this->_objects;
}

std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &Module::getNamedProcedureGroups() {
  return this->_named_procedure_groups;
}

const std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
Module::getNamedProcedureGroups() const {
  return this->_named_procedure_groups;
}

std::vector<std::unique_ptr<requite::Procedure>> &Module::getProcedures() {
  return this->_procedures;
}

const std::vector<std::unique_ptr<requite::Procedure>> &
Module::getProcedures() const {
  return this->_procedures;
}

std::vector<std::unique_ptr<requite::Alias>> &Module::getAliases() {
  return this->_aliases;
}

const std::vector<std::unique_ptr<requite::Alias>> &Module::getAliases() const {
  return this->_aliases;
}

std::vector<std::unique_ptr<requite::Variable>> &Module::getVariables() {
  return this->_variables;
}

const std::vector<std::unique_ptr<requite::Variable>> &
Module::getVariables() const {
  return this->_variables;
}

std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Module::getAnonymousFunctions() {
  return this->_anonymous_functions;
}

const std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Module::getAnonymousFunctions() const {
  return this->_anonymous_functions;
}

} // namespace requite