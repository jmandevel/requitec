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
  requite::Module &source_module = this->getSourceModule();
  requite::File &source_file = source_module.getFile();
  llvm::StringRef input_path = requite::getInputFilePath();
  llvm::StringRef output_path = requite::getOutputFilePath();
  if (!this->loadFileBuffer(source_file, input_path)) {
    return false;
  }
  if (!this->validateSourceFileText(source_file)) {
    return false;
  }
  std::vector<requite::Token> tokens = {};
  if (!this->tokenizeTokens(this->getSourceModule(), tokens)) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_TOKENS) {
    if (!this->writeTokens(source_module, tokens, output_path)) {
      return false;
    }
    return true;
  }
  this->createOpcodeTable();
  if (!this->parseAst(source_module, tokens)) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_PARSED) {
    if (!this->writeAst(source_module, output_path)) {
      return false;
    }
    return true;
  }
  if (!this->situateAst(source_module)) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_SITUATED) {
    if (!this->writeAst(source_module, output_path)) {
      return false;
    }
    return true;
  }
  if (!this->determineModuleName(source_module)) {
    return false;
  }
  if (!this->contextualizeModule(source_module)) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_CONTEXTUALIZED) {
    if (!this->writeAst(source_module, output_path)) {
      return false;
    }
    return true;
  }
  if (requite::getEmitMode() == requite::EMIT_SYMBOLS) {
    if (!this->writeUserSymbols(output_path)) {
      return false;
    }
    return true;
  }
  if (!this->buildIr()) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_IR) {
    if (!this->writeLlvmIr(output_path)) {
      return false;
    }
    return true;
  }
  if (requite::getEmitMode() == requite::EMIT_ASSEMBLY) {
    if (!this->writeAssembly(output_path)) {
      return false;
    }
    return true;
  }
  if (requite::getEmitMode() == requite::EMIT_OBJECT) {
    if (!this->writeObject(output_path)) {
      return false;
    }
    return true;
  }
  return true;
}

} // namespace requite