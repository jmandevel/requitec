#pragma once

#include <requite/attribute_flags.hpp>

namespace requite {

struct Module;
struct Expression;
struct Table;
struct Scope;

struct TableUse final {
    using Self = requite::TableUse;

    requite::Expression *_expression_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    requite::Table* _used_table = nullptr;
    bool _is_exported = false;
    requite::TableUse *_next_ptr = nullptr;
    requite::Scope *_containing_scope_ptr = nullptr;
    requite::Module *_containing_module_uptr = nullptr;

    // table_use.cpp
    TableUse() = default;
    TableUse(const Self&) = delete;
    TableUse(Self&&) = delete;
    ~TableUse() = default;
    Self &operator=(const Self&) = delete;
    Self &operator=(Self&&) = delete;
    [[nodiscard]] bool operator==(const Self&) const;
    [[nodiscard]] bool operator!=(const Self&) const;
    [[nodiscard]] bool getHasExpression() const;
    void setExpression(requite::Expression& expression);
    [[nodiscard]] requite::Expression &getExpression();
    [[nodiscard]] const requite::Expression &getExpression() const;
    [[nodiscard]] bool getHasContainingModule() const;
    void setContainingModule(requite::Module &module);
    [[nodiscard]] requite::Module &getContainingModule();
    [[nodiscard]] const requite::Module &getContainingModule() const;
    [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
    [[nodiscard]] bool getIsExported();
    void setIsExported();
    [[nodiscard]] bool getHasContainingScope() const;
    void setContainingScope(requite::Scope& scope);
    [[nodiscard]] requite::Scope &getScope();
    [[nodiscard]] const requite::Scope &getScope() const;
};

}