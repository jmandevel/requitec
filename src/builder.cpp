// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/builder.hpp>

namespace requite {

Builder::Builder(requite::Context &context, requite::Module &module)
    : _context_ref(context), _module_ref(module) {}

requite::Context &Builder::getContext() { return this->_context_ref.get(); }

const requite::Context &Builder::getContext() const {
  return this->_context_ref.get();
}

requite::Module &Builder::getModule() { return this->_module_ref.get(); }

const requite::Module &Builder::getModule() const {
  return this->_module_ref.get();
}

bool Builder::getHasCurrentLlvmBlock() const {
  return this->_current_llvm_block_ptr != nullptr;
}

void Builder::setCurrentLlvmBlock(llvm::BasicBlock &block) {
  requite::setSingleRef(this->_current_llvm_block_ptr, block);
}

void Builder::changeCurrentLlvmBlock(llvm::BasicBlock &block) {
  REQUITE_ASSERT(this->getHasCurrentLlvmBlock());
  this->_current_llvm_block_ptr = &block;
}

void Builder::clearCurrentLlvmBlock() {
  this->_current_llvm_block_ptr = nullptr;
}

llvm::BasicBlock &Builder::getCurrentLlvmBlock() {
  return requite::getRef(this->_current_llvm_block_ptr);
}

const llvm::BasicBlock &Builder::getCurrentLlvmBlock() const {
  return requite::getRef(this->_current_llvm_block_ptr);
}

bool Builder::getIsOk() const { return this->_is_ok; }

void Builder::setNotOk() { this->_is_ok = false; }

} // namespace requite