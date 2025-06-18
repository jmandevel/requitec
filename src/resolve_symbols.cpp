#include <requite/context.hpp>
#include <requite/symbol_path.hpp>

#include <llvm/ADT/SmallPtrSet.h>

namespace requite {

bool Context::resolveSymbolPath(requite::SymbolPath &out_path,
                                requite::Scope &scope,
                                requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL_PATH);
  requite::Expression &branch = expression.getBranch();
  if (branch.getOpcode() == requite::Opcode::_IDENTIFY) {
    this->logErrorNonInstantEvaluatableName(branch);
    return false;
  }
  if (branch.getOpcode() == requite::Opcode::__IDENTIFIER_LITERAL) {
    llvm::StringRef text = branch.getDataText();
    out_path.getBody().push_back(text);
  } else {
    requite::Symbol symbol;
    if (!this->resolveSymbol(symbol, scope, branch)) {
      return false;
    }
    if (!symbol.getSubs().empty()) {
      this->logErrorNonExternallyAccessableTable(branch);
      this->logSourceMessage(branch, requite::LogType::NOTE,
                             "types with subtypes have no lookup tables");
      // TODO add note saying where symbol is declared and what type it is
      // TODO add note about using symbol..[root] to get the root type if its
      // object or table.
      return false;
    }
    requite::RootSymbol &root = symbol.getRoot();
    if (root.getIsTable()) {
      requite::Table &table = root.getTable();
      requite::Scope &table_scope = table.getScope();
      out_path.setHeadScope(table_scope);
    } else if (root.getIsObject()) {
      requite::Object &object = root.getObject();
      requite::Scope &object_scope = object.getScope();
      out_path.setHeadScope(object_scope);
    } else {
      this->logErrorNonExternallyAccessableTable(branch);
      // TODO add note saying where symbol is declared and what type it is
      return false;
    }
    // TODO check accessibility modifiers here
  }
  for (requite::Expression &next : branch.getNextSubrange()) {
    if (next.getOpcode() != requite::Opcode::__IDENTIFIER_LITERAL) {
      this->logErrorNonInstantEvaluatableName(next);
      return false;
    }
    llvm::StringRef name = next.getDataText();
    out_path.getBody().push_back(name);
  }
  return true;
}

bool Context::resolveSymbol(requite::Symbol &out_symbol, requite::Scope &scope,
                            requite::Expression &symbol_expression) {
  switch (const requite::Opcode opcode = symbol_expression.getOpcode()) {
  case requite::Opcode::__IDENTIFIER_LITERAL: {
    // TODO
    return false;
  }
  case requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL_PATH: {
    requite::SymbolPath path;
    if (!this->resolveSymbolPath(path, scope, symbol_expression)) {
      return false;
    }
    // TODO
    return false;
  };
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
  // TODO
  return false;
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
