// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/expression.hpp>
#include <requite/parser.hpp>
#include <requite/precedence_parser.hpp>

namespace requite {

requite::Expression &
PrecedenceParser::parseUnary(requite::Parser &parser, requite::Opcode opcode,
                             requite::Expression *unary_ptr, unsigned count) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  unsigned i = 0;
  while (true) {
    requite::Expression &operation = requite::Expression::makeOperation(opcode);
    operation.setSource(token);
    if (unary_ptr == nullptr) {
      this->appendBranch(operation);
    } else {
      requite::getRef(unary_ptr).setBranch(operation);
    }
    unary_ptr = &operation;
    if (++i == count) {
      return operation;
    }
  }
  REQUITE_UNREACHABLE();
}

void PrecedenceParser::parseBinary(requite::Parser &parser,
                                   requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(this->getOuter(), token);
  operation.setBranchPtr(this->_outer_ptr);
  this->_last_branch_ptr = this->_outer_ptr;
  this->_outer_ptr = &operation;
}

void PrecedenceParser::parseNary(requite::Parser &parser,
                                 requite::Opcode opcode) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  if (this->getOuterOpcode() == opcode) {
    if (this->_outer_ptr->getHasBranch() && this->_last_branch_ptr == nullptr) {
      this->_last_branch_ptr =
          &requite::getRef(this->_outer_ptr).getLastBranch();
    }
    return;
  }
  requite::Expression &operation = requite::Expression::makeOperation(opcode);
  operation.setSource(this->getOuter(), token);
  operation.setBranchPtr(this->_outer_ptr);
  this->_last_branch_ptr = this->_outer_ptr;
  this->_outer_ptr = &operation;
}

void PrecedenceParser::parseAttribute(requite::Parser &parser,
                                      requite::Opcode opcode,
                                      bool new_ascribe_needed,
                                      requite::Expression *unary_ptr) {
  const requite::Token &token = parser.getToken();
  parser.incrementToken(1);
  requite::Expression &attribute = requite::Expression::makeOperation(opcode);
  attribute.setSource(token);
  if (!new_ascribe_needed) {
    this->appendBranch(attribute);
    return;
  }
  requite::Expression &operation =
      requite::Expression::makeOperation(requite::Opcode::_ASCRIBE);
  operation.setSource(token);
  operation.setBranch(attribute);
  if (unary_ptr != nullptr) {
    unary_ptr->setBranch(operation);
  } else {
    this->appendBranch(operation);
  }
  this->_last_branch_ptr = &attribute;
}

requite::Opcode PrecedenceParser::getOuterOpcode() const {
  if (this->_outer_ptr == nullptr) {
    return requite::Opcode::__NONE;
  }
  return requite::getRef(this->_outer_ptr).getOpcode();
}

void PrecedenceParser::setInitialOuter(requite::Expression &outer) {
  REQUITE_ASSERT(this->_last_branch_ptr == nullptr);
  requite::setSingleRef(this->_outer_ptr, outer);
}

void PrecedenceParser::appendBranch(requite::Expression &branch) {
  if (this->_outer_ptr == nullptr) {
    this->_outer_ptr = &branch;
    return;
  } else if (this->_last_branch_ptr == nullptr) {
    requite::getRef(this->_outer_ptr).setBranch(branch);
  } else {
    requite::getRef(this->_last_branch_ptr).setNext(branch);
  }
  this->_last_branch_ptr = &branch;
  requite::getRef(this->_outer_ptr).extendSourceOver(branch);
}

const requite::Expression &PrecedenceParser::getOuter() const {
  return requite::getRef(this->_outer_ptr);
}

requite::Expression &PrecedenceParser::getOuter() {
  return requite::getRef(this->_outer_ptr);
}

} // namespace requite