#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename BumpPtrListItemParam> struct Node;
template <typename BumpPtrListItemParam> struct BumpPtrList;
template <typename BumpPtrListItemParam> struct BumpPtrListIterator;
template <typename BumpPtrListItemParam> struct ConstBumpPtrListIterator;

// a list of items and nodes allocated with a llvm::BumpPtrAllocator
template <typename BumpPtrListItemParam> struct BumpPtrList final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::BumpPtrList<BumpPtrListItem>;

  llvm::PointerUnion<BumpPtrListItem *, rq::Node<BumpPtrListItem> *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE BumpPtrList() = default;
  RQ_ALWAYS_INLINE BumpPtrList(BumpPtrListItem &element) : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE BumpPtrList(rq::Node<BumpPtrListItem> &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~BumpPtrList() = default;
  RQ_ALWAYS_INLINE BumpPtrList(const Self &) = default;
  RQ_ALWAYS_INLINE BumpPtrList(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<BumpPtrListItem *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<rq::Node<BumpPtrListItem> *>(this->_ptr_union);
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node<BumpPtrListItem> &getNode() {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<BumpPtrListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<BumpPtrListItem> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<BumpPtrListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListIterator<BumpPtrListItem> begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListIterator<BumpPtrListItem> end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  cend() const;
};

template <typename BumpPtrListItemParam> struct ConstBumpPtrList final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::ConstBumpPtrList<BumpPtrListItem>;

  llvm::PointerUnion<const BumpPtrListItem *, const rq::Node<BumpPtrListItem> *>
      _ptr_union{nullptr};

  RQ_ALWAYS_INLINE ConstBumpPtrList() = default;
  RQ_ALWAYS_INLINE ConstBumpPtrList(const rq::BumpPtrList<BumpPtrListItem> &rhs)
      : _ptr_union(
            std::bit_cast<llvm::PointerUnion<const BumpPtrListItem *,
                                             const rq::Node<BumpPtrListItem> *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstBumpPtrList(rq::BumpPtrList<BumpPtrListItem> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const BumpPtrListItem *, const rq::Node<BumpPtrListItem> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstBumpPtrList(const BumpPtrListItem &element)
      : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE ConstBumpPtrList(const rq::Node<BumpPtrListItem> &node)
      : _ptr_union(&node) {}
  ~ConstBumpPtrList() = default;
  RQ_ALWAYS_INLINE ConstBumpPtrList(const Self &) = default;
  RQ_ALWAYS_INLINE ConstBumpPtrList(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::BumpPtrList<BumpPtrListItem> &rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const BumpPtrListItem *, const rq::Node<BumpPtrListItem> *>>(rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::BumpPtrList<BumpPtrListItem> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const BumpPtrListItem *, const rq::Node<BumpPtrListItem> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<const BumpPtrListItem *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<const rq::Node<BumpPtrListItem> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &getItem() const {
    return rq::dereferencePtr(
        llvm::cast<const BumpPtrListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<BumpPtrListItem> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<const rq::Node<BumpPtrListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListIterator<BumpPtrListItem>
  cend() const;
};

template <typename BumpPtrListItemParam> struct Node final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::Node<BumpPtrListItem>;

  BumpPtrListItem *_item_ptr{nullptr};
  rq::BumpPtrList<BumpPtrListItem> _next{};

  RQ_ALWAYS_INLINE Node() = default;
  RQ_ALWAYS_INLINE Node(BumpPtrListItem &item_a, BumpPtrListItem &item_b)
      : _item_ptr(&item_a), _next(item_b) {}
  RQ_ALWAYS_INLINE Node(BumpPtrListItem &item, rq::Node<BumpPtrListItem> &node)
      : _item_ptr(&item), _next(node) {}
  RQ_ALWAYS_INLINE Node(BumpPtrListItem &item,
                        const rq::BumpPtrList<BumpPtrListItem> &list)
      : _item_ptr(&item), _next(list) {}
  Node(const Self &) = delete;
  Node(Self &&) = delete;
  RQ_ALWAYS_INLINE ~Node() = default;
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
    } else if (this->_list.getIsNode()) {
      this->_list = this->_list.getNode().getNext();
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
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE BumpPtrListItem *operator->() {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return &this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem *operator->() const {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return &this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename BumpPtrListItemParam> struct ConstBumpPtrListIterator final {
  using BumpPtrListItem = BumpPtrListItemParam;
  using Self = rq::ConstBumpPtrListIterator<BumpPtrListItem>;
  using value_type = const BumpPtrListItem;
  using reference = const BumpPtrListItem &;
  using pointer = BumpPtrListItem *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ConstBumpPtrList<BumpPtrListItem> _list;

  RQ_ALWAYS_INLINE ConstBumpPtrListIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstBumpPtrListIterator(
      const rq::BumpPtrList<BumpPtrListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE explicit ConstBumpPtrListIterator(
      const rq::ConstBumpPtrList<BumpPtrListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list = rq::ConstBumpPtrList<BumpPtrListItem>();
    } else if (this->_list.getIsNode()) {
      this->_list = this->_list.getNode().getNext();
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
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const BumpPtrListItem *operator->() const {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return &this->_list.getNode().getItem();
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

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::begin() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>(*this);
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::end() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>();
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::cbegin() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>(*this);
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
BumpPtrList<BumpPtrListItemParam>::cend() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>();
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
ConstBumpPtrList<BumpPtrListItemParam>::begin() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>(*this);
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
ConstBumpPtrList<BumpPtrListItemParam>::end() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>();
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
ConstBumpPtrList<BumpPtrListItemParam>::cbegin() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>(*this);
}

template <typename BumpPtrListItemParam>
inline rq::ConstBumpPtrListIterator<BumpPtrListItemParam>
ConstBumpPtrList<BumpPtrListItemParam>::cend() const {
  return rq::ConstBumpPtrListIterator<BumpPtrListItemParam>();
}

} // namespace rq
