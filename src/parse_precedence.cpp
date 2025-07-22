// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/expression.hpp>
#include <requite/parser.hpp>
#include <requite/precedence_parser.hpp>

namespace requite {

void PrecedenceParser::parseDoubleUnary(requite::Parser &parser,
                                        requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &operation0 = requite::Expression::makeOperation(opcode);
  operation0.setSource(token);
  this->appendBranch(operation0);
  this->_operation_ptr = &operation0;
  this->_last_ptr = nullptr;
  requite::Expression &operation1 = requite::Expression::makeOperation(opcode);
  operation1.setSource(token);
  this->appendBranch(operation1);
  this->_operation_ptr = &operation1;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseUnary(requite::Parser &parser,
                                  requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(token);
  this->appendBranch(operation);
  this->_operation_ptr = &operation;
  this->_last_ptr = nullptr;
}

void PrecedenceParser::parseBinary(requite::Parser &parser,
                                   requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &new_operation =
      requite::Expression::makeOperation(opcode);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceParser::parseBinaryCombination(requite::Parser &parser,
                                              requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &new_operation =
      requite::Expression::makeOperation(opcode);
  new_operation.setSource(this->getOuter(), token);
  new_operation.setBranch(this->getOuter());
  this->_last_ptr = this->_outer_ptr;
  this->_operation_ptr = nullptr;
  this->_outer_ptr = &new_operation;
}

void PrecedenceParser::parseNary(requite::Parser &parser,
                                 requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &existing_operation = this->getOperation();
    if (existing_operation.getOpcode() == opcode) {
      // the existing operation already has this opcode, so we can keep
      // appending to this one
      this->appendRecent();
      return;
    }
  }
  // need to make a new operation of this opcode because one does not exist yet
  requite::Expression &new_operation =
      requite::Expression::makeOperation(opcode);
  new_operation.setSource(this->getRecent(), token);
  this->appendBranch(new_operation);
  this->_operation_ptr = &new_operation;
  this->_last_ptr = nullptr;
  this->appendRecent();
}

void PrecedenceParser::parseNestedNary(requite::Parser &parser,
                                       requite::Opcode opcode) {}

void PrecedenceParser::parseAttribute(requite::Parser &parser,
                                      requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getHasOperation()) {
    requite::Expression &old_operation = this->getOperation();
    if (old_operation.getOpcode() != requite::Opcode::_ASCRIBE_LAST_BRANCH) {
      requite::Expression &new_operation = requite::Expression::makeOperation(
          requite::Opcode::_ASCRIBE_LAST_BRANCH);
      new_operation.setSource(old_operation, token);
      this->appendBranch(new_operation);
      if (!this->getHasOuter()) {
        this->_outer_ptr = &new_operation;
      }
      this->_operation_ptr = &new_operation;
      this->_last_ptr = nullptr;
    }
  } else {
    requite::Expression &operation = requite::Expression::makeOperation(
        requite::Opcode::_ASCRIBE_LAST_BRANCH);
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
  if (this->getHasOperation()) {
    operation.setBranch(this->getOperation());
    this->getOperation().setNextPtr(second_ptr);
  } else {
    operation.setBranch(this->getLast());
    this->getLast().setNextPtr(second_ptr);
  }
  if (!parser.getIsDone()) {
    const requite::Token &right_token = parser.getToken();
    operation.extendSourceOver(right_token);
  }
  this->_outer_ptr = &operation;
  this->_operation_ptr = &operation;
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

void PrecedenceParser::setRecent(requite::Expression &branch) {
  requite::setSingleRef(this->_recent_ptr, branch);
}

void PrecedenceParser::appendRecent() {
  this->appendBranch(this->getRecent());
  this->_recent_ptr = nullptr;
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

bool PrecedenceParser::getHasRecent() const {
  return this->_recent_ptr != nullptr;
}

const requite::Expression &PrecedenceParser::getRecent() const {
  return requite::getRef(this->_recent_ptr);
}

requite::Expression &PrecedenceParser::getRecent() {
  return requite::getRef(this->_recent_ptr);
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
