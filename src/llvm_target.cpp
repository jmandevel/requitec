// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>

#include <llvm/ADT/Twine.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>

namespace requite {

bool Context::initializeLlvm() {
  this->initializeLlvmContext();
  this->initializeLlvmBuilder();
  if (!this->initializeLlvmTarget()) {
    return false;
  }
  this->initializeLlvmModule();
  return true;
}

void Context::initializeLlvmBuilder() {
  this->_llvm_builder_uptr = std::make_unique<llvm::IRBuilder<>>(
      requite::getRef(this->_llvm_context_uptr));
}

void Context::initializeLlvmContext() {
  this->_llvm_context_uptr = std::make_unique<llvm::LLVMContext>();
}

bool Context::getIsLlvmContextInitialized() const {
  return this->_llvm_context_uptr.get() != nullptr;
}

void Context::terminateLlvmContext() { this->_llvm_context_uptr.reset(); }

llvm::LLVMContext &Context::getLlvmContext() {
  REQUITE_ASSERT(this->getIsLlvmContextInitialized());
  return requite::getRef(this->_llvm_context_uptr);
}

const llvm::LLVMContext &Context::getLlvmContext() const {
  REQUITE_ASSERT(this->getIsLlvmContextInitialized());
  return requite::getRef(this->_llvm_context_uptr);
}

bool Context::initializeLlvmTarget() {
  bool is_ok = true;
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmParser();
  llvm::InitializeNativeTargetAsmPrinter();
  this->_target_triple = llvm::sys::getDefaultTargetTriple();
  std::string error;
  this->_llvm_target_ptr =
      llvm::TargetRegistry::lookupTarget(this->_target_triple, error);
  if (this->_llvm_target_ptr == nullptr) {
    this->logMessage(
        llvm::Twine("error: failed to find llvm target.\n\ttriple: ") +
        llvm::Twine(this->_target_triple.c_str()) + llvm::Twine("\n\terror: ") +
        llvm::Twine(error.c_str()));
    is_ok = false;
  }
  this->_llvm_target_machine_ptr = this->_llvm_target_ptr->createTargetMachine(
      this->_target_triple, "generic", "", this->_llvm_options,
      llvm::Reloc::PIC_);
  this->_llvm_data_layout_uptr = std::make_unique<llvm::DataLayout>(
      this->_llvm_target_machine_ptr->createDataLayout());
  return is_ok;
}

const llvm::Target &Context::getLlvmTarget() const {
  return requite::getRef(this->_llvm_target_ptr);
}

llvm::TargetOptions &Context::getLlvmTargetOptions() {
  REQUITE_ASSERT(this->_llvm_target_ptr != nullptr);
  return this->_llvm_options;
}

const llvm::TargetOptions &Context::getLlvmTargetOptions() const {
  REQUITE_ASSERT(this->_llvm_target_ptr != nullptr);
  return this->_llvm_options;
}

llvm::TargetMachine &Context::getLlvmTargetMachine() {
  return requite::getRef(this->_llvm_target_machine_ptr);
}

const llvm::TargetMachine &Context::getLlvmTargetMachine() const {
  return requite::getRef(this->_llvm_target_machine_ptr);
}

const llvm::DataLayout &Context::getLlvmDataLayout() const {
  return requite::getRef(this->_llvm_data_layout_uptr);
}

llvm::StringRef Context::getLlvmTargetTriple() const {
  REQUITE_ASSERT(this->_llvm_target_ptr != nullptr);
  return this->_target_triple;
}

llvm::IRBuilder<> &Context::getLlvmBuilder() {
  return requite::getRef(this->_llvm_builder_uptr);
}

const llvm::IRBuilder<> &Context::getLlvmBuilder() const {
  return requite::getRef(this->_llvm_builder_uptr);
}

bool Context::getIsLittleEndian() const {
  return this->getLlvmDataLayout().isLittleEndian();
}

bool Context::getIsBigEndian() const {
  return this->getLlvmDataLayout().isBigEndian();
}

bool Context::getIsLegalIntegerDepth(unsigned depth) const {
  return this->getLlvmDataLayout().isLegalInteger(depth);
}

bool Context::getFitsInLegalIntegerDepth(unsigned depth) const {
  return this->getLlvmDataLayout().fitsInLegalInteger(depth);
}

unsigned Context::getAddressDepth() const {
  return this->getLlvmDataLayout().getPointerSizeInBits();
}

unsigned Context::getAddressSize() const {
  return this->getLlvmDataLayout().getPointerSize();
}

unsigned Context::getLargestLegalIntegerDepth() const {
  return this->getLlvmDataLayout().getLargestLegalIntTypeSizeInBits();
}

unsigned Context::getMaxIndexDepth() const {
  return this->getLlvmDataLayout().getIndexSizeInBits(0);
}

unsigned Context::getIndexDepth() const {
  return this->getLlvmDataLayout().getIndexSizeInBits(0);
}

} // namespace requite