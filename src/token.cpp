// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/context.hpp>
#include <requite/token.hpp>

#include <cstdint>

namespace requite {

Token::Token(requite::TokenType type, unsigned line, unsigned column,
             const char *text_ptr, unsigned length,
             requite::TokenSpacing spacing)
    : _type(type), _line(line), _column(column), _source_text_ptr(text_ptr),
      _source_text_length(length), _spacing(spacing) {}

requite::TokenType Token::getType() const { return this->_type; }

unsigned Token::getLine() const { return this->_line; }

unsigned Token::getColumn() const { return this->_column; }

requite::TokenSpacing Token::getSpacing() const { return this->_spacing; }

bool Token::getHasBinaryOperatorSpacing() const {
  return this->_spacing == requite::TokenSpacing::NONE ||
         this->_spacing == requite::TokenSpacing::BEFORE_AND_AFTER ||
         this->_spacing == requite::TokenSpacing::AFTER;
}

bool Token::getHasUnaryOperatorSpacing() const {
  return this->_spacing == requite::TokenSpacing::BEFORE ||
         this->_spacing == requite::TokenSpacing::NONE;
}

bool Token::getHasInvalidOperatorSpacing() const {
  return this->_spacing == requite::TokenSpacing::AFTER;
}

llvm::StringRef Token::getSourceText() const {
  return llvm::StringRef(this->_source_text_ptr, this->_source_text_length);
}

const char *Token::getSourceTextPtr() const { return this->_source_text_ptr; }

unsigned Token::getSourceTextLength() const {
  return this->_source_text_length;
}

void Token::dropFrontAndBack() {
  REQUITE_ASSERT(this->getSourceTextLength() >= 2);
  this->_source_text_ptr++;
  this->_source_text_length -= 2;
}

void Token::dropFront() {
  REQUITE_ASSERT(this->getSourceTextLength() >= 1);
  this->_source_text_ptr++;
  this->_source_text_length -= 1;
}

void Token::dropBack() {
  REQUITE_ASSERT(this->getSourceTextLength() >= 1);
  this->_source_text_length -= 1;
}

void Token::setUnmatched() { this->_type = requite::getUnmatched(this->_type); }

llvm::SMLoc Token::getSourceStart() const {
  return llvm::SMLoc::getFromPointer(this->_source_text_ptr);
}

llvm::SMLoc Token::getSourceEnd() const {
  return llvm::SMLoc::getFromPointer(this->_source_text_ptr +
                                     this->_source_text_length);
}

llvm::SMRange Token::getSourceRange() const {
  return llvm::SMRange(this->getSourceStart(), this->getSourceEnd());
}

} // namespace requite