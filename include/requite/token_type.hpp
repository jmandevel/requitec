// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <string_view>

namespace requite {

enum class TokenType {
  NONE,

  // OPERATORS
  HASH_OPERATOR,              // #
  AT_OPERATOR,                // @
  GREATER_OPERATOR,           // >
  DOUBLE_GREATER_OPERATOR,    // >>
  GREATER_EQUAL_OPERATOR,     // >=
  LESS_OPERATOR,              // <
  DOUBLE_LESS_OPERATOR,       // <<
  LESS_EQUAL_OPERATOR,        // <=
  BANG_EQUAL_OPERATOR,        // !=
  BANG_OPERATOR,              // !
  DOT_OPERATOR,               // .
  DOUBLE_DOT_OPERATOR,        // ..
  COLON_OPERATOR,             // :
  DOUBLE_COLON_OPERATOR,      // ::
  BACKSLASH_OPERATOR,         // \     .
  DOUBLE_BACKSLASH_OPERATOR,  // \\    .
  PLUS_OPERATOR,              // +
  DASH_OPERATOR,              // -
  STAR_OPERATOR,              // *
  SLASH_OPERATOR,             // /
  PERCENT_OPERATOR,           // %
  CAROT_OPERATOR,             // ^
  PIPE_OPERATOR,              // |
  TILDE_OPERATOR,             // ~
  AMPERSAND_OPERATOR,         // &
  DOUBLE_AMPERSAND_OPERATOR,  // &&
  DOUBLE_PIPE_OPERATOR,       // ||
  EQUAL_OPERATOR,             // =
  WALRUS_OPERATOR,            // :=
  PIRATE_WALRUS_OPERATOR,     // .=
  FOUR_EYED_WALRUS_OPERATOR,  // ::=
  DOUBLE_EQUAL_OPERATOR,      // ==
  PLUS_EQUAL_OPERATOR,        // +=
  DASH_EQUAL_OPERATOR,        // -=
  STAR_EQUAL_OPERATOR,        // *=
  SLASH_EQUAL_OPERATOR,       // /=
  PERCENT_EQUAL_OPERATOR,     // %=
  SWAP_OPERATOR,              // <>
  DOLLAR_OPERATOR,            // $
  QUESTION_OPERATOR,          // ?
  GRAVE_OPERATOR,             // `
  DOUBLE_GRAVE_OPERATOR,      // ``
  ARROW_OPERATOR,             // ->
  CONCATENATE_OPERATOR,       // +>
  THICK_ARROW_OPERATOR,       // =>
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
  UNARY_SIGNATURE_OPERATOR,   // (|)

  // SEPERATOR SYMBOLS
  TRAILER_SEPERATOR,   // <:>
  SEMICOLON_SEPERATOR, // ;
  COMMA_SEPERATOR,     // ,

  // GROUPING SYMBOLS
  LEFT_BRACKET_GROUPING,      // [
  RIGHT_BRACKET_GROUPING,     // ]
  LEFT_TRIP_GROUPING,         // {
  RIGHT_TRIP_GROUPING,        // }
  LEFT_OPEN_CAP_GROUPING,     // [.
  RIGHT_OPEN_CAP_GROUPING,    // .]
  LEFT_CLOSED_CAP_GROUPING,   // [:
  RIGHT_CLOSED_CAP_GROUPING,  // :]
  LEFT_PARENTHESIS_GROUPING,  // (
  RIGHT_PARENTHESIS_GROUPING, // )
  LEFT_SIGNATURE_GROUPING,    // (|
  RIGHT_SIGNATURE_GROUPING,   // |)
  LEFT_VARIANT_GROUPING,      // {|
  RIGHT_VARIANT_GROUPING,     // |}

  // LITERAL
  IDENTIFIER_LITERAL, // var0 MyType ☺
  CODEUNIT_LITERAL,   // 'a'
  STRING_LITERAL,     // "Hello, world!"
  INTEGER_LITERAL,    // 12345 16xDEADBEEF 2x1010_0110
  FRACTIONAL_LITERAL, // 3.14 9000.00000000001 10_020.434

  // "My_name_is_{name}._I_am_{age}_years_old."

  LEFT_INTERPOLATED_STRING_LITERAL,   // "My_name_is_
  MIDDLE_INTERPOLATED_STRING_LITERAL, // ._I_am_
  RIGHT_INTERPOLATED_STRING_LITERAL,  // _years_old."

  // ERROR
  ERROR_INVALID,
  ERROR_UNTERMINATED_STRING_LITERAL,
  ERROR_UNTERMINATED_CODEUNIT_LITERAL,
  ERROR_UNMATCHED_LEFT_BRACKET_GROUPING,
  ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING,
  ERROR_UNMATCHED_LEFT_TRIP_GROUPING,
  ERROR_UNMATCHED_RIGHT_TRIP_GROUPING,
  ERROR_UNMATCHED_LEFT_OPEN_CAP_GROUPING,
  ERROR_UNMATCHED_RIGHT_OPEN_CAP_GROUPING,
  ERROR_UNMATCHED_LEFT_CLOSED_CAP_GROUPING,
  ERROR_UNMATCHED_RIGHT_CLOSED_CAP_GROUPING,
  ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING,
  ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING,
  ERROR_UNMATCHED_LEFT_VARIANT_GROUPING,
  ERROR_UNMATCHED_RIGHT_VARIANT_GROUPING
};

[[nodiscard]] constexpr std::string_view getName(requite::TokenType token);
[[nodiscard]] constexpr requite::TokenType
getUnmatched(requite::TokenType token);
[[nodiscard]] constexpr bool getIsOperator(requite::TokenType token);
[[nodiscard]] constexpr bool getIsSeperator(requite::TokenType token);
[[nodiscard]] constexpr bool getIsLiteral(requite::TokenType token);
[[nodiscard]] constexpr bool getIsLeftGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool getIsRightGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool getIsGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool
getIsLeftUnmatchedGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool
getIsRightUnmathcedGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool getIsUnmatchedGrouping(requite::TokenType token);
[[nodiscard]] constexpr bool getIsError(requite::TokenType token);
[[nodiscard]] constexpr bool getIsExpressionEnd(requite::TokenType token);

} // namespace requite

#include <requite/detail/token_type.hpp>