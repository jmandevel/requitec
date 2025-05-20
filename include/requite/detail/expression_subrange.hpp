// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline std::ranges::subrange<requite::ExpressionIterator,
                             requite::ExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getHorizontalSubrange() {
  return std::ranges::subrange(requite::ExpressionIterator(*(this)),
                               requite::ExpressionIterator());
}

inline std::ranges::subrange<requite::ConstExpressionIterator,
                             requite::ConstExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getHorizontalSubrange() const {
  return std::ranges::subrange(
      requite::ConstExpressionIterator(*(this)),
      requite::ConstExpressionIterator());
}

inline std::ranges::subrange<requite::ExpressionIterator,
                             requite::ExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getBranchSubrange() {
  return std::ranges::subrange(
      requite::ExpressionIterator(*(this->_branch_ptr)),
      requite::ExpressionIterator());
}

inline std::ranges::subrange<requite::ConstExpressionIterator,
                             requite::ConstExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getBranchSubrange() const {
  return std::ranges::subrange(
      requite::ConstExpressionIterator(*(this->_branch_ptr)),
      requite::ConstExpressionIterator());
}

} // namespace requite