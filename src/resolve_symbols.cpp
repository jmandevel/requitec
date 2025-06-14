#include <requite/context.hpp>

namespace requite {

bool Context::resolveSymbol(requite::Symbol &out_symbol, requite::Scope &scope,
                            requite::Expression &symbol_expression) {
  switch (const requite::Opcode opcode = symbol_expression.getOpcode()) {
  case requite::Opcode::__IDENTIFIER_LITERAL: {
    for (requite::Scope &containing_scope : scope.getContainingSubrange()) {
      requite::RootSymbol user =
          containing_scope.lookupInternalRootSymbol(symbol_expression.getDataText());
      if (user.getIsNone()) {
        continue;
      } else if (user.getIsAlias()) {
        requite::Alias &alias = user.getAlias();
        if (!this->prototypeUserSymbol(alias)) {
          return false;
        }
        out_symbol.wrapSymbol(alias.getSymbol());
        return true;
      } else if (user.getIsObject()) {
        requite::Object &object = user.getObject();
        if (!this->prototypeUserSymbol(object)) {
          return false;
        }
        out_symbol.getRoot().setType(requite::RootSymbolType::OBJECT);
        out_symbol.getRoot().setObject(object);
        return true;
      }
    }
    return false;
  }
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    requite::Expression &ascribed = symbol_expression.getBranch();
    requite::MakeAttributesResult result =
        requite::Attributes::makeAttributes(*this, ascribed.getNext());
    if (result.has_error) {
      return false;
    }
    out_symbol.applyAttributes(result.attributes);
    return this->resolveSymbol(out_symbol, scope, ascribed);
  }
  case requite::Opcode::SIGNED_INTEGER: {
    out_symbol.getRoot().setType(requite::RootSymbolType::SIGNED_INTEGER);
    unsigned depth;
    if (!this->evaluateConstantUnsigned(depth, scope,
                                        symbol_expression.getBranch())) {
      return false;
    }
    out_symbol.getRoot().setDepth(depth);
    return true;
  }
  case requite::Opcode::_REFERENCE: {
    requite::SubSymbol &sub = out_symbol.makeSubSymbol();
    sub.setType(requite::SubSymbolType::REFERENCE);
    return this->resolveSymbol(out_symbol, scope,
                               symbol_expression.getBranch());
  }
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