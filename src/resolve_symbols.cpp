#include <requite/context.hpp>

namespace requite {

bool Context::resolveSymbol(requite::Symbol &out_symbol, requite::Scope &scope,
                            requite::Expression &symbol_expression) {
  switch (const requite::Opcode opcode = symbol_expression.getOpcode()) {
  case requite::Opcode::SIGNED_INTEGER: {
    out_symbol.getRoot().setType(requite::RootSymbolType::SIGNED_INTEGER);
    unsigned depth;
    if (!this->evaluateConstantUnsigned(depth, scope, symbol_expression.getBranch())) {
        return false;
    }
    out_symbol.getRoot().setDepth(depth);
  } return true;
  default:
    break;
  }
  return false;
}

bool Context::resolveTypeOfValue(requite::Symbol &out_symbol,
                                 requite::Scope &scope,
                                 requite::Expression &symbol_expression,
                                 requite::Expression &value_expression) {
  // TODO
  return false;
}

bool Context::inferenceTypeOfValue(requite::Symbol &out_symbol,
                                   requite::Scope &scope,
                                   requite::Expression &value_expression) {
  // TODO
  return false;
}

} // namespace requite