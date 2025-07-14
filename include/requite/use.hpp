#pragma once 

#include <requite/attribute_flags.hpp>

namespace requite {

struct Expression;
struct Scope;

struct Use final {
    using Self = requite::Use;

    requite::Expression* _expression_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    requite::Scope* _scope_ptr = nullptr;
    requite::Use* _next_ptr = nullptr;

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
    [[nodiscard]] bool getHasScope() const;
    void setScope(requite::Scope& scope);
    [[nodiscard]] requite::Scope &getScope();
    [[nodiscard]] const requite::Scope &getScope() const;
    [[nodiscard]] bool getHasNext() const;
    void setNext(requite::Use &next);
    [[nodiscard]] requite::Use &getNext();
    [[nodiscard]] const requite::Use &getNext() const;
};

}