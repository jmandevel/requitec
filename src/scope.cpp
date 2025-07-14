// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/assert.hpp>
#include <requite/import.hpp>
#include <requite/module.hpp>
#include <requite/object.hpp>
#include <requite/scope.hpp>
#include <requite/table.hpp>
#include <requite/use.hpp>

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

bool Scope::getHasImport() const { return this->_first_import_ptr != nullptr; }

void Scope::addImport(requite::Import &import) {
  REQUITE_ASSERT(!import.getHasNext());
  import._next_ptr = this->_first_import_ptr;
  this->_first_import_ptr = &import;
}

requite::Import &Scope::getFirstImport() {
  return requite::getRef(this->_first_import_ptr);
}

const requite::Import &Scope::getFirstImport() const {
  return requite::getRef(this->_first_import_ptr);
}

bool Scope::getHasUse() const { return this->_first_import_ptr != nullptr; }

void Scope::addUse(requite::Use &use) {
  REQUITE_ASSERT(!use.getHasNext());
  use._next_ptr = this->_first_use_ptr;
  this->_first_use_ptr = &use;
}

requite::Use &Scope::getFirstUse() {
  return requite::getRef(this->_first_use_ptr);
}

const requite::Use &Scope::getFirstUse() const {
  return requite::getRef(this->_first_use_ptr);
}

bool Scope::getIsEmpty() const { return this->_symbol_map.empty(); }

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

bool Scope::getHasTable() const { return this->_table_ptr != nullptr; }

void Scope::setTable(requite::Table &table) {
  REQUITE_ASSERT(this->getIsEmpty());
  requite::setSingleRef(this->_table_ptr, table);
}

requite::Table &Scope::getTable() { return requite::getRef(this->_table_ptr); }

const requite::Table &Scope::getTable() const {
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

bool Scope::getHasBlock() const { return this->_block_ptr != nullptr; }

void Scope::setBlock(requite::Block &block) {
  requite::setSingleRef(this->_block_ptr, block);
}

requite::Block &Scope::getBlock() { return requite::getRef(this->_block_ptr); }

const requite::Block &Scope::getBlock() const {
  return requite::getRef(this->_block_ptr);
}

} // namespace requite