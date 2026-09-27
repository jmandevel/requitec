#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename ItemParam> struct AllocatedList;
template <typename ItemParam> struct AllocatedListItem;
template <typename ItemParam> struct AllocatedListRef;
template <typename ItemParam> struct ConstAllocatedListRef;
template <typename ItemParam> struct AllocatedListIterator;
template <typename ItemParam> struct ConstAllocatedListIterator;

template <typename ItemParam> struct AllocatedListItem final {
  using Item = ItemParam;
  using List = rq::AllocatedList<Item>;
  using Self = rq::AllocatedListItem<Item>;

  Item *item_ptr{nullptr};
  List next{};

  RQ_ALWAYS_INLINE AllocatedListItem() = default;
  AllocatedListItem(const Self &) = delete;
  AllocatedListItem(Self &&) = delete;
  RQ_ALWAYS_INLINE ~AllocatedListItem() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

template <typename ItemParam> struct AllocatedList final {
  using Item = ItemParam;
  using Allocated = rq::AllocatedListItem<Item>;
  using Iterator = rq::AllocatedListIterator<Item>;
  using ConstIterator = rq::ConstAllocatedListIterator<Item>;
  using Self = rq::AllocatedList<Item>;
  using Ref = rq::AllocatedListRef<Item>;
  using ConstRef = rq::ConstAllocatedListRef<Item>;

  llvm::PointerUnion<Item *, Allocated *> _ptr_union{nullptr};

  AllocatedList() = default;
  AllocatedList(Item& item) : _ptr_union(&item) {}
  ~AllocatedList() = default;
  AllocatedList(const Self &) = delete;
  AllocatedList(Self &&) = default;
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
                                    Item &item);
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &getHead() const {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    return rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).list;
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
AllocatedList<ItemParam>::insertFront(rq::BumpPtrAllocator &allocator,
                                    ItemParam &item) {
  using Item = ItemParam;
  using Allocated = rq::AllocatedListItem<Item>;
  if (this->getIsEmpty()) {
    this->_ptr_union = &item;
    return;
  }
  Allocated &node = allocator.allocateValue<Allocated>();
  node.item_ptr = &item;
  if (llvm::isa<Allocated *>(this->_ptr_union)) {
    Allocated &old_node = rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union));
    node.next._ptr_union = &old_node;
    this->_ptr_union = &node;
    return;
  }
  Item &old_item = rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
  node.next._ptr_union = &old_item;
  this->_ptr_union = &node;
}

template <typename ItemParam> struct AllocatedListRef final {
  using Item = ItemParam;
  using Allocated = rq::AllocatedListItem<Item>;
  using Iterator = rq::AllocatedListIterator<Item>;
  using ConstIterator = rq::ConstAllocatedListIterator<Item>;
  using List = rq::AllocatedList<Item>;
  using Self = rq::AllocatedListRef<Item>;
  using Ref = rq::AllocatedListRef<Item>;
  using ConstRef = rq::ConstAllocatedListRef<Item>;

  llvm::PointerUnion<Item *, Allocated *> _ptr_union{nullptr};

  AllocatedListRef() = default;
  AllocatedListRef(List &list) : _ptr_union(list._ptr_union) {}
  ~AllocatedListRef() = default;
  AllocatedListRef(const Self &) = default;
  AllocatedListRef(Self &&) = default;
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
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item &getHead() {
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Ref getTail() {
    return rq::dereferencePtr(llvm::cast<Allocated *>(this->_ptr_union)).list;
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

template <typename ItemParam> struct ConstAllocatedListRef final {
  using Item = ItemParam;
  using Allocated = rq::AllocatedListItem<Item>;
  using Iterator = rq::AllocatedListIterator<Item>;
  using ConstIterator = rq::ConstAllocatedListIterator<Item>;
  using List = rq::AllocatedList<Item>;
  using Self = rq::ConstAllocatedListRef<Item>;
  using Ref = rq::AllocatedListRef<Item>;
  using ConstRef = rq::ConstAllocatedListRef<Item>;

  llvm::PointerUnion<const Item *, const Allocated *> _ptr_union{nullptr};

  ConstAllocatedListRef() = default;
  ConstAllocatedListRef(const List &list)
      : _ptr_union(
            llvm::PointerUnion<const Item *, const Allocated *>::getFromOpaqueValue(
                list._ptr_union.getOpaqueValue())) {}
  ~ConstAllocatedListRef() = default;
  ConstAllocatedListRef(const Self &) = default;
  ConstAllocatedListRef(Self &&) = default;
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
    if (llvm::isa<Item *>(this->_ptr_union)) {
      return rq::dereferencePtr(llvm::cast<const Item *>(this->_ptr_union));
    }
    return rq::dereferencePtr(
        rq::dereferencePtr(llvm::cast<const Allocated *>(this->_ptr_union))
            .item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE ConstRef getTail() const {
    return rq::dereferencePtr(llvm::cast<const Allocated *>(this->_ptr_union)).list;
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

template <typename ItemParam> struct AllocatedListIterator final {
  using Item = ItemParam;
  using Ref = rq::AllocatedListRef<Item>;
  using Allocated = rq::AllocatedListItem<Item>;
  using Self = rq::AllocatedListIterator<Item>;
  using value_type = Item;
  using reference = Item &;
  using pointer = Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Ref _list;

  RQ_ALWAYS_INLINE AllocatedListIterator() = default;
  RQ_ALWAYS_INLINE explicit AllocatedListIterator(const Ref &list) : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (llvm::isa<Item *>(this->_list._ptr_union)) {
      this->_list._ptr_union = nullptr;
    } else if (llvm::isa<Allocated *>(this->_list._ptr_union)) {
      this->_list._ptr_union =
          rq::dereferencePtr(llvm::cast<Allocated *>(this->_list._ptr_union))
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
    } else if (llvm::isa<Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<Allocated *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item &operator*() const {
    if (llvm::isa<Item *>(this->_list.getIsItem())) {
      return rq::dereferencePtr(llvm::cast<Item *>(this->_list._ptr_union));
    } else if (llvm::isa<Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<Allocated *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Item *operator->() {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      return llvm::cast<Item *>(this->_list._ptr_union);
    } else if (llvm::isa<Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Allocated *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
    if (llvm::isa<Item *>(this->_list._ptr_union)) {
      return llvm::cast<Item *>(this->_list._ptr_union);
    } else if (llvm::isa<Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(llvm::cast<Allocated *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename ItemParam> struct ConstAllocatedListIterator final {
  using Item = ItemParam;
  using Ref = rq::ConstAllocatedListRef<Item>;
  using Allocated = rq::AllocatedListItem<Item>;
  using Self = rq::ConstAllocatedListIterator<Item>;
  using value_type = Item;
  using reference = const Item &;
  using pointer = const Item *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Ref _list;

  RQ_ALWAYS_INLINE ConstAllocatedListIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstAllocatedListIterator(const Ref &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      this->_list._ptr_union = nullptr;
    } else if (llvm::isa<const Allocated *>(this->_list._ptr_union)) {
      this->_list._ptr_union =
          llvm::PointerUnion<const Item *, const Allocated *>::getFromOpaqueValue(
              rq::dereferencePtr(
                  llvm::cast<const Allocated *>(this->_list._ptr_union))
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
    } else if (llvm::isa<const Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
          rq::dereferencePtr(llvm::cast<const Allocated *>(this->_list._ptr_union))
              .item_ptr);
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Item *operator->() const {
    if (llvm::isa<const Item *>(this->_list._ptr_union)) {
      return llvm::cast<const Item *>(this->_list._ptr_union);
    } else if (llvm::isa<const Allocated *>(this->_list._ptr_union)) {
      return rq::dereferencePtr(
                 llvm::cast<const Allocated *>(this->_list._ptr_union))
          .item_ptr;
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_list.getIsEmpty();
  }
};

template <typename ItemParam>
inline rq::AllocatedListIterator<ItemParam> AllocatedList<ItemParam>::begin() {
  return rq::AllocatedListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::AllocatedListIterator<ItemParam> AllocatedList<ItemParam>::end() {
  return rq::AllocatedListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedList<ItemParam>::begin() const {
  return rq::ConstAllocatedListIterator<ItemParam>(
      rq::ConstAllocatedListRef<ItemParam>(*this));
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedList<ItemParam>::end() const {
  return rq::ConstAllocatedListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedList<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedList<ItemParam>::cend() const {
  return end();
}

template <typename ItemParam>
inline rq::AllocatedListIterator<ItemParam> AllocatedListRef<ItemParam>::begin() {
  return rq::AllocatedListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::AllocatedListIterator<ItemParam> AllocatedListRef<ItemParam>::end() {
  return rq::AllocatedListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedListRef<ItemParam>::begin() const {
  return rq::ConstAllocatedListIterator<ItemParam>(
      rq::ConstAllocatedListRef<ItemParam>(*this));
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedListRef<ItemParam>::end() const {
  return rq::ConstAllocatedListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedListRef<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
AllocatedListRef<ItemParam>::cend() const {
  return end();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
ConstAllocatedListRef<ItemParam>::begin() const {
  return rq::ConstAllocatedListIterator<ItemParam>(*this);
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
ConstAllocatedListRef<ItemParam>::end() const {
  return rq::ConstAllocatedListIterator<ItemParam>();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
ConstAllocatedListRef<ItemParam>::cbegin() const {
  return begin();
}

template <typename ItemParam>
inline rq::ConstAllocatedListIterator<ItemParam>
ConstAllocatedListRef<ItemParam>::cend() const {
  return end();
}

} // namespace rq
