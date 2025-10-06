#pragma once

#include <rq/source_ranger.hpp>
#include <rq/tokens.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

#include <functional>
#include <vector>

namespace rq {

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
  void pushGrouping(rq::GroupingType grouping) {
    this->_grouping_stack.emplace_back(grouping, this->getTokens().size() - 1);
  }
  void popGrouping() {
    RQ_ASSERT(this->getHasGrouping(), "no grouping");
    this->_grouping_stack.pop_back();
  }
  void _tokenizeSourceText();
  [[nodiscard]] bool tokenizeSourceText();
  void tokenizeLengthToken(rq::TokenType type, unsigned length);
  void tokenizeUnmatchedLengthToken(rq::TokenType type, unsigned length);
  void tokenizeLeftGrouping(rq::GroupingType grouping, rq::TokenType type,
                            unsigned length);
  void tokenizeRightGrouping(rq::GroupingType grouping, rq::TokenType type,
                             unsigned length);
  void checkFinalGroupings();
  void logErrorUnmatchedRightToken(const rq::Token &token);
  template <bool CAN_HAVE_INTERPOLATION_PARAM, char END_QUOTE_PARAM,
            rq::TokenType TYPE_PARAM, rq::TokenType ERROR_UNTERMINATED_PARAM>
  void tokenizeQuotedLiteral() {
    this->getRanger().startSubToken();
    this->getRanger().incrementChar(1);
    this->getRanger().addColumns(1);
    while (true) {
      switch (this->getRanger().getChar(0)) {
      case '\\':
        switch (this->getRanger().getChar(1)) {
        case '\v':
          [[fallthrough]];
        case '\n':
          this->getRanger().incrementChar(2);
          this->getRanger().addLines(1);
          break;
        case '\r':
          switch (this->getRanger().getChar(2)) {
          case '\n':
            this->getRanger().incrementChar(3);
            this->getRanger().addLines(1);
            break;
          default:
            this->getRanger().incrementChar(2);
            break;
          }
          this->getRanger().addLines(1);
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
          this->getRanger().addColumns(2);
          break;
        default:
          this->getRanger().incrementChar(1);
          this->getRanger().addColumns(1);
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
              this->getRanger().addLines(1);
              break;
            case '\r':
              switch (this->getRanger().getChar(1)) {
              case '\n':
                this->getRanger().incrementChar(3);
                this->getRanger().addLines(1);
                break;
              default:
                this->getRanger().incrementChar(2);
                break;
              }
              this->getRanger().addLines(1);
              break;
            default:
              this->getRanger().incrementChar(1);
              this->getRanger().addColumns(1);
            }
          }
        }
        break;
      case END_QUOTE_PARAM:
        this->getRanger().incrementChar(1);
        this->getTokens().push_back(this->getRanger().getSubToken(TYPE_PARAM));
        return;
      case '\n':
        this->getRanger().incrementChar(1);
        this->getRanger().addLines(1);
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
        this->getRanger().addLines(1);
        break;
      case '\x00':
        this->getTokens().push_back(
            this->getRanger().getSubToken(ERROR_UNTERMINATED_PARAM));
        return;
      case '{':
        if constexpr (CAN_HAVE_INTERPOLATION_PARAM) {
          this->getTokens().push_back(this->getRanger().getSubToken(
              rq::TokenType::LEFT_INTERPOLATION_LITERAL));
          this->tokenizeLengthToken(rq::TokenType::LEFT_BRACE_GROUPING, 1);
          this->pushGrouping(rq::GroupingType::INTERPOLATION);
          return;
        }
        [[fallthrough]];
      default:
        this->getRanger().incrementChar(1);
        this->getRanger().addColumns(1);
        break;
      }
    }
  }
};

} // namespace rq