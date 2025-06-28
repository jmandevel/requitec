// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <ranges>

namespace requite {

struct Procedure;

struct ProcedureIterator final {
  using value_type = requite::Procedure;
  using reference = requite::Procedure &;
  using pointer = requite::Procedure *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  requite::Procedure *_procedure_ptr = nullptr;

  ProcedureIterator() = default;
  ProcedureIterator(requite::Procedure &procedure);

  requite::ProcedureIterator &operator++();

  requite::ProcedureIterator operator++(int);

  [[nodiscard]]
  bool operator==(const requite::ProcedureIterator &it) const;

  [[nodiscard]]
  bool operator!=(const requite::ProcedureIterator &it) const;

  requite::Procedure &operator*();

  const requite::Procedure &operator*() const;

  requite::Procedure *operator->();

  const requite::Procedure *operator->() const;
};

} // namespace requite