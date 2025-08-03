// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

#include <magic_enum.hpp>

namespace requite {

constexpr std::string_view getName(requite::TokenType token) {
  switch (token) {
  case requite::TokenType::NONE:
    return "none";
  case requite::TokenType::HASH_OPERATOR:
    return "hash_operator";
  case requite::TokenType::AT_OPERATOR:
    return "at_operator";
  case requite::TokenType::GREATER_OPERATOR:
    return "greater_operator";
  case requite::TokenType::DOUBLE_GREATER_OPERATOR:
    return "double_greater_operator";
  case requite::TokenType::GREATER_EQUAL_OPERATOR:
    return "greater_equal_operator";
  case requite::TokenType::LESS_OPERATOR:
    return "less_operator";
  case requite::TokenType::DOUBLE_LESS_OPERATOR:
    return "double_less_operator";
  case requite::TokenType::LESS_EQUAL_OPERATOR:
    return "less_equal_operator";
  case requite::TokenType::BANG_EQUAL_OPERATOR:
    return "bang_equal_operator";
  case requite::TokenType::BANG_OPERATOR:
    return "bang_operator";
  case requite::TokenType::DOT_OPERATOR:
    return "dot_operator";
  case requite::TokenType::DOUBLE_DOT_OPERATOR:
    return "double_dot_operator";
  case requite::TokenType::COLON_OPERATOR:
    return "colon_operator";
  case requite::TokenType::DOUBLE_COLON_OPERATOR:
    return "double_colon_operator";
  case requite::TokenType::BACKSLASH_OPERATOR:
    return "backslash_operator";
  case requite::TokenType::DOUBLE_BACKSLASH_OPERATOR:
    return "double_backslash_operator";
  case requite::TokenType::PLUS_OPERATOR:
    return "plus_operator";
  case requite::TokenType::DASH_OPERATOR:
    return "dash_operator";
  case requite::TokenType::STAR_OPERATOR:
    return "star_operator";
  case requite::TokenType::SLASH_OPERATOR:
    return "slash_operator";
  case requite::TokenType::PERCENT_OPERATOR:
    return "percent_operator";
  case requite::TokenType::CAROT_OPERATOR:
    return "carot_operator";
  case requite::TokenType::PIPE_OPERATOR:
    return "pipe_operator";
  case requite::TokenType::TILDE_OPERATOR:
    return "tilde_operator";
  case requite::TokenType::AMPERSAND_OPERATOR:
    return "ampersand_operator";
  case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
    return "double_ampersand_operator";
  case requite::TokenType::DOUBLE_PIPE_OPERATOR:
    return "double_pipe_operator";
  case requite::TokenType::EQUAL_OPERATOR:
    return "equal_operator";
  case requite::TokenType::WALRUS_OPERATOR:
    return "walrus_operator";
  case requite::TokenType::PIRATE_WALRUS_OPERATOR:
    return "pirate_walrus_operator";
  case requite::TokenType::FOUR_EYED_WALRUS_OPERATOR:
    return "four_eyed_walrus_operator";
  case requite::TokenType::DOUBLE_EQUAL_OPERATOR:
    return "double_equal_operator";
  case requite::TokenType::PLUS_EQUAL_OPERATOR:
    return "plus_equal_operator";
  case requite::TokenType::DASH_EQUAL_OPERATOR:
    return "dash_equal_operator";
  case requite::TokenType::STAR_EQUAL_OPERATOR:
    return "star_equal_operator";
  case requite::TokenType::SLASH_EQUAL_OPERATOR:
    return "slash_equal_operator";
  case requite::TokenType::PERCENT_EQUAL_OPERATOR:
    return "percent_equal_operator";
  case requite::TokenType::SWAP_OPERATOR:
    return "swap_operator";
  case requite::TokenType::DOLLAR_OPERATOR:
    return "dollar_operator";
  case requite::TokenType::QUESTION_OPERATOR:
    return "question_operator";
  case requite::TokenType::GRAVE_OPERATOR:
    return "grave_operator";
  case requite::TokenType::DOUBLE_GRAVE_OPERATOR:
    return "double_grave_operator";
  case requite::TokenType::ARROW_OPERATOR:
    return "arrow_operator";
  case requite::TokenType::CONCATENATE_OPERATOR:
    return "concatenate_operator";
  case requite::TokenType::THICK_ARROW_OPERATOR:
    return "thick_arrow_operator";
  case requite::TokenType::DOT_PLUS_OPERATOR:
    return "dot_plus_operator";
  case requite::TokenType::DOT_DASH_OPERATOR:
    return "dot_dash_operator";
  case requite::TokenType::DOT_STAR_OPERATOR:
    return "dot_star_operator";
  case requite::TokenType::DOT_SLASH_OPERATOR:
    return "dot_slash_operator";
  case requite::TokenType::DOT_PERCENT_OPERATOR:
    return "dot_percent_operator";
  case requite::TokenType::DOT_LESS_OPERATOR:
    return "dot_less_operator";
  case requite::TokenType::DOT_LESS_EQUAL_OPERATOR:
    return "dot_less_equal_operator";
  case requite::TokenType::DOT_GREATER_OPERATOR:
    return "dot_greater_operator";
  case requite::TokenType::DOT_GREATER_EQUAL_OPERATOR:
    return "dot_greater_equal_operator";
  case requite::TokenType::DOT_DOUBLE_EQUAL_OPERATOR:
    return "dot_double_equal_operator";
  case requite::TokenType::UNARY_SIGNATURE_OPERATOR:
    return "unary_signature_operator";
  case requite::TokenType::DOT_BANG_EQUAL_OPERATOR:
    return "dot_bang_equal_operator";
  case requite::TokenType::TRAILER_SEPERATOR:
    return "trailer_seperator";
  case requite::TokenType::SEMICOLON_SEPERATOR:
    return "semicolon_seperator";
  case requite::TokenType::COMMA_SEPERATOR:
    return "comma_seperator";
  case requite::TokenType::LEFT_BRACKET_GROUPING:
    return "left_bracket_grouping";
  case requite::TokenType::RIGHT_BRACKET_GROUPING:
    return "right_bracket_grouping";
  case requite::TokenType::LEFT_TRIP_GROUPING:
    return "left_trip_grouping";
  case requite::TokenType::RIGHT_TRIP_GROUPING:
    return "right_trip_grouping";
  case requite::TokenType::LEFT_OPEN_CAP_GROUPING:
    return "left_open_cap_grouping";
  case requite::TokenType::RIGHT_OPEN_CAP_GROUPING:
    return "right_open_cap_grouping";
  case requite::TokenType::LEFT_CLOSED_CAP_GROUPING:
    return "left_closed_cap_gropuing";
  case requite::TokenType::RIGHT_CLOSED_CAP_GROUPING:
    return "right_closed_cap_grouping";
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
    return "left_parenthesis_grouping";
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING:
    return "right_parenthesis_grouping";
  case requite::TokenType::LEFT_SIGNATURE_GROUPING:
    return "left_signature_grouping";
  case requite::TokenType::RIGHT_SIGNATURE_GROUPING:
    return "right_signature_grouping";
  case requite::TokenType::IDENTIFIER_LITERAL:
    return "identifier_literal";
  case requite::TokenType::CODEUNIT_LITERAL:
    return "codeunit_literal";
  case requite::TokenType::STRING_LITERAL:
    return "string_literal";
  case requite::TokenType::INTEGER_LITERAL:
    return "integer_literal";
  case requite::TokenType::FRACTIONAL_LITERAL:
    return "fractional_literal";
  case requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
    return "left_interpolated_string_literal";
  case requite::TokenType::MIDDLE_INTERPOLATED_STRING_LITERAL:
    return "middle_interpolated_string_literal";
  case requite::TokenType::RIGHT_INTERPOLATED_STRING_LITERAL:
    return "right_interpolated_string_literal";
  case requite::TokenType::ERROR_INVALID:
    return "error_invalid";
  case requite::TokenType::ERROR_UNTERMINATED_STRING_LITERAL:
    return "error_unterminated_string_literal";
  case requite::TokenType::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return "error_unterminated_codeunit_literal";
  case requite::TokenType::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return "error_unmatched_left_bracket_grouping";
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return "error_unmatched_right_bracket_grouping";
  case requite::TokenType::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return "error_unmatched_trip_grouping";
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return "error_unmatched_right_trip_grouping";
  case requite::TokenType::ERROR_UNMATCHED_LEFT_OPEN_CAP_GROUPING:
    return "error_unmatched_left_open_cap_grouping";
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_OPEN_CAP_GROUPING:
    return "error_unmatched_right_open_cap_grouping";
  case requite::TokenType::ERROR_UNMATCHED_LEFT_CLOSED_CAP_GROUPING:
    return "error_unmatched_left_closed_cap_grouping";
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_CLOSED_CAP_GROUPING:
    return "error_unmatched_right_closed_cap_grouping";
  case requite::TokenType::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return "error_unmatched_left_parenthesis-grouping";
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return "error_unmatched_right_parenthesis_grouping";
  }
  return "error_unknown";
}

constexpr requite::TokenType getUnmatched(requite::TokenType token) {
  switch (token) {
  case requite::TokenType::LEFT_BRACKET_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING;
  case requite::TokenType::RIGHT_BRACKET_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING;
  case requite::TokenType::LEFT_TRIP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_TRIP_GROUPING;
  case requite::TokenType::RIGHT_TRIP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING;
  case requite::TokenType::LEFT_OPEN_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_OPEN_CAP_GROUPING;
  case requite::TokenType::RIGHT_OPEN_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_OPEN_CAP_GROUPING;
  case requite::TokenType::LEFT_CLOSED_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_CLOSED_CAP_GROUPING;
  case requite::TokenType::RIGHT_CLOSED_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_CLOSED_CAP_GROUPING;
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING;
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

namespace _token {
enum _TokenFlags : std::uint8_t {
  _NONE = 0,
  _OPERATOR = requite::getBit(6),
  _SEPERATOR = requite::getBit(5),
  _LITERAL = requite::getBit(4),
  _LEFT_GROUPING = requite::getBit(3),
  _RIGHT_GROUPING = requite::getBit(2),
  _LEFT_UNMATCHED_GROUPING = requite::getBit(1),
  _RIGHT_UNMATCHED_GROUPING = requite::getBit(0),
  _ERROR = requite::getBit(7),
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
  switch (token) {
  case TokenType::NONE:
    return _NONE;

  // OPERATORS
  case TokenType::HASH_OPERATOR:
    [[fallthrough]];
  case TokenType::AT_OPERATOR:
    [[fallthrough]];
  case TokenType::GREATER_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_GREATER_OPERATOR:
    [[fallthrough]];
  case TokenType::GREATER_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::LESS_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_LESS_OPERATOR:
    [[fallthrough]];
  case TokenType::LESS_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::BANG_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::BANG_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_DOT_OPERATOR:
    [[fallthrough]];
  case TokenType::COLON_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_COLON_OPERATOR:
    [[fallthrough]];
  case TokenType::BACKSLASH_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_BACKSLASH_OPERATOR:
    [[fallthrough]];
  case TokenType::PLUS_OPERATOR:
    [[fallthrough]];
  case TokenType::DASH_OPERATOR:
    [[fallthrough]];
  case TokenType::STAR_OPERATOR:
    [[fallthrough]];
  case TokenType::SLASH_OPERATOR:
    [[fallthrough]];
  case TokenType::PERCENT_OPERATOR:
    [[fallthrough]];
  case TokenType::CAROT_OPERATOR:
    [[fallthrough]];
  case TokenType::PIPE_OPERATOR:
    [[fallthrough]];
  case TokenType::TILDE_OPERATOR:
    [[fallthrough]];
  case TokenType::AMPERSAND_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_AMPERSAND_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_PIPE_OPERATOR:
    [[fallthrough]];
  case TokenType::EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::WALRUS_OPERATOR:
    [[fallthrough]];
  case TokenType::PIRATE_WALRUS_OPERATOR:
    [[fallthrough]];
  case TokenType::FOUR_EYED_WALRUS_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::PLUS_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::DASH_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::STAR_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::SLASH_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::PERCENT_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::SWAP_OPERATOR:
    [[fallthrough]];
  case TokenType::DOLLAR_OPERATOR:
    [[fallthrough]];
  case TokenType::QUESTION_OPERATOR:
    [[fallthrough]];
  case TokenType::GRAVE_OPERATOR:
    [[fallthrough]];
  case TokenType::DOUBLE_GRAVE_OPERATOR:
    [[fallthrough]];
  case TokenType::ARROW_OPERATOR:
    [[fallthrough]];
  case TokenType::CONCATENATE_OPERATOR:
    [[fallthrough]];
  case TokenType::THICK_ARROW_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_PLUS_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_DASH_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_STAR_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_SLASH_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_PERCENT_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_LESS_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_LESS_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_GREATER_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_GREATER_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_DOUBLE_EQUAL_OPERATOR:
    [[fallthrough]];
  case TokenType::DOT_BANG_EQUAL_OPERATOR:
    return _OPERATOR;
  

  // SEPERATOR SYMBOLS
  case TokenType::TRAILER_SEPERATOR:
    [[fallthrough]];
  case TokenType::SEMICOLON_SEPERATOR:
    [[fallthrough]];
  case TokenType::COMMA_SEPERATOR:
    return _SEPERATOR;

  // GROUPING SYMBOLS
  case TokenType::LEFT_BRACKET_GROUPING:
    return _LEFT_GROUPING;
  case TokenType::RIGHT_BRACKET_GROUPING:
    return _RIGHT_GROUPING;
  case TokenType::LEFT_TRIP_GROUPING:
    return _LEFT_GROUPING;
  case TokenType::RIGHT_TRIP_GROUPING:
    return _RIGHT_GROUPING;
  case TokenType::LEFT_OPEN_CAP_GROUPING:
    return _LEFT_GROUPING;
  case TokenType::RIGHT_OPEN_CAP_GROUPING:
    return _RIGHT_GROUPING;
  case TokenType::LEFT_CLOSED_CAP_GROUPING:
    return _LEFT_GROUPING;
  case TokenType::RIGHT_CLOSED_CAP_GROUPING:
    return _RIGHT_GROUPING;
  case TokenType::LEFT_PARENTHESIS_GROUPING:
    return _LEFT_GROUPING;
  case TokenType::RIGHT_PARENTHESIS_GROUPING:
    return _RIGHT_GROUPING;

  // LITERAL
  case TokenType::IDENTIFIER_LITERAL:
    [[fallthrough]];
  case TokenType::CODEUNIT_LITERAL:
    [[fallthrough]];
  case TokenType::STRING_LITERAL:
    [[fallthrough]];
  case TokenType::INTEGER_LITERAL:
    [[fallthrough]];
  case TokenType::FRACTIONAL_LITERAL:
    [[fallthrough]];
  case TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
    [[fallthrough]];
  case TokenType::MIDDLE_INTERPOLATED_STRING_LITERAL:
    [[fallthrough]];
  case TokenType::RIGHT_INTERPOLATED_STRING_LITERAL:
    return _LITERAL;

  // ERROR
  case TokenType::ERROR_INVALID:
    return _ERROR;
  case TokenType::ERROR_UNTERMINATED_STRING_LITERAL:
    return _ERROR;
  case TokenType::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return _ERROR;
  case TokenType::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return _LEFT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return _RIGHT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return _LEFT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return _RIGHT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_LEFT_OPEN_CAP_GROUPING:
    return _LEFT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_RIGHT_OPEN_CAP_GROUPING:
    return _RIGHT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_LEFT_CLOSED_CAP_GROUPING:
    return _LEFT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_RIGHT_CLOSED_CAP_GROUPING:
    return _RIGHT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return _LEFT_UNMATCHED_GROUPING | _ERROR;
  case TokenType::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return _RIGHT_UNMATCHED_GROUPING | _ERROR;
  default:
    break;
  }
  return _NONE;
}

[[nodiscard]] constexpr bool
_getHasSomeFlags(requite::TokenType token, requite::_token::_TokenFlags flags) {
  requite::_token::_TokenFlags token_flags = requite::_getFlags(token);
  const bool has_flags = (token_flags & flags) != requite::_token::_NONE;
  return has_flags;
}

constexpr bool getIsOperator(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_OPERATOR);
}

constexpr bool getIsSeperator(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_SEPERATOR);
}

constexpr bool getIsLiteral(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_LITERAL);
}

constexpr bool getIsLeftGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_LEFT_GROUPING);
}

constexpr bool getIsRightGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_RIGHT_GROUPING);
}

constexpr bool getIsGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_LEFT_GROUPING |
                                              requite::_token::_RIGHT_GROUPING);
}

constexpr bool getIsLeftUnmatchedGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token,
                                   requite::_token::_LEFT_UNMATCHED_GROUPING);
}

constexpr bool getIsRightUnmathcedGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(token,
                                   requite::_token::_RIGHT_UNMATCHED_GROUPING);
}

constexpr bool getIsUnmatchedGrouping(requite::TokenType token) {
  return requite::_getHasSomeFlags(
      token, requite::_token::_LEFT_UNMATCHED_GROUPING |
                 requite::_token::_RIGHT_UNMATCHED_GROUPING);
}

constexpr bool getIsError(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_ERROR);
}

constexpr bool getIsExpressionEnd(requite::TokenType token) {
  return requite::_getHasSomeFlags(token, requite::_token::_SEPERATOR |
                                              requite::_token::_RIGHT_GROUPING);
}

} // namespace requite