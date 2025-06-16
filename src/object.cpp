// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/object.hpp>

namespace requite {

Object::Object() {
  this->getScope().setType(requite::ScopeType::OBJECT);
  this->getScope().setObject(*this);
}

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
  return this->getScope().getHasExpression();
}
void Object::setExpression(requite::Expression &expression) {
  this->getScope().setExpression(expression);
}

requite::Expression &Object::getExpression() {
  return this->getScope().getExpression();
}

const requite::Expression &Object::getExpression() const {
  return this->getScope().getExpression();
}

requite::Scope &Object::getScope() { return this->_scope; }

const requite::Scope &Object::getScope() const { return this->_scope; }

bool Object::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void Object::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}

requite::Scope &Object::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &Object::getContaining() const {
  return this->getScope().getContaining();
}

void Object::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
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