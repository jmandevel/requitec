#pragma once

#include <rq/utility.hpp>

#include <llvm/IR/IRBuilder.h>

#include <memory>

namespace rq {

struct Context;
struct Module;
struct SymbolTable;
struct Function;
struct Instruction;
struct Entity;
struct Symbol;

struct Builder final {
  using Self = rq::Builder;

  rq::Context *_context_ptr;
  bool _is_ok{true};

  Builder(rq::Context &context) : _context_ptr(&context) {}
  Builder(const Self &) = delete;
  Builder(Self &&) = delete;
  ~Builder() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return rq::dereferencePtr(this->_context_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  void buildLlvmIr();
  void build(rq::Function &function);
  [[nodiscard]] bool buildScope(rq::Function &func, rq::SymbolTable &scope,
                  rq::Instruction &instructions, llvm::BasicBlock &llvm_bb,
                  llvm::BasicBlock &llvm_exit_bb,
                  llvm::Value *llvm_this_ptr, llvm::Value *llvm_result_ptr,
                  llvm::Value *llvm_out_ptr);
  [[nodiscard]] llvm::Value* buildLocation(rq::Entity& lvalue, llvm::Value* llvm_this_ptr);
  [[nodiscard]] llvm::Value* buildRvalue(rq::Entity& rvalue, rq::Symbol& type, llvm::Value* llvm_this_ptr);
};

} // namespace rq