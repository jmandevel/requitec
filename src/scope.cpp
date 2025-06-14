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

bool Scope::getHasModule() const { return this->_module_ptr != nullptr; }
void Scope::setModule(requite::Module &module) {
  requite::setSingleRef(this->_module_ptr, module);
}

requite::Module &Scope::getModule() {
  return requite::getRef(this->_module_ptr);
}

const requite::Module &Scope::getModule() const {
  return requite::getRef(this->_module_ptr);
}

void Scope::setType(requite::ScopeType type) {
  REQUITE_ASSERT(this->_type == requite::ScopeType::NONE);
  REQUITE_ASSERT(type != requite::ScopeType::NONE);
  this->_type = type;
}

requite::ScopeType Scope::getType() const { return this->_type; }

llvm::StringMap<requite::RootSymbol> &Scope::getSymbolMap() {
  return this->_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &Scope::getSymbolMap() const {
  return this->_symbol_map;
}

bool Scope::getHasContaining() const {
  return this->_containing_scope_ptr != nullptr;
}

void Scope::setContaining(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Scope::getContaining() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Scope::getContaining() const {
  return requite::getRef(this->_containing_scope_ptr);
}

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

bool Scope::getIsEmpty() const {
  return this->_symbol_map.empty() && this->_nodes.empty();
}

bool Scope::getHasObject() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT);
  return this->_object_ptr != nullptr;
}

void Scope::setObject(requite::Object &object) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::OBJECT);
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
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
  requite::setSingleRef(this->_table_ptr, table);
}

requite::Table &Scope::getTable() {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
  return requite::getRef(this->_table_ptr);
}

const requite::Table &Scope::getTable() const {
  return requite::getRef(this->_table_ptr);
  REQUITE_ASSERT(this->getType() == requite::ScopeType::TABLE);
}

void Scope::setProcedure(requite::Procedure &procedure) {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::PROCEDURE);
  requite::setSingleRef(this->_procedure_ptr, procedure);
}

bool Scope::getHasProcedure() const {
  REQUITE_ASSERT(this->getType() == requite::ScopeType::PROCEDURE);
  return this->_procedure_ptr != nullptr;
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
  REQUITE_ASSERT(this->getType() == requite::ScopeType::ANONYMOUS_FUNCTION);
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

bool Scope::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Scope::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Scope::replaceExpression(requite::Expression &expression) {
  requite::Expression &old_expression = this->getExpression();
  this->_expression_ptr = &expression;
  return old_expression;
}

requite::Expression &Scope::popExpression() {
  requite::Expression &old_expression = this->getExpression();
  this->_expression_ptr = nullptr;
  return old_expression;
}

requite::Expression &Scope::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Scope::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

bool Scope::getHasNext() const { return this->_next_ptr != nullptr; }

void Scope::setNext(requite::Scope &scope) {
  requite::setSingleRef(this->_next_ptr, scope);
}

requite::Scope &Scope::getNext() { return requite::getRef(this->_next_ptr); }

const requite::Scope &Scope::getNext() const {
  return requite::getRef(this->_next_ptr);
}

requite::Scope *Scope::getNextPtr() { return this->_next_ptr; }

const requite::Scope *Scope::getNextPtr() const { return this->_next_ptr; }

} // namespace requite