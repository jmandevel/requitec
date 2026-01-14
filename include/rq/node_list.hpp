#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/PointerUnion.h>

#include <bit>
#include <cstddef>
#include <iterator>

namespace rq {

template <typename NodeListItemParam> struct Node;
template <typename NodeListItemParam> struct NodeList;
template <typename NodeListItemParam> struct NodeListIterator;
template <typename NodeListItemParam> struct ConstNodeListIterator;

template <typename NodeListItemParam> struct NodeList final {
  using NodeListItem = NodeListItemParam;
  using Self = rq::NodeList<NodeListItem>;

  llvm::PointerUnion<NodeListItem *, rq::Node<NodeListItem> *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE NodeList() = default;
  RQ_ALWAYS_INLINE NodeList(NodeListItem &element) : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE NodeList(rq::Node<NodeListItem> &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~NodeList() = default;
  RQ_ALWAYS_INLINE NodeList(const Self &) = default;
  RQ_ALWAYS_INLINE NodeList(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<NodeListItem *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<rq::Node<NodeListItem> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE NodeListItem &getItem() {
    return rq::dereferencePtr(llvm::cast<NodeListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem &getItem() const {
    return rq::dereferencePtr(llvm::cast<NodeListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node<NodeListItem> &getNode() {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<NodeListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<NodeListItem> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<NodeListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListIterator<NodeListItem> begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeListIterator<NodeListItem> end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  cend() const;
};

template <typename NodeListItemParam> struct ConstNodeList final {
  using NodeListItem = NodeListItemParam;
  using Self = rq::ConstNodeList<NodeListItem>;

  llvm::PointerUnion<const NodeListItem *, const rq::Node<NodeListItem> *>
      _ptr_union{nullptr};

  RQ_ALWAYS_INLINE ConstNodeList() = default;
  RQ_ALWAYS_INLINE ConstNodeList(const rq::NodeList<NodeListItem> &rhs)
      : _ptr_union(
            std::bit_cast<llvm::PointerUnion<const NodeListItem *,
                                             const rq::Node<NodeListItem> *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstNodeList(rq::NodeList<NodeListItem> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const NodeListItem *, const rq::Node<NodeListItem> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstNodeList(const NodeListItem &element)
      : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE ConstNodeList(const rq::Node<NodeListItem> &node)
      : _ptr_union(&node) {}
  ~ConstNodeList() = default;
  RQ_ALWAYS_INLINE ConstNodeList(const Self &) = default;
  RQ_ALWAYS_INLINE ConstNodeList(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::NodeList<NodeListItem> &rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const NodeListItem *, const rq::Node<NodeListItem> *>>(rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::NodeList<NodeListItem> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const NodeListItem *, const rq::Node<NodeListItem> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsItem() const {
    return llvm::isa<const NodeListItem *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<const rq::Node<NodeListItem> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem &getItem() const {
    return rq::dereferencePtr(
        llvm::cast<const NodeListItem *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<NodeListItem> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<const rq::Node<NodeListItem> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstNodeListIterator<NodeListItem>
  cend() const;
};

template <typename NodeListItemParam> struct Node final {
  using NodeListItem = NodeListItemParam;
  using Self = rq::Node<NodeListItem>;

  NodeListItem *_item_ptr{nullptr};
  rq::NodeList<NodeListItem> _next{};

  RQ_ALWAYS_INLINE Node() = default;
  RQ_ALWAYS_INLINE Node(NodeListItem &item_a, NodeListItem &item_b)
      : _item_ptr(&item_a), _next(item_b) {}
  RQ_ALWAYS_INLINE Node(NodeListItem &item, rq::Node<NodeListItem> &node)
      : _item_ptr(&item), _next(node) {}
  RQ_ALWAYS_INLINE Node(NodeListItem &item,
                        const rq::NodeList<NodeListItem> &list)
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
  [[nodiscard]] RQ_ALWAYS_INLINE NodeListItem &getItem() {
    return rq::dereferencePtr(this->_item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem &getItem() const {
    return rq::dereferencePtr(this->_item_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NodeList<NodeListItem> &getNext() {
    return this->_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::NodeList<NodeListItem> &
  getNext() const {
    return this->_next;
  }
};

template <typename NodeListItemParam> struct NodeListIterator final {
  using NodeListItem = NodeListItemParam;
  using Self = rq::NodeListIterator<NodeListItem>;
  using value_type = NodeListItem;
  using reference = NodeListItem &;
  using pointer = NodeListItem *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::NodeList<NodeListItem> _list;

  RQ_ALWAYS_INLINE NodeListIterator() = default;
  RQ_ALWAYS_INLINE explicit NodeListIterator(rq::NodeList<NodeListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list = rq::NodeList<NodeListItem>();
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
  [[nodiscard]] RQ_ALWAYS_INLINE NodeListItem &operator*() {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE NodeListItem *operator->() {
    if (this->_list.getIsItem()) {
      return &this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return &this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem *operator->() const {
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

template <typename NodeListItemParam> struct ConstNodeListIterator final {
  using NodeListItem = NodeListItemParam;
  using Self = rq::ConstNodeListIterator<NodeListItem>;
  using value_type = const NodeListItem;
  using reference = const NodeListItem &;
  using pointer = NodeListItem *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ConstNodeList<NodeListItem> _list;

  RQ_ALWAYS_INLINE ConstNodeListIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstNodeListIterator(
      const rq::NodeList<NodeListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE explicit ConstNodeListIterator(
      const rq::ConstNodeList<NodeListItem> &list)
      : _list(list) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_list.getIsItem()) {
      this->_list = rq::ConstNodeList<NodeListItem>();
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
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem &operator*() const {
    if (this->_list.getIsItem()) {
      return this->_list.getItem();
    } else if (this->_list.getIsNode()) {
      return this->_list.getNode().getItem();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const NodeListItem *operator->() const {
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

template <typename NodeListItemParam>
inline rq::NodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::begin() {
  return rq::NodeListIterator<NodeListItemParam>(*this);
}

template <typename NodeListItemParam>
inline rq::NodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::end() {
  return rq::NodeListIterator<NodeListItemParam>();
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::begin() const {
  return rq::ConstNodeListIterator<NodeListItemParam>(*this);
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::end() const {
  return rq::ConstNodeListIterator<NodeListItemParam>();
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::cbegin() const {
  return rq::ConstNodeListIterator<NodeListItemParam>(*this);
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
NodeList<NodeListItemParam>::cend() const {
  return rq::ConstNodeListIterator<NodeListItemParam>();
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
ConstNodeList<NodeListItemParam>::begin() const {
  return rq::ConstNodeListIterator<NodeListItemParam>(*this);
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
ConstNodeList<NodeListItemParam>::end() const {
  return rq::ConstNodeListIterator<NodeListItemParam>();
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
ConstNodeList<NodeListItemParam>::cbegin() const {
  return rq::ConstNodeListIterator<NodeListItemParam>(*this);
}

template <typename NodeListItemParam>
inline rq::ConstNodeListIterator<NodeListItemParam>
ConstNodeList<NodeListItemParam>::cend() const {
  return rq::ConstNodeListIterator<NodeListItemParam>();
}

} // namespace rq
