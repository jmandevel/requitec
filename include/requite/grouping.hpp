// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/grouping_type.hpp>

namespace requite {

struct Grouping final {
  requite::GroupingType type = requite::GroupingType::NONE;
  unsigned token_i = 0;

  Grouping(requite::GroupingType type, unsigned token_i)
      : type(type), token_i(token_i) {}
};

} // namespace requite