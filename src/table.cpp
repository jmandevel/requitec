// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/table.hpp>

namespace requite {

Table::Table() { this->getScope().setTable(*this); }

bool Table::getHasName() const { return !this->_name.empty(); }

void Table::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

llvm::StringRef Table::getName() const { return this->_name; }

void Table::setHasDependentName() {
  REQUITE_ASSERT(this->_dependent_name == false);
  this->_dependent_name = true;
}

bool Table::getHasDependentName() const { return this->_dependent_name; }

requite::Scope &Table::getScope() { return this->_scope; }

const requite::Scope &Table::getScope() const { return this->_scope; }

bool Table::getHasObject() const { return this->getScope().getHasObject(); }

void Table::setObject(requite::Object &object) {
  this->getScope().setObject(object);
}

requite::Object &Table::getObject() { return this->getScope().getObject(); }

const requite::Object &Table::getObject() const {
  return this->getScope().getObject();
}

bool Table::getHasContainingScope() const {
  return this->getScope().getHasContainingScope();
}

void Table::setContainingScope(requite::Scope &scope) {
  this->getScope().setContainingScope(scope);
}

requite::Scope &Table::getContainingScope() {
  return this->getScope().getContainingScope();
}

const requite::Scope &Table::getContainingScope() const {
  return this->getScope().getContainingScope();
}

bool Table::getHasExpression() const {
  return this->getScope().getHasExpression();
}

void Table::setExpression(requite::Expression &expression) {
  this->getScope().setExpression(expression);
}

requite::Expression &Table::replaceExpression(requite::Expression &expression) {
  return this->getScope().replaceExpression(expression);
}

requite::Expression &Table::popExpression() {
  return this->getScope().popExpression();
}

requite::Expression &Table::getExpression() {
  return this->getScope().getExpression();
}

const requite::Expression &Table::getExpression() const {
  return this->getScope().getExpression();
}

} // namespace requite