#pragma once

#include <requite/scope.hpp>
#include <requite/symbol_status.hpp>

namespace requite {

struct Expression;

struct Block final {
  using Self = Block;

  requite::Scope _scope = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::SymbolStatus _symbol_status = requite::SymbolStatus::EXPAND_NAME;

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
  [[nodiscard]] requite::SymbolStatus getSymbolStatus() const;
  void incrementSymbolStatus();
};

} // namespace requite