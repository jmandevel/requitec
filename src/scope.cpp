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

bool Scope::getHasSymbolTable() const {
  return this->_symbol_table_ptr != nullptr;
}

void Scope::setSymbolTable(requite::SymbolTable &table) {
  requite::setSingleRef(this->_symbol_table_ptr, table);
}

requite::SymbolTable &Scope::getSymbolTable() {
  return requite::getRef(this->_symbol_table_ptr);
}

const requite::SymbolTable &Scope::getSymbolTable() const {
  return requite::getRef(this->_symbol_table_ptr);
}

bool Scope::getIsEmpty() const {
  return this->getNodes().empty() &&
         (!this->getHasSymbolTable() || this->getSymbolTable().getIsEmpty());
}

bool Scope::getHasObject() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT);
  return this->_object_ptr != nullptr;
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

bool Scope::getHasTable() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
  return this->_table_ptr != nullptr;
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

bool Scope::getHasProcedure() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::PROCEDURE);
  return this->_procedure_ptr != nullptr;
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

bool Scope::getHasAnonymousFunction() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::ANONYMOUS_FUNCTION);
  return this->_anonymous_function_ptr != nullptr;
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

bool Scope::getHasGlobal() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::GLOBAL);
  return this->_global_ptr != nullptr;
}

void Scope::setGlobal(requite::Global &variable) {
  REQUITE_ASSERT(this->getIsEmpty());
  this->_type = requite::ScopeType::GLOBAL;
  requite::setSingleRef(this->_global_ptr, variable);
}

requite::Global &Scope::getGlobal() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::GLOBAL);
  return requite::getRef(this->_global_ptr);
}

const requite::Global &Scope::getGlobal() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::GLOBAL);
  return requite::getRef(this->_global_ptr);
}

bool Scope::getHasBaseOrTableBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BASE_OR_TABLE_BLOCK);
  return this->_base_or_table_block_ptr != nullptr;
}

void Scope::setBaseOrTableBlock(requite::BaseOrTableBlock &block) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BASE_OR_TABLE_BLOCK);
  requite::setSingleRef(this->_base_or_table_block_ptr, block);
}

requite::BaseOrTableBlock &Scope::getBaseOrTableBlock() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BASE_OR_TABLE_BLOCK);
  return requite::getRef(this->_base_or_table_block_ptr);
}

const requite::BaseOrTableBlock &Scope::getBaseOrTableBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BASE_OR_TABLE_BLOCK);
  return requite::getRef(this->_base_or_table_block_ptr);
}

bool Scope::getHasObjectBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT_BLOCK);
  return this->_object_block_ptr != nullptr;
}

void Scope::setObjectBlock(requite::ObjectBlock &block) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT_BLOCK);
  requite::setSingleRef(this->_object_block_ptr, block);
}

requite::ObjectBlock &Scope::getObjectBlock() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT_BLOCK);
  return requite::getRef(this->_object_block_ptr);
}

const requite::ObjectBlock &Scope::getObjectBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT_BLOCK);
  return requite::getRef(this->_object_block_ptr);
}

bool Scope::getHasLocalBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_BLOCK);
  return this->_local_block_ptr != nullptr;
}

void Scope::setLocalBlock(requite::LocalBlock &block) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_BLOCK);
  requite::setSingleRef(this->_local_block_ptr, block);
}

requite::LocalBlock &Scope::getLocalBlock() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_BLOCK);
  return requite::getRef(this->_local_block_ptr);
}

const requite::LocalBlock &Scope::getLocalBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::LOCAL_BLOCK);
  return requite::getRef(this->_local_block_ptr);
}

} // namespace requite