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
  if (return_type.getSymbol().getKind() == rq::SymbolKind::VOID_TYPE) {
    this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_exit_bb);
    this->getContext().getLlvmIrBuilder().CreateRetVoid();
  } else {
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
  llvm::Value *llvm_this_ptr = nullptr;
  if (sig.getRecieverTypePtr() != nullptr) {
    llvm_this_ptr = arg_it++;
  }
  // TODO arguments
  const bool has_jump =
      this->buildScope(func, func, instructions, llvm_entry_bb, llvm_exit_bb,
                       llvm_this_ptr, llvm_result_ptr, nullptr);
  if (!has_jump) {
    this->getContext().getLlvmIrBuilder().CreateBr(&llvm_exit_bb);
  }
}

[[nodiscard]] bool
Builder::buildScope(rq::Function &func, rq::SymbolTable &scope,
                    rq::Instruction &instructions, llvm::BasicBlock &llvm_bb,
                    llvm::BasicBlock &llvm_exit_bb, llvm::Value *llvm_this_ptr,
                    llvm::Value *llvm_result_ptr, llvm::Value *llvm_out_ptr) {
  std::ignore = llvm_exit_bb;
  using O = rq::Opcode;
  using K = rq::Keyword;
  llvm::Function &llvm_func = rq::dereferencePtr(func.getLlvmFunctionPtr());
  this->getContext().getLlvmIrBuilder().SetInsertPointPastAllocas(&llvm_func);
  for (auto &kvp : scope.getSymbolListSubrange()) {
    rq::BumpPtrListRef<rq::Symbol> list = kvp.getSecond();
    for (rq::Symbol &symbol : list) {
      if (llvm::isa<rq::LocalDynamicVariable>(symbol)) {
        rq::LocalDynamicVariable &var =
            llvm::cast<rq::LocalDynamicVariable>(symbol);
        if (var.getName().getKeyword() == K::RESULT) {
          var.setLlvmLocation(rq::dereferencePtr(llvm_result_ptr));
          continue;
        }
        if (var.getName().getKeyword() == K::OUT) {
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
  for (rq::Entity &entity : instructions.getDottedSubrange<O::STATEMENT>()) {
    rq::Instruction &instruction = llvm::cast<rq::Instruction>(entity);
    switch (instruction.getOpcode()) {
    case O::ASSIGN: {
      rq::Entity &lvalue = instruction.getAddress0();
      rq::Entity &rvalue = instruction.getAddress1();
      llvm::Value *llvm_location_ptr =
          this->buildLocation(lvalue, llvm_this_ptr);
      if (llvm_location_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("invalid location");
      }
      llvm::Value *llvm_rvalue_ptr = this->buildRvalue(rvalue, llvm_this_ptr);
      if (llvm_rvalue_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("invalid rvalue");
      }
      this->getContext().getLlvmIrBuilder().CreateStore(
          llvm_rvalue_ptr, llvm_location_ptr, false);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  return false;
}

[[nodiscard]] llvm::Value *Builder::buildLocation(rq::Entity &lvalue,
                                                  llvm::Value *llvm_this_ptr) {}

[[nodiscard]] llvm::Value *Builder::buildRvalue(rq::Entity &rvalue,
                                                rq::Symbol &type,
                                                llvm::Value *llvm_this_ptr) {
  std::ignore = llvm_this_ptr;
  using C = rq::ConstantKind;
  if (llvm::isa<rq::ConstantWord>(rvalue)) {
    rq::ConstantWord &word = llvm::cast<rq::ConstantWord>(rvalue);
    if (type.getIsIntegerType()) {
      llvm::Type *llvm_type_ptr = this->getContext().getLlvmTypePtr(type);
      if (llvm_type_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("llvm error");
      }
      llvm::Type &llvm_type = rq::dereferencePtr(llvm_type_ptr);
      return llvm::ConstantInt::get(&llvm_type, word.getWord());
    } else if (type.getIsFloatType()) {
      const llvm::fltSemantics &llvm_semantics =
          this->getContext().getLlvmFltSemantics(type.getKind());
      llvm::APFloat llvm_float = word.getAsFloat(llvm_semantics);
      return llvm::ConstantFP::get(this->getContext().getLlvmContext(),
                                   llvm_float);
    }
  }
  RQ_UNREACHABLE();
}

} // namespace rq