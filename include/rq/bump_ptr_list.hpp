#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename BumpPtrListItemParam> struct BumpPtrListNode;
template <typename BumpPtrListItemParam> struct BumpPtrList;
template <typename BumpPtrListItemParam> struct BumpPtrListIterator;

// a list of items and BumpPtrListNodes allocated with a llvm::BumpPtrAllocator
template <typename BumpPtrListItemParam> struct BumpPtrList final {
  using Item = BumpPtrListItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  using Iterator = rq::BumpPtrListIterator<Item>;
  using Self = rq::BumpPtrList<Item>;

  llvm::PointerUnion<Item *, rq::BumpPtrListNode<Item> *> _ptr_union{nullptr};

  RQ_ALWAYS_INLINE BumpPtrList() = default;
  RQ_ALWAYS_INLINE ~BumpPtrList() = default;
  RQ_ALWAYS_INLINE BumpPtrList(const Self &) = delete;
  RQ_ALWAYS_INLINE BumpPtrList(Self &&) = delete;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = delete;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<Item *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBumpPtrListNode() const {
    return llvm::isa<Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  RQ_ALWAYS_INLINE void insertFront(rq::BumpPtrAllocator &allocator,
                                    Item &item);
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getItem() {
    return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getItem() const {
    return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Node &getNode() {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Node &getBumpPtrListNode() const {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator begin();
  [[nodiscard]] RQ_ALWAYS_INLINE Iterator end();
};

template <typename BumpPtrListItemParam> struct BumpPtrListNode final {
  using Item = BumpPtrListItemParam;
  using List = rq::BumpPtrList<Item>;
  using Self = rq::BumpPtrListNode<Item>;

  Item *item_ptr{nullptr};
  List next{};

  RQ_ALWAYS_INLINE BumpPtrListNode() = default;
  BumpPtrListNode(const Self &) = delete;
  BumpPtrListNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~BumpPtrListNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

template <typename BumpPtrListItemParam>
RQ_ALWAYS_INLINE void
BumpPtrList<BumpPtrListItemParam>::insertFront(rq::BumpPtrAllocator &allocator,
                                               BumpPtrListItemParam &item) {
  using Item = BumpPtrListItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  if (this->getIsEmpty()) {
    this->_ptr_union = &item;
    return;
  }
  Node &node = allocator.allocateValue<Node>();
  node.item_ptr = &item;
  if (this->getIsBumpPtrListNode()) {
    Node &old_node = this->getBumpPtrListNode();
    node.next._ptr_union = &old_node;
    this->_ptr_union = &node;
    return;
  }
  RQ_ASSERT(this->getIsItem(), "invalid state");
  Item &old_item = this->getItem();
  node.item_ptr = &old_item;
  this->_ptr_union = &node;
}

template <typename BumpPtrListItemParam> struct BumpPtrListIterator final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::BumpPtrListIterator<BumpPtrListItem>;
  using value_type = BumpPtrListItem;
  using reference = BumpPtrListItem &;
  using pointer = BumpPtrListItem *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::BumpPtrList<BumpPtrListItem> _list;

  RQ_ALWAYS_INLINE BumpPtrListIterator() = default;
  RQ_ALWAYS_INLINE explicit BumpPtrListIterator(
      rq::BumpPtrList<BumpPtrListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list = rq::BumpPtrList<BumpPtrListItem>();
    } else if (this->_list.getIsBumpPtrListNode()) {
      this->_list = this->_list.getBumpPtrListNode().getNext();
    } else {
      RQ_UNREACHABLE();
    }
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_list == it._list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_list != it._list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE BumpPtrListItem &operator*() {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsBumpPtrListNode()) {
      return this->_list.getBumpPtrListNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsBumpPtrListNode()) {
      return this->_list.getBumpPtrListNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE BumpPtrListItem *operator->() {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsBumpPtrListNode()) {
      return &this->_list.getBumpPtrListNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem *operator->() const {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsBumpPtrListNode()) {
      return &this->_list.getBumpPtrListNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename BumpPtrListItemParam>
inline rq::BumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::begin() {
  return rq::BumpPtrListIterator<BumpPtrListItemParam>(*this);
}

template <typename BumpPtrListItemParam>
inline rq::BumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::end() {
  return rq::BumpPtrListIterator<BumpPtrListItemParam>();
}

} // namespace rq
