// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once


#include <string_view>

namespace requite {

enum class TokenSpacing {
  NONE,
  BEFORE,
  AFTER,
  BEFORE_AND_AFTER
};

[[nodiscard]] constexpr std::string_view getDescription(requite::TokenSpacing spacing);

[[nodiscard]] constexpr requite::TokenSpacing getSpacing(char before, char after);

} // namespace requite

#include <requite/detail/token_spacing.hpp>