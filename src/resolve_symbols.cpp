#include <requite/context.hpp>

namespace requite {

bool Context::resolveSymbol(requite::Symbol &out_symbol, requite::Scope &scope,
                            requite::Expression &symbol_expression) {
  // TODO
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