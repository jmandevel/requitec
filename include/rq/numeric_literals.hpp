#pragma once

#include <rq/source_ranger.hpp>
#include <rq/symbol.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>

#include <cstdint>

namespace rq {

enum class SuffixTypeKind {
  NONE,
  INTEGER,
  SIGNED,
  UNSIGNED,
  FLOAT,
  BINARY,
  BFLOAT
};

enum class SuffixScalarKind {
  NONE,
  EXACT,
  LEAST,
  FASTEST,
  BEST,
  INDEX,
  ADDRESS
};

struct Suffix final {
  using Self = rq::Suffix;

  rq::SuffixTypeKind type_kind{rq::SuffixTypeKind::NONE};
  rq::SuffixScalarKind scalar_kind{rq::SuffixScalarKind::NONE};
  unsigned scalar{0};
};

enum class SuffixResult {
  OK,
  ERROR_INVALID_TYPE_KIND,
  ERROR_INVALID_SCALAR_KIND,
  ERROR_INVALID_SCALAR_FOR_TYPE,
  ERROR_NO_SCALAR_VALUE_FOR_SCALAR_KIND,
  ERROR_SUFFIX_SCALAR_VALUE_LEADING_ZERO,
  ERROR_SCALAR_VALUE_TOO_BIG
};

[[nodiscard]] inline llvm::StringRef getDescription(rq::SuffixResult result) {
  using namespace rq;
  using SR = SuffixResult;
  switch (result) {
  case SR::OK:
    return "suffix ok";
  case SR::ERROR_INVALID_TYPE_KIND:
    return "invalid suffix type";
  case SR::ERROR_INVALID_SCALAR_KIND:
    return "invalid suffix scalar";
  case SR::ERROR_NO_SCALAR_VALUE_FOR_SCALAR_KIND:
    return "invalid suffix scalar for suffix type";
  case SR::ERROR_SUFFIX_SCALAR_VALUE_LEADING_ZERO:
    return "suffix scalar value starts with 0";
  case SR::ERROR_SCALAR_VALUE_TOO_BIG:
    return "suffix scalar value too big";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] rq::SuffixResult inline getSuffix(llvm::StringRef text,
                                                rq::Suffix &out_suffix) {
  // NOTE: errors unrelated to suffixes are not found until
  // rq::getNumericValue() is called later.
  text = text.trim();
  rq::SourceRanger ranger{text};
  while (!ranger.getIsDone()) {
    const char c0 = ranger.getChar(0);
    ranger.incrementChar(1);
    if (c0 == 'x') {
      return rq::SuffixResult::OK;
    } else if (c0 == 'i') {
      out_suffix.type_kind = rq::SuffixTypeKind::INTEGER;
      break;
    } else if (c0 == 's') {
      out_suffix.type_kind = rq::SuffixTypeKind::SIGNED;
      break;
    } else if (c0 == 'u') {
      out_suffix.type_kind = rq::SuffixTypeKind::UNSIGNED;
      break;
    } else if (c0 == 'f') {
      out_suffix.type_kind = rq::SuffixTypeKind::FLOAT;
      break;
    } else if (c0 == 'b') {
      const char c1 = ranger.getChar(0);
      if (c1 == 'f') {
        ranger.incrementChar(1);
        out_suffix.type_kind = rq::SuffixTypeKind::BFLOAT;
        break;
      }
      out_suffix.type_kind = rq::SuffixTypeKind::BINARY;
      break;
    }
  }
  if (ranger.getIsDone()) {
    return rq::SuffixResult::OK;
  }
  const unsigned base = 10;
  const unsigned max_digit_multiplier = base - 1;
  const unsigned max_before_multiply =
      rq::MAX_SCALED_BUILTIN_SCALAR / max_digit_multiplier;
  const unsigned max_before_add =
      rq::MAX_SCALED_BUILTIN_SCALAR - max_digit_multiplier;
  bool scalar_digit_found = false;
  while (!ranger.getIsDone()) {
    const char c0 = ranger.getChar(0);
    ranger.incrementChar(1);
    if (rq::getIsDecimalDigit(c0)) {
      const unsigned digit_base_multiplier = rq::getDigitBaseMultiplier(c0);
      if (out_suffix.scalar > max_before_multiply) {
        return rq::SuffixResult::ERROR_SCALAR_VALUE_TOO_BIG;
      }
      out_suffix.scalar *= base;
      if (out_suffix.scalar > max_before_add) {
        return rq::SuffixResult::ERROR_SCALAR_VALUE_TOO_BIG;
      }
      out_suffix.scalar += digit_base_multiplier;
      if (!scalar_digit_found && out_suffix.scalar == 0) {
        return rq::SuffixResult::ERROR_SUFFIX_SCALAR_VALUE_LEADING_ZERO;
      }
      scalar_digit_found = true;
    }
    RQ_TODO_IMPLEMENTATION();
    switch (c0) {
    case 'x':
      return rq::SuffixResult::OK;
    case 'e':

    case 'f':

    case 'l':

    case 'b':

    case 'i':

    case 'a':
      break;
    }
  }
  return rq::SuffixResult::OK;
}

enum class NumericResult {
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

[[nodiscard]] inline llvm::StringRef getDescription(rq::NumericResult result) {
  using namespace rq;
  using NR = NumericResult;
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
  RQ_UNREACHABLE();
}

static constexpr unsigned MAX_BASE = 64;

static constexpr unsigned MIN_UPPER_BASE = 36;

[[nodiscard]] inline rq::NumericResult
cleanFloatText(llvm::StringRef text, llvm::SmallString<16> &out_clean) {
  bool found_decimal = false;
  for (const char c : text) {
    switch (c) {
    case '.':
      if (found_decimal) {
        return rq::NumericResult::ERROR_MULTIPLE_DECIMAL_POINT;
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
      return rq::NumericResult::ERROR_INVALID_DIGIT;
    }
  }
  return rq::NumericResult::OK;
}

template <typename NumericParam>
[[nodiscard]] inline rq::NumericResult getNumericValue(llvm::StringRef text,
                                                       NumericParam &out_term) {
  using Numeric = NumericParam;
  text = text.trim();
  if (text.empty()) {
    return rq::NumericResult::ERROR_EMPTY;
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
          return rq::NumericResult::ERROR_ZERO_BASE;
        } else if (base > rq::MAX_BASE) {
          return rq::NumericResult::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT;
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
          return rq::NumericResult::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term > max_before_multiply) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term > max_before_add) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResult::ERROR_NO_DIGITS;
    }
    if constexpr (std::signed_integral<Numeric>) {
      if (!explicit_base) {
        const Unsigned signed_max =
            static_cast<Unsigned>(std::numeric_limits<Numeric>::max());
        if (unsigned_term > signed_max) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
      }
    }
    out_term = std::bit_cast<Numeric>(unsigned_term);
    return rq::NumericResult::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt>) {
    const unsigned bit_depth = out_term.getBitWidth();
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
          return rq::NumericResult::ERROR_ZERO_BASE;
        } else if (base.ugt(max_base)) {
          return rq::NumericResult::ERROR_BASE_TOO_BIG;
        }
        explicit_base = true;
        digit_found = false;
      } else if (c == '.') {
        return rq::NumericResult::ERROR_INTEGER_WITH_DECIMAL_POINT;
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
          return rq::NumericResult::ERROR_INVALID_DIGIT;
        }
        if (unsigned_term.ugt(max_before_multiply)) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term *= base;
        if (unsigned_term.ugt(max_before_add)) {
          return rq::NumericResult::ERROR_TERM_TOO_BIG;
        }
        unsigned_term += digit_base_multiplier;
        digit_found = true;
      }
    }
    if (!digit_found) {
      return rq::NumericResult::ERROR_NO_DIGITS;
    }
    out_term = unsigned_term;
    return rq::NumericResult::OK;
  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    std::ignore = rq::cleanFloatText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), out_term, 10);
  } else {
    static_assert(false, "type not supported");
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &out_term,
                const llvm::fltSemantics &llvm_semantics) {
  llvm::SmallString<16> buffer;
  rq::NumericResult result = rq::cleanFloatText(text, buffer);
  if (result != rq::NumericResult::OK) {
    return result;
  }
  out_term = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

} // namespace rq