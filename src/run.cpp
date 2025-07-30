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
  llvm::SmallString<128> input_path = requite::getInputFilePath();
  llvm::StringRef output_path = requite::getOutputFilePath();
  std::error_code ec = llvm::sys::fs::make_absolute(input_path);
  if (ec) {
    this->logMessage(
        llvm::Twine("failed to determine absolute input file path\n\treason: ") +
        ec.message());
    return false;
  }
  ec = llvm::sys::fs::real_path(input_path, input_path, false);
  if (ec) {
    this->logMessage(
        llvm::Twine("failed to determine real input file path\n\treason: ") +
        ec.message() + "\n");
    return false;
  }
  if (!this->loadFileBuffer(source_file, input_path)) {
    return false;
  }
  this->_module_map.insert(std::pair<llvm::StringRef, requite::Module *>(
      input_path, &source_module));
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
  if (!this->initializeLlvm()) {
    return false;
  }
  if (!this->tabulateModule(source_module)) {
    return false;
  }
  this->propogateExportedImports();
  this->diffuseUses();
  if (requite::getEmitMode() == requite::EMIT_SYMBOLS) {
    if (!this->writeUserSymbols(output_path)) {
      return false;
    }
    return true;
  }
  if (!this->implementAll()) {
    return false;
  }
  if (requite::getEmitMode() == requite::EMIT_IMPLEMENTED) {
    if (!this->writeAst(source_module, output_path)) {
      return false;
    }
    return true;
  }

  if (!this->checkEntryPointCount()) {
    return false;
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