// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace requite {

constexpr std::string_view getDescription(requite::GroupingType grouping) {
  using namespace requite;
  using G = GroupingType;
  switch (grouping) {
  case G::NONE:
    return "none";
  case G::INTERPOLATION:
    return "interpolation";
  case G::BRACKET:
    return "bracket";
  case G::TRIP:
    return "trip";
  case G::PARENTHESIS:
    return "parenthsis";
  default:
    break;
  }
  return "unknown";
}

} // namespace requite