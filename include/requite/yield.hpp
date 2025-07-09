#pragma once

#include <requite/attribute_status.hpp>
#include <requite/situation.hpp>

namespace requite {

struct Expression;
struct Object;
struct Procedure;
struct Table;
struct Property;
struct Global;
struct Alias;
struct BaseOrTableBlock;
struct ObjectBlock;

enum class YieldType {
  NONE,
  OBJECT,
  PROCEDURE,
  TABLE,
  PROPERTY,
  GLOBAL,
  ALIAS,
  BASE_OR_TABLE_BLOCK,
  OBJECT_BLOCK,
  EXPAND_STATEMENT
};

struct Yield final {
  using Self = Yield;

  requite::YieldType _type;
  requite::Situation _situation;
  requite::AttributeStatus _attribute_status;
  union {
    void *_nothing_ptr = nullptr;
    requite::Object *_object_ptr;
    requite::Procedure *_procedure_ptr;
    requite::Table *_table_ptr;
    requite::Property *_property_ptr;
    requite::Global *_global_ptr;
    requite::Alias *_alias_ptr;
    requite::BaseOrTableBlock *_base_or_table_block_ptr;
    requite::ObjectBlock *_object_block_ptr;
    requite::Expression *_expand_statement_ptr;
  };

  // yield.cpp
  Yield(requite::Object &object, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Procedure &procedure, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Table &table, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Property &property, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Global &global, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Alias &alias, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::BaseOrTableBlock &block, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::ObjectBlock &block, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(requite::Expression &expression, requite::Situation situation,
        requite::AttributeStatus status);
  Yield(const Yield &) = delete;
  Yield(Yield &&) = delete;
  ~Yield() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] requite::YieldType getType() const;
  [[nodiscard]] requite::Object &getObject();
  [[nodiscard]] const requite::Object &getObject() const;
  [[nodiscard]] requite::Procedure &getProcedure();
  [[nodiscard]] requite::Procedure &getProcedure() const;
  [[nodiscard]] requite::Table &getTable();
  [[nodiscard]] const requite::Table &getTable() const;
  [[nodiscard]] requite::Global &getGlobal();
  [[nodiscard]] const requite::Global &getGlobal() const;
  [[nodiscard]] requite::Alias &getAlias();
  [[nodiscard]] const requite::Alias &getAlias() const;
  [[nodiscard]] requite::BaseOrTableBlock &getBaseOrTableBlock();
  [[nodiscard]] const requite::BaseOrTableBlock &getBaseOrTableBlock() const;
  [[nodiscard]] requite::ObjectBlock &getObjectBlock();
  [[nodiscard]] const requite::ObjectBlock &getObjectBlock() const;
  [[nodiscard]] requite::Expression &getExpandExpression();
  [[nodiscard]] const requite::Expression &getExpandExpression() const;
  [[nodiscard]] requite::AttributeStatus getAttributeStatus() const;
  void incrementAttributeStatus();
};

} // namespace requite