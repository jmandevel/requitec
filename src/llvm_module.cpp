// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/module.hpp>

namespace requite {

void Module::initializeLlvmModule(requite::Context &context) {
  this->_llvm_module_uptr = std::make_unique<llvm::Module>(
      this->getTable().getName(), context.getLlvmContext());
  this->_llvm_module_uptr->setSourceFileName(this->getSource().getPath());
  this->_llvm_module_uptr->setDataLayout(context.getLlvmDataLayout());
  this->_llvm_module_uptr->setTargetTriple(context.getLlvmTargetTriple());
}

bool Module::getIsLlvmModuleInitialized() const {
  return this->_llvm_module_uptr.get() != nullptr;
}

void Module::terminateLlvmModule() { this->_llvm_module_uptr.reset(); }

llvm::Module &Module::getLlvmModule() {
  return requite::getRef(this->_llvm_module_uptr);
}

const llvm::Module &Module::getLlvmModule() const {
  return requite::getRef(this->_llvm_module_uptr);
}

std::string Module::getLlvmIrSourceText() const {
  std::string ir_text;
  llvm::raw_string_ostream ir_stream(ir_text);
  this->getLlvmModule().print(ir_stream, nullptr);
  ir_stream.flush();
  return ir_text;
}

} // namespace requite