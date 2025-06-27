// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/node.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/ordered_variable.hpp>
#include <requite/unordered_variable.hpp>

namespace requite {

requite::Scope &Context::makeScope() {
  std::unique_ptr<requite::Scope> &scope_uptr =
      this->_scope_uptrs.emplace_back(std::make_unique<requite::Scope>());
  requite::Scope &scope = requite::getRef(scope_uptr);
  return scope;
}

requite::Table &Context::makeTable() {
  std::unique_ptr<requite::Table> &table_uptr =
      this->_table_uptrs.emplace_back(std::make_unique<requite::Table>());
  return requite::getRef(table_uptr);
}

requite::Object &Context::makeObject() {
  std::unique_ptr<requite::Object> &object_uptr =
      this->_object_uptrs.emplace_back(std::make_unique<requite::Object>());
  return requite::getRef(object_uptr);
}

requite::NamedProcedureGroup &Context::makeNamedProcedureGroup() {
  std::unique_ptr<requite::NamedProcedureGroup> &procedure_uptr =
      this->_named_procedure_group_uptrs.emplace_back(
          std::make_unique<requite::NamedProcedureGroup>());
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Context::makeProcedure() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(
          std::make_unique<requite::Procedure>());
  return requite::getRef(procedure_uptr);
}

requite::Alias &Context::makeAlias() {
  std::unique_ptr<requite::Alias> &alias_uptr =
      this->_alias_uptrs.emplace_back(std::make_unique<requite::Alias>());
  return requite::getRef(alias_uptr);
}

requite::OrderedVariable &Context::makeOrderedVariable() {
  std::unique_ptr<requite::OrderedVariable> &ordered_variable_uptr =
      this->_ordered_variable_uptrs.emplace_back(std::make_unique<requite::OrderedVariable>());
  return requite::getRef(ordered_variable_uptr);
}

requite::UnorderedVariable &Context::makeUnorderedVariable() {
  std::unique_ptr<requite::UnorderedVariable> &unordered_variable_uptr =
      this->_unordered_variable_uptrs.emplace_back(std::make_unique<requite::UnorderedVariable>());
  return requite::getRef(unordered_variable_uptr);
}

requite::AnonymousFunction &Context::makeAnonymousFunction() {
  std::unique_ptr<requite::AnonymousFunction> &anonymous_function_uptr =
      this->_anonymous_function_uptrs.emplace_back(
          std::make_unique<requite::AnonymousFunction>());
  return requite::getRef(anonymous_function_uptr);
}

requite::Label &Context::makeLabel() {
  std::unique_ptr<requite::Label> &label_uptr =
      this->_label_uptrs.emplace_back(std::make_unique<requite::Label>());
  return requite::getRef(label_uptr);
}

std::vector<std::unique_ptr<requite::Scope>> &Context::getScopeUptrs() {
  return this->_scope_uptrs;
}

const std::vector<std::unique_ptr<requite::Scope>> &
Context::getScopeUptrs() const {
  return this->_scope_uptrs;
}

std::vector<std::unique_ptr<requite::Table>> &Context::getTableUptrs() {
  return this->_table_uptrs;
}

const std::vector<std::unique_ptr<requite::Table>> &
Context::getTableUptrs() const {
  return this->_table_uptrs;
}

std::vector<std::unique_ptr<requite::Object>> &Context::getObjectUptrs() {
  return this->_object_uptrs;
}

const std::vector<std::unique_ptr<requite::Object>> &
Context::getObjectUptrs() const {
  return this->_object_uptrs;
}

std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
Context::getNamedProcedureGroupUptrs() {
  return this->_named_procedure_group_uptrs;
}

const std::vector<std::unique_ptr<requite::NamedProcedureGroup>> &
Context::getNamedProcedureGroupUptrs() const {
  return this->_named_procedure_group_uptrs;
}

std::vector<std::unique_ptr<requite::Procedure>> &Context::getProcedureUptrs() {
  return this->_procedure_uptrs;
}

const std::vector<std::unique_ptr<requite::Procedure>> &
Context::getProcedureUptrs() const {
  return this->_procedure_uptrs;
}

std::vector<std::unique_ptr<requite::Alias>> &Context::getAliasUptrs() {
  return this->_alias_uptrs;
}

const std::vector<std::unique_ptr<requite::Alias>> &
Context::getAliasUptrs() const {
  return this->_alias_uptrs;
}

std::vector<std::unique_ptr<requite::OrderedVariable>> &Context::getOrderedVariableUptrs() {
  return this->_ordered_variable_uptrs;
}

const std::vector<std::unique_ptr<requite::OrderedVariable>> &
Context::getOrderedVariableUptrs() const {
  return this->_ordered_variable_uptrs;
}

std::vector<std::unique_ptr<requite::UnorderedVariable>> &Context::getUnorderedVariableUptrs() {
  return this->_unordered_variable_uptrs;
}

const std::vector<std::unique_ptr<requite::UnorderedVariable>> &
Context::getUnorderedVariableUptrs() const {
  return this->_unordered_variable_uptrs;
}

std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Context::getAnonymousFunctionUptrs() {
  return this->_anonymous_function_uptrs;
}

const std::vector<std::unique_ptr<requite::AnonymousFunction>> &
Context::getAnonymousFunctionUptrs() const {
  return this->_anonymous_function_uptrs;
}

std::vector<std::unique_ptr<requite::Label>> &Context::getLabelUptrs() {
  return this->_label_uptrs;
}

const std::vector<std::unique_ptr<requite::Label>> &
Context::getLabelUptrs() const {
  return this->_label_uptrs;
}

} // namespace requite