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

} // namespace requite