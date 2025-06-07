// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>

namespace requite {

inline void Expression::clear() {
  this->_opcode = requite::Opcode::__NONE;
  this->_next_ptr = nullptr;
  this->_branch_ptr = nullptr;
  this->_source_text_ptr = nullptr;
  this->_source_text_length = 0;
  this->_data.emplace<std::monostate>();
}

bool Expression::getHasBranch() const { return this->_branch_ptr != nullptr; }

bool Expression::getHasNext() const { return this->_next_ptr != nullptr; }

bool Expression::getHasOneBranch() const {
  if (this->_branch_ptr == nullptr) {
    return false;
  }
  return this->_branch_ptr->_next_ptr == nullptr;
}

bool Expression::getHasOneNext() const {
  if (this->_next_ptr == nullptr) {
    return false;
  }
  return this->_next_ptr->_next_ptr == nullptr;
}

unsigned Expression::getBranchCount() const {
  const requite::Expression *expression_ptr = this->_branch_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_branch_ptr;
  }
  return count;
}

unsigned Expression::getNextCount() const {
  const requite::Expression *expression_ptr = this->_next_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_next_ptr;
  }
  return count;
}

requite::Expression &Expression::getBranch() {
  return requite::getRef(this->_branch_ptr);
}

const requite::Expression &Expression::getBranch() const {
  return requite::getRef(this->_branch_ptr);
}

requite::Expression *Expression::getBranchPtr() { return this->_branch_ptr; }

const requite::Expression *Expression::getBranchPtr() const {
  return this->_branch_ptr;
}

requite::Expression &Expression::getBranch(unsigned branch_i) {
  requite::Expression *expression_ptr = this->_branch_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_branch_ptr;
    if (count == branch_i) {
      return requite::getRef(expression_ptr);
    }
  }
  REQUITE_UNREACHABLE();
}

requite::Expression &Expression::getLastBranch() {
  REQUITE_ASSERT(this->getHasBranch());
  requite::Expression *expression_ptr = this->_branch_ptr;
  while (expression_ptr->_next_ptr != nullptr) {
    expression_ptr = expression_ptr->_next_ptr;
  }
  return requite::getRef(expression_ptr);
}

const requite::Expression &Expression::getLastBranch() const {
  const requite::Expression *expression_ptr = this;
  while (expression_ptr->_branch_ptr != nullptr) {
    expression_ptr = expression_ptr->_branch_ptr;
  }
  return requite::getRef(expression_ptr);
}

const requite::Expression &Expression::getBranch(unsigned branch_i) const {
  const requite::Expression *expression_ptr = this->_branch_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_branch_ptr;
    if (count == branch_i) {
      return requite::getRef(expression_ptr);
    }
  }
  REQUITE_UNREACHABLE();
}

requite::Expression &Expression::getNext() {
  return requite::getRef(this->_next_ptr);
}

const requite::Expression &Expression::getNext() const {
  return requite::getRef(this->_next_ptr);
}

requite::Expression *Expression::getNextPtr() { return this->_next_ptr; }

const requite::Expression *Expression::getNextPtr() const {
  return this->_next_ptr;
}

requite::Expression &Expression::getNext(unsigned next_i) {
  requite::Expression *expression_ptr = this->_next_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_next_ptr;
    if (count == next_i) {
      return requite::getRef(expression_ptr);
    }
  }
  REQUITE_UNREACHABLE();
}

const requite::Expression &Expression::getNext(unsigned next_i) const {
  const requite::Expression *expression_ptr = this->_next_ptr;
  unsigned count = 0;
  while (expression_ptr != nullptr) {
    ++count;
    expression_ptr = expression_ptr->_next_ptr;
    if (count == next_i) {
      return requite::getRef(expression_ptr);
    }
  }
  REQUITE_UNREACHABLE();
}

requite::Expression &Expression::getLastNext() {
  requite::Expression *expression_ptr = this;
  while (expression_ptr->_next_ptr != nullptr) {
    expression_ptr = expression_ptr->_next_ptr;
  }
  return requite::getRef(expression_ptr);
}

const requite::Expression &Expression::getLastNext() const {
  const requite::Expression *expression_ptr = this;
  while (expression_ptr->_next_ptr != nullptr) {
    expression_ptr = expression_ptr->_next_ptr;
  }
  return requite::getRef(expression_ptr);
}

requite::Expression &Expression::getUnascribed() {
  if (this->getOpcode() == requite::Opcode::_ASCRIBE) {
    return this->getLastBranch().getUnascribed();
  }
  return *this;
}

const requite::Expression &Expression::getUnascribed() const {
  if (this->getOpcode() == requite::Opcode::_ASCRIBE) {
    return this->getLastBranch().getUnascribed();
  }
  return *this;
}

void Expression::setBranch(requite::Expression &branch) {
  REQUITE_ASSERT(this->getIsOperation());
  REQUITE_ASSERT(this->_branch_ptr == nullptr);
  this->_branch_ptr = &branch;
}

inline void Expression::setBranchPtr(requite::Expression *branch_ptr) {
  this->_branch_ptr = branch_ptr;
}

void Expression::setNext(requite::Expression &next) {
  REQUITE_ASSERT(this->_next_ptr == nullptr);
  this->_next_ptr = &next;
}

void Expression::setNextPtr(requite::Expression *next_ptr) {
  this->_next_ptr = next_ptr;
}

requite::Expression &
Expression::replaceBranch(requite::Expression &replacement) {
  REQUITE_ASSERT(this->_branch_ptr != nullptr);
  REQUITE_ASSERT(this->_branch_ptr != &replacement);
  requite::Expression *old_branch_ptr = this->_branch_ptr;
  this->_branch_ptr = &replacement;
  return requite::getRef(old_branch_ptr);
}

requite::Expression &Expression::replaceNext(requite::Expression &replacement) {
  REQUITE_ASSERT(this->_next_ptr != nullptr);
  REQUITE_ASSERT(this->_next_ptr != &replacement);
  requite::Expression *old_next_ptr = this->_next_ptr;
  this->_next_ptr = &replacement;
  return requite::getRef(old_next_ptr);
}

requite::Expression &Expression::popBranch() {
  REQUITE_ASSERT(this->_branch_ptr != nullptr);
  requite::Expression *old_branch_ptr = this->_branch_ptr;
  this->_branch_ptr = nullptr;
  return *old_branch_ptr;
}

inline requite::Expression *Expression::popBranchPtr() {
  requite::Expression *old_branch_ptr = this->_branch_ptr;
  this->_branch_ptr = nullptr;
  return old_branch_ptr;
}

requite::Expression &Expression::popNext() {
  REQUITE_ASSERT(this->_next_ptr != nullptr);
  requite::Expression *old_next_ptr = this->_next_ptr;
  this->_next_ptr = nullptr;
  return *old_next_ptr;
}

requite::Expression *Expression::popNextPtr() {
  requite::Expression *old_next_ptr = this->_next_ptr;
  this->_next_ptr = nullptr;
  return old_next_ptr;
}

void Expression::mergeBranch() {
  requite::Expression &branch = this->popBranch();
  if (this->getHasNext()) {
    requite::Expression &branch_last_next = branch.getLastNext();
    branch_last_next.setNext(this->popNext());
  }
  this->clear();
  this->setOpcode(branch.getOpcode());
  if (branch.getHasBranch()) {
    this->setBranch(branch.popBranch());
  }
  if (branch.getHasNext()) {
    this->setNext(branch.popNext());
  }
  this->setSource(branch);
  if (branch.getHasDataText()) {
    if (this->getHasDataText()) {
      this->changeDataText(branch.getDataText());
    } else {
      this->setDataText(branch.getDataText());
    }
  }
  delete &branch;
}

inline void
Expression::replaceWithRecursiveCopy(requite::Expression &replacement) {
  if (this->getHasBranch()) {
    requite::Expression &branch = this->popBranch();
    requite::Expression::deleteExpression(branch);
  }
  if (this->getHasNext()) {
    requite::Expression &next = this->popNext();
    requite::Expression::deleteExpression(next);
  }
  if (replacement.getHasBranch()) {
    requite::Expression &branch = replacement.getBranch();
    this->setBranch(requite::Expression::copyExpression(branch));
  }
  if (replacement.getHasNext()) {
    requite::Expression &next = replacement.getNext();
    this->setNext(requite::Expression::copyExpression(next));
  }
  this->changeOpcode(replacement.getOpcode());
  this->setSource(replacement);
  this->setDataText(replacement.getDataText());
}

} // namespace requite
