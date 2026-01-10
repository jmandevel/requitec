#pragma once

#include <rq/ast.hpp>
#include <rq/codeunits.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/StringSaver.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

enum class SymbolKind : std::uint_fast8_t {
  NONE,

  // ROOT WITH TYPE ATTRIBUTES
  TYPE,

  // SIMPLE BUILTIN
  INFERENCE,
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

  // DEPTHED BUILTIN
  WORD,
  SIGNED,
  UNSIGNED,

  // SIMPLE SUBTYPE
  RANGE,
  REFERENCE,
  POINTER,
  FAT_POINTER,
  INFERENCED_COUNT_ARRAY,

  // COUNTED SUBTYPE
  ARRAY,

  // COMPOSITE SUBTYPE
  LAYOUT,
  SIGNATURE,
  EXTENSION,

  // ARITHMETIC SEQUENCE
  ARITHMETIC_INTERVAL,
  FINITE_ARITHMETIC_PROGRESSION,
  INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  MODULE,
  MUTATION,

  // BINDING
  VARIABLE,
  ENUMERATOR,
  LAYOUT_PROPERTY,
  CLASS_PROPERTY,
  SIGNATURE_PARAMETER,
  TEMPLATE_PARAMETER,

  // SCOPES
  SCOPE,
  TABLE,
  CLASS,
  ENUMERATION,

  // PROCEDURES
  ENTRY,
  FUNCTION,
  METHOD,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  CONSTRUCTOR,
  DESTRUCTOR,
  RANGER,

  // TEMPLATE
  TEMPLATE_CLASS,
  TEMPLATE_ENUMERATION,
  TEMPLATE_VARIABLE,
  TEMPLATE_FUNCTION,
  TEMPLATE_METHOD,
  TEMPLATE_EXTENSION_FUNCTION,
  TEMPLATE_EXTENSION_METHOD,
  TEMPLATE_CONSTRUCTOR,

  // PARTIAL SPECIALIZATION
  PARTIAL_CLASS,
  PARTIAL_ENUMERATION,
  PARTIAL_VARIABLE,
  PARTIAL_FUNCTION,
  PARTIAL_METHOD,
  PARTIAL_EXTENSION_FUNCTION,
  PARTIAL_EXTENSION_METHOD,
  PARTIAL_CONSTRUCTOR
};

[[nodiscard]] inline llvm::StringRef getName(rq::SymbolKind kind) {
  using namespace rq;
  using SY = SymbolKind;
  switch (kind) {
  case SY::NONE:
    return "none";

  // ROOT WITH TYPE ATTRIBUTES
  case SY::TYPE:
    return "type";

  // SIMPLE BUILTIN
  case SY::INFERENCE:
    return "inference";
  case SY::VOID:
    return "void";
  case SY::NULL_:
    return "null";
  case SY::NO_RETURN:
    return "no_return";
  case SY::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case SY::BOOLEAN:
    return "boolean";
  case SY::UTF8:
    return "utf8";
  case SY::BFLOAT16:
    return "bfloat16";
  case SY::BINARY16:
    return "binary16";
  case SY::BINARY32:
    return "binary32";
  case SY::BINARY64:
    return "binary64";
  case SY::BINARY128:
    return "binary128";

  // DEPTHED BUILTIN
  case SY::WORD:
    return "word";
  case SY::SIGNED:
    return "signed";
  case SY::UNSIGNED:
    return "unsigned";

  // SIMPLE SUBTYPE
  case SY::RANGE:
    return "range";
  case SY::REFERENCE:
    return "reference";
  case SY::POINTER:
    return "pointer";
  case SY::FAT_POINTER:
    return "fat_pointer";
  case SY::INFERENCED_COUNT_ARRAY:
    return "inferenced_count_array";

  // COUNTED SUBTYPE
  case SY::ARRAY:
    return "array";

  // COMPOSITE SUBTYPE
  case SY::LAYOUT:
    return "layout";
  case SY::SIGNATURE:
    return "signature";
  case SY::EXTENSION:
    return "extension";

  // ARITHMETIC SEQUENCE
  case SY::ARITHMETIC_INTERVAL:
    return "arithmetic_interval";
  case SY::FINITE_ARITHMETIC_PROGRESSION:
    return "finite_arithmetic_progression";
  case SY::INFINITE_ARITHMETIC_PROGRESSION:
    return "infinite_arithmetic_progression";

  // MISC
  case SY::MODULE:
    return "module";
  case SY::MUTATION:
    return "mutation";

  // BINDING
  case SY::VARIABLE:
    return "variable";
  case SY::ENUMERATOR:
    return "enumerator";
  case SY::LAYOUT_PROPERTY:
    return "layout_property";
  case SY::CLASS_PROPERTY:
    return "class_property";
  case SY::SIGNATURE_PARAMETER:
    return "signature_parameter";
  case SY::TEMPLATE_PARAMETER:
    return "template_parameter";

  // SCOPES
  case SY::SCOPE:
    return "scope";
  case SY::TABLE:
    return "table";
  case SY::CLASS:
    return "class";
  case SY::ENUMERATION:
    return "enumeration";

  // PROCEDURES
  case SY::ENTRY:
    return "entry";
  case SY::FUNCTION:
    return "function";
  case SY::METHOD:
    return "method";
  case SY::EXTENSION_FUNCTION:
    return "extension_function";
  case SY::EXTENSION_METHOD:
    return "extension_method";
  case SY::CONSTRUCTOR:
    return "constructor";
  case SY::DESTRUCTOR:
    return "destructor";
  case SY::RANGER:
    return "ranger";

  // TEMPLATE
  case SY::TEMPLATE_CLASS:
    return "template_class";
  case SY::TEMPLATE_ENUMERATION:
    return "template_enumeration";
  case SY::TEMPLATE_VARIABLE:
    return "template_variable";
  case SY::TEMPLATE_FUNCTION:
    return "template_function";
  case SY::TEMPLATE_METHOD:
    return "template_method";
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return "template_extension_function";
  case SY::TEMPLATE_EXTENSION_METHOD:
    return "template_extension_method";
  case SY::TEMPLATE_CONSTRUCTOR:
    return "template_constructor";

  // PARTIAL SPECIALIZATION
  case SY::PARTIAL_CLASS:
    return "partial_class";
  case SY::PARTIAL_ENUMERATION:
    return "partial_enumeration";
  case SY::PARTIAL_VARIABLE:
    return "partial_variable";
  case SY::PARTIAL_FUNCTION:
    return "partial_function";
  case SY::PARTIAL_METHOD:
    return "partial_method";
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return "partial_extension_function";
  case SY::PARTIAL_EXTENSION_METHOD:
    return "partial_extension_method";
  case SY::PARTIAL_CONSTRUCTOR:
    return "partial_constructor";
  }
  RQ_UNREACHABLE();
}

enum class SymbolFlags : std::uint_fast32_t {
  NONE = 0,
  SIMPLE_BUILTIN = rq::getBit(0),
  DEPTHED_BUILTIN = rq::getBit(1),
  SIMPLE_SUBTYPE = rq::getBit(2),
  COUNTED_SUBTYPE = rq::getBit(3),
  COMPOSITE_SUBTYE = rq::getBit(4),
  ARITHMETIC_SEQUENCE = rq::getBit(5),
  BINDING = rq::getBit(6),
  SCOPED = rq::getBit(7),
  SCOPE = rq::getBit(8),
  PROCEDURE = rq::getBit(9),
  TEMPLATE = rq::getBit(10),
  PARTIAL_SPECIALIZATION = rq::getBit(11),
  HAS_TEMPLATE = rq::getBit(12),
  ROOT = rq::getBit(13),
  INTEGER = rq::getBit(14),
  FLOAT = rq::getBit(15),
  CODEUNIT = rq::getBit(16),
  NAMED = rq::getBit(17)
};

template <> struct is_flags<rq::SymbolFlags> : std::true_type {};

[[nodiscard]] inline rq::SymbolFlags getFlags(rq::SymbolKind kind) {
  using namespace rq;
  using SY = SymbolKind;
  using SYF = SymbolFlags;
  switch (kind) {
  case rq::SymbolKind::NONE:
    return SYF::NONE;

  // ROOT WITH TYPE ATTRIBUTES
  case SY::TYPE:
    return SYF::ROOT;

  // SIMPLE BUILTIN
  case SY::INFERENCE:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::VOID:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::NULL_:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::NO_RETURN:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::VARIADIC_ARGUMENTS:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::BOOLEAN:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT;
  case SY::UTF8:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::CODEUNIT;
  case SY::BFLOAT16:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::FLOAT;
  case SY::BINARY16:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::FLOAT;
  case SY::BINARY32:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::FLOAT;
  case SY::BINARY64:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::FLOAT;
  case SY::BINARY128:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::FLOAT;

  // DEPTHED BUILTIN
  case SY::WORD:
    return SYF::DEPTHED_BUILTIN | SYF::ROOT | SYF::INTEGER;
  case SY::SIGNED:
    return SYF::DEPTHED_BUILTIN | SYF::ROOT | SYF::INTEGER;
  case SY::UNSIGNED:
    return SYF::DEPTHED_BUILTIN | SYF::ROOT | SYF::INTEGER;

  // SIMPLE SUBTYPE
  case SY::RANGE:
    return SYF::SIMPLE_SUBTYPE;
  case SY::REFERENCE:
    return SYF::SIMPLE_SUBTYPE;
  case SY::POINTER:
    return SYF::SIMPLE_SUBTYPE;
  case SY::FAT_POINTER:
    return SYF::SIMPLE_SUBTYPE;
  case SY::INFERENCED_COUNT_ARRAY:
    return SYF::SIMPLE_SUBTYPE;

  // COUNTED SUBTYPE
  case SY::ARRAY:
    return SYF::COUNTED_SUBTYPE;

  // COMPOSITE SUBTYPE
  case SY::LAYOUT:
    return SYF::COMPOSITE_SUBTYE | SYF::SCOPE | SYF::NAMED;
  case SY::SIGNATURE:
    return SYF::COMPOSITE_SUBTYE;
  case SY::EXTENSION:
    return SYF::COMPOSITE_SUBTYE | SYF::SCOPE | SYF::NAMED;

  // ARITHMETIC SEQUENCE
  case SY::ARITHMETIC_INTERVAL:
    return SYF::ARITHMETIC_SEQUENCE;
  case SY::FINITE_ARITHMETIC_PROGRESSION:
    return SYF::ARITHMETIC_SEQUENCE;
  case SY::INFINITE_ARITHMETIC_PROGRESSION:
    return SYF::ARITHMETIC_SEQUENCE;

  // MISC
  case SY::MODULE:
    return SYF::SCOPE | SYF::NAMED;
  case SY::MUTATION:
    return SYF::NAMED;

  // BINDING
  case SY::VARIABLE:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;
  case SY::ENUMERATOR:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;
  case SY::LAYOUT_PROPERTY:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;
  case SY::CLASS_PROPERTY:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;
  case SY::SIGNATURE_PARAMETER:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;
  case SY::TEMPLATE_PARAMETER:
    return SYF::SCOPED | SYF::BINDING | SYF::NAMED;

  // SCOPES
  case SY::SCOPE:
    return SYF::SCOPE;
  case SY::TABLE:
    return SYF::SCOPE | SYF::NAMED;
  case SY::CLASS:
    return SYF::SCOPE | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::ENUMERATION:
    return SYF::SCOPE | SYF::NAMED | SYF::HAS_TEMPLATE;

  // PROCEDURES
  case SY::ENTRY:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED;
  case SY::FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::HAS_TEMPLATE;
  case SY::DESTRUCTOR:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED;
  case SY::RANGER:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED;

  // TEMPLATE
  case SY::TEMPLATE_CLASS:
    return SYF::SCOPE | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_ENUMERATION:
    return SYF::SCOPE | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_VARIABLE:
    return SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;
  case SY::TEMPLATE_CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED | SYF::TEMPLATE;

  // PARTIAL SPECIALIZATION
  case SY::PARTIAL_CLASS:
    return SYF::SCOPE | SYF::NAMED | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_ENUMERATION:
    return SYF::SCOPE | SYF::NAMED | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_VARIABLE:
    return SYF::SCOPED | SYF::NAMED | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::SCOPED | SYF::NAMED |
           SYF::PARTIAL_SPECIALIZATION;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::ROOT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SIMPLE_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDepthedBuiltin(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::DEPTHED_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SIMPLE_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::COUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::COMPOSITE_SUBTYE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequence(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScoped(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SCOPED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SCOPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPartialSpecialization(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::PARTIAL_SPECIALIZATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::SymbolFlags::HAS_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags,
                        rq::SymbolFlags::INTEGER | rq::SymbolFlags::FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::CODEUNIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamed(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::NAMED);
}

[[nodiscard]] inline rq::SymbolKind getTemplate(rq::SymbolKind symbol) {
  using namespace rq;
  using SY = SymbolKind;
  switch (symbol) {
  case SY::CLASS:
    return SY::TEMPLATE_CLASS;
  case SY::ENUMERATION:
    return SY::TEMPLATE_ENUMERATION;
  case SY::VARIABLE:
    return SY::TEMPLATE_VARIABLE;
  case SY::FUNCTION:
    return SY::TEMPLATE_FUNCTION;
  case SY::METHOD:
    return SY::TEMPLATE_METHOD;
  case SY::EXTENSION_FUNCTION:
    return SY::TEMPLATE_EXTENSION_FUNCTION;
  case SY::EXTENSION_METHOD:
    return SY::TEMPLATE_EXTENSION_METHOD;
  case SY::CONSTRUCTOR:
    return SY::TEMPLATE_CONSTRUCTOR;
  case SY::PARTIAL_CLASS:
    return SY::TEMPLATE_CLASS;
  case SY::PARTIAL_ENUMERATION:
    return SY::TEMPLATE_ENUMERATION;
  case SY::PARTIAL_VARIABLE:
    return SY::TEMPLATE_VARIABLE;
  case SY::PARTIAL_FUNCTION:
    return SY::TEMPLATE_FUNCTION;
  case SY::PARTIAL_METHOD:
    return SY::TEMPLATE_METHOD;
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return SY::TEMPLATE_EXTENSION_FUNCTION;
  case SY::PARTIAL_EXTENSION_METHOD:
    return SY::TEMPLATE_EXTENSION_METHOD;
  case SY::PARTIAL_CONSTRUCTOR:
    return SY::TEMPLATE_CONSTRUCTOR;
  case SY::TEMPLATE_CLASS:
    return SY::TEMPLATE_CLASS;
  case SY::TEMPLATE_ENUMERATION:
    return SY::TEMPLATE_ENUMERATION;
  case SY::TEMPLATE_VARIABLE:
    return SY::TEMPLATE_VARIABLE;
  case SY::TEMPLATE_FUNCTION:
    return SY::TEMPLATE_FUNCTION;
  case SY::TEMPLATE_METHOD:
    return SY::TEMPLATE_METHOD;
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return SY::TEMPLATE_EXTENSION_FUNCTION;
  case SY::TEMPLATE_EXTENSION_METHOD:
    return SY::TEMPLATE_EXTENSION_METHOD;
  case SY::TEMPLATE_CONSTRUCTOR:
    return SY::TEMPLATE_CONSTRUCTOR;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::SymbolKind
getPartialSpecialization(rq::SymbolKind symbol) {
  using namespace rq;
  using SY = SymbolKind;
  switch (symbol) {
  case SY::CLASS:
    return SY::PARTIAL_CLASS;
  case SY::ENUMERATION:
    return SY::PARTIAL_ENUMERATION;
  case SY::VARIABLE:
    return SY::PARTIAL_VARIABLE;
  case SY::FUNCTION:
    return SY::PARTIAL_FUNCTION;
  case SY::METHOD:
    return SY::PARTIAL_METHOD;
  case SY::EXTENSION_FUNCTION:
    return SY::PARTIAL_EXTENSION_FUNCTION;
  case SY::EXTENSION_METHOD:
    return SY::PARTIAL_EXTENSION_METHOD;
  case SY::CONSTRUCTOR:
    return SY::PARTIAL_CONSTRUCTOR;
  case SY::TEMPLATE_CLASS:
    return SY::PARTIAL_CLASS;
  case SY::TEMPLATE_ENUMERATION:
    return SY::PARTIAL_ENUMERATION;
  case SY::TEMPLATE_VARIABLE:
    return SY::PARTIAL_VARIABLE;
  case SY::TEMPLATE_FUNCTION:
    return SY::PARTIAL_FUNCTION;
  case SY::TEMPLATE_METHOD:
    return SY::PARTIAL_METHOD;
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return SY::PARTIAL_EXTENSION_FUNCTION;
  case SY::TEMPLATE_EXTENSION_METHOD:
    return SY::PARTIAL_EXTENSION_METHOD;
  case SY::TEMPLATE_CONSTRUCTOR:
    return SY::PARTIAL_CONSTRUCTOR;
  case SY::PARTIAL_CLASS:
    return SY::PARTIAL_CLASS;
  case SY::PARTIAL_ENUMERATION:
    return SY::PARTIAL_ENUMERATION;
  case SY::PARTIAL_VARIABLE:
    return SY::PARTIAL_VARIABLE;
  case SY::PARTIAL_FUNCTION:
    return SY::PARTIAL_FUNCTION;
  case SY::PARTIAL_METHOD:
    return SY::PARTIAL_METHOD;
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return SY::PARTIAL_EXTENSION_FUNCTION;
  case SY::PARTIAL_EXTENSION_METHOD:
    return SY::PARTIAL_EXTENSION_METHOD;
  case SY::PARTIAL_CONSTRUCTOR:
    return SY::PARTIAL_CONSTRUCTOR;
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::SymbolKind
getFullSpecialization(rq::SymbolKind symbol) {
  using namespace rq;
  using SY = SymbolKind;
  switch (symbol) {
  case SY::CLASS:
    return SY::CLASS;
  case SY::ENUMERATION:
    return SY::ENUMERATION;
  case SY::VARIABLE:
    return SY::VARIABLE;
  case SY::FUNCTION:
    return SY::FUNCTION;
  case SY::METHOD:
    return SY::METHOD;
  case SY::EXTENSION_FUNCTION:
    return SY::EXTENSION_FUNCTION;
  case SY::EXTENSION_METHOD:
    return SY::EXTENSION_METHOD;
  case SY::CONSTRUCTOR:
    return SY::CONSTRUCTOR;
  case SY::TEMPLATE_CLASS:
    return SY::CLASS;
  case SY::TEMPLATE_ENUMERATION:
    return SY::ENUMERATION;
  case SY::TEMPLATE_VARIABLE:
    return SY::VARIABLE;
  case SY::TEMPLATE_FUNCTION:
    return SY::FUNCTION;
  case SY::TEMPLATE_METHOD:
    return SY::METHOD;
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return SY::EXTENSION_FUNCTION;
  case SY::TEMPLATE_EXTENSION_METHOD:
    return SY::EXTENSION_METHOD;
  case SY::TEMPLATE_CONSTRUCTOR:
    return SY::CONSTRUCTOR;
  case SY::PARTIAL_CLASS:
    return SY::CLASS;
  case SY::PARTIAL_ENUMERATION:
    return SY::ENUMERATION;
  case SY::PARTIAL_VARIABLE:
    return SY::VARIABLE;
  case SY::PARTIAL_FUNCTION:
    return SY::FUNCTION;
  case SY::PARTIAL_METHOD:
    return SY::METHOD;
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return SY::EXTENSION_FUNCTION;
  case SY::PARTIAL_EXTENSION_METHOD:
    return SY::EXTENSION_METHOD;
  case SY::PARTIAL_CONSTRUCTOR:
    return SY::CONSTRUCTOR;
  default:
    RQ_UNREACHABLE();
  }
}

// ROOT WITH TYPE ATTRIBUTES
struct TypeSymbol;

// SIMPLE BUILTIN
struct InferenceSymbol;
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

// DEPTHED BUILTIN
struct DepthedBuiltinSymbol;
struct WordSymbol;
struct SignedSymbol;
struct UnsignedSymbol;

// SIMPLE SUBTYPE
struct SimpleSubtypeSymbol;
struct RangeSymbol;
struct ReferenceSymbol;
struct PointerSymbol;
struct FatPointerSymbol;
struct InferencedCountArraySymbol;

// COUNTED SUBTYPE
struct ArraySymbol;

// COMPOSITE SUBTYPE
struct LayoutSymbol;
struct SignatureSymbol;
struct ExtensionSymbol;

// ARITHMETIC SEQUENCE
struct ArithmeticSequenceSymbol;
struct ArithmeticIntervalSymbol;
struct FiniteArithmeticProgressionSymbol;
struct InfiniteArithmeticProgressionSymbol;

// MISC
struct ModuleSymbol;
struct MutationSymbol;

// BINDING
struct VariableSymbol;
struct EnumeratorSymbol;
struct LayoutPropertySymbol;
struct ClassPropertySymbol;
struct SignatureParameterSymbol;
struct TemplateParameterSymbol;

// SCOPES
struct ScopeSymbol;
struct TableSymbol;
struct ClassSymbol;
struct EnumerationSymbol;

// PROCEDURES
struct ProcedureSymbol;
struct EntrySymbol;
struct FunctionSymbol;
struct MethodSymbol;
struct ExtensionFunctionSymbol;
struct ExtensionMethodSymbol;
struct ConstructorSymbol;
struct DestructorSymbol;
struct RangerSymbol;

// TEMPLATE
struct TemplateSymbol;
struct TemplateClassSymbol;
struct TemplateEnumerationSymbol;
struct TemplateVariableSymbol;
struct TemplateFunctionSymbol;
struct TemplateMethodSymbol;
struct TemplateExtensionFunctionSymbol;
struct TemplateExtensionMethodSymbol;
struct TemplateConstructorSymbol;

// PARTIAL SPECIALIZATION
struct PartialSpecializationSymbol;
struct PartialClassSymbol;
struct PartialEnumerationSymbol;
struct PartialVariableSymbol;
struct PartialFunctionSymbol;
struct PartialMethodSymbol;
struct PartialExtensionFunctionSymbol;
struct PartialExtensionMethodSymbol;
struct PartialConstructorSymbol;

struct ContextCache {
  using Self = ContextCache;

  llvm::BumpPtrAllocator _llvm_arena{};
  llvm::StringSaver _llvm_string_saver{_llvm_arena};
  std::vector<rq::Expression *> _unused_expression_ptrs{};
  llvm::FoldingSet<rq::TypeSymbol> _type_symbols{};
  rq::InferenceSymbol *_inference_symbol{nullptr};
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
  llvm::FoldingSet<rq::DepthedBuiltinSymbol> _depthed_symbols{};
  llvm::FoldingSet<rq::SimpleSubtypeSymbol> _simple_subtype_symbols{};
  llvm::FoldingSet<rq::ArraySymbol> _array_symbols{};
  llvm::FoldingSet<rq::LayoutSymbol> _layout_symbols{};
  llvm::FoldingSet<rq::SignatureSymbol> _signature_symbols{};
  llvm::FoldingSet<rq::ExtensionSymbol> _extension_symbols{};
  llvm::FoldingSet<rq::ArithmeticSequenceSymbol> _arithmetic_sequence_symbols{};

  ContextCache() = default;
  ContextCache(const Self &) = delete;
  ContextCache(Self &&) = delete;
  ~ContextCache() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  template <typename TypeParam, typename... ArgNParam>
  [[nodiscard]] inline TypeParam &allocateValue(ArgNParam &&...arg_n);
  [[nodiscard]] inline llvm::StringRef saveString(llvm::Twine twine);
  [[nodiscard]] rq::Expression &acquireExpression();
  inline void discardExpression(rq::Expression &expression);
  [[nodiscard]] rq::Expression &copyExpression(rq::Expression &expression);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &getTypeSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferenceSymbol &getInferenceSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VoidSymbol &getVoidSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NullSymbol &getNullSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::NoReturnSymbol &getNoReturnSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::VariadicArgumentsSymbol &
  getVariadicArgumentsSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BooleanSymbol &getBooleanSymbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Utf8Symbol &getUtf8Symbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Bfloat16Symbol &getBfloat16Symbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary16Symbol &getBinary16Symbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary32Symbol &getBinary32Symbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary64Symbol &getBinary64Symbol();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Binary128Symbol &getBinary128Symbol();
  [[nodiscard]] inline rq::DepthedBuiltinSymbol &
  _getOrInsertBuiltinDepthSymbol(rq::SymbolKind kind, unsigned depth);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::WordSymbol &
  getWordSymbol(unsigned bit_depth);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::UnsignedSymbol &
  getUnsignedSymbol(unsigned bit_depth);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignedSymbol &
  getSignedSymbol(unsigned bit_depth);
  [[nodiscard]] inline rq::SimpleSubtypeSymbol &
  _getOrInsertSimpleSubtypeSymbol(rq::SymbolKind, unsigned depth);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::RangeSymbol &
  getRange(rq::TypeSymbol &root);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ReferenceSymbol &
  getReference(rq::TypeSymbol &root);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::PointerSymbol &
  getPointer(rq::TypeSymbol &root);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FatPointerSymbol &
  getFatPointer(rq::TypeSymbol &root);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::InferencedCountArraySymbol &
  getInferecedCountArray(rq::TypeSymbol &root);
  [[nodiscard]] inline rq::ArraySymbol &
  _getOrInsertCountedSubtypeSymbol(rq::SymbolKind kind, rq::TypeSymbol &root,
                                   unsigned count);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArraySymbol &getArray(rq::TypeSymbol &root,
                                                           unsigned count);
  [[nodiscard]] inline rq::ArithmeticSequenceSymbol &
  _getOrInsertArithmeticSequenceSymbol(
      rq::SymbolKind kind, rq::TypeSymbol &root,
      rq::ArithmeticSequenceStep step,
      rq::ArithmeticSequenceCondition condition);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticIntervalSymbol &
  getArithmeticInterval(rq::TypeSymbol &root, rq::ArithmeticSequenceStep step);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &
  getFiniteArithmeticProgression(rq::TypeSymbol &root,
                                 rq::ArithmeticSequenceStep step,
                                 rq::ArithmeticSequenceCondition condition);
  [[nodiscard]] RQ_ALWAYS_INLINE rq::FiniteArithmeticProgressionSymbol &
  getInfiniteArithmeticProgression(rq::TypeSymbol &root,
                                   rq::ArithmeticSequenceStep step);
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

  // ROOT WITH TYPE ATTRIBUTES
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRoot() const {
    return rq::getIsRoot(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return this->_kind == rq::SymbolKind::TYPE;
  }

  // SIMPLE BUILTIN
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin() const {
    return rq::getIsSimpleBuiltin(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInference() const {
    return this->_kind == rq::SymbolKind::INFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVoid() const {
    return this->_kind == rq::SymbolKind::VOID;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNull() const {
    return this->_kind == rq::SymbolKind::NULL_;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoReturn() const {
    return this->_kind == rq::SymbolKind::NO_RETURN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariadicArguments() const {
    return this->_kind == rq::SymbolKind::VARIADIC_ARGUMENTS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBoolean() const {
    return this->_kind == rq::SymbolKind::BOOLEAN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUtf8() const {
    return this->_kind == rq::SymbolKind::UTF8;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloat16() const {
    return this->_kind == rq::SymbolKind::BFLOAT16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary16() const {
    return this->_kind == rq::SymbolKind::BINARY16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary32() const {
    return this->_kind == rq::SymbolKind::BINARY32;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary64() const {
    return this->_kind == rq::SymbolKind::BINARY64;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary128() const {
    return this->_kind == rq::SymbolKind::BINARY128;
  }

  // DEPTHED BUILTIN
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDepthedBuiltin() const {
    return rq::getIsDepthedBuiltin(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsWord() const {
    return this->_kind == rq::SymbolKind::WORD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return this->_kind == rq::SymbolKind::SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return this->_kind == rq::SymbolKind::UNSIGNED;
  }

  // SIMPLE SUBTYPE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleSubtype() const {
    return rq::getIsSimpleSubtype(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRange() const {
    return this->_kind == rq::SymbolKind::RANGE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReference() const {
    return this->_kind == rq::SymbolKind::REFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointer() const {
    return this->_kind == rq::SymbolKind::POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFatPointer() const {
    return this->_kind == rq::SymbolKind::FAT_POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferencedCountArray() const {
    return this->_kind == rq::SymbolKind::INFERENCED_COUNT_ARRAY;
  }

  // COUNTED SUBTYPE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype() const {
    return rq::getIsCountedSubtype(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArray() const {
    return this->_kind == rq::SymbolKind::ARRAY;
  }

  // COMPOSITE SUBTYPE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype() const {
    return rq::getIsCompositeSubtype(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayout() const {
    return this->_kind == rq::SymbolKind::LAYOUT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignature() const {
    return this->_kind == rq::SymbolKind::SIGNATURE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtension() const {
    return this->_kind == rq::SymbolKind::EXTENSION;
  }

  // ARITHMETIC SEQUENCE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence() const {
    return rq::getIsArithmeticSequence(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticInterval() const {
    return this->_kind == rq::SymbolKind::ARITHMETIC_INTERVAL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFiniteArithmeticProgression() const {
    return this->_kind == rq::SymbolKind::FINITE_ARITHMETIC_PROGRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsInfiniteArithmeticProgression() const {
    return this->_kind == rq::SymbolKind::INFINITE_ARITHMETIC_PROGRESSION;
  }

  // MISC
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModule() const {
    return this->_kind == rq::SymbolKind::MODULE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMutation() const {
    return this->_kind == rq::SymbolKind::MUTATION;
  }

  // BINDING
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScoped() const {
    return rq::getIsScoped(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariable() const {
    return this->_kind == rq::SymbolKind::VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumerator() const {
    return this->_kind == rq::SymbolKind::ENUMERATOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProperty() const {
    return this->_kind == rq::SymbolKind::LAYOUT_PROPERTY ||
           this->_kind == rq::SymbolKind::CLASS_PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutProperty() const {
    return this->_kind == rq::SymbolKind::LAYOUT_PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassProperty() const {
    return this->_kind == rq::SymbolKind::CLASS_PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureParameter() const {
    return this->_kind == rq::SymbolKind::SIGNATURE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateParameter() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_PARAMETER;
  }

  // SCOPES
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScope() const {
    return this->_kind == rq::SymbolKind::SCOPE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTable() const {
    return this->_kind == rq::SymbolKind::TABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClass() const {
    return this->_kind == rq::SymbolKind::CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumeration() const {
    return this->_kind == rq::SymbolKind::ENUMERATION;
  }

  // PROCEDURES
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure() const {
    return rq::getIsProcedure(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntry() const {
    return this->_kind == rq::SymbolKind::ENTRY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunction() const {
    return this->_kind == rq::SymbolKind::FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMethod() const {
    return this->_kind == rq::SymbolKind::METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionFunction() const {
    return this->_kind == rq::SymbolKind::EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionMethod() const {
    return this->_kind == rq::SymbolKind::EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstructor() const {
    return this->_kind == rq::SymbolKind::CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDestructor() const {
    return this->_kind == rq::SymbolKind::DESTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger() const {
    return this->_kind == rq::SymbolKind::RANGER;
  }

  // TEMPLATE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate() const {
    return rq::getIsTemplate(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateClass() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateEnumeration() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateVariable() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateFunction() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateMethod() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateExtensionFunction() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateExtensionMethod() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateConstructor() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_CONSTRUCTOR;
  }

  // PARTIAL SPECIALIZATION
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSpecialization() const {
    return rq::getIsPartialSpecialization(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialClass() const {
    return this->_kind == rq::SymbolKind::PARTIAL_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialEnumeration() const {
    return this->_kind == rq::SymbolKind::PARTIAL_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialVariable() const {
    return this->_kind == rq::SymbolKind::PARTIAL_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialFunction() const {
    return this->_kind == rq::SymbolKind::PARTIAL_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialMethod() const {
    return this->_kind == rq::SymbolKind::PARTIAL_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialExtensionFunction() const {
    return this->_kind == rq::SymbolKind::PARTIAL_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialExtensionMethod() const {
    return this->_kind == rq::SymbolKind::PARTIAL_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialConstructor() const {
    return this->_kind == rq::SymbolKind::PARTIAL_CONSTRUCTOR;
  }

  // ADDITIONAL FLAG GETTERS
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const {
    return rq::getHasTemplate(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsInteger(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunit(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamed() const {
    return rq::getIsNamed(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->_kind);
  }
};

} // namespace rq
namespace llvm {

// ROOT WITH TYPE ATTRIBUTES
template <> struct isa_impl<rq::TypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsType(); }
};

// SIMPLE BUILTIN
template <> struct isa_impl<rq::InferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInference();
  }
};

template <> struct isa_impl<rq::VoidSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsVoid(); }
};

template <> struct isa_impl<rq::NullSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsNull(); }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsNoReturn(); }
};

template <> struct isa_impl<rq::VariadicArgumentsSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsVariadicArguments();
  }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBoolean(); }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsUtf8(); }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBfloat16(); }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary16(); }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary32(); }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary64(); }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary128();
  }
};

// DEPTHED BUILTIN
template <> struct isa_impl<rq::DepthedBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDepthedBuiltin();
  }
};

template <> struct isa_impl<rq::WordSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsWord(); }
};

template <> struct isa_impl<rq::SignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsSigned(); }
};

template <> struct isa_impl<rq::UnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsUnsigned(); }
};

// SIMPLE SUBTYPE
template <> struct isa_impl<rq::SimpleSubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSimpleSubtype();
  }
};

template <> struct isa_impl<rq::RangeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsRange(); }
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsReference();
  }
};

template <> struct isa_impl<rq::PointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsPointer(); }
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFatPointer();
  }
};

template <> struct isa_impl<rq::InferencedCountArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInferencedCountArray();
  }
};

// COUNTED SUBTYPE
template <> struct isa_impl<rq::ArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsArray(); }
};

// COMPOSITE SUBTYPE
template <> struct isa_impl<rq::LayoutSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsLayout(); }
};

template <> struct isa_impl<rq::SignatureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSignature();
  }
};

template <> struct isa_impl<rq::ExtensionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtension();
  }
};

// ARITHMETIC SEQUENCE
template <> struct isa_impl<rq::ArithmeticSequenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsArithmeticSequence();
  }
};

template <> struct isa_impl<rq::ArithmeticIntervalSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsArithmeticInterval();
  }
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInfiniteArithmeticProgression();
  }
};

template <> struct isa_impl<rq::FiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFiniteArithmeticProgression();
  }
};

// MISC
template <> struct isa_impl<rq::ModuleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsModule(); }
};

template <> struct isa_impl<rq::MutationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsMutation(); }
};

// BINDING
template <> struct isa_impl<rq::VariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsVariable(); }
};

template <> struct isa_impl<rq::EnumeratorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumerator();
  }
};

template <> struct isa_impl<rq::LayoutPropertySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsLayoutProperty();
  }
};

template <> struct isa_impl<rq::ClassPropertySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsClassProperty();
  }
};

template <> struct isa_impl<rq::SignatureParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSignatureParameter();
  }
};

template <> struct isa_impl<rq::TemplateParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateParameter();
  }
};

// SCOPES
template <> struct isa_impl<rq::ScopeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsScope(); }
};

template <> struct isa_impl<rq::TableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsTable(); }
};

template <> struct isa_impl<rq::ClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsClass(); }
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumeration();
  }
};

// PROCEDURES
template <> struct isa_impl<rq::ProcedureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsProcedure();
  }
};

template <> struct isa_impl<rq::EntrySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEntry();
  }
};

template <> struct isa_impl<rq::FunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsFunction(); }
};

template <> struct isa_impl<rq::MethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsMethod(); }
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionFunction();
  }
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionMethod();
  }
};

template <> struct isa_impl<rq::ConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsConstructor();
  }
};

template <> struct isa_impl<rq::DestructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDestructor();
  }
};

template <> struct isa_impl<rq::RangerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsRanger(); }
};

// TEMPLATE
template <> struct isa_impl<rq::TemplateSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsTemplate(); }
};

template <> struct isa_impl<rq::TemplateClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateClass();
  }
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateEnumeration();
  }
};

template <> struct isa_impl<rq::TemplateVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateVariable();
  }
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateFunction();
  }
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateMethod();
  }
};

template <> struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionFunction();
  }
};

template <> struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionMethod();
  }
};

template <> struct isa_impl<rq::TemplateConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateConstructor();
  }
};

// PARTIAL SPECIALIZATION
template <> struct isa_impl<rq::PartialSpecializationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialSpecialization();
  }
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialClass();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialEnumeration();
  }
};

template <> struct isa_impl<rq::PartialVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialVariable();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialFunction();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialMethod();
  }
};

template <> struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionFunction();
  }
};

template <> struct isa_impl<rq::PartialExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionMethod();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialConstructor();
  }
};

} // namespace llvm
namespace rq {

struct TypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::TypeSymbol;

  rq::Symbol *_root_ptr{nullptr};
  rq::TypeFlags _flags{};

  TypeSymbol(rq::SymbolKind kind, rq::Symbol &root, rq::TypeFlags flags)
      : rq::Symbol(kind), _root_ptr(&root), _flags(flags) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::TypeAttribute attribute) const {
    return rq::getHasAttribute(this->_flags, attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::MutationFlags getMutationFlags() const {
    return rq::getMutationFlags(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMutable(rq::TypeAttribute attribute) const {
    return rq::getHasMutable(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasConstant(rq::TypeAttribute attribute) const {
    return rq::getHasConstant(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasPartiallyMutable(rq::TypeAttribute attribute) const {
    return rq::getHasPartiallyMutable(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasVolatile(rq::TypeAttribute attribute) const {
    return rq::getHasVolatile(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAtomic(rq::TypeAttribute attribute) const {
    return rq::getHasAtomic(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasNullTerminated(rq::TypeAttribute attribute) const {
    return rq::getHasNullTerminated(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasMayDiscard(rq::TypeAttribute attribute) const {
    return rq::getHasMayDiscard(attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasDebugTrapOnPanic(rq::TypeAttribute attribute) const {
    return rq::getHasDebugTrapOnPanic(attribute);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
    id.AddPointer(this->_root_ptr);
    id.AddInteger(static_cast<unsigned>(this->_flags));
  }
};

template <typename EntryElementParam> struct Node;
template <typename EntryElementParam> struct Entry;
template <typename EntryElementParam> struct EntryIterator;
template <typename EntryElementParam> struct ConstEntryIterator;

template <typename EntryElementParam> struct Entry final {
  using EntryElement = EntryElementParam;
  using Self = rq::Entry<EntryElement>;

  llvm::PointerUnion<EntryElement *, rq::Node<EntryElement> *> _ptr_union{
      nullptr};

  RQ_ALWAYS_INLINE Entry() = default;
  RQ_ALWAYS_INLINE Entry(EntryElement &element) : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE Entry(rq::Node<EntryElement> &node) : _ptr_union(&node) {}
  RQ_ALWAYS_INLINE ~Entry() = default;
  RQ_ALWAYS_INLINE Entry(const Self &) = default;
  RQ_ALWAYS_INLINE Entry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntryElement() const {
    return llvm::isa<EntryElement *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<rq::Node<EntryElement> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE EntryElement &getEntryElement() {
    return rq::dereferencePtr(llvm::cast<EntryElement *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement &getEntryElement() const {
    return rq::dereferencePtr(llvm::cast<EntryElement *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Node<EntryElement> &getNode() {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<EntryElement> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<EntryElement> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<rq::Node<EntryElement> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntryIterator<EntryElement> begin();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntryIterator<EntryElement> end();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  cend() const;
};

template <typename EntryElementParam> struct ConstEntry final {
  using EntryElement = EntryElementParam;
  using Self = rq::ConstEntry<EntryElement>;

  llvm::PointerUnion<const EntryElement *, const rq::Node<EntryElement> *>
      _ptr_union{nullptr};

  RQ_ALWAYS_INLINE ConstEntry() = default;
  RQ_ALWAYS_INLINE ConstEntry(const rq::Entry<EntryElement> &rhs)
      : _ptr_union(
            std::bit_cast<llvm::PointerUnion<const EntryElement *,
                                             const rq::Node<EntryElement> *>>(
                rhs._ptr_union)) {}
  RQ_ALWAYS_INLINE ConstEntry(rq::Entry<EntryElement> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const EntryElement *, const rq::Node<EntryElement> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
  }
  RQ_ALWAYS_INLINE ConstEntry(const EntryElement &element)
      : _ptr_union(&element) {}
  RQ_ALWAYS_INLINE ConstEntry(const rq::Node<EntryElement> &node)
      : _ptr_union(&node) {}
  ~ConstEntry() = default;
  RQ_ALWAYS_INLINE ConstEntry(const Self &) = default;
  RQ_ALWAYS_INLINE ConstEntry(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const Self &) = default;
  RQ_ALWAYS_INLINE Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator=(const rq::Entry<EntryElement> &rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const EntryElement *, const rq::Node<EntryElement> *>>(rhs._ptr_union);
    return *this;
  }
  Self RQ_ALWAYS_INLINE &operator=(rq::Entry<EntryElement> &&rhs) {
    this->_ptr_union = std::bit_cast<llvm::PointerUnion<
        const EntryElement *, const rq::Node<EntryElement> *>>(rhs._ptr_union);
    rhs._ptr_union = nullptr;
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntryElement() const {
    return llvm::isa<const EntryElement *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNode() const {
    return llvm::isa<const rq::Node<EntryElement> *>(this->_ptr_union);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEmpty() const {
    return this->_ptr_union.isNull();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement &getEntryElement() const {
    return rq::dereferencePtr(
        llvm::cast<const EntryElement *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Node<EntryElement> &getNode() const {
    return rq::dereferencePtr(
        llvm::cast<const rq::Node<EntryElement> *>(this->_ptr_union));
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this->_ptr_union == rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this->_ptr_union != rhs._ptr_union;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  begin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  end() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  cbegin() const;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstEntryIterator<EntryElement>
  cend() const;
};

template <typename EntryElementParam> struct Node final {
  using EntryElement = EntryElementParam;
  using Self = rq::Node<EntryElement>;

  EntryElement *_element_ptr{nullptr};
  rq::Entry<EntryElement> _scope_entry{};

  RQ_ALWAYS_INLINE Node() = default;
  RQ_ALWAYS_INLINE Node(EntryElement &element_a, EntryElement &element_b)
      : _element_ptr(&element_a), _scope_entry(element_b) {}
  RQ_ALWAYS_INLINE Node(EntryElement &element, rq::Node<EntryElement> &node)
      : _element_ptr(&element), _scope_entry(node) {}
  RQ_ALWAYS_INLINE Node(EntryElement &element,
                        const rq::Entry<EntryElement> &entry)
      : _element_ptr(&element), _scope_entry(entry) {}
  Node(const Self &) = delete;
  Node(Self &&) = delete;
  RQ_ALWAYS_INLINE ~Node() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasElement() const {
    return this->_element_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEntry() const {
    return !this->_scope_entry.getIsEmpty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE EntryElement &getElement() {
    return rq::dereferencePtr(this->_element_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement &getElement() const {
    return rq::dereferencePtr(this->_element_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entry<EntryElement> &getEntry() {
    return this->_scope_entry;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entry<EntryElement> &
  getEntry() const {
    return this->_scope_entry;
  }
};

template <typename EntryElementParam> struct EntryIterator final {
  using EntryElement = EntryElementParam;
  using Self = rq::EntryIterator<EntryElement>;
  using value_type = EntryElement;
  using reference = EntryElement &;
  using pointer = EntryElement *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Entry<EntryElement> _entry;

  RQ_ALWAYS_INLINE EntryIterator() = default;
  RQ_ALWAYS_INLINE explicit EntryIterator(rq::Entry<EntryElement> &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsEntryElement()) {
      this->_entry = rq::Entry<EntryElement>();
    } else if (this->_entry.getIsNode()) {
      this->_entry = rq::Entry<EntryElement>(this->_entry.getNode());
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
  [[nodiscard]] RQ_ALWAYS_INLINE EntryElement &operator*() {
    if (this->_entry.getIsEntryElement()) {
      return this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement &operator*() const {
    if (this->_entry.getIsEntryElement()) {
      return this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE EntryElement *operator->() {
    if (this->_entry.getIsEntryElement()) {
      return &this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return &this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement *operator->() const {
    if (this->_entry.getIsEntryElement()) {
      return &this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return &this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

template <typename EntryElementParam> struct ConstEntryIterator final {
  using EntryElement = EntryElementParam;
  using Self = rq::ConstEntryIterator<EntryElement>;
  using value_type = const EntryElement;
  using reference = const EntryElement &;
  using pointer = EntryElement *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::ConstEntry<EntryElement> _entry;

  RQ_ALWAYS_INLINE ConstEntryIterator() = default;
  RQ_ALWAYS_INLINE explicit ConstEntryIterator(
      const rq::Entry<EntryElement> &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE explicit ConstEntryIterator(
      const rq::ConstEntry<EntryElement> &entry)
      : _entry(entry) {}
  RQ_ALWAYS_INLINE Self &operator++() {
    if (this->_entry.getIsEntryElement()) {
      this->_entry = rq::ConstEntry<EntryElement>();
    } else if (this->_entry.getIsNode()) {
      this->_entry = rq::ConstEntry<EntryElement>(this->_entry.getNode());
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
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement &operator*() const {
    if (this->_entry.getIsEntryElement()) {
      return this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const EntryElement *operator->() const {
    if (this->_entry.getIsEntryElement()) {
      return &this->_entry.getEntryElement();
    } else if (this->_entry.getIsNode()) {
      return &this->_entry.getNode().getElement();
    }
    RQ_UNREACHABLE();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_entry.getIsEmpty();
  }
};

template <typename EntryElementParam>
inline rq::EntryIterator<EntryElementParam> Entry<EntryElementParam>::begin() {
  return rq::EntryIterator<EntryElementParam>(*this);
}

template <typename EntryElementParam>
inline rq::EntryIterator<EntryElementParam> Entry<EntryElementParam>::end() {
  return rq::EntryIterator<EntryElementParam>();
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
Entry<EntryElementParam>::begin() const {
  return rq::ConstEntryIterator<EntryElementParam>(*this);
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
Entry<EntryElementParam>::end() const {
  return rq::ConstEntryIterator<EntryElementParam>();
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
Entry<EntryElementParam>::cbegin() const {
  return rq::ConstEntryIterator<EntryElementParam>(*this);
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
Entry<EntryElementParam>::cend() const {
  return rq::ConstEntryIterator<EntryElementParam>();
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
ConstEntry<EntryElementParam>::begin() const {
  return rq::ConstEntryIterator<EntryElementParam>(*this);
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
ConstEntry<EntryElementParam>::end() const {
  return rq::ConstEntryIterator<EntryElementParam>();
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
ConstEntry<EntryElementParam>::cbegin() const {
  return rq::ConstEntryIterator<EntryElementParam>(*this);
}

template <typename EntryElementParam>
inline rq::ConstEntryIterator<EntryElementParam>
ConstEntry<EntryElementParam>::cend() const {
  return rq::ConstEntryIterator<EntryElementParam>();
}

struct SimpleBuiltinSymbol : public rq::Symbol {
  using Self = rq::SimpleBuiltinSymbol;

  SimpleBuiltinSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {
    RQ_ASSERT(rq::getIsSimpleBuiltin(kind), "kind not builtin simple symbol");
  }
  SimpleBuiltinSymbol(const Self &) = delete;
  SimpleBuiltinSymbol(Self &&) = delete;
  virtual ~SimpleBuiltinSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InferenceSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::InferenceSymbol;

  InferenceSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::INFERENCE) {}
  InferenceSymbol(const Self &) = delete;
  InferenceSymbol(Self &&) = delete;
  virtual ~InferenceSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VoidSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VoidSymbol;

  VoidSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::VOID) {}
  VoidSymbol(const Self &) = delete;
  VoidSymbol(Self &&) = delete;
  virtual ~VoidSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NullSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NullSymbol;

  NullSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::NULL_) {}
  NullSymbol(const Self &) = delete;
  NullSymbol(Self &&) = delete;
  virtual ~NullSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NoReturnSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NoReturnSymbol;

  NoReturnSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::NO_RETURN) {}
  NoReturnSymbol(const Self &) = delete;
  NoReturnSymbol(Self &&) = delete;
  virtual ~NoReturnSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VariadicArgumentsSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VariadicArgumentsSymbol;

  VariadicArgumentsSymbol()
      : rq::SimpleBuiltinSymbol(rq::SymbolKind::VARIADIC_ARGUMENTS) {}
  VariadicArgumentsSymbol(const Self &) = delete;
  VariadicArgumentsSymbol(Self &&) = delete;
  virtual ~VariadicArgumentsSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BooleanSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::BooleanSymbol;

  BooleanSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BOOLEAN) {}
  BooleanSymbol(const Self &) = delete;
  BooleanSymbol(Self &&) = delete;
  virtual ~BooleanSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Utf8Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;

  Utf8Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::UTF8) {}
  Utf8Symbol(const Self &) = delete;
  Utf8Symbol(Self &&) = delete;
  virtual ~Utf8Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Bfloat16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Bfloat16Symbol;

  Bfloat16Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BFLOAT16) {}
  Bfloat16Symbol(const Self &) = delete;
  Bfloat16Symbol(Self &&) = delete;
  virtual ~Bfloat16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary16Symbol;

  Binary16Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BINARY16) {}
  Binary16Symbol(const Self &) = delete;
  Binary16Symbol(Self &&) = delete;
  virtual ~Binary16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary32Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary32Symbol;

  Binary32Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BINARY32) {}
  Binary32Symbol(const Self &) = delete;
  Binary32Symbol(Self &&) = delete;
  virtual ~Binary32Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary64Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary64Symbol;

  Binary64Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BINARY64) {}
  Binary64Symbol(const Self &) = delete;
  Binary64Symbol(Self &&) = delete;
  virtual ~Binary64Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary128Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary128Symbol;

  Binary128Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::BINARY128) {}
  Binary128Symbol(const Self &) = delete;
  Binary128Symbol(Self &&) = delete;
  virtual ~Binary128Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DepthedBuiltinSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::DepthedBuiltinSymbol;

  unsigned _bit_depth{};

  DepthedBuiltinSymbol(rq::SymbolKind kind, unsigned bit_depth)
      : rq::Symbol(kind), _bit_depth(bit_depth) {
    RQ_ASSERT(rq::getIsDepthedBuiltin(kind), "kind not builtin depthed symbol");
  }
  DepthedBuiltinSymbol(const Self &) = delete;
  DepthedBuiltinSymbol(Self &&) = delete;
  virtual ~DepthedBuiltinSymbol() {}
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

struct WordSymbol : public rq::DepthedBuiltinSymbol {
  using Self = rq::WordSymbol;

  WordSymbol(unsigned bit_depth)
      : rq::DepthedBuiltinSymbol(rq::SymbolKind::WORD, bit_depth) {}
  WordSymbol(const Self &) = delete;
  WordSymbol(Self &&) = delete;
  virtual ~WordSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct UnsignedSymbol : public rq::DepthedBuiltinSymbol {
  using Self = rq::UnsignedSymbol;

  UnsignedSymbol(unsigned bit_depth)
      : rq::DepthedBuiltinSymbol(rq::SymbolKind::UNSIGNED, bit_depth) {}
  UnsignedSymbol(const Self &) = delete;
  UnsignedSymbol(Self &&) = delete;
  virtual ~UnsignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignedSymbol : public rq::DepthedBuiltinSymbol {
  using Self = rq::SignedSymbol;

  SignedSymbol(unsigned bit_depth)
      : rq::DepthedBuiltinSymbol(rq::SymbolKind::SIGNED, bit_depth) {}
  SignedSymbol(const Self &) = delete;
  SignedSymbol(Self &&) = delete;
  virtual ~SignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SimpleSubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SimpleSubtypeSymbol;

  rq::Symbol *_root_ptr{nullptr};

  SimpleSubtypeSymbol(rq::SymbolKind kind, rq::Symbol &base)
      : rq::Symbol(kind), _root_ptr(&base) {
    RQ_ASSERT(rq::getIsSimpleSubtype(kind), "kind not simple subtype symbol");
  }
  SimpleSubtypeSymbol(const Self &) = delete;
  SimpleSubtypeSymbol(Self &&) = delete;
  virtual ~SimpleSubtypeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot() {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_root_ptr);
    id.AddInteger(static_cast<unsigned>(this->_kind));
  }
};

struct RangeSymbol : public rq::SimpleSubtypeSymbol {
  using Self = rq::RangeSymbol;

  RangeSymbol(rq::Symbol &root)
      : rq::SimpleSubtypeSymbol(rq::SymbolKind::RANGE, root) {}
  RangeSymbol(const Self &) = delete;
  RangeSymbol(Self &&) = delete;
  virtual ~RangeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ReferenceSymbol : public rq::SimpleSubtypeSymbol {
  using Self = rq::ReferenceSymbol;

  ReferenceSymbol(rq::Symbol &root)
      : rq::SimpleSubtypeSymbol(rq::SymbolKind::REFERENCE, root) {}
  ReferenceSymbol(const Self &) = delete;
  ReferenceSymbol(Self &&) = delete;
  virtual ~ReferenceSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PointerSymbol : public rq::SimpleSubtypeSymbol {
  using Self = rq::PointerSymbol;

  PointerSymbol(rq::Symbol &root)
      : rq::SimpleSubtypeSymbol(rq::SymbolKind::POINTER, root) {}
  PointerSymbol(const Self &) = delete;
  PointerSymbol(Self &&) = delete;
  virtual ~PointerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FatPointerSymbol : public rq::SimpleSubtypeSymbol {
  using Self = rq::FatPointerSymbol;

  FatPointerSymbol(rq::Symbol &root)
      : rq::SimpleSubtypeSymbol(rq::SymbolKind::FAT_POINTER, root) {}
  FatPointerSymbol(const Self &) = delete;
  FatPointerSymbol(Self &&) = delete;
  virtual ~FatPointerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InferencedCountArraySymbol : public rq::SimpleSubtypeSymbol {
  using Self = rq::InferencedCountArraySymbol;

  InferencedCountArraySymbol(rq::Symbol &root)
      : rq::SimpleSubtypeSymbol(rq::SymbolKind::INFERENCED_COUNT_ARRAY, root) {}
  InferencedCountArraySymbol(const Self &) = delete;
  InferencedCountArraySymbol(Self &&) = delete;
  virtual ~InferencedCountArraySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ArraySymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArraySymbol;

  rq::Symbol *_root_ptr;
  unsigned _count;

  ArraySymbol(rq::Symbol &root, unsigned count)
      : rq::Symbol(rq::SymbolKind::ARRAY), _root_ptr(&root), _count(count) {}
  ArraySymbol(const Self &) = delete;
  ArraySymbol(Self &&) = delete;
  virtual ~ArraySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot() {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getCount() const {
    return this->_count;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_root_ptr);
    id.AddInteger(this->_count);
  }
};

struct LayoutSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::LayoutSymbol;

  rq::Entry<rq::Symbol> _properties;

  LayoutSymbol(rq::Entry<rq::Symbol> properties)
      : rq::Symbol(rq::SymbolKind::LAYOUT), _properties(properties) {}
  LayoutSymbol(const Self &) = delete;
  LayoutSymbol(Self &&) = delete;
  virtual ~LayoutSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entry<rq::Symbol> getProperties() const {
    return this->_properties;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
  }
};

struct SignatureSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SignatureSymbol;

  rq::Symbol *_return_ptr;
  rq::Entry<rq::Symbol> _parameters;

  SignatureSymbol(rq::Symbol &return_, rq::Entry<rq::Symbol> parameters)
      : rq::Symbol(rq::SymbolKind::SIGNATURE), _return_ptr(&return_),
        _parameters(parameters) {}
  SignatureSymbol(const Self &) = delete;
  SignatureSymbol(Self &&) = delete;
  virtual ~SignatureSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getReturn() {
    return rq::dereferencePtr(this->_return_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getReturn() const {
    return rq::dereferencePtr(this->_return_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entry<rq::Symbol> getParameters() const {
    return this->_parameters;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_return_ptr);
    id.AddInteger(static_cast<unsigned>(this->_kind));
  }
};

struct ExtensionSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ExtensionSymbol;

  rq::Symbol *_lvalue_ptr;
  rq::Symbol *_rvalue_ptr;

  ExtensionSymbol(rq::Symbol &rvalue, rq::Symbol &lvalue)
      : rq::Symbol(rq::SymbolKind::EXTENSION), _lvalue_ptr(&lvalue),
        _rvalue_ptr(&rvalue) {}
  ExtensionSymbol(const Self &) = delete;
  ExtensionSymbol(Self &&) = delete;
  virtual ~ExtensionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getLValue() {
    return rq::dereferencePtr(this->_lvalue_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getLValue() const {
    return rq::dereferencePtr(this->_lvalue_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRValue() {
    return rq::dereferencePtr(this->_rvalue_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRValue() const {
    return rq::dereferencePtr(this->_rvalue_ptr);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_lvalue_ptr);
    id.AddPointer(this->_rvalue_ptr);
  }
};

struct ArithmeticSequenceSymbol : public rq::Symbol,
                                  public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceSymbol;

  rq::Symbol *_root_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  ArithmeticSequenceSymbol(rq::SymbolKind kind, rq::Symbol &root,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step)
      : rq::Symbol(kind), _root_ptr(&root), _condition(condition), _step(step) {
    RQ_ASSERT(rq::getIsArithmeticSequence(kind),
              "kind not arithmetic sequence symbol");
  }
  ArithmeticSequenceSymbol(const Self &) = delete;
  ArithmeticSequenceSymbol(Self &&) = delete;
  virtual ~ArithmeticSequenceSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot() {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const {
    return this->_condition;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const {
    return this->_step;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    // no need to fold kind
    id.AddPointer(this->_root_ptr);
    id.AddInteger(static_cast<unsigned>(this->_condition));
    id.AddInteger(static_cast<unsigned>(this->_step));
  }
};

struct ArithmeticIntervalSymbol : public rq::ArithmeticSequenceSymbol {
  using Self = rq::ArithmeticIntervalSymbol;

  ArithmeticIntervalSymbol(rq::Symbol &root,
                           rq::ArithmeticSequenceCondition condition)
      : rq::ArithmeticSequenceSymbol(rq::SymbolKind::ARITHMETIC_INTERVAL, root,
                                     condition,
                                     rq::ArithmeticSequenceStep::NONE) {}
  ArithmeticIntervalSymbol(const Self &) = delete;
  ArithmeticIntervalSymbol(Self &&) = delete;
  virtual ~ArithmeticIntervalSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FiniteArithmeticProgressionSymbol : public rq::ArithmeticSequenceSymbol {
  using Self = rq::FiniteArithmeticProgressionSymbol;

  FiniteArithmeticProgressionSymbol(rq::Symbol &root,
                                    rq::ArithmeticSequenceCondition condition,
                                    rq::ArithmeticSequenceStep step)
      : rq::ArithmeticSequenceSymbol(
            rq::SymbolKind::FINITE_ARITHMETIC_PROGRESSION, root, condition,
            step) {}
  FiniteArithmeticProgressionSymbol(const Self &) = delete;
  FiniteArithmeticProgressionSymbol(Self &&) = delete;
  virtual ~FiniteArithmeticProgressionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InfiniteArithmeticProgressionSymbol
    : public rq::ArithmeticSequenceSymbol {
  using Self = rq::FiniteArithmeticProgressionSymbol;

  InfiniteArithmeticProgressionSymbol(rq::Symbol &root,
                                      rq::ArithmeticSequenceStep step)
      : rq::ArithmeticSequenceSymbol(
            rq::SymbolKind::INFINITE_ARITHMETIC_PROGRESSION, root,
            rq::ArithmeticSequenceCondition::NONE, step) {}
  InfiniteArithmeticProgressionSymbol(const Self &) = delete;
  InfiniteArithmeticProgressionSymbol(Self &&) = delete;
  virtual ~InfiniteArithmeticProgressionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScopeSymbol : rq::Symbol {
  using Self = rq::ScopeSymbol;

  llvm::SmallDenseMap<llvm::StringRef, rq::Entry<rq::Symbol>> _named_values{};
  rq::Entry<rq::Symbol> _unamed_values{};

  ScopeSymbol() : rq::Symbol(rq::SymbolKind::SCOPE) {}
  ScopeSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {}
  ScopeSymbol(const Self &) = delete;
  ScopeSymbol(Self &&) = delete;
  ~ScopeSymbol() override {
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
      rq::Entry<rq::Symbol> &entry = it->second;
      rq::Node<rq::Symbol> &node =
          cache.allocateValue<rq::Node<rq::Symbol>>(symbol, entry);
      entry = rq::Entry<rq::Symbol>(node);
    } else {
      this->_named_values.insert({name, rq::Entry<rq::Symbol>(symbol)});
    }
  }
  inline void tabulateUnamedSymbol(rq::ContextCache &cache,
                                   rq::Symbol &symbol) {
    rq::Entry<rq::Symbol> &entry = this->_unamed_values;
    if (entry.getIsEmpty()) {
      entry = symbol;
      return;
    }
    rq::Node<rq::Symbol> &node =
        cache.allocateValue<rq::Node<rq::Symbol>>(symbol, entry);
    entry = rq::Entry<rq::Symbol>(node);
  }
  [[nodiscard]] inline rq::Entry<rq::Symbol>
  getNamedEntry(llvm::StringRef name) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::Entry<rq::Symbol>();
  }
  [[nodiscard]] inline rq::ConstEntry<rq::Symbol>
  getNamedEntry(llvm::StringRef name) const {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::ConstEntry<rq::Symbol>();
  }
  [[nodiscard]] inline rq::Entry<rq::Symbol> getUnamedEntry() {
    return this->_unamed_values;
  }
  [[nodiscard]] inline rq::ConstEntry<rq::Symbol> getUnamedEntry() const {
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

namespace detail {
struct LocatedSymbol {
  using Self = rq::detail::LocatedSymbol;

  const rq::Expression *_expression_ptr;

  LocatedSymbol(const rq::Expression &expression)
      : _expression_ptr(&expression) {}
  LocatedSymbol(const Self &) = delete;
  LocatedSymbol(Self &&) = delete;
  virtual ~LocatedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};
struct ModuleMemberSymbol {
  using Self = rq::detail::ModuleMemberSymbol;

  rq::ModuleSymbol *_module_ptr;

  ModuleMemberSymbol(rq::ModuleSymbol &module) : _module_ptr(&module) {}
  ModuleMemberSymbol(const Self &) = delete;
  ModuleMemberSymbol(Self &&) = delete;
  virtual ~ModuleMemberSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &getModule() const {
    return rq::dereferencePtr(this->_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getModule() {
    return rq::dereferencePtr(this->_module_ptr);
  }
};
struct ScopeMemberSymbol {
  using Self = rq::detail::ScopeMemberSymbol;

  rq::ScopeSymbol *_scope_ptr;

  ScopeMemberSymbol(rq::ScopeSymbol &scope) : _scope_ptr(&scope) {}
  ScopeMemberSymbol(const Self &) = delete;
  ScopeMemberSymbol(Self &&) = delete;
  virtual ~ScopeMemberSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ScopeSymbol &getScope() const {
    return rq::dereferencePtr(this->_scope_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScopeSymbol &getScope() {
    return rq::dereferencePtr(this->_scope_ptr);
  }
};
struct StatementAscribedSymbol {
  using Self = StatementAscribedSymbol;

  rq::StatementAttribute _attributes;

  StatementAscribedSymbol(rq::StatementAttribute attributes)
      : _attributes(attributes) {}
  StatementAscribedSymbol(const Self &) = delete;
  StatementAscribedSymbol(Self &&) = delete;
  virtual ~StatementAscribedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::StatementAttribute
  getStatementAttributes() const {
    return this->_attributes;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque() const {
    return rq::getHasOpaque(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobal() const {
    return rq::getHasGlobal(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatic() const {
    return rq::getHasStatic(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapture() const {
    return rq::getHasCapture(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEager() const {
    return rq::getHasEager(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayParent() const {
    return rq::getHasMayParent(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasParent() const {
    return rq::getHasParent(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAbstract() const {
    return rq::getHasAbstract(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVirtual() const {
    return rq::getHasVirtual(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverride() const {
    return rq::getHasOverride(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPosition() const {
    return rq::getHasPosition(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangle() const {
    return rq::getHasMangle(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPack() const {
    return rq::getHasPack(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLabel() const {
    return rq::getHasLabel(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const {
    return rq::getHasTemplate(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikely() const {
    return rq::getHasLikely(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnlikely() const {
    return rq::getHasUnlikely(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDepreciated() const {
    return rq::getHasDepreciated(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExport() const {
    return rq::getHasExport(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPublic() const {
    return rq::getHasPublic(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasProtected() const {
    return rq::getHasProtected(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayCopy() const {
    return rq::getHasMayCopy(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayMove() const {
    return rq::getHasMayMove(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutateWith() const {
    return rq::getHasMutateWith(this->_attributes);
  }
};
struct NamedSymbol {
  using Self = rq::detail::NamedSymbol;

  llvm::StringRef _name;

  NamedSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }
  NamedSymbol(const Self &) = delete;
  NamedSymbol(Self &&) = delete;
  virtual ~NamedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct MaybeNamedSymbol {
  using Self = rq::detail::NamedSymbol;

  llvm::StringRef _name{};

  MaybeNamedSymbol() = default;
  MaybeNamedSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }
  MaybeNamedSymbol(const Self &) = delete;
  MaybeNamedSymbol(Self &&) = delete;
  virtual ~MaybeNamedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const {
    return !this->_name.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct TypedSymbol {
  using Self = rq::detail::TypedSymbol;

  rq::TypeSymbol *_type_ptr{nullptr};

  TypedSymbol() {}
  TypedSymbol(const Self &) = delete;
  TypedSymbol(Self &&) = delete;
  virtual ~TypedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const {
    return this->_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeSymbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  RQ_ALWAYS_INLINE void setType(rq::TypeSymbol &type) {
    rq::assignSingleValue(this->_type_ptr, &type);
  }
};
} // namespace detail

struct VariableSymbol : public rq::Symbol,
                        public rq::detail::LocatedSymbol,
                        public rq::detail::ModuleMemberSymbol,
                        public rq::detail::ScopeMemberSymbol,
                        public rq::detail::StatementAscribedSymbol,
                        public rq::detail::NamedSymbol,
                        public rq::detail::TypedSymbol {
  using Self = rq::VariableSymbol;

  VariableSymbol(rq::ModuleSymbol &module, rq::ScopeSymbol &scope,
                 llvm::StringRef name, rq::StatementAttribute attributes,
                 const rq::Expression &expression)
      : rq::Symbol(rq::SymbolKind::VARIABLE),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::NamedSymbol(name) {}
  VariableSymbol(const Self &) = delete;
  VariableSymbol(Self &&) = delete;
  virtual ~VariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumeratorSymbol : public rq::Symbol,
                          public rq::detail::LocatedSymbol,
                          public rq::detail::ModuleMemberSymbol,
                          public rq::detail::StatementAscribedSymbol,
                          public rq::detail::NamedSymbol,
                          public rq::detail::TypedSymbol {
  using Self = rq::EnumeratorSymbol;

  rq::EnumerationSymbol *_enumeration_ptr;

  EnumeratorSymbol(rq::ModuleSymbol &module, llvm::StringRef name,
                   rq::StatementAttribute attributes,
                   const rq::Expression &expression)
      : rq::Symbol(rq::SymbolKind::ENUMERATOR),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::NamedSymbol(name) {}
  EnumeratorSymbol(const Self &) = delete;
  EnumeratorSymbol(Self &&) = delete;
  virtual ~EnumeratorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LayoutPropertySymbol : public rq::Symbol,
                              public rq::detail::LocatedSymbol,
                              public rq::detail::StatementAscribedSymbol,
                              public rq::detail::MaybeNamedSymbol,
                              public rq::detail::TypedSymbol {
  using Self = rq::LayoutPropertySymbol;

  rq::LayoutSymbol *_layout_ptr;

  LayoutPropertySymbol(llvm::StringRef name, rq::StatementAttribute attributes,
                       const rq::Expression &expression)
      : rq::Symbol(rq::SymbolKind::LAYOUT_PROPERTY),
        rq::detail::LocatedSymbol(expression),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::MaybeNamedSymbol(name) {}
  LayoutPropertySymbol(const Self &) = delete;
  LayoutPropertySymbol(Self &&) = delete;
  virtual ~LayoutPropertySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassPropertySymbol : public rq::Symbol,
                             public rq::detail::LocatedSymbol,
                             public rq::detail::StatementAscribedSymbol,
                             public rq::detail::MaybeNamedSymbol,
                             public rq::detail::TypedSymbol {
  using Self = rq::ClassPropertySymbol;

  rq::ClassSymbol *_class_ptr;

  ClassPropertySymbol(llvm::StringRef name, rq::StatementAttribute attributes,
                      const rq::Expression &expression)
      : rq::Symbol(rq::SymbolKind::CLASS_PROPERTY),
        rq::detail::LocatedSymbol(expression),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::MaybeNamedSymbol(name) {}
  ClassPropertySymbol(const Self &) = delete;
  ClassPropertySymbol(Self &&) = delete;
  virtual ~ClassPropertySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignatureParameterSymbol : public rq::Symbol,
                                  public rq::detail::LocatedSymbol,
                                  public rq::detail::ModuleMemberSymbol,
                                  public rq::detail::ScopeMemberSymbol,
                                  public rq::detail::StatementAscribedSymbol,
                                  public rq::detail::MaybeNamedSymbol,
                                  public rq::detail::TypedSymbol {
  using Self = rq::SignatureParameterSymbol;

  rq::SignatureSymbol *_signature_ptr;

  SignatureParameterSymbol(
      const rq::Expression &expression, rq::ModuleSymbol &module,
      rq::ScopeSymbol &scope, llvm::StringRef name = "",
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::Symbol(rq::SymbolKind::SIGNATURE_PARAMETER),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::MaybeNamedSymbol(name) {}
  SignatureParameterSymbol(const Self &) = delete;
  SignatureParameterSymbol(Self &&) = delete;
  virtual ~SignatureParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateParameterSymbol : public rq::Symbol,
                                 public rq::detail::LocatedSymbol,
                                 public rq::detail::ModuleMemberSymbol,
                                 public rq::detail::ScopeMemberSymbol,
                                 public rq::detail::StatementAscribedSymbol,
                                 public rq::detail::MaybeNamedSymbol,
                                 public rq::detail::TypedSymbol {
  using Self = rq::TemplateParameterSymbol;

  rq::TemplateSymbol *_template_ptr;

  TemplateParameterSymbol(
      const rq::Expression &expression, rq::ModuleSymbol &module,
      rq::ScopeSymbol &scope, llvm::StringRef name = "",
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::Symbol(rq::SymbolKind::TEMPLATE_PARAMETER),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::MaybeNamedSymbol(name) {}
  TemplateParameterSymbol(const Self &) = delete;
  TemplateParameterSymbol(Self &&) = delete;
  virtual ~TemplateParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct MutationSymbol : public rq::Symbol,
                        public rq::detail::LocatedSymbol,
                        public rq::detail::ModuleMemberSymbol,
                        public rq::detail::ScopeMemberSymbol,
                        public rq::detail::StatementAscribedSymbol,
                        public rq::detail::NamedSymbol {
  using Self = rq::MutationSymbol;

  MutationSymbol(llvm::StringRef name, rq::StatementAttribute attributes,
                 const rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::ScopeSymbol &scope)
      : rq::Symbol(rq::SymbolKind::MUTATION),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::NamedSymbol(name) {}
  MutationSymbol(const Self &) = delete;
  MutationSymbol(Self &&) = delete;
  virtual ~MutationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TableSymbol : public rq::ScopeSymbol, public rq::detail::NamedSymbol {
  using Self = rq::TableSymbol;

  TableSymbol(llvm::StringRef name)
      : rq::ScopeSymbol(rq::SymbolKind::TABLE), rq::detail::NamedSymbol(name) {}
  TableSymbol(const Self &) = delete;
  TableSymbol(Self &&) = delete;
  virtual ~TableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassSymbol : public rq::ScopeSymbol,
                     public rq::detail::LocatedSymbol,
                     public rq::detail::ModuleMemberSymbol,
                     public rq::detail::ScopeMemberSymbol,
                     public rq::detail::StatementAscribedSymbol,
                     public rq::detail::NamedSymbol {
  using Self = rq::ClassSymbol;

  rq::Entry<rq::ClassPropertySymbol> _class_properties;

  ClassSymbol(llvm::StringRef name, rq::StatementAttribute attributes,
              const rq::Expression &expression, rq::ModuleSymbol &module,
              rq::ScopeSymbol &scope)
      : rq::ScopeSymbol(rq::SymbolKind::CLASS),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::NamedSymbol(name) {}
  ClassSymbol(const Self &) = delete;
  ClassSymbol(Self &&) = delete;
  virtual ~ClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumerationSymbol : public rq::ScopeSymbol,
                           public rq::detail::LocatedSymbol,
                           public rq::detail::ModuleMemberSymbol,
                           public rq::detail::ScopeMemberSymbol,
                           public rq::detail::StatementAscribedSymbol,
                           public rq::detail::NamedSymbol {
  using Self = rq::EnumerationSymbol;

  rq::Entry<rq::EnumeratorSymbol> _enumerators;

  EnumerationSymbol(llvm::StringRef name, rq::StatementAttribute attributes,
                    const rq::Expression &expression, rq::ModuleSymbol &module,
                    rq::ScopeSymbol &scope)
      : rq::ScopeSymbol(rq::SymbolKind::ENUMERATION),
        rq::detail::LocatedSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::ScopeMemberSymbol(scope),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::NamedSymbol(name) {}
  EnumerationSymbol(const Self &) = delete;
  EnumerationSymbol(Self &&) = delete;
  virtual ~EnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ProcedureSymbol : public rq::ScopeSymbol,
                         public rq::detail::LocatedSymbol,
                         public rq::detail::StatementAscribedSymbol,
                         public rq::detail::ModuleMemberSymbol {
  using Self = rq::ProcedureSymbol;

  rq::SignatureSymbol *_signature_ptr;

  ProcedureSymbol(
      rq::SymbolKind kind, rq::ModuleSymbol &module,
      const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ScopeSymbol(kind), rq::detail::LocatedSymbol(expression),
        rq::detail::StatementAscribedSymbol(attributes),
        rq::detail::ModuleMemberSymbol(module), _signature_ptr(nullptr) {}
  ProcedureSymbol(const Self &) = delete;
  ProcedureSymbol(Self &&) = delete;
  virtual ~ProcedureSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EntrySymbol : public rq::ProcedureSymbol {
  using Self = rq::EntrySymbol;

  EntrySymbol(rq::ModuleSymbol &module, const rq::Expression &expression)
      : rq::ProcedureSymbol(rq::SymbolKind::ENTRY, module, expression) {}
  EntrySymbol(const Self &) = delete;
  EntrySymbol(Self &&) = delete;
  virtual ~EntrySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FunctionSymbol : public rq::ProcedureSymbol,
                        public rq::detail::NamedSymbol {
  using Self = rq::FunctionSymbol;

  FunctionSymbol(
      rq::ModuleSymbol &module, llvm::StringRef name,
      const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::FUNCTION, module, expression,
                            attributes),
        rq::detail::NamedSymbol(name) {}
  FunctionSymbol(const Self &) = delete;
  FunctionSymbol(Self &&) = delete;
  virtual ~FunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct MethodSymbol : public rq::ProcedureSymbol,
                      public rq::detail::NamedSymbol {
  using Self = rq::MethodSymbol;

  MethodSymbol(rq::ModuleSymbol &module, llvm::StringRef name,
               const rq::Expression &expression,
               rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::METHOD, module, expression,
                            attributes),
        rq::detail::NamedSymbol(name) {}
  MethodSymbol(const Self &) = delete;
  MethodSymbol(Self &&) = delete;
  virtual ~MethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionFunctionSymbol : public rq::ProcedureSymbol,
                                 public rq::detail::NamedSymbol {
  using Self = rq::ExtensionFunctionSymbol;

  ExtensionFunctionSymbol(
      rq::ModuleSymbol &module, llvm::StringRef name,
      const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::EXTENSION_FUNCTION, module,
                            expression, attributes),
        rq::detail::NamedSymbol(name) {}
  ExtensionFunctionSymbol(const Self &) = delete;
  ExtensionFunctionSymbol(Self &&) = delete;
  virtual ~ExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionMethodSymbol : public rq::ProcedureSymbol,
                               public rq::detail::NamedSymbol {
  using Self = rq::ExtensionMethodSymbol;

  ExtensionMethodSymbol(
      rq::ModuleSymbol &module, llvm::StringRef name,
      const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::EXTENSION_METHOD, module,
                            expression, attributes),
        rq::detail::NamedSymbol(name) {}
  ExtensionMethodSymbol(const Self &) = delete;
  ExtensionMethodSymbol(Self &&) = delete;
  virtual ~ExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ConstructorSymbol : public rq::ProcedureSymbol {
  using Self = rq::ConstructorSymbol;

  ConstructorSymbol(
      rq::ModuleSymbol &module, const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::CONSTRUCTOR, module, expression,
                            attributes) {}
  ConstructorSymbol(const Self &) = delete;
  ConstructorSymbol(Self &&) = delete;
  virtual ~ConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DestructorSymbol : public rq::ProcedureSymbol {
  using Self = rq::DestructorSymbol;

  DestructorSymbol(
      rq::ModuleSymbol &module, const rq::Expression &expression,
      rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::DESTRUCTOR, module, expression,
                            attributes) {}
  DestructorSymbol(const Self &) = delete;
  DestructorSymbol(Self &&) = delete;
  virtual ~DestructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangerSymbol : public rq::ProcedureSymbol {
  using Self = rq::RangerSymbol;

  RangerSymbol(rq::ModuleSymbol &module, const rq::Expression &expression,
               rq::StatementAttribute attributes = rq::StatementAttribute::NONE)
      : rq::ProcedureSymbol(rq::SymbolKind::RANGER, module, expression,
                            attributes) {}
  RangerSymbol(const Self &) = delete;
  RangerSymbol(Self &&) = delete;
  virtual ~RangerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateSymbol : public rq::ScopeSymbol, public rq::detail::NamedSymbol {
  using Self = rq::TemplateSymbol;

  rq::Entry<rq::TemplateParameterSymbol> _template_parameters;

  TemplateSymbol(llvm::StringRef name, rq::SymbolKind kind,
                 const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::ScopeSymbol(kind), rq::detail::NamedSymbol(name),
        _template_parameters(parameters) {}
  TemplateSymbol(const Self &) = delete;
  TemplateSymbol(Self &&) = delete;
  virtual ~TemplateSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateClassSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateClassSymbol;

  TemplateClassSymbol(llvm::StringRef name,
                      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_CLASS, parameters) {}
  TemplateClassSymbol(const Self &) = delete;
  TemplateClassSymbol(Self &&) = delete;
  virtual ~TemplateClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateEnumerationSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateEnumerationSymbol;

  TemplateEnumerationSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_ENUMERATION,
                           parameters) {}
  TemplateEnumerationSymbol(const Self &) = delete;
  TemplateEnumerationSymbol(Self &&) = delete;
  virtual ~TemplateEnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateVariableSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateVariableSymbol;

  TemplateVariableSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_VARIABLE,
                           parameters) {}
  TemplateVariableSymbol(const Self &) = delete;
  TemplateVariableSymbol(Self &&) = delete;
  virtual ~TemplateVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateFunctionSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateFunctionSymbol;

  TemplateFunctionSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_FUNCTION,
                           parameters) {}
  TemplateFunctionSymbol(const Self &) = delete;
  TemplateFunctionSymbol(Self &&) = delete;
  virtual ~TemplateFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateMethodSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateMethodSymbol;

  TemplateMethodSymbol(llvm::StringRef name,
                       const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_METHOD, parameters) {}
  TemplateMethodSymbol(const Self &) = delete;
  TemplateMethodSymbol(Self &&) = delete;
  virtual ~TemplateMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionFunctionSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateExtensionFunctionSymbol;

  TemplateExtensionFunctionSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_EXTENSION_FUNCTION,
                           parameters) {}
  TemplateExtensionFunctionSymbol(const Self &) = delete;
  TemplateExtensionFunctionSymbol(Self &&) = delete;
  virtual ~TemplateExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionMethodSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateExtensionMethodSymbol;

  TemplateExtensionMethodSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_EXTENSION_METHOD,
                           parameters) {}
  TemplateExtensionMethodSymbol(const Self &) = delete;
  TemplateExtensionMethodSymbol(Self &&) = delete;
  virtual ~TemplateExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateConstructorSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateConstructorSymbol;

  TemplateConstructorSymbol(
      llvm::StringRef name,
      const rq::Entry<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(name, rq::SymbolKind::TEMPLATE_CONSTRUCTOR,
                           parameters) {}
  TemplateConstructorSymbol(const Self &) = delete;
  TemplateConstructorSymbol(Self &&) = delete;
  virtual ~TemplateConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialSpecializationSymbol : public rq::ScopeSymbol {
  using Self = rq::PartialSpecializationSymbol;

  PartialSpecializationSymbol()
      : rq::ScopeSymbol(rq::SymbolKind::PARTIAL_CLASS) {}
  PartialSpecializationSymbol(rq::SymbolKind kind) : rq::ScopeSymbol(kind) {}
  PartialSpecializationSymbol(const Self &) = delete;
  PartialSpecializationSymbol(Self &&) = delete;
  virtual ~PartialSpecializationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialClassSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialClassSymbol;

  PartialClassSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_CLASS;
  }
  PartialClassSymbol(const Self &) = delete;
  PartialClassSymbol(Self &&) = delete;
  virtual ~PartialClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialEnumerationSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialEnumerationSymbol;

  PartialEnumerationSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_ENUMERATION;
  }
  PartialEnumerationSymbol(const Self &) = delete;
  PartialEnumerationSymbol(Self &&) = delete;
  virtual ~PartialEnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialVariableSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialVariableSymbol;

  PartialVariableSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_VARIABLE;
  }
  PartialVariableSymbol(const Self &) = delete;
  PartialVariableSymbol(Self &&) = delete;
  virtual ~PartialVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialFunctionSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialFunctionSymbol;

  PartialFunctionSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_FUNCTION;
  }
  PartialFunctionSymbol(const Self &) = delete;
  PartialFunctionSymbol(Self &&) = delete;
  virtual ~PartialFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialMethodSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialMethodSymbol;

  PartialMethodSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_METHOD;
  }
  PartialMethodSymbol(const Self &) = delete;
  PartialMethodSymbol(Self &&) = delete;
  virtual ~PartialMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionFunctionSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialExtensionFunctionSymbol;

  PartialExtensionFunctionSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_EXTENSION_FUNCTION;
  }
  PartialExtensionFunctionSymbol(const Self &) = delete;
  PartialExtensionFunctionSymbol(Self &&) = delete;
  virtual ~PartialExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionMethodSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialExtensionMethodSymbol;

  PartialExtensionMethodSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_EXTENSION_METHOD;
  }
  PartialExtensionMethodSymbol(const Self &) = delete;
  PartialExtensionMethodSymbol(Self &&) = delete;
  virtual ~PartialExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialConstructorSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialConstructorSymbol;

  PartialConstructorSymbol() : rq::PartialSpecializationSymbol() {
    this->_kind = rq::SymbolKind::PARTIAL_CONSTRUCTOR;
  }
  PartialConstructorSymbol(const Self &) = delete;
  PartialConstructorSymbol(Self &&) = delete;
  virtual ~PartialConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
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

struct ModuleSymbol final : public rq::Symbol {
  using Self = rq::ModuleSymbol;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;
  rq::Expression *_expression_ptr = nullptr;

  ModuleSymbol(rq::ModuleKind kind, llvm::StringRef path,
               llvm::MemoryBufferRef &&buffer)
      : rq::Symbol(rq::SymbolKind::MODULE), _module_kind(kind),
        _llvm_buffer_ref(std::move(buffer)), _path(path) {}
  ModuleSymbol(const Self &) = delete;
  ModuleSymbol(Self &&) = delete;
  ~ModuleSymbol() override {}
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

inline rq::InferenceSymbol &rq::ContextCache::getInferenceSymbol() {
  if (!this->_inference_symbol) {
    this->_inference_symbol = &this->allocateValue<rq::InferenceSymbol>();
  }
  return rq::dereferencePtr(this->_inference_symbol);
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

inline rq::DepthedBuiltinSymbol &
rq::ContextCache::_getOrInsertBuiltinDepthSymbol(rq::SymbolKind kind,
                                                 unsigned parameter) {
  llvm::FoldingSetNodeID id;
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(parameter);
  void *insert_pos = nullptr;
  if (rq::DepthedBuiltinSymbol *existing =
          this->_depthed_symbols.FindNodeOrInsertPos(id, insert_pos)) {
    return rq::dereferencePtr(existing);
  }
  rq::DepthedBuiltinSymbol &new_type =
      this->allocateValue<rq::DepthedBuiltinSymbol>(kind, parameter);
  this->_depthed_symbols.InsertNode(&new_type, insert_pos);
  return new_type;
}

inline rq::WordSymbol &rq::ContextCache::getWordSymbol(unsigned bit_depth) {
  return static_cast<rq::WordSymbol &>(
      this->_getOrInsertBuiltinDepthSymbol(rq::SymbolKind::WORD, bit_depth));
}

inline rq::UnsignedSymbol &
rq::ContextCache::getUnsignedSymbol(unsigned bit_depth) {
  return static_cast<rq::UnsignedSymbol &>(this->_getOrInsertBuiltinDepthSymbol(
      rq::SymbolKind::UNSIGNED, bit_depth));
}

inline rq::SignedSymbol &rq::ContextCache::getSignedSymbol(unsigned bit_depth) {
  return static_cast<rq::SignedSymbol &>(
      this->_getOrInsertBuiltinDepthSymbol(rq::SymbolKind::SIGNED, bit_depth));
}

} // namespace rq
