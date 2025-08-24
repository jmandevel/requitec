// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/options.hpp>
#include <requite/token.hpp>

#include <llvm/Support/raw_ostream.h>

#include <mutex>

namespace requite {

void Context::logMessage(llvm::Twine message) {
  std::scoped_lock guard(this->_mutex);
  llvm::outs() << message << "\n";
}

void Context::logInputFileMessage(requite::LogType type, llvm::Twine message) {
  llvm::StringRef input_path = requite::getInputFilePath();
  this->logSourceMessage(input_path, type, message);
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
#if !defined(_NDEBUG) && __has_builtin(__builtin_debugtrap)
  if (type == requite::LogType::ERROR) {
    __builtin_debugtrap();
  }
#endif
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
#if !defined(_NDEBUG) && __has_builtin(__builtin_debugtrap)
  if (type == requite::LogType::ERROR) {
    __builtin_debugtrap();
  }
#endif
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

void Context::logErrorNumericParse(requite::Expression &expression,
                                   requite::NumericResult result) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         requite::getDescription(result));
}

void Context::logNotSupportedYet(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         "not supported yet");
}

void Context::logErrorMustNotHaveAttributes(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(expression.getOpcode())) +
                             " must not have attributes");
}

void Context::logErrorMissingTrailingSemicolon(
    requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(expression.getOpcode())) +
                             " expression is missing trailing semicolon");
}

void Context::logErrorExpectedCommaSeperator(const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected comma seperator");
}

void Context::logErrorExpectedSemicolonSeperator(const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected semicolon seperator");
}

void Context::logErrorExpectedSeperatorOrRightBracket(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected seperator or right bracket grouping");
}

void Context::logErrorExpectedSeperator(const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected comma or semicolon seperator");
}

void Context::logErrorUnterminatedExpression(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine("unterminated ") +
                             requite::getName(expression.getOpcode()));
}

void Context::logErrorUnterminatedAttribute(const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "unterminated attribute");
}

void Context::logErrorMustNotHaveParameterMark(
    requite::Expression &containing_expression, const requite::Token &token) {
  this->logSourceMessage(
      token, requite::LogType::ERROR,
      llvm::Twine(requite::getName(containing_expression.getOpcode())) +
          " must not have parameter mark");
}

void Context::logErrorMustHaveParameterMarks(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(expression.getOpcode())) +
                             " must have parameter marks");
}

} // namespace requite