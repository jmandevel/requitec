#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/iterators.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename ItemParam> struct NodeList;
template <typename ItemParam> struct NodeListItem;
template <typename ItemParam> struct NodeListRef;
template <typename ItemParam> struct ConstNodeListRef;

template <typename ItemParam> struct NodeListIteratorPolicy final {
  using Item = ItemParam;
  using Ref = rq::NodeListRef<Item>;
  using Allocated = rq::NodeListItem<Item>;

  RQ_ALWAYS_INLINE void next(Ref &state) const {
    if (llvm::isa<Item *>(state._ptr_union)) {
      state._ptr_union = nullptr;
    } else if (llvm::isa<Allocated *>(state._ptr_union)) {
      Allocated *node_ptr = llvm::cast<Allocated *>(state._ptr_union);
      Allocated &node = rq::dereferencePtr(node_ptr);
      state._ptr_union = node.next._ptr_union;
    } else {
      RQ_UNREACHABLE();
    }
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &value(Ref &state) const {
    return state.getHead();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &value(const Ref &state) const {
    return state.getHead();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone(const Ref &state) const {
    return state.getIsEmpty();
  }
};

template <typename ItemParam> struct ConstNodeListIteratorPolicy final {
  using Item = ItemParam;
  using Ref = rq::ConstNodeListRef<Item>;
  using Allocated = rq::NodeListItem<Item>;
  using PtrUnion = llvm::PointerUnion<const Item *, const Allocated *>;

  RQ_ALWAYS_INLINE void next(Ref &state) const {
    if (llvm::isa<const Item *>(state._ptr_union)) {
      state._ptr_union = nullptr;
    } else if (llvm::isa<const Allocated *>(state._ptr_union)) {
      const Allocated *node_ptr =
          llvm::cast<const Allocated *>(state._ptr_union);
      const Allocated &node = rq::dereferencePtr(node_ptr);
      void *next_value = node.next._ptr_union.getOpaqueValue();
      state._ptr_union = PtrUnion::getFromOpaqueValue(next_value);
    } else {
      RQ_UNREACHABLE();
    }
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &value(const Ref &state) const {
    return state.getHead();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone(const Ref &state) const {
    return state.getIsEmpty();
  }
};

template <typename ItemParam>
using NodeListIterator =
    rq::Iterator<rq::NodeListRef<ItemParam>,
                 rq::NodeListIteratorPolicy<ItemParam>, ItemParam>;

template <typename ItemParam>
using ConstNodeListIterator =
    rq::Iterator<rq::ConstNodeListRef<ItemParam>,
                 rq::ConstNodeListIteratorPolicy<ItemParam>, const ItemParam>;

template <typename ItemParam> struct NodeListItem final {
  using Item = ItemParam;
  using List = rq::NodeList<Item>;
  using Self = rq::NodeListItem<Item>;

  Item *item_ptr{nullptr};
  List next{};

  RQ_ALWAYS_INLINE NodeListItem() = default;
  NodeListItem(const Self &) = delete;
  NodeListItem(Self &&) = delete;
  RQ_ALWAYS_INLINE ~NodeListItem() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

template <typename ItemParam> struct NodeList final {
  using Item = ItemParam;
  using Allocated = rq::NodeListItem<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using Self = rq::NodeList<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;

  llvm::PointerUnion<Item *, Allocated *> _ptr_union{nullptr};

  NodeList() = default;
  NodeList(Item &item) : _ptr_union(&item) {}
  ~NodeList() = default;
  NodeList(const Self &) = delete;
  NodeList(Self &&) = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasHead() const {
    return !this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTail() const {
    return llvm::isa<Allocated *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  RQ_ALWAYS_INLINE void insertFront(rq::BumpPtrAllocator &allocator,
                                    Item &item) {
    if (this->getIsEmpty()) {
      this->_ptr_union = &item;
      return;
    }
    Allocated &node = allocator.allocateValue<Allocated>();
    node.item_ptr = &item;
    if (llvm::isa<Allocated *>(this->_ptr_union)) {
      Allocated *old_node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
      Allocated &old_node = rq::dereferencePtr(old_node_ptr);
      node.next._ptr_union = &old_node;
      this->_ptr_union = &node;
      return;
    }
    Item *old_item_ptr = llvm::cast<Item *>(this->_ptr_union);
    Item &old_item = rq::dereferencePtr(old_item_ptr);
    node.next._ptr_union = &old_item;
    this->_ptr_union = &node;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      Item *item_ptr = llvm::cast<Item *>(this->_ptr_union);
      return rq::dereferencePtr(item_ptr);
    }
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return rq::dereferencePtr(node.item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      Item *item_ptr = llvm::cast<Item *>(this->_ptr_union);
      return rq::dereferencePtr(item_ptr);
    }
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return rq::dereferencePtr(node.item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return node.list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return node.list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator begin() { return Iterator(*this); }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator end() { return Iterator(); }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const {
    ConstRef ref(*this);
    return ConstIterator(ref);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const {
    return ConstIterator();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const {
    return begin();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const { return end(); }
};

template <typename ItemParam> struct NodeListRef final {
  using Item = ItemParam;
  using Allocated = rq::NodeListItem<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using List = rq::NodeList<Item>;
  using Self = rq::NodeListRef<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;

  llvm::PointerUnion<Item *, Allocated *> _ptr_union{nullptr};

  NodeListRef() = default;
  NodeListRef(List &list) : _ptr_union(list._ptr_union) {}
  ~NodeListRef() = default;
  NodeListRef(const Self &) = default;
  NodeListRef(Self &&) = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasHead() const {
    return !this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTail() const {
    return llvm::isa<Allocated *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      Item *item_ptr = llvm::cast<Item *>(this->_ptr_union);
      return rq::dereferencePtr(item_ptr);
    }
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return rq::dereferencePtr(node.item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      Item *item_ptr = llvm::cast<Item *>(this->_ptr_union);
      return rq::dereferencePtr(item_ptr);
    }
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return rq::dereferencePtr(node.item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return node.list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    Allocated *node_ptr = llvm::cast<Allocated *>(this->_ptr_union);
    Allocated &node = rq::dereferencePtr(node_ptr);
    return node.list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator begin() { return Iterator(*this); }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator end() { return Iterator(); }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const {
    ConstRef ref(*this);
    return ConstIterator(ref);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const {
    return ConstIterator();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const {
    return begin();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const { return end(); }
};

template <typename ItemParam> struct ConstNodeListRef final {
  using Item = ItemParam;
  using Allocated = rq::NodeListItem<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using List = rq::NodeList<Item>;
  using Self = rq::ConstNodeListRef<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;
  using PtrUnion = llvm::PointerUnion<const Item *, const Allocated *>;

  PtrUnion _ptr_union{nullptr};

  ConstNodeListRef() = default;
  ConstNodeListRef(const List &list) {
    void *list_value = list._ptr_union.getOpaqueValue();
    this->_ptr_union = PtrUnion::getFromOpaqueValue(list_value);
  }
  ConstNodeListRef(const Ref &list) {
    void *list_value = list._ptr_union.getOpaqueValue();
    this->_ptr_union = PtrUnion::getFromOpaqueValue(list_value);
  }
  ~ConstNodeListRef() = default;
  ConstNodeListRef(const Self &) = default;
  ConstNodeListRef(Self &&) = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasHead() const {
    return !this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTail() const {
    return llvm::isa<const Allocated *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<const Item *>(this->_ptr_union)) {
      const Item *item_ptr = llvm::cast<const Item *>(this->_ptr_union);
      return rq::dereferencePtr(item_ptr);
    }
    const Allocated *node_ptr = llvm::cast<const Allocated *>(this->_ptr_union);
    const Allocated &node = rq::dereferencePtr(node_ptr);
    return rq::dereferencePtr(node.item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    const Allocated *node_ptr = llvm::cast<const Allocated *>(this->_ptr_union);
    const Allocated &node = rq::dereferencePtr(node_ptr);
    return node.list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const {
    ConstRef ref(*this);
    return ConstIterator(ref);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const {
    return ConstIterator();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const {
    return begin();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const { return end(); }
};

} // namespace rq
