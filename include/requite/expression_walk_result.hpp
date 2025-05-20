// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

struct ExpressionWalkResult final {
  unsigned _count;
  bool _is_done;

  inline ExpressionWalkResult(unsigned count, bool is_done);

  [[nodiscard]] inline unsigned getWalkedCount() const;

  [[nodiscard]] inline bool getIsDone() const;

  [[nodiscard]] inline bool getWalkedAtLeast(unsigned count);

  [[nodiscard]] inline bool getWalkedExactly(unsigned count);
};

} // namespace requite

#include <requite/detail/expression_walk_result.hpp>