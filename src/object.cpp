// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/object.hpp>

namespace requite {

bool Object::getHasName() const { return this->getTable().getHasName(); }

void Object::setName(llvm::StringRef name) { this->getTable().setName(name); }

llvm::StringRef Object::getName() const { return this->getTable().getName(); }

bool Object::getHasExpression() const {
  return this->getTable().getScope().getHasExpression();
}
void Object::setExpression(requite::Expression &expression) {
  this->getTable().getScope().setExpression(expression);
}

requite::Expression &Object::getExpression() {
  return this->getTable().getScope().getExpression();
}

const requite::Expression &Object::getExpression() const {
  return this->getTable().getScope().getExpression();
}

requite::Table &Object::getTable() { return this->_table; }

const requite::Table &Object::getTable() const { return this->_table; }

requite::Scope &Object::getScope() { return this->getTable().getScope(); }

const requite::Scope &Object::getScope() const {
  return this->getTable().getScope();
}

bool Object::getHasContainingScope() const {
  return this->getTable().getScope().getHasContainingScope();
}

void Object::setContainingScope(requite::Scope &scope) {
  this->getTable().getScope().setContainingScope(scope);
}

requite::Scope &Object::getContainingScope() {
  return this->getTable().getScope().getContainingScope();
}

const requite::Scope &Object::getContainingScope() const {
  return this->getTable().getScope().getContainingScope();
}

void Object::setMangledName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_mangled_name.empty());
  this->_mangled_name = name;
}

llvm::StringRef Object::getMangledName() const { return this->_mangled_name; }

bool Object::getHasMangledName() const { return !this->_mangled_name.empty(); }

void Object::setDestructor(requite::Procedure &destructor) {
  requite::setSingleRef(this->_destructor_ptr, destructor);
}

bool Object::getHasDestructor() const {
  return this->_destructor_ptr != nullptr;
}

requite::Procedure &Object::getDestructor() {
  return requite::getRef(this->_destructor_ptr);
}

const requite::Procedure &Object::getDestructor() const {
  return requite::getRef(this->_destructor_ptr);
}

void Object::addConstructor(requite::Procedure &constructor) {
  constructor._next_procedure_ptr = this->_first_constructor_ptr;
  this->_first_constructor_ptr = &constructor;
}

bool Object::getHasConstructor() const {
  return this->_first_constructor_ptr != nullptr;
}

requite::Procedure &Object::getFirstConstructor() {
  return requite::getRef(this->_first_constructor_ptr);
}

const requite::Procedure &Object::getFirstConstructor() const {
  return requite::getRef(this->_first_constructor_ptr);
}

void Object::setAscribe(requite::Expression &ascribe) {
  requite::setSingleRef(this->_ascribe_ptr, ascribe);
}

bool Object::getHasAscribe() const { return this->_ascribe_ptr != nullptr; }

requite::Expression &Object::getAscribe() {
  return requite::getRef(this->_ascribe_ptr);
}

const requite::Expression &Object::getAscribe() const {
  return requite::getRef(this->_ascribe_ptr);
}

void Object::setModule(requite::Module &module) {
  requite::setSingleRef(this->_module_ptr, module);
}

bool Object::getHasModule() const { return this->_module_ptr != nullptr; }

requite::Module &Object::getModule() {
  return requite::getRef(this->_module_ptr);
}

const requite::Module &Object::getModule() const {
  return requite::getRef(this->_module_ptr);
}

requite::Attributes &Object::getAttributes() { return this->_attributes; }

} // namespace requite