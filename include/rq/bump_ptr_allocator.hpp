#pragma once

#include <rq/utility.hpp>

#include <gc/gc.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/Support/Allocator.h>

#include <cstring>
#include <new>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rq {

struct Top;

struct NodeListItem final : llvm::FoldingSetNode {
  using Self = rq::NodeListItem;

  void *const item_ptr;
  void *const next;

  NodeListItem(void *item_ptr, void *next) : item_ptr(item_ptr), next(next) {}
  NodeListItem(const Self &) = delete;
  NodeListItem(Self &&) = delete;
  ~NodeListItem() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  static inline void Profile(llvm::FoldingSetNodeID &inout_id,
                             const void *item_ptr, const void *next) {
    inout_id.AddPointer(item_ptr);
    inout_id.AddPointer(next);
  }
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &inout_id) const {
    Self::Profile(inout_id, this->item_ptr, this->next);
  }
};

class GcSlabAllocator : public llvm::AllocatorBase<rq::GcSlabAllocator> {
  using Self = rq::GcSlabAllocator;

public:
  void *Allocate(std::size_t size, std::size_t alignment) {
    RQ_ASSERT(alignment <= 16, "slab alignment larger than GC garuntees");
    std::ignore = alignment;
    void *slab_ptr = GC_MALLOC_UNCOLLECTABLE(size);
    if (!slab_ptr) {
      llvm::report_bad_alloc_error("GC slab allocation failed");
    }
    return slab_ptr;
  }

  void Deallocate(const void *slab_ptr, std::size_t size,
                  std::size_t alignment) {
    std::ignore = size;
    std::ignore = alignment;
    GC_FREE(const_cast<void *>(slab_ptr)); // nasty const cast
  }

  using AllocatorBase<Self>::Allocate;
  using AllocatorBase<Self>::Deallocate;
};

template <typename FlagsParam> struct is_gc_atomic final : std::false_type {};

template <typename FlagsParam>
constexpr bool is_gc_atomic_v = rq::is_gc_atomic<FlagsParam>::value;

template <typename FlagsParam>
concept gc_atomic = rq::is_gc_atomic<FlagsParam>::value;

#define RQ_DEFINE_GC_ATOMIC(TYPE)                                              \
  template <> struct is_gc_atomic<TYPE> final : std::true_type {}

struct BumpPtrAllocator {
  using Self = rq::BumpPtrAllocator;

  llvm::BumpPtrAllocatorImpl<rq::GcSlabAllocator> _llvm_arena{};
  llvm::DenseSet<llvm::StringRef> _unique_strings{};
  llvm::FoldingSet<rq::NodeListItem> _unique_node_list_items{};

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
  inline rq::NodeListItem &getNodeListItem(void *item_ptr, void *next) {
    llvm::FoldingSetNodeID id;
    rq::NodeListItem::Profile(id, item_ptr, next);
    void *insert_pos = nullptr;
    rq::NodeListItem *existing_ptr =
        this->_unique_node_list_items.FindNodeOrInsertPos(id, insert_pos);
    if (existing_ptr != nullptr) {
      return rq::dereferencePtr(existing_ptr);
    }
    rq::NodeListItem &node =
        this->allocateValue<rq::NodeListItem>(item_ptr, next);
    this->_unique_node_list_items.InsertNode(&node, insert_pos);
    return node;
  }
  template <typename TypeParam, typename... ArgNParam>
  inline std::span<TypeParam> allocateZeroedArray(unsigned count) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(count);
    std::memset(static_cast<void *>(ptr), 0, sizeof(TypeParam) * count);
    return std::span<TypeParam>(ptr, count);
  }
  template <typename TypeParam, typename... ArgNParam>
  inline TypeParam &allocateGcValue(ArgNParam &&...arg_n) {
    TypeParam *ptr = nullptr;
    if constexpr (rq::gc_atomic<TypeParam>) {
      ptr = static_cast<TypeParam *>(GC_MALLOC_ATOMIC(sizeof(TypeParam)));
    } else {
      ptr = static_cast<TypeParam *>(GC_MALLOC(sizeof(TypeParam)));
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
    char *ptr = this->_llvm_arena.Allocate<char>(ref.size());
    std::memcpy(ptr, ref.data(), ref.size());
    llvm::StringRef ret(ptr, ref.size());
    this->_unique_strings.insert(ret);
    return ret;
  }
};

template <typename TypeParam = void, typename SetParam, typename... ArgNParam>
[[nodiscard]] inline auto &acquire(rq::BumpPtrAllocator &allocator,
                                   llvm::FoldingSet<SetParam> &set,
                                   ArgNParam &&...arg_n) {
  using Type =
      std::conditional_t<std::is_void_v<TypeParam>, SetParam, TypeParam>;
  static_assert(std::is_base_of_v<SetParam, Type>);
  llvm::FoldingSetNodeID id;
  Type::Profile(id, arg_n...);
  void *insert_pos = nullptr;
  SetParam *existing_ptr = set.FindNodeOrInsertPos(id, insert_pos);
  if (existing_ptr != nullptr) {
    return *static_cast<Type *>(existing_ptr);
  }
  Type &node = allocator.allocateValue<Type>(std::forward<ArgNParam>(arg_n)...);
  set.InsertNode(&node, insert_pos);
  return node;
}

} // namespace rq
