// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/expression.hpp>
#include <requite/parser.hpp>
#include <requite/precedence_parser.hpp>

namespace requite {

void PrecedenceParser::parseUnary(requite::Parser &parser,
                                  requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(token);
  this->appendBranch(operation);
  if (!this->getHasOuter()) {
    this->_outer_ptr = &operation;
  }
  this->_operation_ptr = &operation;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseBinary(requite::Parser &parser,
                                   requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &old_operation = this->getOperation();
    requite::Expression &new_operation =
        requite::Expression::makeOperation(opcode);
    new_operation.setSource(old_operation, token);
    new_operation.setBranch(old_operation);
    this->_outer_ptr = &new_operation;
    this->_operation_ptr = &new_operation;
    this->_last_ptr = &old_operation;
    return;
  }
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  if (this->getHasLast()) {
    requite::Expression &last = this->getLast();
    operation.setSource(last, token);
    operation.setBranch(last);
  } else {
    operation.setSource(token);
  }
  this->_outer_ptr = &operation;
  this->_operation_ptr = &operation;
}

void PrecedenceParser::parseBinaryCombination(requite::Parser &parser,
                                              requite::Opcode opcode) {
  this->_last_ptr = this->_outer_ptr;
  this->_operation_ptr = nullptr;
  this->parseBinary(parser, opcode);
}

void PrecedenceParser::parseNary(requite::Parser &parser,
                                 requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &old_operation = this->getOperation();
    if (old_operation.getOpcode() == opcode) {
      return;
    }
    requite::Expression &new_operation =
        requite::Expression::makeOperation(opcode);
    new_operation.setSource(old_operation, token);
    new_operation.setBranch(old_operation);
    this->_outer_ptr = &new_operation;
    this->_operation_ptr = &new_operation;
    this->_last_ptr = &old_operation;
    return;
  }
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  if (this->getHasLast()) {
    requite::Expression &last = this->getLast();
    operation.setSource(last, token);
    operation.setBranch(last);
  } else {
    operation.setSource(token);
  }
  this->_outer_ptr = &operation;
  this->_operation_ptr = &operation;
}

void PrecedenceParser::parseNestedNary(requite::Parser &parser,
                                       requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &old_operation = this->getOperation();
    if (old_operation.getOpcode() == opcode) {
      return;
    }
  }
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  if (this->getHasLast()) {
    requite::Expression &last = this->getLast();
    operation.setSource(last, token);
    last.setBranch(operation);
  } else if (this->getHasOperation()) {
    operation.setSource(token);
    requite::Expression &old_operation = this->getOperation();
    old_operation.extendSourceOver(token);
    old_operation.setBranch(operation);
  } else {
    operation.setSource(token);
  }
  this->_operation_ptr = &operation;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseAttribute(requite::Parser &parser,
                                      requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &old_operation = this->getOperation();
    if (old_operation.getOpcode() != requite::Opcode::_ASCRIBE_LAST_BRANCH) {
      requite::Expression &new_operation =
          requite::Expression::makeOperation(requite::Opcode::_ASCRIBE_LAST_BRANCH);
      new_operation.setSource(old_operation, token);
      this->appendBranch(new_operation);
      if (!this->getHasOuter()) {
        this->_outer_ptr = &new_operation;
      }
      this->_operation_ptr = &new_operation;
      this->_last_ptr = nullptr;
    }
  } else {
    requite::Expression &operation =
        requite::Expression::makeOperation(requite::Opcode::_ASCRIBE_LAST_BRANCH);
    if (this->getHasLast()) {
      requite::Expression &last = this->getLast();
      operation.setSource(last);
      operation.setBranch(last);
    } else {
      operation.setSource(token);
    }
    if (!this->getHasOuter()) {
      this->_outer_ptr = &operation;
    }
    this->_operation_ptr = &operation;
  }
  requite::Expression &attribute = requite::Expression::makeOperation(opcode);
  attribute.setSource(token);
  this->appendBranch(attribute);
  requite::Expression &ascribe = this->getOperation();
  REQUITE_ASSERT(ascribe.getOpcode() == requite::Opcode::_ASCRIBE_LAST_BRANCH);
  ascribe.extendSourceOver(attribute);
}

void PrecedenceParser::parseHorned(requite::Parser &parser,
                                   requite::Opcode opcode,
                                   requite::TokenType right_token) {
  REQUITE_ASSERT(!parser.getIsDone());
  const requite::Token &left_token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression *second_ptr =
      parser.parseBranches(left_token, right_token);
  parser.incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(this->getLast(), left_token);
  operation.setBranch(this->getLast());
  if (!parser.getIsDone()) {
    const requite::Token &right_token = parser.getToken();
    operation.extendSourceOver(right_token);
  }
  this->_outer_ptr = &operation;
  this->_operation_ptr = &operation;
  this->getLast().setNextPtr(second_ptr);
}

bool PrecedenceParser::getHasOuter() const {
  return this->_outer_ptr != nullptr;
}

void PrecedenceParser::appendBranch(requite::Expression &branch) {
  if (!this->getHasOuter()) {
    this->_outer_ptr = &branch;
  }
  if (this->getHasLast()) {
    this->getLast().setNext(branch);
  }
  if (this->getHasOperation()) {
    requite::Expression &operation = this->getOperation();
    if (!this->getHasLast()) {
      operation.setBranch(branch);
    }
    operation.extendSourceOver(branch);
  }
  this->_last_ptr = &branch;
}

const requite::Expression &PrecedenceParser::getOuter() const {
  return requite::getRef(this->_outer_ptr);
}

requite::Expression &PrecedenceParser::getOuter() {
  return requite::getRef(this->_outer_ptr);
}

bool PrecedenceParser::getHasOperation() const {
  return this->_operation_ptr != nullptr;
}

const requite::Expression &PrecedenceParser::getOperation() const {
  return requite::getRef(this->_operation_ptr);
}

requite::Expression &PrecedenceParser::getOperation() {
  return requite::getRef(this->_operation_ptr);
}

bool PrecedenceParser::getHasLast() const { return this->_last_ptr != nullptr; }

const requite::Expression &PrecedenceParser::getLast() const {
  return requite::getRef(this->_last_ptr);
}

requite::Expression &PrecedenceParser::getLast() {
  return requite::getRef(this->_last_ptr);
}

bool PrecedenceParser::getHasUnary() const {
  return this->getHasOuter() && !this->getHasLast();
}

const requite::Expression &PrecedenceParser::getUnary() const {
  REQUITE_ASSERT(this->getHasUnary());
  return requite::getRef(this->_operation_ptr);
}

requite::Expression &PrecedenceParser::getUnary() {
  REQUITE_ASSERT(this->getHasUnary());
  return requite::getRef(this->_operation_ptr);
}

} // namespace requite
