// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/module.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/ordered_variable.hpp>
#include <requite/unordered_variable.hpp>

namespace requite {

requite::Scope &Module::makeScope() {
  std::unique_ptr<requite::Scope> &scope_uptr =
      this->_scope_uptrs.emplace_back(std::make_unique<requite::Scope>());
  requite::Scope &scope = requite::getRef(scope_uptr);
  scope.setModule(*this);
  return scope;
}

requite::Table &Module::makeTable() {
  std::unique_ptr<requite::Table> &table_uptr =
      this->_table_uptrs.emplace_back(std::make_unique<requite::Table>());
  return requite::getRef(table_uptr);
}

requite::Object &Module::makeObject() {
  std::unique_ptr<requite::Object> &object_uptr =
      this->_object_uptrs.emplace_back(std::make_unique<requite::Object>());
  return requite::getRef(object_uptr);
}

requite::NamedProcedureGroup &Module::makeNamedProcedureGroup() {
  std::unique_ptr<requite::NamedProcedureGroup> &procedure_uptr =
      this->_named_procedure_group_uptrs.emplace_back(
          std::make_unique<requite::NamedProcedureGroup>());
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Module::makeProcedure() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(
          std::make_unique<requite::Procedure>());
  return requite::getRef(procedure_uptr);
}

requite::Alias &Module::makeAlias() {
  std::unique_ptr<requite::Alias> &alias_uptr =
      this->_alias_uptrs.emplace_back(std::make_unique<requite::Alias>());
  return requite::getRef(alias_uptr);
}

requite::OrderedVariable &Module::makeOrderedVariable() {
  std::unique_ptr<requite::OrderedVariable> &ordered_variable_uptr =
      this->_ordered_variable_uptrs.emplace_back(std::make_unique<requite::OrderedVariable>());
  return requite::getRef(ordered_variable_uptr);
}

requite::UnorderedVariable &Module::makeUnorderedVariable() {
  std::unique_ptr<requite::UnorderedVariable> &unordered_variable_uptr =
      this->_unordered_variable_uptrs.emplace_back(std::make_unique<requite::UnorderedVariable>());
  return requite::getRef(unordered_variable_uptr);
}

requite::AnonymousFunction &Module::makeAnonymousFunction() {
  std::unique_ptr<requite::AnonymousFunction> &anonymous_function_uptr =
      this->_anonymous_function_uptrs.emplace_back(
          std::make_unique<requite::AnonymousFunction>());
  return requite::getRef(anonymous_function_uptr);
}

requite::Label &Module::makeLabel() {
  std::unique_ptr<requite::Label> &label_uptr =
      this->_label_uptrs.emplace_back(std::make_unique<requite::Label>());
  return requite::getRef(label_uptr);
}

std::vector<std::unique_ptr<requite::Scope>> &Module::getScopeUptrs() {
  return this->_scope_uptrs;
}

const std::vector<std::unique_ptr<requite::Scope>> &
Module::getScopeUptrs() const {
  return this->_scope_uptrs;
}

std::vector<std::unique_ptr<requite::Table>> &Module::getTableUptrs() {
  return this->_table_uptrs;
}

const std::vector<std::unique_ptr<requite::Table>> &
Module::getTableUptrs() const {
  return this->_table_uptrs;
}

std::vector<std::unique_ptr<requite::Object>> &Module::getObjectUptrs() {
  return this->_object_uptrs;
}

const std::vector<std::unique_ptr<requite::Object>> &
Module::getObjectUptrs() const {
  return this->_object_uptrs;
}

std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
Module::getNamedProcedureGroupUptrs() {
  return this->_named_procedure_group_uptrs;
}

const std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
Module::getNamedProcedureGroupUptrs() const {
  return this->_named_procedure_group_uptrs;
}

std::vector<std::unique_ptr<requite::Procedure>> &Module::getProcedureUptrs() {
  return this->_procedure_uptrs;
}

const std::vector<std::unique_ptr<requite::Procedure>> &
Module::getProcedureUptrs() const {
  return this->_procedure_uptrs;
}

std::vector<std::unique_ptr<requite::Alias>> &Module::getAliasUptrs() {
  return this->_alias_uptrs;
}

const std::vector<std::unique_ptr<requite::Alias>> &
Module::getAliasUptrs() const {
  return this->_alias_uptrs;
}

std::vector<std::unique_ptr<requite::OrderedVariable>> &Module::getOrderedVariableUptrs() {
  return this->_ordered_variable_uptrs;
}

const std::vector<std::unique_ptr<requite::OrderedVariable>> &
Module::getOrderedVariableUptrs() const {
  return this->_ordered_variable_uptrs;
}

std::vector<std::unique_ptr<requite::UnorderedVariable>> &Module::getUnorderedVariableUptrs() {
  return this->_unordered_variable_uptrs;
}

const std::vector<std::unique_ptr<requite::UnorderedVariable>> &
Module::getUnorderedVariableUptrs() const {
  return this->_unordered_variable_uptrs;
}

std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Module::getAnonymousFunctionUptrs() {
  return this->_anonymous_function_uptrs;
}

const std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Module::getAnonymousFunctionUptrs() const {
  return this->_anonymous_function_uptrs;
}

std::vector<std::unique_ptr<requite::Label>> &Module::getLabelUptrs() {
  return this->_label_uptrs;
}

const std::vector<std::unique_ptr<requite::Label>> &
Module::getLabelUptrs() const {
  return this->_label_uptrs;
}

} // namespace requite