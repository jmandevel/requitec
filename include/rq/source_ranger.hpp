#pragma once

#include <rq/codeunits.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>

#include <cstdint>

namespace rq {

struct SourceRanger final {
  using Self = rq::SourceRanger;

  llvm::StringRef::iterator _start;
  llvm::StringRef::iterator _current;
  llvm::StringRef::iterator _end;

  std::uint_fast32_t _line;
  std::uint_fast32_t _column;

  std::uint_fast32_t _sub_line;
  std::uint_fast32_t _sub_column;
  llvm::StringRef::iterator _sub_start;

  SourceRanger(llvm::StringRef text)
      : _start(text.begin()), _current(text.begin()), _end(text.end()),
        _line(1), _column(1), _sub_line(0), _sub_column(0),
        _sub_start(nullptr) {
    RQ_ASSERT(*this->_end == 0x00,
                    "end of source text not null terminator character");
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_current >= this->_end;
  }
  [[nodiscard]]
  const char &getChar(std::ptrdiff_t offset) const {
    RQ_ASSERT(offset >= 0, "offset must be greater than or equal to 0");
    if ((this->_current + offset) >= this->_end) {
      return *this->_end;
    }
    return this->_current[offset];
  }
  [[nodiscard]]
  const char &getSubChar(std::ptrdiff_t offset) const {
    RQ_ASSERT(offset >= 0, "offset not greater than or equal to 0");
    if ((this->_sub_start + offset) >= this->_end) {
      return *this->_end;
    }
    return this->_sub_start[offset];
  }
  const char &getPreviousChar(std::ptrdiff_t offset) const {
    RQ_ASSERT(offset < 0, "offset not less than 0");
    if ((this->_current + offset) < this->_start) {
      return *this->_end;
    }
    return this->_current[offset];
  }

  const char &getPreviousSubChar(std::ptrdiff_t offset) const {
    RQ_ASSERT(offset < 0, "offset not less than 0");
    if ((this->_sub_start + offset) < this->_start) {
      return *this->_end;
    }
    return this->_sub_start[offset];
  }

  RQ_ALWAYS_INLINE void incrementChar(std::ptrdiff_t offset) {
    this->_current += offset;
  }

  RQ_ALWAYS_INLINE void startSubToken() {
    this->_sub_line = this->_line;
    this->_sub_column = this->_column;
    this->_sub_start = this->_current;
  }

  rq::Token getSubToken(rq::TokenType type) {
    RQ_ASSERT(this->_sub_start != nullptr, "sub token not started");
    RQ_ASSERT(this->_current > this->_sub_start,
                    "current token before sub token start");
    const char before = this->getPreviousSubChar(-1);
    const char after = this->getChar(1);
    rq::Token token(type, this->_sub_line, this->_sub_column, this->_sub_start,
                    this->_current - this->_sub_start);
    return token;
  }

  [[nodiscard]] rq::Token getLengthToken(rq::TokenType type,
                                         std::uint_fast32_t length) {
    RQ_ASSERT((this->_current + length) <= this->_end,
                    "length token out of range");
    const char before = this->getPreviousChar(-1);
    const char after = this->getChar(length);
    rq::Token token(type, this->_line, this->_column, this->_current, length);
    this->addColumns(length);
    this->incrementChar(length);
    return token;
  }

  RQ_ALWAYS_INLINE void addLines(std::uint_fast32_t count) {
    this->_column = 1;
    this->_line += count;
  }

  RQ_ALWAYS_INLINE void addColumns(std::uint_fast32_t count) {
    this->_column += count;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIdentifier() const {
    return rq::getIsIdentifier(this->getChar(0));
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDecimalDigit() const {
    return rq::getIsDecimalDigit(this->getChar(0));
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return rq::getIsSymbol(this->getChar(0));
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->getChar(0));
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsWhitespace() const {
    return rq::getIsWhitespace(this->getChar(0));
  }

  void skipWhitespace() {
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
};

} // namespace rq