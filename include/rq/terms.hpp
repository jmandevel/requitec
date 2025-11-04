#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/MemoryBufferRef.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

// TODO static values. only care about dynamic runtime for now (types and
// symbols all that are needed).

enum class TermKind : std::uint_fast8_t {
  VOID_TYPE,
  NULL_TYPE,
  NO_RETURN,
  VARIADIC_ARGUMENTS_TYPE,
  BOOLEAN_TYPE,
  WORD_TYPE,
  SIGNED_TYPE,
  UNSIGNED_TYPE,
  UTF8_TYPE,
  BFLOAT16_TYPE,
  BINARY16_TYPE,
  BINARY32_TYPE,
  BINARY64_TYPE,
  BINARY128_TYPE,
  MODULE_SYMBOL,
  TABLE_SYMBOL,
  OBJECT_TYPE,
  ENUMERATION_TYPE,
  ENUMERATOR_SYMBOL,
  TUPLE_TYPE,
  VARIABLE_SYMBOL,
  FUNCTION_SYMBOL,
  METHOD_SYMBOL
};

struct Term {
  using Self = rq::Term;

  rq::TermKind _kind;

  Term(rq::TermKind kind) : _kind(kind) {}
  Term(const Self &) = delete;
  Term(Self &&) = delete;
  virtual ~Term() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TermKind getKind() const {
    return this->_kind;
  }
};

struct Node;

using TermPtr = llvm::PointerUnion<rq::Term *, rq::Expression *>;
using EntryPtr = llvm::PointerUnion<rq::TermPtr, rq::Node *>;

struct Node final {
  llvm::PointerIntPair<rq::TermPtr, 1> _this_ptr{nullptr};
  llvm::PointerIntPair<rq::EntryPtr, 2> _next_ptr{nullptr};
};

struct Table : rq::Term {
  using Self = rq::Table;

  llvm::SmallDenseMap<llvm::StringRef, rq::EntryPtr, 4> _terms{};

  Table() : rq::Term(rq::TermKind::TABLE_SYMBOL) {}
  Table(rq::TermKind kind) : rq::Term(kind) {}
  Table(const Self &) = delete;
  Table(Self &&) = delete;
  ~Table() {
    // TODO call destructors of all contained terms
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Layout final : public rq::Table {
  using Self = rq::Layout;

  Layout(rq::TermKind kind) : rq::Table(kind) {}
};

struct Procedure final : public rq::Table {
  using Self = rq::Procedure;

  // NOTE: procedures do not internally point to their own static frame, because
  // the static frame of a procedure has lifetime that continues only while the
  // procedure is being built.

  Procedure(rq::TermKind kind) : rq::Table(kind) {}
};

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  switch (kind) {
  case rq::ModuleKind::NONE:
    return "none";
  case rq::ModuleKind::SOURCE:
    return "source";
  case rq::ModuleKind::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

enum class Language : std::uint_fast8_t {
  NONE,
  UNKNOWN,
  NORMATIVE_REQUITE,
  SYMBOLIC_REQUITE
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
getDescription(rq::Language language) {
  switch (language) {
  case rq::Language::NONE:
    return "none";
  case rq::Language::UNKNOWN:
    return "unknown language";
  case rq::Language::NORMATIVE_REQUITE:
    return "normative requite";
  case rq::Language::SYMBOLIC_REQUITE:
    return "symbolic requite";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::Language
getLanguageOfExtension(llvm::StringRef extension) {
  if (extension == ".rq") {
    return rq::Language::NORMATIVE_REQUITE;
  } else if (extension == ".srq") {
    return rq::Language::SYMBOLIC_REQUITE;
  }
  return rq::Language::UNKNOWN;
}

struct Module final : public rq::Term {
  using Self = rq::Module;

  rq::ModuleKind _kind;
  rq::Language _language;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleKind kind, rq::Language language, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : rq::Term(rq::TermKind::MODULE_SYMBOL), _kind(kind), _language(language),
        _llvm_buffer_ref(std::move(buffer)), _path(path) {}
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getKind() const {
    return this->_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Language getLanguage() const {
    return this->_language;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInitialized() const {
    return this->getLanguage() != rq::Language::NONE &&
           this->getKind() != rq::ModuleKind::NONE && !this->_path.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  llvm::StringRef getSourceText() const {
    return this->_llvm_buffer_ref.getBuffer();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
  RQ_ALWAYS_INLINE void changeExpression(rq::Expression &expression) {
    RQ_ASSERT(this->_expression_ptr != nullptr, "no expression");
    this->_expression_ptr = &expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::TermKind kind) {
  using namespace rq;
  using TK = TermKind;
  switch (kind) {
  case TK::BFLOAT16_TYPE:
    return llvm::APFloat::BFloat();
  case TK::BINARY16_TYPE:
    return llvm::APFloat::IEEEhalf();
  case TK::BINARY32_TYPE:
    return llvm::APFloat::IEEEsingle();
  case TK::BINARY64_TYPE:
    return llvm::APFloat::IEEEdouble();
  case TK::BINARY128_TYPE:
    return llvm::APFloat::IEEEquad();
  default:
    break;
  }
  RQ_UNREACHABLE();
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

[[nodiscard]] constexpr llvm::StringRef
getDescription(rq::NumericResult result) {
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
    return "numeric literal term is too big";
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

[[nodiscard]] inline rq::NumericResult
cleanFloatText(llvm::StringRef text, llvm::SmallString<16> &ost_clean) {
  bool found_decimal = false;
  for (const char c : text) {
    switch (c) {
    case '.':
      if (found_decimal) {
        return rq::NumericResult::ERROR_MULTIPLE_DECIMAL_POINT;
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
      return rq::NumericResult::ERROR_INVALID_DIGIT;
    }
  }
  return rq::NumericResult::OK;
}

template <typename NumericParam>
[[nodiscard]] inline rq::NumericResult getNumericTerm(llvm::StringRef text,
                                                      NumericParam &ost_term) {
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
    ost_term = std::bit_cast<Numeric>(unsigned_term);
    return rq::NumericResult::OK;
  } else if constexpr (std::same_as<Numeric, llvm::APInt>) {
    const unsigned bit_width = ost_term.getBitWidth();
    llvm::APInt max_base = llvm::APInt(bit_width, rq::MAX_BASE);
    llvm::APInt min_upper_base = llvm::APInt(bit_width, rq::MIN_UPPER_BASE);
    const llvm::APInt unsigned_max = llvm::APInt::getMaxValue(bit_width);
    llvm::APInt base = llvm::APInt(bit_width, 10);
    llvm::APInt max_digit_multiplier = base - 1;
    llvm::APInt max_before_multiply = unsigned_max.udiv(max_digit_multiplier);
    llvm::APInt max_before_add = unsigned_max - max_digit_multiplier;
    bool explicit_base = false;
    bool digit_found = false;
    llvm::APInt unsigned_term = llvm::APInt(bit_width, 0);
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
            llvm::APInt(bit_width, rq::getDigitBaseMultiplier(lower_c));
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
    ost_term = unsigned_term;
    return rq::NumericResult::OK;
  } else if constexpr (std::floating_point<Numeric>) {
    llvm::SmallString<16> clean_text;
    std::ignore = rq::cleanFloatText(text, clean_text);
    std::from_chars(clean_text.begin(), clean_text.end(), ost_term, 10);
  } else {
    static_assert(false, "type not supported");
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::NumericResult getNumericTerm(llvm::StringRef text,
                                                      llvm::APFloat &ost_term,
                                                      rq::TermKind semantics) {
  llvm::SmallString<16> buffer;
  rq::NumericResult result = rq::cleanFloatText(text, buffer);
  if (result != rq::NumericResult::OK) {
    return result;
  }
  const llvm::fltSemantics &llvm_semantics =
      rq::getLlvmFloatSemantics(semantics);
  ost_term = llvm::APFloat(llvm_semantics, buffer);
  return result;
}

} // namespace rq