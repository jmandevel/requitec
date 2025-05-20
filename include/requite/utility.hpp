// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>

namespace requite {

template<typename EnumParam>
[[nodiscard]] constexpr std::underlying_type_t<EnumParam> getUnderlying(const EnumParam& value)
{
    return static_cast<std::underlying_type_t<EnumParam>>(value);
}

}