#pragma once

#include <requite/scope.hpp>
#include <requite/symbol_status.hpp>

namespace requite {

struct Global;
struct Expression;

struct BaseOrTableBlock final {
  using Self = BaseOrTableBlock;

  requite::Scope _scope = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::Global *_first_global_ptr = nullptr;
  requite::AttributeFlags _attribute_flags = {};
  requite::SymbolStatus _status = requite::SymbolStatus::EXPAND_NAME;

  // base_or_global_block.cpp
  BaseOrTableBlock() = default;
  BaseOrTableBlock(const Self &) = delete;
  BaseOrTableBlock(Self &&) = delete;
  ~BaseOrTableBlock() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasGlobal() const;
  void addGlobal(requite::Global &global);
  [[nodiscard]] requite::Global &getFirstGlobal();
  [[nodiscard]] const requite::Global &getFirstGlobal() const;
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
};

} // namespace requite