// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/expression_walk_result.hpp>

#include <functional>

namespace requite {

struct Expression;

struct ExpressionWalker final {
  using Self = requite::ExpressionWalker;

  requite::Expression *_expression_ptr = nullptr;
  unsigned _count = 0;

  ExpressionWalker() = default;
  inline ExpressionWalker(requite::Expression *expression_ptr);

  [[nodiscard]] inline bool getHasExpression() const;

  [[nodiscard]] inline requite::Expression &getExpression();

  [[nodiscard]] inline const requite::Expression &getExpression() const;

  [[nodiscard]] inline requite::Expression *getExpressionPtr();

  [[nodiscard]] inline const requite::Expression *getExpressionPtr() const;

  [[nodiscard]] inline unsigned getCount() const;

  [[nodiscard]] inline requite::ExpressionWalkResult getResult() const;

  inline Self &
  doOne(std::function<void(requite::Expression &branch)> predicate);

  inline bool
  tryOne(std::function<bool(unsigned branch_i, requite::Expression &branch)>
             predicate);

  inline bool
  tryOne(std::function<bool(requite::Expression &branch)> predicate);

  inline Self &
  doOne(std::function<void(unsigned branch_i, requite::Expression &branch)>
            predicate);

  template <unsigned COUNT_PARAM>
  inline Self &
  doSome(std::function<void(requite::Expression &branch)> predicate);

  template <unsigned COUNT_PARAM>
  inline Self &
  doSome(std::function<void(unsigned branch_i, requite::Expression &branch)>
             predicate);

  inline requite::ExpressionWalkResult
  doAll(std::function<void(requite::Expression &branch)> predicate);

  inline requite::ExpressionWalkResult
  doAll(std::function<void(unsigned branch_i, requite::Expression &branch)>
            predicate);

  inline Self &
  doUntilLast(std::function<void(requite::Expression &branch)> predicate);

  inline Self &doUntilLast(
      std::function<void(unsigned branch_i, requite::Expression &branch)>
          predicate);

  inline Self &doUntilLastOrCondition(
      std::function<bool(requite::Expression &branch)> predicate);

  inline Self &doUntilLastOrCondition(
      std::function<bool(unsigned branch_i, requite::Expression &branch)>
          predicate);

  inline requite::ExpressionWalkResult
  doLast(std::function<void(requite::Expression &branch)> predicate);

  inline requite::ExpressionWalkResult
  doLast(std::function<void(unsigned branch_i, requite::Expression &branch)>
             predicate);
};

} // namespace requite

#include <requite/detail/expression_walker.hpp>