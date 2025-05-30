// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/module.hpp>

namespace requite {

Module::Module() { this->_scope.setType(requite::ScopeType::MODULE); }

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
  return this->getScope().getHasExpression();
}

void Module::setExpression(requite::Expression &expression) {
  this->getScope().setExpression(expression);
}

requite::Expression &
Module::replaceExpression(requite::Expression &expression) {
  return this->getScope().replaceExpression(expression);
}

requite::Expression &Module::popExpression() {
  return this->getScope().popExpression();
}

requite::Expression &Module::getExpression() {
  return this->getScope().getExpression();
}

const requite::Expression &Module::getExpression() const {
  return this->getScope().getExpression();
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