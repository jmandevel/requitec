// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline std::ranges::subrange<requite::ContainingScopeIterator,
                             requite::ContainingScopeIterator,
                             std::ranges::subrange_kind::unsized>
Scope::getContainingSubrange() {
  return std::ranges::subrange(requite::ContainingScopeIterator(*(this)),
                               requite::ContainingScopeIterator());
}

} // namespace requite