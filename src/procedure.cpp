// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/procedure.hpp>

namespace requite {

void Procedure::setMangledName(llvm::StringRef name) {
  REQUITE_ASSERT(this->_mangled_name.empty());
  this->_mangled_name = name;
}

bool Procedure::getHasMangledName() const {
  return !this->_mangled_name.empty();
}

llvm::StringRef Procedure::getMangledName() const { return this->_mangled_name; }

void Procedure::setType(requite::ProcedureType type)
{
  REQUITE_ASSERT(this->_type == requite::ProcedureType::NONE);
  REQUITE_ASSERT(type != requite::ProcedureType::NONE);
  this->_type = type;
}

requite::ProcedureType Procedure::getType() const
{
  return this->_type;
}

requite::Signature &Procedure::getSignature() { return this->_signature; }

const requite::Signature &Procedure::getSignature() const {
  return this->_signature;
}

requite::Scope &Procedure::getScope() { return this->_scope; }

const requite::Scope &Procedure::getScope() const { return this->_scope; }

requite::Attributes &Procedure::getAttributes() { return this->_attributes; }

const requite::Attributes &Procedure::getAttributes() const {
  return this->_attributes;
}

void Procedure::setAscribe(requite::Expression &ascribe) {
  requite::setSingleRef(this->_ascribe_ptr, ascribe);
}

bool Procedure::getHasAscribe() const { return this->_ascribe_ptr != nullptr; }

requite::Expression &Procedure::getAscribe() {
  return requite::getRef(this->_ascribe_ptr);
}

const requite::Expression &Procedure::getAscribe() const {
  return requite::getRef(this->_ascribe_ptr);
}

void Procedure::setNamedProcedureGroup(requite::NamedProcedureGroup &named_procedure_group) {
  requite::setSingleRef(this->_named_procedure_group_ptr, named_procedure_group);
}

bool Procedure::getHasNamedProcedureGroup() const { return this->_named_procedure_group_ptr != nullptr; }

requite::NamedProcedureGroup &Procedure::getNamedProcedureGroup() {
  return requite::getRef(this->_named_procedure_group_ptr);
}

const requite::NamedProcedureGroup &Procedure::getNamedProcedureGroup() const {
  return requite::getRef(this->_named_procedure_group_ptr);
}

void Procedure::setNextProcedure(requite::Procedure &procedure) {
  requite::setSingleRef(this->_next_procedure_ptr, procedure);
}

bool Procedure::getHasNextProcedure() const {
  return this->_next_procedure_ptr != nullptr;
}

requite::Procedure &Procedure::getNextProcedure() {
  return requite::getRef(this->_next_procedure_ptr);
}

const requite::Procedure &Procedure::getNextProcedure() const {
  return requite::getRef(this->_next_procedure_ptr);
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