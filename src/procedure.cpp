// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/named_procedure_group.hpp>
#include <requite/procedure.hpp>

namespace requite {

Procedure::Procedure() { this->getScope().setProcedure(*this); }

void Procedure::setMangledName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_mangled_name.empty());
  this->_mangled_name = name;
}

bool Procedure::getHasMangledName() const {
  return !this->_mangled_name.empty();
}

llvm::StringRef Procedure::getMangledName() const {
  return this->_mangled_name;
}

bool Procedure::getHasContaining() const {
  return this->getScope().getHasContaining();
}

void Procedure::setContaining(requite::Scope &scope) {
  this->getScope().setContaining(scope);
}
requite::Scope &Procedure::getContaining() {
  return this->getScope().getContaining();
}

const requite::Scope &Procedure::getContaining() const {
  return this->getScope().getContaining();
}

bool Procedure::getIsNamed() const {
  return requite::getIsNamed(this->getType());
}

bool Procedure::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

void Procedure::setExpression(requite::Expression &expression) {
  requite::setSingleRef(this->_expression_ptr, expression);
}

requite::Expression &Procedure::getExpression() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &Procedure::getExpression() const {
  return requite::getRef(this->_expression_ptr);
}

void Procedure::setType(requite::ProcedureType type) {
  REQUITE_ASSERT(this->_type == requite::ProcedureType::NONE);
  REQUITE_ASSERT(type != requite::ProcedureType::NONE);
  this->_type = type;
}

requite::ProcedureType Procedure::getType() const { return this->_type; }

requite::Signature &Procedure::getSignature() { return this->_signature; }

const requite::Signature &Procedure::getSignature() const {
  return this->_signature;
}

requite::Scope &Procedure::getScope() { return this->_scope; }

const requite::Scope &Procedure::getScope() const { return this->_scope; }

void Procedure::setAttributeFlags(requite::AttributeFlags attributes) {
  this->_attributes = attributes;
}

requite::AttributeFlags &Procedure::getAttributeFlags() {
  return this->_attributes;
}

const requite::AttributeFlags &Procedure::getAttributeFlags() const {
  return this->_attributes;
}

void Procedure::setNamedProcedureGroup(requite::NamedProcedureGroup &group) {
  REQUITE_ASSERT(group.getContaining() == this->getContaining());
  requite::setSingleRef(this->_group_ptr, group);
  this->_next_ptr = group._first_ptr;
  group._first_ptr = this;
}

bool Procedure::getHasNamedProcedureGroup() const {
  return this->_group_ptr != nullptr;
}

requite::NamedProcedureGroup &Procedure::getNamedProcedureGroup() {
  return requite::getRef(this->_group_ptr);
}

const requite::NamedProcedureGroup &Procedure::getNamedProcedureGroup() const {
  return requite::getRef(this->_group_ptr);
}

void Procedure::setNextProcedure(requite::Procedure &procedure) {
  requite::setSingleRef(this->_next_ptr, procedure);
}

bool Procedure::getHasNextProcedure() const {
  return this->_next_ptr != nullptr;
}

requite::Procedure &Procedure::getNextProcedure() {
  return requite::getRef(this->_next_ptr);
}

const requite::Procedure &Procedure::getNextProcedure() const {
  return requite::getRef(this->_next_ptr);
}

bool Procedure::getHasModule() const { return this->_module_ptr != nullptr; }

void Procedure::setModule(requite::Module &module) {
  requite::setSingleRef(this->_module_ptr, module);
}

requite::Module &Procedure::getModule() {
  return requite::getRef(this->_module_ptr);
}

const requite::Module &Procedure::getModule() const {
  return requite::getRef(this->_module_ptr);
}

bool Procedure::getHasLlvmFunctionType() const {
  return this->_llvm_function_type_ptr != nullptr;
}

void Procedure::setLlvmFunctionType(llvm::FunctionType &type) {
  requite::setSingleRef(this->_llvm_function_type_ptr, type);
}

llvm::FunctionType &Procedure::getLlvmFunctionType() {
  return requite::getRef(this->_llvm_function_type_ptr);
}

const llvm::FunctionType &Procedure::getLlvmFunctionType() const {
  return requite::getRef(this->_llvm_function_type_ptr);
}

bool Procedure::getHasLlvmFunction() const {
  return this->_llvm_function_ptr != nullptr;
}

void Procedure::setLlvmFunction(llvm::Function &function) {
  requite::setSingleRef(this->_llvm_function_ptr, function);
}

llvm::Function &Procedure::getLlvmFunction() {
  return requite::getRef(this->_llvm_function_ptr);
}

const llvm::Function &Procedure::getLlvmFunction() const {
  return requite::getRef(this->_llvm_function_ptr);
}

bool Procedure::getHasLlvmBlock() const {
  return this->_llvm_block_ptr != nullptr;
}

void Procedure::setLlvmBlock(llvm::BasicBlock &block) {
  requite::setSingleRef(this->_llvm_block_ptr, block);
}

llvm::BasicBlock &Procedure::getLlvmBlock() {
  return requite::getRef(this->_llvm_block_ptr);
}

const llvm::BasicBlock &Procedure::getLlvmBlock() const {
  return requite::getRef(this->_llvm_block_ptr);
}

} // namespace requite