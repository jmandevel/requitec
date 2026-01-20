#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
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

struct Context;

// NOTE: all symbols have unique instances. you can always test if two symbols
// are exactly the same (including type attriubutes, etc) by comparing their
// pointers.

enum class SymbolKind : std::uint8_t {
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
  ASCII,
  UTF8,

  // SCALED BUILTIN
  GENERIC_INTEGER,
  SIGNED,
  UNSIGNED,
  GENERIC_FLOAT,
  BINARY,
  BFLOAT,

  // UNARY SUBTYPE
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

  // ARITHMETIC SEQUENCE
  ARITHMETIC_INTERVAL,
  FINITE_ARITHMETIC_PROGRESSION,
  INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  MODULE,
  IMPORT,
  FACADE,
  MUTATION,
  EXTENSION,

  // BINDING
  DYNAMIC_VARIABLE,
  STATIC_VARIABLE,
  ENUMERATOR,
  PROPERTY,
  CLASS_PARAMETER,
  LAYOUT_PARAMETER,
  TEMPLATE_PARAMETER,
  SIGNATURE_PARAMETER,
  LABEL,

  // SYMBOL TABLES
  TOP,
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
  TEMPLATE_DYNAMIC_VARIABLE,
  TEMPLATE_STATIC_VARIABLE,
  TEMPLATE_FUNCTION,
  TEMPLATE_METHOD,
  TEMPLATE_EXTENSION_FUNCTION,
  TEMPLATE_EXTENSION_METHOD,
  TEMPLATE_CONSTRUCTOR,

  // PARTIAL SPECIALIZATION
  PARTIAL_CLASS,
  PARTIAL_ENUMERATION,
  PARTIAL_DYNAMIC_VARIABLE,
  PARTIAL_STATIC_VARIABLE,
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
  case SY::ASCII:
    return "ascii";
  case SY::UTF8:
    return "utf8";

  // SCALED
  case SY::GENERIC_INTEGER:
    return "integer";
  case SY::SIGNED:
    return "signed";
  case SY::UNSIGNED:
    return "unsigned";
  case SY::GENERIC_FLOAT:
    return "float";
  case SY::BINARY:
    return "binary";
  case SY::BFLOAT:
    return "bfloat";

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
  case SY::IMPORT:
    return "import";
  case SY::FACADE:
    return "facade";
  case SY::MUTATION:
    return "mutation";

  // BINDING
  case SY::DYNAMIC_VARIABLE:
    return "dynamic_variable";
  case SY::STATIC_VARIABLE:
    return "static_variable";
  case SY::ENUMERATOR:
    return "enumerator";
  case SY::PROPERTY:
    return "property";
  case SY::CLASS_PARAMETER:
    return "class_parameter";
  case SY::LAYOUT_PARAMETER:
    return "layout_parameter";
  case SY::TEMPLATE_PARAMETER:
    return "template_parameter";
  case SY::SIGNATURE_PARAMETER:
    return "signature_parameter";
  case SY::LABEL:
    return "label";

  // SYMBOL TABLES
  case SY::TOP:
    return "top";
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
  case SY::TEMPLATE_DYNAMIC_VARIABLE:
    return "template_dynamic_variable";
  case SY::TEMPLATE_STATIC_VARIABLE:
    return "template_static_variable";
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
  case SY::PARTIAL_DYNAMIC_VARIABLE:
    return "partial_dynamic_variable";
  case SY::PARTIAL_STATIC_VARIABLE:
    return "partial_static_variable";
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
  // USABLE INHERITING PROPERTIES - are implmented via inherited types that can
  // be used themselves. every symbol can have at most one of these.
  TYPE = rq::getBit(0),
  SIMPLE_BUILTIN = rq::getBit(1),
  SCALED_BUILTIN = rq::getBit(2),
  UNARY_SUBTYPE = rq::getBit(3),
  COUNTED_SUBTYPE = rq::getBit(4),
  COMPOSITE_SUBTYPE = rq::getBit(5),
  ARITHMETIC_SEQUENCE = rq::getBit(6),
  MAYBE_DEFAULT_VALUED = rq::getBit(7),
  SYMBOL_TABLE = rq::getBit(8),
  PROCEDURE = rq::getBit(9),
  TEMPLATE = rq::getBit(10),
  PARTIAL_SPECIALIZATION = rq::getBit(11),
  // UNUSABLE INHERITING PROPERTIES - are implemented via inherited types that
  // can not be used themselves
  HAS_LOCATION = rq::getBit(12),
  MODULE_MEMBER = rq::getBit(13),
  MAYBE_CLASS_MEMBER = rq::getBit(14),
  HAS_LAYOUT = rq::getBit(15),
  HAS_SIGNATURE = rq::getBit(16),
  HAS_TEMPLATE_LAYOUT = rq::getBit(17),
  HAS_POSITIONAL_ENTRIES = rq::getBit(18),
  SYMBOL_TABLE_MEMBER = rq::getBit(19),
  HAS_STATIC_VALUE = rq::getBit(20),
  HAS_ATTRIBUTES = rq::getBit(21),
  MAYBE_HAS_NAME = rq::getBit(22),
  HAS_NAME = rq::getBit(23),
  HAS_TYPE = rq::getBit(24),
  HAS_IMPORT_MODULE = rq::getBit(25),
  // INFO PROPERTIES - have no data associated
  HAS_TEMPLATE_ALTERNATIVE = rq::getBit(26),
  ROOT = rq::getBit(27),
  INTEGER = rq::getBit(28),
  FLOAT = rq::getBit(29),
  CODEUNIT = rq::getBit(30),
  GENERIC = rq::getBit(31)
};

template <> struct is_flags<rq::SymbolFlags> : std::true_type {};

[[nodiscard]] inline rq::SymbolFlags getFlags(SymbolKind kind) {
  using namespace rq;
  using SY = SymbolKind;
  using SYF = SymbolFlags;
  switch (kind) {
  case rq::SymbolKind::NONE:
    return SYF::NONE;

  // ROOT WITH TYPE ATTRIBUTES
  case SY::TYPE:
    return SYF::TYPE | SYF::ROOT;

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
  case SY::ASCII:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::CODEUNIT;
  case SY::UTF8:
    return SYF::SIMPLE_BUILTIN | SYF::ROOT | SYF::CODEUNIT;

  // SCALED INTEGER
  case SY::GENERIC_INTEGER:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::INTEGER | SYF::GENERIC;
  case SY::SIGNED:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::INTEGER;
  case SY::UNSIGNED:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::INTEGER;
  case SY::GENERIC_FLOAT:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::FLOAT | SYF::GENERIC;
  case SY::BINARY:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::FLOAT;
  case SY::BFLOAT:
    return SYF::SCALED_BUILTIN | SYF::ROOT | SYF::FLOAT;

  // UNARY SUBTYPE
  case SY::RANGE:
    return SYF::UNARY_SUBTYPE | SYF::ROOT;
  case SY::REFERENCE:
    return SYF::UNARY_SUBTYPE | SYF::ROOT;
  case SY::POINTER:
    return SYF::UNARY_SUBTYPE | SYF::ROOT;
  case SY::FAT_POINTER:
    return SYF::UNARY_SUBTYPE | SYF::ROOT;
  case SY::INFERENCED_COUNT_ARRAY:
    return SYF::UNARY_SUBTYPE | SYF::ROOT;

  // COUNTED SUBTYPE
  case SY::ARRAY:
    return SYF::COUNTED_SUBTYPE | SYF::ROOT;

  // COMPOSITE SUBTYPE
  case SY::LAYOUT:
    return SYF::SYMBOL_TABLE | SYF::COMPOSITE_SUBTYPE | SYF::ROOT;
  case SY::SIGNATURE:
    return SYF::SYMBOL_TABLE | SYF::COMPOSITE_SUBTYPE | SYF::ROOT;

  // ARITHMETIC SEQUENCE
  case SY::ARITHMETIC_INTERVAL:
    return SYF::ARITHMETIC_SEQUENCE | SYF::ROOT;
  case SY::FINITE_ARITHMETIC_PROGRESSION:
    return SYF::ARITHMETIC_SEQUENCE | SYF::ROOT;
  case SY::INFINITE_ARITHMETIC_PROGRESSION:
    return SYF::ARITHMETIC_SEQUENCE | SYF::ROOT;

  // MISC
  case SY::MODULE:
    return SYF::SYMBOL_TABLE | SYF::HAS_LOCATION;
  case SY::IMPORT:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::HAS_IMPORT_MODULE;
  case SY::FACADE:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES;
  case SY::MUTATION:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES;
  case SY::EXTENSION:
    return SYF::HAS_SIGNATURE | SYF::HAS_TYPE;

  // BINDING
  case SY::DYNAMIC_VARIABLE:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::HAS_NAME | SYF::HAS_TYPE |
           SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::STATIC_VARIABLE:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_STATIC_VALUE | SYF::HAS_ATTRIBUTES | SYF::HAS_NAME |
           SYF::HAS_TYPE | SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::ENUMERATOR:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::MAYBE_DEFAULT_VALUED | SYF::HAS_NAME |
           SYF::HAS_TYPE;
  case SY::PROPERTY:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::MAYBE_DEFAULT_VALUED |
           SYF::MAYBE_HAS_NAME | SYF::HAS_TYPE;
  case SY::CLASS_PARAMETER:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::HAS_TYPE;
  case SY::LAYOUT_PARAMETER:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::MAYBE_HAS_NAME | SYF::HAS_TYPE;
  case SY::SIGNATURE_PARAMETER:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::MAYBE_DEFAULT_VALUED |
           SYF::MAYBE_HAS_NAME | SYF::HAS_TYPE;
  case SY::TEMPLATE_PARAMETER:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_ATTRIBUTES | SYF::MAYBE_DEFAULT_VALUED |
           SYF::MAYBE_HAS_NAME | SYF::HAS_TYPE;
  case SY::LABEL:
    return SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_LOCATION |
           SYF::HAS_NAME;

    // SYMBOL TABLES
  case SY::TOP:
    return SYF::SYMBOL_TABLE;
  case SY::TABLE:
    return SYF::SYMBOL_TABLE | SYF::HAS_NAME;
  case SY::CLASS:
    return SYF::SYMBOL_TABLE | SYF::HAS_LOCATION | SYF::MODULE_MEMBER |
           SYF::HAS_LAYOUT | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::HAS_TEMPLATE_ALTERNATIVE | SYF::ROOT;
  case SY::ENUMERATION:
    return SYF::SYMBOL_TABLE | SYF::HAS_LOCATION | SYF::MODULE_MEMBER |
           SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES | SYF::HAS_NAME |
           SYF::HAS_TEMPLATE_ALTERNATIVE | SYF::ROOT;

  // PROCEDURES
  case SY::ENTRY:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES;
  case SY::FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_TEMPLATE_ALTERNATIVE;
  case SY::DESTRUCTOR:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES;
  case SY::RANGER:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES;

  // TEMPLATE
  case SY::TEMPLATE_CLASS:
    return SYF::SYMBOL_TABLE | SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_ENUMERATION:
    return SYF::SYMBOL_TABLE | SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_DYNAMIC_VARIABLE:
    return SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_STATIC_VARIABLE:
    return SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_NAME | SYF::HAS_STATIC_VALUE |
           SYF::TEMPLATE;
  case SY::TEMPLATE_FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::TEMPLATE;
  case SY::TEMPLATE_CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::TEMPLATE;

  // PARTIAL SPECIALIZATION
  case SY::PARTIAL_CLASS:
    return SYF::SYMBOL_TABLE | SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_ENUMERATION:
    return SYF::SYMBOL_TABLE | SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_DYNAMIC_VARIABLE:
    return SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_NAME |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_STATIC_VARIABLE:
    return SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_NAME | SYF::HAS_STATIC_VALUE |
           SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_EXTENSION_FUNCTION:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_EXTENSION_METHOD:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
           SYF::HAS_NAME | SYF::PARTIAL_SPECIALIZATION;
  case SY::PARTIAL_CONSTRUCTOR:
    return SYF::PROCEDURE | SYF::HAS_LOCATION | SYF::HAS_SIGNATURE |
           SYF::MODULE_MEMBER | SYF::SYMBOL_TABLE_MEMBER | SYF::HAS_ATTRIBUTES |
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SCALED_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::UNARY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::COUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::COMPOSITE_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsMaybeDefaultValued(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::MAYBE_DEFAULT_VALUED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequence(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolTableMember(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SYMBOL_TABLE_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::SYMBOL_TABLE);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::SymbolFlags::HAS_TEMPLATE_ALTERNATIVE);
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
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getMaybeHasName(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::MAYBE_HAS_NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocation(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_LOCATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsModuleMember(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::MODULE_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsMaybeClassMember(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::MAYBE_CLASS_MEMBER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLayout(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_LAYOUT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasSignature(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_SIGNATURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateLayout(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_TEMPLATE_LAYOUT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPositionalEntries(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_POSITIONAL_ENTRIES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasStaticValue(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_STATIC_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAttributes(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_ATTRIBUTES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasType(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasImportModule(rq::SymbolKind kind) {
  rq::SymbolFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::SymbolFlags::HAS_IMPORT_MODULE);
}

[[nodiscard]] inline rq::SymbolKind getTemplate(rq::SymbolKind symbol) {
  using namespace rq;
  using SY = SymbolKind;
  switch (symbol) {
  case SY::CLASS:
    return SY::TEMPLATE_CLASS;
  case SY::ENUMERATION:
    return SY::TEMPLATE_ENUMERATION;
  case SY::DYNAMIC_VARIABLE:
    return SY::TEMPLATE_DYNAMIC_VARIABLE;
  case SY::STATIC_VARIABLE:
    return SY::TEMPLATE_STATIC_VARIABLE;
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
  case SY::PARTIAL_DYNAMIC_VARIABLE:
    return SY::TEMPLATE_DYNAMIC_VARIABLE;
  case SY::PARTIAL_STATIC_VARIABLE:
    return SY::TEMPLATE_STATIC_VARIABLE;
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
  case SY::TEMPLATE_DYNAMIC_VARIABLE:
    return SY::TEMPLATE_DYNAMIC_VARIABLE;
  case SY::TEMPLATE_STATIC_VARIABLE:
    return SY::TEMPLATE_STATIC_VARIABLE;
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
  case SY::DYNAMIC_VARIABLE:
    return SY::PARTIAL_DYNAMIC_VARIABLE;
  case SY::STATIC_VARIABLE:
    return SY::PARTIAL_STATIC_VARIABLE;
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
  case SY::TEMPLATE_DYNAMIC_VARIABLE:
    return SY::PARTIAL_DYNAMIC_VARIABLE;
  case SY::TEMPLATE_STATIC_VARIABLE:
    return SY::PARTIAL_STATIC_VARIABLE;
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
  case SY::PARTIAL_DYNAMIC_VARIABLE:
    return SY::PARTIAL_DYNAMIC_VARIABLE;
  case SY::PARTIAL_STATIC_VARIABLE:
    return SY::PARTIAL_STATIC_VARIABLE;
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
  case SY::DYNAMIC_VARIABLE:
    return SY::DYNAMIC_VARIABLE;
  case SY::STATIC_VARIABLE:
    return SY::STATIC_VARIABLE;
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
  case SY::TEMPLATE_DYNAMIC_VARIABLE:
    return SY::DYNAMIC_VARIABLE;
  case SY::TEMPLATE_STATIC_VARIABLE:
    return SY::STATIC_VARIABLE;
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
  case SY::PARTIAL_DYNAMIC_VARIABLE:
    return SY::DYNAMIC_VARIABLE;
  case SY::PARTIAL_STATIC_VARIABLE:
    return SY::STATIC_VARIABLE;
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
struct SimpleBuiltinSymbol;
struct InferenceSymbol;
struct VoidSymbol;
struct NullSymbol;
struct NoReturnSymbol;
struct VariadicArgumentsSymbol;
struct BooleanSymbol;
struct AsciiSymbol;
struct Utf8Symbol;

// SCALED INTEGER
struct ScaledBuiltinSymbol;
struct IntegerSymbol;
struct SignedSymbol;
struct UnsignedSymbol;
struct FloatSymbol;
struct BinarySymbol;
struct BfloatSymbol;

// UNARY SUBTYPE
struct UnarySubtypeSymbol;
struct RangeSymbol;
struct ReferenceSymbol;
struct PointerSymbol;
struct FatPointerSymbol;
struct InferencedCountArraySymbol;

// COUNTED SUBTYPE
struct CountedSubtypeSymbol;
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
struct ImportSymbol;
struct FacadeSymbol;
struct MutationSymbol;

// BINDING
struct DynamicVariableSymbol;
struct StaticVariableSymbol;
struct EnumeratorSymbol;
struct PropertySymbol;
struct ClassParameterSymbol;
struct LayoutParameterSymbol;
struct TemplateParameterSymbol;
struct SignatureParameterSymbol;

// SYMBOL TABLES
struct SymbolTableSymbol;
struct TopSymbol;
struct TableSymbol;
struct ClassSymbol;
struct EnumerationSymbol;
struct LabelSymbol;

// PROCEDURE
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
struct TemplateDynamicVariableSymbol;
struct TemplateStaticVariableSymbol;
struct TemplateFunctionSymbol;
struct TemplateMethodSymbol;
struct TemplateExtensionFunctionSymbol;
struct TemplateExtensionMethodSymbol;
struct TemplateConstructorSymbol;

// PARTIAL SPECIALIZATION
struct PartialSpecializationSymbol;
struct PartialClassSymbol;
struct PartialEnumerationSymbol;
struct PartialDynamicVariableSymbol;
struct PartialStaticVariableSymbol;
struct PartialFunctionSymbol;
struct PartialMethodSymbol;
struct PartialExtensionFunctionSymbol;
struct PartialExtensionMethodSymbol;
struct PartialConstructorSymbol;

struct Symbol {
  using Self = rq::Symbol;

  rq::SymbolKind _kind;

protected:
  Symbol(rq::SymbolKind kind) : _kind(kind) {}

public:
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAscii() const {
    return this->_kind == rq::SymbolKind::ASCII;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUtf8() const {
    return this->_kind == rq::SymbolKind::UTF8;
  }

  // SCALED BUILTIN
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericInteger() const {
    return this->_kind == rq::SymbolKind::GENERIC_INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return this->_kind == rq::SymbolKind::SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return this->_kind == rq::SymbolKind::UNSIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericFloat() const {
    return this->_kind == rq::SymbolKind::GENERIC_FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary() const {
    return this->_kind == rq::SymbolKind::BINARY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloat() const {
    return this->_kind == rq::SymbolKind::BFLOAT;
  }

  // UNARY SUBTYPE
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype() const {
    return rq::getIsUnarySubtype(this->_kind);
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImport() const {
    return this->_kind == rq::SymbolKind::IMPORT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMutation() const {
    return this->_kind == rq::SymbolKind::MUTATION;
  }

  // BINDING
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTableMember() const {
    return rq::getIsSymbolTableMember(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMaybeDefaultValued() const {
    return rq::getIsMaybeDefaultValued(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariable() const {
    return this->_kind == rq::SymbolKind::DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticVariable() const {
    return this->_kind == rq::SymbolKind::STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumerator() const {
    return this->_kind == rq::SymbolKind::ENUMERATOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProperty() const {
    return this->_kind == rq::SymbolKind::PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassParameter() const {
    return this->_kind == rq::SymbolKind::CLASS_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutParameter() const {
    return this->_kind == rq::SymbolKind::LAYOUT_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateParameter() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureParameter() const {
    return this->_kind == rq::SymbolKind::SIGNATURE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFacade() const {
    return this->_kind == rq::SymbolKind::FACADE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLabel() const {
    return this->_kind == rq::SymbolKind::LABEL;
  }

  // SYMBOL TABLES
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTop() const {
    return this->_kind == rq::SymbolKind::TOP;
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
    return this->_kind == rq::SymbolKind::TEMPLATE_DYNAMIC_VARIABLE ||
           this->_kind == rq::SymbolKind::TEMPLATE_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateDynamicVariable() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateStaticVariable() const {
    return this->_kind == rq::SymbolKind::TEMPLATE_STATIC_VARIABLE;
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
    return this->_kind == rq::SymbolKind::PARTIAL_DYNAMIC_VARIABLE ||
           this->_kind == rq::SymbolKind::PARTIAL_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialDynamicVariable() const {
    return this->_kind == rq::SymbolKind::PARTIAL_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialStaticVariable() const {
    return this->_kind == rq::SymbolKind::PARTIAL_STATIC_VARIABLE;
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const {
    return rq::getHasTemplateAlternative(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsInteger(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin() const {
    return rq::getIsScaledBuiltin(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunit(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNamed() const {
    return rq::getIsNamed(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getMaybeHasName() const {
    return rq::getMaybeHasName(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocation() const {
    return rq::getHasLocation(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModuleMember() const {
    return rq::getIsModuleMember(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMaybeClassMember() const {
    return rq::getIsMaybeClassMember(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLayout() const {
    return rq::getHasLayout(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSignature() const {
    return rq::getHasSignature(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateLayout() const {
    return rq::getHasTemplateLayout(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPositionalEntries() const {
    return rq::getHasPositionalEntries(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStaticValue() const {
    return rq::getHasStaticValue(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAttributes() const {
    return rq::getHasAttributes(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const {
    return rq::getHasType(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable() const {
    return rq::getIsSymbolTable(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasImportModule() const {
    return rq::getHasImportModule(this->_kind);
  }
};

struct SimpleBuiltinSymbol : public rq::Symbol {
  using Self = rq::SimpleBuiltinSymbol;

protected:
  SimpleBuiltinSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {
    RQ_ASSERT(rq::getIsSimpleBuiltin(kind), "kind not builtin simple symbol");
  }

public:
  SimpleBuiltinSymbol(const Self &) = delete;
  SimpleBuiltinSymbol(Self &&) = delete;
  virtual ~SimpleBuiltinSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

static constexpr unsigned MAX_SCALED_BUILTIN_SCALAR =
    std::numeric_limits<std::uint16_t>::max();

static constexpr unsigned MAX_SCALED_BUILTIN_UID =
    std::numeric_limits<std::uint16_t>::max();

enum class ScaledBuiltinFlags : std::uint8_t {
  NONE,
  BYTES = rq::getBit(0),
  INDEX = rq::getBit(1),
  ADDRESS = rq::getBit(2),
  BITS_NONE_MASK = BYTES | INDEX | ADDRESS,
  FASTEST = rq::getBit(3),
  LEAST = rq::getBit(4),
  BEST = rq::getBit(5),
  EXACT_NONE_MASK = FASTEST | LEAST | BEST,
  LITTLE_ENDIAN_ = rq::getBit(6),
  BIG_ENDIAN_ = rq::getBit(7),
  PLATFORM_ENDIAN_NONE_MASK = LITTLE_ENDIAN_ | BIG_ENDIAN_
};

template <> struct is_flags<rq::ScaledBuiltinFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBitsScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags, rq::ScaledBuiltinFlags::BITS_NONE_MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasBytesScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::BYTES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasIndexScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::INDEX);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAddressScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::ADDRESS);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags, rq::ScaledBuiltinFlags::EXACT_NONE_MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::FASTEST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::LEAST);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasNone(flags,
                        rq::ScaledBuiltinFlags::PLATFORM_ENDIAN_NONE_MASK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsBigEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::BIG_ENDIAN_);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsLittleEndian(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::LITTLE_ENDIAN_);
}

void RQ_ALWAYS_INLINE profileScaledIntegerSymbol(llvm::FoldingSetNodeID &id,
                                                 rq::SymbolKind kind,
                                                 std::uint16_t scalar,
                                                 std::uint16_t uid,
                                                 rq::ScaledBuiltinFlags flags) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(static_cast<unsigned>(scalar));
  id.AddInteger(static_cast<unsigned>(uid));
  id.AddInteger(static_cast<unsigned>(flags));
}

struct ScaledBuiltinSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledBuiltinSymbol;
  friend struct Context;

  std::uint16_t _scalar;
  std::uint16_t _uid;
  rq::ScaledBuiltinFlags _flags;

protected:
  ScaledBuiltinSymbol(rq::SymbolKind kind, unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags)
      : rq::Symbol(kind), _scalar(scalar), _uid(uid), _flags(flags) {
    RQ_ASSERT(rq::getIsScaledBuiltin(kind), "kind not scaled builtin symbol");
    RQ_ASSERT(scalar < rq::MAX_SCALED_BUILTIN_SCALAR, "scalar too large");
    RQ_ASSERT(uid < rq::MAX_SCALED_BUILTIN_UID, "uid too large");
  }

public:
  ScaledBuiltinSymbol(const Self &) = delete;
  ScaledBuiltinSymbol(Self &&) = delete;
  virtual ~ScaledBuiltinSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getScalar() const {
    return this->_scalar;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE std::uint16_t getUid() const {
    return this->_uid;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ScaledBuiltinFlags getFlags() const {
    return this->_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBytesScalar() const {
    return rq::getHasBytesScalar(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndexScalar() const {
    return rq::getHasIndexScalar(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddressScalar() const {
    return rq::getHasAddressScalar(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExact() const {
    return rq::getIsExact(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFastest() const {
    return rq::getIsFastest(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLeast() const {
    return rq::getIsLeast(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformEndian() const {
    return rq::getIsPlatformEndian(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBigEndian() const {
    return rq::getIsBigEndian(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLittleEndian() const {
    return rq::getIsLittleEndian(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnique() const {
    return this->_uid != 0;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileScaledIntegerSymbol(id, this->_kind, this->_scalar, this->_uid,
                                   this->_flags);
  }
};

struct IntegerSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::IntegerSymbol;

public:
  IntegerSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(rq::SymbolKind::GENERIC_INTEGER, scalar, uid,
                                flags) {}

protected:
  IntegerSymbol(rq::SymbolKind kind, unsigned scalar, unsigned uid,
                rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}

public:
  IntegerSymbol(const Self &) = delete;
  IntegerSymbol(Self &&) = delete;
  virtual ~IntegerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FloatSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::FloatSymbol;

public:
  FloatSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(rq::SymbolKind::GENERIC_FLOAT, scalar, uid,
                                flags) {}

protected:
  FloatSymbol(rq::SymbolKind kind, unsigned scalar, unsigned uid,
              rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}

public:
  FloatSymbol(const Self &) = delete;
  FloatSymbol(Self &&) = delete;
  virtual ~FloatSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

void RQ_ALWAYS_INLINE profileUnarySubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                rq::SymbolKind kind,
                                                const rq::Symbol &root) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&root);
}

struct UnarySubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtypeSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr{nullptr};

protected:
  UnarySubtypeSymbol(rq::SymbolKind kind, rq::Symbol &root)
      : rq::Symbol(kind), _root_ptr(&root) {
    RQ_ASSERT(rq::getIsUnarySubtype(kind), "kind not unary subtype symbol");
    RQ_ASSERT(root.getIsRoot(), "not root");
  }

public:
  UnarySubtypeSymbol(const Self &) = delete;
  UnarySubtypeSymbol(Self &&) = delete;
  virtual ~UnarySubtypeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getRoot() {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileUnarySubtypeSymbol(id, this->getKind(), this->getRoot());
  }
};

void RQ_ALWAYS_INLINE profileCountedSubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                  rq::SymbolKind kind,
                                                  const rq::Symbol &root,
                                                  unsigned count) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&root);
  id.AddInteger(count);
}

struct CountedSubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::CountedSubtypeSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr;
  unsigned _count;

protected:
  CountedSubtypeSymbol(rq::SymbolKind kind, rq::Symbol &root, unsigned count)
      : rq::Symbol(kind), _root_ptr(&root), _count(count) {
    RQ_ASSERT(rq::getIsCountedSubtype(kind), "not counted subtype");
    RQ_ASSERT(root.getIsRoot(), "not root");
  }

public:
  CountedSubtypeSymbol(const Self &) = delete;
  CountedSubtypeSymbol(Self &&) = delete;
  virtual ~CountedSubtypeSymbol() {}
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
    rq::profileCountedSubtypeSymbol(id, this->getKind(), this->getRoot(),
                                    this->getCount());
  }
};

void RQ_ALWAYS_INLINE profileArithmeticSequenceSymbol(
    llvm::FoldingSetNodeID &id, const rq::Symbol &root,
    rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  // no need to fold kind
  id.AddPointer(&root);
  id.AddInteger(static_cast<unsigned>(condition));
  id.AddInteger(static_cast<unsigned>(step));
}

struct ArithmeticSequenceSymbol : public rq::Symbol,
                                  public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequenceSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

protected:
  ArithmeticSequenceSymbol(rq::SymbolKind kind, rq::Symbol &root,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step)
      : rq::Symbol(kind), _root_ptr(&root), _condition(condition), _step(step) {
    RQ_ASSERT(root.getIsRoot(), "not root");
    RQ_ASSERT(rq::getIsArithmeticSequence(kind),
              "kind not arithmetic sequence symbol");
  }

public:
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
    rq::profileArithmeticSequenceSymbol(id, this->getRoot(),
                                        this->getCondition(), this->getStep());
  }
};

namespace detail {
struct HasLocationSymbol {
  using Self = rq::detail::HasLocationSymbol;

  rq::Expression *_expression_ptr;

protected:
  HasLocationSymbol(rq::Expression &expression)
      : _expression_ptr(&expression) {}

public:
  HasLocationSymbol() : _expression_ptr(nullptr) {}
  HasLocationSymbol(const Self &) = delete;
  HasLocationSymbol(Self &&) = delete;
  virtual ~HasLocationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
};
struct ModuleMemberSymbol {
  using Self = rq::detail::ModuleMemberSymbol;

  rq::ModuleSymbol *_module_ptr;

protected:
  ModuleMemberSymbol(rq::ModuleSymbol &module) : _module_ptr(&module) {}

public:
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
struct SymbolTableMemberSymbol {
  using Self = rq::detail::SymbolTableMemberSymbol;

  rq::SymbolTableSymbol *_scope_ptr;

protected:
  SymbolTableMemberSymbol(rq::SymbolTableSymbol &scope) : _scope_ptr(&scope) {}

public:
  SymbolTableMemberSymbol(const Self &) = delete;
  SymbolTableMemberSymbol(Self &&) = delete;
  virtual ~SymbolTableMemberSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &getScope() const {
    return rq::dereferencePtr(this->_scope_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &getScope() {
    return rq::dereferencePtr(this->_scope_ptr);
  }
};
struct HasAttributesSymbol {
  using Self = HasAttributesSymbol;

  rq::SymbolAttributeFlags _attributes;

protected:
  HasAttributesSymbol(rq::SymbolAttributeFlags attributes)
      : _attributes(attributes) {}

public:
  HasAttributesSymbol(const Self &) = delete;
  HasAttributesSymbol(Self &&) = delete;
  virtual ~HasAttributesSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolAttributeFlags
  getSymbolAttributes() const {
    return this->_attributes;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque() const {
    return rq::getHasOpaque(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOutside() const {
    return rq::getHasOutside(this->_attributes);
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::SymbolAttribute attribute) const {
    return rq::getHasAttribute(this->_attributes, attribute);
  }
};
struct HasNameSymbol {
  using Self = rq::detail::HasNameSymbol;

  llvm::StringRef _name;

protected:
  HasNameSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }

public:
  HasNameSymbol(const Self &) = delete;
  HasNameSymbol(Self &&) = delete;
  virtual ~HasNameSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct MaybeHasNameSymbol {
  using Self = rq::detail::MaybeHasNameSymbol;

  llvm::StringRef _name{};

protected:
  MaybeHasNameSymbol() : _name() {}
  MaybeHasNameSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }

public:
  MaybeHasNameSymbol(const Self &) = delete;
  MaybeHasNameSymbol(Self &&) = delete;
  virtual ~MaybeHasNameSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const {
    return !this->_name.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct HasTypeSymbol {
  using Self = rq::detail::HasTypeSymbol;

  rq::TypeSymbol *_type_ptr{nullptr};

protected:
  HasTypeSymbol() {}

public:
  HasTypeSymbol(const Self &) = delete;
  HasTypeSymbol(Self &&) = delete;
  virtual ~HasTypeSymbol() {}
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
struct HasImportModuleSymbol {
  using Self = rq::detail::HasImportModuleSymbol;

  rq::ModuleSymbol *_import_module_ptr{nullptr};

protected:
  HasImportModuleSymbol() {}

public:
  HasImportModuleSymbol(const Self &) = delete;
  HasImportModuleSymbol(Self &&) = delete;
  virtual ~HasImportModuleSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasImportModule() const {
    return this->_import_module_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ModuleSymbol &
  getImportModule() const {
    return rq::dereferencePtr(this->_import_module_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleSymbol &getImportModule() {
    return rq::dereferencePtr(this->_import_module_ptr);
  }
  RQ_ALWAYS_INLINE void setImportModule(rq::ModuleSymbol &import_module) {
    rq::assignSingleValue(this->_import_module_ptr, &import_module);
  }
};
} // namespace detail

struct SymbolTableSymbol : public rq::Symbol {
  using Self = rq::SymbolTableSymbol;

  llvm::SmallDenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_values{};
  rq::BumpPtrList<rq::Symbol> _unamed_values{};

protected:
  SymbolTableSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {}

public:
  SymbolTableSymbol(const Self &) = delete;
  SymbolTableSymbol(Self &&) = delete;
  ~SymbolTableSymbol() override {
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
  void tabulateNamedSymbol(rq::Context &context, llvm::StringRef name,
                           rq::Symbol &symbol);
  void tabulateUnamedSymbol(rq::Context &context, rq::Symbol &symbol);
  [[nodiscard]] inline rq::BumpPtrList<rq::Symbol>
  getNamedList(llvm::StringRef name) {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::BumpPtrList<rq::Symbol>();
  }
  [[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol>
  getNamedList(llvm::StringRef name) const {
    auto it = this->_named_values.find(name);
    if (it != this->_named_values.end()) {
      return it->second;
    }
    return rq::ConstBumpPtrList<rq::Symbol>();
  }
  [[nodiscard]] inline rq::BumpPtrList<rq::Symbol> getUnnamedList() {
    return this->_unamed_values;
  }
  [[nodiscard]] inline rq::ConstBumpPtrList<rq::Symbol> getUnnamedList() const {
    return this->_unamed_values;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedListRange() {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getNamedListRange() const {
    return std::ranges::subrange(this->_named_values.begin(),
                                 this->_named_values.end());
  }
};

struct ProcedureSymbol : public rq::SymbolTableSymbol,
                         public rq::detail::HasLocationSymbol,
                         public rq::detail::ModuleMemberSymbol,
                         public rq::detail::SymbolTableMemberSymbol,
                         public rq::detail::HasAttributesSymbol {
  using Self = rq::ProcedureSymbol;

  rq::SignatureSymbol *_signature_ptr;

protected:
  ProcedureSymbol(rq::SymbolKind kind, rq::Expression &expression,
                  rq::ModuleSymbol &module, rq::SymbolTableSymbol &scope,
                  rq::SymbolAttributeFlags attributes)
      : rq::SymbolTableSymbol(kind), rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes), _signature_ptr(nullptr) {}

public:
  ProcedureSymbol(const Self &) = delete;
  ProcedureSymbol(Self &&) = delete;
  virtual ~ProcedureSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateSymbol : public rq::Symbol {
  using Self = rq::TemplateSymbol;

  rq::BumpPtrList<rq::TemplateParameterSymbol> _template_parameters;

protected:
  TemplateSymbol(rq::SymbolKind kind,
                 const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::Symbol(kind), _template_parameters(parameters) {}

public:
  TemplateSymbol(const Self &) = delete;
  TemplateSymbol(Self &&) = delete;
  virtual ~TemplateSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialSpecializationSymbol : public rq::Symbol {
  using Self = rq::PartialSpecializationSymbol;

protected:
  PartialSpecializationSymbol(rq::SymbolKind kind) : rq::Symbol(kind) {}

public:
  PartialSpecializationSymbol(const Self &) = delete;
  PartialSpecializationSymbol(Self &&) = delete;
  virtual ~PartialSpecializationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace rq
namespace llvm {

// ROOT WITH TYPE ATTRIBUTES
template <> struct isa_impl<rq::TypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsType(); }
};

// SIMPLE BUILTIN
template <> struct isa_impl<rq::SimpleBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSimpleBuiltin();
  }
};

template <> struct isa_impl<rq::InferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInference();
  }
};

template <> struct isa_impl<rq::InferenceSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsInference();
  }
};

template <> struct isa_impl<rq::VoidSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsVoid(); }
};

template <> struct isa_impl<rq::VoidSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsVoid();
  }
};

template <> struct isa_impl<rq::NullSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsNull(); }
};

template <> struct isa_impl<rq::NullSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsNull();
  }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsNoReturn(); }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsNoReturn();
  }
};

template <> struct isa_impl<rq::VariadicArgumentsSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsVariadicArguments();
  }
};

template <>
struct isa_impl<rq::VariadicArgumentsSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsVariadicArguments();
  }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBoolean(); }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBoolean();
  }
};

template <> struct isa_impl<rq::AsciiSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsAscii(); }
};

template <> struct isa_impl<rq::AsciiSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsAscii();
  }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsUtf8(); }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsUtf8();
  }
};

// SCALED BUILTIN
template <> struct isa_impl<rq::ScaledBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledBuiltin();
  }
};

template <> struct isa_impl<rq::IntegerSymbol, rq::Symbol> {
  static inline bool doit(const rq::IntegerSymbol &val) {
    return val.getIsInteger();
  }
};

template <> struct isa_impl<rq::IntegerSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsInteger();
  }
};

template <> struct isa_impl<rq::SignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsSigned(); }
};

template <> struct isa_impl<rq::SignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsSigned();
  }
};

template <> struct isa_impl<rq::SignedSymbol, rq::IntegerSymbol> {
  static inline bool doit(const rq::IntegerSymbol &val) {
    return val.getIsSigned();
  }
};

template <> struct isa_impl<rq::UnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsUnsigned(); }
};

template <> struct isa_impl<rq::UnsignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsUnsigned();
  }
};

template <> struct isa_impl<rq::UnsignedSymbol, rq::IntegerSymbol> {
  static inline bool doit(const rq::IntegerSymbol &val) {
    return val.getIsSigned();
  }
};

template <> struct isa_impl<rq::FloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsFloat(); }
};

template <> struct isa_impl<rq::FloatSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsFloat();
  }
};

template <> struct isa_impl<rq::BinarySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary(); }
};

template <> struct isa_impl<rq::BinarySymbol, rq::FloatSymbol> {
  static inline bool doit(const rq::FloatSymbol &val) {
    return val.getIsBinary();
  }
};

template <> struct isa_impl<rq::BinarySymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsBinary();
  }
};

template <> struct isa_impl<rq::BfloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBfloat(); }
};

template <> struct isa_impl<rq::BfloatSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsBfloat();
  }
};

template <> struct isa_impl<rq::BfloatSymbol, rq::FloatSymbol> {
  static inline bool doit(const rq::FloatSymbol &val) {
    return val.getIsBfloat();
  }
};

// UNARY SUBTYPE
template <> struct isa_impl<rq::UnarySubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsUnarySubtype();
  }
};

template <> struct isa_impl<rq::RangeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsRange(); }
};

template <> struct isa_impl<rq::RangeSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsRange();
  }
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsReference();
  }
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsReference();
  }
};

template <> struct isa_impl<rq::PointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsPointer(); }
};

template <> struct isa_impl<rq::PointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsPointer();
  }
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFatPointer();
  }
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsFatPointer();
  }
};

template <> struct isa_impl<rq::InferencedCountArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInferencedCountArray();
  }
};

template <>
struct isa_impl<rq::InferencedCountArraySymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsInferencedCountArray();
  }
};

// COUNTED SUBTYPE
template <> struct isa_impl<rq::CountedSubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsCountedSubtype();
  }
};

template <> struct isa_impl<rq::ArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsArray(); }
};

template <> struct isa_impl<rq::ArraySymbol, rq::CountedSubtypeSymbol> {
  static inline bool doit(const rq::CountedSubtypeSymbol &val) {
    return val.getIsArray();
  }
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
struct isa_impl<rq::ArithmeticIntervalSymbol, rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsArithmeticInterval();
  }
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInfiniteArithmeticProgression();
  }
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsInfiniteArithmeticProgression();
  }
};

template <> struct isa_impl<rq::FiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFiniteArithmeticProgression();
  }
};

template <>
struct isa_impl<rq::FiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsFiniteArithmeticProgression();
  }
};

// MISC
template <> struct isa_impl<rq::ModuleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsModule(); }
};

template <> struct isa_impl<rq::ImportSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsImport(); }
};

template <> struct isa_impl<rq::FacadeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsFacade(); }
};

template <> struct isa_impl<rq::MutationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsMutation(); }
};

// BINDING
template <> struct isa_impl<rq::DynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDynamicVariable();
  }
};

template <> struct isa_impl<rq::StaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsStaticVariable();
  }
};

template <> struct isa_impl<rq::EnumeratorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumerator();
  }
};

template <> struct isa_impl<rq::PropertySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsProperty(); }
};

template <> struct isa_impl<rq::TemplateParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateParameter();
  }
};

template <> struct isa_impl<rq::SignatureParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSignatureParameter();
  }
};

template <> struct isa_impl<rq::ClassParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsClassParameter();
  }
};

template <> struct isa_impl<rq::LayoutParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsLayoutParameter();
  }
};

template <> struct isa_impl<rq::LabelSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsLabel(); }
};

// SYMBOL TABLES
template <> struct isa_impl<rq::SymbolTableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSymbolTable();
  }
};

template <> struct isa_impl<rq::TopSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsTop(); }
};

template <> struct isa_impl<rq::TopSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsTop();
  }
};

template <> struct isa_impl<rq::TableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsTable(); }
};

template <> struct isa_impl<rq::TableSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsTable();
  }
};

template <> struct isa_impl<rq::ClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsClass(); }
};

template <> struct isa_impl<rq::ClassSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsClass();
  }
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumeration();
  }
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
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
  static inline bool doit(const rq::Symbol &val) { return val.getIsEntry(); }
};

template <> struct isa_impl<rq::EntrySymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsEntry();
  }
};

template <> struct isa_impl<rq::FunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsFunction(); }
};

template <> struct isa_impl<rq::FunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsFunction();
  }
};

template <> struct isa_impl<rq::MethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsMethod(); }
};

template <> struct isa_impl<rq::MethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsMethod();
  }
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionFunction();
  }
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsExtensionFunction();
  }
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionMethod();
  }
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsExtensionMethod();
  }
};

template <> struct isa_impl<rq::ConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsConstructor();
  }
};

template <> struct isa_impl<rq::ConstructorSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsConstructor();
  }
};

template <> struct isa_impl<rq::DestructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDestructor();
  }
};

template <> struct isa_impl<rq::DestructorSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsDestructor();
  }
};

template <> struct isa_impl<rq::RangerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsRanger(); }
};

template <> struct isa_impl<rq::RangerSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsRanger();
  }
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

template <> struct isa_impl<rq::TemplateClassSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateClass();
  }
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateEnumeration();
  }
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateEnumeration();
  }
};

template <> struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateDynamicVariable();
  }
};

template <>
struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateDynamicVariable();
  }
};

template <> struct isa_impl<rq::TemplateStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateStaticVariable();
  }
};

template <>
struct isa_impl<rq::TemplateStaticVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateStaticVariable();
  }
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateFunction();
  }
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateFunction();
  }
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateMethod();
  }
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateMethod();
  }
};

template <> struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionFunction();
  }
};

template <>
struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateExtensionFunction();
  }
};

template <> struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionMethod();
  }
};

template <>
struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateExtensionMethod();
  }
};

template <> struct isa_impl<rq::TemplateConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateConstructor();
  }
};

template <> struct isa_impl<rq::TemplateConstructorSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
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

template <>
struct isa_impl<rq::PartialClassSymbol, rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialClass();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialEnumeration();
  }
};

template <>
struct isa_impl<rq::PartialEnumerationSymbol, rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialEnumeration();
  }
};

template <> struct isa_impl<rq::PartialDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialDynamicVariable();
  }
};

template <>
struct isa_impl<rq::PartialDynamicVariableSymbol,
                rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialDynamicVariable();
  }
};

template <> struct isa_impl<rq::PartialStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialStaticVariable();
  }
};

template <>
struct isa_impl<rq::PartialStaticVariableSymbol,
                rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialStaticVariable();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialFunction();
  }
};

template <>
struct isa_impl<rq::PartialFunctionSymbol, rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialFunction();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialMethod();
  }
};

template <>
struct isa_impl<rq::PartialMethodSymbol, rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialMethod();
  }
};

template <> struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionFunction();
  }
};

template <>
struct isa_impl<rq::PartialExtensionFunctionSymbol,
                rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialExtensionFunction();
  }
};

template <> struct isa_impl<rq::PartialExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionMethod();
  }
};

template <>
struct isa_impl<rq::PartialExtensionMethodSymbol,
                rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialExtensionMethod();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialConstructor();
  }
};

template <>
struct isa_impl<rq::PartialConstructorSymbol, rq::PartialSpecializationSymbol> {
  static inline bool doit(const rq::PartialSpecializationSymbol &val) {
    return val.getIsPartialConstructor();
  }
};

} // namespace llvm
namespace rq {

struct TypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::TypeSymbol;

  rq::Symbol *_root_ptr{nullptr};
  rq::TypeAttributeFlags _flags{};

  TypeSymbol(rq::SymbolKind kind, rq::Symbol &root,
             rq::TypeAttributeFlags flags)
      : rq::Symbol(kind), _root_ptr(&root), _flags(flags) {}
  TypeSymbol(const Self &) = delete;
  TypeSymbol(Self &&) = delete;
  virtual ~TypeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getRoot() const {
    return rq::dereferencePtr(this->_root_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttributeFlags getFlags() const {
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

struct AsciiSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;

  AsciiSymbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::ASCII) {}
  AsciiSymbol(const Self &) = delete;
  AsciiSymbol(Self &&) = delete;
  virtual ~AsciiSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Utf8Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;
  friend struct Context;

private:
  Utf8Symbol() : rq::SimpleBuiltinSymbol(rq::SymbolKind::UTF8) {}
public:
  Utf8Symbol(const Self &) = delete;
  Utf8Symbol(Self &&) = delete;
  virtual ~Utf8Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct UnsignedSymbol : public rq::IntegerSymbol {
  using Self = rq::IntegerSymbol;
  friend struct Context;

private:
  UnsignedSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags)
      : rq::IntegerSymbol(rq::SymbolKind::UNSIGNED, scalar, uid, flags) {}

public:
  UnsignedSymbol(const Self &) = delete;
  UnsignedSymbol(Self &&) = delete;
  virtual ~UnsignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignedSymbol : public rq::IntegerSymbol {
  using Self = rq::SignedSymbol;
  friend struct Context;

private:
  SignedSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags)
      : rq::IntegerSymbol(rq::SymbolKind::SIGNED, scalar, uid, flags) {}

public:
  SignedSymbol(const Self &) = delete;
  SignedSymbol(Self &&) = delete;
  virtual ~SignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BinarySymbol : public rq::FloatSymbol {
  using Self = rq::BinarySymbol;
  friend struct Context;

private:
  BinarySymbol(unsigned scalar, std::uint16_t uid, rq::ScaledBuiltinFlags flags)
      : rq::FloatSymbol(rq::SymbolKind::BINARY, scalar, uid, flags) {}

public:
  BinarySymbol(const Self &) = delete;
  BinarySymbol(Self &&) = delete;
  virtual ~BinarySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BfloatSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::FloatSymbol;
  friend struct Context;

private:
  BfloatSymbol(unsigned scalar, rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(rq::SymbolKind::BFLOAT, scalar, 0, flags) {}

public:
  BfloatSymbol(const Self &) = delete;
  BfloatSymbol(Self &&) = delete;
  virtual ~BfloatSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangeSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::RangeSymbol;
  friend struct Context;

private:
  RangeSymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::SymbolKind::RANGE, root) {}

public:
  RangeSymbol(const Self &) = delete;
  RangeSymbol(Self &&) = delete;
  virtual ~RangeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ReferenceSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::ReferenceSymbol;
  friend struct Context;

private:
  ReferenceSymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::SymbolKind::REFERENCE, root) {}

public:
  ReferenceSymbol(const Self &) = delete;
  ReferenceSymbol(Self &&) = delete;
  virtual ~ReferenceSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PointerSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::PointerSymbol;
  friend struct Context;

private:
  PointerSymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::SymbolKind::POINTER, root) {}

public:
  PointerSymbol(const Self &) = delete;
  PointerSymbol(Self &&) = delete;
  virtual ~PointerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FatPointerSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::FatPointerSymbol;
  friend struct Context;

private:
  FatPointerSymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::SymbolKind::FAT_POINTER, root) {}

public:
  FatPointerSymbol(const Self &) = delete;
  FatPointerSymbol(Self &&) = delete;
  virtual ~FatPointerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct InferencedCountArraySymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::InferencedCountArraySymbol;
  friend struct Context;

private:
  InferencedCountArraySymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::SymbolKind::INFERENCED_COUNT_ARRAY, root) {}

public:
  InferencedCountArraySymbol(const Self &) = delete;
  InferencedCountArraySymbol(Self &&) = delete;
  virtual ~InferencedCountArraySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ArraySymbol : public rq::CountedSubtypeSymbol {
  using Self = rq::ArraySymbol;
  friend struct Context;

private:
  ArraySymbol(rq::Symbol &root, unsigned count)
      : rq::CountedSubtypeSymbol(rq::SymbolKind::ARRAY, root, count) {}

public:
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
};

struct LayoutSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::LayoutSymbol;
  friend struct Context;

  rq::BumpPtrList<rq::Symbol> _properties;

private:
  LayoutSymbol(rq::BumpPtrList<rq::Symbol> properties)
      : rq::Symbol(rq::SymbolKind::LAYOUT), _properties(properties) {}

public:
  LayoutSymbol(const Self &) = delete;
  LayoutSymbol(Self &&) = delete;
  virtual ~LayoutSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Symbol>
  getProperties() const {
    return this->_properties;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddInteger(static_cast<unsigned>(this->_kind));
  }
};

struct SignatureSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::SignatureSymbol;
  friend struct Context;

  rq::Symbol *_return_ptr;
  rq::BumpPtrList<rq::Symbol> _parameters;

private:
  SignatureSymbol(rq::Symbol &return_, rq::BumpPtrList<rq::Symbol> parameters)
      : rq::Symbol(rq::SymbolKind::SIGNATURE), _return_ptr(&return_),
        _parameters(parameters) {}

public:
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Symbol>
  getParameters() const {
    return this->_parameters;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_return_ptr);
    id.AddInteger(static_cast<unsigned>(this->_kind));
  }
};

struct ExtensionSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ExtensionSymbol;
  friend struct Context;

  rq::SignatureSymbol *_signature_ptr{nullptr};
  rq::TypeSymbol *_type_ptr{nullptr};

private:
  ExtensionSymbol(rq::SignatureSymbol &signature, rq::TypeSymbol &type)
      : rq::Symbol(rq::SymbolKind::EXTENSION), _signature_ptr(&signature),
        _type_ptr(&type) {}

public:
  ExtensionSymbol(const Self &) = delete;
  ExtensionSymbol(Self &&) = delete;
  virtual ~ExtensionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SignatureSymbol &getSignature() {
    return rq::dereferencePtr(this->_signature_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SignatureSymbol &
  getSignature() const {
    return rq::dereferencePtr(this->_signature_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeSymbol &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeSymbol &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    id.AddPointer(this->_signature_ptr);
    id.AddPointer(this->_type_ptr);
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

struct TopSymbol : rq::SymbolTableSymbol {
  using Self = rq::TopSymbol;

  TopSymbol() : rq::SymbolTableSymbol(rq::SymbolKind::TOP) {}
  TopSymbol(const Self &) = delete;
  TopSymbol(Self &&) = delete;
  ~TopSymbol() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DynamicVariableSymbol : public rq::Symbol,
                               public rq::detail::HasLocationSymbol,
                               public rq::detail::ModuleMemberSymbol,
                               public rq::detail::SymbolTableMemberSymbol,
                               public rq::detail::HasAttributesSymbol,
                               public rq::detail::HasNameSymbol,
                               public rq::detail::HasTypeSymbol {
  using Self = rq::DynamicVariableSymbol;

  DynamicVariableSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &scope, llvm::StringRef name,
                        rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::DYNAMIC_VARIABLE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::HasNameSymbol(name) {}
  DynamicVariableSymbol(const Self &) = delete;
  DynamicVariableSymbol(Self &&) = delete;
  virtual ~DynamicVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct StaticVariableSymbol : public rq::Symbol,
                              public rq::detail::HasLocationSymbol,
                              public rq::detail::ModuleMemberSymbol,
                              public rq::detail::SymbolTableMemberSymbol,
                              public rq::detail::HasAttributesSymbol,
                              public rq::detail::HasNameSymbol,
                              public rq::detail::HasTypeSymbol {
  using Self = rq::StaticVariableSymbol;

  StaticVariableSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                       rq::SymbolTableSymbol &scope, llvm::StringRef name,
                       rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::STATIC_VARIABLE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::HasNameSymbol(name) {}
  StaticVariableSymbol(const Self &) = delete;
  StaticVariableSymbol(Self &&) = delete;
  virtual ~StaticVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumeratorSymbol : public rq::Symbol,
                          public rq::detail::HasLocationSymbol,
                          public rq::detail::ModuleMemberSymbol,
                          public rq::detail::SymbolTableMemberSymbol,
                          public rq::detail::HasAttributesSymbol,
                          public rq::detail::HasNameSymbol,
                          public rq::detail::HasTypeSymbol {
  using Self = rq::EnumeratorSymbol;

  rq::EnumerationSymbol *_enumeration_ptr;

  EnumeratorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                   rq::SymbolTableSymbol &scope, llvm::StringRef name,
                   rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::ENUMERATOR),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::HasNameSymbol(name) {}
  EnumeratorSymbol(const Self &) = delete;
  EnumeratorSymbol(Self &&) = delete;
  virtual ~EnumeratorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PropertySymbol : public rq::Symbol,
                        public rq::detail::HasLocationSymbol,
                        public rq::detail::ModuleMemberSymbol,
                        public rq::detail::SymbolTableMemberSymbol,
                        public rq::detail::HasAttributesSymbol,
                        public rq::detail::MaybeHasNameSymbol,
                        public rq::detail::HasTypeSymbol {
  using Self = rq::PropertySymbol;

  PropertySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &scope,
                 rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::PROPERTY),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  PropertySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &scope, llvm::StringRef name,
                 rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::PROPERTY),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol(name) {}
  PropertySymbol(const Self &) = delete;
  PropertySymbol(Self &&) = delete;
  virtual ~PropertySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SignatureParameterSymbol : public rq::Symbol,
                                  public rq::detail::HasLocationSymbol,
                                  public rq::detail::ModuleMemberSymbol,
                                  public rq::detail::SymbolTableMemberSymbol,
                                  public rq::detail::HasAttributesSymbol,
                                  public rq::detail::MaybeHasNameSymbol,
                                  public rq::detail::HasTypeSymbol {
  using Self = rq::SignatureParameterSymbol;

  rq::SignatureSymbol *_signature_ptr;

  SignatureParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                           rq::SymbolTableSymbol &scope,
                           rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::SIGNATURE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  SignatureParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                           rq::SymbolTableSymbol &scope, llvm::StringRef name,
                           rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::SIGNATURE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol(name) {}
  SignatureParameterSymbol(const Self &) = delete;
  SignatureParameterSymbol(Self &&) = delete;
  virtual ~SignatureParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateParameterSymbol : public rq::Symbol,
                                 public rq::detail::HasLocationSymbol,
                                 public rq::detail::ModuleMemberSymbol,
                                 public rq::detail::SymbolTableMemberSymbol,
                                 public rq::detail::HasAttributesSymbol,
                                 public rq::detail::MaybeHasNameSymbol,
                                 public rq::detail::HasTypeSymbol {
  using Self = rq::TemplateParameterSymbol;

  rq::TemplateSymbol *_template_ptr;

  TemplateParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &scope,
                          rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::TEMPLATE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  TemplateParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &scope, llvm::StringRef name,
                          rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::TEMPLATE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol(name) {}
  TemplateParameterSymbol(const Self &) = delete;
  TemplateParameterSymbol(Self &&) = delete;
  virtual ~TemplateParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassParameterSymbol : public rq::Symbol,
                              public rq::detail::HasLocationSymbol,
                              public rq::detail::ModuleMemberSymbol,
                              public rq::detail::SymbolTableMemberSymbol,
                              public rq::detail::HasAttributesSymbol,
                              public rq::detail::HasTypeSymbol {
  using Self = rq::ClassParameterSymbol;

  ClassParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                       rq::SymbolTableSymbol &scope,
                       rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::CLASS_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes) {}
  ClassParameterSymbol(const Self &) = delete;
  ClassParameterSymbol(Self &&) = delete;
  virtual ~ClassParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LayoutParameterSymbol : public rq::Symbol,
                               public rq::detail::HasLocationSymbol,
                               public rq::detail::ModuleMemberSymbol,
                               public rq::detail::SymbolTableMemberSymbol,
                               public rq::detail::HasAttributesSymbol,
                               public rq::detail::MaybeHasNameSymbol,
                               public rq::detail::HasTypeSymbol {
  using Self = rq::LayoutParameterSymbol;

  LayoutParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &scope,
                        rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::LAYOUT_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  LayoutParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &scope, llvm::StringRef name,
                        rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::LAYOUT_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol(name) {}
  LayoutParameterSymbol(const Self &) = delete;
  LayoutParameterSymbol(Self &&) = delete;
  virtual ~LayoutParameterSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct MutationSymbol : public rq::Symbol,
                        public rq::detail::HasLocationSymbol,
                        public rq::detail::ModuleMemberSymbol,
                        public rq::detail::SymbolTableMemberSymbol,
                        public rq::detail::HasAttributesSymbol {
  using Self = rq::MutationSymbol;

  MutationSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &scope,
                 rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::MUTATION),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes) {}
  MutationSymbol(const Self &) = delete;
  MutationSymbol(Self &&) = delete;
  virtual ~MutationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LabelSymbol : public rq::Symbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol,
                     public rq::detail::SymbolTableMemberSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::LabelSymbol;

  rq::Expression *_statement_ptr;

  LabelSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &scope, llvm::StringRef name,
              rq::Expression &statement)
      : rq::Symbol(rq::SymbolKind::LABEL),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasNameSymbol(name), _statement_ptr(&statement) {}
  LabelSymbol(const Self &) = delete;
  LabelSymbol(Self &&) = delete;
  virtual ~LabelSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getStatement() {
    return rq::dereferencePtr(this->_statement_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getStatement() const {
    return rq::dereferencePtr(this->_statement_ptr);
  }
  RQ_ALWAYS_INLINE void setStatement(rq::Expression &statement) {
    rq::assignSingleValue(this->_statement_ptr, &statement);
  }
};

struct TableSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::TableSymbol;

  TableSymbol(llvm::StringRef name)
      : rq::SymbolTableSymbol(rq::SymbolKind::TABLE),
        rq::detail::HasNameSymbol(name) {}
  TableSymbol(const Self &) = delete;
  TableSymbol(Self &&) = delete;
  virtual ~TableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ClassSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol,
                     public rq::detail::SymbolTableMemberSymbol,
                     public rq::detail::HasAttributesSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::ClassSymbol;

  rq::BumpPtrList<rq::PropertySymbol> _class_properties;

  ClassSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &scope, llvm::StringRef name,
              rq::SymbolAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::SymbolKind::CLASS),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::HasNameSymbol(name) {}
  ClassSymbol(const Self &) = delete;
  ClassSymbol(Self &&) = delete;
  virtual ~ClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EnumerationSymbol : public rq::SymbolTableSymbol,
                           public rq::detail::HasLocationSymbol,
                           public rq::detail::ModuleMemberSymbol,
                           public rq::detail::SymbolTableMemberSymbol,
                           public rq::detail::HasAttributesSymbol,
                           public rq::detail::HasNameSymbol {
  using Self = rq::EnumerationSymbol;

  rq::BumpPtrList<rq::EnumeratorSymbol> _enumerators;

  EnumerationSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                    rq::SymbolTableSymbol &scope, llvm::StringRef name,
                    rq::SymbolAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::SymbolKind::ENUMERATION),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::HasNameSymbol(name) {}
  EnumerationSymbol(const Self &) = delete;
  EnumerationSymbol(Self &&) = delete;
  virtual ~EnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EntrySymbol : public rq::ProcedureSymbol {
  using Self = rq::EntrySymbol;

  EntrySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &scope, rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::ENTRY, expression, module, scope,
                            attributes) {}
  EntrySymbol(const Self &) = delete;
  EntrySymbol(Self &&) = delete;
  virtual ~EntrySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FunctionSymbol : public rq::ProcedureSymbol,
                        public rq::detail::HasNameSymbol {
  using Self = rq::FunctionSymbol;

  FunctionSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &scope, llvm::StringRef name,
                 rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::FUNCTION, expression, module, scope,
                            attributes),
        rq::detail::HasNameSymbol(name) {}
  FunctionSymbol(const Self &) = delete;
  FunctionSymbol(Self &&) = delete;
  virtual ~FunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct MethodSymbol : public rq::ProcedureSymbol,
                      public rq::detail::HasNameSymbol {
  using Self = rq::MethodSymbol;

  MethodSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &scope, llvm::StringRef name,
               rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::METHOD, expression, module, scope,
                            attributes),
        rq::detail::HasNameSymbol(name) {}
  MethodSymbol(const Self &) = delete;
  MethodSymbol(Self &&) = delete;
  virtual ~MethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionFunctionSymbol : public rq::ProcedureSymbol,
                                 public rq::detail::HasNameSymbol {
  using Self = rq::ExtensionFunctionSymbol;

  ExtensionFunctionSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &scope, llvm::StringRef name,
                          rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::EXTENSION_FUNCTION, expression,
                            module, scope, attributes),
        rq::detail::HasNameSymbol(name) {}
  ExtensionFunctionSymbol(const Self &) = delete;
  ExtensionFunctionSymbol(Self &&) = delete;
  virtual ~ExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExtensionMethodSymbol : public rq::ProcedureSymbol,
                               public rq::detail::HasNameSymbol {
  using Self = rq::ExtensionMethodSymbol;

  ExtensionMethodSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &scope, llvm::StringRef name,
                        rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::EXTENSION_METHOD, expression,
                            module, scope, attributes),
        rq::detail::HasNameSymbol(name) {}
  ExtensionMethodSymbol(const Self &) = delete;
  ExtensionMethodSymbol(Self &&) = delete;
  virtual ~ExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ConstructorSymbol : public rq::ProcedureSymbol {
  using Self = rq::ConstructorSymbol;

  ConstructorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                    rq::SymbolTableSymbol &scope,
                    rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::CONSTRUCTOR, expression, module,
                            scope, attributes) {}
  ConstructorSymbol(const Self &) = delete;
  ConstructorSymbol(Self &&) = delete;
  virtual ~ConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DestructorSymbol : public rq::ProcedureSymbol {
  using Self = rq::DestructorSymbol;

  DestructorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                   rq::SymbolTableSymbol &scope,
                   rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::DESTRUCTOR, expression, module,
                            scope, attributes) {}
  DestructorSymbol(const Self &) = delete;
  DestructorSymbol(Self &&) = delete;
  virtual ~DestructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangerSymbol : public rq::ProcedureSymbol {
  using Self = rq::RangerSymbol;

  RangerSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &scope,
               rq::SymbolAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::SymbolKind::RANGER, expression, module, scope,
                            attributes) {}
  RangerSymbol(const Self &) = delete;
  RangerSymbol(Self &&) = delete;
  virtual ~RangerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateClassSymbol : public rq::TemplateSymbol,
                             public rq::detail::HasNameSymbol {
  using Self = rq::TemplateClassSymbol;

  TemplateClassSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_CLASS, parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateClassSymbol(const Self &) = delete;
  TemplateClassSymbol(Self &&) = delete;
  virtual ~TemplateClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateEnumerationSymbol : public rq::TemplateSymbol,
                                   public rq::detail::HasNameSymbol {
  using Self = rq::TemplateEnumerationSymbol;

  TemplateEnumerationSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_ENUMERATION, parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateEnumerationSymbol(const Self &) = delete;
  TemplateEnumerationSymbol(Self &&) = delete;
  virtual ~TemplateEnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateDynamicVariableSymbol : public rq::TemplateSymbol,
                                       public rq::detail::HasNameSymbol {
  using Self = rq::TemplateDynamicVariableSymbol;

  TemplateDynamicVariableSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_DYNAMIC_VARIABLE,
                           parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateDynamicVariableSymbol(const Self &) = delete;
  TemplateDynamicVariableSymbol(Self &&) = delete;
  virtual ~TemplateDynamicVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateStaticVariableSymbol : public rq::TemplateSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::TemplateStaticVariableSymbol;

  TemplateStaticVariableSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_STATIC_VARIABLE,
                           parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateStaticVariableSymbol(const Self &) = delete;
  TemplateStaticVariableSymbol(Self &&) = delete;
  virtual ~TemplateStaticVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateFunctionSymbol : public rq::TemplateSymbol,
                                public rq::detail::HasNameSymbol {
  using Self = rq::TemplateFunctionSymbol;

  TemplateFunctionSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_FUNCTION, parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateFunctionSymbol(const Self &) = delete;
  TemplateFunctionSymbol(Self &&) = delete;
  virtual ~TemplateFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateMethodSymbol : public rq::TemplateSymbol,
                              public rq::detail::HasNameSymbol {
  using Self = rq::TemplateMethodSymbol;

  TemplateMethodSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_METHOD, parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateMethodSymbol(const Self &) = delete;
  TemplateMethodSymbol(Self &&) = delete;
  virtual ~TemplateMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionFunctionSymbol : public rq::TemplateSymbol,
                                         public rq::detail::HasNameSymbol {
  using Self = rq::TemplateExtensionFunctionSymbol;

  TemplateExtensionFunctionSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_EXTENSION_FUNCTION,
                           parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateExtensionFunctionSymbol(const Self &) = delete;
  TemplateExtensionFunctionSymbol(Self &&) = delete;
  virtual ~TemplateExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateExtensionMethodSymbol : public rq::TemplateSymbol,
                                       public rq::detail::HasNameSymbol {
  using Self = rq::TemplateExtensionMethodSymbol;

  TemplateExtensionMethodSymbol(
      llvm::StringRef name,
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_EXTENSION_METHOD,
                           parameters),
        rq::detail::HasNameSymbol(name) {}
  TemplateExtensionMethodSymbol(const Self &) = delete;
  TemplateExtensionMethodSymbol(Self &&) = delete;
  virtual ~TemplateExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateConstructorSymbol : public rq::TemplateSymbol {
  using Self = rq::TemplateConstructorSymbol;

  TemplateConstructorSymbol(
      const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::TemplateSymbol(rq::SymbolKind::TEMPLATE_CONSTRUCTOR, parameters) {}
  TemplateConstructorSymbol(const Self &) = delete;
  TemplateConstructorSymbol(Self &&) = delete;
  virtual ~TemplateConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialClassSymbol : public rq::PartialSpecializationSymbol,
                            public rq::detail::HasNameSymbol {
  using Self = rq::PartialClassSymbol;

  PartialClassSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(rq::SymbolKind::PARTIAL_CLASS),
        rq::detail::HasNameSymbol(name) {}
  PartialClassSymbol(const Self &) = delete;
  PartialClassSymbol(Self &&) = delete;
  virtual ~PartialClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialEnumerationSymbol : public rq::PartialSpecializationSymbol,
                                  public rq::detail::HasNameSymbol {
  using Self = rq::PartialEnumerationSymbol;

  PartialEnumerationSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(rq::SymbolKind::PARTIAL_ENUMERATION),
        rq::detail::HasNameSymbol(name) {}
  PartialEnumerationSymbol(const Self &) = delete;
  PartialEnumerationSymbol(Self &&) = delete;
  virtual ~PartialEnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialDynamicVariableSymbol : public rq::PartialSpecializationSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialDynamicVariableSymbol;

  PartialDynamicVariableSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(
            rq::SymbolKind::PARTIAL_DYNAMIC_VARIABLE),
        rq::detail::HasNameSymbol(name) {}
  PartialDynamicVariableSymbol(const Self &) = delete;
  PartialDynamicVariableSymbol(Self &&) = delete;
  virtual ~PartialDynamicVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialStaticVariableSymbol : public rq::PartialSpecializationSymbol,
                                     public rq::detail::HasNameSymbol {
  using Self = rq::PartialStaticVariableSymbol;

  PartialStaticVariableSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(
            rq::SymbolKind::PARTIAL_STATIC_VARIABLE),
        rq::detail::HasNameSymbol(name) {}
  PartialStaticVariableSymbol(const Self &) = delete;
  PartialStaticVariableSymbol(Self &&) = delete;
  virtual ~PartialStaticVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialFunctionSymbol : public rq::PartialSpecializationSymbol,
                               public rq::detail::HasNameSymbol {
  using Self = rq::PartialFunctionSymbol;

  PartialFunctionSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(rq::SymbolKind::PARTIAL_FUNCTION),
        rq::detail::HasNameSymbol(name) {}
  PartialFunctionSymbol(const Self &) = delete;
  PartialFunctionSymbol(Self &&) = delete;
  virtual ~PartialFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialMethodSymbol : public rq::PartialSpecializationSymbol,
                             public rq::detail::HasNameSymbol {
  using Self = rq::PartialMethodSymbol;

  PartialMethodSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(rq::SymbolKind::PARTIAL_METHOD),
        rq::detail::HasNameSymbol(name) {}
  PartialMethodSymbol(const Self &) = delete;
  PartialMethodSymbol(Self &&) = delete;
  virtual ~PartialMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionFunctionSymbol : public rq::PartialSpecializationSymbol,
                                        public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionFunctionSymbol;

  PartialExtensionFunctionSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(
            rq::SymbolKind::PARTIAL_EXTENSION_FUNCTION),
        rq::detail::HasNameSymbol(name) {}
  PartialExtensionFunctionSymbol(const Self &) = delete;
  PartialExtensionFunctionSymbol(Self &&) = delete;
  virtual ~PartialExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionMethodSymbol : public rq::PartialSpecializationSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionMethodSymbol;

  PartialExtensionMethodSymbol(llvm::StringRef name)
      : rq::PartialSpecializationSymbol(
            rq::SymbolKind::PARTIAL_EXTENSION_METHOD),
        rq::detail::HasNameSymbol(name) {}
  PartialExtensionMethodSymbol(const Self &) = delete;
  PartialExtensionMethodSymbol(Self &&) = delete;
  virtual ~PartialExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialConstructorSymbol : public rq::PartialSpecializationSymbol {
  using Self = rq::PartialConstructorSymbol;

  PartialConstructorSymbol()
      : rq::PartialSpecializationSymbol(rq::SymbolKind::PARTIAL_CONSTRUCTOR) {}
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

struct ModuleSymbol final : public rq::Symbol,
                            public rq::detail::HasLocationSymbol {
  using Self = rq::ModuleSymbol;

  rq::ModuleKind _module_kind;
  llvm::MemoryBufferRef _llvm_buffer_ref;
  llvm::StringRef _path;

  ModuleSymbol(rq::ModuleKind kind, llvm::StringRef path,
               llvm::MemoryBufferRef &&buffer)
      : rq::Symbol(rq::SymbolKind::MODULE), rq::detail::HasLocationSymbol(),
        _module_kind(kind), _llvm_buffer_ref(std::move(buffer)), _path(path) {}
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
  RQ_ALWAYS_INLINE void changeExpression(rq::Expression &expression) {
    RQ_ASSERT(this->_expression_ptr != nullptr, "no expression");
    this->_expression_ptr = &expression;
  }
  [[nodiscard]] rq::Expression &popExpression() {
    rq::Expression &expression = rq::dereferencePtr(this->_expression_ptr);
    this->_expression_ptr = nullptr;
    return expression;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression) {
    rq::Expression &replaced_expression = this->getExpression();
    this->_expression_ptr = &expression;
    return replaced_expression;
  }
};

struct ImportSymbol final : public rq::Symbol,
                            public rq::detail::HasLocationSymbol,
                            public rq::detail::ModuleMemberSymbol,
                            public rq::detail::SymbolTableMemberSymbol,
                            public rq::detail::HasAttributesSymbol,
                            public rq::detail::HasImportModuleSymbol {
  using Self = rq::ImportSymbol;

  ImportSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &scope,
               rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::IMPORT),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes) {}
  ImportSymbol(const Self &) = delete;
  ImportSymbol(Self &&) = delete;
  virtual ~ImportSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FacadeSymbol final : public rq::Symbol,
                            public rq::detail::HasLocationSymbol,
                            public rq::detail::ModuleMemberSymbol,
                            public rq::detail::SymbolTableMemberSymbol,
                            public rq::detail::HasAttributesSymbol {
  using Self = rq::FacadeSymbol;

  FacadeSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &scope,
               rq::SymbolAttributeFlags attributes)
      : rq::Symbol(rq::SymbolKind::FACADE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes) {}
  FacadeSymbol(const Self &) = delete;
  FacadeSymbol(Self &&) = delete;
  virtual ~FacadeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace rq
