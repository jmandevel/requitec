// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/module.hpp>
#include <requite/options.hpp>
#include <requite/token.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/FileSystem.h>

#include <vector>

namespace requite {

bool Context::run() {
  bool is_ok = true;
  if (!this->initializeLlvmTarget()) {
    return false;
  }
  if (!this->determineDirectoryPaths()) {
    return false;
  }
  this->createOpcodeTable();
  this->startScheduler();
  this->_module_uptrs.reserve(
      requite::options::MODULE_FILES.getNumOccurrences());
  for (llvm::StringRef module_path : requite::options::MODULE_FILES) {
    std::unique_ptr<requite::Module> &module_uptr =
        this->_module_uptrs.emplace_back();
    module_uptr = std::make_unique<requite::Module>();
    requite::Module &module = requite::getRef(module_uptr);
    if (!this->loadFileBuffer(module.getFile(), module_path)) {
      is_ok = false;
    }
  }
  if (is_ok == false) {
    return false;
  }
  std::atomic<bool> is_ok_a = true;
  std::atomic<bool> is_ready_a = true;
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->scheduleTask([this, &is_ok_a, &is_ready_a, &module]() {
      std::vector<requite::Token> tokens;
      if (!this->validateSourceFileText(module.getFile())) {
        is_ok_a.store(false);
        return;
      }
      const bool tokens_ok = this->tokenizeTokens(module, tokens);
      if (requite::options::INTERMEDIATE_TOKEN_CSV.getValue()) {
        this->writeTokenCsv(module, tokens);
      }
      if (!tokens_ok) {
        is_ok_a.store(false);
        return;
      }
      if (!this->parseAst(module, tokens)) {
        is_ok_a.store(false);
        return;
      }
      if (requite::options::INTERMEDIATE_PARSED_AST.getValue()) {
        this->writeAst(module, ".parsed");
      }
      if (!this->situateAst(module)) {
        is_ok_a.store(false);
        return;
      }
      if (requite::options::INTERMEDIATE_SITUATED_AST.getValue()) {
        this->writeAst(module, ".situated");
      }
    });
  }
  this->initializeLlvmContext();
  if (!this->setupModuleNames()) {
    return false;
  }
  // find all symbols and allocate data structs for each
  if (!this->makeUserSymbols())
  {
    return false;
  }
  // loop over symbols over and over until all are named or its impossible to
  // continue
  if (!this->nameUserSymbols()) {
    return false;
  }
  // loop over symbols over and over until all are resolved or its impossible to
  // continue
  if (!this->resolveUserSymbols()) {
    return false;
  }
  // loop over symbols over and over until all are built or its impossible to
  // continue
  if (!this->buildUserSymbols()) {
    return false;
  }
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    if (!this->buildIr(module)) {
      is_ok = false;
      continue;
    }
    if (requite::options::INTERMEDIATE_LLVM_IR.getValue()) {
      this->writeLlvmIr(module);
    }
    if (!this->compileObject(module)) {
      is_ok = false;
    }
  }
  return is_ok;
}

} // namespace requite