#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename ItemParam> struct NodeList;
template <typename ItemParam> struct NodeListNode;
template <typename ItemParam> struct NodeListRef;
template <typename ItemParam> struct ConstNodeListRef;
template <typename ItemParam> struct NodeListIterator;
template <typename ItemParam> struct ConstNodeListIterator;

template <typename ItemParam> struct NodeListNode final {
  using Item = ItemParam;
  using List = rq::NodeList<Item>;
  using Self = rq::NodeListNode<Item>;

  Item *item_ptr{nullptr};
  List next{};

  RQ_ALWAYS_INLINE NodeListNode() = default;
  NodeListNode(const Self &) = delete;
  NodeListNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~NodeListNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

template <typename ItemParam> struct NodeList final {
  using Item = ItemParam;
  using Node = rq::NodeListNode<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using Self = rq::NodeList<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;

  llvm::PointerUnion<Item *, Node *> _ptr_union{nullptr};

  NodeList() = default;
  NodeList(Item& item) : _ptr_union(&item) {}
  ~NodeList() = default;
  NodeList(const Self &) = delete;
  NodeList(Self &&) = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasHead() const {
    return !this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTail() const {
    return llvm::isa<Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  RQ_ALWAYS_INLINE void insertFront(rq::BumpPtrAllocator &allocator,
                                    Item &item);
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).list;
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

template <typename ItemParam>
RQ_ALWAYS_INLINE void
NodeList<ItemParam>::insertFront(rq::BumpPtrAllocator &allocator,
                                    ItemParam &item) {
  using Item = ItemParam;
  using Node = rq::NodeListNode<Item>;
  if (this->getIsEmpty()) {
    this->_ptr_union = &item;
    return;
  }
  Node &node = allocator.allocateValue<Node>();
  node.item_ptr = &item;
  if (llvm::isa<Node *>(this->_ptr_union)) {
    Node &old_node = rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union));
    node.next._ptr_union = &old_node;
    this->_ptr_union = &node;
    return;
  }
  Item &old_item = rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  node.next._ptr_union = &old_item;
  this->_ptr_union = &node;
}

template <typename ItemParam> struct NodeListRef final {
  using Item = ItemParam;
  using Node = rq::NodeListNode<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using List = rq::NodeList<Item>;
  using Self = rq::NodeListRef<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;

  llvm::PointerUnion<Item *, Node *> _ptr_union{nullptr};

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
    return llvm::isa<Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    return rq::dereferencePtr(llvm::cast<Node *>(this->_ptr_union)).list;
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

template <typename ItemParam> struct ConstNodeListRef final {
  using Item = ItemParam;
  using Node = rq::NodeListNode<Item>;
  using Iterator = rq::NodeListIterator<Item>;
  using ConstIterator = rq::ConstNodeListIterator<Item>;
  using List = rq::NodeList<Item>;
  using Self = rq::ConstNodeListRef<Item>;
  using Ref = rq::NodeListRef<Item>;
  using ConstRef = rq::ConstNodeListRef<Item>;

  llvm::PointerUnion<const Item *, const Node *> _ptr_union{nullptr};

  ConstNodeListRef() = default;
  ConstNodeListRef(const List &list)
      : _ptr_union(
            llvm::PointerUnion<const Item *, const Node *>::getFromOpaqueValue(
                list._ptr_union.getOpaqueValue())) {}
  ~ConstNodeListRef() = default;
  ConstNodeListRef(const Self &) = default;
  ConstNodeListRef(Self &&) = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasHead() const {
    return !this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTail() const {
    return llvm::isa<const Node *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<const Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<const Node *>(this->_ptr_union))
            .item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<const Node *>(this->_ptr_union)).list;
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

template <typename ItemParam> struct NodeListIterator final {
  using Item = ItemParam;
  using Ref = rq::NodeListRef<Item>;
  using Node = rq::NodeListNode<Item>;
  using Self = rq::NodeListIterator<Item>;
  using value_type = Item;
  using reference = Item &;
  using pointer = Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Ref _list;

  RQ_ALWAYS_INLINE NodeListIterator() = default;
  RQ_ALWAYS_INLINE explicit NodeListIterator(const Ref &list) : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (llvm::isa<Item *>(this->_list._ptr_union)) {
      this->_list._ptr_union = nullptr;
    } else if (llvm::isa<Node *>(this->_list._ptr_union)) {
      this->_list._ptr_union =
          rq::dereferencePtr(llvm::cast<Node *>(this->_list._ptr_union))
              .next._ptr_union;
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
    if (llvm::isa<Item *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_list._ptr_union));
    } else if (llvm::isa<Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<Node *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &operator*() const {
    if (llvm::isa<Item *>(this->_list.getIsItem())) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_list._ptr_union));
    } else if (llvm::isa<Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<Node *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item *operator->() {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      return llvm::cast<Item *>(this->_list._ptr_union);
    } else if (llvm::isa<Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Node *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
    if (llvm::isa<Item *>(this->_list._ptr_union)) {
      return llvm::cast<Item *>(this->_list._ptr_union);
    } else if (llvm::isa<Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Node *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename ItemParam> struct ConstNodeListIterator final {
  using Item = ItemParam;
  using Ref = rq::ConstNodeListRef<Item>;
  using Node = rq::NodeListNode<Item>;
  using Self = rq::ConstNodeListIterator<Item>;
  using value_type = Item;
  using reference = const Item &;
  using pointer = const Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Ref _list;

  RQ_ALWAYS_INLINE ConstNodeListIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstNodeListIterator(const Ref &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      this->_list._ptr_union = nullptr;
    } else if (llvm::isa<const Node *>(this->_list._ptr_union)) {
      this->_list._ptr_union =
          llvm::PointerUnion<const Item *, const Node *>::getFromOpaqueValue(
              rq::dereferencePtr(
                  llvm::cast<const Node *>(this->_list._ptr_union))
                  .next._ptr_union.getOpaqueValue());
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
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          llvm::cast<const Item *>(this->_list._ptr_union));
    } else if (llvm::isa<const Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<const Node *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      return llvm::cast<const Item *>(this->_list._ptr_union);
    } else if (llvm::isa<const Node *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
                 llvm::cast<const Node *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename ItemParam>
inline rq::NodeListIterator<ItemParam> NodeList<ItemParam>::begin() {
  return rq::NodeListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::NodeListIterator<ItemParam> NodeList<ItemParam>::end() {
  return rq::NodeListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeList<ItemParam>::begin() const {
  return rq::ConstNodeListIterator<ItemParam>(
      rq::ConstNodeListRef<ItemParam>(*this));
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeList<ItemParam>::end() const {
  return rq::ConstNodeListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeList<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeList<ItemParam>::cend() const {
  return end();
}

template <typename ItemParam>
inline rq::NodeListIterator<ItemParam> NodeListRef<ItemParam>::begin() {
  return rq::NodeListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::NodeListIterator<ItemParam> NodeListRef<ItemParam>::end() {
  return rq::NodeListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeListRef<ItemParam>::begin() const {
  return rq::ConstNodeListIterator<ItemParam>(
      rq::ConstNodeListRef<ItemParam>(*this));
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeListRef<ItemParam>::end() const {
  return rq::ConstNodeListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeListRef<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
NodeListRef<ItemParam>::cend() const {
  return end();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
ConstNodeListRef<ItemParam>::begin() const {
  return rq::ConstNodeListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
ConstNodeListRef<ItemParam>::end() const {
  return rq::ConstNodeListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
ConstNodeListRef<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstNodeListIterator<ItemParam>
ConstNodeListRef<ItemParam>::cend() const {
  return end();
}

} // namespace rq
