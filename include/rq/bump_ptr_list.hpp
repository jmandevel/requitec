#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename BumpPtrListItemParam> struct BumpPtrListNode;
template <typename BumpPtrListItemParam> struct BumpPtrList;
template <typename BumpPtrListItemParam> struct BumpPtrListIterator;
template <typename BumpPtrListItemParam> struct ConstBumpPtrListIterator;

// a list of items and BumpPtrListNodes allocated with a llvm::BumpPtrAllocator
template <typename BumpPtrListItemParam> struct BumpPtrList final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::BumpPtrList<BumpPtrListItem>;

  llvm::PointerUnion<BumpPtrListItem *, rq::BumpPtrListNode<BumpPtrListItem> *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE BumpPtrList() = default;
  RQ_ALWAYS_INLINE BumpPtrList(BumpPtrListItem &element) : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE BumpPtrList(rq::BumpPtrListNode<BumpPtrListItem> &BumpPtrListNode) : _ptr_union(&BumpPtrListNode) {}
  RQ_ALWAYS_INLINE ~BumpPtrList() = default;
  RQ_ALWAYS_INLINE BumpPtrList(const Self &) = default;
  RQ_ALWAYS_INLINE BumpPtrList(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<BumpPtrListItem *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBumpPtrListNode() const {
    return llvm::isa<rq::BumpPtrListNode<BumpPtrListItem> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE BumpPtrListItem &getItem() {
    return rq::dereferencePtr(llvm::cast<BumpPtrListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &getItem() const {
    return rq::dereferencePtr(llvm::cast<BumpPtrListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListNode<BumpPtrListItem> &getBumpPtrListNode() {
    return rq::dereferencePtr(
        llvm::cast<rq::BumpPtrListNode<BumpPtrListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::BumpPtrListNode<BumpPtrListItem> &getBumpPtrListNode() const {
    return rq::dereferencePtr(
        llvm::cast<rq::BumpPtrListNode<BumpPtrListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListIterator<BumpPtrListItem> begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListIterator<BumpPtrListItem> end();
};

template <typename BumpPtrListItemParam> struct BumpPtrListNode final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::BumpPtrListNode<BumpPtrListItem>;

  BumpPtrListItem *_item_ptr{nullptr};
  rq::BumpPtrList<BumpPtrListItem> _next{};

  RQ_ALWAYS_INLINE BumpPtrListNode() = default;
  RQ_ALWAYS_INLINE BumpPtrListNode(BumpPtrListItem &item_a, BumpPtrListItem &item_b)
      : _item_ptr(&item_a), _next(item_b) {}
  RQ_ALWAYS_INLINE BumpPtrListNode(BumpPtrListItem &item, rq::BumpPtrListNode<BumpPtrListItem> &BumpPtrListNode)
      : _item_ptr(&item), _next(BumpPtrListNode) {}
  RQ_ALWAYS_INLINE BumpPtrListNode(BumpPtrListItem &item,
                        const rq::BumpPtrList<BumpPtrListItem> &list)
      : _item_ptr(&item), _next(list) {}
  BumpPtrListNode(const Self &) = delete;
  BumpPtrListNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~BumpPtrListNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasItem() const {
    return this->_item_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNext() const {
    return !this->_next.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE BumpPtrListItem &getItem() {
    return rq::dereferencePtr(this->_item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &getItem() const {
    return rq::dereferencePtr(this->_item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<BumpPtrListItem> &getNext() {
    return this->_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::BumpPtrList<BumpPtrListItem> &
  getNext() const {
    return this->_next;
  }
};

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
  RQ_ALWAYS_INLINE explicit BumpPtrListIterator(rq::BumpPtrList<BumpPtrListItem> &list)
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
