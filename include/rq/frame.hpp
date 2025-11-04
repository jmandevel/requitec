#pragma once

#include <rq/terms.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/StringSaver.h>

namespace rq {

struct Token;
struct Expression;

struct Frame final {
  using Self = rq::Frame;

  llvm::BumpPtrAllocator _llvm_arena;
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs;
  rq::Table _table;

  Frame() = default;
  Frame(const Self &) = delete;
  Frame(Self &&) = delete;
  ~Frame() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam &allocateValue(ArgNParam... arg_n) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef saveString(llvm::Twine twine) {
    llvm::StringRef saved_string = this->_llvm_string_saver.save(twine);
    return saved_string;
  }
  [[nodiscard]] rq::Expression &acquireExpression();
  RQ_ALWAYS_INLINE void discardExpression(rq::Expression &expression) {
    this->_unused_expression_ptrs.emplace_back(&expression);
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  void replaceWithRecursiveCopy(rq::Expression &initial,
                                rq::Expression &replacement);
};

} // namespace rq