// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Expression;

struct ExpressionIterator final {
  using value_type = requite::Expression;
  using reference = requite::Expression &;
  using pointer = requite::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  requite::Expression *_expression_ptr = nullptr;

  ExpressionIterator() = default;
  ExpressionIterator(requite::Expression *expression_ptr);

  requite::ExpressionIterator &operator++();

  requite::ExpressionIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ExpressionIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ExpressionIterator &it) const;

  requite::Expression &operator*();

  const requite::Expression &operator*() const;

  requite::Expression *operator->();

  const requite::Expression *operator->() const;

  const bool getIsDone() const;
};

} // namespace requite