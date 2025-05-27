#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct SubScopeIterator final {
  using value_type = requite::Scope;
  using reference = requite::Scope &;
  using pointer = requite::Scope *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  requite::Scope *_sub_scope_ptr = nullptr;

  SubScopeIterator() = default;
  SubScopeIterator(requite::Scope &sub_scope);

  requite::SubScopeIterator &operator++();

  requite::SubScopeIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::SubScopeIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::SubScopeIterator &it) const;

  requite::Scope &operator*();

  requite::Scope *operator->();
};

} // namespace requite