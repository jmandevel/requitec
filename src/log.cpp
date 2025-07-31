// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/options.hpp>
#include <requite/symbol.hpp>
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

void Context::logErrorDuplicateAttribute(requite::Expression &expression,
                                         requite::AttributeType type) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine(requite::getName(type)) +
                             " attribute is ascribed more than once");
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

void Context::logErrorInvalidExpectedTypeForOperation(
    requite::Expression &expression, const requite::Symbol &expected_type) {
  llvm::SmallString<32> buffer;
  llvm::StringRef type_name = expected_type.getName(buffer);
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine("operation of opcode \"") +
                             requite::getName(expression.getOpcode()) +
                             "\" can not result in value of type" + type_name);
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

void Context::logErrorExpectedExpressionBeforeSemicolon(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected expression before semicolon");
}

void Context::logErrorMissingCommmaSeperator(const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "missing comma seperator");
}

void Context::logErrorExpectedExpressionBeforeComma(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected expression before comma");
}

void Context::logErrorExpectedExpressionAfterComma(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "expected expression after comma");
}

void Context::logErrorPositionalFieldsEndBeforeExpression(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "unexpected positional fields end before expression");
}

void Context::logErrorNamedFieldsBeginAfterExpression(
    const requite::Token &token) {
  this->logSourceMessage(token, requite::LogType::ERROR,
                         "unexpected named fields begin after expression");
}

void Context::logErrorUnterminatedExpression(requite::Expression &expression) {
  this->logSourceMessage(expression, requite::LogType::ERROR,
                         llvm::Twine("unterminated ") +
                             requite::getName(expression.getOpcode()));
}

} // namespace requite