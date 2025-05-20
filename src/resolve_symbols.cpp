// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/alias.hpp>
#include <requite/context.hpp>
#include <requite/expression.hpp>
#include <requite/module.hpp>
#include <requite/numeric.hpp>
#include <requite/object.hpp>
#include <requite/procedure.hpp>
#include <requite/resolver.hpp>
#include <requite/symbol.hpp>
#include <requite/variable.hpp>

#include <ranges>
#include <tuple>

namespace requite {

bool Context::resolveSymbols() {
  requite::Resolver resolver(*this);
  resolver.resolveSymbols();
  return resolver.getIsOk();
}

void Resolver::resolveSymbols() {
  for (std::unique_ptr<requite::Module> &module_uptr :
       this->getContext().getModuleUptrs()) {
    requite::Module &module = requite::getRef(module_uptr);
    this->resolveModuleSymbols(module);
  }
}

void Resolver::resolveModuleSymbols(requite::Module &module) {
  for (std::unique_ptr<requite::Alias> &alias_uptr : module.getAliases()) {
    requite::Alias &alias = requite::getRef(alias_uptr);
    this->resolveAlias(alias);
  }
  for (std::unique_ptr<requite::Procedure> &procedure_uptr :
       module.getProcedures()) {
    requite::Procedure &procedure = requite::getRef(procedure_uptr);
    this->resolveProcedure(procedure);
  }
  for (std::unique_ptr<requite::Variable> &variable_uptr :
       module.getVariables()) {
    requite::Variable &variable = requite::getRef(variable_uptr);
    this->resolveVariable(variable);
  }
}

void Resolver::resolveAlias(requite::Alias &alias) {
  requite::Symbol &symbol = alias.getSymbol();
  if (!symbol.getIsEmpty()) {
    return;
  }
  requite::Scope &containing_scope = alias.getContainingScope();
  requite::Expression &expression = alias.getExpression();
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::ALIAS);
  requite::Expression &name_expression = expression.getBranch();
  requite::Expression &symbol_expression = name_expression.getNext();
  const bool had_error = this->resolveMatteSymbolExpression(
      symbol, containing_scope, symbol_expression);
  return;
}

void Resolver::resolveProcedure(requite::Procedure &procedure) {
  requite::Signature &signature = procedure.getSignature();
  if (!signature.getIsEmpty()) {
    return;
  }
  if (procedure.getType() == requite::ProcedureType::ENTRY_POINT) {
    requite::Symbol &return_type = signature.getReturnType();
    requite::RootSymbol &return_root = return_type.getRoot();
    return_root.setType(requite::RootSymbolType::SIGNED_INTEGER);
    return_root.setDepth(32); // TODO use address depth
    // TODO add arguments
    return;
  }
  requite::Scope &scope = procedure.getScope();
  requite::Expression &expression = scope.getExpression();
  requite::Scope &containing_scope = scope.getContainingScope();

  requite::Expression *signature_expression_ptr = nullptr;
  if (procedure.getType() == requite::ProcedureType::FUNCTION ||
      procedure.getType() == requite::ProcedureType::METHOD) {
    requite::Expression &name_expression = expression.getBranch();
    signature_expression_ptr = &name_expression.getNext();
  }
  requite::Expression &signature_expression =
      requite::getRef(signature_expression_ptr);
  const bool had_error = this->resolveMatteSymbolSignatureExpression(
      signature, containing_scope, signature_expression);
  return;
}

void Resolver::resolveVariable(requite::Variable &variable) {
  requite::Symbol &symbol = variable.getDataType();
  if (!symbol.getIsEmpty()) {
    return;
  }
  requite::Scope &containing_scope = variable.getContainingScope();
  requite::Expression &expression = variable.getExpression();
  requite::Expression &symbol_binding_expression = expression.getBranch();
  requite::Expression &name_expression = symbol_binding_expression.getBranch();
  requite::Expression &symbol_expression = name_expression.getNext();
  requite::Expression &initial_value_expression =
      symbol_binding_expression.getNext();
  // if (!this->resolveMatteSymbolExpression(symbol, containing_scope,
  // symbol_expression))
  //{
  //   return;
  // }
  if (!symbol.getRoot().getIsInference()) {
    return;
  }
  // TODO
  //  inference from initial_value_expression
}

bool Resolver::resolveCompileTimeUnsignedInteger(
    unsigned &uint, requite::Expression &expression) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_INTEGER_LITERAL: {
    requite::NumericResult result =
        requite::getNumericValue(expression.getSourceText(), uint);
    if (result != requite::NumericResult::OK) {
      this->getContext().logSourceMessage(
          expression, requite::LogType::ERROR,
          llvm::Twine("failed to get compile time numeric value \n\treason: ") +
              requite::getDescription(result) + ".");
    }
  }
    return true;
  case requite::Opcode::ADDRESS_SIZE: {
    uint = this->getContext().getLlvmDataLayout().getPointerSize();
  }
    return true;
  case requite::Opcode::ADDRESS_DEPTH: {
    uint = this->getContext().getLlvmDataLayout().getPointerSizeInBits();
  }
    return true;
  case requite::Opcode::BITS_PER_BYTE:
    uint = 8;
    return true;
  default:
    this->getContext().logSourceMessage(
        expression, requite::LogType::ERROR,
        "unable to resolve numeric value at compile time.");
  }
  return false;
}

} // namespace requite