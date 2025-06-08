// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace requite {

enum class GroupingType {
  NONE,
  VALUE_INTERPOLATION,
  STATEMENT_INTERPOLATION,
  BRACKET,
  TRIP,
  CAP,
  COMPAS,
  PARENTHESIS,
  QUOTE
};

[[nodiscard]] constexpr std::string_view getName(requite::GroupingType grouping);

} // namespace requite

#include <requite/detail/grouping_type.hpp>