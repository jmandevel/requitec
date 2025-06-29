#include <requite/builder.hpp>
#include <requite/context.hpp>
#include <requite/numeric.hpp>
#include <requite/strings.hpp>

namespace requite {

bool Context::buildIr() {
  requite::Builder builder(*this);
  bool is_ok = true;
  requite::Module &source_module = this->getSourceModule();
  if (source_module.getHasEntryPoint()) {
    for (requite::Procedure &entry_point :
         source_module.getEntryPoint().getOverloadSubrange()) {
      if (!builder.buildSymbolEntryPoint(entry_point)) {
        is_ok = false;
      }
    }
  }
  return is_ok;
}

bool Builder::buildSymbolEntryPoint(requite::Procedure &entry_point) {
  bool is_ok = true;
  entry_point.setLlvmFunctionType(requite::getRef(llvm::FunctionType::get(
      llvm::Type::getInt32Ty(this->getContext().getLlvmContext()), false)));
  entry_point.setLlvmFunction(requite::getRef(llvm::Function::Create(
      &entry_point.getLlvmFunctionType(), llvm::Function::ExternalLinkage,
      entry_point.getMangledName(), this->getContext().getLlvmModule())));
  entry_point.setLlvmBlock(requite::getRef(llvm::BasicBlock::Create(
      this->getContext().getLlvmContext(), requite::PROCEDURE_ENTRY_BLOCK_NAME,
      &entry_point.getLlvmFunction())));
  this->getContext().getLlvmBuilder().SetInsertPoint(
      &entry_point.getLlvmBlock());
  for (requite::Expression &statement :
       entry_point.getExpression().getBranchSubrange()) {
    if (!this->buildStatement(statement)) {
      is_ok = false;
    }
  }
  return is_ok;
}

bool Builder::buildStatement(requite::Expression &statement) {
  switch (const requite::Opcode opcode = statement.getOpcode()) {
  case requite::Opcode::_LOCAL:
    return this->buildStatement_Local(statement);
  case requite::Opcode::EXIT:
    return this->buildStatementExit(statement);
  default:
    break;
  }
  return false;
}

bool Builder::buildStatementExit(requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::EXIT);
  // TODO: check inside entry_point
  requite::Expression &branch = statement.getBranch();
  requite::Symbol return_type;
  return_type.getRoot().setType(requite::RootSymbolType::SIGNED_INTEGER);
  return_type.getRoot().setDepth(32);
  llvm::Value *value = this->buildValue(branch, return_type);
  this->getContext().getLlvmBuilder().CreateRet(value);
  return true;
}

bool Builder::buildStatement_Local(requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_LOCAL);
  requite::OrderedVariable &local = statement.getOrderedVariable();

  return true;
}

llvm::Value *Builder::buildValue(requite::Expression &expression,
                                 const requite::Symbol &expected_type) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::__INTEGER_LITERAL:
    return this->buildValue__IntegerLiteral(expression, expected_type);
  case requite::Opcode::_ADD:
    return this->buildValue_Add(expression, expected_type);
  default:
    break;
  }
  return nullptr;
}

llvm::Value *
Builder::buildValue__IntegerLiteral(requite::Expression &expression,
                                    const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::__INTEGER_LITERAL);
  // TODO: check expected type
  // TODO: load integer of expected type size using APInt
  unsigned integer;
  requite::NumericResult result =
      requite::getNumericValue(expression.getSourceText(), integer);
  llvm::Value *value = this->getContext().getLlvmBuilder().getInt32(integer);
  return value;
}

llvm::Value *Builder::buildValue_Add(requite::Expression &expression,
                                     const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ADD);
  if (expected_type.getIsInteger()) {
    requite::Expression& first = expression.getBranch();
    llvm::Value *lhs = this->buildValue(first, expected_type);
    for (requite::Expression &branch : first.getNextSubrange()) {
      llvm::Value *rhs = this->buildValue(branch, expected_type);
      lhs = this->getContext().getLlvmBuilder().CreateAdd(lhs, rhs);
    }
    return lhs;
  }
  // TODO other addable stuff
  return nullptr;
}

} // namespace requite