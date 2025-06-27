// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>

namespace requite {

void Context::initializeLlvmModule() {
  requite::Module &source_module = this->getSourceModule();
  llvm::StringRef source_name = source_module.getName();
  llvm::LLVMContext &llvm_context = this->getLlvmContext();
  this->_llvm_module_uptr =
      std::make_unique<llvm::Module>(source_name, llvm_context);
  llvm::StringRef source_path = source_module.getFile().getPath();
  this->_llvm_module_uptr->setSourceFileName(source_path);
  this->_llvm_module_uptr->setDataLayout(this->getLlvmDataLayout());
  this->_llvm_module_uptr->setTargetTriple(this->getLlvmTargetTriple());
}

bool Context::getIsLlvmModuleInitialized() const {
  return this->_llvm_module_uptr.get() != nullptr;
}

void Context::terminateLlvmModule() { this->_llvm_module_uptr.reset(); }

llvm::Module &Context::getLlvmModule() {
  return requite::getRef(this->_llvm_module_uptr);
}

const llvm::Module &Context::getLlvmModule() const {
  return requite::getRef(this->_llvm_module_uptr);
}

std::string Context::getLlvmIrSourceText() const {
  std::string ir_text;
  llvm::raw_string_ostream ir_stream(ir_text);
  const llvm::Module& module = this->getLlvmModule();
  module.print(ir_stream, nullptr);
  ir_stream.flush();
  return ir_text;
}

} // namespace requite