// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

#include <magic_enum.hpp>

namespace requite {

constexpr std::string_view getName(requite::TokenType token) {
  std::string_view name = magic_enum::enum_name(token);
  return name;
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
  case requite::TokenType::LEFT_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_CAP_GROUPING;
  case requite::TokenType::RIGHT_CAP_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_CAP_GROUPING;
  case requite::TokenType::LEFT_COMPAS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_COMPAS_GROUPING;
  case requite::TokenType::RIGHT_COMPAS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_COMPAS_GROUPING;
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING;
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING:
    return requite::TokenType::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING;
  default:
    break;
  }
  REQUITE_UNREACHABLE();
}

constexpr requite::Opcode getPrefixNotationOpcode(requite::TokenType token) {
  switch (token) {
  case requite::TokenType::NONE:
    return requite::Opcode::_ERROR;
  case requite::TokenType::HASH_OPERATOR:
    return requite::Opcode::_ERROR;
  case requite::TokenType::AT_OPERATOR:
    return requite::Opcode::BAKE;
  case requite::TokenType::GREATER_OPERATOR:
    return requite::Opcode::GREATER;
  case requite::TokenType::DOUBLE_GREATER_OPERATOR:
    return requite::Opcode::BITWISE_SHIFT_RIGHT;
  case requite::TokenType::GREATER_CAROT_OPERATOR:
    return requite::Opcode::BITWISE_ROTATE_RIGHT;
  case requite::TokenType::GREATER_EQUAL_OPERATOR:
    return requite::Opcode::GREATER_EQUAL;
  case requite::TokenType::LESS_OPERATOR:
    return requite::Opcode::LESS;
  case requite::TokenType::DOUBLE_LESS_OPERATOR:
    return requite::Opcode::BITWISE_SHIFT_LEFT;
  case requite::TokenType::CAROT_LESS_OPERATOR:
    return requite::Opcode::BITWISE_ROTATE_LEFT;
  case requite::TokenType::LESS_EQUAL_OPERATOR:
    return requite::Opcode::LESS_EQUAL;
  case requite::TokenType::BANG_EQUAL_OPERATOR:
    return requite::Opcode::NOT_EQUAL;
  case requite::TokenType::BANG_OPERATOR:
    return requite::Opcode::LOGICAL_COMPLEMENT;
  case requite::TokenType::DOT_OPERATOR:
    return requite::Opcode::REFLECT_VALUE;
  case requite::TokenType::DOUBLE_DOT_OPERATOR:
    return requite::Opcode::REFLECT_SYMBOL;
  case requite::TokenType::COLON_OPERATOR:
    return requite::Opcode::BIND_VALUE;
  case requite::TokenType::DOUBLE_COLON_OPERATOR:
    return requite::Opcode::BIND_SYMBOL;
  case requite::TokenType::COMMA_OPERATOR:
    return requite::Opcode::ASCRIBE;
  case requite::TokenType::BACKSLASH_OPERATOR:
    return requite::Opcode::IDENTIFY;
  case requite::TokenType::DOUBLE_BACKSLASH_OPERATOR:
    return requite::Opcode::_ERROR;
  case requite::TokenType::SEMICOLON_OPERATOR:
    return requite::Opcode::CAST;
  case requite::TokenType::DOUBLE_SEMICOLON_OPERATOR:
    return requite::Opcode::BITWISE_CAST;
  case requite::TokenType::PLUS_OPERATOR:
    return requite::Opcode::ADD;
  case requite::TokenType::DASH_OPERATOR:
    return requite::Opcode::SITUATIONAL_SUBTRACT_OR_NEGATE;
  case requite::TokenType::STAR_OPERATOR:
    return requite::Opcode::SITUATIONAL_MULTIPLY_OR_POINTER;
  case requite::TokenType::SLASH_OPERATOR:
    return requite::Opcode::SITUATIONAL_DIVIDE_OR_FAT_POINTER;
  case requite::TokenType::PERCENT_OPERATOR:
    return requite::Opcode::SITUATIONAL_MODULUS_OR_ARRAY;
  case requite::TokenType::CAROT_OPERATOR:
    return requite::Opcode::BITWISE_XOR;
  case requite::TokenType::PIPE_OPERATOR:
    return requite::Opcode::BITWISE_OR;
  case requite::TokenType::TILDE_OPERATOR:
    return requite::Opcode::BITWISE_COMPLEMENT;
  case requite::TokenType::AMBERSAND_OPERATOR:
    return requite::Opcode::SITUATIONAL_BITWISE_AND_OR_REFERENCE;
  case requite::TokenType::DOUBLE_AMPERSAND_OPERATOR:
    return requite::Opcode::LOGICAL_AND;
  case requite::TokenType::DOUBLE_PIPE_OPERATOR:
    return requite::Opcode::LOGICAL_OR;
  case requite::TokenType::EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN;
  case requite::TokenType::WALRUS_OPERATOR:
    return requite::Opcode::LOCAL;
  case requite::TokenType::DOUBLE_EQUAL_OPERATOR:
    return requite::Opcode::EQUAL;
  case requite::TokenType::PLUS_EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN_ADD;
  case requite::TokenType::DASH_EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN_SUBTRACT;
  case requite::TokenType::STAR_EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN_MULTIPLY;
  case requite::TokenType::SLASH_EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN_DIVIDE;
  case requite::TokenType::PERCENT_EQUAL_OPERATOR:
    return requite::Opcode::ASSIGN_MODULUS;
  case requite::TokenType::SWAP_OPERATOR:
    return requite::Opcode::SWAP;
  case requite::TokenType::DOLLAR_OPERATOR:
    return requite::Opcode::STOLEN_REFERENCE;
  case requite::TokenType::QUESTION_OPERATOR:
    return requite::Opcode::SITUATIONAL_INFERENCE_OR_INDETERMINATE;
  case requite::TokenType::GRAVE_OPERATOR:
    return requite::Opcode::MUTABLE;
  case requite::TokenType::ARROW_OPERATOR:
    return requite::Opcode::_ERROR;
  case requite::TokenType::CONCATENATE_OPERATOR:
    return requite::Opcode::CONCATINATE;
  case requite::TokenType::LEFT_BRACKET_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_BRACKET_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::LEFT_TRIP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_TRIP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::LEFT_CAP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_CAP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::LEFT_PARENTHESIS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_PARENTHESIS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::LEFT_COMPAS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_COMPAS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::IDENTIFIER_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::CODEUNIT_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::STRING_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::INTEGER_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::REAL_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::LEFT_INTERPOLATED_STRING_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::MIDDLE_INTERPOLATED_STRING_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::RIGHT_INTERPOLATED_STRING_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_INVALID:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNTERMINATED_STRING_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNTERMINATED_CODEUNIT_LITERAL:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_LEFT_BRACKET_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_BRACKET_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_LEFT_TRIP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_TRIP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_LEFT_CAP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_CAP_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_LEFT_PARENTHESIS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_PARENTHESIS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_LEFT_COMPAS_GROUPING:
    return requite::Opcode::_ERROR;
  case requite::TokenType::ERROR_UNMATCHED_RIGHT_COMPAS_GROUPING:
    return requite::Opcode::_ERROR;
  }
  return requite::Opcode::_ERROR;
}

} // namespace requite