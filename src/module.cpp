// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/module.hpp>

namespace requite {

bool Module::operator==(const Self &rhs) const { return this == &rhs; }

bool Module::operator!=(const Self &rhs) const { return this != &rhs; }

bool Module::getHasName() const { return this->getTable().getHasName(); }

void Module::setName(llvm::StringRef name) { this->getTable().setName(name); }

llvm::StringRef Module::getName() const { return this->getTable().getName(); }

requite::Table &Module::getTable() { return this->_table; }

const requite::Table &Module::getTable() const { return this->_table; }

requite::Scope &Module::getScope() { return this->getTable().getScope(); }

const requite::Scope &Module::getScope() const {
  return this->getTable().getScope();
}

requite::File &Module::getFile() { return this->_file; }

const requite::File &Module::getFile() const { return this->_file; }

bool Module::getHasEntryPoint() const {
  return this->_entry_point_ptr != nullptr;
}

void Module::setEntryPoint(requite::Procedure &procedure) {
  requite::setSingleRef(this->_entry_point_ptr, procedure);
}

requite::Procedure &Module::getEntryPoint() {
  return requite::getRef(this->_entry_point_ptr);
}

const requite::Procedure &Module::getEntryPoint() const {
  return requite::getRef(this->_entry_point_ptr);
}

bool Module::getHasExpression() const {
  return this->getTable().getScope().getHasExpression();
}

void Module::setExpression(requite::Expression &expression) {
  this->getTable().getScope().setExpression(expression);
}

requite::Expression &
Module::replaceExpression(requite::Expression &expression) {
  return this->getTable().getScope().replaceExpression(expression);
}

requite::Expression &Module::popExpression() {
  return this->getTable().getScope().popExpression();
}

requite::Expression &Module::getExpression() {
  return this->getTable().getScope().getExpression();
}

const requite::Expression &Module::getExpression() const {
  return this->getTable().getScope().getExpression();
}

llvm::StringRef Module::getPath() const { return this->getFile().getPath(); }

llvm::StringRef Module::getIdentifier() const {
  return this->getFile().getIdentifier();
}

llvm::StringRef Module::getText() const { return this->getFile().getText(); }

const char *Module::getTextPtr() const { return this->getFile().getTextPtr(); }

std::uint_fast32_t Module::getBufferI() const {
  return this->getFile().getBufferI();
}

} // namespace requite