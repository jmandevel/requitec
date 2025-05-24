// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/anonymous_object.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/numeric.hpp>
#include <requite/resolver.hpp>
#include <requite/scope.hpp>
#include <requite/scope_walker.hpp>
#include <requite/signature.hpp>
#include <requite/symbol.hpp>
#include <requite/tuple.hpp>

#include <llvm/ADT/StringSet.h>

namespace requite {

bool Resolver::resolveMatteSymbolExpression(requite::Symbol &symbol,
                                            requite::Scope &containing_scope,
                                            requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_IDENTIFIER_LITERAL:
    return resolveMatteSymbolIdentifierLiteralExpression(
        symbol, containing_scope, expression);
    break;
  case requite::Opcode::REFLECT_VALUE:
    return resolveMatteSymbolReflectValueExpression(symbol, containing_scope,
                                                    expression);
    break;
  case requite::Opcode::REFLECT_SYMBOL:
    return resolveMatteSymbolReflectSymbolExpression(symbol, containing_scope,
                                                     expression);
    break;
  case requite::Opcode::ASCRIBE:
    return resolveMatteSymbolAscribeExpression(symbol, containing_scope,
                                               expression);
    break;
  case requite::Opcode::FAT_POINTER:
    return resolveMatteSymbolFatPointerExpression(symbol, containing_scope,
                                                  expression);
    break;
  case requite::Opcode::ARRAY:
    return resolveMatteSymbolArrayExpression(symbol, containing_scope,
                                             expression);
    break;
  case requite::Opcode::REFERENCE:
    return resolveMatteSymbolReferenceExpression(symbol, containing_scope,
                                                 expression);
    break;
  case requite::Opcode::STOLEN_REFERENCE:
    return resolveMatteSymbolStolenReferenceExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::POINTER:
    return resolveMatteSymbolPointerExpression(symbol, containing_scope,
                                               expression);
    break;
  case requite::Opcode::TUPLE:
    return resolveMatteSymbolTupleExpression(symbol, containing_scope,
                                             expression);
    break;
  case requite::Opcode::ANONYMOUS_OBJECT:
    return resolveMatteSymbolAnonymousObjectExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::NO_PARAMETER_SIGNATURE:
    [[fallthrough]];
  case requite::Opcode::NAMED_PARAMETER_SIGNATURE:
    [[fallthrough]];
  case requite::Opcode::POSITIONAL_PARAMETER_SIGNATURE:
    return this->resolveMatteSymbolSignatureExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::NULL_:
    return resolveMatteSymbolNullExpression(symbol, containing_scope,
                                            expression);
    break;
  case requite::Opcode::INFERENCE:
    return resolveMatteSymbolInferenceExpression(symbol, containing_scope,
                                                 expression);
    break;
  case requite::Opcode::VOID:
    return resolveMatteSymbolVoidExpression(symbol, containing_scope,
                                            expression);
    break;
  case requite::Opcode::BOOLEAN:
    return resolveMatteSymbolBooleanExpression(symbol, containing_scope,
                                               expression);
    break;
  case requite::Opcode::WORD:
    return resolveMatteSymbolWordExpression(symbol, containing_scope,
                                            expression);
    break;
  case requite::Opcode::SIGNED_INTEGER:
    return resolveMatteSymbolSignedIntegerExpression(symbol, containing_scope,
                                                     expression);
    break;
  case requite::Opcode::UNSIGNED_INTEGER:
    return resolveMatteSymbolUnsignedIntegerExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::BINARY_HALF_FLOAT:
    return resolveMatteSymbolBinaryHalfFloatExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::BINARY_SINGLE_FLOAT:
    return resolveMatteSymbolBinarySingleFloatExpression(
        symbol, containing_scope, expression);
    break;
  case requite::Opcode::BINARY_DOUBLE_FLOAT:
    return resolveMatteSymbolBinaryDoubleFloatExpression(
        symbol, containing_scope, expression);
    break;
  case requite::Opcode::BINARY_QUAD_FLOAT:
    return resolveMatteSymbolBinaryQuadFloatExpression(symbol, containing_scope,
                                                       expression);
    break;
  case requite::Opcode::C_CHAR:
    return resolveMatteSymbolCCharExpression(symbol, containing_scope,
                                             expression);
    break;
  case requite::Opcode::UTF8:
    return resolveMatteSymbolUtf8Expression(symbol, containing_scope,
                                            expression);
    break;
  case requite::Opcode::VARIADIC_ARGUMENTS:
    return resolveMatteSymbolVariadicArgumentsExpression(
        symbol, containing_scope, expression);
    break;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

bool Resolver::resolveMatteSymbolIdentifierLiteralExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() ==
                 requite::Opcode::_IDENTIFIER_LITERAL);
  llvm::StringRef name = expression.getDataText();
  requite::ScopeWalkerResult result =
      containing_scope.walkScopes(this->getContext())
          .doSearch([&](requite::Scope &scope) {
            requite::RootSymbol found = scope.lookupInternalRootSymbol(name);
            if (found.getIsNone())
              return requite::ScopeWalkerResultType::NOT_FOUND;
            symbol.getRoot() = found;
            return requite::ScopeWalkerResultType::FOUND;
          });
  if (result.type == requite::ScopeWalkerResultType::NOT_FOUND) {
    this->getContext().logSourceMessage(expression, requite::LogType::ERROR,
                                        llvm::Twine("symbol not found: \"") +
                                            name + "\"");
    this->setNotOk();
  }
  return true;
}

bool Resolver::resolveMatteSymbolReflectValueExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::REFLECT_VALUE);
  // TODO
  return true;
}

bool Resolver::resolveMatteSymbolReflectSymbolExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  return true;
}

bool Resolver::resolveMatteSymbolAscribeExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::MakeAttributesResult result =
      requite::Attributes::makeAttributes(this->getContext(), expression);
  if (result.has_error) {
    this->setNotOk();
    return true;
  }
  symbol.applyAttributes(result.attributes);
  return this->resolveMatteSymbolExpression(
      symbol, containing_scope, requite::getRef(result.last_expression_ptr));
}

bool Resolver::resolveMatteSymbolFatPointerExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::FAT_POINTER);
  requite::SubSymbol &sub = symbol.makeSubSymbol();
  sub.setType(requite::SubSymbolType::FAT_POINTER);
  requite::Expression &branch = expression.getBranch();
  return this->resolveMatteSymbolExpression(symbol, containing_scope, branch);
}

bool Resolver::resolveMatteSymbolArrayExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ARRAY);
  requite::SubSymbol &sub = symbol.makeSubSymbol();
  sub.setType(requite::SubSymbolType::ARRAY);
  requite::Expression &branch = expression.getBranch();
  requite::Expression &count_expression = branch.getNext();
  if (count_expression.getOpcode() == requite::Opcode::INFERENCED_COUNT) {
    sub.setHasInferencedCount();
  } else if (count_expression.getOpcode() ==
             requite::Opcode::_INTEGER_LITERAL) {
    unsigned count;
    const bool keep_going =
        this->resolveCompileTimeUnsignedInteger(count, count_expression);
    if (!keep_going) {
      this->setNotOk();
      return true;
    }
    sub.setCount(count);
  }
  return this->resolveMatteSymbolExpression(symbol, containing_scope, branch);
}

bool Resolver::resolveMatteSymbolReferenceExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::REFERENCE);
  requite::SubSymbol &sub = symbol.makeSubSymbol();
  sub.setType(requite::SubSymbolType::REFERENCE);
  return this->resolveMatteSymbolExpression(symbol, containing_scope,
                                            expression);
}

bool Resolver::resolveMatteSymbolStolenReferenceExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::STOLEN_REFERENCE);
  requite::SubSymbol &sub = symbol.makeSubSymbol();
  sub.setType(requite::SubSymbolType::STOLEN_REFERENCE);
  return this->resolveMatteSymbolExpression(symbol, containing_scope,
                                            expression);
}

bool Resolver::resolveMatteSymbolPointerExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::POINTER);
  requite::SubSymbol &sub = symbol.makeSubSymbol();
  sub.setType(requite::SubSymbolType::POINTER);
  return this->resolveMatteSymbolExpression(symbol, containing_scope,
                                            expression);
}

bool Resolver::resolveMatteSymbolTupleExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::Tuple tuple = {};
  const bool is_ok = this->resolveMatteSymbolTupleExpression(
      tuple, containing_scope, expression);
  if (is_ok) {
    requite::RootSymbol &root = symbol.getRoot();
    root.setType(requite::RootSymbolType::TUPLE);
    std::ignore = root.getTuple() = tuple;
  }
  return is_ok;
}

bool Resolver::resolveMatteSymbolTupleExpression(
    requite::Tuple &tuple, requite::Scope &containing_scope,
    requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::TUPLE);
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    requite::Symbol &element = tuple.getElementTypes().emplace_back();
    const bool keep_going =
        this->resolveMatteSymbolExpression(element, containing_scope, branch);
    if (!keep_going) {
      return false;
    }
  }
  return true;
}

bool Resolver::resolveMatteSymbolAnonymousObjectExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::AnonymousObject anonymous_object = {};
  const bool is_ok = this->resolveMatteSymbolAnonymousObjectExpression(
      anonymous_object, containing_scope, expression);
  if (is_ok) {
    requite::RootSymbol &root = symbol.getRoot();
    root.setType(requite::RootSymbolType::ANONYMOUS_OBJECT);
    std::ignore = root.getAnonymousObject() = anonymous_object;
  }
  return is_ok;
}

bool Resolver::resolveMatteSymbolAnonymousObjectExpression(
    requite::AnonymousObject &anonymous_object,
    requite::Scope &containing_scope, requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ANONYMOUS_OBJECT);
  llvm::StringSet found_property_names = {};
  for (requite::Expression &branch : expression.getBranchSubrange()) {
    requite::AnonymousProperty &property =
        anonymous_object.getProperties().emplace_back();
    if (branch.getOpcode() == requite::Opcode::BIND_SYMBOL) {
      requite::Expression &name_expression = branch.getBranch();
      requite::Expression &type_expression = name_expression.getNext();
      llvm::StringRef name = name_expression.getDataText();
      if (found_property_names.contains(name)) {
        this->getContext().logSourceMessage(
            name_expression, requite::LogType::ERROR,
            llvm::Twine("anonymous object has duplicate property of name \"") +
                name + "\".");
        this->setNotOk();
        return true;
      }
      found_property_names.insert(name);
      property.setName(name);
      requite::Symbol &property_type = property.getSymbol();
      const bool keep_going = this->resolveMatteSymbolExpression(
          property_type, containing_scope, type_expression);
      if (!keep_going) {
        return false;
      }
    } else if (branch.getOpcode() == requite::Opcode::DEFAULT_VALUE) {
      requite::Expression &symbol_binding_expression = branch.getBranch();
      requite::Expression &name_expression =
          symbol_binding_expression.getBranch();
      requite::Expression &type_expression = name_expression.getNext();
      requite::Expression &default_value_expression =
          symbol_binding_expression.getNext();
      property.setDefaultValueExpression(default_value_expression);
      llvm::StringRef name = name_expression.getDataText();
      if (found_property_names.contains(name)) {
        this->getContext().logSourceMessage(
            name_expression, requite::LogType::ERROR,
            llvm::Twine("anonymous object has duplicate property of name \"") +
                name + "\".");
        this->setNotOk();
        return true;
      }
      found_property_names.insert(name);
      property.setName(name);
      requite::Symbol &property_type = property.getSymbol();
      const bool keep_going = this->resolveMatteSymbolExpression(
          property_type, containing_scope, type_expression);
      if (!keep_going) {
        return false;
      }
    } else {
      REQUITE_UNREACHABLE();
    }
  }
  return true;
}

bool Resolver::resolveMatteSymbolSignatureExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::Signature signature = {};
  const bool is_ok = this->resolveMatteSymbolSignatureExpression(
      signature, containing_scope, expression);
  if (is_ok) {
    requite::RootSymbol &root = symbol.getRoot();
    root.setType(requite::RootSymbolType::SIGNATURE);
    std::ignore = root.getSignature() = signature;
  }
  return is_ok;
}

bool Resolver::resolveMatteSymbolSignatureExpression(
    requite::Signature &signature, requite::Scope &containing_scope,
    requite::Expression &expression) {
  llvm::StringSet found_parameter_names = {};
  requite::Expression &callee_expression = expression.getBranch();
  const bool keep_going = this->resolveMatteSymbolExpression(
      signature.getReturnType(), containing_scope, callee_expression);
  if (!keep_going) {
    return false;
  }
  if (expression.getOpcode() == requite::Opcode::NO_PARAMETER_SIGNATURE) {
    return true;
  } else if (expression.getOpcode() ==
             requite::Opcode::NAMED_PARAMETER_SIGNATURE) {
    for (requite::Expression &parameter_expression :
         callee_expression.getNext().getHorizontalSubrange()) {
      requite::SignatureParameter &parameter =
          signature.getParameters().emplace_back();
      if (parameter_expression.getOpcode() == requite::Opcode::DEFAULT_VALUE) {
        requite::Expression &binding_expression =
            parameter_expression.getBranch();
        requite::Expression &name_expression = binding_expression.getBranch();
        requite::Expression &type_expression = name_expression.getNext();
        requite::Expression &default_value_expression =
            binding_expression.getNext();
        llvm::StringRef name = name_expression.getDataText();
        if (found_parameter_names.contains(name)) {
          this->getContext().logSourceMessage(
              name_expression, requite::LogType::ERROR,
              llvm::Twine("signature has duplicate parameter of name \"") +
                  name + "\".");
          this->setNotOk();
          return true;
        }
        found_parameter_names.insert(name);
        parameter.setName(name);
        parameter.setDefaultValueExpression(default_value_expression);
        requite::Symbol &type = parameter.getType();
        const bool keep_going = this->resolveMatteSymbolExpression(
            type, containing_scope, type_expression);
        if (!keep_going) {
          return false;
        }
      } else if (parameter_expression.getOpcode() ==
                 requite::Opcode::BIND_SYMBOL) {
        requite::Expression &name_expression = parameter_expression.getBranch();
        requite::Expression &type_expression = name_expression.getNext();
        llvm::StringRef name = name_expression.getDataText();
        if (found_parameter_names.contains(name)) {
          this->getContext().logSourceMessage(
              name_expression, requite::LogType::ERROR,
              llvm::Twine("signature has duplicate parameter of name \"") +
                  name + "\".");
          this->setNotOk();
          return true;
        }
        found_parameter_names.insert(name);
        parameter.setName(name);
        requite::Symbol &type = parameter.getType();
        const bool keep_going = this->resolveMatteSymbolExpression(
            type, containing_scope, type_expression);
        if (!keep_going) {
          return false;
        }
      } else {
        REQUITE_UNREACHABLE();
      }
    }
  } else if (expression.getOpcode() ==
             requite::Opcode::POSITIONAL_PARAMETER_SIGNATURE) {
    for (requite::Expression &parameter_expression :
         callee_expression.getNext().getHorizontalSubrange()) {
      requite::SignatureParameter &parameter =
          signature.getParameters().emplace_back();
      if (parameter_expression.getOpcode() == requite::Opcode::DEFAULT_VALUE) {
        requite::Expression &type_expression = parameter_expression.getBranch();
        requite::Expression &default_value_expression =
            type_expression.getNext();
        parameter.setDefaultValueExpression(default_value_expression);
        requite::Symbol &type = parameter.getType();
        const bool keep_going = this->resolveMatteSymbolExpression(
            type, containing_scope, type_expression);
        if (!keep_going) {
          return false;
        }
      } else {
        requite::Expression &type_expression = parameter_expression;
        requite::Symbol &type = parameter.getType();
        const bool keep_going = this->resolveMatteSymbolExpression(
            type, containing_scope, type_expression);
        if (!keep_going) {
          return false;
        }
      }
    }
  } else {
    REQUITE_UNREACHABLE();
  }
  return true;
}

bool Resolver::resolveMatteSymbolNullExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::NULL_);
  return true;
}

bool Resolver::resolveMatteSymbolInferenceExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::INFERENCE);
  return true;
}

bool Resolver::resolveMatteSymbolVoidExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::VOID);
  return true;
}

bool Resolver::resolveMatteSymbolBooleanExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::BOOLEAN);
  return true;
}

bool Resolver::resolveMatteSymbolWordExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::WORD);
  requite::Expression &depth_expression = expression.getBranch();
  unsigned depth;
  const bool keep_going =
      this->resolveCompileTimeUnsignedInteger(depth, depth_expression);
  if (!keep_going) {
    return false;
  }
  root.setDepth(depth);
  return true;
}

bool Resolver::resolveMatteSymbolSignedIntegerExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::SIGNED_INTEGER);
  requite::Expression &depth_expression = expression.getBranch();
  unsigned depth;
  const bool keep_going =
      this->resolveCompileTimeUnsignedInteger(depth, depth_expression);
  if (!keep_going) {
    return false;
  }
  root.setDepth(depth);
  return true;
}

bool Resolver::resolveMatteSymbolUnsignedIntegerExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::UNSIGNED_INTEGER);
  requite::Expression &depth_expression = expression.getBranch();
  unsigned depth;
  const bool keep_going =
      this->resolveCompileTimeUnsignedInteger(depth, depth_expression);
  if (!keep_going) {
    return false;
  }
  root.setDepth(depth);
  return true;
}

bool Resolver::resolveMatteSymbolBinaryHalfFloatExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::BINARY_HALF_FLOAT);
  return true;
}

bool Resolver::resolveMatteSymbolBinarySingleFloatExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::BINARY_SINGLE_FLOAT);
  return true;
}

bool Resolver::resolveMatteSymbolBinaryDoubleFloatExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::BINARY_DOUBLE_FLOAT);
  return true;
}

bool Resolver::resolveMatteSymbolBinaryQuadFloatExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::BINARY_QUAD_FLOAT);
  return true;
}

bool Resolver::resolveMatteSymbolCCharExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::C_CHAR);
  return true;
}

bool Resolver::resolveMatteSymbolUtf8Expression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::UTF8);
  return true;
}

bool Resolver::resolveMatteSymbolVariadicArgumentsExpression(
    requite::Symbol &symbol, requite::Scope &containing_scope,
    requite::Expression &expression) {
  requite::RootSymbol &root = symbol.getRoot();
  root.setType(requite::RootSymbolType::VARIADIC_ARGUMENTS);
  return true;
}

} // namespace requite