#include <rq/build.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/symbols.hpp>

namespace rq {

void LlvmIrBuilder::buildLlvmIr() {
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

void LlvmIrBuilder::build(rq::Function &func) {
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
  llvm::BasicBlock *llvm_bb_ptr = llvm::BasicBlock::Create(
      this->getContext().getLlvmContext(), "entry", &llvm_func);
  if (llvm_bb_ptr == nullptr) {
    RQ_UNHANDLED_ERROR("failed to make llvm bb");
  }
  llvm::BasicBlock &llvm_bb = rq::dereferencePtr(llvm_bb_ptr);
  this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_bb);
  // TODO create arguments
  // TODO create result store
  this->buildScope(func, llvm_bb);
}

} // namespace rq