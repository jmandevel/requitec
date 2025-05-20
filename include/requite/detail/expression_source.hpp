// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/token.hpp>

namespace requite {

bool Expression::getHasSourceText() const {
  return this->_source_text_ptr != nullptr;
}

bool Expression::getIsInsertedInSource() const {
  return this->_source_text_length == 0;
}

llvm::StringRef Expression::getSourceText() const {
  REQUITE_ASSERT(this->_source_text_ptr != nullptr);
  return llvm::StringRef(this->_source_text_ptr, this->_source_text_length);
}

const char *Expression::getSourceTextPtr() const {
  return this->_source_text_ptr;
}

unsigned Expression::getSourceTextLength() const {
  return this->_source_text_length;
}

llvm::SMLoc Expression::getSourceStartLlvmLocation() const {
  REQUITE_ASSERT(this->_source_text_ptr != nullptr);
  return llvm::SMLoc::getFromPointer(this->_source_text_ptr);
}

llvm::SMLoc Expression::getSourceEndLlvmLocation() const {
  REQUITE_ASSERT(this->_source_text_ptr != nullptr);
  return llvm::SMLoc::getFromPointer(this->_source_text_ptr +
                                     this->_source_text_length);
}

llvm::SMRange Expression::getSourceLlvmRange() const {
  return llvm::SMRange(this->getSourceStartLlvmLocation(),
                       this->getSourceEndLlvmLocation());
}

void Expression::setSource(const requite::Token &token) {
  this->_source_text_ptr = token.getSourceTextPtr();
  this->_source_text_length = token.getSourceTextLength();
}

void Expression::setSource(const requite::Token &first,
                           const requite::Token &last) {
  REQUITE_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr());
  this->_source_text_ptr = first.getSourceTextPtr();
  this->_source_text_length = static_cast<unsigned>(last.getSourceTextPtr() -
                                                    first.getSourceTextPtr()) +
                              last.getSourceTextLength();
}

void Expression::setSource(const requite::Expression &expression) {
  this->_source_text_ptr = expression.getSourceTextPtr();
  this->_source_text_length = expression.getSourceTextLength();
}

void Expression::setSource(const requite::Expression &first,
                           requite::Expression &last) {
  REQUITE_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr());
  this->_source_text_ptr = first.getSourceTextPtr();
  this->_source_text_length = static_cast<unsigned>(last.getSourceTextPtr() -
                                                    first.getSourceTextPtr()) +
                              last.getSourceTextLength();
}

void Expression::setSource(const requite::Expression &first,
                           const requite::Token &last) {
  REQUITE_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr());
  this->_source_text_ptr = first.getSourceTextPtr();
  this->_source_text_length = static_cast<unsigned>(last.getSourceTextPtr() -
                                                    first.getSourceTextPtr()) +
                              last.getSourceTextLength();
}

void Expression::setSource(const requite::Token &first,
                           const requite::Expression &last) {
  REQUITE_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr());
  this->_source_text_ptr = first.getSourceTextPtr();
  this->_source_text_length = static_cast<unsigned>(last.getSourceTextPtr() -
                                                    first.getSourceTextPtr()) +
                              last.getSourceTextLength();
}

void Expression::extendSourceOver(const requite::Token &token) {
  REQUITE_ASSERT(this->_source_text_ptr != nullptr);
  REQUITE_ASSERT(token.getSourceTextPtr() >= this->_source_text_ptr);
  if (token.getSourceTextPtr() >= this->_source_text_ptr) {
    this->_source_text_length = static_cast<unsigned>(token.getSourceTextPtr() -
                                                      this->_source_text_ptr) +
                                token.getSourceTextLength();
  } else {
    this->_source_text_ptr = token.getSourceTextPtr();
    this->_source_text_length =
        static_cast<unsigned>(this->_source_text_ptr -
                              token.getSourceTextPtr()) +
        this->_source_text_length;
  }
}

void Expression::extendSourceOver(const requite::Expression &expression) {
  REQUITE_ASSERT(this->_source_text_ptr != nullptr);
  if (expression.getSourceTextPtr() >= this->_source_text_ptr) {
    this->_source_text_length =
        static_cast<unsigned>(expression.getSourceTextPtr() -
                              this->_source_text_ptr) +
        expression.getSourceTextLength();
  } else {
    this->_source_text_ptr = expression.getSourceTextPtr();
    this->_source_text_length =
        static_cast<unsigned>(this->_source_text_ptr -
                              expression.getSourceTextPtr()) +
        this->_source_text_length;
  }
}

void Expression::setSourceInsertedAt(const char *text_ptr) {
  this->_source_text_ptr = text_ptr;
  this->_source_text_length = 0;
}

void Expression::setSourceInsertedBefore(const requite::Token &token) {
  REQUITE_ASSERT(token.getSourceTextPtr() != nullptr);
  this->_source_text_ptr = token.getSourceTextPtr();
  this->_source_text_length = 0;
}

void Expression::setSourceInsertedBefore(
    const requite::Expression &expression) {
  REQUITE_ASSERT(expression.getSourceTextPtr() != nullptr);
  this->_source_text_ptr = expression.getSourceTextPtr();
  this->_source_text_length = 0;
}

void Expression::setSourceInsertedAfter(const requite::Token &token) {
  REQUITE_ASSERT(token.getSourceTextPtr() != nullptr);
  this->_source_text_ptr =
      token.getSourceTextPtr() + token.getSourceTextLength();
  this->_source_text_length = 0;
}

void Expression::setSourceInsertedAfter(const requite::Expression &expression) {
  REQUITE_ASSERT(expression.getSourceTextPtr() != nullptr);
  this->_source_text_ptr =
      expression.getSourceTextPtr() + expression.getSourceTextLength();
  this->_source_text_length = 0;
}

} // namespace requite