#include <requite/context.hpp>
#include <requite/numeric.hpp>

namespace requite {

bool Context::evaluateName(llvm::StringRef &out_name, requite::Scope &scope,
                           requite::Expression &value_expression,
                           requite::LogMode log_mode) {
  // TODO evaluate names
  if (value_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    if (log_mode == requite::LogMode::ECHO)
        [[unlikely]] // unlikely because of yielding expressions in
                     // contextualizer0 {
      this->logErrorNonInstantEvaluatableName(value_expression);
      return false;
  }
  out_name = value_expression.getDataText();
  return true;
}

bool Context::evaluateConstantUnsigned(unsigned &out_unsigned,
                                       requite::Scope &scope,
                                       requite::Expression &value_expression,
                                       requite::LogMode log_mode) {
  switch (const requite::Opcode opcode = value_expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL: {
    requite::NumericResult result = requite::getNumericValue(
        value_expression.getSourceText(), out_unsigned);
    if (result != requite::NumericResult::OK) {
      if (log_mode == requite::LogMode::ECHO) [[likely]] {
        this->logErrorNumericParse(value_expression, result);
      }
      return false;
    }
  }
    return true;
  case requite::Opcode::ADDRESS_DEPTH: {
    out_unsigned = this->getAddressDepth();
  }
    return true;
  case requite::Opcode::ADDRESS_SIZE: {
    out_unsigned = this->getAddressSize();
  }
    return true;
  default:
    break;
  }
  return false;
}

} // namespace requite