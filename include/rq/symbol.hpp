#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>
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

struct Scope;
struct Module;
struct Procedure;

enum class SymbolKind : std::uint_fast8_t {
  NONE,

  // ROOT WITH TYPE ATTRIBUTES
  TYPE,

  // BUILTIN SIMPLE
  EXPRESSION,
  VOID,
  NULL_,
  NO_RETURN,
  VARIADIC_ARGUMENTS,
  BOOLEAN,
  UTF8,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,

  // BUILTIN DEPTH
  WORD,
  SIGNED,
  UNSIGNED,

  // SUBTYPE
  RANGE,
  REFERENCE,
  POINTER,
  FAT_POINTER,
  ARRAY,
  TWO_PART_SEQUENCE,
  THREE_PART_SEQUENCE,

  // EVALUATED
  MODULE,
  TOP_SCOPE,
  TABLE,
  SCOPE,
  OBJECT,
  ENUMERATION,
  ENUMERATOR,
  LAYOUT,
  SIGNATURE,
  VARIABLE,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  CONSTRUCTOR,
  DESTRUCTOR,
  RANGER,

  // TEMPLATE
  TEMPLATE_OBJECT,
  TEMPLATE_ENUMERATION,
  TEMPLATE_VARIABLE,
  TEMPLATE_FUNCTION,
  TEMPLATE_METHOD,
  TEMPLATE_EXTENSION_FUNCTION,
  TEMPLATE_EXTENSION_METHOD,
  TEMPLATE_CONSTRUCTOR,

  // PARTIAL SPECIALIZATION
  PARTIAL_OBJECT,
  PARTIAL_ENUMERATION,
  PARTIAL_VARIABLE,
  PARTIAL_METHOD,
  PARTIAL_FUNCTION,
  PARTIAL_EXTENSION_FUNCTION,
  PARTIAL_EXTENSION_METHOD,
  PARTIAL_CONSTRUCTOR,

  // FULL SPECIALIZATION
  FULL_OBJECT,
  FULL_ENUMERATION,
  FULL_VARIABLE,
  FULL_FUNCTION,
  FULL_METHOD,
  FULL_EXTENSION_FUNCTION,
  FULL_EXTENSION_METHOD,
  FULL_CONSTRUCTOR
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::SymbolKind kind) {
  switch (kind) {
  case rq::SymbolKind::NONE:
    return "none";

  // ROOT WITH TYPE ATTRIBUTES
  case rq::SymbolKind::TYPE:
    return "type";

  // BUILTIN SIMPLE
  case rq::SymbolKind::EXPRESSION:
    return "expression";
  case rq::SymbolKind::VOID:
    return "void";
  case rq::SymbolKind::NULL_:
    return "null";
  case rq::SymbolKind::NO_RETURN:
    return "no-return";
  case rq::SymbolKind::VARIADIC_ARGUMENTS:
    return "variadic-arguments";
  case rq::SymbolKind::BOOLEAN:
    return "boolean";
  case rq::SymbolKind::UTF8:
    return "utf8";
  case rq::SymbolKind::BFLOAT16:
    return "bfloat16";
  case rq::SymbolKind::BINARY16:
    return "binary16";
  case rq::SymbolKind::BINARY32:
    return "binary32";
  case rq::SymbolKind::BINARY64:
    return "binary64";
  case rq::SymbolKind::BINARY128:
    return "binary128";

  // BUILTIN DEPTH
  case rq::SymbolKind::WORD:
    return "word";
  case rq::SymbolKind::SIGNED:
    return "signed";
  case rq::SymbolKind::UNSIGNED:
    return "unsigned";

  // SUBTYPE
  case rq::SymbolKind::RANGE:
    return "range";
  case rq::SymbolKind::REFERENCE:
    return "reference";
  case rq::SymbolKind::POINTER:
    return "pointer";
  case rq::SymbolKind::FAT_POINTER:
    return "fat-pointer";
  case rq::SymbolKind::ARRAY:
    return "array";
  case rq::SymbolKind::TWO_PART_SEQUENCE:
    return "two-part-sequence";
  case rq::SymbolKind::THREE_PART_SEQUENCE:
    return "three-part-sequence";

  // EVALUATED
  case rq::SymbolKind::MODULE:
    return "module";
  case rq::SymbolKind::TOP_SCOPE:
    return "top-scope";
  case rq::SymbolKind::TABLE:
    return "table";
  case rq::SymbolKind::SCOPE:
    return "scope";
  case rq::SymbolKind::OBJECT:
    return "object";
  case rq::SymbolKind::ENUMERATION:
    return "enumeration";
  case rq::SymbolKind::ENUMERATOR:
    return "enumerator";
  case rq::SymbolKind::LAYOUT:
    return "layout";
  case rq::SymbolKind::SIGNATURE:
    return "signature";
  case rq::SymbolKind::VARIABLE:
    return "variable";
  case rq::SymbolKind::ENTRY_POINT:
    return "entry-point";
  case rq::SymbolKind::FUNCTION:
    return "function";
  case rq::SymbolKind::METHOD:
    return "method";
  case rq::SymbolKind::EXTENSION_FUNCTION:
    return "extension-function";
  case rq::SymbolKind::EXTENSION_METHOD:
    return "extension-method";
  case rq::SymbolKind::CONSTRUCTOR:
    return "constructor";
  case rq::SymbolKind::DESTRUCTOR:
    return "destructor";
  case rq::SymbolKind::RANGER:
    return "ranger";

  // TEMPLATE
  case rq::SymbolKind::TEMPLATE_OBJECT:
    return "template-object";
  case rq::SymbolKind::TEMPLATE_ENUMERATION:
    return "template-enumeration";
  case rq::SymbolKind::TEMPLATE_VARIABLE:
    return "template-variable";
  case rq::SymbolKind::TEMPLATE_FUNCTION:
    return "template-function";
  case rq::SymbolKind::TEMPLATE_METHOD:
    return "template-method";
  case rq::SymbolKind::TEMPLATE_EXTENSION_FUNCTION:
    return "template-extension-function";
  case rq::SymbolKind::TEMPLATE_EXTENSION_METHOD:
    return "template-extension-method";
  case rq::SymbolKind::TEMPLATE_CONSTRUCTOR:
    return "template-constructor";

  // PARTIAL SPECIALIZATION
  case rq::SymbolKind::PARTIAL_OBJECT:
    return "partial-object";
  case rq::SymbolKind::PARTIAL_ENUMERATION:
    return "partial-enumeration";
  case rq::SymbolKind::PARTIAL_VARIABLE:
    return "partial-variable";
  case rq::SymbolKind::PARTIAL_METHOD:
    return "partial-method";
  case rq::SymbolKind::PARTIAL_FUNCTION:
    return "partial-function";
  case rq::SymbolKind::PARTIAL_EXTENSION_FUNCTION:
    return "partial-extension-function";
  case rq::SymbolKind::PARTIAL_EXTENSION_METHOD:
    return "partial-extension-method";
  case rq::SymbolKind::PARTIAL_CONSTRUCTOR:
    return "partial-constructor";

  // FULL SPECIALIZATION
  case rq::SymbolKind::FULL_OBJECT:
    return "full-object";
  case rq::SymbolKind::FULL_ENUMERATION:
    return "full-enumeration";
  case rq::SymbolKind::FULL_VARIABLE:
    return "full-variable";
  case rq::SymbolKind::FULL_FUNCTION:
    return "full-function";
  case rq::SymbolKind::FULL_METHOD:
    return "full-method";
  case rq::SymbolKind::FULL_EXTENSION_FUNCTION:
    return "full-extension-function";
  case rq::SymbolKind::FULL_EXTENSION_METHOD:
    return "full-extension-method";
  case rq::SymbolKind::FULL_CONSTRUCTOR:
    return "full-constructor";
  }
  RQ_UNREACHABLE();
}

enum class SymbolFlags : std::uint_fast16_t {
  NONE = 0,
  ROOT = rq::getBit(0),
  SUBTYPE = rq::getBit(1),
  SIMPLE = rq::getBit(2),
  DEPTHED = rq::getBit(3),
  SCOPE = rq::getBit(4),
  PROCEDURE = rq::getBit(5),
  FLOAT = rq::getBit(6),
  TEMPLATE = rq::getBit(7),
  PARTIAL_SPECIALIZATION = rq::getBit(8),
  FULL_SPECIALIZTION = rq::getBit(9)
};

template <> struct is_flags<rq::SymbolFlags> : std::true_type {};

[[nodiscard]] inline rq::SymbolFlags getFlags(rq::SymbolKind kind) {
  using SF = rq::SymbolFlags;
  switch (kind) {
  case rq::SymbolKind::NONE:
    return SF::NONE;

  // ROOT WITH TYPE ATTRIBUTES
  case rq::SymbolKind::TYPE:
    return SF::ROOT;

  // BUILTIN SIMPLE
  case rq::SymbolKind::EXPRESSION:
    return SF::SIMPLE;
  case rq::SymbolKind::VOID:
    return SF::SIMPLE;
  case rq::SymbolKind::NULL_:
    return SF::SIMPLE;
  case rq::SymbolKind::NO_RETURN:
    return SF::SIMPLE;
  case rq::SymbolKind::VARIADIC_ARGUMENTS:
    return SF::SIMPLE;
  case rq::SymbolKind::BOOLEAN:
    return SF::SIMPLE;
  case rq::SymbolKind::UTF8:
    return SF::SIMPLE;
  case rq::SymbolKind::BFLOAT16:
    return SF::SIMPLE | SF::FLOAT;
  case rq::SymbolKind::BINARY16:
    return SF::SIMPLE | SF::FLOAT;
  case rq::SymbolKind::BINARY32:
    return SF::SIMPLE | SF::FLOAT;
  case rq::SymbolKind::BINARY64:
    return SF::SIMPLE | SF::FLOAT;
  case rq::SymbolKind::BINARY128:
    return SF::SIMPLE | SF::FLOAT;

  // BUILTIN DEPTH
  case rq::SymbolKind::WORD:
    return SF::DEPTHED;
  case rq::SymbolKind::SIGNED:
    return SF::DEPTHED;
  case rq::SymbolKind::UNSIGNED:
    return SF::DEPTHED;

  // SUBTYPE
  case rq::SymbolKind::RANGE:
    return SF::SUBTYPE;
  case rq::SymbolKind::REFERENCE:
    return SF::SUBTYPE;
  case rq::SymbolKind::POINTER:
    return SF::SUBTYPE;
  case rq::SymbolKind::FAT_POINTER:
    return SF::SUBTYPE;
  case rq::SymbolKind::ARRAY:
    return SF::SUBTYPE;
  case rq::SymbolKind::TWO_PART_SEQUENCE:
    return SF::SUBTYPE;
  case rq::SymbolKind::THREE_PART_SEQUENCE:
    return SF::SUBTYPE;

  // EVALUATED
  case rq::SymbolKind::MODULE:
    return SF::NONE;
  case rq::SymbolKind::TOP_SCOPE:
    return SF::SCOPE;
  case rq::SymbolKind::TABLE:
    return SF::SCOPE;
  case rq::SymbolKind::SCOPE:
    return SF::SCOPE;
  case rq::SymbolKind::OBJECT:
    return SF::NONE | SF::SCOPE;
  case rq::SymbolKind::ENUMERATION:
    return SF::SCOPE;
  case rq::SymbolKind::ENUMERATOR:
    return SF::NONE;
  case rq::SymbolKind::LAYOUT:
    return SF::NONE;
  case rq::SymbolKind::SIGNATURE:
    return SF::NONE;
  case rq::SymbolKind::VARIABLE:
    return SF::NONE;
  case rq::SymbolKind::ENTRY_POINT:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::FUNCTION:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::METHOD:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::EXTENSION_FUNCTION:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::EXTENSION_METHOD:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::CONSTRUCTOR:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::DESTRUCTOR:
    return SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::RANGER:
    return SF::PROCEDURE | SF::SCOPE;

  // TEMPLATE
  case rq::SymbolKind::TEMPLATE_OBJECT:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_ENUMERATION:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_VARIABLE:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_FUNCTION:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_METHOD:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_EXTENSION_FUNCTION:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_EXTENSION_METHOD:
    return SF::TEMPLATE;
  case rq::SymbolKind::TEMPLATE_CONSTRUCTOR:
    return SF::TEMPLATE;

  // PARTIAL SPECIALIZATION
  case rq::SymbolKind::PARTIAL_OBJECT:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_ENUMERATION:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_VARIABLE:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_METHOD:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_FUNCTION:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_EXTENSION_FUNCTION:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_EXTENSION_METHOD:
    return SF::PARTIAL_SPECIALIZATION;
  case rq::SymbolKind::PARTIAL_CONSTRUCTOR:
    return SF::PARTIAL_SPECIALIZATION;

  // FULL SPECIALIZATION
  case rq::SymbolKind::FULL_OBJECT:
    return SF::FULL_SPECIALIZTION | SF::SCOPE;
  case rq::SymbolKind::FULL_ENUMERATION:
    return SF::FULL_SPECIALIZTION | SF::SCOPE;
  case rq::SymbolKind::FULL_VARIABLE:
    return SF::FULL_SPECIALIZTION;
  case rq::SymbolKind::FULL_FUNCTION:
    return SF::FULL_SPECIALIZTION | SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::FULL_METHOD:
    return SF::FULL_SPECIALIZTION | SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::FULL_EXTENSION_FUNCTION:
    return SF::FULL_SPECIALIZTION | SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::FULL_EXTENSION_METHOD:
    return SF::FULL_SPECIALIZTION | SF::PROCEDURE | SF::SCOPE;
  case rq::SymbolKind::FULL_CONSTRUCTOR:
    return SF::FULL_SPECIALIZTION | SF::PROCEDURE | SF::SCOPE;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::SymbolKind kind) {
  return kind == rq::SymbolKind::TYPE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::ROOT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimple(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SIMPLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDepthed(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::DEPTHED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSpecialization(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PARTIAL_SPECIALIZATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFullSpecialization(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::FULL_SPECIALIZTION);
}

struct TypeSymbol;
struct SimpleSymbol;
struct DepthedSymbol;

struct VoidSymbol;
struct NullSymbol;
struct NoReturnSymbol;
struct VariadicArgumentsSymbol;
struct BooleanSymbol;
struct Utf8Symbol;
struct Bfloat16Symbol;
struct Binary16Symbol;
struct Binary32Symbol;
struct Binary64Symbol;
struct Binary128Symbol;
struct WordSymbol;
struct UnsignedSymbol;
struct SignedSymbol;

struct ContextCache {
  using Self = ContextCache;

  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs{};
  rq::VoidSymbol *_void_symbol{nullptr};
  rq::NullSymbol *_null_symbol{nullptr};
  rq::NoReturnSymbol *_no_return_symbol{nullptr};
  rq::VariadicArgumentsSymbol *_variadic_arguments_symbol{nullptr};
  rq::BooleanSymbol *_boolean_symbol{nullptr};
  rq::Utf8Symbol *_utf8_symbol{nullptr};
  rq::Bfloat16Symbol *_bfloat16_symbol{nullptr};
  rq::Binary16Symbol *_binary16_symbol{nullptr};
  rq::Binary32Symbol *_binary32_symbol{nullptr};
  rq::Binary64Symbol *_binary64_symbol{nullptr};
  rq::Binary128Symbol *_binary128_symbol{nullptr};
  llvm::FoldingSet<rq::DepthedSymbol> _depthed_symbols{};

  ContextCache() = default;
  ContextCache(const Self &) = delete;
  ContextCache(Self &&) = delete;
  ~ContextCache() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] TypeParam &allocateValue(ArgNParam &&...arg_n);
  [[nodiscard]] llvm::StringRef saveString(llvm::Twine twine);
  [[nodiscard]] rq::Expression &acquireExpression();
  void discardExpression(rq::Expression &expression);
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] rq::TypeSymbol &getTypeSymbol();
  [[nodiscard]] rq::VoidSymbol &getVoidSymbol();
  [[nodiscard]] rq::NullSymbol &getNullSymbol();
  [[nodiscard]] rq::NoReturnSymbol &getNoReturnSymbol();
  [[nodiscard]] rq::VariadicArgumentsSymbol &getVariadicArgumentsSymbol();
  [[nodiscard]] rq::BooleanSymbol &getBooleanSymbol();
  [[nodiscard]] rq::Utf8Symbol &getUtf8Symbol();
  [[nodiscard]] rq::Bfloat16Symbol &getBfloat16Symbol();
  [[nodiscard]] rq::Binary16Symbol &getBinary16Symbol();
  [[nodiscard]] rq::Binary32Symbol &getBinary32Symbol();
  [[nodiscard]] rq::Binary64Symbol &getBinary64Symbol();
  [[nodiscard]] rq::Binary128Symbol &getBinary128Symbol();
  [[nodiscard]] rq::WordSymbol &getWordSymbol(unsigned bit_depth);
  [[nodiscard]] rq::UnsignedSymbol &getUnsignedSymbol(unsigned bit_depth);
  [[nodiscard]] rq::SignedSymbol &getSignedSymbol(unsigned bit_depth);
  [[nodiscard]] rq::DepthedSymbol &
  _getOrInsertBuiltinDepthSymbol(rq::SymbolKind kind, unsigned depth);
};

struct Symbol {
  using Self = rq::Symbol;

  rq::SymbolKind _kind;

  Symbol(rq::SymbolKind kind) : _kind(kind) {}
  Symbol(const Self &) = delete;
  Symbol(Self &&) = delete;
  virtual ~Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolKind getKind() const {
    return this->_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return rq::getIsType(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot() const {
    return rq::getIsRoot(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const {
    return rq::getIsSubtype(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimple() const {
    return rq::getIsSimple(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope() const {
    return rq::getIsScope(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure() const {
    return rq::getIsProcedure(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate() const {
    return rq::getIsTemplate(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSpecialization() const {
    return rq::getIsPartialSpecialization(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFullSpecialization() const {
    return rq::getIsFullSpecialization(this->_kind);
  }
};

} // namespace rq
namespace llvm {

template <> struct isa_impl<rq::SimpleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return rq::getIsSimple(val.getKind());
  }
};

template <> struct isa_impl<rq::DepthedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return rq::getIsDepthed(val.getKind());
  }
};

template <> struct isa_impl<rq::VoidSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::VOID;
  }
};

template <> struct isa_impl<rq::NullSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::NULL_;
  }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::NO_RETURN;
  }
};

template <> struct isa_impl<rq::VariadicArgumentsSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::VARIADIC_ARGUMENTS;
  }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BOOLEAN;
  }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::UTF8;
  }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BFLOAT16;
  }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BINARY16;
  }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BINARY32;
  }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BINARY64;
  }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::BINARY128;
  }
};

template <> struct isa_impl<rq::WordSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::WORD;
  }
};

template <> struct isa_impl<rq::SignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::SIGNED;
  }
};

template <> struct isa_impl<rq::UnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::UNSIGNED;
  }
};

template <> struct isa_impl<rq::Scope, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    auto kind = val.getKind();
    return kind >= rq::SymbolKind::TOP_SCOPE && kind <= rq::SymbolKind::LAYOUT;
  }
};

template <> struct isa_impl<rq::Procedure, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    auto kind = val.getKind();
    return kind == rq::SymbolKind::FUNCTION || kind == rq::SymbolKind::METHOD ||
           kind == rq::SymbolKind::ENTRY_POINT;
  }
};

template <> struct isa_impl<rq::Module, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getKind() == rq::SymbolKind::MODULE;
  }
};

} // namespace llvm
namespace rq {

struct TypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::TypeSymbol;

  rq::Symbol *_root_ptr{nullptr};
  rq::TypeFlags _flags{};

  TypeSymbol(rq::SymbolKind kind, rq::Symbol &root, rq::TypeFlags flags)
      : rq::Symbol(kind), _root_ptr(&root), _flags(flags) {
  }
  TypeSymbol(const Self &) = delete;
  TypeSymbol(Self &&) = delete;
  virtual ~TypeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAttribute(rq::TypeAttribute attribute) const {
    return rq::getHasAttribute(this->_flags, attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMutable() const {
    return rq::getIsMutable(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant() const {
    return rq::getIsConstant(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartiallyMutable() const {
    return rq::getIsPartiallyMutable(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutability() const {
    return rq::getHasMutability(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVolatile() const {
    return rq::getIsVolatile(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAtomic() const {
    return rq::getIsAtomic(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullTerminated() const {
    return rq::getIsNullTerminated(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMayDiscard() const {
    return rq::getIsMayDiscard(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDebugTrapOnPanic() const {
    return rq::getIsDebugTrapOnPanic(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicCaptureLayout() const {
    return rq::getIsDynamicCaptureLayout(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutabilityClassFlags getMutabilityClassFlags() const {
    return rq::getMutabilityClassFlags(this->_flags);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
    id.AddPointer(this->_root_ptr);
    id.AddInteger(static_cast<unsigned>(this->_flags));
  }
};

struct SimpleSymbol : public rq::Symbol {
  using Self = rq::SimpleSymbol;

  SimpleSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {
    RQ_ASSERT(rq::getIsSimple(kind), "kind not builtin simple");
  }
  SimpleSymbol(const Self &) = delete;
  SimpleSymbol(Self &&) = delete;
  virtual ~SimpleSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DepthedSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::DepthedSymbol;

  unsigned _bit_depth{};

  DepthedSymbol(rq::SymbolKind kind, unsigned bit_depth)
      : rq::Symbol(kind), _bit_depth(bit_depth) {
    RQ_ASSERT(rq::getIsDepthed(kind), "kind not builtin depthed");
  }
  DepthedSymbol(const Self &) = delete;
  DepthedSymbol(Self &&) = delete;
  virtual ~DepthedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getBitDepth() const {
    return this->_bit_depth;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
    id.AddInteger(this->_bit_depth);
  }
};

struct VoidSymbol : public rq::SimpleSymbol {
  using Self = rq::VoidSymbol;

  VoidSymbol() : rq::SimpleSymbol(rq::SymbolKind::VOID) {}
  VoidSymbol(const Self &) = delete;
  VoidSymbol(Self &&) = delete;
  virtual ~VoidSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NullSymbol : public rq::SimpleSymbol {
  using Self = rq::NullSymbol;

  NullSymbol() : rq::SimpleSymbol(rq::SymbolKind::NULL_) {}
  NullSymbol(const Self &) = delete;
  NullSymbol(Self &&) = delete;
  virtual ~NullSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NoReturnSymbol : public rq::SimpleSymbol {
  using Self = rq::NoReturnSymbol;

  NoReturnSymbol() : rq::SimpleSymbol(rq::SymbolKind::NO_RETURN) {}
  NoReturnSymbol(const Self &) = delete;
  NoReturnSymbol(Self &&) = delete;
  virtual ~NoReturnSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VariadicArgumentsSymbol : public rq::SimpleSymbol {
  using Self = rq::VariadicArgumentsSymbol;

  VariadicArgumentsSymbol()
      : rq::SimpleSymbol(rq::SymbolKind::VARIADIC_ARGUMENTS) {}
  VariadicArgumentsSymbol(const Self &) = delete;
  VariadicArgumentsSymbol(Self &&) = delete;
  virtual ~VariadicArgumentsSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BooleanSymbol : public rq::SimpleSymbol {
  using Self = rq::BooleanSymbol;

  BooleanSymbol() : rq::SimpleSymbol(rq::SymbolKind::BOOLEAN) {}
  BooleanSymbol(const Self &) = delete;
  BooleanSymbol(Self &&) = delete;
  virtual ~BooleanSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Utf8Symbol : public rq::SimpleSymbol {
  using Self = rq::Utf8Symbol;

  Utf8Symbol() : rq::SimpleSymbol(rq::SymbolKind::UTF8) {}
  Utf8Symbol(const Self &) = delete;
  Utf8Symbol(Self &&) = delete;
  virtual ~Utf8Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Bfloat16Symbol : public rq::SimpleSymbol {
  using Self = rq::Bfloat16Symbol;

  Bfloat16Symbol() : rq::SimpleSymbol(rq::SymbolKind::BFLOAT16) {}
  Bfloat16Symbol(const Self &) = delete;
  Bfloat16Symbol(Self &&) = delete;
  virtual ~Bfloat16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary16Symbol : public rq::SimpleSymbol {
  using Self = rq::Binary16Symbol;

  Binary16Symbol() : rq::SimpleSymbol(rq::SymbolKind::BINARY16) {}
  Binary16Symbol(const Self &) = delete;
  Binary16Symbol(Self &&) = delete;
  virtual ~Binary16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary32Symbol : public rq::SimpleSymbol {
  using Self = rq::Binary32Symbol;

  Binary32Symbol() : rq::SimpleSymbol(rq::SymbolKind::BINARY32) {}
  Binary32Symbol(const Self &) = delete;
  Binary32Symbol(Self &&) = delete;
  virtual ~Binary32Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary64Symbol : public rq::SimpleSymbol {
  using Self = rq::Binary64Symbol;

  Binary64Symbol() : rq::SimpleSymbol(rq::SymbolKind::BINARY64) {}
  Binary64Symbol(const Self &) = delete;
  Binary64Symbol(Self &&) = delete;
  virtual ~Binary64Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary128Symbol : public rq::SimpleSymbol {
  using Self = rq::Binary128Symbol;

  Binary128Symbol() : rq::SimpleSymbol(rq::SymbolKind::BINARY128) {}
  Binary128Symbol(const Self &) = delete;
  Binary128Symbol(Self &&) = delete;
  virtual ~Binary128Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct WordSymbol : public rq::DepthedSymbol {
  using Self = rq::WordSymbol;

  WordSymbol(unsigned bit_depth)
      : rq::DepthedSymbol(rq::SymbolKind::WORD, bit_depth) {}
  WordSymbol(const Self &) = delete;
  WordSymbol(Self &&) = delete;
  virtual ~WordSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct UnsignedSymbol : public rq::DepthedSymbol {
  using Self = rq::UnsignedSymbol;

  UnsignedSymbol(unsigned bit_depth)
      : rq::DepthedSymbol(rq::SymbolKind::UNSIGNED, bit_depth) {}
  UnsignedSymbol(const Self &) = delete;
  UnsignedSymbol(Self &&) = delete;
  virtual ~UnsignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignedSymbol : public rq::DepthedSymbol {
  using Self = rq::SignedSymbol;

  SignedSymbol(unsigned bit_depth)
      : rq::DepthedSymbol(rq::SymbolKind::SIGNED, bit_depth) {}
  SignedSymbol(const Self &) = delete;
  SignedSymbol(Self &&) = delete;
  virtual ~SignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SymbolNode;
struct SymbolEntry;
struct SymbolEntryIterator;
struct ConstSymbolEntryIterator;

struct SymbolEntry final {
  using Self = rq::SymbolEntry;

  llvm::PointerUnion<rq::Symbol *, rq::SymbolNode *> _ptr_union{nullptr};

  RQ_ALWAYS_INLINE SymbolEntry() = default;
  RQ_ALWAYS_INLINE SymbolEntry(rq::Symbol &symbol) : _ptr_union(&symbol) {}
  RQ_ALWAYS_INLINE SymbolEntry(rq::SymbolNode &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~SymbolEntry() = default;
  RQ_ALWAYS_INLINE SymbolEntry(const Self &) = default;
  RQ_ALWAYS_INLINE SymbolEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return llvm::isa<rq::Symbol *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolNode() const {
    return llvm::isa<rq::SymbolNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(llvm::cast<rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(llvm::cast<rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolNode &getSymbolNode() {
    return rq::dereferencePtr(llvm::cast<rq::SymbolNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolNode &getSymbolNode() const {
    return rq::dereferencePtr(llvm::cast<rq::SymbolNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolEntryIterator begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolEntryIterator end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator cend() const;
};

struct ConstSymbolEntry final {
  using Self = rq::ConstSymbolEntry;

  llvm::PointerUnion<const rq::Symbol *, const rq::SymbolNode *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE ConstSymbolEntry() = default;
  RQ_ALWAYS_INLINE ConstSymbolEntry(const rq::SymbolEntry &rhs)
      : _ptr_union(
            std::bit_cast<
                llvm::PointerUnion<const rq::Symbol *, const rq::SymbolNode *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstSymbolEntry(rq::SymbolEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::SymbolNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstSymbolEntry(const rq::Symbol &symbol)
      : _ptr_union(&symbol) {}
  RQ_ALWAYS_INLINE ConstSymbolEntry(const rq::SymbolNode &node)
      : _ptr_union(&node) {}
  ~ConstSymbolEntry() = default;
  RQ_ALWAYS_INLINE ConstSymbolEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ConstSymbolEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::SymbolEntry &rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::SymbolNode *>>(
        rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::SymbolEntry &&rhs) {
    this->_ptr_union = std::bit_cast<
        llvm::PointerUnion<const rq::Symbol *, const rq::SymbolNode *>>(
        rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return llvm::isa<const rq::Symbol *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolNode() const {
    return llvm::isa<const rq::SymbolNode *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(llvm::cast<const rq::Symbol *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolNode &getSymbolNode() const {
    return rq::dereferencePtr(
        llvm::cast<const rq::SymbolNode *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstSymbolEntryIterator cend() const;
};

struct SymbolNode final {
  using Self = rq::SymbolNode;

  rq::Symbol *_symbol_ptr{nullptr};
  rq::SymbolEntry _scope_entry{};

  RQ_ALWAYS_INLINE SymbolNode() = default;
  RQ_ALWAYS_INLINE SymbolNode(rq::Symbol &symbol_a, rq::Symbol &symbol_b)
      : _symbol_ptr(&symbol_a), _scope_entry(symbol_b) {}
  RQ_ALWAYS_INLINE SymbolNode(rq::Symbol &symbol, rq::SymbolNode &node)
      : _symbol_ptr(&symbol), _scope_entry(node) {}
  RQ_ALWAYS_INLINE SymbolNode(rq::Symbol &symbol, const rq::SymbolEntry &entry)
      : _symbol_ptr(&symbol), _scope_entry(entry) {}
  SymbolNode(const Self &) = delete;
  SymbolNode(Self &&) = delete;
  RQ_ALWAYS_INLINE ~SymbolNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSymbol() const {
    return this->_symbol_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSymbolEntry() const {
    return !this->_scope_entry.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolEntry &getSymbolEntry() {
    return this->_scope_entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolEntry &getSymbolEntry() const {
    return this->_scope_entry;
  }
};

struct SymbolEntryIterator final {
  using Self = rq::SymbolEntryIterator;
  using value_type = rq::Symbol;
  using reference = rq::Symbol &;
  using pointer = rq::Symbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolEntry _entry;

  RQ_ALWAYS_INLINE SymbolEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit SymbolEntryIterator(rq::SymbolEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsSymbol()) {
      this->_entry = rq::SymbolEntry();
    } else if (this->_entry.getIsSymbolNode()) {
      this->_entry = rq::SymbolEntry(this->_entry.getSymbolNode());
    } else {
      RQ_UNREACHABLE();
    }
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &operator*() {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &operator*() const {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol *operator->() {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return &this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *operator->() const {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return &this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

struct ConstSymbolEntryIterator final {
  using Self = rq::ConstSymbolEntryIterator;
  using value_type = const rq::Symbol;
  using reference = const rq::Symbol &;
  using pointer = rq::Symbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ConstSymbolEntry _entry;

  RQ_ALWAYS_INLINE ConstSymbolEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstSymbolEntryIterator(const rq::SymbolEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE explicit ConstSymbolEntryIterator(
      const rq::ConstSymbolEntry &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsSymbol()) {
      this->_entry = rq::ConstSymbolEntry();
    } else if (this->_entry.getIsSymbolNode()) {
      this->_entry = rq::ConstSymbolEntry(this->_entry.getSymbolNode());
    } else {
      RQ_UNREACHABLE();
    }
    return *this;
  }
  RQ_ALWAYS_INLINE Self operator++(int) {
    Self backup = *this;
    ++(*this);
    return backup;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_entry == it._entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_entry != it._entry;
    ;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &operator*() const {
    if (this->_entry.getIsSymbol()) {
      return this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol *operator->() const {
    if (this->_entry.getIsSymbol()) {
      return &this->_entry.getSymbol();
    } else if (this->_entry.getIsSymbolNode()) {
      return &this->_entry.getSymbolNode().getSymbol();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

rq::SymbolEntryIterator SymbolEntry::begin() {
  return rq::SymbolEntryIterator(*this);
}

rq::SymbolEntryIterator SymbolEntry::end() { return rq::SymbolEntryIterator(); }

rq::ConstSymbolEntryIterator SymbolEntry::begin() const {
  return rq::ConstSymbolEntryIterator(*this);
}

rq::ConstSymbolEntryIterator SymbolEntry::end() const {
  return rq::ConstSymbolEntryIterator();
}

rq::ConstSymbolEntryIterator SymbolEntry::cbegin() const {
  return rq::ConstSymbolEntryIterator(*this);
}

rq::ConstSymbolEntryIterator SymbolEntry::cend() const {
  return rq::ConstSymbolEntryIterator();
}

rq::ConstSymbolEntryIterator ConstSymbolEntry::begin() const {
  return rq::ConstSymbolEntryIterator(*this);
}

rq::ConstSymbolEntryIterator ConstSymbolEntry::end() const {
  return rq::ConstSymbolEntryIterator();
}

rq::ConstSymbolEntryIterator ConstSymbolEntry::cbegin() const {
  return rq::ConstSymbolEntryIterator(*this);
}

rq::ConstSymbolEntryIterator ConstSymbolEntry::cend() const {
  return rq::ConstSymbolEntryIterator();
}

struct Scope : rq::Symbol {
  using Self = rq::Scope;

  llvm::SmallDenseMap<llvm::StringRef, rq::SymbolEntry> _named_values{};
  rq::SymbolEntry _unamed_values{};

  Scope() : rq::Symbol(rq::SymbolKind::SCOPE) {}
  Scope(rq::SymbolKind kind) : rq::Symbol(kind) {}
  Scope(const Self &) = delete;
  Scope(Self &&) = delete;
  ~Scope() override {
    // TODO call destructors of all contained terms
  }
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  void inline tabulateNamedSymbol(rq::ContextCache &cache, llvm::StringRef name,
                                  rq::Symbol &symbol) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      rq::SymbolEntry &entry = it->second;
      rq::SymbolNode &node = cache.allocateValue<rq::SymbolNode>(symbol, entry);
      entry = rq::SymbolEntry(node);
    } else {
      this->_named_values.insert({name, rq::SymbolEntry(symbol)});
    }
  }
  inline void tabulateUnamedSymbol(rq::ContextCache &cache,
                                   rq::Symbol &symbol) {
    rq::SymbolEntry &entry = this->_unamed_values;
    if (entry.getIsEmpty()) {
      entry = symbol;
      return;
    }
    rq::SymbolNode &node = cache.allocateValue<rq::SymbolNode>(symbol, entry);
    entry = rq::SymbolEntry(node);
  }
  [[nodiscard]] inline rq::SymbolEntry getNamedEntry(llvm::StringRef name) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::SymbolEntry();
  }
  [[nodiscard]] inline rq::ConstSymbolEntry
  getNamedEntry(llvm::StringRef name) const {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::ConstSymbolEntry();
  }
  [[nodiscard]] inline rq::SymbolEntry getUnamedEntry() {
    return this->_unamed_values;
  }
  [[nodiscard]] inline rq::ConstSymbolEntry getUnamedEntry() const {
    return this->_unamed_values;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedEntryRange() {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedEntryRange() const {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
};

struct Layout final : public rq::Scope {
  using Self = rq::Layout;

  Layout() : rq::Scope(rq::SymbolKind::LAYOUT) {}
};

struct Procedure final : public rq::Scope {
  using Self = rq::Procedure;

  rq::Module *_module_ptr{nullptr};
  const rq::Expression *_expression_ptr{nullptr};

  Procedure(rq::SymbolKind kind, rq::Module &module)
      : rq::Scope(kind), _module_ptr(&module) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Module &getModule() {
    return rq::dereferencePtr(this->_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Module &getModule() const {
    return rq::dereferencePtr(this->_module_ptr);
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

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  Module(rq::ModuleKind kind, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : rq::Symbol(rq::SymbolKind::MODULE), _module_kind(kind),
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

inline const llvm::fltSemantics &getLlvmFloatSemantics(rq::SymbolKind kind) {
  using namespace rq;
  switch (kind) {
  case rq::SymbolKind::BFLOAT16:
    return llvm::APFloat::BFloat();
  case rq::SymbolKind::BINARY16:
    return llvm::APFloat::IEEEhalf();
  case rq::SymbolKind::BINARY32:
    return llvm::APFloat::IEEEsingle();
  case rq::SymbolKind::BINARY64:
    return llvm::APFloat::IEEEdouble();
  case rq::SymbolKind::BINARY128:
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
                rq::SymbolKind semantics) {
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

template <typename TypeParam, typename... ArgNParam>
inline TypeParam &rq::ContextCache::allocateValue(ArgNParam &&...arg_n) {
  TypeParam *ptr = this->_llvm_arena.Allocate<TypeParam>(1);
  ptr = new (ptr) TypeParam(std::forward<ArgNParam>(arg_n)...);
  return rq::dereferencePtr(ptr);
}

inline llvm::StringRef rq::ContextCache::saveString(llvm::Twine twine) {
  return this->_llvm_string_saver.save(twine);
}

inline void rq::ContextCache::discardExpression(rq::Expression &expression) {
  this->_unused_expression_ptrs.emplace_back(&expression);
}

inline rq::VoidSymbol &rq::ContextCache::getVoidSymbol() {
  if (!this->_void_symbol) {
    this->_void_symbol = &this->allocateValue<rq::VoidSymbol>();
  }
  return rq::dereferencePtr(this->_void_symbol);
}

inline rq::NullSymbol &rq::ContextCache::getNullSymbol() {
  if (!this->_null_symbol) {
    this->_null_symbol = &this->allocateValue<rq::NullSymbol>();
  }
  return rq::dereferencePtr(this->_null_symbol);
}

inline rq::NoReturnSymbol &rq::ContextCache::getNoReturnSymbol() {
  if (!this->_no_return_symbol) {
    this->_no_return_symbol = &this->allocateValue<rq::NoReturnSymbol>();
  }
  return rq::dereferencePtr(this->_no_return_symbol);
}

inline rq::VariadicArgumentsSymbol &
rq::ContextCache::getVariadicArgumentsSymbol() {
  if (!this->_variadic_arguments_symbol) {
    this->_variadic_arguments_symbol =
        &this->allocateValue<rq::VariadicArgumentsSymbol>();
  }
  return rq::dereferencePtr(this->_variadic_arguments_symbol);
}

inline rq::BooleanSymbol &rq::ContextCache::getBooleanSymbol() {
  if (!this->_boolean_symbol) {
    this->_boolean_symbol = &this->allocateValue<rq::BooleanSymbol>();
  }
  return rq::dereferencePtr(this->_boolean_symbol);
}

inline rq::Utf8Symbol &rq::ContextCache::getUtf8Symbol() {
  if (!this->_utf8_symbol) {
    this->_utf8_symbol = &this->allocateValue<rq::Utf8Symbol>();
  }
  return rq::dereferencePtr(this->_utf8_symbol);
}

inline rq::Bfloat16Symbol &rq::ContextCache::getBfloat16Symbol() {
  if (!this->_bfloat16_symbol) {
    this->_bfloat16_symbol = &this->allocateValue<rq::Bfloat16Symbol>();
  }
  return rq::dereferencePtr(this->_bfloat16_symbol);
}

inline rq::Binary16Symbol &rq::ContextCache::getBinary16Symbol() {
  if (!this->_binary16_symbol) {
    this->_binary16_symbol = &this->allocateValue<rq::Binary16Symbol>();
  }
  return rq::dereferencePtr(this->_binary16_symbol);
}

inline rq::Binary32Symbol &rq::ContextCache::getBinary32Symbol() {
  if (!this->_binary32_symbol) {
    this->_binary32_symbol = &this->allocateValue<rq::Binary32Symbol>();
  }
  return rq::dereferencePtr(this->_binary32_symbol);
}

inline rq::Binary64Symbol &rq::ContextCache::getBinary64Symbol() {
  if (!this->_binary64_symbol) {
    this->_binary64_symbol = &this->allocateValue<rq::Binary64Symbol>();
  }
  return rq::dereferencePtr(this->_binary64_symbol);
}

inline rq::Binary128Symbol &rq::ContextCache::getBinary128Symbol() {
  if (!this->_binary128_symbol) {
    this->_binary128_symbol = &this->allocateValue<rq::Binary128Symbol>();
  }
  return rq::dereferencePtr(this->_binary128_symbol);
}

inline rq::DepthedSymbol &
rq::ContextCache::_getOrInsertBuiltinDepthSymbol(rq::SymbolKind kind,
                                                 unsigned parameter) {
  llvm::FoldingSetNodeID id;
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(parameter);
  void *insert_pos = nullptr;
  if (rq::DepthedSymbol *existing =
          this->_depthed_symbols.FindNodeOrInsertPos(id, insert_pos)) {
    return rq::dereferencePtr(existing);
  }
  rq::DepthedSymbol &new_type =
      this->allocateValue<rq::DepthedSymbol>(kind, parameter);
  this->_depthed_symbols.InsertNode(&new_type, insert_pos);
  return new_type;
}

inline rq::WordSymbol &rq::ContextCache::getWordSymbol(unsigned bit_depth) {
  return static_cast<rq::WordSymbol &>(
      this->_getOrInsertBuiltinDepthSymbol(rq::SymbolKind::WORD, bit_depth));
}

inline rq::UnsignedSymbol &
rq::ContextCache::getUnsignedSymbol(unsigned bit_depth) {
  return static_cast<rq::UnsignedSymbol &>(
      this->_getOrInsertBuiltinDepthSymbol(rq::SymbolKind::UNSIGNED, bit_depth));
}

inline rq::SignedSymbol &rq::ContextCache::getSignedSymbol(unsigned bit_depth) {
  return static_cast<rq::SignedSymbol &>(
      this->_getOrInsertBuiltinDepthSymbol(rq::SymbolKind::SIGNED, bit_depth));
}

} // namespace rq
