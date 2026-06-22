#pragma once

#include <rq/utility.hpp>

#include <llvm/IR/IRBuilder.h>

#include <memory>

namespace rq {

struct Context;
struct Module;
struct Function;

struct LlvmIrBuilder final {
  using Self = rq::LlvmIrBuilder;

  rq::Context *_context_ptr;
  bool _is_ok{true};


  LlvmIrBuilder(rq::Context &context)
      : _context_ptr(&context) {
      }
  LlvmIrBuilder(const Self &) = delete;
  LlvmIrBuilder(Self &&) = delete;
  ~LlvmIrBuilder() = default;
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
  void build(rq::Function& function);
};

} // namespace rq