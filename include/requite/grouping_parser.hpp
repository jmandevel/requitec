#pragma once

#include <requite/opcode.hpp>

namespace requite {

struct Expression;
struct Token;

struct GroupingParser final {
    using Self = requite::GroupingParser;

    requite::Expression* _operation_ptr = nullptr;
    requite::Expression* _last_ptr = nullptr;

    // grouping_parser.cpp
    GroupingParser() = default;
    GroupingParser(const Self&) = delete;
    GroupingParser(Self&&) = delete;
    ~GroupingParser() = default;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;
    [[nodiscard]] bool getHasOperation() const;
    void setOperation(requite::Expression& operation);
    [[nodiscard]] requite::Expression& getOperation();
    [[nodiscard]] const requite::Expression &getOperation() const;
    [[nodiscard]] requite::Expression& getLast();
    [[nodiscard]] const requite::Expression &getLast() const;

    // grouping_parse.cpp
    void startGroup(requite::Expression& existing_expression);
    void appendBranch(requite::Expression& branch);
    void finishOperation(const requite::Token& last_token);
};

}