#pragma once

#include <requite/scope.hpp>
#include <requite/attribute_flags.hpp>

namespace requite {

struct Property;
struct Global;
struct Expression;

struct ObjectBlock final {
  using Self = ObjectBlock;

  requite::Scope _scope = {};
  requite::Expression *_expression_ptr = nullptr;
  requite::Property *_first_property_ptr = nullptr;
  requite::Global *_first_global_ptr = nullptr;
  requite::AttributeFlags _attribute_flags = {};

  // object_block.cpp
  ObjectBlock() = default;
  ObjectBlock(const Self &) = delete;
  ObjectBlock(Self &&) = delete;
  ~ObjectBlock() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::Scope &getScope();
  [[nodiscard]] const requite::Scope &getScope() const;
  [[nodiscard]] bool getHasExpression() const;
  void setExpression(requite::Expression &expression);
  [[nodiscard]] requite::Expression &getExpression();
  [[nodiscard]] const requite::Expression &getExpression() const;
  [[nodiscard]] bool getHasProperty() const;
  void addProperty(requite::Property &property);
  [[nodiscard]] requite::Property &getFirstProperty();
  [[nodiscard]] const requite::Property &getFirstProperty() const;
  [[nodiscard]] bool getHasGlobal() const;
  void addGlobal(requite::Global &global);
  [[nodiscard]] requite::Global &getFirstGlobal();
  [[nodiscard]] const requite::Global &getFirstGlobal() const;
  [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
};

} // namespace requite