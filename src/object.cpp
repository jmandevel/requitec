// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>

namespace requite {

Object::Object() {
  this->getScope().setObject(*this);
}

bool Object::operator==(const Self &rhs) const { return this == &rhs; }

bool Object::operator!=(const Self &rhs) const { return this != &rhs; }

bool Object::getHasName() const { return !this->_name.empty(); }

void Object::setName(llvm::StringRef name) {
  REQUITE_ASSERT(!this->getHasName());
  this->_name = name.str();
}

llvm::StringRef Object::getName() const {
  REQUITE_ASSERT(this->getHasName());
  return this->_name;
}

bool Object::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}
void Object::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Object::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Object::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Scope &Object::getScope() { return this->_scope; }

const requite::Scope &Object::getScope() const { return this->_scope; }

bool Object::getHasContainingScope() const {
  return this->getScope().getHasContainingScope();
}

void Object::setContainingScope(requite::Scope &scope) {
  this->getScope().setContainingScope(scope);
}

requite::Scope &Object::getContainingScope() {
  return this->getScope().getContainingScope();
}

const requite::Scope &Object::getContainingScope() const {
  return this->getScope().getContainingScope();
}

requite::AttributeFlags &Object::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &Object::getAttributeFlags() const {
  return this->_attributes;
}

void Object::setMangledName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_mangled_name.empty());
  this->_mangled_name = name;
}

llvm::StringRef Object::getMangledName() const { return this->_mangled_name; }

bool Object::getHasMangledName() const { return !this->_mangled_name.empty(); }

void Object::addDestructor(requite::Procedure &destructor) {
  // NOTE: objects in should have at most one destructor. if more are added, this
  // should eventually cause a compiler error.
  destructor._next_ptr = this->_first_destructor_ptr;
  this->_first_constructor_ptr = &destructor;
}

bool Object::getHasDestructor() const {
  return this->_first_destructor_ptr != nullptr;
}

bool Object::getHasMultipleDestructors() const {
  if (!this->getHasDestructor()) {
    return false;
  }
  const requite::Procedure &destructor = this->getDestructor();
  if (destructor.getHasNextProcedure()) {
    return true;
  }
  return false;
}

requite::Procedure &Object::getDestructor() {
  return requite::getRef(this->_first_destructor_ptr);
}

const requite::Procedure &Object::getDestructor() const {
  return requite::getRef(this->_first_destructor_ptr);
}

void Object::addConstructor(requite::Procedure &constructor) {
  constructor._next_ptr = this->_first_constructor_ptr;
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

} // namespace requite