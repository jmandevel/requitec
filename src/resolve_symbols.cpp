#include <requite/context.hpp>

namespace requite {

bool Context::resolveSymbol(requite::Symbol &out_symbol, requite::Scope &scope,
                            requite::Expression &symbol_expression) {
  switch (const requite::Opcode opcode = symbol_expression.getOpcode()) {
  case requite::Opcode::__IDENTIFIER_LITERAL: {
    for (requite::Scope &containing_scope : scope.getContainingSubrange()) {
      requite::RootSymbol user = containing_scope.lookupInternalUserSymbol(
          symbol_expression.getDataText());
      if (user.getIsNone()) {
        continue;
      } else if (user.getIsAlias()) {
        requite::Alias &alias = user.getAlias();
        // if (!this->prototypeUserSymbol(alias)) {
        //   return false;
        // }
        out_symbol.wrapSymbol(alias.getSymbol());
        return true;
      } else if (user.getIsObject()) {
        requite::Object &object = user.getObject();
        // if (!this->prototypeUserSymbol(object)) {
        //   return false;
        // }
        out_symbol.getRoot().setType(requite::RootSymbolType::OBJECT);
        out_symbol.getRoot().setObject(object);
        return true;
      }
    }
    return false;
  }
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH: {
    requite::AttributeFlags flags = {};
    requite::Expression &unascribed = symbol_expression.getBranch();
    requite::Expression &first_attribute = unascribed.getNext();
    const bool attributes_ok =
        this->resolveTypeAttributes(flags, first_attribute);
    out_symbol.applyAttributeFlags(flags);
    const bool next_resolve_ok =
        this->resolveSymbol(out_symbol, scope, unascribed);
    return attributes_ok && next_resolve_ok;
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
  switch (const requite::Opcode opcode = value_expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL: {
    requite::RootSymbol &root = out_symbol.getRoot();
    root.setType(requite::RootSymbolType::SIGNED_INTEGER);
    root.setDepth(this->getAddressDepth());
    return true;
  }
  case requite::Opcode::_ADD:
    return this->inferenceTypeOfNaryValue(out_symbol, scope, value_expression.getBranch());
  }
  this->logSourceMessage(value_expression, requite::LogType::ERROR,
                         "failed to inference type of value");
  return false;
}

bool Context::inferenceTypeOfNaryValue(requite::Symbol &out_symbol,
                                       requite::Scope &scope,
                                       requite::Expression &first) {

                                       }

bool Context::resolveTypeAttributes(requite::AttributeFlags flags,
                                    requite::Expression &first) {
  bool is_ok = true;
  for (requite::Expression &attribute : first.getHorizontalSubrange()) {
    const requite::Opcode opcode = attribute.getOpcode();
    const requite::AttributeType type = requite::getAttributeType(opcode);
    if (!requite::getIsTypeAttribute(type)) {
      this->logSourceMessage(attribute, requite::LogType::ERROR,
                             llvm::Twine(requite::getName(type)) +
                                 " attribute is not type attribute");
      is_ok = false;
      continue;
    }
    if (type != requite::AttributeType::NONE) {
      if (flags.getHasAttribute(type)) {
        this->logErrorDuplicateAttribute(attribute, type);
        is_ok = false;
        continue;
      }
      flags.addAttribute(type);
    }
  }
  return is_ok;
}

} // namespace requite
