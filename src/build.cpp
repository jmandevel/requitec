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
  requite::setSingleRef(this->_procedure_ptr, entry_point);
  this->setScope(entry_point.getScope());
  bool is_ok = true;
  entry_point.setLlvmFunctionType(requite::getRef(llvm::FunctionType::get(
      llvm::Type::getIntNTy(this->getContext().getLlvmContext(),
                            this->getContext().getAddressDepth()),
      false)));
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
  requite::Expression &branch = statement.getBranch();
  requite::Symbol return_type;
  return_type.getRoot().setAsSigned(64);
  llvm::Value *value = this->buildValue(branch, return_type);
  this->getContext().getLlvmBuilder().CreateRet(value);
  return true;
}

llvm::Type *Builder::makeLlvmType(const requite::Symbol &type) {
  llvm::Type *llvm_type = nullptr;
  const requite::RootSymbol &root = type.getRoot();
  switch (const requite::RootSymbolType type = root.getType()) {
  case requite::RootSymbolType::SIGNED: {
    llvm_type = this->getContext().getLlvmBuilder().getIntNTy(root.getDepth());
    break;
  }
  }
  // TODO do more stuff
  return llvm_type;
}

void Builder::buildAssignment(llvm::Value *llvm_value,
                              llvm::AllocaInst *llvm_alloca) {
  this->getContext().getLlvmBuilder().CreateStore(
      llvm_value, llvm_alloca,
      false // TODO set true if type has volatile attribute flag
  );
}

bool Builder::buildStatement_Local(requite::Expression &statement) {
  REQUITE_ASSERT(statement.getOpcode() == requite::Opcode::_LOCAL);
  requite::Local &local = statement.getLocal();
  requite::Symbol &type = local.getDataType();
  llvm::Type *llvm_type = this->makeLlvmType(type);
  llvm::AllocaInst *llvm_alloca =
      this->buildLlvmAlloca(llvm_type, local.getName());
  local.setLlvmAllocaPtr(llvm_alloca);
  requite::Expression &name_expression = statement.getBranch();
  requite::Expression &value_expression = name_expression.getNext();
  // todo store values for aggregate types
  llvm::Value *llvm_value = this->buildValue(value_expression, type);
  if (llvm_value == nullptr) {
    return false;
  }
  this->buildAssignment(llvm_value, llvm_alloca);
  return true;
}

llvm::AllocaInst *Builder::buildLlvmAlloca(llvm::Type *llvm_type,
                                           llvm::StringRef name) {
  llvm::IRBuilderBase::InsertPoint old_insertion_point =
      this->getContext().getLlvmBuilder().saveAndClearIP();
  this->getContext().getLlvmBuilder().SetInsertPointPastAllocas(
      &this->getProcedure().getLlvmFunction());
  llvm::AllocaInst *llvm_alloca =
      this->getContext().getLlvmBuilder().CreateAlloca(llvm_type, nullptr,
                                                       name);
  this->getContext().getLlvmBuilder().restoreIP(old_insertion_point);
  return llvm_alloca;
}

llvm::Value *Builder::buildValue(requite::Expression &expression,
                                 const requite::Symbol &expected_type) {
  switch (const requite::Opcode opcode = expression.getOpcode()) {
  case requite::Opcode::__LOCAL_HANDLE:
    return this->buildValue__LocalHandle(expression, expected_type);
  case requite::Opcode::__IDENTIFIER_LITERAL:
    REQUITE_UNREACHABLE();
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
Builder::buildValue__LocalHandle(requite::Expression &expression,
                                 const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::__LOCAL_HANDLE);
  requite::Local &local = expression.getLocal();
  llvm::StringRef name = local.getName();
  llvm::AllocaInst *llvm_alloca = local.getLlvmAllocaPtr();
  return llvm_alloca;
}

llvm::Value *
Builder::buildValue__IntegerLiteral(requite::Expression &expression,
                                    const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::__INTEGER_LITERAL);
  unsigned depth = expected_type.getRoot().getDepth();
  llvm::APInt integer(depth, {});
  requite::NumericResult result =
      requite::getNumericValue(expression.getSourceText(), integer);
  llvm::Value *value = this->getContext().getLlvmBuilder().getInt(integer);
  return value;
}

llvm::Value *Builder::buildValue_Add(requite::Expression &expression,
                                     const requite::Symbol &expected_type) {
  REQUITE_ASSERT(expression.getOpcode() == requite::Opcode::_ADD);
  if (expected_type.getIsInteger()) {
    requite::Expression &first = expression.getBranch();
    llvm::Value *lhs = this->buildValue(first, expected_type);
    for (requite::Expression &branch : first.getNextSubrange()) {
      llvm::Value *rhs = this->buildValue(branch, expected_type);
      lhs = this->getContext().getLlvmBuilder().CreateAdd(lhs, rhs);
    }
    return lhs;
  } else if (expected_type.getIsFloat()) {
    requite::Expression &first = expression.getBranch();
    llvm::Value *lhs = this->buildValue(first, expected_type);
    for (requite::Expression &branch : first.getNextSubrange()) {
      llvm::Value *rhs = this->buildValue(branch, expected_type);
      lhs = this->getContext().getLlvmBuilder().CreateFAdd(lhs, rhs);
    }
    return lhs;
  }
  this->getContext().logErrorInvalidExpectedTypeForOperation(expression,
                                                             expected_type);
  return nullptr;
}

} // namespace requite