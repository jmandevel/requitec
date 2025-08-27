// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

namespace requite {

constexpr std::string_view getName(requite::TokenType token) {
  using namespace requite;
  using T = TokenType;
  switch (token) {
  case T::NONE:
    return "none";

  // OPERATORS
  case T::HASH_OPERATOR:
    return "hash_operator";
  case T::AT_OPERATOR:
    return "at_operator";
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
  case T::DOLLAR_OPERATOR:
    return "dollar_operator";
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
  case T::THICK_ARROW_OPERATOR:
    return "thick_arrow_operator";
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
  case T::DOUBLE_DOT_DOUBLE_EQUAL_OPERATOR:
    return "double_dot_double_equal_operator";
  case T::DOUBLE_DOT_BANG_EQUAL_OPERATOR:
    return "double_dot_bang_equal_operator";
  case T::DOUBLE_DOT_GREATER_OPERATOR:
    return "double_dot_greater_operator";
  case T::DOUBLE_DOT_GREATER_EQUAL_OPERATOR:
    return "double_dot_greater_equal_operator";
  case T::DOUBLE_DOT_LESS_OPERATOR:
    return "double_dot_less_operator";
  case T::DOUBLE_DOT_LESS_EQUAL_OPERATOR:
    return "double_dot_less_equal_operator";

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
  case T::LEFT_TRIP_GROUPING:
    return "left_trip_grouping";
  case T::RIGHT_TRIP_GROUPING:
    return "right_trip_grouping";
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
  case T::LEFT_INTERPOLATED_STRING_LITERAL:
    return "left_interpolated_string_literal";
  case T::MIDDLE_INTERPOLATED_STRING_LITERAL:
    return "middle_interpolated_string_literal";
  case T::RIGHT_INTERPOLATED_STRING_LITERAL:
    return "right_interpolated_string_literal";

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
  case T::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return "error_unmatched_left_trip_grouping";
  case T::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return "error_unmatched_right_trip_grouping";
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return "error_unmatched_left_parenthesis_grouping";
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return "error_unmatched_right_parenthesis_grouping";
  }
  return "unknown";
}

constexpr std::string_view getDescription(requite::TokenType token) {
  using namespace requite;
  using T = TokenType;
  switch (token) {
  case T::NONE:
    return "none";

  // OPERATORS
  case T::HASH_OPERATOR:
    return "hash operator";
  case T::AT_OPERATOR:
    return "at operator";
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
  case T::DOLLAR_OPERATOR:
    return "dollar operator";
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
  case T::THICK_ARROW_OPERATOR:
    return "thick arrow operator";
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
  case T::DOUBLE_DOT_DOUBLE_EQUAL_OPERATOR:
    return "double dot double equal operator";
  case T::DOUBLE_DOT_BANG_EQUAL_OPERATOR:
    return "double dot bang equal operator";
  case T::DOUBLE_DOT_GREATER_OPERATOR:
    return "double dot greater operator";
  case T::DOUBLE_DOT_GREATER_EQUAL_OPERATOR:
    return "double dot greater equal operator";
  case T::DOUBLE_DOT_LESS_OPERATOR:
    return "double dot less operator";
  case T::DOUBLE_DOT_LESS_EQUAL_OPERATOR:
    return "double dot less equal operator";

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
  case T::LEFT_TRIP_GROUPING:
    return "left trip grouping";
  case T::RIGHT_TRIP_GROUPING:
    return "right trip grouping";
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
  case T::LEFT_INTERPOLATED_STRING_LITERAL:
    return "left interpolated string literal";
  case T::MIDDLE_INTERPOLATED_STRING_LITERAL:
    return "middle interpolated string literal";
  case T::RIGHT_INTERPOLATED_STRING_LITERAL:
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
  case T::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return "unmatched left trip grouping";
  case T::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return "unmatched right trip grouping";
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return "unmatched left parenthesis grouping";
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return "unmatched right parenthesis grouping";
  }

  return "unknown";
}

constexpr requite::TokenType getUnmatched(requite::TokenType token) {
  using namespace requite;
  using T = TokenType;
  switch (token) {
  case T::LEFT_BRACKET_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING;
  case T::RIGHT_BRACKET_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING;
  case T::LEFT_TRIP_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_TRIP_GROUPING;
  case T::RIGHT_TRIP_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING;
  case T::LEFT_PARENTHESIS_GROUPING:
    return T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING;
  case T::RIGHT_PARENTHESIS_GROUPING:
    return T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

namespace _token {
enum _TokenFlags : std::uint8_t {
  _NONE = 0,
  _OPERATOR = requite::getBit(7),
  _SEPERATOR = requite::getBit(6),
  _LITERAL = requite::getBit(5),
  _LEFT_GROUPING = requite::getBit(4),
  _RIGHT_GROUPING = requite::getBit(3),
  _ERROR = requite::getBit(2),
  _INFERENCE_TERMINATOR = requite::getBit(1),
};
}

[[nodiscard]] constexpr requite::_token::_TokenFlags
operator|(requite::_token::_TokenFlags lhs, requite::_token::_TokenFlags rhs) {
  using Underlying = std::underlying_type_t<requite::_token::_TokenFlags>;
  return static_cast<requite::_token::_TokenFlags>(
      static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

[[nodiscard]] constexpr requite::_token::_TokenFlags
_getFlags(requite::TokenType token) {
  using namespace requite;
  using namespace requite::_token;
  using T = TokenType;
  switch (token) {
  case T::NONE:
    return _NONE;

  // OPERATORS
  case T::HASH_OPERATOR:
    return _OPERATOR;
  case T::AT_OPERATOR:
    return _OPERATOR;
  case T::GREATER_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_GREATER_OPERATOR:
    return _OPERATOR;
  case T::GREATER_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::LESS_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_LESS_OPERATOR:
    return _OPERATOR;
  case T::LESS_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::BANG_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::BANG_OPERATOR:
    return _OPERATOR;
  case T::COLON_OPERATOR:
    return _OPERATOR | _INFERENCE_TERMINATOR;
  case T::DOUBLE_COLON_OPERATOR:
    return _OPERATOR;
  case T::PLUS_OPERATOR:
    return _OPERATOR;
  case T::DASH_OPERATOR:
    return _OPERATOR;
  case T::STAR_OPERATOR:
    return _OPERATOR;
  case T::SLASH_OPERATOR:
    return _OPERATOR;
  case T::PERCENT_OPERATOR:
    return _OPERATOR;
  case T::CAROT_OPERATOR:
    return _OPERATOR;
  case T::PIPE_OPERATOR:
    return _OPERATOR;
  case T::TILDE_OPERATOR:
    return _OPERATOR;
  case T::AMPERSAND_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_AMPERSAND_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_PIPE_OPERATOR:
    return _OPERATOR;
  case T::EQUAL_OPERATOR:
    return _OPERATOR | _INFERENCE_TERMINATOR;
  case T::DOUBLE_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::PLUS_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DASH_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::STAR_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::SLASH_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::PERCENT_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOLLAR_OPERATOR:
    return _OPERATOR;
  case T::QUESTION_OPERATOR:
    return _OPERATOR;
  case T::GRAVE_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_GRAVE_OPERATOR:
    return _OPERATOR;
  case T::ARROW_OPERATOR:
    return _OPERATOR | _INFERENCE_TERMINATOR;
  case T::CONCATENATE_OPERATOR:
    return _OPERATOR;
  case T::THICK_ARROW_OPERATOR:
    return _OPERATOR;
  case T::DOT_PLUS_OPERATOR:
    return _OPERATOR;
  case T::DOT_OPERATOR:
    return _OPERATOR | _INFERENCE_TERMINATOR;
  case T::DOT_DASH_OPERATOR:
    return _OPERATOR;
  case T::DOT_STAR_OPERATOR:
    return _OPERATOR;
  case T::DOT_SLASH_OPERATOR:
    return _OPERATOR;
  case T::DOT_PERCENT_OPERATOR:
    return _OPERATOR;
  case T::DOT_LESS_OPERATOR:
    return _OPERATOR;
  case T::DOT_LESS_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOT_GREATER_OPERATOR:
    return _OPERATOR;
  case T::DOT_GREATER_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOT_DOUBLE_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOT_BANG_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_DOUBLE_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_BANG_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_GREATER_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_GREATER_EQUAL_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_LESS_OPERATOR:
    return _OPERATOR;
  case T::DOUBLE_DOT_LESS_EQUAL_OPERATOR:
    return _OPERATOR;

  // SEPERATOR SYMBOLS
  case T::TRAILER_SEPERATOR:
    return _SEPERATOR;
  case T::SEMICOLON_SEPERATOR:
    return _SEPERATOR;
  case T::COMMA_SEPERATOR:
    return _SEPERATOR | _INFERENCE_TERMINATOR;

  // GROUPING SYMBOLS
  case T::LEFT_BRACKET_GROUPING:
    return _LEFT_GROUPING;
  case T::RIGHT_BRACKET_GROUPING:
    return _RIGHT_GROUPING | _INFERENCE_TERMINATOR;
  case T::LEFT_TRIP_GROUPING:
    return _LEFT_GROUPING;
  case T::RIGHT_TRIP_GROUPING:
    return _RIGHT_GROUPING | _INFERENCE_TERMINATOR;
  case T::LEFT_PARENTHESIS_GROUPING:
    return _LEFT_GROUPING;
  case T::RIGHT_PARENTHESIS_GROUPING:
    return _RIGHT_GROUPING | _INFERENCE_TERMINATOR;

  // LITERAL
  case T::IDENTIFIER_LITERAL:
    [[fallthrough]];
  case T::CODEUNIT_LITERAL:
    [[fallthrough]];
  case T::STRING_LITERAL:
    [[fallthrough]];
  case T::INTEGER_LITERAL:
    [[fallthrough]];
  case T::FLOAT_LITERAL:
    [[fallthrough]];
  case T::LEFT_INTERPOLATED_STRING_LITERAL:
    [[fallthrough]];
  case T::MIDDLE_INTERPOLATED_STRING_LITERAL:
    [[fallthrough]];
  case T::RIGHT_INTERPOLATED_STRING_LITERAL:
    return _LITERAL;

  // ERROR
  case T::ERROR_INVALID:
    return _ERROR;
  case T::ERROR_UNTERMINATED_STRING_LITERAL:
    return _LITERAL | _ERROR;
  case T::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return _LITERAL | _ERROR;
  case T::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return _LEFT_GROUPING | _ERROR;
  case T::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return _RIGHT_GROUPING | _ERROR;
  case T::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return _LEFT_GROUPING | _ERROR;
  case T::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return _RIGHT_GROUPING | _ERROR;
  case T::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return _LEFT_GROUPING | _ERROR;
  case T::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return _RIGHT_GROUPING | _ERROR;
  default:
    break;
  }
  return _NONE;
}

[[nodiscard]] constexpr bool _getHasFlags(requite::TokenType token,
                                          requite::_token::_TokenFlags flags) {
  requite::_token::_TokenFlags token_flags = requite::_getFlags(token);
  const bool has_flags = (token_flags & flags) == flags;
  return has_flags;
}

[[nodiscard]] constexpr bool
_getHasSomeFlags(requite::TokenType token, requite::_token::_TokenFlags flags) {
  requite::_token::_TokenFlags token_flags = requite::_getFlags(token);
  const bool has_flags = (token_flags & flags) != requite::_token::_NONE;
  return has_flags;
}

constexpr bool getIsOperator(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_OPERATOR);
}

constexpr bool getIsSeperator(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_SEPERATOR);
}

constexpr bool getIsLiteral(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_LITERAL);
}

constexpr bool getIsLeftGrouping(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_LEFT_GROUPING);
}

constexpr bool getIsRightGrouping(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_RIGHT_GROUPING);
}

constexpr bool getIsGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_LEFT_GROUPING |
                                              requite::_token::_RIGHT_GROUPING);
}

constexpr bool getIsLeftUnmatchedGrouping(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_ERROR |
                                          requite::_token::_LEFT_GROUPING);
}

constexpr bool getIsRightUnmathcedGrouping(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_ERROR |
                                          requite::_token::_RIGHT_GROUPING);
}

constexpr bool getIsUnmatchedGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(
             token, requite::_token::_ERROR | requite::_token::_LEFT_GROUPING |
                        requite::_token::_RIGHT_GROUPING) &&
         requite::_getHasFlags(token, requite::_token::_ERROR);
}

constexpr bool getIsError(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_ERROR);
}

constexpr bool getIsErrorLiteral(requite::TokenType token) {
  return requite::_getHasFlags(token, requite::_token::_ERROR |
                                          requite::_token::_LITERAL);
}

constexpr bool getIsTacitTerminator(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_INFERENCE_TERMINATOR);
}

constexpr bool getIsExpressionEnd(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_SEPERATOR |
                                              requite::_token::_RIGHT_GROUPING);
}

} // namespace requite