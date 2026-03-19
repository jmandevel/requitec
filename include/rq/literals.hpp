#pragma once

#include <rq/entity.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>

#include <cstdint>

namespace rq {

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::EntityKind kind) {
  using namespace rq;
  switch (kind) {
  case rq::EntityKind::SY_BFLOAT16:
    return llvm::APFloat::BFloat();
  case rq::EntityKind::SY_BINARY16:
    return llvm::APFloat::IEEEhalf();
  case rq::EntityKind::SY_BINARY32:
    return llvm::APFloat::IEEEsingle();
  case rq::EntityKind::SY_BINARY64:
    return llvm::APFloat::IEEEdouble();
  case rq::EntityKind::SY_BINARY128:
    return llvm::APFloat::IEEEquad();
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class NumericResultCode : unsigned {
  OK,
  ERROR_EMPTY,
  ERROR_INVALID_DIGIT,
  ERROR_NO_DIGITS,
  ERROR_NO_DIGITS_IN_BASE,
  ERROR_ZERO_BASE,
  ERROR_TERM_TOO_BIG,
  ERROR_BASE_TOO_BIG,
  ERROR_MULTIPLE_DECIMAL_POINT,
  ERROR_INTEGER_WITH_DECIMAL_POINT,
  ERROR_FLOAT_WITH_BASE
};

[[nodiscard]] constexpr inline llvm::StringRef
getDescription(rq::NumericResultCode result) {
  using namespace rq;
  using NR = NumericResultCode;
  switch (result) {
  case NR::OK:
    return "no error occured";
  case NR::ERROR_EMPTY:
    return "numeric literal has no codeunits";
  case NR::ERROR_INVALID_DIGIT:
    return "invalid digit in numeric literal";
  case NR::ERROR_NO_DIGITS:
    return "numeric literal has no digits";
  case NR::ERROR_NO_DIGITS_IN_BASE:
    return "numeric literal has base with no digits";
  case NR::ERROR_ZERO_BASE:
    return "numeric literal has a base of 0";
  case NR::ERROR_TERM_TOO_BIG:
    return "numeric literal Value is too big";
  case NR::ERROR_BASE_TOO_BIG:
    return "numeric literal base is too big";
  case NR::ERROR_MULTIPLE_DECIMAL_POINT:
    return "numeric literal has multiple decimal points";
  case NR::ERROR_INTEGER_WITH_DECIMAL_POINT:
    return "integer literal has decimal point";
  case NR::ERROR_FLOAT_WITH_BASE:
    return "floating point numeric literal has base";
  }
  return "unkown error";
}

static constexpr unsigned MAX_BASE = 64;

static constexpr unsigned MIN_UPPER_BASE = 36;

[[nodiscard]] inline rq::NumericResultCode
cleanFloatText(llvm::StringRef text, llvm::SmallString<16> &ost_clean) {
  bool found_decimal = false;
  for (const char c : text) {
    switch (c) {
    case '.':
      if (found_decimal) {
        return rq::NumericResultCode::ERROR_MULTIPLE_DECIMAL_POINT;
      }
      found_decimal = true;
      ost_clean += c;
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
      ost_clean += c;
      break;
    default:
      return rq::NumericResultCode::ERROR_INVALID_DIGIT;
    }
  }
  return rq::NumericResultCode::OK;
}

template <typename NumericParam>
[[nodiscard]] inline rq::NumericResultCode
getNumericValue(llvm::StringRef text, NumericParam &ost_term) {
  using Numeric = NumericParam;
  text = text.trim();
  if (text.empty()) {
    return rq::NumericResultCode::ERROR_EMPTY;
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
    Unsigned unsigned_term = 0;
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_term;
        unsigned_term = 0;
        max_digit_multiplier = base - 1;
        max_before_multiply = unsigned_max / max_digit_multiplier;
        max_before_add = unsigned_max - max_digit_multiplier;
        if (base == 0) {
          return rq::NumericResultCode::ERROR_ZERO_BASE;
        } else if (base > rq::MAX_BASE) {
          return rq::NumericResultCode::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResultCode::ERROR_INTEGER_WITH_DECIMAL_POINT;
      } else if (c == '_') {
        continue;
      } else {
        char lower_c = c;
        if (base < rq::MIN_UPPER_BASE) {
          lower_c = rq::getLowercaseLetter(c);
        }
        const Numeric digit_base_multiplier =
            rq::getDigitBaseMultiplier(lower_c);
        if (digit_base_multiplier >= base) {
          return rq::NumericResultCode::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term > max_before_multiply) {
          return rq::NumericResultCode::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term > max_before_add) {
          return rq::NumericResultCode::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResultCode::ERROR_NO_DIGITS;
    }
    if constexpr (std::signed_integral<Numeric>) {
      if (!explicit_base) {
        const Unsigned signed_max =
            static_cast<Unsigned>(std::numeric_limits<Numeric>::max());
        if (unsigned_term > signed_max) {
          return rq::NumericResultCode::ERROR_TERM_TOO_BIG;
        }
      }
    }
    ost_term = std::bit_cast<Numeric>(unsigned_term);
    return rq::NumericResultCode::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt>) {
    const unsigned bit_depth = ost_term.getBitWidth();
    llvm::APInt max_base = llvm::APInt(bit_depth, rq::MAX_BASE);
    llvm::APInt min_upper_base = llvm::APInt(bit_depth, rq::MIN_UPPER_BASE);
    const llvm::APInt unsigned_max = llvm::APInt::getMaxValue(bit_depth);
    llvm::APInt base = llvm::APInt(bit_depth, 10);
    llvm::APInt max_digit_multiplier = base - 1;
    llvm::APInt max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
    llvm::APInt max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    llvm::APInt unsigned_term = llvm::APInt(bit_depth, 0);
    for (const char c : text) {
      if (!explicit_base && c == 'x') {
        base = unsigned_term;
        unsigned_term = 0;
        max_digit_multiplier = base - 1;
        max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
        max_before_add = unsigned_max - max_digit_multiplier;
        if (base == 0) {
          return rq::NumericResultCode::ERROR_ZERO_BASE;
        } else if (base.ugt(max_base)) {
          return rq::NumericResultCode::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResultCode::ERROR_INTEGER_WITH_DECIMAL_POINT;
      } else if (c == '_') {
        continue;
      } else {
        char lower_c = c;
        if (base.ugt(min_upper_base)) {
          lower_c = rq::getLowercaseLetter(c);
        }
        const llvm::APInt digit_base_multiplier =
            llvm::APInt(bit_depth, rq::getDigitBaseMultiplier(lower_c));
        if (digit_base_multiplier.uge(base)) {
          return rq::NumericResultCode::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term.ugt(max_before_multiply)) {
          return rq::NumericResultCode::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term.ugt(max_before_add)) {
          return rq::NumericResultCode::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResultCode::ERROR_NO_DIGITS;
    }
    ost_term = unsigned_term;
    return rq::NumericResultCode::OK;
  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    std::ignore = rq::cleanFloatText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), ost_term, 10);
  } else {
    static_assert(false, "type not supported");
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::NumericResultCode
getNumericValue(llvm::StringRef text, llvm::APFloat &ost_term,
                rq::EntityKind semantics) {
  llvm::SmallString<16> buffer;
  rq::NumericResultCode result = rq::cleanFloatText(text, buffer);
  if (result != rq::NumericResultCode::OK) {
    return result;
  }
  const llvm::fltSemantics &llvm_semantics =
      rq::getLlvmFloatSemantics(semantics);
  ost_term = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

} // namespace rq