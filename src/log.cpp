// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/token.hpp>

#include <llvm/Support/raw_ostream.h>

#include <mutex>

namespace requite {

void Context::logMessage(llvm::Twine message) {
  std::scoped_lock guard(this->_mutex);
  llvm::outs() << message << "\n";
}

void Context::logSourceMessage(llvm::Twine filename, requite::LogType type,
                               llvm::Twine message) {
  llvm::SmallString<128> buffer_a;
  llvm::SmallString<128> buffer_b;
  std::scoped_lock guard(this->_mutex);
  this->_source_mgr.PrintMessage(
      llvm::outs(),
      llvm::SMDiagnostic(filename.toStringRef(buffer_a),
                         static_cast<llvm::SourceMgr::DiagKind>(type),
                         message.toStringRef(buffer_b)));
}

void Context::logSourceMessage(const requite::Token &token,
                               requite::LogType type, llvm::Twine message,
                               llvm::ArrayRef<llvm::SMRange> ranges,
                               llvm::ArrayRef<llvm::SMFixIt> fixits) {
  std::scoped_lock guard(this->_mutex);
  this->_source_mgr.PrintMessage(
      llvm::outs(), llvm::SMLoc::getFromPointer(token.getSourceTextPtr()),
      static_cast<llvm::SourceMgr::DiagKind>(type), message, ranges, fixits,
      true);
}

void Context::logSourceMessage(const requite::Expression &expression,
                               requite::LogType type, llvm::Twine message,
                               llvm::ArrayRef<llvm::SMRange> ranges,
                               llvm::ArrayRef<llvm::SMFixIt> fixits) {
  std::scoped_lock guard(this->_mutex);
  this->_source_mgr.PrintMessage(
      llvm::outs(), llvm::SMLoc::getFromPointer(expression.getSourceTextPtr()),
      static_cast<llvm::SourceMgr::DiagKind>(type), message, ranges, fixits,
      true);
}

void Context::logErrorNonInstantEvaluatableName(
    requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "symbol names must be instantly evaluatable");
}

void Context::logErrorNonExternallyAccessableTable(
    requite::Expression &expression) {
  this->logSourceMessage(
      expression, requite::LogType::ERROR,
      "symbol does not have externally accessable lookup table");
}

void Context::logErrorAlreadySymbolOfName(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "already symbol of name");
}

void Context::logErrorDuplicateAttribute(requite::Expression &expression,
                                         requite::AttributeType type) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(type)) +
                             " attribute is ascribed more than once");
}

void Context::logErrorMustNotHaveAttributeFlags(
    requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(expression.getOpcode())) +
                             " must not have attributes");
}

void Context::logNotSupportedYet(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "not supported yet");
}

} // namespace requite