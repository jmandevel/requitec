// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

inline std::ranges::subrange<requite::ProcedureIterator,
                             requite::ProcedureIterator,
                             std::ranges::subrange_kind::unsized>
Procedure::getOverloadSubrange() {
  return std::ranges::subrange(requite::ProcedureIterator(*this),
                               requite::ProcedureIterator());
}

} // namespace requite