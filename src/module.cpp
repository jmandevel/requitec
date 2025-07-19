// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/module.hpp>
#include <requite/procedure.hpp>

namespace requite {

bool Module::operator==(const Self &rhs) const { return this == &rhs; }

bool Module::operator!=(const Self &rhs) const { return this != &rhs; }

llvm::StringRef Module::getName() const {
  llvm::StringRef path = this->getPath();
  std::size_t last_dot_pos = path.rfind('.');
  if (last_dot_pos == path.npos) {
    return path;
  }
  return path.take_front(last_dot_pos);
}

requite::File &Module::getFile() { return this->_file; }

const requite::File &Module::getFile() const { return this->_file; }

bool Module::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Module::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

void Module::changeExpression(requite::Expression &expression) {
  REQUITE_ASSERT(this->_expression_ptr != nullptr);
  this->_expression_ptr = &expression;
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
  
std::set<requite::Module*> &Module::getImportModulePtrSet() {
  return this->_import_module_ptr_set;
}

const std::set<requite::Module*> &Module::getImportModulePtrSet() const {
  return this->_import_module_ptr_set;
}

std::vector<requite::Module*> &Module::getExportTargetModulePtrs() {
  return this->_export_target_module_ptrs;
}

const std::vector<requite::Module*> &Module::getExportTargetModulePtrs() const {
  return this->_export_target_module_ptrs;
}


} // namespace requite