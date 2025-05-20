// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/codeunits.hpp>
#include <requite/source_ranger.hpp>

namespace requite {

SourceRanger::SourceRanger(llvm::StringRef text)
    : _start(text.begin()), _current(text.begin()), _end(text.end()), _line(1),
      _column(1), _sub_line(0), _sub_column(0), _sub_start(nullptr) {
  REQUITE_ASSERT(*this->_end == 0x00);
}

bool SourceRanger::getIsDone() const { return this->_current >= this->_end; }

const char &SourceRanger::getChar(std::ptrdiff_t offset) const {
  REQUITE_ASSERT(offset >= 0);
  if ((this->_current + offset) >= this->_end) {
    REQUITE_ASSERT(*this->_end == 0x00);
    return *this->_end;
  }
  return this->_current[offset];
}

const char &SourceRanger::getSubChar(std::ptrdiff_t offset) const {
  REQUITE_ASSERT(offset >= 0);
  if ((this->_sub_start + offset) >= this->_end) {
    REQUITE_ASSERT(*this->_end == 0x00);
    return *this->_end;
  }
  return this->_sub_start[offset];
}

const char &SourceRanger::getPreviousChar(std::ptrdiff_t offset) const {
  REQUITE_ASSERT(offset < 0);
  if ((this->_current + offset) < this->_start) {
    REQUITE_ASSERT(*this->_end == 0x00);
    return *this->_end;
  }
  return this->_current[offset];
}

const char &SourceRanger::getPreviousSubChar(std::ptrdiff_t offset) const {
  REQUITE_ASSERT(offset < 0);
  if ((this->_sub_start + offset) < this->_start) {
    REQUITE_ASSERT(*this->_end == 0x00);
    return *this->_end;
  }
  return this->_sub_start[offset];
}

void SourceRanger::incrementChar(std::ptrdiff_t offset) {
  this->_current += offset;
}

void SourceRanger::startSubToken() {
  this->_sub_line = this->_line;
  this->_sub_column = this->_column;
  this->_sub_start = this->_current;
}

requite::Token SourceRanger::getSubToken(requite::TokenType type) {
  REQUITE_ASSERT(this->_sub_start != nullptr);
  REQUITE_ASSERT(this->_current > this->_sub_start);
  const char before = this->getPreviousSubChar(-1);
  const char after = this->getChar(1);
  requite::TokenSpacing spacing = requite::getSpacing(before, after);
  requite::Token token(type, this->_sub_line, this->_sub_column,
                       this->_sub_start, this->_current - this->_sub_start,
                       spacing);
  return token;
}

requite::Token SourceRanger::getLengthToken(requite::TokenType type,
                                            std::uint_fast32_t length) {
  REQUITE_ASSERT((this->_current + length) <= this->_end);
  const char before = this->getPreviousChar(-1);
  const char after = this->getChar(length);
  requite::TokenSpacing spacing = requite::getSpacing(before, after);
  requite::Token token(type, this->_line, this->_column, this->_current, length,
                       spacing);
  this->addColumns(length);
  this->incrementChar(length);
  return token;
}

void SourceRanger::addLines(std::uint_fast32_t count) {
  this->_column = 1;
  this->_line += count;
}

void SourceRanger::addColumns(std::uint_fast32_t count) {
  this->_column += count;
}

bool SourceRanger::getIsIdentifier() const {
  return requite::getIsIdentifier(this->getChar(0));
}

bool SourceRanger::getIsDecimalDigit() const {
  return requite::getIsDecimalDigit(this->getChar(0));
}

bool SourceRanger::getIsSymbol() const {
  return requite::getIsSymbol(this->getChar(0));
}

bool SourceRanger::getIsNumeric() const {
  return requite::getIsNumericLiteral(this->getChar(0));
}

bool SourceRanger::getIsWhitespace() const {
  return requite::getIsWhitespace(this->getChar(0));
}

void SourceRanger::skipWhitespace() {
  bool is_escaped = false;
  while (true) {
    switch (const char c0 = this->getChar(0)) {
    case ' ':
      [[fallthrough]];
    case '\t':
      this->incrementChar(1);
      this->addColumns(1);
      break;
    case '\v':
      [[fallthrough]];
    case '\n':
      this->incrementChar(1);
      this->addLines(1);
      break;
    case '\r':
      switch (const char c1 = this->getChar(1)) {
      case '\n':
        this->incrementChar(2);
        break;
      default:
        this->incrementChar(1);
      }
      this->addLines(1);
      break;
    default:
      return;
    }
  }
  return;
}

} // namespace requite