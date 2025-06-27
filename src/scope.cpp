// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/assert.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/object.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>

namespace requite {

bool Scope::operator==(const Self &rhs) const { return this == &rhs; }

bool Scope::operator!=(Self &&rhs) const { return this != &rhs; }

bool Scope::getHasModule() const {
  return this->getType() == requite::ScopeType::MODULE;
}

void Scope::setModule(requite::Module &module) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::MODULE;
  requite::setSingleRef(this->_module_ptr, module);
}

requite::Module &Scope::getModule() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::MODULE);
  return requite::getRef(this->_module_ptr);
}

const requite::Module &Scope::getModule() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::MODULE);
  return requite::getRef(this->_module_ptr);
}

requite::ScopeType Scope::getType() const { return this->_type; }

llvm::StringMap<requite::RootSymbol> &Scope::getInternalSymbolMap() {
  return this->_internal_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &Scope::getInternalSymbolMap() const {
  return this->_internal_symbol_map;
}

bool Scope::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void Scope::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
  this->_scope_depth = scope.getScopeDepth();
}

requite::Scope &Scope::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Scope::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

unsigned Scope::getScopeDepth() const { return this->_scope_depth; }

requite::Scope *Scope::getContainingPtr() {
  return this->_containing_scope_ptr;
}

const requite::Scope *Scope::getContainingPtr() const {
  return this->_containing_scope_ptr;
}

std::vector<requite::Node> &Scope::getNodes() { return this->_nodes; }

const std::vector<requite::Node> &Scope::getNodes() const {
  return this->_nodes;
}

bool Scope::getHasExportTable() const {
  return this->_export_table_ptr != nullptr;
}

void Scope::setExportTable(requite::ExportTable &table) {
  requite::setSingleRef(this->_export_table_ptr, table);
}

requite::ExportTable &Scope::getExportTable() {
  return requite::getRef(this->_export_table_ptr);
}

const requite::ExportTable &Scope::getExportTable() const {
  return requite::getRef(this->_export_table_ptr);
}

bool Scope::getIsEmpty() const {
  return this->getInternalSymbolMap().empty() && this->getNodes().empty() &&
         !this->getHasExportTable();
}

void Scope::setObject(requite::Object &object) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::OBJECT;
  requite::setSingleRef(this->_object_ptr, object);
}

requite::Object &Scope::getObject() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT);
  return requite::getRef(this->_object_ptr);
}

const requite::Object &Scope::getObject() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT);
  return requite::getRef(this->_object_ptr);
}

void Scope::setTable(requite::Table &table) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::TABLE;
  requite::setSingleRef(this->_table_ptr, table);
}

requite::Table &Scope::getTable() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
  return requite::getRef(this->_table_ptr);
}

const requite::Table &Scope::getTable() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
  return requite::getRef(this->_table_ptr);
}

void Scope::setProcedure(requite::Procedure &procedure) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::PROCEDURE;
  requite::setSingleRef(this->_procedure_ptr, procedure);
}

requite::Procedure &Scope::getProcedure() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::PROCEDURE);
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &Scope::getProcedure() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::PROCEDURE);
  return requite::getRef(this->_procedure_ptr);
}

void Scope::setAnonymousFunction(
    requite::AnonymousFunction &anonymous_function) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::ANONYMOUS_FUNCTION;
  requite::setSingleRef(this->_anonymous_function_ptr, anonymous_function);
}

requite::AnonymousFunction &Scope::getAnonymousFunction() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::ANONYMOUS_FUNCTION);
  return requite::getRef(this->_anonymous_function_ptr);
}

const requite::AnonymousFunction &Scope::getAnonymousFunction() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::ANONYMOUS_FUNCTION);
  return requite::getRef(this->_anonymous_function_ptr);
}

void Scope::setLocalStatement(requite::Expression &expression) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::LOCAL_STATEMENT;
  requite::setSingleRef(this->_local_statement_ptr, expression);
}

requite::Expression &Scope::getLocalStatement() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_STATEMENT);
  return requite::getRef(this->_local_statement_ptr);
}

const requite::Expression &Scope::getLocalStatement() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_STATEMENT);
  return requite::getRef(this->_local_statement_ptr);
}

void Scope::setUnorderedVariable(requite::UnorderedVariable &variable) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::UNORDERED_VARIABLE;
  requite::setSingleRef(this->_unordered_variable_ptr, variable);
}

requite::UnorderedVariable &Scope::getUnorderedVariable() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::UNORDERED_VARIABLE);
  return requite::getRef(this->_unordered_variable_ptr);
}

const requite::UnorderedVariable &Scope::getUnorderedVariable() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::UNORDERED_VARIABLE);
  return requite::getRef(this->_unordered_variable_ptr);
}

} // namespace requite