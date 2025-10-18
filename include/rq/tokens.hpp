#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

namespace rq {

enum class TokenType : std::uint_fast16_t {
  NONE,

  // OPERATORS
  HASH_OPERATOR,               // #
  GREATER_OPERATOR,            // >
  DOUBLE_GREATER_OPERATOR,     // >>
  GREATER_EQUAL_OPERATOR,      // >=
  LESS_OPERATOR,               // <
  DOUBLE_LESS_OPERATOR,        // <<
  LESS_EQUAL_OPERATOR,         // <=
  BANG_EQUAL_OPERATOR,         // !=
  BANG_OPERATOR,               // !
  COLON_OPERATOR,              // :
  DOUBLE_COLON_OPERATOR,       // ::
  PLUS_OPERATOR,               // +
  DASH_OPERATOR,               // -
  STAR_OPERATOR,               // *
  SLASH_OPERATOR,              // /
  PERCENT_OPERATOR,            // %
  BACKSLASH_OPERATOR,          // \    .
  CAROT_OPERATOR,              // ^
  PIPE_OPERATOR,               // |
  TILDE_OPERATOR,              // ~
  AMPERSAND_OPERATOR,          // &
  DOUBLE_AMPERSAND_OPERATOR,   // &&
  DOUBLE_PIPE_OPERATOR,        // ||
  EQUAL_OPERATOR,              // =
  DOUBLE_EQUAL_OPERATOR,       // ==
  PLUS_EQUAL_OPERATOR,         // +=
  DASH_EQUAL_OPERATOR,         // -=
  STAR_EQUAL_OPERATOR,         // *=
  SLASH_EQUAL_OPERATOR,        // /=
  PERCENT_EQUAL_OPERATOR,      // %=
  QUESTION_OPERATOR,           // ?
  GRAVE_OPERATOR,              // `
  DOUBLE_GRAVE_OPERATOR,       // ``
  ARROW_OPERATOR,              // ->
  CONCATENATE_OPERATOR,        // +>
  DOT_OPERATOR,                // .
  DOT_PLUS_OPERATOR,           // .+
  DOT_DASH_OPERATOR,           // .-
  DOT_STAR_OPERATOR,           // .*
  DOT_SLASH_OPERATOR,          // ./
  DOT_PERCENT_OPERATOR,        // .%
  DOT_LESS_OPERATOR,           // .<
  DOT_LESS_EQUAL_OPERATOR,     // .<=
  DOT_GREATER_OPERATOR,        // .>
  DOT_GREATER_EQUAL_OPERATOR,  // .>=
  DOT_DOUBLE_EQUAL_OPERATOR,   // .==
  DOT_BANG_EQUAL_OPERATOR,     // .!=

  // SIGILS
  AT_SIGIL,     // @
  DOLLAR_SIGIL, // $

  // SEPERATOR SYMBOLS
  TRAILER_SEPERATOR,   // %%
  SEMICOLON_SEPERATOR, // ;
  COMMA_SEPERATOR,     // ,

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

  // "My_name_is_{name}._I_am_{age}_years_old."

  LEFT_INTERPOLATION_LITERAL,   // "My_name_is_
  MIDDLE_INTERPOLATION_LITERAL, // ._I_am_
  RIGHT_INTERPOLATION_LITERAL,  // _years_old."

  // ERROR
  ERROR_INVALID,
  ERROR_UNTERMINATED_STRING_LITERAL,
  ERROR_UNTERMINATED_CODEUNIT_LITERAL,
  ERROR_UNMATCHED_LEFT_BRACKET_GROUPING,
  ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING,
  ERROR_UNMATCHED_LEFT_BRACE_GROUPING,
  ERROR_UNMATCHED_RIGHT_BRACE_GROUPING,
  ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING,
  ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING
};

enum class TokenFlags : std::uint8_t {
  NONE = 0,
  OPERATOR = rq::getBit(7),
  SIGIL = rq::getBit(6),
  SEPERATOR = rq::getBit(5),
  LITERAL = rq::getBit(4),
  LEFT_GROUPING = rq::getBit(3),
  RIGHT_GROUPING = rq::getBit(2),
  ERROR = rq::getBit(1),
  INFERENCE_TERMINATOR = rq::getBit(0)
};

template <> struct is_flags<rq::TokenFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getName(rq::TokenType token) {
  using namespace rq;
  using T = TokenType;
  switch (token) {
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
  case T::DOUBLE_AMPERSAND_OPERATOR:
    return "double_ampersand_operator";
  case T::DOUBLE_PIPE_OPERATOR:
    return "double_pipe_operator";
  case T::EQUAL_OPERATOR:
    return "equal_operator";
  case T::DOUBLE_EQUAL_OPERATOR:
    return "double_equal_operator";
  case T::PLUS_EQUAL_OPERATOR:
    return "plus_equal_operator";
  case T::DASH_EQUAL_OPERATOR:
    return "dash_equal_operator";
  case T::STAR_EQUAL_OPERATOR:
    return "star_equal_operator";
  case T::SLASH_EQUAL_OPERATOR:
    return "slash_equal_operator";
  case T::PERCENT_EQUAL_OPERATOR:
    return "percent_equal_operator";
  case T::QUESTION_OPERATOR:
    return "question_operator";
  case T::GRAVE_OPERATOR:
    return "grave_operator";
  case T::DOUBLE_GRAVE_OPERATOR:
    return "double_grave_operator";
  case T::ARROW_OPERATOR:
    return "arrow_operator";
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

  // SIGILS
  case T::AT_SIGIL:
    return "at_sigil";
  case T::DOLLAR_SIGIL:
    return "dollar_sigil";

  // SEPERATOR SYMBOLS
  case T::TRAILER_SEPERATOR:
    return "trailer_seperator";
  case T::SEMICOLON_SEPERATOR:
    return "semicolon_seperator";
  case T::COMMA_SEPERATOR:
    return "comma_seperator";

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
  case T::LEFT_INTERPOLATION_LITERAL:
    return "left_interpolation_literal";
  case T::MIDDLE_INTERPOLATION_LITERAL:
    return "middle_interpolation_literal";
  case T::RIGHT_INTERPOLATION_LITERAL:
    return "right_interpolation_literal";

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
getDescription(rq::TokenType token) {
  using namespace rq;
  using T = TokenType;
  switch (token) {
  case T::NONE:
    return "none";

  // OPERATORS
  case T::HASH_OPERATOR:
    return "hash operator";
  case T::GREATER_OPERATOR:
    return "greater operator";
  case T::DOUBLE_GREATER_OPERATOR:
    return "double greater operator";
  case T::GREATER_EQUAL_OPERATOR:
    return "greater equal operator";
  case T::LESS_OPERATOR:
    return "less operator";
  case T::DOUBLE_LESS_OPERATOR:
    return "double less operator";
  case T::LESS_EQUAL_OPERATOR:
    return "less equal operator";
  case T::BANG_EQUAL_OPERATOR:
    return "bang equal operator";
  case T::BANG_OPERATOR:
    return "bang operator";
  case T::COLON_OPERATOR:
    return "colon operator";
  case T::DOUBLE_COLON_OPERATOR:
    return "double colon operator";
  case T::PLUS_OPERATOR:
    return "plus operator";
  case T::DASH_OPERATOR:
    return "dash operator";
  case T::STAR_OPERATOR:
    return "star operator";
  case T::SLASH_OPERATOR:
    return "slash operator";
  case T::BACKSLASH_OPERATOR:
    return "backslash operator";
  case T::PERCENT_OPERATOR:
    return "percent operator";
  case T::CAROT_OPERATOR:
    return "carot operator";
  case T::PIPE_OPERATOR:
    return "pipe operator";
  case T::TILDE_OPERATOR:
    return "tilde operator";
  case T::AMPERSAND_OPERATOR:
    return "ampersand operator";
  case T::DOUBLE_AMPERSAND_OPERATOR:
    return "double ampersand operator";
  case T::DOUBLE_PIPE_OPERATOR:
    return "double pipe operator";
  case T::EQUAL_OPERATOR:
    return "equal operator";
  case T::DOUBLE_EQUAL_OPERATOR:
    return "double equal operator";
  case T::PLUS_EQUAL_OPERATOR:
    return "plus equal operator";
  case T::DASH_EQUAL_OPERATOR:
    return "dash equal operator";
  case T::STAR_EQUAL_OPERATOR:
    return "star equal operator";
  case T::SLASH_EQUAL_OPERATOR:
    return "slash equal operator";
  case T::PERCENT_EQUAL_OPERATOR:
    return "percent equal operator";
  case T::QUESTION_OPERATOR:
    return "question operator";
  case T::GRAVE_OPERATOR:
    return "grave operator";
  case T::DOUBLE_GRAVE_OPERATOR:
    return "double grave operator";
  case T::ARROW_OPERATOR:
    return "arrow operator";
  case T::CONCATENATE_OPERATOR:
    return "concatenate operator";
  case T::DOT_OPERATOR:
    return "dot operator";
  case T::DOT_PLUS_OPERATOR:
    return "dot plus operator";
  case T::DOT_DASH_OPERATOR:
    return "dot dash operator";
  case T::DOT_STAR_OPERATOR:
    return "dot star operator";
  case T::DOT_SLASH_OPERATOR:
    return "dot slash operator";
  case T::DOT_PERCENT_OPERATOR:
    return "dot percent operator";
  case T::DOT_LESS_OPERATOR:
    return "dot less operator";
  case T::DOT_LESS_EQUAL_OPERATOR:
    return "dot less equal operator";
  case T::DOT_GREATER_OPERATOR:
    return "dot greater operator";
  case T::DOT_GREATER_EQUAL_OPERATOR:
    return "dot greater equal operator";
  case T::DOT_DOUBLE_EQUAL_OPERATOR:
    return "dot double equal operator";
  case T::DOT_BANG_EQUAL_OPERATOR:
    return "dot bang equal operator";

  // SIGILS
  case T::AT_SIGIL:
    return "at_sigil";
  case T::DOLLAR_SIGIL:
    return "dollar_sigil";

  // SEPERATOR SYMBOLS
  case T::TRAILER_SEPERATOR:
    return "trailer seperator";
  case T::SEMICOLON_SEPERATOR:
    return "semicolon seperator";
  case T::COMMA_SEPERATOR:
    return "comma seperator";

  // GROUPING SYMBOLS
  case T::LEFT_BRACKET_GROUPING:
    return "left bracket grouping";
  case T::RIGHT_BRACKET_GROUPING:
    return "right bracket grouping";
  case T::LEFT_BRACE_GROUPING:
    return "left brace grouping";
  case T::RIGHT_BRACE_GROUPING:
    return "right brace grouping";
  case T::LEFT_PARENTHESIS_GROUPING:
    return "left parenthesis grouping";
  case T::RIGHT_PARENTHESIS_GROUPING:
    return "right parenthesis grouping";

  // LITERAL
  case T::IDENTIFIER_LITERAL:
    return "identifier literal";
  case T::CODEUNIT_LITERAL:
    return "codeunit literal";
  case T::STRING_LITERAL:
    return "string literal";
  case T::INTEGER_LITERAL:
    return "integer literal";
  case T::FLOAT_LITERAL:
    return "float literal";
  case T::LEFT_INTERPOLATION_LITERAL:
    return "left interpolated string literal";
  case T::MIDDLE_INTERPOLATION_LITERAL:
    return "middle interpolated string literal";
  case T::RIGHT_INTERPOLATION_LITERAL:
    return "right interpolated string literal";

  // ERROR
  case T::ERROR_INVALID:
    return "invalid";
  case T::ERROR_UNTERMINATED_STRING_LITERAL:
    return "unterminated string literal";
  case T::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return "unterminated codeunit literal";
  case T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return "unmatched left bracket grouping";
  case T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return "unmatched right bracket grouping";
  case T::ERROR_UNMATCHED_LEFT_BRACE_GROUPING:
    return "unmatched left brace grouping";
  case T::ERROR_UNMATCHED_RIGHT_BRACE_GROUPING:
    return "unmatched right brace grouping";
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return "unmatched left parenthesis grouping";
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return "unmatched right parenthesis grouping";
  }

  return "unknown";
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::TokenFlags
getFlags(rq::TokenType token) {
  using namespace rq;
  using T = TokenType;
  using TF = TokenFlags;
  switch (token) {
  case T::NONE:
    return TF::ERROR;
  case T::HASH_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::GREATER_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_GREATER_OPERATOR:
    return TF::OPERATOR;
  case T::GREATER_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::LESS_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_LESS_OPERATOR:
    return TF::OPERATOR;
  case T::LESS_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::BANG_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::BANG_OPERATOR:
    return TF::OPERATOR;
  case T::COLON_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::DOUBLE_COLON_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
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
    return TF::OPERATOR;
  case T::TILDE_OPERATOR:
    return TF::OPERATOR;
  case T::AMPERSAND_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_AMPERSAND_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_PIPE_OPERATOR:
    return TF::OPERATOR;
  case T::EQUAL_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::DOUBLE_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::PLUS_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::DASH_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::STAR_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::SLASH_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::PERCENT_EQUAL_OPERATOR:
    return TF::OPERATOR;
  case T::QUESTION_OPERATOR:
    return TF::OPERATOR;
  case T::GRAVE_OPERATOR:
    return TF::OPERATOR;
  case T::DOUBLE_GRAVE_OPERATOR:
    return TF::OPERATOR;
  case T::ARROW_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
  case T::CONCATENATE_OPERATOR:
    return TF::OPERATOR;
  case T::DOT_OPERATOR:
    return TF::OPERATOR | TF::INFERENCE_TERMINATOR;
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
  case T::AT_SIGIL:
    return TF::SIGIL;
  case T::DOLLAR_SIGIL:
    return TF::SIGIL;
  case T::TRAILER_SEPERATOR:
    return TF::SEPERATOR;
  case T::SEMICOLON_SEPERATOR:
    return TF::SEPERATOR;
  case T::COMMA_SEPERATOR:
    return TF::SEPERATOR | TF::INFERENCE_TERMINATOR;
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
  case T::LEFT_INTERPOLATION_LITERAL:
    return TF::LITERAL;
  case T::MIDDLE_INTERPOLATION_LITERAL:
    return TF::LITERAL;
  case T::RIGHT_INTERPOLATION_LITERAL:
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

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::TokenType
getUnmatched(rq::TokenType token) {
  using namespace rq;
  using T = TokenType;
  switch (token) {
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
getIsOperator(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::OPERATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsSigil(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::SIGIL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsSeperator(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::SEPERATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLiteral(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLeftGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::LEFT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsRightGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasSome(flags, rq::TokenFlags::LEFT_GROUPING |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLeftUnmatchedGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags,
                       rq::TokenFlags::ERROR | rq::TokenFlags::LEFT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsRightUnmathcedGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags,
                       rq::TokenFlags::ERROR | rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsUnmatchedGrouping(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR) &&
         rq::getHasSome(flags, rq::TokenFlags::LEFT_GROUPING |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsError(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsErrorLiteral(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::ERROR | rq::TokenFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsInferenceTerminator(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasAll(flags, rq::TokenFlags::INFERENCE_TERMINATOR);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsExpressionEnd(rq::TokenType token) {
  const rq::TokenFlags flags = rq::getFlags(token);
  return rq::getHasSome(flags, rq::TokenFlags::SEPERATOR |
                                   rq::TokenFlags::RIGHT_GROUPING);
}

struct Token final {
  using Self = rq::Token;

  rq::TokenType _type;
  unsigned _line;
  unsigned _column;
  const char *_lexume_ptr;
  unsigned _lexume_length;

  Token() = delete;
  Token(rq::TokenType type, unsigned line, unsigned column,
        const char *lexume_ptr, unsigned lexume_length)
      : _type(type), _line(line), _column(column), _lexume_ptr(lexume_ptr),
        _lexume_length(lexume_length) {}
  Token(const Self &) = default;
  Token(Self &&) = default;
  ~Token() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TokenType getType() const {
    return this->_type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return rq::getName(this->_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getLine() const {
    return this->_line;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getColumn() const {
    return this->_column;
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
    return rq::getIsOperator(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSeperator() const {
    return rq::getIsSeperator(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigil() const {
    return rq::getIsSigil(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return rq::getIsLiteral(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeftGrouping() const {
    return rq::getIsLeftGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRightGrouping() const {
    return rq::getIsRightGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGrouping() const {
    return rq::getIsGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeftUnmatchedGrouping() const {
    return rq::getIsLeftUnmatchedGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRightUnmatchedGrouping() const {
    return rq::getIsRightUnmathcedGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnmatchedGrouping() const {
    return rq::getIsUnmatchedGrouping(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsError() const {
    return rq::getIsError(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsErrorLiteral() const {
    return rq::getIsErrorLiteral(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferenceTerminator() const {
    return rq::getIsInferenceTerminator(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionEnd() const {
    return rq::getIsExpressionEnd(this->getType());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceStart() const {
    return llvm::SMLoc::getFromPointer(this->_lexume_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceEnd() const {
    return llvm::SMLoc::getFromPointer(this->_lexume_ptr +
                                       this->_lexume_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMRange getLlvmSourceRange() const {
    return llvm::SMRange(this->getLlvmSourceStart(), this->getLlvmSourceEnd());
  }
  void RQ_ALWAYS_INLINE setUnmatched() {
    this->_type = rq::getUnmatched(this->_type);
  }
};

enum class GroupingType { NONE, INTERPOLATION, BRACKET, BRACE, PARENTHESIS };

constexpr llvm::StringRef getDescription(rq::GroupingType grouping) {
  using namespace rq;
  using G = GroupingType;
  switch (grouping) {
  case G::NONE:
    return "none";
  case G::INTERPOLATION:
    return "interpolation";
  case G::BRACKET:
    return "bracket";
  case G::BRACE:
    return "brace";
  case G::PARENTHESIS:
    return "parenthsis";
  default:
    break;
  }
  return "unknown";
}

struct Grouping final {
  rq::GroupingType _type = rq::GroupingType::NONE;
  unsigned _token_i = 0;

  Grouping(rq::GroupingType type, unsigned token_i)
      : _type(type), _token_i(token_i) {}
  rq::GroupingType getType() const { return this->_type; }
  unsigned getTokenI() const { return this->_token_i; }
};

} // namespace rq