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
  bool is_ok;
  switch (requite::getEmitMode()) {
  case requite::EMIT_TOKENS:
    is_ok = this->emitTokens();
    break;
  case requite::EMIT_PARSED:
    is_ok = this->emitParsed();
    break;
  case requite::EMIT_SITUATED:
    is_ok = this->emitSituated();
    break;
  case requite::EMIT_EXPANDED:
    is_ok = this->emitExpanded();
    break;
  case requite::EMIT_IR:
    is_ok = this->emitIr();
    break;
  case requite::EMIT_ASSEMBLY:
    is_ok = this->emitAssembly();
    break;
  case requite::EMIT_OBJECT:
    is_ok = this->emitObject();
    break;
  default:
    this->logMessage("[error] invalid emit mode");
    is_ok = false;
  }
  return true;
}

bool Context::emitTokens() {
  requite::Module &source_module = this->getSourceModule();
  requite::File &source_file = source_module.getFile();
  llvm::StringRef input_path = requite::getInputFilePath();
  if (!this->loadFileBuffer(source_file, input_path)) {
    return false;
  }
  std::vector<requite::Token> tokens = {};
  if (!this->tokenizeTokens(this->getSourceModule(), tokens)) {
    return false;
  }
  llvm::StringRef out_path = requite::getOutputFilePath();
  if (!this->writeTokenCsv(source_module, tokens, out_path)) {
    return false;
  }
  return true;
}

bool Context::emitParsed() {
  requite::Module &source_module = this->getSourceModule();
  requite::File &source_file = source_module.getFile();
  llvm::StringRef input_path = requite::getInputFilePath();
  if (!this->loadFileBuffer(source_file, input_path)) {
    return false;
  }
  std::vector<requite::Token> tokens = {};
  if (!this->tokenizeTokens(this->getSourceModule(), tokens)) {
    return false;
  }
  this->createOpcodeTable();
  if (!this->parseAst(source_module, tokens)) {
    return false;
  }
  llvm::StringRef out_path = requite::getOutputFilePath();
  if (!this->writeAst(source_module, out_path)) {
    return false;
  }
  return true;
}

bool Context::emitSituated() {
  requite::Module &source_module = this->getSourceModule();
  requite::File &source_file = source_module.getFile();
  llvm::StringRef input_path = requite::getInputFilePath();
  if (!this->loadFileBuffer(source_file, input_path)) {
    return false;
  }
  std::vector<requite::Token> tokens = {};
  if (!this->tokenizeTokens(this->getSourceModule(), tokens)) {
    return false;
  }
  this->createOpcodeTable();
  if (!this->parseAst(source_module, tokens)) {
    return false;
  }
  if (!this->situateAst(source_module)) {
    return false;
  }
  llvm::StringRef out_path = requite::getOutputFilePath();
  if (!this->writeAst(source_module, out_path)) {
    return false;
  }
  return true;
}

bool Context::emitExpanded() { return false; }

bool Context::emitIr() { return false; }

bool Context::emitAssembly() { return false; }

bool Context::emitObject() { return false; }

} // namespace requite