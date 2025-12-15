#pragma once

#include <rq/codeunits.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

#include <functional>
#include <vector>

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

struct Context;
struct Token;

struct Tokenizer final {
  using Self = rq::Tokenizer;

  std::reference_wrapper<rq::Context> _context_ref;
  std::reference_wrapper<std::vector<rq::Token>> _tokens_ref;
  llvm::SmallVector<rq::Grouping, 128> _grouping_stack = {};
  rq::SourceRanger _ranger;
  bool _is_ok = true;

  Tokenizer(rq::Context &context, llvm::StringRef source_text,
            std::vector<rq::Token> &tokens)
      : _context_ref(context), _tokens_ref(tokens), _ranger(source_text) {}
  Tokenizer(const Self &) = delete;
  Tokenizer(Self &&) = delete;
  ~Tokenizer() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOk() const { return this->_is_ok; }
  RQ_ALWAYS_INLINE void setNotOk() { this->_is_ok = false; }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Context &getContext() {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Context &getContext() const {
    return this->_context_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SourceRanger &getRanger() {
    return this->_ranger;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SourceRanger &getRanger() const {
    return this->_ranger;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::vector<rq::Token> &getTokens() {
    return this->_tokens_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const std::vector<rq::Token> &
  getTokens() const {
    return this->_tokens_ref.get();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGrouping() const {
    return !this->_grouping_stack.empty();
  }
  [[nodiscard]] const rq::Grouping &getTopGrouping() const {
    RQ_ASSERT(this->getHasGrouping(), "no grouping");
    return this->_grouping_stack.back();
  }
  void pushGrouping(rq::GroupingKind kind) {
    this->_grouping_stack.emplace_back(kind, this->getTokens().size() - 1);
  }
  void popGrouping() {
    RQ_ASSERT(this->getHasGrouping(), "no grouping");
    this->_grouping_stack.pop_back();
  }
  void _tokenizeSourceText();
  [[nodiscard]] bool tokenizeSourceText();
  void tokenizeLengthToken(rq::TokenKind kind, unsigned length);
  void tokenizeUnmatchedLengthToken(rq::TokenKind kind, unsigned length);
  void tokenizeLeftGrouping(rq::GroupingKind grouping_kind,
                            rq::TokenKind token_kind, unsigned length);
  void tokenizeRightGrouping(rq::GroupingKind grouping_kind,
                             rq::TokenKind token_kind, unsigned length);
  void checkFinalGroupings();
  void logErrorUnmatchedRightToken(const rq::Token &token);
  template <bool CAN_HAVE_INTERPOLATION_PARAM, char END_QUOTE_PARAM,
            rq::TokenKind KIND_PARAM, rq::TokenKind ERROR_UNTERMINATED_PARAM>
  void tokenizeQuotedLiteral() {
    this->getRanger().startSubToken();
    this->getRanger().incrementChar(1);
    while (true) {
      switch (this->getRanger().getChar(0)) {
      case '\\':
        switch (this->getRanger().getChar(1)) {
        case '\v':
          [[fallthrough]];
        case '\n':
          this->getRanger().incrementChar(2);
          break;
        case '\r':
          switch (this->getRanger().getChar(2)) {
          case '\n':
            this->getRanger().incrementChar(3);
            break;
          default:
            this->getRanger().incrementChar(2);
            break;
          }
          break;
        case 'a':
          [[fallthrough]];
        case 'b':
          [[fallthrough]];
        case 'e':
          [[fallthrough]];
        case 'f':
          [[fallthrough]];
        case 'n':
          [[fallthrough]];
        case 'r':
          [[fallthrough]];
        case 't':
          [[fallthrough]];
        case 'v':
          [[fallthrough]];
        case '\\':
          [[fallthrough]];
        case '\"':
          [[fallthrough]];
        case '\'':
          this->getRanger().incrementChar(2);
          break;
        default:
          this->getRanger().incrementChar(1);
          while (true) {
            const char escape_c = this->getRanger().getChar(0);
            if (this->getRanger().getIsDone() || escape_c == '\\' ||
                escape_c == END_QUOTE_PARAM) {
              break;
            }
            switch (escape_c) {
            case '\v':
              [[fallthrough]];
            case '\n':
              this->getRanger().incrementChar(2);
              break;
            case '\r':
              switch (this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(3);
                break;
              default:
                this->getRanger().incrementChar(2);
                break;
              }
              break;
            default:
              this->getRanger().incrementChar(1);
            }
          }
        }
        break;
      case END_QUOTE_PARAM:
        this->getRanger().incrementChar(1);
        this->getTokens().push_back(this->getRanger().getSubToken(KIND_PARAM));
        return;
      case '\n':
        this->getRanger().incrementChar(1);
        break;
      case '\r':
        switch (this->getRanger().getChar(1)) {
        case '\n':
          this->getRanger().incrementChar(2);
          break;
        default:
          this->getRanger().incrementChar(1);
          break;
        }
        break;
      case '\x00':
        this->getTokens().push_back(
            this->getRanger().getSubToken(ERROR_UNTERMINATED_PARAM));
        return;
      case '{':
        if constexpr (CAN_HAVE_INTERPOLATION_PARAM) {
          this->getTokens().push_back(this->getRanger().getSubToken(
              rq::TokenKind::LEFT_INTERPOLATION_LITERAL));
          this->tokenizeLengthToken(rq::TokenKind::LEFT_BRACE_GROUPING, 1);
          this->pushGrouping(rq::GroupingKind::INTERPOLATION);
          return;
        }
        [[fallthrough]];
      default:
        this->getRanger().incrementChar(1);
        break;
      }
    }
  }
};

} // namespace rq
