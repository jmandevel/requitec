#pragma once

#include <requite/attribute_flags.hpp>

#include <llvm/ADT/StringRef.h>

#include <string>

namespace requite {

struct Expression;
struct Table;
struct Scope;
struct Module;

struct TableAlias final {
    using Self = requite::TableAlias;

    std::string _name = {};
    requite::Expression *_expression_ptr = nullptr;
    requite::Table* _aliased_table_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    requite::Scope *_containing_scope_ptr = nullptr;
    requite::Module *_containing_module_ptr = nullptr;

    // table_alias.cpp
    TableAlias() = default;
    TableAlias(const Self&) = delete;
    TableAlias(Self&&) = delete;
    ~TableAlias() = default;
    Self &operator=(const Self&) = delete;
    Self &operator=(Self&&) = delete;
    [[nodiscard]] bool operator==(const Self&) const;
    [[nodiscard]] bool operator!=(const Self&) const;
    [[nodiscard]] bool getHasName() const;
    void setName(llvm::StringRef name);
    [[nodiscard]] llvm::StringRef getName() const;
    [[nodiscard]] bool getHasExpression() const;
    void setExpression(requite::Expression& expression);
    [[nodiscard]] requite::Expression &getExpresssion();
    [[nodiscard]] const requite::Expression &getExpression() const;
    [[nodiscard]] bool getHasAliasedTable() const;
    void setAliasedTable(requite::Table& table);
    [[nodiscard]] requite::Table &getAliasedTable();
    [[nodiscard]] const requite::Table &getAliasedTable() const;
    [[nodiscard]] bool getHasContainingScope() const;
    void setContainingScope(requite::Scope &scope);
    [[nodiscard]] requite::Scope &getContainingScope();
    [[nodiscard]] const requite::Scope &getContainingScope() const;
    [[nodiscard]] bool getHasContainingModule() const;
    void setContainingModule(requite::Module &module);
    [[nodiscard]] requite::Module& getContainingModule();
    [[nodiscard]] const requite::Module& getContainingModule() const;
    [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
    [[nodiscard]] const requite::AttributeFlags &getAttributeFlags() const;
};

}