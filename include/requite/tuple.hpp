// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/symbol.hpp>

#include <vector>

namespace requite {

struct Tuple final {
  using Self = requite::Tuple;
  
  std::vector<requite::Symbol> _element_types = {};

  // tuple.cpp
  Tuple() = default;
  Tuple(const Self &that) = default;
  Tuple(Self &&that) = default;
  ~Tuple() = default;
  [[nodiscard]] Self &operator=(const Self &rhs) = default;
  [[nodiscard]] Self &operator=(Self &&rhs) = default;
  [[nodiscard]] bool operator==(const Self& rhs) const;
  [[nodiscard]] bool operator!=(const Self& rhs) const;
  [[nodiscard]] std::vector<requite::Symbol> &getElementTypes();
  [[nodiscard]] const std::vector<requite::Symbol> &getElementTypes() const;
};

} // namespace requite