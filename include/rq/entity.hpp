#pragma once

#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>
#include <rq/ast.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/StringSaver.h>

#include <bit>
#include <cstdint>
#include <string>
#include <utility>

namespace rq {

struct Context;

enum class EntityKind : std::uint16_t {
  NONE = 0,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same (including type attriubutes, etc) by comparing their
  // pointers.

  // TYPE DEFINITION
  SY_TYPE_DEFINITION,

  // SIMPLE BUILTIN
  SY_INFERENCE,
  SY_EXPRESSION,
  SY_ENTITY_KIND,
  SY_GENERIC_TYPE,
  SY_GENERIC_SYMBOL,
  SY_VOID,
  SY_NULL,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_GENERIC_FLOAT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_GENERIC_BINARY,
  SY_GENERIC_BFLOAT,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_GENERIC_INTEGER,
  SY_GENERIC_SIGNED,
  SY_GENERIC_UNSIGNED,
  SY_GENERIC_CODEUNIT,
  SY_ASCII,
  SY_UTF8,

  // SCALED BUILTIN
  SY_SCALED_SIGNED,
  SY_SCALED_UNSIGNED,

  // UNARY SUBTYPE
  SY_RANGE,
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCED_COUNT_ARRAY,

  // COUNTED SUBTYPE
  SY_ARRAY,

  // COMPOSITE SUBTYPE
  SY_LAYOUT,
  SY_SIGNATURE,

  // ARITHMETIC SEQUENCE
  SY_ARITHMETIC_INTERVAL,
  SY_FINITE_ARITHMETIC_PROGRESSION,
  SY_INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  SY_MODULE,
  SY_IMPORT,
  SY_FACADE,
  SY_MUTATION,
  SY_EXTENSION,

  // BINDING
  SY_DYNAMIC_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_PROPERTY,
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,
  SY_LABEL,

  // SYMBOL TABLE
  SY_TOP,
  SY_SCOPE,
  SY_TABLE,
  SY_CLASS,
  SY_ENUMERATION,

  // PROCEDURE
  SY_ENTRY,
  SY_FUNCTION,
  SY_METHOD,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,
  SY_CONSTRUCTOR,
  SY_DESTRUCTOR,
  SY_RANGER,

  // TEMPLATE
  SY_TEMPLATE_CLASS,
  SY_TEMPLATE_ENUMERATION,
  SY_TEMPLATE_DYNAMIC_VARIABLE,
  SY_TEMPLATE_STATIC_VARIABLE,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,
  SY_TEMPLATE_CONSTRUCTOR,

  // PARTIAL SPECIALIZATION
  SY_PARTIAL_CLASS,
  SY_PARTIAL_ENUMERATION,
  SY_PARTIAL_DYNAMIC_VARIABLE,
  SY_PARTIAL_STATIC_VARIABLE,
  SY_PARTIAL_FUNCTION,
  SY_PARTIAL_METHOD,
  SY_PARTIAL_EXTENSION_FUNCTION,
  SY_PARTIAL_EXTENSION_METHOD,
  SY_PARTIAL_CONSTRUCTOR,

  // =====CONSTANTS=====

  CT_INTEGER,
  CT_FLOAT,
  CT_STRING,
  CT_ARRAY,

  LAST
};

static constexpr std::size_t ENTITY_COUNT =
    static_cast<std::size_t>(rq::EntityKind::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::NONE:
    return "none";

  // =====SYMBOLS=====

  // TYPE DEFINITION
  case E::SY_TYPE_DEFINITION:
    return "sy_type_definition";

  // SIMPLE BUILTIN
  case E::SY_INFERENCE:
    return "sy_inference";
  case E::SY_EXPRESSION:
    return "sy_expression";
  case E::SY_ENTITY_KIND:
    return "sy_entity_kind";
  case E::SY_GENERIC_TYPE:
    return "sy_generic_type";
  case E::SY_GENERIC_SYMBOL:
    return "sy_generic_symbol";
  case E::SY_VOID:
    return "sy_void";
  case E::SY_NULL:
    return "sy_null";
  case E::SY_NO_RETURN:
    return "sy_no_return";
  case E::SY_VARIADIC_ARGUMENTS:
    return "sy_variadic_arguments";
  case E::SY_BOOLEAN:
    return "sy_boolean";
  case E::SY_GENERIC_FLOAT:
    return "sy_generic_float";
  case E::SY_HALF:
    return "sy_half";
  case E::SY_SINGLE:
    return "sy_single";
  case E::SY_DOUBLE:
    return "sy_double";
  case E::SY_QUADRUPLE:
    return "sy_quadruple";
  case E::SY_GENERIC_BINARY:
    return "sy_generic_binary";
  case E::SY_GENERIC_BFLOAT:
    return "sy_generic_float";
  case E::SY_BINARY16:
    return "sy_binary16";
  case E::SY_BINARY32:
    return "sy_binary32";
  case E::SY_BINARY64:
    return "sy_binary64";
  case E::SY_BINARY128:
    return "sy_binary128";
  case E::SY_BFLOAT16:
    return "sy_bfloat16";
  case E::SY_GENERIC_INTEGER:
    return "sy_generic_integer";
  case E::SY_GENERIC_SIGNED:
    return "sy_generic_signed";
  case E::SY_GENERIC_UNSIGNED:
    return "sy_generic_unsigned";
  case E::SY_GENERIC_CODEUNIT:
    return "sy_generic_codeunit";
  case E::SY_ASCII:
    return "sy_ascii";
  case E::SY_UTF8:
    return "sy_utf8";

  // SCALED BUILTIN
  case E::SY_SCALED_SIGNED:
    return "sy_scaled_signed";
  case E::SY_SCALED_UNSIGNED:
    return "sy_scaled_unsigned";

  // UNARY SUBTYPE
  case E::SY_RANGE:
    return "sy_range";
  case E::SY_REFERENCE:
    return "sy_reference";
  case E::SY_POINTER:
    return "sy_pointer";
  case E::SY_FAT_POINTER:
    return "sy_fat_pointer";
  case E::SY_INFERENCED_COUNT_ARRAY:
    return "sy_inferenced_count_array";

  // COUNTED SUBTYPE
  case E::SY_ARRAY:
    return "sy_array";

  // COMPOSITE SUBTYPE
  case E::SY_LAYOUT:
    return "sy_layout";
  case E::SY_SIGNATURE:
    return "sy_signature";

  // ARITHMETIC SEQUENCE
  case E::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";

  // MISC
  case E::SY_MODULE:
    return "sy_module";
  case E::SY_IMPORT:
    return "sy_import";
  case E::SY_FACADE:
    return "sy_facade";
  case E::SY_MUTATION:
    return "sy_mutation";
  case E::SY_EXTENSION:
    return "sy_extension";

  // BINDING
  case E::SY_DYNAMIC_VARIABLE:
    return "sy_dynamic_variable";
  case E::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case E::SY_ENUMERATOR:
    return "sy_enumerator";
  case E::SY_PROPERTY:
    return "sy_property";
  case E::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case E::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case E::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case E::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
  case E::SY_LABEL:
    return "sy_label";

  // SYMBOL TABLE
  case E::SY_TOP:
    return "sy_top";
  case E::SY_SCOPE:
    return "sy_scope";
  case E::SY_TABLE:
    return "sy_table";
  case E::SY_CLASS:
    return "sy_class";
  case E::SY_ENUMERATION:
    return "sy_enumeration";

  // PROCEDURE
  case E::SY_ENTRY:
    return "sy_entry";
  case E::SY_FUNCTION:
    return "sy_function";
  case E::SY_METHOD:
    return "sy_method";
  case E::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case E::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case E::SY_CONSTRUCTOR:
    return "sy_constructor";
  case E::SY_DESTRUCTOR:
    return "sy_destructor";
  case E::SY_RANGER:
    return "sy_ranger";

  // TEMPLATE
  case E::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case E::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return "sy_template_dynamic_variable";
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return "sy_template_static_variable";
  case E::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case E::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";
  case E::SY_TEMPLATE_CONSTRUCTOR:
    return "sy_template_constructor";

  // PARTIAL SPECIALIZATION
  case E::SY_PARTIAL_CLASS:
    return "sy_partial_class";
  case E::SY_PARTIAL_ENUMERATION:
    return "sy_partial_enumeration";
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return "sy_partial_dynamic_variable";
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return "sy_partial_static_variable";
  case E::SY_PARTIAL_FUNCTION:
    return "sy_partial_function";
  case E::SY_PARTIAL_METHOD:
    return "sy_partial_method";
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return "sy_partial_extension_function";
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return "sy_partial_extension_method";
  case E::SY_PARTIAL_CONSTRUCTOR:
    return "sy_partial_constructor";

    // =====CONSTANTS=====

  case E::CT_INTEGER:
    return "ct_integer";
  case E::CT_FLOAT:
    return "ct_float";
  case E::CT_STRING:
    return "ct_string";
  case E::CT_ARRAY:
    return "ct_array";

  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class EntityFlags : std::uint32_t {
  NONE = 0,

  // CATEGORIES
  // the first two bits used to determine category
  // the rest of the bits mean different things depending on the
  // category.
  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),

  // SYMBOL FLAGS
  // these flags are valid only when working with symbols
  // SYMBOL INHERITING PROPERTIES - are implmented via inherited types that can
  // be used themselves. every symbol can have at most one of these.
  SY_SIMPLE_BUILTIN = rq::getBit(2),
  SY_SCALED_BUILTIN = rq::getBit(3),
  SY_UNARY_SUBTYPE = rq::getBit(4),
  SY_COUNTED_SUBTYPE = rq::getBit(5),
  SY_COMPOSITE_SUBTYPE = rq::getBit(6),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(7),
  SY_SYMBOL_TABLE = rq::getBit(8),
  SY_PROCEDURE = rq::getBit(9),
  SY_TEMPLATE = rq::getBit(10),
  SY_PARTIAL = rq::getBit(11),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(12),
  SY_TYPE = rq::getBit(13),
  SY_GENERIC = rq::getBit(14),
  SY_CONCRETE = rq::getBit(15),
  SY_SUBTYPE = rq::getBit(16),
  SY_PLATFORM_CHANGING = rq::getBit(17),
  SY_INTEGER = rq::getBit(18),
  SY_FLOAT = rq::getBit(19),
  SY_CODEUNIT = rq::getBit(20),
  SY_SIGNED = rq::getBit(21),
  SY_UNSIGNED = rq::getBit(22),
  SY_TOP_OF_FRAME = rq::getBit(23)

  // CONSTANT FLAGS
  // TODO
};

template <> struct is_flags<EntityFlags> : std::true_type {};

[[nodiscard]] inline rq::EntityFlags getFlags(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  using EF = EntityFlags;
  switch (kind) {
  case E::NONE:
    return EF::NONE;

  // TYPE DEFINITION SYMBOL
  case E::SY_TYPE_DEFINITION:
    return EF::SYMBOL | EF::SY_TYPE;

  // SIMPLE BUILTIN SYMBOL
  case E::SY_INFERENCE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_EXPRESSION:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_ENTITY_KIND:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_GENERIC_TYPE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_GENERIC_SYMBOL:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_VOID:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_NULL:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_NO_RETURN:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_VARIADIC_ARGUMENTS:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_BOOLEAN:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_GENERIC_FLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_HALF:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_SINGLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_DOUBLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_QUADRUPLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_GENERIC_BINARY:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_GENERIC_BFLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_GENERIC;
  case E::SY_BINARY16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY32:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY64:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BINARY128:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_BFLOAT16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_FLOAT |
           EF::SY_CONCRETE;
  case E::SY_GENERIC_INTEGER:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_SIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_UNSIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_INTEGER |
           EF::SY_GENERIC;
  case E::SY_GENERIC_CODEUNIT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CODEUNIT |
           EF::SY_GENERIC;
  case E::SY_ASCII:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CODEUNIT |
           EF::SY_CONCRETE;
  case E::SY_UTF8:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CODEUNIT |
           EF::SY_CONCRETE;

  // SCALED BUILTIN SYMBOL
  case E::SY_SCALED_SIGNED:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE | EF::SY_INTEGER |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING | EF::SY_SIGNED;
  case E::SY_SCALED_UNSIGNED:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE | EF::SY_INTEGER |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;

  // UNARY SUBTYPE SYMBOL
  case E::SY_RANGE:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_REFERENCE:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_FAT_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_INFERENCED_COUNT_ARRAY:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_GENERIC;

  // COUNTED SUBTYPE SYMBOL
  case E::SY_ARRAY:
    return EF::SYMBOL | EF::SY_COUNTED_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;

  // COMPOSITE SUBTYPE SYMBOL
  case E::SY_LAYOUT:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE | EF::SY_PLATFORM_CHANGING;
  case E::SY_SIGNATURE:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;

  // ARITHMETIC SEQUENCE SYMBOL
  case E::SY_ARITHMETIC_INTERVAL:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;

  // MISC SYMBOL
  case E::SY_MODULE:
    return EF::SYMBOL;
  case E::SY_IMPORT:
    return EF::SYMBOL;
  case E::SY_FACADE:
    return EF::SYMBOL;
  case E::SY_MUTATION:
    return EF::SYMBOL;
  case E::SY_EXTENSION:
    return EF::SYMBOL | EF::SY_TYPE | EF::SY_SUBTYPE | EF::SY_CONCRETE;

  // BINDING SYMBOL
  case E::SY_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENUMERATOR:
    return EF::SYMBOL;
  case E::SY_PROPERTY:
    return EF::SYMBOL;
  case E::SY_CLASS_PARAMETER:
    return EF::SYMBOL;
  case E::SY_LAYOUT_PARAMETER:
    return EF::SYMBOL;
  case E::SY_TEMPLATE_PARAMETER:
    return EF::SYMBOL;
  case E::SY_SIGNATURE_PARAMETER:
    return EF::SYMBOL;
  case E::SY_LABEL:
    return EF::SYMBOL;

  // SYMBOL TABLE SYMBOL
  case E::SY_TOP:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TOP_OF_FRAME;
  case E::SY_SCOPE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE;
  case E::SY_TABLE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TOP_OF_FRAME;
  case E::SY_CLASS:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_ENUMERATION:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;

  // PROCEDURE SYMBOL
  case E::SY_ENTRY:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_TOP_OF_FRAME;
  case E::SY_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_CONSTRUCTOR:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_DESTRUCTOR:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_TOP_OF_FRAME;
  case E::SY_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_TOP_OF_FRAME;

  // TEMPLATE SYMBOL
  case E::SY_TEMPLATE_CLASS:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_ENUMERATION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE;
  case E::SY_TEMPLATE_CONSTRUCTOR:
    return EF::SYMBOL | EF::SY_TEMPLATE;

  // PARTIAL SPECIALIZATION SYMBOL
  case E::SY_PARTIAL_CLASS:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_ENUMERATION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL;
  case E::SY_PARTIAL_CONSTRUCTOR:
    return EF::SYMBOL | EF::SY_PARTIAL;

  // CONSTANT
  case E::CT_INTEGER:
    return EF::CONSTANT;
  case E::CT_FLOAT:
    return EF::CONSTANT;
  case E::CT_STRING:
    return EF::CONSTANT;
  case E::CT_ARRAY:
    return EF::CONSTANT;

  case E::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::CONSTANT);
}

#define RQ_ASSERT_SYMBOL(kind) RQ_ASSERT(rq::getIsSymbol((kind)), "not symbol")

#define RQ_ASSERT_CONSTANT(kind)                                               \
  RQ_ASSERT(rq::getIsConstant((kind)), "not constant")

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSimpleBuiltinSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIMPLE_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsScaledBuiltinSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SCALED_BUILTIN);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsUnarySubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNARY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCountedSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COUNTED_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsCompositeSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COMPOSITE_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequenceSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_ARITHMETIC_SEQUENCE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsSymbolTableSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SYMBOL_TABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PROCEDURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PARTIAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternativeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_HAS_TEMPLATE_ALTERNATIVE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_GENERIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcreteSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CONCRETE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtypeSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SUBTYPE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsPlatformChangingSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PLATFORM_CHANGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_INTEGER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_FLOAT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CODEUNIT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNSIGNED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrameSymbol(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TOP_OF_FRAME);
}


[[nodiscard]] inline rq::EntityKind getTemplate(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_CONSTRUCTOR:
    return E::SY_TEMPLATE_CONSTRUCTOR;
  case E::SY_PARTIAL_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_PARTIAL_CONSTRUCTOR:
    return E::SY_TEMPLATE_CONSTRUCTOR;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CONSTRUCTOR:
    return E::SY_TEMPLATE_CONSTRUCTOR;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::EntityKind getPartial(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_CONSTRUCTOR:
    return E::SY_PARTIAL_CONSTRUCTOR;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CONSTRUCTOR:
    return E::SY_PARTIAL_CONSTRUCTOR;
  case E::SY_PARTIAL_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_PARTIAL_CONSTRUCTOR:
    return E::SY_PARTIAL_CONSTRUCTOR;
  default:
    RQ_UNREACHABLE();
  }
}

[[nodiscard]] inline rq::EntityKind getFullSpecialization(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    return E::SY_CLASS;
  case E::SY_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_METHOD:
    return E::SY_METHOD;
  case E::SY_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_CONSTRUCTOR:
    return E::SY_CONSTRUCTOR;
  case E::SY_TEMPLATE_CLASS:
    return E::SY_CLASS;
  case E::SY_TEMPLATE_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_TEMPLATE_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_TEMPLATE_METHOD:
    return E::SY_METHOD;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_TEMPLATE_CONSTRUCTOR:
    return E::SY_CONSTRUCTOR;
  case E::SY_PARTIAL_CLASS:
    return E::SY_CLASS;
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_PARTIAL_METHOD:
    return E::SY_METHOD;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_PARTIAL_CONSTRUCTOR:
    return E::SY_CONSTRUCTOR;
  default:
    RQ_UNREACHABLE();
  }
}

struct Entity {
  using Self = Entity;

  rq::EntityKind _kind;

  Entity(rq::EntityKind kind) : _kind(kind) {}
  Entity(const Self &) = delete;
  Entity(Self &&) = delete;
  virtual ~Entity() {};
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getKind() const {
    return this->_kind;
  }
  [[nodiscard]] llvm::StringRef getKindName() const {
    return rq::getName(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return rq::getIsSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant() const {
    return rq::getIsConstant(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeDefinitionSymbol() const {
    return this->_kind == rq::EntityKind::SY_TYPE_DEFINITION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltinSymbol() const {
    return rq::getIsSimpleBuiltinSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferenceSymbol() const {
    return this->_kind == rq::EntityKind::SY_INFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionSymbol() const {
    return this->_kind == rq::EntityKind::SY_EXPRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntityKindSymbol() const {
    return this->_kind == rq::EntityKind::SY_ENTITY_KIND;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericTypeSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_TYPE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbolSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_SYMBOL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVoidSymbol() const {
    return this->_kind == rq::EntityKind::SY_VOID;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullSymbol() const {
    return this->_kind == rq::EntityKind::SY_NULL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoReturnSymbol() const {
    return this->_kind == rq::EntityKind::SY_NO_RETURN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariadicArgumentsSymbol() const {
    return this->_kind == rq::EntityKind::SY_VARIADIC_ARGUMENTS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBooleanSymbol() const {
    return this->_kind == rq::EntityKind::SY_BOOLEAN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericFloatSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHalfSymbol() const {
    return this->_kind == rq::EntityKind::SY_HALF;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSingleSymbol() const {
    return this->_kind == rq::EntityKind::SY_SINGLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDoubleSymbol() const {
    return this->_kind == rq::EntityKind::SY_DOUBLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsQuadrupleSymbol() const {
    return this->_kind == rq::EntityKind::SY_QUADRUPLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBinarySymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_BINARY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBfloatSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_BFLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary16Symbol() const {
    return this->_kind == rq::EntityKind::SY_BINARY16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary32Symbol() const {
    return this->_kind == rq::EntityKind::SY_BINARY32;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary64Symbol() const {
    return this->_kind == rq::EntityKind::SY_BINARY64;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary128Symbol() const {
    return this->_kind == rq::EntityKind::SY_BINARY128;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloat16Symbol() const {
    return this->_kind == rq::EntityKind::SY_BFLOAT16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericIntegerSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSignedSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericUnsignedSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_UNSIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericCodeunitSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_CODEUNIT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAsciiSymbol() const {
    return this->_kind == rq::EntityKind::SY_ASCII;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUtf8Symbol() const {
    return this->_kind == rq::EntityKind::SY_UTF8;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltinSymbol() const {
    return rq::getIsScaledBuiltinSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledSignedSymbol() const {
    return this->_kind == rq::EntityKind::SY_SCALED_SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledUnsignedSymbol() const {
    return this->_kind == rq::EntityKind::SY_SCALED_UNSIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtypeSymbol() const {
    return rq::getIsUnarySubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRangeSymbol() const {
    return this->_kind == rq::EntityKind::SY_RANGE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReferenceSymbol() const {
    return this->_kind == rq::EntityKind::SY_REFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointerSymbol() const {
    return this->_kind == rq::EntityKind::SY_POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFatPointerSymbol() const {
    return this->_kind == rq::EntityKind::SY_FAT_POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferencedCountArraySymbol() const {
    return this->_kind == rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtypeSymbol() const {
    return rq::getIsCountedSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArraySymbol() const {
    return this->_kind == rq::EntityKind::SY_ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtypeSymbol() const {
    return rq::getIsCompositeSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutSymbol() const {
    return this->_kind == rq::EntityKind::SY_LAYOUT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureSymbol() const {
    return this->_kind == rq::EntityKind::SY_SIGNATURE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequenceSymbol() const {
    return rq::getIsArithmeticSequenceSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticIntervalSymbol() const {
    return this->_kind == rq::EntityKind::SY_ARITHMETIC_INTERVAL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsFiniteArithmeticProgressionSymbol() const {
    return this->_kind == rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsInfiniteArithmeticProgressionSymbol() const {
    return this->_kind == rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModuleSymbol() const {
    return this->_kind == rq::EntityKind::SY_MODULE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImportSymbol() const {
    return this->_kind == rq::EntityKind::SY_IMPORT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFacadeSymbol() const {
    return this->_kind == rq::EntityKind::SY_FACADE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMutationSymbol() const {
    return this->_kind == rq::EntityKind::SY_MUTATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionSymbol() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumeratorSymbol() const {
    return this->_kind == rq::EntityKind::SY_ENUMERATOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPropertySymbol() const {
    return this->_kind == rq::EntityKind::SY_PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassParameterSymbol() const {
    return this->_kind == rq::EntityKind::SY_CLASS_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutParameterSymbol() const {
    return this->_kind == rq::EntityKind::SY_LAYOUT_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateParameterSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureParameterSymbol() const {
    return this->_kind == rq::EntityKind::SY_SIGNATURE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLabelSymbol() const {
    return this->_kind == rq::EntityKind::SY_LABEL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTableSymbol() const {
    return rq::getIsSymbolTableSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopSymbol() const {
    return this->_kind == rq::EntityKind::SY_TOP;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScopeSymbol() const {
    return this->_kind == rq::EntityKind::SY_SCOPE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTableSymbol() const {
    return this->_kind == rq::EntityKind::SY_TABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassSymbol() const {
    return this->_kind == rq::EntityKind::SY_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumerationSymbol() const {
    return this->_kind == rq::EntityKind::SY_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedureSymbol() const {
    return rq::getIsProcedureSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntrySymbol() const {
    return this->_kind == rq::EntityKind::SY_ENTRY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstructorSymbol() const {
    return this->_kind == rq::EntityKind::SY_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDestructorSymbol() const {
    return this->_kind == rq::EntityKind::SY_DESTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRangerSymbol() const {
    return this->_kind == rq::EntityKind::SY_RANGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateSymbol() const {
    return rq::getIsTemplateSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateClassSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateEnumerationSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateDynamicVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateStaticVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateExtensionFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsTemplateExtensionMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateConstructorSymbol() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialSymbol() const {
    return rq::getIsPartialSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialClassSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialEnumerationSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsPartialDynamicVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialStaticVariableSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsPartialExtensionFunctionSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsPartialExtensionMethodSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialConstructorSymbol() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerConstant() const {
    return this->_kind == rq::EntityKind::CT_INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatConstant() const {
    return this->_kind == rq::EntityKind::CT_FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStringConstant() const {
    return this->_kind == rq::EntityKind::CT_STRING;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArrayConstant() const {
    return this->_kind == rq::EntityKind::CT_ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternativeSymbol() const {
    return rq::getHasTemplateAlternativeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeSymbol() const {
    return rq::getIsTypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol() const {
    return rq::getIsGenericSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcreteSymbol() const {
    return rq::getIsConcreteSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtypeSymbol() const {
    return rq::getIsSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChangingSymbol() const {
    return rq::getIsPlatformChangingSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIntegerSymbol() const {
    return rq::getIsIntegerSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloatSymbol() const {
    return rq::getIsFloatSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunitSymbol() const {
    return rq::getIsCodeunitSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedSymbol() const {
    return rq::getIsSignedSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedSymbol() const {
    return rq::getIsUnsignedSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrameSymbol() const {
    return rq::getIsTopOfFrameSymbol(this->_kind);
  }
};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTableSymbol;
  using reference = rq::SymbolTableSymbol &;
  using pointer = rq::SymbolTableSymbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTableSymbol *_symbol_table_ptr = nullptr;

  SymbolTableIterator() = default;
  SymbolTableIterator(rq::SymbolTableSymbol *symbol_table_ptr)
      : _symbol_table_ptr(symbol_table_ptr) {}
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_symbol_table_ptr == it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_symbol_table_ptr != it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &operator*() {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  operator*() const {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *operator->() {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  operator->() const {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_symbol_table_ptr == nullptr;
  }
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = const rq::SymbolTableSymbol;
  using reference = const rq::SymbolTableSymbol &;
  using pointer = const rq::SymbolTableSymbol *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTableSymbol *_symbol_table_ptr = nullptr;

  ConstSymbolTableIterator() = default;
  ConstSymbolTableIterator(const rq::SymbolTableSymbol *symbol_table_ptr)
      : _symbol_table_ptr(symbol_table_ptr) {}
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_symbol_table_ptr == it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_symbol_table_ptr != it._symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  operator*() const {
    return rq::dereferencePtr(this->_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  operator->() const {
    return this->_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_symbol_table_ptr == nullptr;
  }
};

// ROOT WITH TYPE ATTRIBUTES
struct TypeDefinitionSymbol;

// SIMPLE BUILTIN
struct SimpleBuiltinSymbol;
struct InferenceSymbol;
struct ExpressionSymbol;
struct EntityKindSymbol;
struct VoidSymbol;
struct NullSymbol;
struct NoReturnSymbol;
struct VariadicArgumentsSymbol;
struct BooleanSymbol;
struct GenericFloatSymbol;
struct HalfSymbol;
struct SingleSymbol;
struct DoubleSymbol;
struct QuadrupleSymbol;
struct GenericBinarySymbol;
struct GenericBfloatSymbol;
struct Binary16Symbol;
struct Binary32Symbol;
struct Binary64Symbol;
struct Binary128Symbol;
struct Bfloat16Symbol;
struct GenericIntegerSymbol;
struct GenericSignedSymbol;
struct GenericUnsignedSymbol;
struct GenericCodeunitSymbol;
struct AsciiSymbol;
struct Utf8Symbol;

// SCALED INTEGER
struct ScaledBuiltinSymbol;
struct ScaledSignedSymbol;
struct ScaledUnsignedSymbol;

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
struct ScopeSymbol;
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
struct PartialSymbol;
struct PartialClassSymbol;
struct PartialEnumerationSymbol;
struct PartialDynamicVariableSymbol;
struct PartialStaticVariableSymbol;
struct PartialFunctionSymbol;
struct PartialMethodSymbol;
struct PartialExtensionFunctionSymbol;
struct PartialExtensionMethodSymbol;
struct PartialConstructorSymbol;

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  Symbol(rq::EntityKind kind) : rq::Entity(kind) { RQ_ASSERT_SYMBOL(kind); }

  Symbol(const Self &) = delete;
  Symbol(Self &&) = delete;
  ~Symbol() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SimpleBuiltinSymbol : public rq::Symbol {
  using Self = rq::SimpleBuiltinSymbol;

  SimpleBuiltinSymbol(rq::EntityKind kind) : rq::Symbol(kind) {
    RQ_ASSERT(rq::getIsSimpleBuiltinSymbol(kind),
              "kind not builtin simple symbol");
  }

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
  EXACT_NONE_MASK = FASTEST | LEAST,
  LITTLE_ENDIAN_ = rq::getBit(5),
  BIG_ENDIAN_ = rq::getBit(6),
  PLATFORM_ENDIAN_NONE_MASK = LITTLE_ENDIAN_ | BIG_ENDIAN_,
  PLATFORM_SCALAR = rq::getBit(7)
};

template <> struct is_flags<ScaledBuiltinFlags> : std::true_type {};

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

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPlatformScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::PLATFORM_SCALAR);
}

void RQ_ALWAYS_INLINE profileScaledIntegerSymbol(llvm::FoldingSetNodeID &id,
                                                 rq::EntityKind kind,
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

  ScaledBuiltinSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags)
      : rq::Symbol(kind), _scalar(scalar), _uid(uid), _flags(flags) {
    RQ_ASSERT(rq::getIsScaledBuiltinSymbol(kind),
              "kind not scaled builtin symbol");
    RQ_ASSERT(scalar < rq::MAX_SCALED_BUILTIN_SCALAR, "scalar too large");
    RQ_ASSERT(uid < rq::MAX_SCALED_BUILTIN_UID, "uid too large");
  }

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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPlatformScalar() const {
    return rq::getHasPlatformScalar(this->_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSynonym() const {
    return this->_uid != 0;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileScaledIntegerSymbol(id, this->_kind, this->_scalar, this->_uid,
                                   this->_flags);
  }
};

struct ScaledIntegerSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::ScaledIntegerSymbol;

  ScaledIntegerSymbol(unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(rq::EntityKind::SY_GENERIC_INTEGER, scalar, uid,
                                flags) {}

  ScaledIntegerSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
                      rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}

  ScaledIntegerSymbol(const Self &) = delete;
  ScaledIntegerSymbol(Self &&) = delete;
  virtual ~ScaledIntegerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct FloatSymbol : public rq::ScaledBuiltinSymbol {
  using Self = rq::FloatSymbol;

  FloatSymbol(unsigned scalar, unsigned uid, rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(rq::EntityKind::SY_GENERIC_FLOAT, scalar, uid,
                                flags) {}

  FloatSymbol(rq::EntityKind kind, unsigned scalar, unsigned uid,
              rq::ScaledBuiltinFlags flags)
      : rq::ScaledBuiltinSymbol(kind, scalar, uid, flags) {}

  FloatSymbol(const Self &) = delete;
  FloatSymbol(Self &&) = delete;
  virtual ~FloatSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

void RQ_ALWAYS_INLINE profileUnarySubtypeSymbol(llvm::FoldingSetNodeID &id,
                                                rq::EntityKind kind,
                                                const rq::Symbol &root) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&root);
}

struct UnarySubtypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtypeSymbol;
  friend struct Context;

  rq::Symbol *_root_ptr{nullptr};

  UnarySubtypeSymbol(rq::EntityKind kind, rq::Symbol &root)
      : rq::Symbol(kind), _root_ptr(&root) {
    RQ_ASSERT(rq::getIsUnarySubtypeSymbol(kind),
              "kind not unary subtype symbol");
    RQ_ASSERT(root.getIsTypeSymbol(), "not type");
  }

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
                                                  rq::EntityKind kind,
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
  std::size_t _count;

  CountedSubtypeSymbol(rq::EntityKind kind, rq::Symbol &root, std::size_t count)
      : rq::Symbol(kind), _root_ptr(&root), _count(count) {
    RQ_ASSERT(rq::getIsCountedSubtypeSymbol(kind), "not counted subtype");
    RQ_ASSERT(root.getIsTypeSymbol(), "not type");
  }

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
  [[nodiscard]] RQ_ALWAYS_INLINE std::size_t getCount() const {
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

  ArithmeticSequenceSymbol(rq::EntityKind kind, rq::Symbol &root,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step)
      : rq::Symbol(kind), _root_ptr(&root), _condition(condition), _step(step) {
    RQ_ASSERT(root.getIsTypeSymbol(), "not type");
    RQ_ASSERT(rq::getIsArithmeticSequenceSymbol(kind),
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
    rq::profileArithmeticSequenceSymbol(id, this->getRoot(),
                                        this->getCondition(), this->getStep());
  }
};

namespace detail {
struct HasLocationSymbol {
  using Self = rq::detail::HasLocationSymbol;

  rq::Expression *_expression_ptr;

  HasLocationSymbol(rq::Expression &expression)
      : _expression_ptr(&expression) {}

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
struct SymbolTableMemberSymbol {
  using Self = rq::detail::SymbolTableMemberSymbol;

  rq::SymbolTableSymbol *_containing_symbol_table_ptr;

  SymbolTableMemberSymbol() : _containing_symbol_table_ptr(nullptr) {}
  SymbolTableMemberSymbol(rq::SymbolTableSymbol &containing_table)
      : _containing_symbol_table_ptr(&containing_table) {}

  SymbolTableMemberSymbol(const Self &) = delete;
  SymbolTableMemberSymbol(Self &&) = delete;
  virtual ~SymbolTableMemberSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingSymbolTable() const {
    return this->_containing_symbol_table_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol &
  getContainingSymbolTable() const {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol &
  getContainingSymbolTable() {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableSymbol *
  getContainingSymbolTablePtr() const {
    return this->_containing_symbol_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableSymbol *
  getContainingSymbolTablePtr() {
    return this->_containing_symbol_table_ptr;
  }
};
struct HasAttributesSymbol {
  using Self = HasAttributesSymbol;

  rq::ExpressionAttributeFlags _attributes;

  HasAttributesSymbol(rq::ExpressionAttributeFlags attributes)
      : _attributes(attributes) {}

  HasAttributesSymbol(const Self &) = delete;
  HasAttributesSymbol(Self &&) = delete;
  virtual ~HasAttributesSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttributeFlags
  getExpressionAttributes() const {
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
  getHasAttribute(rq::ExpressionAttribute attribute) const {
    return rq::getHasAttribute(this->_attributes, attribute);
  }
};
struct HasNameSymbol {
  using Self = rq::detail::HasNameSymbol;

  llvm::StringRef _name;

  HasNameSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }

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

  MaybeHasNameSymbol() : _name() {}
  MaybeHasNameSymbol(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "empty name");
  }

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
struct HasBindingTypeSymbol {
  using Self = rq::detail::HasBindingTypeSymbol;

  rq::TypeDefinitionSymbol *_binding_type_ptr{nullptr};

  HasBindingTypeSymbol() {}

  HasBindingTypeSymbol(const Self &) = delete;
  HasBindingTypeSymbol(Self &&) = delete;
  virtual ~HasBindingTypeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBindingType() const {
    return this->_binding_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeDefinitionSymbol &
  getBindingType() const {
    return rq::dereferencePtr(this->_binding_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeDefinitionSymbol &getBindingType() {
    return rq::dereferencePtr(this->_binding_type_ptr);
  }
  RQ_ALWAYS_INLINE void setBindingType(rq::TypeDefinitionSymbol &type) {
    rq::assignSingleValue(this->_binding_type_ptr, &type);
  }
};
struct HasImportModuleSymbol {
  using Self = rq::detail::HasImportModuleSymbol;

  rq::ModuleSymbol *_import_module_ptr{nullptr};

  HasImportModuleSymbol() {}

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

struct SymbolTableSymbol : public rq::Symbol,
                           public rq::detail::SymbolTableMemberSymbol {
  using Self = rq::SymbolTableSymbol;

  llvm::SmallDenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_values{};
  rq::BumpPtrList<rq::Symbol> _unamed_values{};

  SymbolTableSymbol(rq::EntityKind kind)
      : rq::Symbol(kind), rq::detail::SymbolTableMemberSymbol() {}
  SymbolTableSymbol(rq::EntityKind kind,
                    rq::SymbolTableSymbol &containing_table)
      : rq::Symbol(kind),
        rq::detail::SymbolTableMemberSymbol(containing_table) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE auto getFrameSymbolTableTableRange() {
    return std::ranges::subrange(
        rq::SymbolTableIterator(this->getContainingSymbolTablePtr()),
        rq::SymbolTableIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE auto getFrameSymbolTableTableRange() const {
    return std::ranges::subrange(
        rq::ConstSymbolTableIterator(this->getContainingSymbolTablePtr()),
        rq::ConstSymbolTableIterator());
  }
};

rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  if (this->_symbol_table_ptr == nullptr) {
    return *this;
  }
  rq::SymbolTableSymbol &table = rq::dereferencePtr(this->_symbol_table_ptr);
  if (table.getIsTopOfFrameSymbol()) {
    this->_symbol_table_ptr = nullptr;
    return *this;
  }
  this->_symbol_table_ptr = table.getContainingSymbolTablePtr();
  return *this;
}

rq::SymbolTableIterator SymbolTableIterator::operator++(int) { return ++*this; }

rq::ConstSymbolTableIterator &ConstSymbolTableIterator::operator++() {
  if (this->_symbol_table_ptr == nullptr) {
    return *this;
  }
  const rq::SymbolTableSymbol &table =
      rq::dereferencePtr(this->_symbol_table_ptr);
  if (table.getIsTopOfFrameSymbol()) {
    this->_symbol_table_ptr = nullptr;
    return *this;
  }
  this->_symbol_table_ptr = table.getContainingSymbolTablePtr();
  return *this;
}

rq::ConstSymbolTableIterator ConstSymbolTableIterator::operator++(int) {
  return ++*this;
}

struct ProcedureSymbol : public rq::SymbolTableSymbol,
                         public rq::detail::HasLocationSymbol,
                         public rq::detail::ModuleMemberSymbol,
                         public rq::detail::HasAttributesSymbol {
  using Self = rq::ProcedureSymbol;

  rq::SignatureSymbol *_signature_ptr;

  ProcedureSymbol(rq::EntityKind kind, rq::Expression &expression,
                  rq::ModuleSymbol &module,
                  rq::SymbolTableSymbol &containing_table,
                  rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(kind, containing_table),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::HasAttributesSymbol(attributes), _signature_ptr(nullptr) {}

  ProcedureSymbol(const Self &) = delete;
  ProcedureSymbol(Self &&) = delete;
  virtual ~ProcedureSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TemplateSymbol : public rq::Symbol {
  using Self = rq::TemplateSymbol;

  rq::BumpPtrList<rq::TemplateParameterSymbol> _template_parameters;

  TemplateSymbol(rq::EntityKind kind,
                 const rq::BumpPtrList<rq::TemplateParameterSymbol> &parameters)
      : rq::Symbol(kind), _template_parameters(parameters) {}

  TemplateSymbol(const Self &) = delete;
  TemplateSymbol(Self &&) = delete;
  virtual ~TemplateSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialSymbol : public rq::Symbol {
  using Self = rq::PartialSymbol;

  PartialSymbol(rq::EntityKind kind) : rq::Symbol(kind) {}

  PartialSymbol(const Self &) = delete;
  PartialSymbol(Self &&) = delete;
  virtual ~PartialSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

} // namespace rq
namespace llvm {

// ROOT WITH TYPE ATTRIBUTES
template <> struct isa_impl<rq::TypeDefinitionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTypeDefinitionSymbol();
  }
};

// SIMPLE BUILTIN
template <> struct isa_impl<rq::SimpleBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSimpleBuiltinSymbol();
  }
};

template <> struct isa_impl<rq::InferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInferenceSymbol();
  }
};

template <> struct isa_impl<rq::InferenceSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsInferenceSymbol();
  }
};

template <> struct isa_impl<rq::ExpressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExpressionSymbol();
  }
};

template <> struct isa_impl<rq::ExpressionSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsExpressionSymbol();
  }
};

template <> struct isa_impl<rq::EntityKindSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEntityKindSymbol();
  }
};

template <> struct isa_impl<rq::EntityKindSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsEntityKindSymbol();
  }
};

template <> struct isa_impl<rq::VoidSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsVoidSymbol();
  }
};

template <> struct isa_impl<rq::VoidSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsVoidSymbol();
  }
};

template <> struct isa_impl<rq::NullSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsNullSymbol();
  }
};

template <> struct isa_impl<rq::NullSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsNullSymbol();
  }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsNoReturnSymbol();
  }
};

template <> struct isa_impl<rq::NoReturnSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsNoReturnSymbol();
  }
};

template <> struct isa_impl<rq::VariadicArgumentsSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsVariadicArgumentsSymbol();
  }
};

template <>
struct isa_impl<rq::VariadicArgumentsSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsVariadicArgumentsSymbol();
  }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBooleanSymbol();
  }
};

template <> struct isa_impl<rq::BooleanSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBooleanSymbol();
  }
};

template <> struct isa_impl<rq::GenericFloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericFloatSymbol();
  }
};

template <> struct isa_impl<rq::GenericFloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericFloatSymbol();
  }
};

template <> struct isa_impl<rq::HalfSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsHalfSymbol();
  }
};

template <> struct isa_impl<rq::HalfSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsHalfSymbol();
  }
};

template <> struct isa_impl<rq::SingleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSingleSymbol();
  }
};

template <> struct isa_impl<rq::SingleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsSingleSymbol();
  }
};

template <> struct isa_impl<rq::DoubleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDoubleSymbol();
  }
};

template <> struct isa_impl<rq::DoubleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsDoubleSymbol();
  }
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsQuadrupleSymbol();
  }
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsQuadrupleSymbol();
  }
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericBinarySymbol();
  }
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericBinarySymbol();
  }
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericBfloatSymbol();
  }
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericBfloatSymbol();
  }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary16Symbol();
  }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary16Symbol();
  }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary32Symbol();
  }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary32Symbol();
  }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary64Symbol();
  }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary64Symbol();
  }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary128Symbol();
  }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary128Symbol();
  }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBfloat16Symbol();
  }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBfloat16Symbol();
  }
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericIntegerSymbol();
  }
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericIntegerSymbol();
  }
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericSignedSymbol();
  }
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericSignedSymbol();
  }
};

template <> struct isa_impl<rq::GenericUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericUnsignedSymbol();
  }
};

template <>
struct isa_impl<rq::GenericUnsignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericUnsignedSymbol();
  }
};

template <> struct isa_impl<rq::GenericCodeunitSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericCodeunitSymbol();
  }
};

template <>
struct isa_impl<rq::GenericCodeunitSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericCodeunitSymbol();
  }
};

template <> struct isa_impl<rq::AsciiSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsAsciiSymbol();
  }
};

template <> struct isa_impl<rq::AsciiSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsAsciiSymbol();
  }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsUtf8Symbol();
  }
};

template <> struct isa_impl<rq::Utf8Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsUtf8Symbol();
  }
};

// SCALED BUILTIN
template <> struct isa_impl<rq::ScaledBuiltinSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledBuiltinSymbol();
  }
};

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledSignedSymbol();
  }
};

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsScaledSignedSymbol();
  }
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledUnsignedSymbol();
  }
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsScaledUnsignedSymbol();
  }
};

// UNARY SUBTYPE
template <> struct isa_impl<rq::UnarySubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsUnarySubtypeSymbol();
  }
};

template <> struct isa_impl<rq::RangeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsRangeSymbol();
  }
};

template <> struct isa_impl<rq::RangeSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsRangeSymbol();
  }
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsReferenceSymbol();
  }
};

template <> struct isa_impl<rq::ReferenceSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsReferenceSymbol();
  }
};

template <> struct isa_impl<rq::PointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPointerSymbol();
  }
};

template <> struct isa_impl<rq::PointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsPointerSymbol();
  }
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFatPointerSymbol();
  }
};

template <> struct isa_impl<rq::FatPointerSymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsFatPointerSymbol();
  }
};

template <> struct isa_impl<rq::InferencedCountArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInferencedCountArraySymbol();
  }
};

template <>
struct isa_impl<rq::InferencedCountArraySymbol, rq::UnarySubtypeSymbol> {
  static inline bool doit(const rq::UnarySubtypeSymbol &val) {
    return val.getIsInferencedCountArraySymbol();
  }
};

// COUNTED SUBTYPE
template <> struct isa_impl<rq::CountedSubtypeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsCountedSubtypeSymbol();
  }
};

template <> struct isa_impl<rq::ArraySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsArraySymbol();
  }
};

template <> struct isa_impl<rq::ArraySymbol, rq::CountedSubtypeSymbol> {
  static inline bool doit(const rq::CountedSubtypeSymbol &val) {
    return val.getIsArraySymbol();
  }
};

// COMPOSITE SUBTYPE
template <> struct isa_impl<rq::LayoutSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsLayoutSymbol();
  }
};

template <> struct isa_impl<rq::SignatureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSignatureSymbol();
  }
};

template <> struct isa_impl<rq::ExtensionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionSymbol();
  }
};

// ARITHMETIC SEQUENCE
template <> struct isa_impl<rq::ArithmeticSequenceSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsArithmeticSequenceSymbol();
  }
};

template <> struct isa_impl<rq::ArithmeticIntervalSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsArithmeticIntervalSymbol();
  }
};

template <>
struct isa_impl<rq::ArithmeticIntervalSymbol, rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsArithmeticIntervalSymbol();
  }
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsInfiniteArithmeticProgressionSymbol();
  }
};

template <>
struct isa_impl<rq::InfiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsInfiniteArithmeticProgressionSymbol();
  }
};

template <> struct isa_impl<rq::FiniteArithmeticProgressionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFiniteArithmeticProgressionSymbol();
  }
};

template <>
struct isa_impl<rq::FiniteArithmeticProgressionSymbol,
                rq::ArithmeticSequenceSymbol> {
  static inline bool doit(const rq::ArithmeticSequenceSymbol &val) {
    return val.getIsFiniteArithmeticProgressionSymbol();
  }
};

// MISC
template <> struct isa_impl<rq::ModuleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsModuleSymbol();
  }
};

template <> struct isa_impl<rq::ImportSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsImportSymbol();
  }
};

template <> struct isa_impl<rq::FacadeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFacadeSymbol();
  }
};

template <> struct isa_impl<rq::MutationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsMutationSymbol();
  }
};

// BINDING
template <> struct isa_impl<rq::DynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDynamicVariableSymbol();
  }
};

template <> struct isa_impl<rq::StaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsStaticVariableSymbol();
  }
};

template <> struct isa_impl<rq::EnumeratorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumeratorSymbol();
  }
};

template <> struct isa_impl<rq::PropertySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPropertySymbol();
  }
};

template <> struct isa_impl<rq::TemplateParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateParameterSymbol();
  }
};

template <> struct isa_impl<rq::SignatureParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSignatureParameterSymbol();
  }
};

template <> struct isa_impl<rq::ClassParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsClassParameterSymbol();
  }
};

template <> struct isa_impl<rq::LayoutParameterSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsLayoutParameterSymbol();
  }
};

template <> struct isa_impl<rq::LabelSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsLabelSymbol();
  }
};

// SYMBOL TABLES
template <> struct isa_impl<rq::SymbolTableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsSymbolTableSymbol();
  }
};

template <> struct isa_impl<rq::TopSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTopSymbol();
  }
};

template <> struct isa_impl<rq::TopSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsTopSymbol();
  }
};

template <> struct isa_impl<rq::ScopeSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScopeSymbol();
  }
};

template <> struct isa_impl<rq::ScopeSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsScopeSymbol();
  }
};

template <> struct isa_impl<rq::TableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTableSymbol();
  }
};

template <> struct isa_impl<rq::TableSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsTableSymbol();
  }
};

template <> struct isa_impl<rq::ClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsClassSymbol();
  }
};

template <> struct isa_impl<rq::ClassSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsClassSymbol();
  }
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEnumerationSymbol();
  }
};

template <> struct isa_impl<rq::EnumerationSymbol, rq::SymbolTableSymbol> {
  static inline bool doit(const rq::SymbolTableSymbol &val) {
    return val.getIsEnumerationSymbol();
  }
};

// PROCEDURES
template <> struct isa_impl<rq::ProcedureSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsProcedureSymbol();
  }
};

template <> struct isa_impl<rq::EntrySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsEntrySymbol();
  }
};

template <> struct isa_impl<rq::EntrySymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsEntrySymbol();
  }
};

template <> struct isa_impl<rq::FunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsFunctionSymbol();
  }
};

template <> struct isa_impl<rq::FunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsFunctionSymbol();
  }
};

template <> struct isa_impl<rq::MethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsMethodSymbol();
  }
};

template <> struct isa_impl<rq::MethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsMethodSymbol();
  }
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionFunctionSymbol();
  }
};

template <> struct isa_impl<rq::ExtensionFunctionSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsExtensionFunctionSymbol();
  }
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsExtensionMethodSymbol();
  }
};

template <> struct isa_impl<rq::ExtensionMethodSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsExtensionMethodSymbol();
  }
};

template <> struct isa_impl<rq::ConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsConstructorSymbol();
  }
};

template <> struct isa_impl<rq::ConstructorSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsConstructorSymbol();
  }
};

template <> struct isa_impl<rq::DestructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsDestructorSymbol();
  }
};

template <> struct isa_impl<rq::DestructorSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsDestructorSymbol();
  }
};

template <> struct isa_impl<rq::RangerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsRangerSymbol();
  }
};

template <> struct isa_impl<rq::RangerSymbol, rq::ProcedureSymbol> {
  static inline bool doit(const rq::ProcedureSymbol &val) {
    return val.getIsRangerSymbol();
  }
};

// TEMPLATE
template <> struct isa_impl<rq::TemplateSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateSymbol();
  }
};

template <> struct isa_impl<rq::TemplateClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateClassSymbol();
  }
};

template <> struct isa_impl<rq::TemplateClassSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateClassSymbol();
  }
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateEnumerationSymbol();
  }
};

template <> struct isa_impl<rq::TemplateEnumerationSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateEnumerationSymbol();
  }
};

template <> struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateDynamicVariableSymbol();
  }
};

template <>
struct isa_impl<rq::TemplateDynamicVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateDynamicVariableSymbol();
  }
};

template <> struct isa_impl<rq::TemplateStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateStaticVariableSymbol();
  }
};

template <>
struct isa_impl<rq::TemplateStaticVariableSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateStaticVariableSymbol();
  }
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateFunctionSymbol();
  }
};

template <> struct isa_impl<rq::TemplateFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateFunctionSymbol();
  }
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateMethodSymbol();
  }
};

template <> struct isa_impl<rq::TemplateMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateMethodSymbol();
  }
};

template <> struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionFunctionSymbol();
  }
};

template <>
struct isa_impl<rq::TemplateExtensionFunctionSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateExtensionFunctionSymbol();
  }
};

template <> struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateExtensionMethodSymbol();
  }
};

template <>
struct isa_impl<rq::TemplateExtensionMethodSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateExtensionMethodSymbol();
  }
};

template <> struct isa_impl<rq::TemplateConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsTemplateConstructorSymbol();
  }
};

template <> struct isa_impl<rq::TemplateConstructorSymbol, rq::TemplateSymbol> {
  static inline bool doit(const rq::TemplateSymbol &val) {
    return val.getIsTemplateConstructorSymbol();
  }
};

// PARTIAL SPECIALIZATION
template <> struct isa_impl<rq::PartialSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialSymbol();
  }
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialClassSymbol();
  }
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialClassSymbol();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialEnumerationSymbol();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialEnumerationSymbol();
  }
};

template <> struct isa_impl<rq::PartialDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialDynamicVariableSymbol();
  }
};

template <>
struct isa_impl<rq::PartialDynamicVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialDynamicVariableSymbol();
  }
};

template <> struct isa_impl<rq::PartialStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialStaticVariableSymbol();
  }
};

template <>
struct isa_impl<rq::PartialStaticVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialStaticVariableSymbol();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialFunctionSymbol();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialFunctionSymbol();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialMethodSymbol();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialMethodSymbol();
  }
};

template <> struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionFunctionSymbol();
  }
};

template <>
struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialExtensionFunctionSymbol();
  }
};

template <> struct isa_impl<rq::PartialExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionMethodSymbol();
  }
};

template <>
struct isa_impl<rq::PartialExtensionMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialExtensionMethodSymbol();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialConstructorSymbol();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialConstructorSymbol();
  }
};

} // namespace llvm
namespace rq {

struct TypeSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::TypeSymbol;

  rq::Symbol *_root_ptr{nullptr};
  rq::TypeAttributeFlags _flags{};

  TypeSymbol(rq::EntityKind kind, rq::Symbol &root,
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

  InferenceSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_INFERENCE) {}
  InferenceSymbol(const Self &) = delete;
  InferenceSymbol(Self &&) = delete;
  virtual ~InferenceSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ExpressionSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::ExpressionSymbol;

  ExpressionSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_EXPRESSION) {}
  ExpressionSymbol(const Self &) = delete;
  ExpressionSymbol(Self &&) = delete;
  virtual ~ExpressionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct EntityKindSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::EntityKindSymbol;

  EntityKindSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_ENTITY_KIND) {}
  EntityKindSymbol(const Self &) = delete;
  EntityKindSymbol(Self &&) = delete;
  virtual ~EntityKindSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VoidSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VoidSymbol;

  VoidSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_VOID) {}
  VoidSymbol(const Self &) = delete;
  VoidSymbol(Self &&) = delete;
  virtual ~VoidSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NullSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NullSymbol;

  NullSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_NULL) {}
  NullSymbol(const Self &) = delete;
  NullSymbol(Self &&) = delete;
  virtual ~NullSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct NoReturnSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::NoReturnSymbol;

  NoReturnSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_NO_RETURN) {}
  NoReturnSymbol(const Self &) = delete;
  NoReturnSymbol(Self &&) = delete;
  virtual ~NoReturnSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct VariadicArgumentsSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::VariadicArgumentsSymbol;

  VariadicArgumentsSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_VARIADIC_ARGUMENTS) {}
  VariadicArgumentsSymbol(const Self &) = delete;
  VariadicArgumentsSymbol(Self &&) = delete;
  virtual ~VariadicArgumentsSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct BooleanSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::BooleanSymbol;

  BooleanSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BOOLEAN) {}
  BooleanSymbol(const Self &) = delete;
  BooleanSymbol(Self &&) = delete;
  virtual ~BooleanSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericFloatSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericFloatSymbol;

  GenericFloatSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_FLOAT) {}
  GenericFloatSymbol(const Self &) = delete;
  GenericFloatSymbol(Self &&) = delete;
  virtual ~GenericFloatSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct HalfSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::HalfSymbol;

  HalfSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_HALF) {}
  HalfSymbol(const Self &) = delete;
  HalfSymbol(Self &&) = delete;
  virtual ~HalfSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct SingleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::SingleSymbol;

  SingleSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_SINGLE) {}
  SingleSymbol(const Self &) = delete;
  SingleSymbol(Self &&) = delete;
  virtual ~SingleSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DoubleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::DoubleSymbol;

  DoubleSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_DOUBLE) {}
  DoubleSymbol(const Self &) = delete;
  DoubleSymbol(Self &&) = delete;
  virtual ~DoubleSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct QuadrupleSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::QuadrupleSymbol;

  QuadrupleSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_QUADRUPLE) {}
  QuadrupleSymbol(const Self &) = delete;
  QuadrupleSymbol(Self &&) = delete;
  virtual ~QuadrupleSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericBinarySymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericBinarySymbol;

  GenericBinarySymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_BINARY) {}
  GenericBinarySymbol(const Self &) = delete;
  GenericBinarySymbol(Self &&) = delete;
  virtual ~GenericBinarySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericBfloatSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericBfloatSymbol;

  GenericBfloatSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_BFLOAT) {}
  GenericBfloatSymbol(const Self &) = delete;
  GenericBfloatSymbol(Self &&) = delete;
  virtual ~GenericBfloatSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary16Symbol;

  Binary16Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY16) {}
  Binary16Symbol(const Self &) = delete;
  Binary16Symbol(Self &&) = delete;
  virtual ~Binary16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary32Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary32Symbol;

  Binary32Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY32) {}
  Binary32Symbol(const Self &) = delete;
  Binary32Symbol(Self &&) = delete;
  virtual ~Binary32Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary64Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary64Symbol;

  Binary64Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY64) {}
  Binary64Symbol(const Self &) = delete;
  Binary64Symbol(Self &&) = delete;
  virtual ~Binary64Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Binary128Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Binary128Symbol;

  Binary128Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BINARY128) {}
  Binary128Symbol(const Self &) = delete;
  Binary128Symbol(Self &&) = delete;
  virtual ~Binary128Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct Bfloat16Symbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Bfloat16Symbol;

  Bfloat16Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_BFLOAT16) {}
  Bfloat16Symbol(const Self &) = delete;
  Bfloat16Symbol(Self &&) = delete;
  virtual ~Bfloat16Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericIntegerSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericIntegerSymbol;

  GenericIntegerSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_INTEGER) {}
  GenericIntegerSymbol(const Self &) = delete;
  GenericIntegerSymbol(Self &&) = delete;
  virtual ~GenericIntegerSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericSignedSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericSignedSymbol;

  GenericSignedSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_SIGNED) {}
  GenericSignedSymbol(const Self &) = delete;
  GenericSignedSymbol(Self &&) = delete;
  virtual ~GenericSignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericUnsignedSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericUnsignedSymbol;

  GenericUnsignedSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_UNSIGNED) {}
  GenericUnsignedSymbol(const Self &) = delete;
  GenericUnsignedSymbol(Self &&) = delete;
  virtual ~GenericUnsignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct GenericCodeunitSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::GenericCodeunitSymbol;

  GenericCodeunitSymbol()
      : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_GENERIC_CODEUNIT) {}
  GenericCodeunitSymbol(const Self &) = delete;
  GenericCodeunitSymbol(Self &&) = delete;
  virtual ~GenericCodeunitSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct AsciiSymbol : public rq::SimpleBuiltinSymbol {
  using Self = rq::Utf8Symbol;

  AsciiSymbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_ASCII) {}
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
  Utf8Symbol() : rq::SimpleBuiltinSymbol(rq::EntityKind::SY_UTF8) {}

  Utf8Symbol(const Self &) = delete;
  Utf8Symbol(Self &&) = delete;
  virtual ~Utf8Symbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScaledUnsignedSymbol : public rq::ScaledIntegerSymbol {
  using Self = rq::ScaledUnsignedSymbol;
  friend struct Context;

private:
  ScaledUnsignedSymbol(unsigned scalar, unsigned uid,
                       rq::ScaledBuiltinFlags flags)
      : rq::ScaledIntegerSymbol(rq::EntityKind::SY_SCALED_UNSIGNED, scalar, uid,
                                flags) {}

  ScaledUnsignedSymbol(const Self &) = delete;
  ScaledUnsignedSymbol(Self &&) = delete;
  virtual ~ScaledUnsignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScaledSignedSymbol : public rq::ScaledIntegerSymbol {
  using Self = rq::ScaledSignedSymbol;
  friend struct Context;

private:
  ScaledSignedSymbol(unsigned scalar, unsigned uid,
                     rq::ScaledBuiltinFlags flags)
      : rq::ScaledIntegerSymbol(rq::EntityKind::SY_SCALED_SIGNED, scalar, uid,
                                flags) {}

  ScaledSignedSymbol(const Self &) = delete;
  ScaledSignedSymbol(Self &&) = delete;
  virtual ~ScaledSignedSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangeSymbol : public rq::UnarySubtypeSymbol {
  using Self = rq::RangeSymbol;
  friend struct Context;

private:
  RangeSymbol(rq::Symbol &root)
      : rq::UnarySubtypeSymbol(rq::EntityKind::SY_RANGE, root) {}

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
      : rq::UnarySubtypeSymbol(rq::EntityKind::SY_REFERENCE, root) {}

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
      : rq::UnarySubtypeSymbol(rq::EntityKind::SY_POINTER, root) {}

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
      : rq::UnarySubtypeSymbol(rq::EntityKind::SY_FAT_POINTER, root) {}

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
      : rq::UnarySubtypeSymbol(rq::EntityKind::SY_INFERENCED_COUNT_ARRAY,
                               root) {}

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
  ArraySymbol(rq::Symbol &root, std::size_t count)
      : rq::CountedSubtypeSymbol(rq::EntityKind::SY_ARRAY, root, count) {}

  ArraySymbol(const Self &) = delete;
  ArraySymbol(Self &&) = delete;
  virtual ~ArraySymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct LayoutSymbol : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::LayoutSymbol;
  friend struct Context;

  rq::BumpPtrList<rq::Symbol> _properties;

private:
  LayoutSymbol(rq::BumpPtrList<rq::Symbol> properties)
      : rq::Symbol(rq::EntityKind::SY_LAYOUT), _properties(properties) {}

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
      : rq::Symbol(rq::EntityKind::SY_SIGNATURE), _return_ptr(&return_),
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
      : rq::Symbol(rq::EntityKind::SY_EXTENSION), _signature_ptr(&signature),
        _type_ptr(&type) {}

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
      : rq::ArithmeticSequenceSymbol(rq::EntityKind::SY_ARITHMETIC_INTERVAL,
                                     root, condition,
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
            rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION, root, condition,
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
            rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION, root,
            rq::ArithmeticSequenceCondition::NONE, step) {}
  InfiniteArithmeticProgressionSymbol(const Self &) = delete;
  InfiniteArithmeticProgressionSymbol(Self &&) = delete;
  virtual ~InfiniteArithmeticProgressionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct TopSymbol : rq::SymbolTableSymbol {
  using Self = rq::TopSymbol;

  TopSymbol() : rq::SymbolTableSymbol(rq::EntityKind::SY_TOP) {}
  TopSymbol(const Self &) = delete;
  TopSymbol(Self &&) = delete;
  ~TopSymbol() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct ScopeSymbol : public rq::SymbolTableSymbol,
                     public rq::detail::HasLocationSymbol,
                     public rq::detail::ModuleMemberSymbol {
  using Self = rq::ScopeSymbol;

  ScopeSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &containing_table)
      : rq::SymbolTableSymbol(rq::EntityKind::SY_SCOPE, containing_table),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module) {}
  ScopeSymbol(const Self &) = delete;
  ScopeSymbol(Self &&) = delete;
  ~ScopeSymbol() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DynamicVariableSymbol : public rq::Symbol,
                               public rq::detail::HasLocationSymbol,
                               public rq::detail::ModuleMemberSymbol,
                               public rq::detail::SymbolTableMemberSymbol,
                               public rq::detail::HasAttributesSymbol,
                               public rq::detail::HasNameSymbol,
                               public rq::detail::HasBindingTypeSymbol {
  using Self = rq::DynamicVariableSymbol;

  DynamicVariableSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_DYNAMIC_VARIABLE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                              public rq::detail::HasBindingTypeSymbol {
  using Self = rq::StaticVariableSymbol;

  rq::Gendex<rq::SymbolicValue> _symbolic_value{};

  StaticVariableSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                       rq::SymbolTableSymbol &containing_table,
                       llvm::StringRef name,
                       rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_STATIC_VARIABLE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                          public rq::detail::HasBindingTypeSymbol {
  using Self = rq::EnumeratorSymbol;

  rq::EnumerationSymbol *_enumeration_ptr;

  EnumeratorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                   rq::SymbolTableSymbol &containing_table,
                   llvm::StringRef name,
                   rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_ENUMERATOR),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                        public rq::detail::HasBindingTypeSymbol {
  using Self = rq::PropertySymbol;

  PropertySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &containing_table,
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_PROPERTY),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  PropertySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_PROPERTY),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                                  public rq::detail::HasBindingTypeSymbol {
  using Self = rq::SignatureParameterSymbol;

  rq::SignatureSymbol *_signature_ptr;

  SignatureParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                           rq::SymbolTableSymbol &containing_table,
                           rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  SignatureParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                           rq::SymbolTableSymbol &containing_table,
                           llvm::StringRef name,
                           rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                                 public rq::detail::HasBindingTypeSymbol {
  using Self = rq::TemplateParameterSymbol;

  rq::TemplateSymbol *_template_ptr;

  TemplateParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &containing_table,
                          rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  TemplateParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &containing_table,
                          llvm::StringRef name,
                          rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                              public rq::detail::HasBindingTypeSymbol {
  using Self = rq::ClassParameterSymbol;

  ClassParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                       rq::SymbolTableSymbol &containing_table,
                       rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_CLASS_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                               public rq::detail::HasBindingTypeSymbol {
  using Self = rq::LayoutParameterSymbol;

  LayoutParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  LayoutParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
                 rq::SymbolTableSymbol &containing_table,
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_MUTATION),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
              rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
              rq::Expression &statement)
      : rq::Symbol(rq::EntityKind::SY_LABEL),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
      : rq::SymbolTableSymbol(rq::EntityKind::SY_TABLE),
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
                     public rq::detail::HasAttributesSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::ClassSymbol;

  rq::BumpPtrList<rq::PropertySymbol> _class_properties;

  ClassSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
              rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::EntityKind::SY_CLASS, containing_table),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
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
                           public rq::detail::HasAttributesSymbol,
                           public rq::detail::HasNameSymbol {
  using Self = rq::EnumerationSymbol;

  rq::BumpPtrList<rq::EnumeratorSymbol> _enumerators;

  EnumerationSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                    rq::SymbolTableSymbol &containing_table,
                    llvm::StringRef name,
                    rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::EntityKind::SY_ENUMERATION, containing_table),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
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
              rq::SymbolTableSymbol &containing_table,
              rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_ENTRY, expression, module,
                            containing_table, attributes) {}
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
                 rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
                 rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_FUNCTION, expression, module,
                            containing_table, attributes),
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
               rq::SymbolTableSymbol &containing_table, llvm::StringRef name,
               rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_METHOD, expression, module,
                            containing_table, attributes),
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
                          rq::SymbolTableSymbol &containing_table,
                          llvm::StringRef name,
                          rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_FUNCTION, expression,
                            module, containing_table, attributes),
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
                        rq::SymbolTableSymbol &containing_table,
                        llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_METHOD, expression,
                            module, containing_table, attributes),
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
                    rq::SymbolTableSymbol &containing_table,
                    rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_CONSTRUCTOR, expression, module,
                            containing_table, attributes) {}
  ConstructorSymbol(const Self &) = delete;
  ConstructorSymbol(Self &&) = delete;
  virtual ~ConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct DestructorSymbol : public rq::ProcedureSymbol {
  using Self = rq::DestructorSymbol;

  DestructorSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                   rq::SymbolTableSymbol &containing_table,
                   rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_DESTRUCTOR, expression, module,
                            containing_table, attributes) {}
  DestructorSymbol(const Self &) = delete;
  DestructorSymbol(Self &&) = delete;
  virtual ~DestructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct RangerSymbol : public rq::ProcedureSymbol {
  using Self = rq::RangerSymbol;

  RangerSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
               rq::SymbolTableSymbol &containing_table,
               rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_RANGER, expression, module,
                            containing_table, attributes) {}
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_CLASS, parameters),
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_ENUMERATION, parameters),
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE,
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE,
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_FUNCTION, parameters),
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_METHOD, parameters),
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION,
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD,
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
      : rq::TemplateSymbol(rq::EntityKind::SY_TEMPLATE_CONSTRUCTOR,
                           parameters) {}
  TemplateConstructorSymbol(const Self &) = delete;
  TemplateConstructorSymbol(Self &&) = delete;
  virtual ~TemplateConstructorSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialClassSymbol : public rq::PartialSymbol,
                            public rq::detail::HasNameSymbol {
  using Self = rq::PartialClassSymbol;

  PartialClassSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_CLASS),
        rq::detail::HasNameSymbol(name) {}
  PartialClassSymbol(const Self &) = delete;
  PartialClassSymbol(Self &&) = delete;
  virtual ~PartialClassSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialEnumerationSymbol : public rq::PartialSymbol,
                                  public rq::detail::HasNameSymbol {
  using Self = rq::PartialEnumerationSymbol;

  PartialEnumerationSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_ENUMERATION),
        rq::detail::HasNameSymbol(name) {}
  PartialEnumerationSymbol(const Self &) = delete;
  PartialEnumerationSymbol(Self &&) = delete;
  virtual ~PartialEnumerationSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialDynamicVariableSymbol : public rq::PartialSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialDynamicVariableSymbol;

  PartialDynamicVariableSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE),
        rq::detail::HasNameSymbol(name) {}
  PartialDynamicVariableSymbol(const Self &) = delete;
  PartialDynamicVariableSymbol(Self &&) = delete;
  virtual ~PartialDynamicVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialStaticVariableSymbol : public rq::PartialSymbol,
                                     public rq::detail::HasNameSymbol {
  using Self = rq::PartialStaticVariableSymbol;

  PartialStaticVariableSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE),
        rq::detail::HasNameSymbol(name) {}
  PartialStaticVariableSymbol(const Self &) = delete;
  PartialStaticVariableSymbol(Self &&) = delete;
  virtual ~PartialStaticVariableSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialFunctionSymbol : public rq::PartialSymbol,
                               public rq::detail::HasNameSymbol {
  using Self = rq::PartialFunctionSymbol;

  PartialFunctionSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_FUNCTION),
        rq::detail::HasNameSymbol(name) {}
  PartialFunctionSymbol(const Self &) = delete;
  PartialFunctionSymbol(Self &&) = delete;
  virtual ~PartialFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialMethodSymbol : public rq::PartialSymbol,
                             public rq::detail::HasNameSymbol {
  using Self = rq::PartialMethodSymbol;

  PartialMethodSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_METHOD),
        rq::detail::HasNameSymbol(name) {}
  PartialMethodSymbol(const Self &) = delete;
  PartialMethodSymbol(Self &&) = delete;
  virtual ~PartialMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionFunctionSymbol : public rq::PartialSymbol,
                                        public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionFunctionSymbol;

  PartialExtensionFunctionSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION),
        rq::detail::HasNameSymbol(name) {}
  PartialExtensionFunctionSymbol(const Self &) = delete;
  PartialExtensionFunctionSymbol(Self &&) = delete;
  virtual ~PartialExtensionFunctionSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialExtensionMethodSymbol : public rq::PartialSymbol,
                                      public rq::detail::HasNameSymbol {
  using Self = rq::PartialExtensionMethodSymbol;

  PartialExtensionMethodSymbol(llvm::StringRef name)
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD),
        rq::detail::HasNameSymbol(name) {}
  PartialExtensionMethodSymbol(const Self &) = delete;
  PartialExtensionMethodSymbol(Self &&) = delete;
  virtual ~PartialExtensionMethodSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct PartialConstructorSymbol : public rq::PartialSymbol {
  using Self = rq::PartialConstructorSymbol;

  PartialConstructorSymbol()
      : rq::PartialSymbol(rq::EntityKind::SY_PARTIAL_CONSTRUCTOR) {}
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
      : rq::Symbol(rq::EntityKind::SY_MODULE), rq::detail::HasLocationSymbol(),
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
               rq::SymbolTableSymbol &containing_table,
               rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_IMPORT),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
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
               rq::SymbolTableSymbol &containing_table,
               rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_FACADE),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(containing_table),
        rq::detail::HasAttributesSymbol(attributes) {}
  FacadeSymbol(const Self &) = delete;
  FacadeSymbol(Self &&) = delete;
  virtual ~FacadeSymbol() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_INTEGER));
  llvm::SmallString<32> buf;
  value.toString(buf, 10, false);
  id.AddString(llvm::StringRef(buf));
}

void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_FLOAT));
  llvm::APInt bits = value.bitcastToAPInt();
  llvm::SmallString<32> buf;
  bits.toString(buf, 10, false);
  id.AddString(llvm::StringRef(buf));
}

void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef value) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_STRING));
  id.AddString(value);
}

void RQ_ALWAYS_INLINE profileArrayConstant(
    llvm::FoldingSetNodeID &id, const rq::BumpPtrList<rq::Entity *> &elements) {
  id.AddInteger(static_cast<unsigned>(rq::EntityKind::CT_ARRAY));
  for (rq::Entity *e : elements) {
    id.AddPointer(e);
  }
}

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  Constant(rq::EntityKind kind) : rq::Entity(kind) { RQ_ASSERT_CONSTANT(kind); }
  Constant(const Self &) = delete;
  Constant(Self &&) = delete;
  ~Constant() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
};

struct IntegerConstant final : public rq::Constant,
                               public llvm::FoldingSetNode {
  using Self = rq::IntegerConstant;

  llvm::APInt _value;

  IntegerConstant(const llvm::APInt &value)
      : rq::Constant(rq::EntityKind::CT_INTEGER), _value(value) {}
  IntegerConstant(const Self &) = delete;
  IntegerConstant(Self &&) = delete;
  virtual ~IntegerConstant() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getValue() const {
    return this->_value;
  }
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileIntegerConstant(id, this->_value);
  }
};

struct FloatConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::FloatConstant;

  llvm::APFloat _value;

  FloatConstant(const llvm::APFloat &value)
      : rq::Constant(rq::EntityKind::CT_FLOAT), _value(value) {}
  FloatConstant(const Self &) = delete;
  FloatConstant(Self &&) = delete;
  virtual ~FloatConstant() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getValue() const {
    return this->_value;
  }
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileFloatConstant(id, this->_value);
  }
};

struct StringConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::StringConstant;

  llvm::StringRef _value;

  StringConstant(llvm::StringRef value)
      : rq::Constant(rq::EntityKind::CT_STRING), _value(value) {}
  StringConstant(const Self &) = delete;
  StringConstant(Self &&) = delete;
  virtual ~StringConstant() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getValue() const {
    return this->_value;
  }
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileStringConstant(id, this->_value);
  }
};

struct ArrayConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ArrayConstant;

  rq::BumpPtrList<rq::Entity *> _elements;

  ArrayConstant(rq::BumpPtrList<rq::Entity *> elements)
      : rq::Constant(rq::EntityKind::CT_ARRAY), _elements(elements) {}
  ArrayConstant(const Self &) = delete;
  ArrayConstant(Self &&) = delete;
  virtual ~ArrayConstant() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Entity *>
  getElements() const {
    return this->_elements;
  }
  RQ_ALWAYS_INLINE void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileArrayConstant(id, this->_elements);
  }
};

} // namespace rq
