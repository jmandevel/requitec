#include <requite/context.hpp>
#include <requite/strings.hpp>
#include <requite/numeric.hpp>

namespace requite {

bool Context::buildIr() {
  bool is_ok = true;
  requite::Module &source_module = this->getSourceModule();
  if (source_module.getHasEntryPoint()) {
    for (requite::Procedure &entry_point :
         source_module.getEntryPoint().getOverloadSubrange()) {
      if (!this->buildEntryPoint(entry_point)) {
        is_ok = false;
      }
    }
  }
  return is_ok;
}

bool Context::buildEntryPoint(requite::Procedure &entry_point) {
  bool is_ok = true;
  entry_point.setLlvmFunctionType(requite::getRef(llvm::FunctionType::get(
      llvm::Type::getInt32Ty(this->getLlvmContext()), false)));
  entry_point.setLlvmFunction(requite::getRef(llvm::Function::Create(
      &entry_point.getLlvmFunctionType(), llvm::Function::ExternalLinkage,
      entry_point.getMangledName(), this->getLlvmModule())));
  entry_point.setLlvmBlock(requite::getRef(llvm::BasicBlock::Create(
      this->getLlvmContext(), requite::PROCEDURE_ENTRY_BLOCK_NAME,
      &entry_point.getLlvmFunction())));
  this->getLlvmBuilder().SetInsertPoint(&entry_point.getLlvmBlock());
  for (requite::Expression &statement :
       entry_point.getExpression().getBranchSubrange()) {
    if (!this->buildStatement(statement)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Context::buildStatement(requite::Expression &statement) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::EXIT:
    return this->buildStatementExit(statement);
  default:
    break;
  }
  return false;
}

bool Context::buildStatementExit(requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::EXIT);
  // TODO: check inside entry_point
  requite::Expression &branch = statement.getBranch();
  requite::Symbol return_type;
  return_type.getRoot().setType(requite::RootSymbolType::SIGNED_INTEGER);
  return_type.getRoot().setDepth(32);
  llvm::Value *value = this->buildValue(branch, return_type);
  this->getLlvmBuilder().CreateRet(value);
  return true;
}

llvm::Value *Context::buildValue(requite::Expression &expression,
                                 const requite::Symbol &expected_type) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL:
    return this->buildValue_IntegerLiteral(expression, expected_type);
  default:
    break;
  }
  return nullptr;
}

llvm::Value *
Context::buildValue_IntegerLiteral(requite::Expression &expression,
                                   const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::__INTEGER_LITERAL);
  // TODO: check expected type
  // TODO: load integer of expected type size using APInt
  unsigned integer;
  requite::NumericResult result =
      requite::getNumericValue(expression.getSourceText(), integer);
  llvm::Value *value = this->getLlvmBuilder().getInt32(integer);
  return value;
}

} // namespace requite