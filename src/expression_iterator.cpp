// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/expression_iterator.hpp>

namespace requite {

ExpressionIterator::ExpressionIterator(requite::Expression *expression_ptr)
    : _expression_ptr(expression_ptr) {}

requite::ExpressionIterator &ExpressionIterator::operator++() {
  requite::Expression &expression = requite::getRef(this->_expression_ptr);
  this->_expression_ptr = expression._next_ptr;
  return *this;
}

requite::ExpressionIterator ExpressionIterator::operator++(int) {
  REQUITE_ASSERT(this->_expression_ptr != nullptr);
  requite::ExpressionIterator temp = *this;
  ++(*this);
  return temp;
}

bool ExpressionIterator::operator==(
    const requite::ExpressionIterator &it) const {
  return this->_expression_ptr == it._expression_ptr;
}

bool ExpressionIterator::operator!=(
    const requite::ExpressionIterator &it) const {
  return this->_expression_ptr != it._expression_ptr;
}

requite::Expression &ExpressionIterator::operator*() {
  return requite::getRef(this->_expression_ptr);
}

const requite::Expression &ExpressionIterator::operator*() const {
  return requite::getRef(this->_expression_ptr);
}

requite::Expression *ExpressionIterator::operator->() {
  return this->_expression_ptr;
}

const requite::Expression *ExpressionIterator::operator->() const {
  return this->_expression_ptr;
}

} // namespace requite