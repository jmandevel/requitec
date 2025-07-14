#pragma once

#include <requite/attribute_flags.hpp>

namespace requite {

struct Expression;
struct Module;

struct Import final {
    using Self = requite::Import;

    requite::Expression *_expression_ptr = nullptr;
    requite::Module* _module_ptr = nullptr;
    requite::AttributeFlags _attributes = {};
    requite::Import* _next_ptr = nullptr;

    // import.cpp
    Import() = default;
    Import(const Self&) = delete;
    Import(Self&&) = delete;
    ~Import() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] bool operator==(const Self&) const;
    [[nodiscard]] bool operator!=(const Self&) const;
    [[nodiscard]] bool getHasExpression() const;
    void setExpression(requite::Expression& expression);
    [[nodiscard]] requite::Expression& getExpression();
    [[nodiscard]] const requite::Expression& getExpression() const;
    [[nodiscard]] bool getHasModule() const;
    void setModule(requite::Module& module);
    [[nodiscard]] requite::Module& getModule();
    [[nodiscard]] const requite::Module &getModule() const;
    [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
    [[nodiscard]] requite::AttributeFlags getAttributeFlags() const;
    [[nodiscard]] bool getHasNext() const;
    void setNext(requite::Import& next);
    [[nodiscard]] requite::Import &getNext();
    [[nodiscard]] const requite::Import &getNext() const;
};

}