#include <requite/context.hpp>
#include <requite/numeric.hpp>

namespace requite {

bool Context::evaluateInstantName(llvm::StringRef &out_name,
                           requite::Expression &value_expression) {
  // TODO
  return true;
}

bool Context::evaluateConstantUnsigned(unsigned &out_unsigned,
                                       requite::Scope &scope,
                                       requite::Expression &value_expression) {
  switch (const requite::Opcode opcode = value_expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL: {
    // TODO
  }
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
    return true;
}

} // namespace requite