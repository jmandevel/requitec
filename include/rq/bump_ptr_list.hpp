#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename ItemParam> struct BumpPtrList;
template <typename ItemParam> struct BumpPtrListNode;
template <typename ItemParam> struct BumpPtrListRef;
template <typename ItemParam> struct BumpPtrListIterator;
template <typename ItemParam> struct ConstBumpPtrListIterator;

template <typename ItemParam> struct BumpPtrList final {
  using Item = ItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  using Iterator = rq::BumpPtrListIterator<Item>;
  using ConstIterator = rq::ConstBumpPtrListIterator<Item>;
  using Self = rq::BumpPtrList<Item>;

  llvm::PointerUnion<Item *, Node *> _ptr_union{nullptr};

  BumpPtrList() = default;
  ~BumpPtrList() = default;
  BumpPtrList(const Self &) = delete;
  BumpPtrList(Self &&) = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<Item *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE const Node &getNode() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const;
};

template <typename ItemParam> struct BumpPtrListNode final {
  using Item = ItemParam;
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

template <typename ItemParam>
RQ_ALWAYS_INLINE void
BumpPtrList<ItemParam>::insertFront(rq::BumpPtrAllocator &allocator,
                                    ItemParam &item) {
  using Item = ItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  if (this->getIsEmpty()) {
    this->_ptr_union = &item;
    return;
  }
  Node &node = allocator.allocateValue<Node>();
  node.item_ptr = &item;
  if (this->getIsNode()) {
    Node &old_node = this->getNode();
    node.next._ptr_union = &old_node;
    this->_ptr_union = &node;
    return;
  }
  RQ_ASSERT(this->getIsItem(), "invalid state");
  Item &old_item = this->getItem();
  node.next._ptr_union = &old_item;
  this->_ptr_union = &node;
}

template <typename ItemParam> struct BumpPtrListRef final {
  using Item = ItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  using Iterator = rq::BumpPtrListIterator<Item>;
  using ConstIterator = rq::ConstBumpPtrListIterator<Item>;
  using List = rq::BumpPtrList<Item>;
  using Self = rq::BumpPtrListRef<Item>;

  llvm::PointerUnion<Item *, Node *> _ptr_union{nullptr};

  BumpPtrListRef() = default;
  BumpPtrListRef(List &list) : _ptr_union(list._ptr_union) {}
  ~BumpPtrListRef() = default;
  BumpPtrListRef(const Self &) = default;
  BumpPtrListRef(Self &&) = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<Item *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getItem() {
    return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getItem() const {
    return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Node &getNode() {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Node &getNode() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const;
};

template <typename ItemParam> struct ConstBumpPtrListRef final {
  using Item = ItemParam;
  using Node = rq::BumpPtrListNode<Item>;
  using Iterator = rq::BumpPtrListIterator<Item>;
  using ConstIterator = rq::ConstBumpPtrListIterator<Item>;
  using List = rq::BumpPtrList<Item>;
  using Self = rq::ConstBumpPtrListRef<Item>;

  llvm::PointerUnion<const Item *, const Node *> _ptr_union{nullptr};

  ConstBumpPtrListRef() = default;
  ConstBumpPtrListRef(const List &list)
      : _ptr_union(
            llvm::PointerUnion<const Item *, const Node *>::getFromOpaqueValue(
                list._ptr_union.getOpaqueValue())) {}
  ~ConstBumpPtrListRef() = default;
  ConstBumpPtrListRef(const Self &) = default;
  ConstBumpPtrListRef(Self &&) = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<const Item *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<const Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getItem() const {
    return rq::dereferencePtr(llvm::cast<const Item *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Node &getNode() const {
    return rq::dereferencePtr(llvm::cast<const Node *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE ConstIterator cend() const;
};

template <typename ItemParam> struct BumpPtrListIterator final {
  using Item = ItemParam;
  using List = rq::BumpPtrList<Item>;
  using Node = rq::BumpPtrListNode<Item>;
  using Self = rq::BumpPtrListIterator<Item>;
  using value_type = Item;
  using reference = Item &;
  using pointer = Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  List _list;

  RQ_ALWAYS_INLINE BumpPtrListIterator() = default;
  RQ_ALWAYS_INLINE explicit BumpPtrListIterator(List &list) : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list._ptr_union = nullptr;
    } else if (this->_list.getIsNode()) {
      this->_list._ptr_union = this->_list.getNode().next._ptr_union;
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
  [[nodiscard]] RQ_ALWAYS_INLINE Item &operator*() {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return rq::dereferencePtr(this->_list.getNode().item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return rq::dereferencePtr(this->_list.getNode().item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item *operator->() {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return &this->_list.getNode().item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename ItemParam> struct ConstBumpPtrListIterator final {
  using Item = ItemParam;
  using Ref = rq::ConstBumpPtrListRef<Item>;
  using Node = rq::BumpPtrListNode<Item>;
  using Self = rq::ConstBumpPtrListIterator<Item>;
  using value_type = Item;
  using reference = const Item &;
  using pointer = const Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Ref _list;

  RQ_ALWAYS_INLINE ConstBumpPtrListIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstBumpPtrListIterator(const Ref &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list._ptr_union = nullptr;
    } else if (this->_list.getIsNode()) {
      this->_list._ptr_union =
          llvm::PointerUnion<const Item *, const Node *>::getFromOpaqueValue(
              this->_list.getNode().next._ptr_union.getOpaqueValue());
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
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return rq::dereferencePtr(this->_list.getNode().item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
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

template <typename ItemParam>
inline rq::BumpPtrListIterator<ItemParam> BumpPtrList<ItemParam>::begin() {
  return rq::BumpPtrListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::BumpPtrListIterator<ItemParam> BumpPtrList<ItemParam>::end() {
  return rq::BumpPtrListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
BumpPtrList<ItemParam>::begin() const {
  return rq::ConstBumpPtrListIterator<ItemParam>(
      rq::ConstBumpPtrListRef<ItemParam>(*this));
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
BumpPtrList<ItemParam>::end() const {
  return rq::ConstBumpPtrListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
BumpPtrList<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
BumpPtrList<ItemParam>::cend() const {
  return end();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
ConstBumpPtrListRef<ItemParam>::begin() const {
  return rq::ConstBumpPtrListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
ConstBumpPtrListRef<ItemParam>::end() const {
  return rq::ConstBumpPtrListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
ConstBumpPtrListRef<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstBumpPtrListIterator<ItemParam>
ConstBumpPtrListRef<ItemParam>::cend() const {
  return end();
}

} // namespace rq
