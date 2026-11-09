#pragma once

#include <rq/utility.hpp>

#include <llvm/Support/Casting.h>

#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>

namespace rq {

template <typename StateParam, typename NextPolicyParam, typename ValueParam>
struct Iterator final {
  using State = StateParam;
  using NextPolicy = NextPolicyParam;
  using Value = ValueParam;
  using Self = rq::Iterator<State, NextPolicy, Value>;
  using value_type = Value;
  using reference = Value &;
  using pointer = Value *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  State _state{};

  Iterator() = default;
  explicit Iterator(const State &state) : _state(state) {}
  Iterator(const Self &) = default;
  Iterator(Self &&) = default;
  ~Iterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;

  RQ_ALWAYS_INLINE Self &operator++() {
    NextPolicy{}.next(this->_state);
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self temp = *this;
    ++*this;
    return temp;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_state == it._state;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_state != it._state;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Value &operator*() {
    return NextPolicy{}.value(this->_state);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value &operator*() const {
    return NextPolicy{}.value(this->_state);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE Value *operator->() {
    return std::addressof(NextPolicy{}.value(this->_state));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const Value *operator->() const {
    return std::addressof(NextPolicy{}.value(this->_state));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return NextPolicy{}.getIsDone(this->_state);
  }
};

template <typename BaseParam, typename DerivedParam>
struct NextIteratorPolicy final {
  using Base = BaseParam;
  using Derived = DerivedParam;

  static_assert(std::derived_from<Derived, Base>);

  RQ_ALWAYS_INLINE void next(Base *&inout_state_ptr) const {
    Base &state = rq::dereferencePtr(inout_state_ptr);
    inout_state_ptr = state->_next_ptr;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE Derived &
  value(Base *const &inout_state_ptr) const {
    Base &state = rq::dereferencePtr(inout_state_ptr);
    return llvm::cast<Derived>(state);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsDone(Base *const &inout_state_ptr) const {
    return inout_state_ptr == nullptr;
  }
};

template <typename BaseParam, typename DerivedParam>
struct ConstNextIteratorPolicy final {
  using Base = BaseParam;
  using Derived = DerivedParam;

  static_assert(std::derived_from<Derived, Base>);

  RQ_ALWAYS_INLINE void next(const Base *&inout_state_ptr) const {
    const Base &state = rq::dereferencePtr(inout_state_ptr);
    inout_state_ptr = state->_next_ptr;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const Derived &
  value(const Base *const &inout_state_ptr) const {
    const Base &state = rq::dereferencePtr(inout_state_ptr);
    return llvm::cast<const Derived>(state);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsDone(const Base *const &inout_state_ptr) const {
    return inout_state_ptr == nullptr;
  }
};

template <typename BeginParam, typename EndParam = BeginParam>
using Subrange = std::ranges::subrange<BeginParam, EndParam,
                                       std::ranges::subrange_kind::unsized>;

template <typename BaseParam, typename DerivedParam = BaseParam>
using NextIterator =
    rq::Iterator<BaseParam *, rq::NextIteratorPolicy<BaseParam, DerivedParam>,
                 DerivedParam>;

template <typename BaseParam, typename DerivedParam = BaseParam>
using ConstNextIterator =
    rq::Iterator<const BaseParam *,
                 rq::ConstNextIteratorPolicy<BaseParam, DerivedParam>,
                 const DerivedParam>;

template <typename BaseParam, typename DerivedParam = BaseParam>
using NextSubrange = rq::Subrange<rq::NextIterator<BaseParam, DerivedParam>>;

template <typename BaseParam, typename DerivedParam = BaseParam>
using ConstNextSubrange =
    rq::Subrange<rq::ConstNextIterator<BaseParam, DerivedParam>>;

} // namespace rq
