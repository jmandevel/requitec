#pragma once

#include <rq/utility.hpp>

#include <llvm/Support/Allocator.h>
#include <llvm/Support/StringSaver.h>
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

  using AllocatorBase<GCSlabAllocator>::Allocate;
  using AllocatorBase<GCSlabAllocator>::Deallocate;
};

struct BumpPtrAllocator {
  using Self = rq::BumpPtrAllocator;

  llvm::BumpPtrAllocator<rq::GcSlabAllocator> _llvm_arena{};
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
  inline std::span<TypeParam> allocateZeroedArray(unsigned count) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(static_cast<void *>(ptr), 0, sizeof(TypeParam) * count);
    return std::span<TypeParam>(ptr, count);
  }
  inline llvm::StringRef saveString(llvm::Twine twine) {
    return this->_llvm_string_saver.save(twine);
  }
};

} // namespace rq