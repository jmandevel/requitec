#pragma once

#include <requite/scope.hpp>
#include <requite/attribute_flags.hpp>
#include <requite/block_type.hpp>

namespace requite {

struct Expression;

struct Block final {
    using Self = requite::Block;

    requite::BlockType _type = requite::BlockType::NONE;
    requite::Scope _scope = {};
    requite::Expression *_expression_ptr = nullptr;
    requite::AttributeFlags _attributes = {};

    // block.cpp
    Block();
    Block(const Self&) = delete;
    Block(Self&&) = delete;
    ~Block() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] requite::BlockType getType();
    void setType(requite::BlockType type);
    [[nodiscard]] requite::Scope& getScope();
    [[nodiscard]] const requite::Scope& getScope() const;
    [[nodiscard]] bool getHasContainingScope() const;
    void setContainingScope(requite::Scope &scope);
    [[nodiscard]] requite::Scope &getContainingScope();
    [[nodiscard]] const requite::Scope &getContainingScope() const;
    [[nodiscard]] bool getHasExpression() const;
    void setExpression(requite::Expression& expression);
    [[nodiscard]] const requite::Expression& getExpression() const;
    [[nodiscard]] requite::Expression& getExpression();
    [[nodiscard]] requite::AttributeFlags &getAttributeFlags();
    [[nodiscard]] requite::AttributeFlags getAttributeFlags() const;
};

}