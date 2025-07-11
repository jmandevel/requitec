#include <requite/context.hpp>
#include <requite/numeric.hpp>

namespace requite {

requite::EvaluationResult Context::evaluateName(llvm::StringRef &out_name, requite::Scope &scope,
                           requite::Expression &value_expression,
                           requite::LookupMode mode) {
  // TODO evaluate names
  if (value_expression.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
    if (mode == requite::LookupMode::UNFOUND_SYMBOL_IS_ERROR)
        [[unlikely]] // unlikely because of yielding expressions in
                     // contextualizer0 {
      this->logErrorNonInstantEvaluatableName(value_expression);
      return requite::EvaluationResult::GENERATED_NOT_DONE;
  }
  out_name = value_expression.getDataText();
  return requite::EvaluationResult::LITERAL;
}

requite::EvaluationResult Context::evaluateConstantUnsigned(unsigned &out_unsigned,
                                       requite::Scope &scope,
                                       requite::Expression &value_expression,
                                       requite::LookupMode mode) {
  switch (const requite::Opcode opcode = value_expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL: {
    requite::NumericResult result = requite::getNumericValue(
        value_expression.getSourceText(), out_unsigned);
    if (result != requite::NumericResult::OK) {
      this->logErrorNumericParse(value_expression, result);
      return requite::EvaluationResult::ERROR;
    }
  }
    return requite::EvaluationResult::LITERAL;
  case requite::Opcode::ADDRESS_DEPTH: {
    out_unsigned = this->getAddressDepth();
  }
    return requite::EvaluationResult::LITERAL;
  case requite::Opcode::ADDRESS_SIZE: {
    out_unsigned = this->getAddressSize();
  }
    return requite::EvaluationResult::LITERAL;
  default:
    break;
  }
    return requite::EvaluationResult::ERROR;
}

} // namespace requite