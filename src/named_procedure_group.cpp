// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/module.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/object.hpp>

namespace requite {

bool NamedProcedureGroup::operator==(const Self &rhs) const {
  return this == &rhs;
}

bool NamedProcedureGroup::operator!=(const Self &rhs) const {
  return this != &rhs;
}

llvm::StringRef NamedProcedureGroup::getName() const { return this->_name; }

void NamedProcedureGroup::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

bool NamedProcedureGroup::getHasName() const { return !this->_name.empty(); }

bool NamedProcedureGroup::getHasProcedures() const {
  return this->_first_ptr != nullptr;
}

requite::Procedure &NamedProcedureGroup::getFirstProcedure() {
  return requite::getRef(this->_first_ptr);
}

const requite::Procedure &NamedProcedureGroup::getFirstProcedure() const {
  return requite::getRef(this->_first_ptr);
}

void NamedProcedureGroup::addProcedure(requite::Procedure &procedure) {
  REQUITE_ASSERT(procedure._next_ptr == nullptr);
  procedure._next_ptr = this->_first_ptr;
  this->_first_ptr = &procedure;
}

bool NamedProcedureGroup::getHasContainingScope() const {
  return this->_containing_scope_ptr != nullptr;
}

void NamedProcedureGroup::setContainingScope(requite::Scope &scope) {
  requite::setSingleRef(this->_containing_scope_ptr, scope);
}

requite::Scope &NamedProcedureGroup::getContainingScope() {
  return requite::getRef(this->_containing_scope_ptr);
}

const requite::Scope &NamedProcedureGroup::getContainingScope() const {
  return requite::getRef(this->_containing_scope_ptr);
}

} // namespace requite