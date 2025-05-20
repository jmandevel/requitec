// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/expression.hpp>

namespace requite {

bool Expression::operator==(const requite::Expression &rhs) const {
  return this == &rhs;
}

bool Expression::operator!=(const requite::Expression &rhs) const {
  return this != &rhs;
}

} // namespace requite