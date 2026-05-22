#pragma once

#include <rq/utility.hpp>

#include <concepts>
#include <type_traits>

namespace rq {

template <typename BaseParam, typename DerivedParam = BaseParam>
struct NextIterator final {
  using Base = BaseParam;
  using Derived = DerivedParam;
  using Element = DerivedParam;
  using Self = rq::NextIterator<Base, Derived>;
  using value_type = Element;
  using reference = Element &;
  using pointer = Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using NextPtr = decltype(std::declval<Element>()._next_ptr);
  using Next = std::remove_pointer_t<decltype(std::declval<Element>()._next_ptr)>;

  static_assert(std::is_pointer_v<NextPtr>);
  static_assert(std::derived_from<Derived, Base>);
  static_assert(std::same_as<Next, Base>);

  Element *_cur_ptr{nullptr};

  NextIterator() = default;
  explicit RQ_ALWAYS_INLINE NextIterator(Base *cur_ptr)
      : _cur_ptr(llvm::cast<Element>(cur_ptr)) {}
  NextIterator(const Self &) = default;
  NextIterator(Self &&) = default;
  ~NextIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++() {
    this->_cur_ptr =
        llvm::cast<Element>(rq::dereferencePtr(this->_cur_ptr)._next_ptr);
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    rq::NextIterator<Element> temp = *this;
    this->_cur_ptr =
        llvm::cast<Element>(rq::dereferencePtr(this->_cur_ptr)._next_ptr);
    return temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_cur_ptr == it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_cur_ptr != it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &operator*() {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element *operator->() {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_cur_ptr != nullptr;
  }
};

template <typename ElementParam>
struct NextIterator<ElementParam, ElementParam> final {
  using Base = ElementParam;
  using Derived = ElementParam;
  using Element = ElementParam;
  using Self = rq::NextIterator<Base, Derived>;
  using value_type = Element;
  using reference = Element &;
  using pointer = Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using NextPtr = decltype(std::declval<Element>()._next_ptr);
  using Next = std::remove_pointer_t<decltype(std::declval<Element>()._next_ptr)>;

  static_assert(std::is_pointer_v<NextPtr>);
  static_assert(std::derived_from<Derived, Base>);
  static_assert(std::same_as<Next, Base>);

  Element *_cur_ptr{nullptr};

  NextIterator() = default;
  explicit RQ_ALWAYS_INLINE NextIterator(Element *cur_ptr)
      : _cur_ptr(cur_ptr) {}
  NextIterator(const Self &) = default;
  NextIterator(Self &&) = default;
  ~NextIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++() {
    this->_cur_ptr = rq::dereferencePtr(this->_cur_ptr)._next_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    rq::NextIterator<Element> temp = *this;
    this->_cur_ptr = rq::dereferencePtr(this->_cur_ptr)._next_ptr;
    return temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_cur_ptr == it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_cur_ptr != it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &operator*() {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element *operator->() {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_cur_ptr != nullptr;
  }
};

template <typename BaseParam, typename DerivedParam = BaseParam>
struct ConstNextIterator final {
  using Base = BaseParam;
  using Derived = DerivedParam;
  using Element = DerivedParam;
  using Self = rq::ConstNextIterator<Base, Derived>;
  using value_type = Element;
  using reference = Element &;
  using pointer = Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using NextPtr = std::remove_cv_t<decltype(std::declval<Element>()._next_ptr)>;
  using Next = std::remove_pointer_t<decltype(std::declval<Element>()._next_ptr)>;

  static_assert(std::is_pointer_v<NextPtr>);
  static_assert(std::derived_from<Derived, Base>);
  static_assert(std::same_as<Next, Base>);

  const Element *_cur_ptr{nullptr};

  ConstNextIterator() = default;
  explicit RQ_ALWAYS_INLINE ConstNextIterator(const Base *cur_ptr)
      : _cur_ptr(llvm::cast<Element>(cur_ptr)) {}
  ConstNextIterator(const Self &) = default;
  ConstNextIterator(Self &&) = default;
  ~ConstNextIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++() {
    this->_cur_ptr =
        llvm::cast<Element>(rq::dereferencePtr(this->_cur_ptr)._next_ptr);
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    rq::NextIterator<Element> temp = *this;
    this->_cur_ptr =
        llvm::cast<Element>(rq::dereferencePtr(this->_cur_ptr)._next_ptr);
    return temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_cur_ptr == it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_cur_ptr != it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_cur_ptr != nullptr;
  }
};

template <typename ElementParam>
struct ConstNextIterator<ElementParam, ElementParam> final {
  using Base = ElementParam;
  using Derived = ElementParam;
  using Element = ElementParam;
  using Self = rq::ConstNextIterator<Element>;
  using value_type = const Element;
  using reference = const Element &;
  using pointer = const Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using NextPtr = std::remove_cv_t<decltype(std::declval<Element>()._next_ptr)>;
  using Next = std::remove_pointer_t<decltype(std::declval<Element>()._next_ptr)>;

  static_assert(std::is_pointer_v<NextPtr>);
  static_assert(std::derived_from<Derived, Base>);
  static_assert(std::same_as<Next, Base>);

  const Element *_cur_ptr{nullptr};

  ConstNextIterator() = default;
  explicit RQ_ALWAYS_INLINE ConstNextIterator(const Element *cur_ptr)
      : _cur_ptr(cur_ptr) {}
  ConstNextIterator(const Self &) = default;
  ConstNextIterator(Self &&) = default;
  ~ConstNextIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++() {
    this->_cur_ptr = rq::dereferencePtr(this->_cur_ptr)._next_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    rq::ConstNextIterator<Element> temp = *this;
    this->_cur_ptr = rq::dereferencePtr(this->_cur_ptr)._next_ptr;
    return temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_cur_ptr == it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_cur_ptr != it._cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_cur_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_cur_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_cur_ptr != nullptr;
  }
};

} // namespace rq