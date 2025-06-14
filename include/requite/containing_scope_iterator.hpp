// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Scope;

struct ContainingScopeIterator final {
  using value_type = requite::Scope;
  using reference = requite::Scope &;
  using pointer = requite::Scope *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  requite::Scope *_scope_ptr = nullptr;

  ContainingScopeIterator() = default;
  ContainingScopeIterator(requite::Scope &scope);

  requite::ContainingScopeIterator &operator++();

  requite::ContainingScopeIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ContainingScopeIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ContainingScopeIterator &it) const;

  requite::Scope &operator*();

  const requite::Scope &operator*() const;

  requite::Scope *operator->();

  const requite::Scope *operator->() const;
};

} // namespace requite