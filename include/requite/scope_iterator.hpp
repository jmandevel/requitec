// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct ScopeIterator final {
  using value_type = requite::Scope;
  using reference = requite::Scope &;
  using pointer = requite::Scope *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  requite::Scope *_scope_ptr = nullptr;

  ScopeIterator() = default;
  ScopeIterator(requite::Scope &scope);

  requite::ScopeIterator &operator++();

  requite::ScopeIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ScopeIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ScopeIterator &it) const;

  requite::Scope &operator*();

  const requite::Scope &operator*() const;

  requite::Scope *operator->();

  const requite::Scope *operator->() const;
};

} // namespace requite