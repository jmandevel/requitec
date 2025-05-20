// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/const_expression_iterator.hpp>
#include <requite/expression.hpp>

namespace requite {

ConstExpressionIterator::ConstExpressionIterator(
    const requite::Expression &expression)
    : _expression_ptr(&expression) {}

requite::ConstExpressionIterator &ConstExpressionIterator::operator++() {
  const requite::Expression &expression =
      requite::getRef(this->_expression_ptr);
  this->_expression_ptr = expression._next_ptr;
  return *this;
}

requite::ConstExpressionIterator ConstExpressionIterator::operator++(int) {
  requite::ConstExpressionIterator temp = *this;
  ++(*this);
  return temp;
}

[[nodiscard]]
bool ConstExpressionIterator::operator==(
    const requite::ConstExpressionIterator &that) const {
  return this->_expression_ptr == that._expression_ptr;
}

[[nodiscard]]
bool ConstExpressionIterator::operator!=(
    const requite::ConstExpressionIterator &that) const {
  return this->_expression_ptr != that._expression_ptr;
}

const requite::Expression &ConstExpressionIterator::operator*() const {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression *ConstExpressionIterator::operator->() const {
  REQUITE_ASSERT(this->_expression_ptr != nullptr);
  return this->_expression_ptr;
}

} // namespace requite
