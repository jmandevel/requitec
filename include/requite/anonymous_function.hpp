// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/scope.hpp>
#include <requite/signature.hpp>

#include <vector>

namespace requite {

struct Expression;
struct Local;
struct Module;

struct AnonymousFunction final {
  using Self = requite::AnonymousFunction;

  requite::Scope _scope = {};
  requite::Module* _module_ptr = nullptr;
  requite::Expression *_expression_ptr = nullptr;
  requite::Signature _signature = {};
  std::vector<requite::Local *> _captured_ptrs = {};

  // anonymous_function.cpp
  AnonymousFunction();
  AnonymousFunction(const Self &) = delete;
  AnonymousFunction(Self &&) = delete;
  ~AnonymousFunction() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] bool operator==(const Self &rhs) const;
  [[nodiscard]] bool operator!=(const Self &rhs) const;
  [[nodiscard]]
  requite::Scope &getScope();
  [[nodiscard]]
  const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope& scope);
  [[nodiscard]] requite::Scope& getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]]
  requite::Expression &getExpression();
  [[nodiscard]]
  const requite::Expression &getExpression() const;
  [[nodiscard]]
  requite::Signature &getSignature();
  [[nodiscard]]
  const requite::Signature &getSignature() const;
  [[nodiscard]]
  std::vector<requite::Local *> &getCapturedPtrs();
  [[nodiscard]]
  const std::vector<requite::Local *> &getCapturedPtrs() const;
};

} // namespace requite