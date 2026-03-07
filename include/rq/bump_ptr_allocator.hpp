#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/StringSaver.h>

#include <cstring>

namespace rq {

struct BumpPtrAllocator {
  using Self = rq::BumpPtrAllocator;

  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};

  BumpPtrAllocator() = default;
  BumpPtrAllocator(const Self &) = delete;
  BumpPtrAllocator(Self &&) = delete;
  ~BumpPtrAllocator() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateValue(ArgNParam &&...arg_n) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline llvm::ArrayRef<TypeParam> allocateZeroedArray(unsigned count) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(ptr, 0, sizeof(TypeParam) * count);
    return llvm::ArrayRef<TypeParam>(ptr, count);
  }
  inline llvm::StringRef saveString(llvm::Twine twine) {
    return this->_llvm_string_saver.save(twine);
  }
};

} // namespace rq