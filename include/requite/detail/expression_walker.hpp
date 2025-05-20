// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/expression.hpp>
#include <requite/unreachable.hpp>

namespace requite {

ExpressionWalker::ExpressionWalker(requite::Expression *expression_ptr)
    : _expression_ptr(expression_ptr) {}

bool ExpressionWalker::getHasExpression() const {
  return this->_expression_ptr != nullptr;
}

requite::Expression &ExpressionWalker::getExpression() {
  REQUITE_ASSERT(this->_expression_ptr != nullptr);
  return *this->_expression_ptr;
}

const requite::Expression &ExpressionWalker::getExpression() const {
  REQUITE_ASSERT(this->_expression_ptr != nullptr);
  return *this->_expression_ptr;
}

requite::Expression *ExpressionWalker::getExpressionPtr() {
  return this->_expression_ptr;
}

const requite::Expression *ExpressionWalker::getExpressionPtr() const {
  return this->_expression_ptr;
}

unsigned ExpressionWalker::getCount() const { return this->_count; }

requite::ExpressionWalkResult ExpressionWalker::getResult() const {
  requite::ExpressionWalkResult result(this->getCount(),
                                       !this->getHasExpression());
  return result;
}

requite::ExpressionWalker &ExpressionWalker::doOne(
    std::function<void(requite::Expression &branch)> predicate) {
  if (!this->getHasExpression()) {
    return *this;
  }
  predicate(*this->_expression_ptr);
  this->_count++;
  this->_expression_ptr = this->_expression_ptr->_next_ptr;
  return *this;
}

requite::ExpressionWalker &ExpressionWalker::doOne(
    std::function<void(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  if (!this->getHasExpression()) {
    return *this;
  }
  predicate(this->_count, *this->_expression_ptr);
  this->_count++;
  this->_expression_ptr = this->_expression_ptr->_next_ptr;
  return *this;
}

inline bool ExpressionWalker::tryOne(
    std::function<bool(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  if (!this->getHasExpression()) {
    return false;
  }
  const bool result = predicate(this->_count, *this->_expression_ptr);
  this->_count++;
  this->_expression_ptr = this->_expression_ptr->_next_ptr;
  return result;
}

inline bool ExpressionWalker::tryOne(
    std::function<bool(requite::Expression &branch)> predicate) {
  if (!this->getHasExpression()) {
    return false;
  }
  const bool result = predicate(*this->_expression_ptr);
  this->_count++;
  this->_expression_ptr = this->_expression_ptr->_next_ptr;
  return result;
}

template <unsigned COUNT_PARAM>
requite::ExpressionWalker &ExpressionWalker::doSome(
    std::function<void(requite::Expression &branch)> predicate) {
  if constexpr (COUNT_PARAM == 0) {
    return *this;
  } else {
    if (!this->getHasExpression()) {
      return *this;
    }
    constexpr unsigned NEXT_COUNT = COUNT_PARAM - 1;
    this->doOne(predicate);
    return this->doSome<NEXT_COUNT>(predicate);
  }
  REQUITE_UNREACHABLE();
}

template <unsigned COUNT_PARAM>
requite::ExpressionWalker &ExpressionWalker::doSome(
    std::function<void(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  if constexpr (COUNT_PARAM == 0) {
    return *this;
  } else {
    if (!this->getHasExpression()) {
      return *this;
    }
    constexpr unsigned NEXT_COUNT = COUNT_PARAM - 1;
    this->doOne(predicate);
    return this->doSome<NEXT_COUNT>(predicate);
  }
  REQUITE_UNREACHABLE();
}

requite::ExpressionWalkResult ExpressionWalker::doAll(
    std::function<void(requite::Expression &branch)> predicate) {
  while (this->getHasExpression()) {
    this->doOne(predicate);
  }
  return this->getResult();
}

requite::ExpressionWalkResult ExpressionWalker::doAll(
    std::function<void(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  while (this->getHasExpression()) {
    this->doOne(predicate);
  }
  return this->getResult();
}

requite::ExpressionWalker &ExpressionWalker::doUntilLast(
    std::function<void(requite::Expression &branch)> predicate) {
  while (this->getHasExpression() && this->getExpression().getHasNext()) {
    this->doOne(predicate);
  }
  return *this;
}

requite::ExpressionWalker &ExpressionWalker::doUntilLast(
    std::function<void(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  while (this->getHasExpression() && this->getExpression().getHasNext()) {
    this->doOne(predicate);
  }
  return *this;
}

inline requite::ExpressionWalker &ExpressionWalker::doUntilLastOrCondition(
    std::function<bool(requite::Expression &branch)> predicate) {
  while (this->getHasExpression() && this->getExpression().getHasNext()) {
    const bool result = this->tryOne(predicate);
    if (result) {
      break;
    }
  }
  return *this;
}

inline requite::ExpressionWalker &ExpressionWalker::doUntilLastOrCondition(
    std::function<bool(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  while (this->getHasExpression() && this->getExpression().getHasNext()) {
    const bool result = this->tryOne(predicate);
    if (result) {
      break;
    }
  }
  return *this;
}

requite::ExpressionWalkResult ExpressionWalker::doLast(
    std::function<void(requite::Expression &branch)> predicate) {
  REQUITE_ASSERT(!this->getHasExpression() ||
                 !this->getExpression().getHasNext());
  this->doOne(predicate);
  return this->getResult();
}

requite::ExpressionWalkResult ExpressionWalker::doLast(
    std::function<void(unsigned branch_i, requite::Expression &branch)>
        predicate) {
  REQUITE_ASSERT(!this->getHasExpression() ||
                 !this->getExpression().getHasNext());
  this->doOne(predicate);
  return this->getResult();
}

} // namespace requite