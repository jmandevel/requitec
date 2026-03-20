#include <rq/build.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>

namespace rq {

void LlvmIrBuilder::buildLlvmIr() {
  for (rq::Symbol &symbol :
       this->getContext().getTop().getUnamedSymbolsListRef()) {
    if (llvm::isa<rq::Procedure>(symbol)) {
      rq::Procedure &procedure = llvm::cast<rq::Procedure>(symbol);
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
    procedure.llvm_function_type_ptr = llvm::FunctionType::get(
        this->getContext().getLlvmIrBuilder().getInt32Ty(), false);
    procedure.llvm_function_ptr = llvm::Function::Create(
        procedure.llvm_function_type_ptr, llvm::Function::ExternalLinkage,
        "main", this->getContext().getLlvmModule());
    procedure.llvm_block_ptr =
        llvm::BasicBlock::Create(this->getContext().getLlvmContext(), "entry",
                                 procedure.llvm_function_ptr);
    this->getContext().getLlvmIrBuilder().SetInsertPoint(
        procedure.llvm_block_ptr);
    rq::Instruction &instruction = procedure.getInstruction();
    this->getContext().getLlvmIrBuilder().CreateRet(
        llvm::ConstantInt::get(
            this->getContext().getLlvmIrBuilder().getInt32Ty(),
            llvm::cast<rq::IntegerConstant>(instruction.getTail()).getInt()));
  }
}

} // namespace rq