// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Expression;

struct ConstExpressionIterator final {
  using value_type = const requite::Expression;
  using reference = const requite::Expression &;
  using pointer = const requite::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const requite::Expression *_expression_ptr = nullptr;

  ConstExpressionIterator() = default;
  ConstExpressionIterator(const requite::Expression *expression_ptr);
  requite::ConstExpressionIterator &operator++();

  requite::ConstExpressionIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ConstExpressionIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ConstExpressionIterator &it) const;

  const requite::Expression &operator*() const;

  const requite::Expression *operator->() const;
};

} // namespace requite