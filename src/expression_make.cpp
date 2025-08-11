// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/expression.hpp>

namespace requite {

void Expression::deleteExpression(requite::Expression &expression)
{
    if (expression.getHasBranch()) {
        requite::Expression::deleteExpression(expression.getBranch());
    }
    if (expression.getHasNext()) {
        requite::Expression::deleteExpression(expression.getNext());
    }
    delete &expression;
}

requite::Expression& Expression::copyExpression(const requite::Expression& expression)
{
    requite::Expression& new_expression = requite::getRef(new requite::Expression());
    if (expression.getHasBranch()) {
        new_expression.setBranch(requite::Expression::copyExpression(expression.getBranch()));
    }
    if (expression.getHasNext()) {
        new_expression.setNext(requite::Expression::copyExpression(expression.getNext()));
    }
    new_expression._opcode = expression._opcode;
    new_expression._source_text_ptr = expression._source_text_ptr;
    new_expression._source_text_length = expression._source_text_length;
    new_expression._data = expression._data;
    return new_expression;
}

requite::Expression &Expression::makeError()
{
    requite::Expression &expression = requite::getRef(new requite::Expression());
    expression._opcode = requite::Opcode::__ERROR;
    return expression;
}

requite::Expression & Expression::makeOperation(requite::Opcode opcode)
{
    requite::Expression &expression = requite::getRef(new requite::Expression());
    expression._opcode = opcode;
    return expression;
}

requite::Expression &Expression::makeString(requite::SavedString text)
{
    requite::Expression &expression = requite::getRef(new requite::Expression());
    expression._opcode = requite::Opcode::__STRING_LITERAL;
    expression._data._text = text;
    return expression;
}

requite::Expression &Expression::makeCodeunit(requite::SavedString text)
{
    requite::Expression &expression = requite::getRef(new requite::Expression());
    expression._opcode = requite::Opcode::__CODEUNIT_LITERAL;
    expression._data._text = text;
    return expression;
}

requite::Expression& Expression::makeIdentifier(requite::SavedString text)
{
    requite::Expression& expression = requite::getRef(new requite::Expression());
    expression._opcode = requite::Opcode::__IDENTIFIER_LITERAL;
    expression._data._text = text;
    return expression;
}

}