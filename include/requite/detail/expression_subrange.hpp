// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline std::ranges::subrange<requite::ExpressionIterator,
                             requite::ExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getHorizontalSubrange() {
  return std::ranges::subrange(requite::ExpressionIterator(this),
                               requite::ExpressionIterator());
}

inline std::ranges::subrange<requite::ConstExpressionIterator,
                             requite::ConstExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getHorizontalSubrange() const {
  return std::ranges::subrange(requite::ConstExpressionIterator(this),
                               requite::ConstExpressionIterator());
}

inline std::ranges::subrange<requite::ExpressionIterator,
                             requite::ExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getNextSubrange() {
  return std::ranges::subrange(requite::ExpressionIterator(this->getNextPtr()),
                               requite::ExpressionIterator());
}

inline std::ranges::subrange<requite::ConstExpressionIterator,
                             requite::ConstExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getNextSubrange() const {
  return std::ranges::subrange(
      requite::ConstExpressionIterator(this->getNextPtr()),
      requite::ConstExpressionIterator());
}

inline std::ranges::subrange<requite::ExpressionIterator,
                             requite::ExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getBranchSubrange() {
  return std::ranges::subrange(
      requite::ExpressionIterator(this->getBranchPtr()),
      requite::ExpressionIterator());
}

inline std::ranges::subrange<requite::ConstExpressionIterator,
                             requite::ConstExpressionIterator,
                             std::ranges::subrange_kind::unsized>
Expression::getBranchSubrange() const {
  return std::ranges::subrange(
      requite::ConstExpressionIterator(this->getBranchPtr()),
      requite::ConstExpressionIterator());
}

} // namespace requite