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

void PrecedenceParser::parseNaryAfterHorned(requite::Parser &parser,
                                            requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(this->getOuter(), token);
  operation.setBranch(this->getOuter());
  this->_operation_ptr = &operation;
  this->_last_ptr = this->_outer_ptr;
  this->_outer_ptr = &operation;
}

void PrecedenceParser::parseAttribute(requite::Parser &parser) {
  const requite::Token &at_token = parser.getToken();
  parser.incrementToken(1);
  const requite::Token &opcode_token = parser.getToken();
  requite::Opcode opcode = parser.parseOpcode();
  requite::Expression& attribute = requite::Expression::makeOperation(opcode);
  attribute.setSource(at_token, opcode_token);
  const requite::Token &left_token = parser.getToken();
  if (left_token.getType() == requite::TokenType::LEFT_PARENTHESIS_GROUPING) {
    parser.parseAttributeArguments(attribute);
  }
  this->parseAscribe(at_token);
  requite::Expression &ascribe = this->getOperation();
  ascribe.extendSourceOver(attribute);
  this->appendBranch(attribute);
}

void PrecedenceParser::parseUnaryAttribute(requite::Parser &parser,
                                      requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  this->parseAscribe(token);
  requite::Expression &attribute = requite::Expression::makeOperation(opcode);
  attribute.setSource(token);
  this->appendBranch(attribute);
  requite::Expression &ascribe = this->getOperation();
  REQUITE_ASSERT(ascribe.getOpcode() == requite::Opcode::_ASCRIBE_LAST_BRANCH);
  ascribe.extendSourceOver(attribute);
}

void PrecedenceParser::parseAscribe(const requite::Token& token) {
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
}

void PrecedenceParser::parseCallOrSignature(requite::Parser &parser) {
  REQUITE_ASSERT(!parser.getIsDone());
  requite::Expression& operation = parser.parseCallOrSignature(this->_last_ptr);
  this->_outer_ptr = &operation;
  this->_operation_ptr = &operation;
}

void PrecedenceParser::parseSpecialization(requite::Parser &parser) {
  REQUITE_ASSERT(!parser.getIsDone());
  requite::Expression& operation = parser.parseSpecialization(this->_last_ptr);
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
