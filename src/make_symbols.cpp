// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/global.hpp>
#include <requite/local.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>

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

requite::Procedure &Context::makeEntryPoint() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(std::make_unique<requite::Procedure>(
          requite::ProcedureType::ENTRY_POINT));
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Context::makeFunction() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(std::make_unique<requite::Procedure>(
          requite::ProcedureType::FUNCTION));
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Context::makeConstructor() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(std::make_unique<requite::Procedure>(
          requite::ProcedureType::CONSTRUCTOR));
  return requite::getRef(procedure_uptr);
}

requite::Procedure &Context::makeDestructor() {
  std::unique_ptr<requite::Procedure> &procedure_uptr =
      this->_procedure_uptrs.emplace_back(std::make_unique<requite::Procedure>(
          requite::ProcedureType::DESTRUCTOR));
  return requite::getRef(procedure_uptr);
}

requite::Alias &Context::makeAlias() {
  std::unique_ptr<requite::Alias> &alias_uptr =
      this->_alias_uptrs.emplace_back(std::make_unique<requite::Alias>());
  return requite::getRef(alias_uptr);
}

requite::Local &Context::makeLocal() {
  std::unique_ptr<requite::Local> &local_uptr =
      this->_local_uptrs.emplace_back(std::make_unique<requite::Local>());
  return requite::getRef(local_uptr);
}

requite::Global &Context::makeGlobal() {
  std::unique_ptr<requite::Global> &global_uptr =
      this->_global_uptrs.emplace_back(std::make_unique<requite::Global>());
  return requite::getRef(global_uptr);
}

requite::Property &Context::makeProperty() {
  std::unique_ptr<requite::Property> &property_uptr =
      this->_property_uptrs.emplace_back(std::make_unique<requite::Property>());
  return requite::getRef(property_uptr);
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

requite::Import &Context::makeImport() {
  std::unique_ptr<requite::Import> &import_ptr =
      this->_import_uptrs.emplace_back(std::make_unique<requite::Import>());
  return requite::getRef(import_ptr);
}

requite::Use &Context::makeUse() {
  std::unique_ptr<requite::Use> &use_uptr =
      this->_use_uptrs.emplace_back(std::make_unique<requite::Use>());
  return requite::getRef(use_uptr);
}

requite::UseTable &Context::makeUseTable() {
  std::unique_ptr<requite::UseTable> &use_table_uptr =
      this->_use_table_uptrs.emplace_back(std::make_unique<requite::UseTable>());
  return requite::getRef(use_table_uptr);
}

requite::Block &Context::makeBlock() {
  std::unique_ptr<requite::Block> &block_uptr =
      this->_block_uptrs.emplace_back(std::make_unique<requite::Block>());
  return requite::getRef(block_uptr);
}

requite::Module &Context::makeModule() {
  std::unique_ptr<requite::Module> &module_uptr =
      this->_module_uptrs.emplace_back(std::make_unique<requite::Module>());
  return requite::getRef(module_uptr);
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

std::vector<std::unique_ptr<requite::Local>> &Context::getLocalUptrs() {
  return this->_local_uptrs;
}

const std::vector<std::unique_ptr<requite::Local>> &
Context::getLocalUptrs() const {
  return this->_local_uptrs;
}

std::vector<std::unique_ptr<requite::Global>> &Context::getGlobalUptrs() {
  return this->_global_uptrs;
}

const std::vector<std::unique_ptr<requite::Global>> &
Context::getGlobalUptrs() const {
  return this->_global_uptrs;
}

std::vector<std::unique_ptr<requite::Property>> &Context::getPropertyUptrs() {
  return this->_property_uptrs;
}

const std::vector<std::unique_ptr<requite::Property>> &
Context::getPropertyUptrs() const {
  return this->_property_uptrs;
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

std::vector<std::unique_ptr<requite::Import>> &Context::getImportUptrs() {
  return this->_import_uptrs;
}

const std::vector<std::unique_ptr<requite::Import>> &
Context::getImportUptrs() const {
  return this->_import_uptrs;
}

std::vector<std::unique_ptr<requite::Use>> &Context::getUseUptrs() {
  return this->_use_uptrs;
}

const std::vector<std::unique_ptr<requite::Use>> &Context::getUseUptrs() const {
  return this->_use_uptrs;
}

std::vector<std::unique_ptr<requite::UseTable>> &Context::getUseTableUptrs() {
  return this->_use_table_uptrs;
}

const std::vector<std::unique_ptr<requite::UseTable>> &Context::getUseTableUptrs() const {
  return this->_use_table_uptrs;
}

std::vector<std::unique_ptr<requite::Block>> &Context::getBlockUptrs() {
  return this->_block_uptrs;
}

const std::vector<std::unique_ptr<requite::Block>> &
Context::getBlockUptrs() const {
  return this->_block_uptrs;
}

std::vector<std::unique_ptr<requite::Module>> &Context::getModuleUptrs() {
  return this->_module_uptrs;
}

const std::vector<std::unique_ptr<requite::Module>> &
Context::getModuleUptrs() const {
  return this->_module_uptrs;
}

} // namespace requite