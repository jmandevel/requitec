#include <rq/build.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/symbols.hpp>

namespace rq {

void Builder::buildLlvmIr() {
  for (auto &kvp : this->getContext().getTop().getSymbolListSubrange()) {
    rq::BumpPtrListRef<rq::Symbol> list = kvp.getSecond();
    for (rq::Symbol &symbol : list) {
      if (llvm::isa<rq::FunctionPolymorph>(symbol)) {
        rq::FunctionPolymorph &poly = llvm::cast<rq::FunctionPolymorph>(symbol);
        for (rq::Function &func : poly.getDerivedInstanceSubrange()) {
          this->build(func);
        }
      }
    }
  }
}

void Builder::build(rq::Function &func) {
  if (func.getName().getKeyword() == rq::Keyword::MAIN) {
    return;
  }
  if (func.getModule() != this->getContext().getSourceModule()) {
    return;
  }
  rq::Signature &sig = rq::dereferencePtr(func.getSignaturePtr());
  llvm::FunctionType *llvm_func_type_ptr =
      llvm::cast_or_null<llvm::FunctionType>(
          this->getContext().getLlvmTypePtr(sig));
  if (llvm_func_type_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("invalid sig type");
  }
  llvm::FunctionType &llvm_func_type = rq::dereferencePtr(llvm_func_type_ptr);
  if (func.getMangledName().empty()) {
    RQ_TODO_IMPLEMENTATION(); // TODO generate mangled name
  }
  llvm::Function *llvm_func_ptr = llvm::Function::Create(
      &llvm_func_type, llvm::Function::ExternalLinkage, func.getMangledName(),
      this->getContext().getLlvmModule());
  if (llvm_func_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("failed to make llvm function");
  }
  llvm::Function &llvm_func = rq::dereferencePtr(llvm_func_ptr);
  func.setLlvmFunction(llvm_func);
  if (func.getInstructionsPtr() == nullptr) {
    return;
  }
  rq::Instruction &instructions = rq::dereferencePtr(func.getInstructionsPtr());
  llvm::BasicBlock *llvm_entry_bb_ptr = llvm::BasicBlock::Create(
      this->getContext().getLlvmContext(), "entry", &llvm_func);
  if (llvm_entry_bb_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("failed to make llvm bb");
  }
  llvm::BasicBlock &llvm_entry_bb = rq::dereferencePtr(llvm_entry_bb_ptr);
  rq::ConstantSymbol &return_type = sig.getReturnType();
  llvm::BasicBlock *llvm_exit_bb_ptr = llvm::BasicBlock::Create(
      this->getContext().getLlvmContext(), "exit", &llvm_func);
  if (llvm_exit_bb_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("failed to make llvm bb");
  }
  llvm::BasicBlock &llvm_exit_bb = rq::dereferencePtr(llvm_exit_bb_ptr);
  llvm::Argument *arg_it = llvm_func.arg_begin();
  llvm::Value *llvm_result_ptr = nullptr;
  if (return_type.getSymbol().getKind() != rq::SymbolKind::VOID_TYPE) {
    if (this->getContext().getIsSret(return_type.getSymbol())) {
      llvm_result_ptr = arg_it++;
      this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_exit_bb);
      this->getContext().getLlvmIrBuilder().CreateRetVoid();
    } else {
      llvm::Type *llvm_return_type_ptr =
          this->getContext().getLlvmTypePtr(sig.getReturnType().getSymbol());
      RQ_ASSERT(llvm_return_type_ptr != nullptr, "llvm error");
      this->getContext().getLlvmIrBuilder().SetInsertPointPastAllocas(
          &llvm_func);
      llvm_result_ptr = this->getContext().getLlvmIrBuilder().CreateAlloca(
          llvm_return_type_ptr, nullptr, "result");
      this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_exit_bb);
      this->getContext().getLlvmIrBuilder().CreateRet(llvm_result_ptr);
    }
  }
  llvm::Value *llvm_reciever_ptr = nullptr;
  if (sig.getRecieverTypePtr() != nullptr) {
    llvm_reciever_ptr = arg_it++;
  }
  // TODO arguments
  this->buildScope(func, func, instructions, llvm_entry_bb, llvm_exit_bb,
                   llvm_reciever_ptr, llvm_result_ptr, nullptr);
}

void Builder::buildScope(rq::Function &func, rq::SymbolTable &scope,
                         rq::Instruction &instructions,
                         llvm::BasicBlock &llvm_bb,
                         llvm::BasicBlock &llvm_exit_bb,
                         llvm::Value *llvm_reciever_ptr,
                         llvm::Value *llvm_result_ptr,
                         llvm::Value *llvm_out_ptr) {
  llvm::Function &llvm_func = rq::dereferencePtr(func.getLlvmFunctionPtr());
  this->getContext().getLlvmIrBuilder().SetInsertPointPastAllocas(&llvm_func);
  for (auto &kvp : scope.getSymbolListSubrange()) {
    rq::BumpPtrListRef<rq::Symbol> list = kvp.getSecond();
    for (rq::Symbol &symbol : list) {
      if (llvm::isa<rq::LocalDynamicVariable>(symbol)) {
        rq::LocalDynamicVariable &var =
            llvm::cast<rq::LocalDynamicVariable>(symbol);
        if (var.getName().getKeyword() == rq::Keyword::RESULT) {
          var.setLlvmLocation(rq::dereferencePtr(llvm_result_ptr));
          continue;
        }
        if (var.getName().getKeyword() == rq::Keyword::OUT) {
          var.setLlvmLocation(rq::dereferencePtr(llvm_out_ptr));
          continue;
        }
        llvm::Type *llvm_type_ptr =
            this->getContext().getLlvmTypePtr(var.getType().getSymbol());
        if (llvm_type_ptr == nullptr) {
          RQ_UNHANDLED_ERROR("llvm type error");
        }
        llvm::Type &llvm_type = rq::dereferencePtr(llvm_type_ptr);
        llvm::AllocaInst *llvm_alloca_ptr =
            this->getContext().getLlvmIrBuilder().CreateAlloca(
                &llvm_type, nullptr, var.getName().getText());
        if (llvm_alloca_ptr == nullptr) {
          RQ_UNHANDLED_ERROR("llvm error");
        }
        llvm::AllocaInst &alloca = rq::dereferencePtr(llvm_alloca_ptr);
        var.setLlvmLocation(alloca);
      }
    }
  }
  this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_bb);
}

} // namespace rq