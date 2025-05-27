#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct ConstSubScopeIterator final {
  using value_type = const requite::Scope;
  using reference = const requite::Scope &;
  using pointer = const requite::Scope *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const requite::Scope *_sub_scope_ptr = nullptr;

  ConstSubScopeIterator() = default;
  ConstSubScopeIterator(const requite::Scope &sub_scope);

  requite::ConstSubScopeIterator &operator++();

  requite::ConstSubScopeIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ConstSubScopeIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ConstSubScopeIterator &it) const;

  const requite::Scope &operator*() const;

  const requite::Scope *operator->() const;
};

} // namespace requite