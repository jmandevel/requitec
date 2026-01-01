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
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/StringSaver.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

// TODO static values. only care about dynamic runtime for now (types and
// symbols all that are needed).

enum class ValueKind : std::uint_fast8_t {
  VOID,
  NULL_TYPE,
  NO_RETURN,
  VARIADIC_ARGUMENTS_TYPE,
  BOOLEAN,
  WORD,
  SIGNED,
  UNSIGNED,
  UTF8,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  MODULE,
  TABLE,
  SCOPE,
  OBJECT_TYPE,
  ENUMERATION_TYPE,
  ENUMERATOR,
  TUPLE_TYPE,
  VARIABLE,
  FUNCTION,
  METHOD,
  ENTRY_POINT
};

struct StaticValue {
  using Self = rq::StaticValue;

  rq::ValueKind _kind;

  StaticValue(rq::ValueKind kind) : _kind(kind) {}
  StaticValue(const Self &) = delete;
  StaticValue(Self &&) = delete;
  virtual ~StaticValue() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ValueKind getKind() const {
    return this->_kind;
  }
};

struct ScopeNode;
struct ScopeEntry;
struct ScopeEntryIterator;
struct ConstScopeEntryIterator;

struct ScopeEntry final {
  using Self = rq::ScopeEntry;

  llvm::PointerUnion<rq::StaticValue *, rq::ScopeNode *> _ptr_union{nullptr};

  ScopeEntry() = default;
  ScopeEntry(rq::StaticValue &value) : _ptr_union(&value) {}
  ScopeEntry(rq::ScopeNode &node) : _ptr_union(&node) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticValue() const {
    return llvm::isa<rq::StaticValue>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeNode() const {
    return llvm::isa<rq::ScopeNode>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &getStaticValue() {
    return llvm::cast<rq::StaticValue>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &getStaticValue() const {
    return llvm::cast<const rq::StaticValue>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeNode &getScopeNode() {
    return llvm::cast<rq::ScopeNode>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeNode &getScopeNode() const {
    return llvm::cast<const rq::ScopeNode>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self& rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self& rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntryIterator begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntryIterator end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstScopeEntryIterator cend() const;
};

struct ScopeNode final {
  using Self = rq::ScopeNode;

  rq::StaticValue *_value_ptr{nullptr};
  rq::ScopeEntry _scope_entry{};

  ScopeNode() = default;
  ScopeNode(rq::StaticValue &value_a, rq::StaticValue &value_b)
      : _value_ptr(&value_a), _scope_entry(value_b) {}
  ScopeNode(rq::StaticValue &value, rq::ScopeNode &node)
      : _value_ptr(&value), _scope_entry(node) {}
  ScopeNode(const Self &) = delete;
  ScopeNode(Self &&) = delete;
  ~ScopeNode() {
    if (this->_value_ptr != nullptr) {
      std::destroy_at(this->_value_ptr);
    }
    if (this->_scope_entry.getIsScopeNode()) {
      std::destroy_at(&this->_scope_entry.getScopeNode());
    } else if (this->_scope_entry.getIsStaticValue()) {
      std::destroy_at(&this->_scope_entry.getStaticValue());
    }
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStaticValue() const {
    return this->_value_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasScopeEntry() const {
    return !this->_scope_entry.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &getStaticValue() {
    return rq::dereferencePtr(this->_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &getStaticValue() const {
    return rq::dereferencePtr(this->_value_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeEntry &getScopeEntry() {
    return this->_scope_entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeEntry &getScopeEntry() const {
    return this->_scope_entry;
  }
};

struct ScopeEntryIterator final {
  using Self = rq::ScopeEntryIterator;
  using value_type = rq::StaticValue;
  using reference = rq::StaticValue &;
  using pointer = rq::StaticValue *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ScopeEntry _entry;

  ScopeEntryIterator() = default;
  explicit ScopeEntryIterator(rq::ScopeEntry& entry) 
    : _entry(entry)
  {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsStaticValue()) {
      this->_entry = rq::ScopeEntry();
    } else if (this->_entry.getIsScopeNode()) {
      this->_entry = rq::ScopeEntry(this->_entry.getScopeNode());
    } else {
      RQ_UNREACHABLE();
    }
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    (*this)++;
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue &operator*() {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &operator*() const {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StaticValue *operator->() {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue *operator->() const {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

struct ConstScopeEntryIterator final {
  using Self = rq::ConstScopeEntryIterator;
  using value_type = const rq::StaticValue;
  using reference = const rq::StaticValue &;
  using pointer = rq::StaticValue *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ScopeEntry _entry;

  ConstScopeEntryIterator() = default;
  explicit ConstScopeEntryIterator(const rq::ScopeEntry& entry)
    : _entry(entry)
  {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsStaticValue()) {
      this->_entry = rq::ScopeEntry();
    } else if (this->_entry.getIsScopeNode()) {
      this->_entry = rq::ScopeEntry(this->_entry.getScopeNode());
    } else {
      RQ_UNREACHABLE();
    }
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    (*this)++;
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue &operator*() const {
    if (this->_entry.getIsStaticValue()) {
      return this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::StaticValue *operator->() const {
    if (this->_entry.getIsStaticValue()) {
      return &this->_entry.getStaticValue();
    } else if (this->_entry.getIsScopeNode()) {
      return &this->_entry.getScopeNode().getStaticValue();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

struct Scope final : rq::StaticValue {
  using Self = rq::Scope;

  llvm::SmallDenseMap<llvm::StringRef, rq::ScopeEntry, 4> _named_values{};
  rq::ScopeEntry _unamed_values{};

  Scope() : rq::StaticValue(rq::ValueKind::TABLE) {}
  Scope(rq::ValueKind kind) : rq::StaticValue(kind) {}
  Scope(const Self &) = delete;
  Scope(Self &&) = delete;
  ~Scope() override {
    // TODO call destructors of all contained terms
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Layout final : public rq::StaticValue {
  using Self = rq::Layout;

  Layout(rq::ValueKind kind) : rq::StaticValue(kind) {}
};

struct Procedure final : public rq::StaticValue {
  using Self = rq::Procedure;

  const rq::Expression *_expression_ptr{nullptr};

  Procedure(rq::ValueKind kind) : rq::StaticValue(kind) {}
  Procedure(const Self &) = delete;
  Procedure(Self &&) = delete;
  ~Procedure() override = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(Self &&rhs) const {
    return this != &rhs;
  }
  void setExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

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

struct Module final : public rq::StaticValue {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleKind kind, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : rq::StaticValue(rq::ValueKind::MODULE), _module_kind(kind),
        _llvm_buffer_ref(std::move(buffer)), _path(path) {}
  Module(const Self &) = delete;
  Module(Self &&) = delete;
  ~Module() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  bool operator==(const Self &rhs) const { return this == &rhs; }
  bool operator!=(const Self &rhs) const { return this != &rhs; }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const {
    return this->_module_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_llvm_buffer_ref.getBufferSize() != 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
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
  [[nodiscard]] rq::Expression &popExpression() {
    rq::Expression &expression = rq::dereferencePtr(this->_expression_ptr);
    this->_expression_ptr = nullptr;
    return expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression) {
    rq::Expression &replaced_expression = this->getExpression();
    this->_expression_ptr = &expression;
    return replaced_expression;
  }
};

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::ValueKind kind) {
  using namespace rq;
  using TK = ValueKind;
  switch (kind) {
  case TK::BFLOAT16:
    return llvm::APFloat::BFloat();
  case TK::BINARY16:
    return llvm::APFloat::IEEEhalf();
  case TK::BINARY32:
    return llvm::APFloat::IEEEsingle();
  case TK::BINARY64:
    return llvm::APFloat::IEEEdouble();
  case TK::BINARY128:
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

[[nodiscard]] constexpr inline llvm::StringRef
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
    return "numeric literal StaticValue is too big";
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
[[nodiscard]] inline rq::NumericResult getNumericValue(llvm::StringRef text,
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

[[nodiscard]] inline rq::NumericResult
getNumericValue(llvm::StringRef text, llvm::APFloat &ost_term,
                rq::ValueKind semantics) {
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

struct Token;
struct Expression;

struct StaticFrame final {
  using Self = rq::StaticFrame;

  llvm::BumpPtrAllocator _llvm_arena;
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs;
  rq::Scope _table;

  StaticFrame() = default;
  StaticFrame(const Self &) = delete;
  StaticFrame(Self &&) = delete;
  ~StaticFrame() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] RQ_ALWAYS_INLINE TypeParam &allocateValue(ArgNParam... arg_n) {
    TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
    ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
    return rq::dereferencePtr(ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef saveString(llvm::Twine twine) {
    llvm::StringRef saved_string = this->_llvm_string_saver.save(twine);
    return saved_string;
  }
  [[nodiscard]] rq::Expression &acquireExpression();
  RQ_ALWAYS_INLINE void discardExpression(rq::Expression &expression) {
    this->_unused_expression_ptrs.emplace_back(&expression);
  }
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
};

} // namespace rq
