#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/MemoryBufferRef.h>

#include <string>
#include <utility>

namespace rq {

struct Expression;

struct Module final {
  using Self = rq::Module;

  llvm::MemoryBufferRef _llvm_buffer_ref{};
  std::string _path{};
  rq::Expression *_expression_ptr = nullptr;

  Module() = default;
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLlvmBuffer() const {
    return !this->_path.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    RQ_ASSERT(!this->_path.empty(), "has no path");
    return this->_path;
  }
  llvm::StringRef getSourceText() const {
    RQ_ASSERT(this->getHasLlvmBuffer(), "does not have llvm file buffer");
    return this->_llvm_buffer_ref.getBuffer();
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

} // namespace rq