// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/expression_walker.hpp>

namespace requite {

inline requite::ExpressionWalker Expression::walkBranch() {
  return requite::ExpressionWalker(this->_branch_ptr);
}

inline requite::ExpressionWalker Expression::walkHorizontal() {
  return requite::ExpressionWalker(this);
}

} // namespace requite