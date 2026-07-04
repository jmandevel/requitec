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
struct ConstantSymbol;

struct LocationResult final {
  using Self = rq::LocationResult;

  rq::ConstantSymbol *_type_ptr{nullptr};
  llvm::Value *_llvm_value_ptr{nullptr};

  LocationResult() = default;
  LocationResult(rq::ConstantSymbol &type, llvm::Value &llvm_value)
      : _type_ptr(&type), _llvm_value_ptr(&llvm_value) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_type_ptr == nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ConstantSymbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstantSymbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Value& getLlvmValue() const {
    return rq::dereferencePtr(this->_llvm_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Value &getLlvmValue() {
    return rq::dereferencePtr(this->_llvm_value_ptr);
  }
};

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
  [[nodiscard]] bool
  buildScope(rq::Function &func, rq::SymbolTable &scope,
             rq::Instruction &instructions, llvm::BasicBlock &llvm_bb,
             llvm::BasicBlock &llvm_exit_bb, llvm::Value *llvm_this_ptr,
             llvm::Value *llvm_result_ptr, llvm::Value *llvm_out_ptr);
  [[nodiscard]] rq::LocationResult buildLocation(rq::Entity &lvalue,
                                                 llvm::Value *llvm_this_ptr);
  [[nodiscard]] llvm::Value *buildRvalue(rq::Function& func, rq::Entity &rvalue, rq::Symbol &type,
                                         llvm::Value *llvm_this_ptr);
};

} // namespace rq