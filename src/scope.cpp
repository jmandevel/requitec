// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

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

const llvm::StringMap<requite::LookupTableEntry> Scope::getLookupTable() const {
  return this->_symbol_map;
}

bool Scope::getCanHaveLocal() const {
  return requite::getCanHaveLocal(this->getType());
}

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

bool Scope::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void Scope::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
  this->_scope_depth = scope.getScopeDepth();
}

requite::Scope &Scope::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Scope::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

unsigned Scope::getScopeDepth() const { return this->_scope_depth; }

requite::Scope *Scope::getContainingPtr() {
  return this->_containing_scope_ptr;
}

const requite::Scope *Scope::getContainingPtr() const {
  return this->_containing_scope_ptr;
}

bool Scope::getIsEmpty() const { return this->_symbol_map.empty(); }

bool Scope::getIsObject() const {
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

bool Scope::getIsTable() const { return this->_table_ptr != nullptr; }

void Scope::setTable(requite::Table &table) {
  REQUITE_ASSERT(this->getIsEmpty());
  requite::setSingleRef(this->_table_ptr, table);
}

requite::Table &Scope::getTable() { return requite::getRef(this->_table_ptr); }

const requite::Table &Scope::getTable() const {
  return requite::getRef(this->_table_ptr);
}

bool Scope::getIsProcedure() const {
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

bool Scope::getIsAnonymousFunction() const {
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

bool Scope::getIsBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BLOCK);
  return this->_block_ptr != nullptr;
}

void Scope::setBlock(requite::Block &block) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::NONE);
  this->_type = requite::ScopeType::BLOCK;
  requite::setSingleRef(this->_block_ptr, block);
}

requite::Block &Scope::getBlock() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BLOCK);
  return requite::getRef(this->_block_ptr);
}

const requite::Block &Scope::getBlock() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::BLOCK);
  return requite::getRef(this->_block_ptr);
}

} // namespace requite