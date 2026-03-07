#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/Hashing.h>

namespace rq {

// A map backed by a binary search tree ordered by std::hash of the key.
// Nodes are allocated using a llvm::BumpPtrAllocator.

template <typename ValueParam> struct BumpPtrMapNode {
  using Key = llvm::StringRef;
  using Value = ValueParam;
  using Hash = std::hash<Key>;
  using Self = rq::BumpPtrMapNode<Value>;

  Key key;
  Value value;
  Hash hash;
  Self *left_ptr;
  Self *right_ptr;
  Self *next_ptr;
};

template <typename ValueParam> struct BumpPtrMapIterator final {
  using Value = ValueParam;
  using Self = rq::BumpPtrMapIterator<Value>;
  using Node = rq::BumpPtrMapNode<Value>;
  using value_type = Value;
  using reference = Value &;
  using pointer = Value *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Node *_node_ptr = nullptr;

  BumpPtrMapIterator() = default;
  explicit BumpPtrMapIterator(Node *node_ptr) : _node_ptr(node_ptr) {}
  BumpPtrMapIterator(const Self &) = default;
  BumpPtrMapIterator(Self &&) = default;
  ~BumpPtrMapIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_node_ptr == it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_node_ptr != it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Value &operator*() {
    return rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value &operator*() const {
    return rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Value *operator->() {
    return &rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value *operator->() const {
    return &rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_node_ptr == nullptr;
  }
};

template <typename ValueParam> struct ConstBumpPtrMapIterator final {
  using Value = ValueParam;
  using Self = rq::BumpPtrMapIterator<Value>;
  using Node = rq::BumpPtrMapNode<Value>;
  using value_type = const rq::Expression;
  using reference = const rq::Expression &;
  using pointer = rq::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const Node *_node_ptr = nullptr;

  ConstBumpPtrMapIterator() = default;
  explicit ConstBumpPtrMapIterator(const Node *node_ptr)
      : _node_ptr(node_ptr) {}
  ConstBumpPtrMapIterator(const Self &) = default;
  ConstBumpPtrMapIterator(Self &&) = default;
  ~ConstBumpPtrMapIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_node_ptr == it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_node_ptr != it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value &operator*() const {
    return rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value *operator->() const {
    return &rq::dereferencePtr(this->_node_ptr).value;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_node_ptr == nullptr;
  }
};

// Map uses closed addressing and uses binary trees for handling collisions.
// The initial hash table array is allocated with llvm::BumpPtrAllocator from
// rq::Context. The root node exists in the table. The initial hash table does
// not have a load factor (it does not resize). The child nodes nodes in the
// trees per table slot are also allocated using the llvm::BumpPtrAlloctor from
// rq::Context.

template <typename ValueParam> struct BumpPtrMap {
  using Key = llvm::StringRef;
  using Value = ValueParam;
  using Self = rq::BumpPtrMap<Value>;
  using Node = rq::BumpPtrMapNode<Value>;
  using Iterator = rq::BumpPtrMapIterator<Value>;
  using ConstIterator = rq::ConstBumpPtrMapIterator<Value>;

  llvm::ArrayRef<Node> _hash_table;
  std::size_t _count{0};
  Node *_begin_ptr{nullptr};

  explicit BumpPtrMap(rq::BumpPtrAllocator &allocator, unsigned bucket_count)
      : _hash_table(allocator.allocateZeroedArray<Node>(bucket_count)) {}
  BumpPtrMap(const BumpPtrMap &) = delete;
  BumpPtrMap(BumpPtrMap &&) = delete;
  BumpPtrMap &operator=(const BumpPtrMap &) = delete;
  BumpPtrMap &operator=(BumpPtrMap &&) = delete;
  ~BumpPtrMap() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const { return _count; }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const { return _count == 0; }
  inline Value &getOrInsert(rq::BumpPtrAllocator &allocator, llvm::StringRef name) {
    const std::size_t hash = llvm::hash_value(name);
    const std::size_t bucket_i = hash % this->_hash_table.size();
    Node& bucket = this->_hash_table[bucket_i];
    if (bucket.key.empty()) {
      bucket.key = name;
      bucket.hash = hash;
      return bucket.value;
    }
    if (bucket.key == key) {
      return bucket.value;
    }
    Node* cur_ptr = &bucket;
    while (true) {
      Node& cur = rq::dereferencePtr(cur_ptr);
      if (hash > cur.hash) {
        if (cur.right_ptr == nullptr) {
          Node &new_node = allocator.allocateValue<Node>();
          new_node.hash = hash;
          new_node.key = name;
          new_node.next_ptr = this->_begin_ptr;
          this->_begin_ptr = &new_node;
          cur.right_ptr = &new_node;
          return new_node.value;
        }
      }
    }
    RQ_UNREACHABLE();
  }
  inline Value *getValuePtr(llvm::StringRef name) {
    std::ignore = name;
    RQ_TODO_IMPLEMENTATION();
  }
  inline Value *getValuePtr(llvm::StringRef name) const {
    std::ignore = name;
    RQ_TODO_IMPLEMENTATION();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator begin() {
    return Iterator(_begin_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator end() { return Iterator(nullptr); }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const {
    return ConstIterator(_begin_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const {
    return ConstIterator(nullptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const {
    return ConstIterator(_begin_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const {
    return ConstIterator(nullptr);
  }
};

} // namespace rq
