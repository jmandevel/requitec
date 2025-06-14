// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline std::ranges::subrange<requite::ScopeIterator,
                             requite::ScopeIterator,
                             std::ranges::subrange_kind::unsized>
Table::getScopeSubrange() {
  return std::ranges::subrange(requite::ScopeIterator(this->getStubScope()),
                               requite::ScopeIterator());
}

} // namespace requite