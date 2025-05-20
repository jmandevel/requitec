// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr llvm::StringRef getDescription(requite::NumericResult result) {
  switch (result) {
  case requite::NumericResult::OK:
    return "no error occured";
  case requite::NumericResult::ERROR_EMPTY:
    return "numeric literal has no codeunits";
  case requite::NumericResult::ERROR_INVALID_DIGIT:
    return "invalid digit in numeric literal";
  case requite::NumericResult::ERROR_NO_DIGITS:
    return "numeric literal has no digits";
  case requite::NumericResult::ERROR_NO_DIGITS_IN_BASE:
    return "numeric literal has base with no digits";
  case requite::NumericResult::ERROR_ZERO_BASE:
    return "numeric literal has a base of 0";
  case requite::NumericResult::ERROR_VALUE_TOO_BIG:
    return "numeric literal value is too big";
  case requite::NumericResult::ERROR_BASE_TOO_BIG:
    return "numeric literal base is too big";
  case requite::NumericResult::ERROR_MULTIPLE_DECIMAL_POINT:
    return "numeric literal has multiple decimal points";
  case requite::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT:
    return "integer literal has decimal point";
  case requite::NumericResult::ERROR_FLOAT_WITH_BASE:
    return "floating point numeric literal has base";
  }
  REQUITE_UNREACHABLE();
}

} // namespace requite