// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

#include <magic_enum.hpp>

#include <string_view>

namespace requite {

constexpr std::string_view getName(requite::GroupingType grouping) {
  std::string_view name = magic_enum::enum_name(grouping);
  return name;
}

} // namespace requite