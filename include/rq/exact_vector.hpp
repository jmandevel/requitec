#pragma once

#include <rq/utility.hpp>

#include <cstddef>
#include <iterator>

namespace rq {

template <typename ElementParam> struct ExactVector;
template <typename ElementParam> struct ExactVectorIterator;
template <typename ElementParam> struct ConstExactVectorIterator;

// a fixed-size vector that does not resize
template <typename ElementParam> struct ExactVector final {
  using Element = ElementParam;
  using Self = rq::ExactVector<Element>;

  Element *_data_ptr{nullptr};
  std::size_t _size{0};

  RQ_ALWAYS_INLINE ExactVector() = default;
  RQ_ALWAYS_INLINE ExactVector(Element *data_ptr, std::size_t size)
      : _data_ptr(data_ptr), _size(size) {}
  RQ_ALWAYS_INLINE ~ExactVector() = default;
  RQ_ALWAYS_INLINE ExactVector(const Self &) = default;
  RQ_ALWAYS_INLINE ExactVector(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_size == 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getSize() const {
    return this->_size;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element *getData() { return this->_data_ptr; }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *getData() const {
    return this->_data_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &get(std::size_t index) {
    RQ_ASSERT(index < this->_size, "index out of bounds");
    return this->_data_ptr[index];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &get(std::size_t index) const {
    RQ_ASSERT(index < this->_size, "index out of bounds");
    return this->_data_ptr[index];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &operator[](std::size_t index) {
    return this->get(index);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &
  operator[](std::size_t index) const {
    return this->get(index);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &getFront() {
    RQ_ASSERT(this->_size > 0, "vector is empty");
    return this->_data_ptr[0];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &getFront() const {
    RQ_ASSERT(this->_size > 0, "vector is empty");
    return this->_data_ptr[0];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &getBack() {
    RQ_ASSERT(this->_size > 0, "vector is empty");
    return this->_data_ptr[this->_size - 1];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &getBack() const {
    RQ_ASSERT(this->_size > 0, "vector is empty");
    return this->_data_ptr[this->_size - 1];
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_data_ptr == rhs._data_ptr && this->_size == rhs._size;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return !(*this == rhs);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExactVectorIterator<Element> begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExactVectorIterator<Element> end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstExactVectorIterator<Element>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstExactVectorIterator<Element>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstExactVectorIterator<Element>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstExactVectorIterator<Element>
  cend() const;
};

template <typename ElementParam> struct ExactVectorIterator final {
  using Element = ElementParam;
  using Self = rq::ExactVectorIterator<Element>;
  using value_type = Element;
  using reference = Element &;
  using pointer = Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  Element *_ptr{nullptr};

  RQ_ALWAYS_INLINE ExactVectorIterator() = default;
  RQ_ALWAYS_INLINE explicit ExactVectorIterator(Element *ptr) : _ptr(ptr) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    ++this->_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  RQ_ALWAYS_INLINE Self &operator--() {
    --this->_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator--(int) {
    Self backup = *this;
    --(*this);
    return backup;
  }
  RQ_ALWAYS_INLINE Self &operator+=(difference_type n) {
    this->_ptr += n;
    return *this;
  }
  RQ_ALWAYS_INLINE Self &operator-=(difference_type n) {
    this->_ptr -= n;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Self operator+(difference_type n) const {
    return Self(this->_ptr + n);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Self operator-(difference_type n) const {
    return Self(this->_ptr - n);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE difference_type
  operator-(const Self &rhs) const {
    return this->_ptr - rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr == rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr != rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator<(const Self &rhs) const {
    return this->_ptr < rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator<=(const Self &rhs) const {
    return this->_ptr <= rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator>(const Self &rhs) const {
    return this->_ptr > rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator>=(const Self &rhs) const {
    return this->_ptr >= rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &operator*() {
    return rq::dereferencePtr(this->_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element *operator->() { return this->_ptr; }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Element &operator[](difference_type n) {
    return this->_ptr[n];
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &
  operator[](difference_type n) const {
    return this->_ptr[n];
  }
};

template <typename ElementParam> struct ConstExactVectorIterator final {
  using Element = ElementParam;
  using Self = rq::ConstExactVectorIterator<Element>;
  using value_type = const Element;
  using reference = const Element &;
  using pointer = const Element *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  const Element *_ptr{nullptr};

  RQ_ALWAYS_INLINE ConstExactVectorIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstExactVectorIterator(const Element *ptr)
      : _ptr(ptr) {}
  RQ_ALWAYS_INLINE ConstExactVectorIterator(
      const rq::ExactVectorIterator<Element> &it)
      : _ptr(it._ptr) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    ++this->_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  RQ_ALWAYS_INLINE Self &operator--() {
    --this->_ptr;
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator--(int) {
    Self backup = *this;
    --(*this);
    return backup;
  }
  RQ_ALWAYS_INLINE Self &operator+=(difference_type n) {
    this->_ptr += n;
    return *this;
  }
  RQ_ALWAYS_INLINE Self &operator-=(difference_type n) {
    this->_ptr -= n;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Self operator+(difference_type n) const {
    return Self(this->_ptr + n);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Self operator-(difference_type n) const {
    return Self(this->_ptr - n);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE difference_type
  operator-(const Self &rhs) const {
    return this->_ptr - rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr == rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr != rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator<(const Self &rhs) const {
    return this->_ptr < rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator<=(const Self &rhs) const {
    return this->_ptr <= rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator>(const Self &rhs) const {
    return this->_ptr > rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator>=(const Self &rhs) const {
    return this->_ptr >= rhs._ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &operator*() const {
    return rq::dereferencePtr(this->_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element *operator->() const {
    return this->_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Element &
  operator[](difference_type n) const {
    return this->_ptr[n];
  }
};

template <typename ElementParam>
inline rq::ExactVectorIterator<ElementParam>
ExactVector<ElementParam>::begin() {
  return rq::ExactVectorIterator<ElementParam>(this->_data_ptr);
}

template <typename ElementParam>
inline rq::ExactVectorIterator<ElementParam> ExactVector<ElementParam>::end() {
  return rq::ExactVectorIterator<ElementParam>(this->_data_ptr + this->_size);
}

template <typename ElementParam>
inline rq::ConstExactVectorIterator<ElementParam>
ExactVector<ElementParam>::begin() const {
  return rq::ConstExactVectorIterator<ElementParam>(this->_data_ptr);
}

template <typename ElementParam>
inline rq::ConstExactVectorIterator<ElementParam>
ExactVector<ElementParam>::end() const {
  return rq::ConstExactVectorIterator<ElementParam>(this->_data_ptr +
                                                    this->_size);
}

template <typename ElementParam>
inline rq::ConstExactVectorIterator<ElementParam>
ExactVector<ElementParam>::cbegin() const {
  return rq::ConstExactVectorIterator<ElementParam>(this->_data_ptr);
}

template <typename ElementParam>
inline rq::ConstExactVectorIterator<ElementParam>
ExactVector<ElementParam>::cend() const {
  return rq::ConstExactVectorIterator<ElementParam>(this->_data_ptr +
                                                    this->_size);
}

} // namespace rq