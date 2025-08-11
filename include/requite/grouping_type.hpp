// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace requite {

enum class GroupingType {
  NONE,
  INTERPOLATION,
  BRACKET,
  TRIP,
  PARENTHESIS
};

[[nodiscard]] constexpr std::string_view
getDescription(requite::GroupingType grouping);

} // namespace requite

#include <requite/detail/grouping_type.hpp>