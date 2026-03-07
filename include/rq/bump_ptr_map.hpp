#pragma once

#include <functional>
#include <cstddef>

namespace rq {

// A map backed by a binary search tree ordered by std::hash of the key.
// Nodes are allocated using a llvm::BumpPtrAllocator.

template <typename KeyParam, typename ValueParam, typename HashParam = std::hash<KeyParam>>
struct BumpPtrMapNode {
    using Key = KeyParam;
    using Value = ValueParam;
    using Hash = HashParam;
    using Self = rq::BumpPtrMap<Key, Value, Hash>;

    Key key;
    Value value;
    Hash hash;
    Self *left;
    Self *right;
};

template <typename KeyParam, typename ValueParam, typename HashParam = std::hash<KeyParam>>
struct BumpPtrMap {
  using Key = KeyParam;
  using Value = ValueParam;
  using Hash = HashParam;
  using Self = rq::BumpPtrMap<Key, Value, Hash>;
  using Node = rq::BumpPtrMapNode<Key, Value, Hash>;
  using KeyType = Key;
  using ValueType = Value;
  using HashType = Hash;
  using HashValueType = std::size_t;

  Node *_root{nullptr};
  std::size_t _count{0};

  BumpPtrMap() = default;
  BumpPtrMap() = delete;
  BumpPtrMap(const BumpPtrMap &) = delete;
  BumpPtrMap(BumpPtrMap &&) = delete;
  BumpPtrMap &operator=(const BumpPtrMap &) = delete;
  BumpPtrMap &operator=(BumpPtrMap &&) = delete;
  ~BumpPtrMap() = default;

  std::size_t getCount() const { return _count; }
  bool getIsEmpty() const { return _count == 0; }

  // TODO implement rest
};

} // namespace rq
