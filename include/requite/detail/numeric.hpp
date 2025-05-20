// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/codeunits.hpp>
#include <requite/unreachable.hpp>

#include <llvm/ADT/APFixedPoint.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>

#include <bit>
#include <concepts>
#include <limits>
#include <stdfloat>
#include <string>
#include <type_traits>

namespace requite {

template <typename NumericParam>
requite::NumericResult getNumericValue(llvm::StringRef text,
                                       NumericParam &out_value) {
  using Numeric = NumericParam;

  text = text.trim();
  if (text.empty()) {
    return requite::NumericResult::ERROR_EMPTY;
  }
  if constexpr (std::integral<Numeric>) {
    using Unsigned = std::make_unsigned_t<Numeric>;
    const Unsigned unsigned_max = std::numeric_limits<Unsigned>::max();
    Unsigned base = 10;
    Unsigned max_digit_multiplier = base - 1;
    Unsigned max_before_multiply = unsigned_max / max_digit_multiplier;
    Unsigned max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    Unsigned unsigned_value = 0;
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_value;
        unsigned_value = 0;
        max_digit_multiplier = base - 1;
        max_before_multiply = unsigned_max / max_digit_multiplier;
        max_before_add = unsigned_max - max_digit_multiplier;
        if (base == 0) {
          return requite::NumericResult::ERROR_ZERO_BASE;
        } else if (base > requite::MAX_BASE) {
          return requite::NumericResult::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return requite::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT;
      } else if (c == '_') {
        continue;
      } else {
        char lower_c = c;
        if (base < requite::MIN_UPPER_BASE) {
          lower_c = requite::getLowercase(c);
        }
        const Numeric digit_base_multiplier =
            requite::getDigitBaseMultiplier(lower_c);
        if (digit_base_multiplier >= base) {
          return requite::NumericResult::ERROR_INVALID_DIGIT;
        }
        if (unsigned_value > max_before_multiply) {
          return requite::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value *= base;
        if (unsigned_value > max_before_add) {
          return requite::NumericResult::ERROR_VALUE_TOO_BIG;
        }
        unsigned_value += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return requite::NumericResult::ERROR_NO_DIGITS;
    }
    if constexpr (std::signed_integral<Numeric>) {
      if (!explicit_base) {
        const Unsigned signed_max =
            static_cast<Unsigned>(std::numeric_limits<Numeric>::max());
        if (unsigned_value > signed_max) {
          return requite::NumericResult::ERROR_VALUE_TOO_BIG;
        }
      }
    }
    out_value = std::bit_cast<Numeric>(unsigned_value);
    return requite::NumericResult::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt> ||
                       std::same_as<Numeric, llvm::APSInt>) {
    // TODO

  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    requite::NumericResult result = requite::cleanRealText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), out_value, 10);
  } else {
    static_assert(false, "type not supported");
  }
  REQUITE_UNREACHABLE();
}

inline requite::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &out_value,
                requite::FloatSemantics semantics) {
  llvm::SmallString<16> buffer;
  requite::NumericResult result = requite::cleanRealText(text, buffer);
  if (result != requite::NumericResult::OK) {
    return result;
  }
  const llvm::fltSemantics &llvm_semantics =
      requite::getLlvmSemantics(semantics);
  out_value = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

requite::NumericResult cleanRealText(llvm::StringRef text,
                                      llvm::SmallString<16> &out_clean) {
  bool found_decimal = false;
  for (const char c : text) {
    switch (c) {
    case '.':
      if (found_decimal) {
        return requite::NumericResult::ERROR_MULTIPLE_DECIMAL_POINT;
      }
      found_decimal = true;
      out_clean += c;
      break;
    case '_':
      break;
    case '0':
      [[fallthrough]];
    case '1':
      [[fallthrough]];
    case '2':
      [[fallthrough]];
    case '3':
      [[fallthrough]];
    case '4':
      [[fallthrough]];
    case '5':
      [[fallthrough]];
    case '6':
      [[fallthrough]];
    case '7':
      [[fallthrough]];
    case '8':
      [[fallthrough]];
    case '9':
      out_clean += c;
      break;
    default:
      return requite::NumericResult::ERROR_INVALID_DIGIT;
    }
  }
  return requite::NumericResult::OK;
}

} // namespace requite