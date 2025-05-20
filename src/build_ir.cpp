// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/builder.hpp>
#include <requite/context.hpp>
#include <requite/numeric.hpp>
#include <requite/strings.hpp>
#include <requite/symbol.hpp>

#include <llvm/IR/Function.h>

#include <ranges>

namespace requite {

bool Context::buildIr(requite::Module &module) {
  requite::Builder builder(*this, module);
  builder.buildModule();
  return builder.getIsOk();
}

void Builder::buildModule() {
  if (this->getModule().getHasEntryPoint()) {
    this->buildEntryPoint(this->getModule().getEntryPoint());
  }
}

bool Builder::buildEntryPoint(requite::Procedure &procedure) {
  procedure.setMangledName(requite::ENTRY_POINT_MANGLED_NAME);
  procedure.setLlvmFunctionType(requite::getRef(llvm::FunctionType::get(
      llvm::Type::getInt32Ty(this->getContext().getLlvmContext()), false)));
  procedure.setLlvmFunction(requite::getRef(llvm::Function::Create(
      &procedure.getLlvmFunctionType(), llvm::Function::ExternalLinkage,
      procedure.getMangledName(), this->getModule().getLlvmModule())));
  procedure.setLlvmBlock(requite::getRef(llvm::BasicBlock::Create(
      this->getContext().getLlvmContext(), requite::PROCEDURE_ENTRY_BLOCK_NAME,
      &procedure.getLlvmFunction())));
  this->getContext().getLlvmBuilder().SetInsertPoint(&procedure.getLlvmBlock());
  for (requite::Expression &statement :
       procedure.getScope().getExpression().getBranchSubrange()) {
    this->buildStatement(statement);
  }
  return true;
}

bool Builder::buildStatement(requite::Expression &statement) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::EXIT:
    return this->buildStatementExit(statement);
  default:
    break;
  }
  return false;
}

bool Builder::buildStatementExit(requite::Expression &expression) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::EXIT);
  // TODO: check inside entry_point
  requite::Expression &branch = expression.getBranch();
  requite::Symbol return_type;
  return_type.getRoot().setType(requite::RootSymbolType::SIGNED_INTEGER);
  return_type.getRoot().setDepth(32);
  llvm::Value *value = this->buildValue(branch, return_type);
  this->getContext().getLlvmBuilder().CreateRet(value);
  return true;
}

llvm::Value *Builder::buildValue(requite::Expression &expression,
                                 const requite::Symbol &expected_type) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::_INTEGER_LITERAL:
    return this->buildValue_IntegerLiteral(expression, expected_type);
  default:
    break;
  }
  return nullptr;
}

llvm::Value *
Builder::buildValue_IntegerLiteral(requite::Expression &expression,
                                   const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_INTEGER_LITERAL);
  // TODO: check expected type
  // TODO: load integer of expected type size using APInt
  unsigned integer;
  requite::NumericResult result =
      requite::getNumericValue(expression.getSourceText(), integer);
  llvm::Value *value = this->getContext().getLlvmBuilder().getInt32(integer);
  return value;
}

} // namespace requite