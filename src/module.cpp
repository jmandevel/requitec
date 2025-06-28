// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/module.hpp>
#include <requite/procedure.hpp>

namespace requite {

Module::Module() { this->getScope().setModule(*this); }

bool Module::operator==(const Self &rhs) const { return this == &rhs; }

bool Module::operator!=(const Self &rhs) const { return this != &rhs; }

bool Module::getHasName() const { return !this->_name.empty(); }

void Module::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

llvm::StringRef Module::getName() const {
  REQUITE_ASSERT(this->getHasName());
  return this->_name;
}

requite::Scope &Module::getScope() { return this->_scope; }

const requite::Scope &Module::getScope() const { return this->_scope; }

requite::File &Module::getFile() { return this->_file; }

const requite::File &Module::getFile() const { return this->_file; }

bool Module::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Module::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &
Module::replaceExpression(requite::Expression &expression) {
  requite::Expression *old_expression_ptr = this->_expression_ptr;
  this->_expression_ptr = &expression;
  return requite::getRef(old_expression_ptr);
}

requite::Expression &Module::popExpression() {
  requite::Expression *old_expression_ptr = this->_expression_ptr;
  this->_expression_ptr = nullptr;
  return requite::getRef(old_expression_ptr);
}

requite::Expression &Module::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Module::getExpression() const {
  return requite::getRef(this->_expression_ptr);
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

bool Module::getHasEntryPoint() const {
  return this->_entry_point_ptr != nullptr;
}

void Module::addEntryPoint(requite::Procedure &entry_point) {
  entry_point.setContaining(this->getScope());
  if (this->getHasEntryPoint()) {
    entry_point.setNextProcedure(this->getEntryPoint());
    this->_entry_point_ptr = &entry_point;
  }
  this->_entry_point_ptr = &entry_point;
}

requite::Procedure &Module::getEntryPoint() {
  return requite::getRef(this->_entry_point_ptr);
}

const requite::Procedure &Module::getEntryPoint() const {
  return requite::getRef(this->_entry_point_ptr);
}

} // namespace requite