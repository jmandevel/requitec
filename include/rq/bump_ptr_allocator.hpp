#pragma once

#include <rq/utility.hpp>

#include <llvm/Support/Allocator.h>
#include <llvm/ADT/DenseSet.h>
#include <gc/gc.h>

#include <cstring>
#include <span>

namespace rq {

struct Top;

class GcSlabAllocator : public llvm::AllocatorBase<rq::GcSlabAllocator> {
  using Self = rq::GcSlabAllocator;

  void *Allocate(std::size_t size, std::size_t alignment) {
    RQ_ASSERT(alignment <= 16, "slab alignment larger than GC garuntees");
    void* slab_ptr = GC_MALLOC_UNCOLLECTABLE(size);
    if (!slab_ptr) {
      llvm::report_bad_alloc_error("GC slab allocation failed");
    }
    return slab_ptr;
  }

  void Deallocate(const void* slab_ptr, std::size_t size, std::size_t alignment) {
    std::ignore = size;
    std::ignore = alignment;
    GC_FREE(const_cast<void*>(slab_ptr)); // nasty const cast
  }

  using AllocatorBase<Self>::Allocate;
  using AllocatorBase<Self>::Deallocate;
};

template <typename FlagsParam> struct is_gc_atomic final : std::false_type {};

template <typename FlagsParam>
constexpr bool is_gc_atomic_v = rq::is_flags<FlagsParam>::value;

template <typename FlagsParam>
concept gc_atomic = rq::is_gc_atomic<FlagsParam>::value;

struct BumpPtrAllocator {
  using Self = rq::BumpPtrAllocator;

  llvm::BumpPtrAllocatorImpl<rq::GcSlabAllocator> _llvm_arena{};
  llvm::DenseSet<llvm::StringRef> _unique_strings{};

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
  inline std::span<TypeParam> allocateZeroedArray(unsigned count) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(static_cast<void *>(ptr), 0, sizeof(TypeParam) * count);
    return std::span<TypeParam>(ptr, count);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateGcValue(ArgNParam &&...arg_n) {
    TypeParam* ptr = nullptr;
    if constexpr (rq::gc_atomic<TypeParam>) {
      ptr = GC_MALLOC_ATOMIC(sizeof(TypeParam));
    } else {
      ptr = GC_MALLOC(sizeof(TypeParam));
    }
    if (ptr == nullptr) {
      llvm::report_bad_alloc_error("gc allocation failure");
    }
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);    
  }
  inline llvm::StringRef saveString(llvm::StringRef ref) {
    auto it = this->_unique_strings.find(ref);
    if (it != this->_unique_strings.end()) {
      return *it;
    }
    char*ptr = this->_llvm_arena.Allocate<char>(ref.size());
    std::memcpy(ptr, ref.data(), ref.size());
    llvm::StringRef ret(ptr, ref.size());
    this->_unique_strings.insert(ret);
    return ret;
  }
};

} // namespace rq