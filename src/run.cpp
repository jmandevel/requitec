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
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->scheduleTask([this, &is_ok_a, &module]() {
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
      if (!this->makeUserSymbols(module)) {
        is_ok_a.store(false);
        return;
      }
      if (!this->determineModuleName(module)) {
        is_ok_a.store(false);
        return;
      }
    });
  }
  this->waitForTasks();
  if (is_ok_a == false) {
    return false;
  }
  if (!this->mapModules()) {
    return false;
  }
  this->initializeLlvm();
  if (!this->tabulateUserSymbols()) {
    return false;
  }
  if (!this->prototypeUserSymbols()) {
    return false;
  }
  if (!this->buildUserSymbols()) {
    return false;
  }
  if (requite::options::INTERMEDIATE_LLVM_IR.getValue()) {
    this->writeLlvmIr();
  }
  if (!this->compileObjectFiles()) {
    return false;
  }
  return true;
}

} // namespace requite