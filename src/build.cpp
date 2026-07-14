#include <rq/build.hpp>
#include <rq/context.hpp>
#include <rq/entity.hpp>
#include <rq/symbols.hpp>

#include <llvm/ADT/SmallVector.h>

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
      llvm::Value *llvm_result_value_ptr =
          this->getContext().getLlvmIrBuilder().CreateLoad(llvm_return_type_ptr,
                                                           llvm_result_ptr);
      this->getContext().getLlvmIrBuilder().CreateRet(llvm_result_value_ptr);
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
  for (rq::Entity &entity : instructions.getDottedSubrange(O::STATEMENT)) {
    rq::Instruction &instruction = llvm::cast<rq::Instruction>(entity);
    switch (instruction.getOpcode()) {
    case O::ASSIGN: {
      rq::Entity &lvalue = instruction.getAddress0();
      rq::Entity &rvalue = instruction.getAddress1();
      rq::LocationResult location = this->buildLocation(lvalue, llvm_this_ptr);
      if (location.getIsEmpty()) {
        RQ_UNHANDLED_ERROR("invalid location");
      }
      llvm::Value *llvm_rvalue_ptr = this->buildRvalue(
          func, rvalue, location.getType().getSymbol(), llvm_this_ptr);
      if (llvm_rvalue_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("invalid rvalue");
      }
      if (location.getType().getSymbol() ==
          this->getContext().acquireBooleanType()) {
        llvm::Type *llvm_bool_type_ptr = this->getContext().getLlvmTypePtr(
            this->getContext().acquireBooleanType());
        llvm_rvalue_ptr = this->getContext().getLlvmIrBuilder().CreateZExt(
            llvm_rvalue_ptr, llvm_bool_type_ptr);
      }
      this->getContext().getLlvmIrBuilder().CreateStore(
          llvm_rvalue_ptr, &location.getLlvmValue(), false);
      break;
    }
    case O::RETURN: {
      this->getContext().getLlvmIrBuilder().CreateBr(&llvm_exit_bb);
      break;
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  return false;
}

[[nodiscard]] rq::LocationResult
Builder::buildLocation(rq::Entity &lvalue, llvm::Value *llvm_this_ptr) {
  std::ignore = llvm_this_ptr;
  if (llvm::isa<rq::LocalDynamicVariable>(lvalue)) {
    rq::LocalDynamicVariable &var =
        llvm::cast<rq::LocalDynamicVariable>(lvalue);
    return rq::LocationResult(var.getType(),
                              rq::dereferencePtr(var.getLlvmLocationPtr()));
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] llvm::Value *Builder::buildRvalue(rq::Function &func,
                                                rq::Entity &rvalue,
                                                rq::Symbol &type,
                                                llvm::Value *llvm_this_ptr) {
  std::ignore = llvm_this_ptr;
  using O = rq::Opcode;
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
    } else if (type == this->getContext().acquireBooleanType()) {
      llvm::Type *llvm_type_ptr =
          this->getContext().getLlvmIrBuilder().getInt1Ty();
      if (llvm_type_ptr == nullptr) {
        return nullptr;
      }
      llvm::Type &llvm_type = rq::dereferencePtr(llvm_type_ptr);
      const bool value = word.getWord() != 0;
      return llvm::ConstantInt::get(&llvm_type, value);
    }
    RQ_UNREACHABLE();
  } else if (llvm::isa<rq::LocalDynamicVariable>(rvalue)) {
    rq::LocalDynamicVariable &var =
        llvm::cast<rq::LocalDynamicVariable>(rvalue);
    llvm::Value &llvm_location = rq::dereferencePtr(var.getLlvmLocationPtr());
    llvm::Type *llvm_type_ptr =
        this->getContext().getLlvmTypePtr(var.getType().getSymbol());
    if (llvm_type_ptr == nullptr) {
      RQ_UNHANDLED_ERROR("llvm error");
    }
    llvm::Type &llvm_type = rq::dereferencePtr(llvm_type_ptr);
    llvm::Value *llvm_value_ptr =
        this->getContext().getLlvmIrBuilder().CreateLoad(&llvm_type,
                                                         &llvm_location);
    if (llvm_value_ptr == nullptr) {
      RQ_UNHANDLED_ERROR("llvm error");
    }
    if (var.getType().getSymbol() == this->getContext().acquireBooleanType()) {
      // NOTE: booleans are stored as byte depth integers for performance
      // reasons. to do actual logical operations, need to turn into i1.
      // https://llvm.org/docs/Frontend/PerformanceTips.html#avoid-loads-and-stores-of-non-byte-sized-types
      // archive:
      // https://web.archive.org/web/20260628190150/https://llvm.org/docs/Frontend/PerformanceTips.html#avoid-loads-and-stores-of-non-byte-sized-types
      // while bool is temp its i1, while its stored, its i8. on load, load as
      // i8 then convert to i1. on store, convert to i8 and store.
      const unsigned byte_depth = this->getContext().getByteDepth();
      llvm::ConstantInt *llvm_zero_ptr =
          this->getContext().getLlvmIrBuilder().getIntN(byte_depth, 0);
      llvm_value_ptr = this->getContext().getLlvmIrBuilder().CreateICmpNE(
          llvm_value_ptr, llvm_zero_ptr);
      if (llvm_value_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("llvm error");
      }
    }
    llvm::Value &llvm_value = rq::dereferencePtr(llvm_value_ptr);
    return &llvm_value;
  } else if (llvm::isa<rq::Instruction>(rvalue)) {
    rq::Instruction &inst = llvm::cast<rq::Instruction>(rvalue);
    switch (inst.getOpcode()) {
    case O::LOGICAL_COMPLEMENT: {
      rq::Entity &address0 = inst.getAddress0();
      llvm::Value *llvm_value_ptr =
          this->buildRvalue(func, address0, type, llvm_this_ptr);
      if (llvm_value_ptr == nullptr) {
        return nullptr;
      }
      llvm::ConstantInt *llvm_zero_ptr =
          this->getContext().getLlvmIrBuilder().getInt1(0);
      llvm_value_ptr = this->getContext().getLlvmIrBuilder().CreateICmpEQ(
          llvm_value_ptr, llvm_zero_ptr);
      return llvm_value_ptr;
    }
    case O::LOGICAL_AND:
      [[fallthrough]];
    case O::LOGICAL_OR:
      [[fallthrough]];
    case O::ADD:
      [[fallthrough]];
    case O::SUBTRACT:
      [[fallthrough]];
    case O::MULTIPLY:
      [[fallthrough]];
    case O::DIVIDE:
      [[fallthrough]];
    case O::MODULUS: {
      llvm::Value *llvm_prev_ptr = nullptr;
      for (rq::Entity &operand : inst.getDottedSubrange(inst.getOpcode())) {
        llvm::Value *llvm_cur_ptr =
            this->buildRvalue(func, operand, type, llvm_this_ptr);
        if (llvm_cur_ptr == nullptr) {
          return nullptr;
        }
        llvm::Value &llvm_cur = rq::dereferencePtr(llvm_cur_ptr);
        if (llvm_prev_ptr == nullptr) {
          llvm_prev_ptr = &llvm_cur;
          continue;
        }
        llvm::Value &llvm_prev = rq::dereferencePtr(llvm_prev_ptr);
        switch (inst.getOpcode()) {
        case O::LOGICAL_AND: {
          llvm_prev_ptr =
              this->getContext().getLlvmIrBuilder().CreateLogicalAnd(&llvm_prev,
                                                                     &llvm_cur);
          break;
        case O::LOGICAL_OR: {
          llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateLogicalOr(
              &llvm_prev, &llvm_cur);
          break;
        }
        case O::ADD: {
          if (type.getIsIntegerType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateAdd(
                &llvm_prev, &llvm_cur);
            break;
          } else if (type.getIsFloatType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateFAdd(
                &llvm_prev, &llvm_cur);
            break;
          }
          RQ_UNREACHABLE();
        }
        case O::SUBTRACT: {
          if (type.getIsIntegerType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateAdd(
                &llvm_prev, &llvm_cur);
            break;
          } else if (type.getIsFloatType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateFSub(
                &llvm_prev, &llvm_cur);
            break;
          }
          RQ_UNREACHABLE();
        }
        case O::MULTIPLY: {
          if (type.getIsIntegerType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateMul(
                &llvm_prev, &llvm_cur);
            break;
          } else if (type.getIsFloatType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateFMul(
                &llvm_prev, &llvm_cur);
            break;
          }
          RQ_UNREACHABLE();
        }
        case O::DIVIDE: {
          if (type.getIsIntegerType()) {
            if (type.getIsSignedType()) {
              llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateSDiv(
                  &llvm_prev, &llvm_cur);
              break;
            } else if (type.getIsUnsignedType()) {
              llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateUDiv(
                  &llvm_prev, &llvm_cur);
              break;
            }
            RQ_UNREACHABLE();
          } else if (type.getIsFloatType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateFDiv(
                &llvm_prev, &llvm_cur);
            break;
          }
          RQ_UNREACHABLE();
        }
        case O::MODULUS: {
          if (type.getIsIntegerType()) {
            if (type.getIsSignedType()) {
              llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateSRem(
                  &llvm_prev, &llvm_cur);
              break;
            } else if (type.getIsUnsignedType()) {
              llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateURem(
                  &llvm_prev, &llvm_cur);
              break;
            }
            RQ_UNREACHABLE();
          } else if (type.getIsFloatType()) {
            llvm_prev_ptr = this->getContext().getLlvmIrBuilder().CreateFRem(
                &llvm_prev, &llvm_cur);
            break;
          }
          RQ_UNREACHABLE();
        }
        default:
          RQ_UNREACHABLE();
        }
        }
      }
      llvm::Value &llvm_outer = rq::dereferencePtr(llvm_prev_ptr);
      return &llvm_outer;
    }
    case O::NEGATE: {
      rq::Entity &address0 = inst.getAddress0();
      llvm::Value *llvm_rvalue_ptr =
          this->buildRvalue(func, address0, type, llvm_this_ptr);
      if (llvm_rvalue_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("llvm error");
      }
      llvm::Value &llvm_rvalue = rq::dereferencePtr(llvm_rvalue_ptr);
      llvm::Value *llvm_negate_ptr = nullptr;
      if (type.getIsIntegerType()) {
        llvm_negate_ptr =
            this->getContext().getLlvmIrBuilder().CreateNeg(&llvm_rvalue);
      } else if (type.getIsFloatType()) {
        llvm_negate_ptr =
            this->getContext().getLlvmIrBuilder().CreateFNeg(&llvm_rvalue);
      } else {
        RQ_UNREACHABLE();
      }
      if (llvm_negate_ptr == nullptr) {
        RQ_UNHANDLED_ERROR("llvm error");
      }
      llvm::Value &llvm_negate = rq::dereferencePtr(llvm_negate_ptr);
      return &llvm_negate;
    }
    case O::LOGICAL_AND_WITH_SHORTCIRCUIT: {
      [[fallthrough]];
    }
    case O::LOGICAL_OR_WITH_SHORTCIRCUIT: {
      llvm::StringRef merge_name;
      llvm::StringRef next_bb_name;
      llvm::StringRef phi_name;
      if (inst.getOpcode() == O::LOGICAL_AND_WITH_SHORTCIRCUIT) {
        merge_name = "and_with_sc.merge";
        next_bb_name = "and_with_sc.operand";
        phi_name = "and_with_sc.result";
      } else if (inst.getOpcode() == O::LOGICAL_OR_WITH_SHORTCIRCUIT) {
        merge_name = "or_with_sc.merge";
        next_bb_name = "or_with_sc.operand";
        phi_name = "or_with_sc.result";
      } else {
        RQ_UNREACHABLE();
      }
      llvm::BasicBlock *llvm_merge_bb_ptr =
          llvm::BasicBlock::Create(this->getContext().getLlvmContext(),
                                   merge_name, func.getLlvmFunctionPtr());
      if (llvm_merge_bb_ptr == nullptr) {
        return nullptr;
      }
      llvm::BasicBlock &llvm_merge_bb = rq::dereferencePtr(llvm_merge_bb_ptr);
      llvm::SmallVector<std::pair<llvm::Value *, llvm::BasicBlock *>, 2>
          incoming_ptrs{};
      for (rq::Entity &operand : inst.getDottedSubrange(inst.getOpcode())) {
        if (!incoming_ptrs.empty()) {
          llvm::Value *llvm_prev_ptr = incoming_ptrs.back().first;
          llvm::Value &llvm_prev = rq::dereferencePtr(llvm_prev_ptr);
          llvm::BasicBlock *llvm_prev_bb_ptr =
              this->getContext().getLlvmIrBuilder().GetInsertBlock();
          llvm::BasicBlock *llvm_next_bb_ptr =
              llvm::BasicBlock::Create(this->getContext().getLlvmContext(),
                                       next_bb_name, func.getLlvmFunctionPtr());
          if (llvm_next_bb_ptr == nullptr) {
            return nullptr;
          }
          llvm::BasicBlock &llvm_prev_bb = rq::dereferencePtr(llvm_prev_bb_ptr);
          llvm::BasicBlock &llvm_next_bb = rq::dereferencePtr(llvm_next_bb_ptr);
          if (inst.getOpcode() == O::LOGICAL_AND_WITH_SHORTCIRCUIT) {
            this->getContext().getLlvmIrBuilder().CreateCondBr(
                &llvm_prev, &llvm_next_bb, &llvm_merge_bb);
          } else if (inst.getOpcode() == O::LOGICAL_OR_WITH_SHORTCIRCUIT) {
            this->getContext().getLlvmIrBuilder().CreateCondBr(
                &llvm_prev, &llvm_merge_bb, &llvm_next_bb);
          } else {
            RQ_UNREACHABLE();
          }
          incoming_ptrs.back().second = &llvm_prev_bb;
          this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_next_bb);
        }
        rq::Symbol &boolean_ty = this->getContext().acquireBooleanType();
        llvm::Value *llvm_next_ptr =
            this->buildRvalue(func, operand, boolean_ty, llvm_this_ptr);
        llvm::Value &llvm_next = rq::dereferencePtr(llvm_next_ptr);
        incoming_ptrs.push_back({&llvm_next, nullptr});
      }
      this->getContext().getLlvmIrBuilder().CreateBr(&llvm_merge_bb);
      incoming_ptrs.back().second =
          this->getContext().getLlvmIrBuilder().GetInsertBlock();
      this->getContext().getLlvmIrBuilder().SetInsertPoint(&llvm_merge_bb);
      llvm::Type *llvm_bool_ty_ptr =
          llvm::Type::getInt1Ty(this->getContext().getLlvmContext());
      if (llvm_bool_ty_ptr == nullptr) {
        return nullptr;
      }
      llvm::Type &llvm_bool_ty = rq::dereferencePtr(llvm_bool_ty_ptr);
      llvm::PHINode *llvm_phi_ptr =
          this->getContext().getLlvmIrBuilder().CreatePHI(
              &llvm_bool_ty, incoming_ptrs.size(), phi_name);
      if (llvm_phi_ptr == nullptr) {
        return nullptr;
      }
      llvm::PHINode &llvm_phi = rq::dereferencePtr(llvm_phi_ptr);
      for (auto &[llvm_val_ptr, llvm_bb_ptr] : incoming_ptrs) {
        RQ_ASSERT(llvm_bb_ptr != nullptr, "missing bb");
        llvm_phi.addIncoming(llvm_val_ptr, llvm_bb_ptr);
      }
      return &llvm_phi;
    }
    case O::GREATER:
      [[fallthrough]];
    case O::LESS:
      [[fallthrough]];
    case O::GREATER_EQUAL:
      [[fallthrough]];
    case O::LESS_EQUAL:
      [[fallthrough]];
    case O::EQUAL:
      [[fallthrough]];
    case O::NOT_EQUAL: {
      rq::Symbol &type = llvm::cast<rq::Symbol>(inst.getAddress0());
      rq::Instruction &pair = llvm::cast<rq::Instruction>(inst.getAddress1());
      rq::Entity &rvalue0 = pair.getAddress0();
      rq::Entity &rvalue1 = pair.getAddress1();
      llvm::Value *llvm_rvalue0_ptr =
          this->buildRvalue(func, rvalue0, type, llvm_this_ptr);
      if (llvm_rvalue0_ptr == nullptr) {
        return nullptr;
      }
      llvm::Value &llvm_rvalue0 = rq::dereferencePtr(llvm_rvalue0_ptr);
      llvm::Value *llvm_rvalue1_ptr =
          this->buildRvalue(func, rvalue1, type, llvm_this_ptr);
      if (llvm_rvalue1_ptr == nullptr) {
        return nullptr;
      }
      llvm::Value &llvm_rvalue1 = rq::dereferencePtr(llvm_rvalue1_ptr);
      llvm::Type *llvm_ty_ptr = this->getContext().getLlvmTypePtr(type);
      if (llvm_ty_ptr == nullptr) {
        return nullptr;
      }
      switch (inst.getOpcode()) {
      case O::GREATER: {
        if (type.getIsIntegerType()) {
          if (type.getIsSignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpSGT(
                &llvm_rvalue0, &llvm_rvalue1);
          } else if (type.getIsUnsignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpUGT(
                &llvm_rvalue0, &llvm_rvalue1);
          } else {
            RQ_UNREACHABLE();
          }
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpOGT(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      case O::LESS: {
        if (type.getIsIntegerType()) {
          if (type.getIsSignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpSLT(
                &llvm_rvalue0, &llvm_rvalue1);
          } else if (type.getIsUnsignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpUGT(
                &llvm_rvalue0, &llvm_rvalue1);
          } else {
            RQ_UNREACHABLE();
          }
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpOGT(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      case O::GREATER_EQUAL: {
        if (type.getIsIntegerType()) {
          if (type.getIsSignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpSGE(
                &llvm_rvalue0, &llvm_rvalue1);
          } else if (type.getIsUnsignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpUGE(
                &llvm_rvalue0, &llvm_rvalue1);
          } else {
            RQ_UNREACHABLE();
          }
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpOGE(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      case O::LESS_EQUAL: {
        if (type.getIsIntegerType()) {
          if (type.getIsSignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpSLE(
                &llvm_rvalue0, &llvm_rvalue1);
          } else if (type.getIsUnsignedType()) {
            return this->getContext().getLlvmIrBuilder().CreateICmpULE(
                &llvm_rvalue0, &llvm_rvalue1);
          } else {
            RQ_UNREACHABLE();
          }
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpOGE(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      case O::EQUAL: {
        if (type.getIsIntegerType() ||
            type == this->getContext().acquireBooleanType()) {
          return this->getContext().getLlvmIrBuilder().CreateICmpEQ(
              &llvm_rvalue0, &llvm_rvalue1);
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpOEQ(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      case O::NOT_EQUAL: {
        if (type.getIsIntegerType() ||
            type == this->getContext().acquireBooleanType()) {
          return this->getContext().getLlvmIrBuilder().CreateICmpNE(
              &llvm_rvalue0, &llvm_rvalue1);
        } else if (type.getIsFloatType()) {
          return this->getContext().getLlvmIrBuilder().CreateFCmpONE(
              &llvm_rvalue0, &llvm_rvalue1);
        } else {
          RQ_UNREACHABLE();
        }
      }
      default:
        RQ_UNREACHABLE();
      }
    }
    default:
      RQ_UNREACHABLE();
    }
  }
  RQ_UNREACHABLE();
}

} // namespace rq