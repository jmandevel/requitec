#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

namespace rq {

enum class TokenKind : std::uint_fast8_t {
  NONE,

  // OPERATORS
  HASH_OPERATOR,              // #
  GREATER_OPERATOR,           // >
  DOUBLE_GREATER_OPERATOR,    // >>
  GREATER_EQUAL_OPERATOR,     // >=
  LESS_OPERATOR,              // <
  DOUBLE_LESS_OPERATOR,       // <<
  LESS_EQUAL_OPERATOR,        // <=
  BANG_EQUAL_OPERATOR,        // !=
  BANG_OPERATOR,              // !
  WHAT_OPERATOR,              // ?
  COLON_OPERATOR,             // :
  DOUBLE_COLON_OPERATOR,      // ::
  PLUS_OPERATOR,              // +
  DASH_OPERATOR,              // -
  STAR_OPERATOR,              // *
  SLASH_OPERATOR,             // /
  PERCENT_OPERATOR,           // %
  BACKSLASH_OPERATOR,         // \    .
  CAROT_OPERATOR,             // ^
  PIPE_OPERATOR,              // |
  TILDE_OPERATOR,             // ~
  AMPERSAND_OPERATOR,         // &
  DOWN_ARROW_OPERATOR,        // \/
  UP_ARROW_OPERATOR,          // /\   .
  DOUBLE_AMBERSAND_OPERATOR,  // &&
  DOUBLE_PIPE_OPERATOR,       // ||
  AT_OPERATOR,                // @
  DOLLAR_OPERATOR,            // $
  EQUAL_OPERATOR,             // =
  DOUBLE_EQUAL_OPERATOR,      // ==
  ARROW_OPERATOR,             // ->
  THICK_ARROW_OPERATOR,       // =>
  CONCATENATE_OPERATOR,       // +>
  APPEND_OPERATOR,            // *>
  DOT_OPERATOR,               // .
  DOT_PLUS_OPERATOR,          // .+
  DOT_DASH_OPERATOR,          // .-
  DOT_STAR_OPERATOR,          // .*
  DOT_SLASH_OPERATOR,         // ./
  DOT_PERCENT_OPERATOR,       // .%
  DOT_LESS_OPERATOR,          // .<
  DOT_LESS_EQUAL_OPERATOR,    // .<=
  DOT_GREATER_OPERATOR,       // .>
  DOT_GREATER_EQUAL_OPERATOR, // .>=
  DOT_DOUBLE_EQUAL_OPERATOR,  // .==
  DOT_BANG_EQUAL_OPERATOR,    // .!=

  // SEPARATOR SYMBOLS
  SEMICOLON_SEPARATOR, // ;
  COMMA_SEPARATOR,     // ,

  // GROUPING SYMBOLS
  LEFT_BRACKET_GROUPING,      // [
  RIGHT_BRACKET_GROUPING,     // ]
  LEFT_BRACE_GROUPING,        // {
  RIGHT_BRACE_GROUPING,       // }
  LEFT_PARENTHESIS_GROUPING,  // (
  RIGHT_PARENTHESIS_GROUPING, // )

  // LITERAL
  IDENTIFIER_LITERAL, // var0 MyType ☺
  CODEUNIT_LITERAL,   // 'a'
  STRING_LITERAL,     // "Hello, world!"
  INTEGER_LITERAL,    // 12345 16xDEADBEEF 2x1010_0110
  FLOAT_LITERAL,      // 3.14 9000.00000000001 10_020.434

  // ERROR
  ERROR_INVALID,
  ERROR_UNTERMINATED_STRING_LITERAL,
  ERROR_UNTERMINATED_CODEUNIT_LITERAL,
  ERROR_UNMATCHED_LEFT_BRACKET_GROUPING,
  ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING,
  ERROR_UNMATCHED_LEFT_BRACE_GROUPING,
  ERROR_UNMATCHED_RIGHT_BRACE_GROUPING,
  ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING,
  ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING,
};

enum class TokenFlags : std::uint8_t {
  NONE = 0,
  OPERATOR = rq::getBit(7),
  MARK = rq::getBit(6),
  SEPARATOR = rq::getBit(5),
  LITERAL = rq::getBit(4),
  LEFT_GROUPING = rq::getBit(3),
  RIGHT_GROUPING = rq::getBit(2),
  ERROR = rq::getBit(1),
  INFERENCE_TERMINATOR = rq::getBit(0)
};

template <> struct is_flags<rq::TokenFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getName(rq::TokenKind kind) {
  using namespace rq;
  using T = TokenKind;
  switch (kind) {
  case T::NONE:
    return "none";

  // OPERATORS
  case T::HASH_OPERATOR:
    return "hash_operator";
  case T::GREATER_OPERATOR:
    return "greater_operator";
  case T::DOUBLE_GREATER_OPERATOR:
    return "double_greater_operator";
  case T::GREATER_EQUAL_OPERATOR:
    return "greater_equal_operator";
  case T::LESS_OPERATOR:
    return "less_operator";
  case T::DOUBLE_LESS_OPERATOR:
    return "double_less_operator";
  case T::LESS_EQUAL_OPERATOR:
    return "less_equal_operator";
  case T::BANG_EQUAL_OPERATOR:
    return "bang_equal_operator";
  case T::BANG_OPERATOR:
    return "bang_operator";
  case T::WHAT_OPERATOR:
    return "what_operator";
  case T::COLON_OPERATOR:
    return "colon_operator";
  case T::DOUBLE_COLON_OPERATOR:
    return "double_colon_operator";
  case T::PLUS_OPERATOR:
    return "plus_operator";
  case T::DASH_OPERATOR:
    return "dash_operator";
  case T::STAR_OPERATOR:
    return "star_operator";
  case T::SLASH_OPERATOR:
    return "slash_operator";
  case T::BACKSLASH_OPERATOR:
    return "backslash_operator";
  case T::PERCENT_OPERATOR:
    return "percent_operator";
  case T::CAROT_OPERATOR:
    return "carot_operator";
  case T::PIPE_OPERATOR:
    return "pipe_operator";
  case T::TILDE_OPERATOR:
    return "tilde_operator";
  case T::AMPERSAND_OPERATOR:
    return "ampersand_operator";
  case T::DOWN_ARROW_OPERATOR:
    return "down_arrow_operator";
  case T::UP_ARROW_OPERATOR:
    return "up_arrow_operator";
  case T::DOUBLE_AMBERSAND_OPERATOR:
    return "double_ambersand_operator";
  case T::DOUBLE_PIPE_OPERATOR:
    return "double_pipe_operator";
  case T::AT_OPERATOR:
    return "at_operator";
  case T::DOLLAR_OPERATOR:
    return "dollar_operator";
  case T::EQUAL_OPERATOR:
    return "equal_operator";
  case T::DOUBLE_EQUAL_OPERATOR:
    return "double_equal_operator";
  case T::ARROW_OPERATOR:
    return "arrow_operator";
  case T::THICK_ARROW_OPERATOR:
    return "thick_arrow_operator";
  case T::APPEND_OPERATOR:
    return "append_operator";
  case T::CONCATENATE_OPERATOR:
    return "concatenate_operator";
  case T::DOT_OPERATOR:
    return "dot_operator";
  case T::DOT_PLUS_OPERATOR:
    return "dot_plus_operator";
  case T::DOT_DASH_OPERATOR:
    return "dot_dash_operator";
  case T::DOT_STAR_OPERATOR:
    return "dot_star_operator";
  case T::DOT_SLASH_OPERATOR:
    return "dot_slash_operator";
  case T::DOT_PERCENT_OPERATOR:
    return "dot_percent_operator";
  case T::DOT_LESS_OPERATOR:
    return "dot_less_operator";
  case T::DOT_LESS_EQUAL_OPERATOR:
    return "dot_less_equal_operator";
  case T::DOT_GREATER_OPERATOR:
    return "dot_greater_operator";
  case T::DOT_GREATER_EQUAL_OPERATOR:
    return "dot_greater_equal_operator";
  case T::DOT_DOUBLE_EQUAL_OPERATOR:
    return "dot_double_equal_operator";
  case T::DOT_BANG_EQUAL_OPERATOR:
    return "dot_bang_equal_operator";

  // SEPARATOR SYMBOLS
  case T::SEMICOLON_SEPARATOR:
    return "semicolon_separator";
  case T::COMMA_SEPARATOR:
    return "comma_separator";

  // GROUPING SYMBOLS
  case T::LEFT_BRACKET_GROUPING:
    return "left_bracket_grouping";
  case T::RIGHT_BRACKET_GROUPING:
    return "right_bracket_grouping";
  case T::LEFT_BRACE_GROUPING:
    return "left_brace_grouping";
  case T::RIGHT_BRACE_GROUPING:
    return "right_brace_grouping";
  case T::LEFT_PARENTHESIS_GROUPING:
    return "left_parenthesis_grouping";
  case T::RIGHT_PARENTHESIS_GROUPING:
    return "right_parenthesis_grouping";

  // LITERAL
  case T::IDENTIFIER_LITERAL:
    return "identifier_literal";
  case T::CODEUNIT_LITERAL:
    return "codeunit_literal";
  case T::STRING_LITERAL:
    return "string_literal";
  case T::INTEGER_LITERAL:
    return "integer_literal";
  case T::FLOAT_LITERAL:
    return "float_literal";

  // ERROR
  case T::ERROR_INVALID:
    return "error_invalid";
  case T::ERROR_UNTERMINATED_STRING_LITERAL:
    return "error_unterminated_string_literal";
  case T::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return "error_unterminated_codeunit_literal";
  case T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return "error_nmatched_left_bracket_grouping";
  case T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return "error_unmatched_right_bracket_grouping";
  case T::ERROR_UNMATCHED_LEFT_BRACE_GROUPING:
    return "error_unmatched_left_brace_grouping";
  case T::ERROR_UNMATCHED_RIGHT_BRACE_GROUPING:
    return "error_unmatched_right_brace_grouping";
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return "error_unmatched_left_parenthesis_grouping";
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return "error_unmatched_right_parenthesis_grouping";
  }
  return "unknown";
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getDescription(rq::TokenKind kind) {
  return getName(kind);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::TokenFlags
getFlags(rq::TokenKind kind) {
  using namespace rq;
  using T = TokenKind;
  using TF = TokenFlags;
  switch (kind) {
  case T::NONE:
    return TF::ERROR;
  case T::HASH_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::GREATER_OPERATOR:
    return TF::OPERATOR | TF::MARK;
  case T::DOUBLE_GREATER_OPERATOR:
    return TF::OPERATOR;
  case T::GREATER_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::LESS_OPERATOR:
    return TF::OPERATOR | TF::MARK | TF::INFERENCE_TERMINATOR;
  case T::DOUBLE_LESS_OPERATOR:
    return TF::OPERATOR;
  case T::LESS_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::BANG_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::BANG_OPERATOR:
    return TF::OPERATOR;
  case T::WHAT_OPERATOR:
    return TF::ERROR;
  case T::COLON_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_COLON_OPERATOR:
    return TF::OPERATOR;
  case T::PLUS_OPERATOR:
    return TF::OPERATOR;
  case T::DASH_OPERATOR:
    return TF::OPERATOR;
  case T::STAR_OPERATOR:
    return TF::OPERATOR;
  case T::SLASH_OPERATOR:
    return TF::OPERATOR;
  case T::PERCENT_OPERATOR:
    return TF::OPERATOR;
  case T::BACKSLASH_OPERATOR:
    return TF::OPERATOR;
  case T::CAROT_OPERATOR:
    return TF::OPERATOR;
  case T::PIPE_OPERATOR:
    return TF::OPERATOR | TF::MARK;
  case T::TILDE_OPERATOR:
    return TF::OPERATOR;
  case T::AMPERSAND_OPERATOR:
    return TF::OPERATOR;
  case T::DOWN_ARROW_OPERATOR:
    return TF::OPERATOR;
  case T::UP_ARROW_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_AMBERSAND_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_PIPE_OPERATOR:
    return TF::OPERATOR;
  case T::AT_OPERATOR:
    return TF::OPERATOR;
  case T::DOLLAR_OPERATOR:
    return TF::OPERATOR;
  case T::EQUAL_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::DOUBLE_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::ARROW_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::THICK_ARROW_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::APPEND_OPERATOR:
    return TF::OPERATOR;
  case T::CONCATENATE_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_PLUS_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_DASH_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_STAR_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_SLASH_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_PERCENT_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_LESS_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_LESS_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_GREATER_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_GREATER_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_DOUBLE_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_BANG_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::SEMICOLON_SEPARATOR:
    return TF::SEPARATOR | TF::INFERENCE_TERMINATOR;
  case T::COMMA_SEPARATOR:
    return TF::SEPARATOR | TF::INFERENCE_TERMINATOR;
  case T::LEFT_BRACKET_GROUPING:
    return TF::LEFT_GROUPING;
  case T::RIGHT_BRACKET_GROUPING:
    return TF::RIGHT_GROUPING | TF::INFERENCE_TERMINATOR;
  case T::LEFT_BRACE_GROUPING:
    return TF::LEFT_GROUPING;
  case T::RIGHT_BRACE_GROUPING:
    return TF::RIGHT_GROUPING | TF::INFERENCE_TERMINATOR;
  case T::LEFT_PARENTHESIS_GROUPING:
    return TF::LEFT_GROUPING;
  case T::RIGHT_PARENTHESIS_GROUPING:
    return TF::RIGHT_GROUPING | TF::INFERENCE_TERMINATOR;
  case T::IDENTIFIER_LITERAL:
    return TF::LITERAL;
  case T::CODEUNIT_LITERAL:
    return TF::LITERAL;
  case T::STRING_LITERAL:
    return TF::LITERAL;
  case T::INTEGER_LITERAL:
    return TF::LITERAL;
  case T::FLOAT_LITERAL:
    return TF::LITERAL;
  case T::ERROR_INVALID:
    return TF::ERROR;
  case T::ERROR_UNTERMINATED_STRING_LITERAL:
    return TF::ERROR | TF::LITERAL;
  case T::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return TF::ERROR | TF::LITERAL;
  case T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return TF::ERROR | TF::LEFT_GROUPING;
  case T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return TF::ERROR | TF::RIGHT_GROUPING;
  case T::ERROR_UNMATCHED_LEFT_BRACE_GROUPING:
    return TF::ERROR | TF::LEFT_GROUPING;
  case T::ERROR_UNMATCHED_RIGHT_BRACE_GROUPING:
    return TF::ERROR | TF::RIGHT_GROUPING;
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return TF::ERROR | TF::LEFT_GROUPING;
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return TF::ERROR | TF::RIGHT_GROUPING;
  }
  return TF::ERROR;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::TokenKind
getUnmatched(rq::TokenKind kind) {
  using namespace rq;
  using T = TokenKind;
  switch (kind) {
  case T::LEFT_BRACKET_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING;
  case T::RIGHT_BRACKET_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING;
  case T::LEFT_BRACE_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_BRACE_GROUPING;
  case T::RIGHT_BRACE_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_BRACE_GROUPING;
  case T::LEFT_PARENTHESIS_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING;
  case T::RIGHT_PARENTHESIS_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING;
  default:
    break;
  }
  return T::ERROR_INVALID;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsOperator(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::OPERATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsSeparator(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::SEPARATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsLiteral(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getCanBeMark(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::MARK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLeftGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::LEFT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsRightGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::TokenFlags::LEFT_GROUPING |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLeftUnmatchedGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags,
                       rq::TokenFlags::ERROR | rq::TokenFlags::LEFT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsRightUnmathcedGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags,
                       rq::TokenFlags::ERROR | rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsUnmatchedGrouping(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR) &&
         rq::getHasSome(flags, rq::TokenFlags::LEFT_GROUPING |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsError(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsErrorLiteral(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR | rq::TokenFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsInferenceTerminator(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::TokenFlags::INFERENCE_TERMINATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsExpressionEnd(rq::TokenKind kind) {
  const rq::TokenFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::TokenFlags::SEPARATOR |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

struct Token final {
  using Self = rq::Token;

  rq::TokenKind _kind{rq::TokenKind::NONE};
  const char *_lexume_ptr{nullptr};
  unsigned _lexume_length{0};

  Token() = default;
  Token(rq::TokenKind kind, const char *lexume_ptr, unsigned lexume_length)
      : _kind(kind), _lexume_ptr(lexume_ptr), _lexume_length(lexume_length) {}
  Token(const Self &) = default;
  Token(Self &&) = default;
  ~Token() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TokenKind getKind() const {
    return this->_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return rq::getName(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getDescription() const {
    return rq::getDescription(this->_kind);
  }
  [[nodiscard]] llvm::StringRef getSourceText() const {
    return llvm::StringRef(this->_lexume_ptr, this->_lexume_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getSourceTextPtr() const {
    return this->_lexume_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getSourceTextLength() const {
    return this->_lexume_length;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getBeforeSourceTextPtr() const {
    return this->getSourceTextPtr();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getAfterSourceTextPtr() const {
    return this->getSourceTextPtr() + this->getSourceTextLength();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getEndSourceTextPtr() const {
    if (this->getSourceTextLength() == 0) {
      return this->getSourceTextPtr();
    }
    return this->getSourceTextPtr() + this->getSourceTextLength() - 1;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsOperator() const {
    return rq::getIsOperator(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSeparator() const {
    return rq::getIsSeparator(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return rq::getIsLiteral(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeMark() const {
    return rq::getCanBeMark(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeftGrouping() const {
    return rq::getIsLeftGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRightGrouping() const {
    return rq::getIsRightGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGrouping() const {
    return rq::getIsGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeftUnmatchedGrouping() const {
    return rq::getIsLeftUnmatchedGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRightUnmatchedGrouping() const {
    return rq::getIsRightUnmathcedGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnmatchedGrouping() const {
    return rq::getIsUnmatchedGrouping(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsError() const {
    return rq::getIsError(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsErrorLiteral() const {
    return rq::getIsErrorLiteral(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferenceTerminator() const {
    return rq::getIsInferenceTerminator(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionEnd() const {
    return rq::getIsExpressionEnd(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceBegin() const {
    return llvm::SMLoc::getFromPointer(this->_lexume_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceEnd() const {
    return llvm::SMLoc::getFromPointer(this->_lexume_ptr +
                                       this->_lexume_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMRange getLlvmSourceRange() const {
    return llvm::SMRange(this->getLlvmSourceBegin(), this->getLlvmSourceEnd());
  }
  void RQ_ALWAYS_INLINE setUnmatched() {
    this->_kind = rq::getUnmatched(this->_kind);
  }
};

enum class GroupingKind { NONE, BRACKET, DOUBLE_BRACKET, BRACE, PARENTHESIS };

inline llvm::StringRef getDescription(rq::GroupingKind kind) {
  using namespace rq;
  using G = GroupingKind;
  switch (kind) {
  case G::NONE:
    return "none";
  case G::BRACKET:
    return "bracket";
  case G::DOUBLE_BRACKET:
    return "double bracket";
  case G::BRACE:
    return "brace";
  case G::PARENTHESIS:
    return "parenthsis";
  }
  RQ_UNREACHABLE();
}

struct Grouping final {
  rq::GroupingKind _kind = rq::GroupingKind::NONE;
  unsigned _token_i = 0;

  Grouping(rq::GroupingKind kind, unsigned token_i)
      : _kind(kind), _token_i(token_i) {}
  rq::GroupingKind getKind() const { return this->_kind; }
  unsigned getTokenI() const { return this->_token_i; }
  llvm::StringRef getDescription() const {
    return rq::getDescription(this->_kind);
  }
};

} // namespace rq
