#pragma once

#include <requite/scope.hpp>

namespace requite {

struct Expression;

struct Block final {
  using Self = Block;

  requite::Scope _scope = {};
  requite::Expression *_expression_ptr = nullptr;

  // block.cpp
  Block() = default;
  Block(const Self &) = delete;
  Block(Self &&) = delete;
  ~Block() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasContaining() const;
  void setContaining(requite::Scope &scope);
  [[nodiscard]] requite::Scope& getContaining();
  [[nodiscard]] const requite::Scope &getContaining() const;
};

} // namespace requite