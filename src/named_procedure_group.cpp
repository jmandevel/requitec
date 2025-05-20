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

void NamedProcedureGroup::setType(requite::ProcedureType type) {
  REQUITE_ASSERT(this->_type == requite::ProcedureType::NONE);
  this->_type = type;
}

requite::ProcedureType NamedProcedureGroup::getType() const {
  return this->_type;
}

llvm::StringRef NamedProcedureGroup::getName() const { return this->_name; }

void NamedProcedureGroup::setName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_name.empty());
  this->_name = name;
}

bool NamedProcedureGroup::getHasName() const { return !this->_name.empty(); }

bool NamedProcedureGroup::getHasProcedures() const {
  return this->_first_procedure_ptr != nullptr;
}

requite::Procedure &NamedProcedureGroup::getFirstProcedure() {
  return requite::getRef(this->_first_procedure_ptr);
}

const requite::Procedure &NamedProcedureGroup::getFirstProcedure() const {
  return requite::getRef(this->_first_procedure_ptr);
}

void NamedProcedureGroup::setModule(requite::Module &module) {
  requite::setSingleRef(this->_module_ptr, module);
}

bool NamedProcedureGroup::getHasModule() const {
  return this->_module_ptr != nullptr;
}

requite::Module &NamedProcedureGroup::getModule() {
  return requite::getRef(this->_module_ptr);
}

const requite::Module &NamedProcedureGroup::getModule() const {
  return requite::getRef(this->_module_ptr);
}

void NamedProcedureGroup::setAsExported() {
  REQUITE_ASSERT(!this->_is_exported);
  this->_is_exported = true;
}

bool NamedProcedureGroup::getIsExported() const { return this->_is_exported; }

void NamedProcedureGroup::addProcedure(requite::Procedure &procedure) {
  procedure._next_procedure_ptr = this->_first_procedure_ptr;
  this->_first_procedure_ptr = &procedure;
}

bool NamedProcedureGroup::getHasObject() const {
  return this->_object_ptr != nullptr;
}

requite::Object &NamedProcedureGroup::getObject() {
  return requite::getRef(this->_object_ptr);
}

const requite::Object &NamedProcedureGroup::getObject() const {
  return requite::getRef(this->_object_ptr);
}

requite::Object *NamedProcedureGroup::getObjectPtr() {
  return this->_object_ptr;
}

const requite::Object *NamedProcedureGroup::getObjectPtr() const {
  return this->_object_ptr;
}

} // namespace requite