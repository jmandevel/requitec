#pragma once

#include <requite/scope.hpp>
#include <requite/attribute_flags.hpp>

namespace requite {

struct Local;
struct Expression;

struct LocalBlock final {
  using Self = LocalBlock;

  requite::Scope _scope = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::Local *_first_local_ptr = nullptr;
  requite::AttributeFlags _attribute_flags = {};

  // local_block.cpp
  LocalBlock() = default;
  LocalBlock(const Self &) = delete;
  LocalBlock(Self &&) = delete;
  ~LocalBlock() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasLocal() const;
  void addLocal(requite::Local &local);
  [[nodiscard]] requite::Local &getFirstLocal();
  [[nodiscard]] const requite::Local &getFirstLocal() const;
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
};

} // namespace requite