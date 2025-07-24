#pragma once 

#include <requite/attribute_flags.hpp>
#include <requite/user_symbol.hpp>

namespace requite {

struct Expression;
struct Scope;
struct Module;

struct Use final {
    using Self = requite::Use;

    requite::UserSymbol _user_symbol = {};
    requite::Expression* _expression_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    requite::Scope* _containing_scope_ptr = nullptr;
    
    // use.cpp
    Use() = default;
    Use(const Self&) = delete;
    Use(Self&&) = delete;
    ~Use() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] bool operator==(const Self&) const;
    [[nodiscard]] bool operator!=(const Self&) const;
    [[nodiscard]] bool getHasExpression() const;
    void setExpression(requite::Expression& expression);
    [[nodiscard]] requite::Expression& getExpression();
    [[nodiscard]] const requite::Expression& getExpression() const;
    [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
    [[nodiscard]] requite::AttributeFlags getAttributeFlags() const;
    [[nodiscard]] bool getHasContainingScope() const;
    void setContainingScope(requite::Scope& scope);
    [[nodiscard]] requite::Scope &getContainingScope();
    [[nodiscard]] const requite::Scope &getContainingScope() const;
    [[nodiscard]] requite::UserSymbol& getUserSymbol();
    [[nodiscard]] const requite::UserSymbol &getUserSymbol() const;
    [[nodiscard]] bool getHasContainingModule() const;
    void setContainingModule(requite::Module& module);
    [[nodiscard]] requite::Module &getContainingModule();
    [[nodiscard]] const requite::Module &getContainingModule() const;
};

}