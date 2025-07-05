#include <requite/context.hpp>
#include <requite/local_implementor.hpp>

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
        out_symbol.getRoot().setAsUser(object);
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
  case requite::Opcode::SIGNED: {
    unsigned depth;
    if (!this->evaluateConstantUnsigned(depth, scope,
                                        symbol_expression.getBranch())) {
      return false;
    }
    out_symbol.getRoot().setAsSigned(depth);
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

bool LocalImplementor::resolveTypeOfValue(requite::Symbol &out_symbol,
                                 requite::Expression &symbol_expression,
                                 requite::Expression &value_expression) {
  // TODO
  return false;
}

bool LocalImplementor::inferenceTypeOfValue(requite::Symbol &out_symbol,
                                   requite::Expression &value_expression) {
  switch (const requite::Opcode opcode = value_expression.getOpcode()) {
  case requite::Opcode::__LOCAL_HANDLE: {
    requite::Local& local = value_expression.getLocal();
    out_symbol = local.getDataType();
    return true;
  }
  case requite::Opcode::__INTEGER_LITERAL: {
    out_symbol.getRoot().setAsIntegerLiteral();
    return true;
  }
  case requite::Opcode::__IDENTIFIER_LITERAL: {
    llvm::StringRef name = value_expression.getDataText();
    requite::RootSymbol found = this->getScope().lookupUserSymbol(name);
    if (found.getIsNone()) {
      break;
    }
    if (found.getIsLocal()) {
      requite::Local &local = found.getLocal();
      out_symbol = local.getDataType();
      value_expression.clearData();
      value_expression.changeOpcode(requite::Opcode::__LOCAL_HANDLE);
      value_expression.setLocal(local);
      return true;
    }
  }
  case requite::Opcode::_ADD:
    return this->inferenceTypeOfNaryArithmeticValue(out_symbol,
                                                    value_expression);
  }
  this->getContext().logSourceMessage(value_expression, requite::LogType::ERROR,
                         "failed to inference type of value");
  return false;
}

bool LocalImplementor::inferenceTypeOfNaryArithmeticValue(
    requite::Symbol &out_symbol,
    requite::Expression &expression) {
  requite::Expression &first = expression.getBranch();
  if (!this->inferenceTypeOfValue(out_symbol, first)) {
    return false;
  }
  if (!out_symbol.getHasOnlyReferenceSubtypes()) {
    return false;
  }
  out_symbol.getSubs().clear();
  out_symbol.getRootAttributeFlags().clear();
  out_symbol.getRootAttributeFlags().addAttribute(
      requite::AttributeType::CONSTANT);
  for (requite::Expression &branch : first.getNextSubrange()) {
    requite::Symbol branch_symbol;
    if (!this->inferenceTypeOfValue(branch_symbol, branch)) {
      return false;
    }
    if (!branch_symbol.getHasOnlyReferenceSubtypes()) {
      return false;
    }
    const requite::RootSymbolType a_type = out_symbol.getRoot().getType();
    const requite::RootSymbolType b_type = branch_symbol.getRoot().getType();
    if (requite::getIsLiteral(a_type) && requite::getIsLiteral(b_type)) {
      if (a_type != b_type) {
        return false;
      }
      continue;
    }
    if (a_type == requite::RootSymbolType::INTEGER_LITERAL) {
      if (!requite::getIsInteger(b_type)) {
        return false;
      }
      out_symbol.getRoot() = branch_symbol.getRoot();
      continue;
    }
    if (b_type == requite::RootSymbolType::INTEGER_LITERAL) {
      if (!requite::getIsInteger(a_type)) {
        return false;
      }
      continue;
    }
    if (a_type == requite::RootSymbolType::FRACTIONAL_LITERAL) {
      if (!requite::getIsFloat(b_type)) {
        return false;
      }
      out_symbol.getRoot() = branch_symbol.getRoot();
      continue;
    }
    if (b_type == requite::RootSymbolType::FRACTIONAL_LITERAL) {
      if (!requite::getIsFloat(a_type)) {
        return false;
      }
      continue;
    }
    return false;
  }
  return true;
}

bool Context::resolveTypeAttributes(requite::AttributeFlags& flags,
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

void Context::finalizeIfLiteralType(requite::Symbol &symbol) {
  switch (const requite::RootSymbolType type = symbol.getRoot().getType()) {
  case requite::RootSymbolType::INTEGER_LITERAL:
    symbol.getRoot().setAsSigned(this->getAddressDepth());
    break;
  case requite::RootSymbolType::FRACTIONAL_LITERAL:
    symbol.getRoot().setAsBinary64();
    break;
  case requite::RootSymbolType::CODEUNIT_LITERAL:
    symbol.getRoot().setAsUtf8();
    break;
  case requite::RootSymbolType::STRING_LITERAL: {
    requite::SubSymbol &sub = symbol.makeSubSymbol();
    sub.setType(requite::SubSymbolType::FAT_POINTER);
    sub.getAttributeFlags().addAttribute(
        requite::AttributeType::NULL_TERMINATED);
    symbol.getRoot().setAsUtf8();
    symbol.getRootAttributeFlags().addAttribute(
        requite::AttributeType::CONSTANT);
    break;
  }
  default:
    break;
  }
}

} // namespace requite
