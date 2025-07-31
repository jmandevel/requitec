// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <type_traits>

namespace requite {

template <typename EnumParam>
[[nodiscard]] constexpr std::underlying_type_t<EnumParam>
getUnderlying(const EnumParam &value) {
  return static_cast<std::underlying_type_t<EnumParam>>(value);
}

[[nodiscard]] constexpr std::uint64_t getBit(std::uint64_t bit_i) {
  return (static_cast<std::uint64_t>(1) << bit_i);
}

} // namespace requite