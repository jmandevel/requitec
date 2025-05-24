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

llvm::StringMap<requite::RootSymbol> &Scope::getSymbolMap() {
  return this->_symbol_map;
}

const llvm::StringMap<requite::RootSymbol> &Scope::getSymbolMap() const {
  return this->_symbol_map;
}

bool Scope::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void Scope::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &Scope::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &Scope::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

std::vector<requite::Node> &Scope::getNodes() { return this->_nodes; }

const std::vector<requite::Node> &Scope::getNodes() const {
  return this->_nodes;
}

bool Scope::getIsEmpty() const {
  return this->_symbol_map.empty() && this->_nodes.empty();
}

bool Scope::getHasObject() const { return this->_object_ptr != nullptr; }

void Scope::setObject(requite::Object &object) {
  requite::setSingleRef(this->_object_ptr, object);
}

requite::Object &Scope::getObject() {
  return requite::getRef(this->_object_ptr);
}

const requite::Object &Scope::getObject() const {
  return requite::getRef(this->_object_ptr);
}

requite::Object *Scope::getObjectPtr() { return this->_object_ptr; }

const requite::Object *Scope::getObjectPtr() const { return this->_object_ptr; }

void Scope::setProcedure(requite::Procedure &procedure) {
  requite::setSingleRef(this->_procedure_ptr, procedure);
}

bool Scope::getHasProcedure() const { return this->_procedure_ptr != nullptr; }

requite::Procedure &Scope::getProcedure() {
  return requite::getRef(this->_procedure_ptr);
}

const requite::Procedure &Scope::getProcedure() const {
  return requite::getRef(this->_procedure_ptr);
}

requite::Procedure *Scope::getProcedurePtr() { return this->_procedure_ptr; }

const requite::Procedure *Scope::getProcedurePtr() const {
  return this->_procedure_ptr;
}

bool Scope::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Scope::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Scope::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Scope::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

} // namespace requite