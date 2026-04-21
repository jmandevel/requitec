#include <rq/build.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>

namespace rq {

void LlvmIrBuilder::buildLlvmIr() {
  for (rq::Symbol &symbol :
       this->getContext().getTop().getUnamedSymbolsListRef()) {
    if (llvm::isa<rq::Procedure>(symbol)) {
      rq::Procedure &procedure = llvm::integer_cast<rq::Procedure>(symbol);
      if (procedure.getContainingModule() !=
          this->getContext().getSourceModule()) {
        continue;
      }
      this->buildProcedure(procedure);
    }
  }
}

void LlvmIrBuilder::buildProcedure(rq::Procedure &procedure) {
  if (!llvm::isa<rq::Entry>(procedure)) {
    RQ_TODO_IMPLEMENTATION();
  }
  if (procedure.getHasInstruction()) {
    llvm::FunctionType *llvm_function_type_ptr = llvm::FunctionType::get(
        this->getContext().getLlvmIrBuilder().getInt32Ty(), false);
    llvm::Function *llvm_function_ptr = llvm::Function::Create(
        llvm_function_type_ptr, llvm::Function::ExternalLinkage, "main",
        this->getContext().getLlvmModule());
    llvm::BasicBlock *llvm_block_ptr = llvm::BasicBlock::Create(
        this->getContext().getLlvmContext(), "entry", llvm_function_ptr);
    this->getContext().getLlvmIrBuilder().SetInsertPoint(llvm_block_ptr);
    rq::Instruction &instruction = procedure.getInstruction();
    rq::BinaryInstruction &binary_instruction =
        llvm::integer_cast<rq::BinaryInstruction>(instruction);
    this->getContext().getLlvmIrBuilder().CreateRet(llvm::ConstantInt::get(
        this->getContext().getLlvmIrBuilder().getInt32Ty(),
        llvm::integer_cast<rq::IntegerConstant>(binary_instruction.getAddress1())
            .getInt()));
    procedure.setLlvmFunctionPtr(llvm_function_ptr);
  }
}

} // namespace rq