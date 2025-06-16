// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/context.hpp>
#include <requite/signature.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <vector>

namespace requite {

bool Context::prototypeUserSymbols() {
  bool is_ok = true;
  for (std::unique_ptr<requite::Module> &module_uptr : this->getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    if (!this->prototypeUserSymbol(module)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::prototypeUserSymbol(requite::Module &module) {
  bool is_ok = true;
  for (std::unique_ptr<requite::Object> &object_uptr :
       module.getObjectUptrs()) {
    requite::Object &object = requite::getRef(object_uptr);
    if (!this->prototypeUserSymbol(object)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Procedure> &procedure_uptr :
       module.getProcedureUptrs()) {
    requite::Procedure &procedure = requite::getRef(procedure_uptr);
    if (!this->prototypeUserSymbol(procedure)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::Alias> &alias_uptr : module.getAliasUptrs()) {
    requite::Alias &alias = requite::getRef(alias_uptr);
    if (!this->prototypeUserSymbol(alias)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::OrderedVariable> &variable_uptr :
       module.getOrderedVariableUptrs()) {
    requite::OrderedVariable &variable = requite::getRef(variable_uptr);
    if (!this->prototypeUserSymbol(variable)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::UnorderedVariable> &variable_uptr :
       module.getUnorderedVariableUptrs()) {
    requite::UnorderedVariable &variable = requite::getRef(variable_uptr);
    if (!this->prototypeUserSymbol(variable)) {
      is_ok = false;
    }
  }
  for (std::unique_ptr<requite::AnonymousFunction> &anonymous_function_uptr :
       module.getAnonymousFunctionUptrs()) {
    requite::AnonymousFunction &anonymous_function =
        requite::getRef(anonymous_function_uptr);
    if (!this->prototypeUserSymbol(anonymous_function)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::prototypeUserSymbol(requite::Object &object) {
  requite::Expression &expression = object.getExpression();
  return true;
}

bool Context::prototypeUserSymbol(requite::Procedure &procedure) {
  if (procedure.getType() == requite::ProcedureType::ENTRY_POINT) {
    procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
    requite::Signature &signature = procedure.getSignature();
    requite::Symbol &return_type = signature.getReturnType();
    requite::RootSymbol &return_root = return_type.getRoot();
    return_root.setType(requite::RootSymbolType::SIGNED_INTEGER);
    const unsigned pointer_depth = this->getAddressDepth();
    return_root.setDepth(pointer_depth);
    return true;
  }
  requite::Expression &expression = procedure.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

bool Context::prototypeUserSymbol(requite::Alias &alias) {
  requite::Symbol &symbol = alias.getSymbol();
  if (!symbol.getIsEmpty()) {
    return true;
  }
  requite::Expression &expression = alias.getExpression();
  requite::Scope &scope = alias.getContaining();
  requite::Expression &symbol_expression = expression.getBranch(1);
  if (!this->resolveSymbol(symbol, scope, symbol_expression)) {
    return false;
  }
  return true;
}

bool Context::prototypeUserSymbol(requite::OrderedVariable &variable) {
  requite::Expression &expression = variable.getExpression();
  requite::Scope &scope = variable.getContaining();
  requite::Expression &value_expression = expression.getBranch(1);
  requite::Symbol &type = variable.getDataType();
  if (!this->inferenceTypeOfValue(type, scope, value_expression)) {
    return false;
  }
  return true;
}

bool Context::prototypeUserSymbol(requite::UnorderedVariable &variable) {
  requite::Expression &expression = variable.getExpression();
  requite::Scope &scope = variable.getScope();
  requite::Expression &value_expression = expression.getBranch(1);
  requite::Symbol &type = variable.getDataType();
  if (!this->inferenceTypeOfValue(type, scope, value_expression)) {
    return false;
  }
  return true;
}

bool Context::prototypeUserSymbol(
    requite::AnonymousFunction &anonymous_function) {
  requite::Expression &expression = anonymous_function.getExpression();
  this->logNotSupportedYet(expression);
  return false;
}

} // namespace requite