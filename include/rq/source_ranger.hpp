#pragma once

#include <rq/utility.hpp>

namespace rq {

struct SourceRanger final {
  using Self = rq::SourceRanger;

  llvm::StringRef::iterator _start;
  llvm::StringRef::iterator _current;
  llvm::StringRef::iterator _end;
  llvm::StringRef::iterator _sub_start;

  SourceRanger(llvm::StringRef text)
      : _start(text.begin()), _current(text.begin()), _end(text.end()),
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

  RQ_ALWAYS_INLINE void startSubToken() { this->_sub_start = this->_current; }

  rq::Token getSubToken(rq::TokenKind kind) {
    RQ_ASSERT(this->_sub_start != nullptr, "sub token not started");
    RQ_ASSERT(this->_current > this->_sub_start,
              "current token before sub token start");
    rq::Token token(kind, this->_sub_start,
                    static_cast<unsigned>(this->_current - this->_sub_start));
    return token;
  }

  [[nodiscard]] rq::Token getLengthToken(rq::TokenKind kind,
                                         std::uint_fast32_t length) {
    RQ_ASSERT((this->_current + length) <= this->_end,
              "length token out of range");
    rq::Token token(kind, this->_current, length);
    this->incrementChar(length);
    return token;
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

  inline void skipWhitespace() {
    while (!this->getIsDone()) {
      if (this->getIsWhitespace()) {
        this->incrementChar(1);
      }
    }
    return;
  }

  inline void skipUntilVerticalWhitespace() {
    while (!this->getIsDone()) {
      if (rq::getIsVerticalWhitespace(this->getChar(0))) {
        return;
      }
      this->incrementChar(1);
    }
  }

  [[nodiscard]] inline bool skipUntilChar(char codeunit) {
    while (!this->getIsDone()) {
      if (this->getChar(0) == codeunit) {
        return true;
      }
      this->incrementChar(1);
    }
    return false;
  }
};


}