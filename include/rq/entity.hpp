#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

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

// NOTE: this is a massive tree of types that make use of LLVM RTTI
// The root of this tree is "Entity", and the tree contains all symbols,
// constant values, and symbolic instructions.
// https://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html

// NOTE: everything in this tree is meant to be allocated within a
// llvm::BumpPtrAllocator. Nothing has a destructor. Make use of rq::BumpPtrList
// and rq::BumpPtrMap

enum class EntityKind : std::uint16_t {
  NONE = 0,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same by comparing their pointers.

  // SIMPLE BUILTIN
  SY_INFERENCE,
  SY_GENERIC_SYMBOL,
  SY_GENERIC_TYPE,
  SY_EXPRESSION,
  SY_VOID,
  SY_NULL,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_GENERIC_SIGNED,
  SY_GENERIC_UNSIGNED,
  SY_GENERIC_FLOAT,
  SY_GENERIC_BINARY,
  SY_GENERIC_BFLOAT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_GENERIC_INTEGER,
  SY_GENERIC_SIGNED_INTEGER,
  SY_GENERIC_UNSIGNED_INTEGER,
  SY_GENERIC_CODEUNIT,
  SY_GENERIC_STRING,
  SY_ASCII,
  SY_UTF8,

  // SCALED BUILTIN
  SY_SCALED_SIGNED_INTEGER,
  SY_SCALED_UNSIGNED_INTEGER,

  // UNARY SUBTYPE
  SY_REFERENCE,
  SY_POINTER,
  SY_FAT_POINTER,
  SY_INFERENCED_COUNT_ARRAY,
  SY_SYNONYM,

  // COUNTED SUBTYPE
  SY_ARRAY,

  // COMPOSITE SUBTYPE
  SY_LAYOUT,
  SY_SIGNATURE,
  SY_EXTENSION,

  // ARITHMETIC SEQUENCE
  SY_ARITHMETIC_INTERVAL,
  SY_FINITE_ARITHMETIC_PROGRESSION,
  SY_INFINITE_ARITHMETIC_PROGRESSION,

  // MISC
  SY_MODULE,
  SY_IMPORT,
  SY_CODE,
  SY_CATEGORY_DISCRIMINANT, // underlying type of category code
  SY_LABEL,

  // BINDING
  SY_DYNAMIC_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE, // entry within a category referencing a code and
                           // maybe a value type
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,

  // SYMBOL TABLE
  SY_TOP,
  SY_SCOPE,
  SY_TABLE,
  SY_CLASS,
  SY_ENUMERATION,
  SY_CATEGORY,

  // PROCEDURE
  SY_ENTRY,
  SY_FUNCTION,
  SY_METHOD,
  SY_RANGER,
  SY_EXTENSION_FUNCTION,
  SY_EXTENSION_METHOD,
  SY_EXTENSION_RANGER,

  // TEMPLATE
  SY_TEMPLATE_CLASS,
  SY_TEMPLATE_ENUMERATION,
  SY_TEMPLATE_CATEGORY,
  SY_TEMPLATE_DYNAMIC_VARIABLE,
  SY_TEMPLATE_STATIC_VARIABLE,
  SY_TEMPLATE_FUNCTION,
  SY_TEMPLATE_METHOD,
  SY_TEMPLATE_RANGER,
  SY_TEMPLATE_EXTENSION_FUNCTION,
  SY_TEMPLATE_EXTENSION_METHOD,
  SY_TEMPLATE_EXTENSION_RANGER,

  // PARTIAL SPECIALIZATION
  SY_PARTIAL_CLASS,
  SY_PARTIAL_ENUMERATION,
  SY_PARTIAL_CATEGORY,
  SY_PARTIAL_DYNAMIC_VARIABLE,
  SY_PARTIAL_STATIC_VARIABLE,
  SY_PARTIAL_FUNCTION,
  SY_PARTIAL_METHOD,
  SY_PARTIAL_RANGER,
  SY_PARTIAL_EXTENSION_FUNCTION,
  SY_PARTIAL_EXTENSION_METHOD,
  SY_PARTIAL_EXTENSION_RANGER,

  // =====CONSTANTS=====

  CT_TYPE,
  CT_EXPRESSION,
  CT_BOOLEAN,
  CT_INTEGER,
  CT_FLOAT,
  CT_STRING,
  CT_ARRAY,

  // =====OPCODES=====

  OP_NONE,

  OP_LOGICAL_AND,
  OP_LOGICAL_OR,
  OP_LOGICAL_COMPLEMENT,

  OP_GREATER,
  OP_GREATER_EQUAL,
  OP_LESS,
  OP_LESS_EQUAL,
  OP_EQUAL,
  OP_NOT_EQUAL,

  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_MODULUS,
  OP_NEGATE,

  OP_CAST,

  OP_BITWISE_AND,
  OP_BITWISE_OR,
  OP_BITWISE_XOR,
  OP_BITWISE_COMPLEMENT,
  OP_BITWISE_SHIFT_LEFT,
  OP_BITWISE_SHIFT_RIGHT,
  OP_BITWISE_ROTATE_LEFT,
  OP_BITWISE_ROTATE_RIGHT,

  OP_CONTENT_OF,
  OP_ADDRESS_OF,
  OP_CALL,
  OP_DROP,
  OP_MOVE,
  OP_COPY,

  OP_RETURN,
  OP_GOTO,
  OP_CONDITION,

  OP_LOOP_SEQUENCE,
  OP_LOOP_ELEMENTS,
  OP_LOOP_RANGER,

  OP_DEBUG_TRAP,
  OP_UNREACHABLE,
  OP_ASSUME,

  LAST
};

static constexpr std::size_t ENTITY_COUNT =
    static_cast<std::size_t>(rq::EntityKind::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::EntityKind kind) {
  using E = rq::EntityKind;
  switch (kind) {
  case E::NONE:
    return "none";
  case E::SY_INFERENCE:
    return "sy_inference";
  case E::SY_GENERIC_SYMBOL:
    return "sy_generic_symbol";
  case E::SY_GENERIC_TYPE:
    return "sy_generic_type";
  case E::SY_EXPRESSION:
    return "sy_expression";
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
  case E::SY_GENERIC_SIGNED:
    return "sy_generic_signed";
  case E::SY_GENERIC_UNSIGNED:
    return "sy_generic_unsigned";
  case E::SY_GENERIC_FLOAT:
    return "sy_generic_float";
  case E::SY_GENERIC_BINARY:
    return "sy_generic_binary";
  case E::SY_GENERIC_BFLOAT:
    return "sy_generic_bfloat";
  case E::SY_HALF:
    return "sy_half";
  case E::SY_SINGLE:
    return "sy_single";
  case E::SY_DOUBLE:
    return "sy_double";
  case E::SY_QUADRUPLE:
    return "sy_quadruple";
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
  case E::SY_GENERIC_SIGNED_INTEGER:
    return "sy_generic_signed_integer";
  case E::SY_GENERIC_UNSIGNED_INTEGER:
    return "sy_generic_unsigned_integer";
  case E::SY_GENERIC_CODEUNIT:
    return "sy_generic_codeunit";
  case E::SY_GENERIC_STRING:
    return "sy_generic_string";
  case E::SY_ASCII:
    return "sy_ascii";
  case E::SY_UTF8:
    return "sy_utf8";
  case E::SY_SCALED_SIGNED_INTEGER:
    return "sy_scaled_signed_integer";
  case E::SY_SCALED_UNSIGNED_INTEGER:
    return "sy_scaled_unsigned_integer";
  case E::SY_REFERENCE:
    return "sy_reference";
  case E::SY_POINTER:
    return "sy_pointer";
  case E::SY_FAT_POINTER:
    return "sy_fat_pointer";
  case E::SY_INFERENCED_COUNT_ARRAY:
    return "sy_inferenced_count_array";
  case E::SY_ARRAY:
    return "sy_array";
  case E::SY_LAYOUT:
    return "sy_layout";
  case E::SY_SIGNATURE:
    return "sy_signature";
  case E::SY_EXTENSION:
    return "sy_extension";
  case E::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";
  case E::SY_SYNONYM:
    return "sy_synonym";
  case E::SY_MODULE:
    return "sy_module";
  case E::SY_IMPORT:
    return "sy_import";
  case E::SY_CODE:
    return "sy_code";
  case E::SY_CATEGORY_DISCRIMINANT:
    return "sy_category_discriminant";
  case E::SY_LABEL:
    return "sy_label";
  case E::SY_DYNAMIC_VARIABLE:
    return "sy_dynamic_variable";
  case E::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case E::SY_ENUMERATOR:
    return "sy_enumerator";
  case E::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
  case E::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case E::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case E::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case E::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
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
  case E::SY_CATEGORY:
    return "sy_category";
  case E::SY_ENTRY:
    return "sy_entry";
  case E::SY_FUNCTION:
    return "sy_function";
  case E::SY_METHOD:
    return "sy_method";
  case E::SY_RANGER:
    return "sy_ranger";
  case E::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case E::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case E::SY_EXTENSION_RANGER:
    return "sy_extension_ranger";
  case E::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case E::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case E::SY_TEMPLATE_CATEGORY:
    return "sy_template_category";
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return "sy_template_dynamic_variable";
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return "sy_template_static_variable";
  case E::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case E::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case E::SY_TEMPLATE_RANGER:
    return "sy_template_ranger";
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";
  case E::SY_TEMPLATE_EXTENSION_RANGER:
    return "sy_template_extension_ranger";
  case E::SY_PARTIAL_CLASS:
    return "sy_partial_class";
  case E::SY_PARTIAL_ENUMERATION:
    return "sy_partial_enumeration";
  case E::SY_PARTIAL_CATEGORY:
    return "sy_partial_category";
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return "sy_partial_dynamic_variable";
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return "sy_partial_static_variable";
  case E::SY_PARTIAL_FUNCTION:
    return "sy_partial_function";
  case E::SY_PARTIAL_METHOD:
    return "sy_partial_method";
  case E::SY_PARTIAL_RANGER:
    return "sy_partial_ranger";
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return "sy_partial_extension_function";
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return "sy_partial_extension_method";
  case E::SY_PARTIAL_EXTENSION_RANGER:
    return "sy_partial_extension_ranger";
  case E::CT_TYPE:
    return "ct_type";
  case E::CT_EXPRESSION:
    return "ct_expression";
  case E::CT_BOOLEAN:
    return "ct_boolean";
  case E::CT_INTEGER:
    return "ct_integer";
  case E::CT_FLOAT:
    return "ct_float";
  case E::CT_STRING:
    return "ct_string";
  case E::CT_ARRAY:
    return "ct_array";
  case E::OP_NONE:
    return "op_none";
  case E::OP_LOGICAL_AND:
    return "op_logical_and";
  case E::OP_LOGICAL_OR:
    return "op_logical_or";
  case E::OP_LOGICAL_COMPLEMENT:
    return "op_logical_complement";
  case E::OP_GREATER:
    return "op_greater";
  case E::OP_GREATER_EQUAL:
    return "op_greater_equal";
  case E::OP_LESS:
    return "op_less";
  case E::OP_LESS_EQUAL:
    return "op_less_equal";
  case E::OP_EQUAL:
    return "op_equal";
  case E::OP_NOT_EQUAL:
    return "op_not_equal";
  case E::OP_ADD:
    return "op_add";
  case E::OP_SUBTRACT:
    return "op_subtract";
  case E::OP_MULTIPLY:
    return "op_multiply";
  case E::OP_DIVIDE:
    return "op_divide";
  case E::OP_MODULUS:
    return "op_modulus";
  case E::OP_NEGATE:
    return "op_negate";
  case E::OP_CAST:
    return "op_cast";
  case E::OP_BITWISE_AND:
    return "op_bitwise_and";
  case E::OP_BITWISE_OR:
    return "op_bitwise_or";
  case E::OP_BITWISE_XOR:
    return "op_bitwise_xor";
  case E::OP_BITWISE_COMPLEMENT:
    return "op_bitwise_complement";
  case E::OP_BITWISE_SHIFT_LEFT:
    return "op_bitwise_shift_left";
  case E::OP_BITWISE_SHIFT_RIGHT:
    return "op_bitwise_shift_right";
  case E::OP_BITWISE_ROTATE_LEFT:
    return "op_bitwise_rotate_left";
  case E::OP_BITWISE_ROTATE_RIGHT:
    return "op_bitwise_rotate_right";
  case E::OP_CONTENT_OF:
    return "op_content_of";
  case E::OP_ADDRESS_OF:
    return "op_address_of";
  case E::OP_CALL:
    return "op_call";
  case E::OP_DROP:
    return "op_drop";
  case E::OP_MOVE:
    return "op_move";
  case E::OP_COPY:
    return "op_copy";
  case E::OP_RETURN:
    return "op_return";
  case E::OP_GOTO:
    return "op_goto";
  case E::OP_CONDITION:
    return "op_condition";
  case E::OP_LOOP_SEQUENCE:
    return "op_loop_sequence";
  case E::OP_LOOP_ELEMENTS:
    return "op_loop_elements";
  case E::OP_LOOP_RANGER:
    return "op_loop_ranger";
  case E::OP_DEBUG_TRAP:
    return "op_debug_trap";
  case E::OP_UNREACHABLE:
    return "op_unreachable";
  case E::OP_ASSUME:
    return "op_assume";
  case E::LAST:
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
  NOT_OPCODE_MASK = (rq::getBit(0) | rq::getBit(1)),

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
  SY_SUBTYPE = rq::getBit(14),
  SY_GENERIC = rq::getBit(15),
  SY_CONCRETE = rq::getBit(16),
  SY_PLATFORM_CHANGING = rq::getBit(17),
  SY_INTEGER = rq::getBit(18),
  SY_FLOAT = rq::getBit(19),
  SY_BINARY = rq::getBit(20),
  SY_CODEUNIT = rq::getBit(21),
  SY_SIGNED = rq::getBit(22),
  SY_UNSIGNED = rq::getBit(23),
  SY_TOP_OF_FRAME = rq::getBit(24)

  // CONSTANT FLAGS
  // TODO

  // INSTRUCTION FLAGS
  // TODO
};
template <> struct is_flags<EntityFlags> : std::true_type {};
[[nodiscard]] inline rq::EntityFlags getFlags(rq::EntityKind kind) {
  using E = rq::EntityKind;
  using EF = rq::EntityFlags;
  switch (kind) {
  case E::NONE:
    return EF::NONE;
  case E::SY_INFERENCE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_GENERIC_SYMBOL:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_GENERIC_TYPE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_EXPRESSION:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE;
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
  case E::SY_GENERIC_SIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_SIGNED;
  case E::SY_GENERIC_UNSIGNED:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_GENERIC_FLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_GENERIC_BINARY:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_FLOAT | EF::SY_BINARY | EF::SY_SIGNED;
  case E::SY_GENERIC_BFLOAT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_HALF:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_PLATFORM_CHANGING | EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_SINGLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_PLATFORM_CHANGING | EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_DOUBLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_PLATFORM_CHANGING | EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_QUADRUPLE:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_PLATFORM_CHANGING | EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_BINARY16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_FLOAT | EF::SY_BINARY | EF::SY_SIGNED;
  case E::SY_BINARY32:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_FLOAT | EF::SY_BINARY | EF::SY_SIGNED;
  case E::SY_BINARY64:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_FLOAT | EF::SY_BINARY | EF::SY_SIGNED;
  case E::SY_BINARY128:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_FLOAT | EF::SY_BINARY | EF::SY_SIGNED;
  case E::SY_BFLOAT16:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_FLOAT | EF::SY_SIGNED;
  case E::SY_GENERIC_INTEGER:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_INTEGER;
  case E::SY_GENERIC_SIGNED_INTEGER:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_INTEGER | EF::SY_SIGNED;
  case E::SY_GENERIC_UNSIGNED_INTEGER:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_INTEGER | EF::SY_UNSIGNED;
  case E::SY_GENERIC_CODEUNIT:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC |
           EF::SY_CODEUNIT;
  case E::SY_GENERIC_STRING:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_GENERIC;
  case E::SY_ASCII:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_CODEUNIT;
  case E::SY_UTF8:
    return EF::SYMBOL | EF::SY_SIMPLE_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_CODEUNIT;
  case E::SY_SCALED_SIGNED_INTEGER:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_INTEGER | EF::SY_SIGNED;
  case E::SY_SCALED_UNSIGNED_INTEGER:
    return EF::SYMBOL | EF::SY_SCALED_BUILTIN | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_INTEGER | EF::SY_UNSIGNED;
  case E::SY_REFERENCE:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_FAT_POINTER:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_INFERENCED_COUNT_ARRAY:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_GENERIC;
  case E::SY_SYNONYM:
    return EF::SYMBOL | EF::SY_UNARY_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_ARRAY:
    return EF::SYMBOL | EF::SY_COUNTED_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_LAYOUT:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_SIGNATURE:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_EXTENSION:
    return EF::SYMBOL | EF::SY_COMPOSITE_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_ARITHMETIC_INTERVAL:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return EF::SYMBOL | EF::SY_ARITHMETIC_SEQUENCE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_MODULE:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_IMPORT:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_CODE:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_CATEGORY_DISCRIMINANT:
    return EF::SYMBOL | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_LABEL:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_ENUMERATOR:
    return EF::SYMBOL | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_CATEGORY_ALTERNATIVE:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_CLASS_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_LAYOUT_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_SIGNATURE_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_TOP:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_SCOPE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_CONCRETE;
  case E::SY_TABLE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_CLASS:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_ENUMERATION:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_CATEGORY:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_ENTRY:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_EXTENSION_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_TEMPLATE_CLASS:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_ENUMERATION:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_CATEGORY:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_RANGER:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_EXTENSION_RANGER:
    return EF::SYMBOL | EF::SY_TEMPLATE | EF::SY_CONCRETE;
  case E::SY_PARTIAL_CLASS:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_ENUMERATION:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_CATEGORY:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_RANGER:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::SY_PARTIAL_EXTENSION_RANGER:
    return EF::SYMBOL | EF::SY_PARTIAL | EF::SY_CONCRETE;
  case E::CT_TYPE:
    return EF::CONSTANT;
  case E::CT_EXPRESSION:
    return EF::CONSTANT;
  case E::CT_BOOLEAN:
    return EF::CONSTANT;
  case E::CT_INTEGER:
    return EF::CONSTANT;
  case E::CT_FLOAT:
    return EF::CONSTANT;
  case E::CT_STRING:
    return EF::CONSTANT;
  case E::CT_ARRAY:
    return EF::CONSTANT;
  case E::OP_NONE:
    return EF::NONE;
  case E::OP_LOGICAL_AND:
    return EF::NONE;
  case E::OP_LOGICAL_OR:
    return EF::NONE;
  case E::OP_LOGICAL_COMPLEMENT:
    return EF::NONE;
  case E::OP_GREATER:
    return EF::NONE;
  case E::OP_GREATER_EQUAL:
    return EF::NONE;
  case E::OP_LESS:
    return EF::NONE;
  case E::OP_LESS_EQUAL:
    return EF::NONE;
  case E::OP_EQUAL:
    return EF::NONE;
  case E::OP_NOT_EQUAL:
    return EF::NONE;
  case E::OP_ADD:
    return EF::NONE;
  case E::OP_SUBTRACT:
    return EF::NONE;
  case E::OP_MULTIPLY:
    return EF::NONE;
  case E::OP_DIVIDE:
    return EF::NONE;
  case E::OP_MODULUS:
    return EF::NONE;
  case E::OP_NEGATE:
    return EF::NONE;
  case E::OP_CAST:
    return EF::NONE;
  case E::OP_BITWISE_AND:
    return EF::NONE;
  case E::OP_BITWISE_OR:
    return EF::NONE;
  case E::OP_BITWISE_XOR:
    return EF::NONE;
  case E::OP_BITWISE_COMPLEMENT:
    return EF::NONE;
  case E::OP_BITWISE_SHIFT_LEFT:
    return EF::NONE;
  case E::OP_BITWISE_SHIFT_RIGHT:
    return EF::NONE;
  case E::OP_BITWISE_ROTATE_LEFT:
    return EF::NONE;
  case E::OP_BITWISE_ROTATE_RIGHT:
    return EF::NONE;
  case E::OP_CONTENT_OF:
    return EF::NONE;
  case E::OP_ADDRESS_OF:
    return EF::NONE;
  case E::OP_CALL:
    return EF::NONE;
  case E::OP_DROP:
    return EF::NONE;
  case E::OP_MOVE:
    return EF::NONE;
  case E::OP_COPY:
    return EF::NONE;
  case E::OP_RETURN:
    return EF::NONE;
  case E::OP_GOTO:
    return EF::NONE;
  case E::OP_CONDITION:
    return EF::NONE;
  case E::OP_LOOP_SEQUENCE:
    return EF::NONE;
  case E::OP_LOOP_ELEMENTS:
    return EF::NONE;
  case E::OP_LOOP_RANGER:
    return EF::NONE;
  case E::OP_DEBUG_TRAP:
    return EF::NONE;
  case E::OP_UNREACHABLE:
    return EF::NONE;
  case E::OP_ASSUME:
    return EF::NONE;
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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOpcode(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasNone(flags, rq::EntityFlags::NOT_OPCODE_MASK);
}

#define RQ_ASSERT_SYMBOL(kind) RQ_ASSERT(rq::getIsSymbol((kind)), "not symbol")
#define RQ_ASSERT_CONSTANT(kind)                                               \
  RQ_ASSERT(rq::getIsConstant((kind)), "not constant")

#define RQ_ASSERT_OPCODE(opcode)                                               \
  RQ_ASSERT(rq::getIsOpcode(opcode), "not opcode")

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIMPLE_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SCALED_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNARY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COUNTED_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_COMPOSITE_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsArithmeticSequence(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_ARITHMETIC_SEQUENCE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SYMBOL_TABLE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PROCEDURE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TEMPLATE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PARTIAL);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_HAS_TEMPLATE_ALTERNATIVE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsGeneric(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_GENERIC);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcrete(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CONCRETE);
}
// NOTE: SCALED_SIGNED_INTEGER and SCALED_UNSIGNED_INTEGER is platform chaning
// only if depth is not exact that is checked in member function of Entity, not
// here.
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PLATFORM_CHANGING);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags,
                       rq::EntityFlags::SY_INTEGER | rq::EntityFlags::SY_FLOAT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_FLOAT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_BINARY);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_CODEUNIT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNSIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_SIGNED |
                                  rq::EntityFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_UNSIGNED |
                                  rq::EntityFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_TOP_OF_FRAME);
}
[[nodiscard]] inline rq::EntityKind getTemplate(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  RQ_ASSERT(rq::getHasTemplateAlternative(kind), "no template alternative");
  using E = rq::EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    [[fallthrough]];
  case E::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case E::SY_PARTIAL_CLASS:
    return E::SY_TEMPLATE_CLASS;
  case E::SY_ENUMERATION:
    [[fallthrough]];
  case E::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_TEMPLATE_ENUMERATION;
  case E::SY_CATEGORY:
    [[fallthrough]];
  case E::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case E::SY_PARTIAL_CATEGORY:
    return E::SY_TEMPLATE_CATEGORY;
  case E::SY_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_TEMPLATE_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_TEMPLATE_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_TEMPLATE_FUNCTION;
  case E::SY_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_METHOD:
    return E::SY_TEMPLATE_METHOD;
  case E::SY_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_RANGER:
    return E::SY_TEMPLATE_RANGER;
  case E::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_TEMPLATE_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_TEMPLATE_EXTENSION_METHOD;
  case E::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_RANGER:
    return E::SY_TEMPLATE_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] inline rq::EntityKind getPartial(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  RQ_ASSERT(rq::getHasTemplateAlternative(kind), "no template alternative");
  using E = rq::EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    [[fallthrough]];
  case E::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case E::SY_PARTIAL_CLASS:
    return E::SY_PARTIAL_CLASS;
  case E::SY_ENUMERATION:
    [[fallthrough]];
  case E::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_PARTIAL_ENUMERATION;
  case E::SY_CATEGORY:
    [[fallthrough]];
  case E::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case E::SY_PARTIAL_CATEGORY:
    return E::SY_PARTIAL_CATEGORY;
  case E::SY_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_PARTIAL_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_PARTIAL_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_PARTIAL_FUNCTION;
  case E::SY_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_METHOD:
    return E::SY_PARTIAL_METHOD;
  case E::SY_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_RANGER:
    return E::SY_PARTIAL_RANGER;
  case E::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_PARTIAL_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_PARTIAL_EXTENSION_METHOD;
  case E::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_RANGER:
    return E::SY_PARTIAL_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] inline rq::EntityKind getFull(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  RQ_ASSERT(rq::getHasTemplateAlternative(kind), "no template alternative");
  using E = rq::EntityKind;
  switch (kind) {
  case E::SY_CLASS:
    [[fallthrough]];
  case E::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case E::SY_PARTIAL_CLASS:
    return E::SY_CLASS;
  case E::SY_ENUMERATION:
    [[fallthrough]];
  case E::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case E::SY_PARTIAL_ENUMERATION:
    return E::SY_ENUMERATION;
  case E::SY_CATEGORY:
    [[fallthrough]];
  case E::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case E::SY_PARTIAL_CATEGORY:
    return E::SY_CATEGORY;
  case E::SY_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_DYNAMIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_DYNAMIC_VARIABLE:
    return E::SY_DYNAMIC_VARIABLE;
  case E::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case E::SY_PARTIAL_STATIC_VARIABLE:
    return E::SY_STATIC_VARIABLE;
  case E::SY_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_FUNCTION:
    return E::SY_FUNCTION;
  case E::SY_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_METHOD:
    return E::SY_METHOD;
  case E::SY_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_RANGER:
    return E::SY_RANGER;
  case E::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_FUNCTION:
    return E::SY_EXTENSION_FUNCTION;
  case E::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_METHOD:
    return E::SY_EXTENSION_METHOD;
  case E::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case E::SY_PARTIAL_EXTENSION_RANGER:
    return E::SY_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

struct Entity;
struct Symbol;
struct SimpleBuiltin;
struct Inference;
struct GenericSymbol;
struct GenericType;
struct Void;
struct Null;
struct NoReturn;
struct VariadicArguments;
struct Boolean;
struct GenericSigned;
struct GenericUnsigned;
struct GenericFloat;
struct GenericBinary;
struct GenericBFloat;
struct Half;
struct Single;
struct Double;
struct Quadruple;
struct Binary16;
struct Binary32;
struct Binary64;
struct Binary128;
struct BFloat16;
struct GenericInteger;
struct GenericSignedInteger;
struct GenericUnsignedInteger;
struct GenericCodeunit;
struct GenericString;
struct Ascii;
struct Utf8;
struct ScaledBuiltin;
struct ScaledSignedInteger;
struct ScaledUnsignedInteger;
struct UnarySubtype;
struct Reference;
struct Pointer;
struct FatPointer;
struct InferencedCountArray;
struct CountedSubtype;
struct Array;
struct CompositeSubtype;
struct Layout;
struct Signature;
struct Extension;
struct ArithmeticSequence;
struct ArithmeticInterval;
struct FiniteArithmeticProgression;
struct InfiniteArithmeticProgression;
struct Synonym;
struct Module;
struct Import;
struct Code;
struct CategoryDiscriminant;
struct Label;
struct DynamicVariable;
struct StaticVariable;
struct Enumerator;
struct CategoryAlternative;
struct ClassParameter;
struct LayoutParameter;
struct TemplateParameter;
struct SignatureParameter;
struct SymbolTable;
struct Top;
struct Scope;
struct Table;
struct Class;
struct Enumeration;
struct Category;
struct Procdedure;
struct Entry;
struct Function;
struct Method;
struct Ranger;
struct ExtensionFunction;
struct ExtensionMethod;
struct ExtensionRanger;
struct Template;
struct TemplateClass;
struct TemplateEnumeration;
struct TemplateCategory;
struct TemplateDynamicVariable;
struct TemplateStaticVariable;
struct TemplateFunction;
struct TemplateMethod;
struct TemplateRanger;
struct TemplateExtensionFunction;
struct TemplateExtensionMethod;
struct TemplateExtensionRanger;
struct Partial;
struct PartialClass;
struct PartialEnumeration;
struct PartialCategory;
struct PartialDynamicVariable;
struct PartialStaticVariable;
struct PartialFunction;
struct PartialMethod;
struct PartialRanger;
struct PartialExtensionFunction;
struct PartialExtensionMethod;
struct PartialExtensionRanger;
struct Constant;
struct TypeConstant;
struct ExpressionConstant;
struct BooleanConstant;
struct IntegerConstant;
struct FloatConstant;
struct StringConstant;
struct ArrayConstant;
struct Instruction;

struct InitialExpression {
  using Self = rq::InitialExpression;

  rq::Expression *_expression_ptr{nullptr};

  InitialExpression() = default;
  explicit InitialExpression(rq::Expression &expression)
      : _expression_ptr(&expression) {}
  InitialExpression(const Self &) = delete;
  InitialExpression(Self &&) = delete;
  ~InitialExpression() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};
struct LateExpression : public rq::InitialExpression {
  using Self = rq::LateExpression;

  LateExpression() = default;
  explicit LateExpression(rq::Expression &expression)
      : InitialExpression(expression) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
};
struct ReplacableExpression : public rq::LateExpression {
  using Self = rq::ReplacableExpression;

  ReplacableExpression() = default;
  explicit ReplacableExpression(rq::Expression &expression)
      : LateExpression(expression) {}
  RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression) {
    return rq::replaceValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression *expression) {
    return rq::replaceValue(this->_expression_ptr, expression);
  }
  RQ_ALWAYS_INLINE rq::Expression *
  replaceExpressionPtr(rq::Expression &expression) {
    return rq::replaceValuPtr(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE rq::Expression *
  replaceExpressionPtr(rq::Expression *expression) {
    return rq::replaceValuPtr(this->_expression_ptr, expression);
  }
};
struct InitialExpressionAttributes {
  using Self = InitialExpressionAttributes;

  rq::ExpressionAttributeFlags _attributes;

  InitialExpressionAttributes(rq::ExpressionAttributeFlags attributes)
      : _attributes(attributes) {}
  InitialExpressionAttributes(const Self &) = delete;
  InitialExpressionAttributes(Self &&) = delete;
  virtual ~InitialExpressionAttributes() {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOk() const {
    return rq::getHasOk(this->_attributes);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::ExpressionAttribute attribute) const {
    return rq::getHasAttribute(this->_attributes, attribute);
  }
};

struct Entity {
  using Self = rq::Entity;

  rq::EntityKind _kind;

  explicit Entity(rq::EntityKind k) : _kind(k) {}
  Entity() = delete;
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  Entity &operator=(const Entity &) = delete;
  Entity &operator=(Entity &&) = delete;
  ~Entity() = default;

  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Entity &other) const {
    return this == &other;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Entity &other) const {
    return this != &other;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getKind() const {
    return _kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const {
    return rq::getHasTemplateAlternative(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return rq::getIsType(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const {
    return rq::getIsSubtype(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGeneric() const {
    return rq::getIsGeneric(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcrete() const {
    return rq::getIsConcrete(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging() const {
    if (rq::getIsScaledBuiltin(this->getKind())) {
      // TODO
    }
    return rq::getIsPlatformChanging(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsInteger(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary() const {
    return rq::getIsBinary(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunit(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return rq::getIsSigned(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return rq::getIsUnsigned(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger() const {
    return rq::getIsSignedInteger(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger() const {
    return rq::getIsUnsignedInteger(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame() const {
    return rq::getIsTopOfFrame(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getTemplateKind() const {
    return rq::getTemplate(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getPartialKind() const {
    return rq::getPartial(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getFullKind() const {
    return rq::getFull(this->getKind());
  }

  static bool classof(const Entity *entity) {
    std::ignore = entity;
    return true;
  }
};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  explicit Symbol(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSymbol(rq::dereferencePtr(entity).getKind());
  }
};

struct SimpleBuiltin : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  explicit SimpleBuiltin(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSimpleBuiltin(rq::dereferencePtr(entity).getKind());
  }
};

struct Inference : public rq::SimpleBuiltin {
  using Self = rq::Inference;

  Inference() : SimpleBuiltin(rq::EntityKind::SY_INFERENCE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_INFERENCE;
  }
};

struct GenericSymbol : public rq::SimpleBuiltin {
  using Self = rq::GenericSymbol;

  GenericSymbol() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SYMBOL) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SYMBOL;
  }
};

struct GenericType : public rq::SimpleBuiltin {
  using Self = rq::GenericType;

  GenericType() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_TYPE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_TYPE;
  }
};

struct Void : public rq::SimpleBuiltin {
  using Self = rq::Void;

  Void() : SimpleBuiltin(rq::EntityKind::SY_VOID) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_VOID;
  }
};

struct Null : public rq::SimpleBuiltin {
  using Self = rq::Null;

  Null() : SimpleBuiltin(rq::EntityKind::SY_NULL) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_NULL;
  }
};

struct NoReturn : public rq::SimpleBuiltin {
  using Self = rq::NoReturn;

  NoReturn() : SimpleBuiltin(rq::EntityKind::SY_NO_RETURN) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_NO_RETURN;
  }
};

struct VariadicArguments : public rq::SimpleBuiltin {
  using Self = rq::VariadicArguments;

  VariadicArguments() : SimpleBuiltin(rq::EntityKind::SY_VARIADIC_ARGUMENTS) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_VARIADIC_ARGUMENTS;
  }
};

struct Boolean : public rq::SimpleBuiltin {
  using Self = rq::Boolean;

  Boolean() : SimpleBuiltin(rq::EntityKind::SY_BOOLEAN) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BOOLEAN;
  }
};

struct GenericSigned : public rq::SimpleBuiltin {
  using Self = rq::GenericSigned;

  GenericSigned() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SIGNED) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SIGNED;
  }
};

struct GenericUnsigned : public rq::SimpleBuiltin {
  using Self = rq::GenericUnsigned;

  GenericUnsigned() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_UNSIGNED) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_UNSIGNED;
  }
};

struct GenericFloat : public rq::SimpleBuiltin {
  using Self = rq::GenericFloat;

  GenericFloat() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_FLOAT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_FLOAT;
  }
};

struct GenericBinary : public rq::SimpleBuiltin {
  using Self = rq::GenericBinary;

  GenericBinary() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_BINARY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_BINARY;
  }
};

struct GenericBFloat : public rq::SimpleBuiltin {
  using Self = rq::GenericBFloat;

  GenericBFloat() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_BFLOAT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_BFLOAT;
  }
};

struct Half : public rq::SimpleBuiltin {
  using Self = rq::Half;

  Half() : SimpleBuiltin(rq::EntityKind::SY_HALF) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_HALF;
  }
};

struct Single : public rq::SimpleBuiltin {
  using Self = rq::Single;

  Single() : SimpleBuiltin(rq::EntityKind::SY_SINGLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SINGLE;
  }
};

struct Double : public rq::SimpleBuiltin {
  using Self = rq::Double;

  Double() : SimpleBuiltin(rq::EntityKind::SY_DOUBLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_DOUBLE;
  }
};

struct Quadruple : public rq::SimpleBuiltin {
  using Self = rq::Quadruple;

  Quadruple() : SimpleBuiltin(rq::EntityKind::SY_QUADRUPLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_QUADRUPLE;
  }
};

struct Binary16 : public rq::SimpleBuiltin {
  using Self = rq::Binary16;

  Binary16() : SimpleBuiltin(rq::EntityKind::SY_BINARY16) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY16;
  }
};

struct Binary32 : public rq::SimpleBuiltin {
  using Self = rq::Binary32;

  Binary32() : SimpleBuiltin(rq::EntityKind::SY_BINARY32) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY32;
  }
};

struct Binary64 : public rq::SimpleBuiltin {
  using Self = rq::Binary64;

  Binary64() : SimpleBuiltin(rq::EntityKind::SY_BINARY64) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY64;
  }
};

struct Binary128 : public rq::SimpleBuiltin {
  using Self = rq::Binary128;

  Binary128() : SimpleBuiltin(rq::EntityKind::SY_BINARY128) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY128;
  }
};

struct BFloat16 : public rq::SimpleBuiltin {
  using Self = rq::BFloat16;

  BFloat16() : SimpleBuiltin(rq::EntityKind::SY_BFLOAT16) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BFLOAT16;
  }
};

struct GenericInteger : public rq::SimpleBuiltin {
  using Self = rq::GenericInteger;

  GenericInteger() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_INTEGER;
  }
};

struct GenericSignedInteger : public rq::SimpleBuiltin {
  using Self = rq::GenericSignedInteger;

  GenericSignedInteger()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SIGNED_INTEGER;
  }
};

struct GenericUnsignedInteger : public rq::SimpleBuiltin {
  using Self = rq::GenericUnsignedInteger;

  GenericUnsignedInteger()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER;
  }
};

struct GenericCodeunit : public rq::SimpleBuiltin {
  using Self = rq::GenericCodeunit;

  GenericCodeunit() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_CODEUNIT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_CODEUNIT;
  }
};

struct GenericString : public rq::SimpleBuiltin {
  using Self = rq::GenericString;

  GenericString() : SimpleBuiltin(rq::EntityKind::SY_GENERIC_STRING) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_STRING;
  }
};

struct Ascii : public rq::SimpleBuiltin {
  using Self = rq::Ascii;

  Ascii() : SimpleBuiltin(rq::EntityKind::SY_ASCII) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ASCII;
  }
};

struct Utf8 : public rq::SimpleBuiltin {
  using Self = rq::Utf8;

  Utf8() : SimpleBuiltin(rq::EntityKind::SY_UTF8) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_UTF8;
  }
};

struct ScaledBuiltin : public rq::Symbol {
  using Self = rq::ScaledBuiltin;

  explicit ScaledBuiltin(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsScaledBuiltin(rq::dereferencePtr(entity).getKind());
  }
};

struct ScaledSignedInteger : public rq::ScaledBuiltin {
  using Self = rq::ScaledSignedInteger;

  ScaledSignedInteger()
      : ScaledBuiltin(rq::EntityKind::SY_SCALED_SIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SCALED_SIGNED_INTEGER;
  }
};

struct ScaledUnsignedInteger : public rq::Entity {
  using Self = rq::ScaledUnsignedInteger;

  ScaledUnsignedInteger()
      : Entity(rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER;
  }
};

struct UnarySubtype : public rq::Symbol {
  using Self = rq::UnarySubtype;

  explicit UnarySubtype(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsUnarySubtype(rq::dereferencePtr(entity).getKind());
  }
};

struct Reference : public rq::UnarySubtype {
  using Self = rq::Reference;

  Reference() : UnarySubtype(rq::EntityKind::SY_REFERENCE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_REFERENCE;
  }
};

struct Pointer : public rq::UnarySubtype {
  using Self = rq::Pointer;

  Pointer() : UnarySubtype(rq::EntityKind::SY_POINTER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_POINTER;
  }
};

struct FatPointer : public rq::UnarySubtype {
  using Self = rq::FatPointer;

  FatPointer() : UnarySubtype(rq::EntityKind::SY_FAT_POINTER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_FAT_POINTER;
  }
};

struct InferencedCountArray : public rq::UnarySubtype {
  using Self = rq::InferencedCountArray;

  InferencedCountArray()
      : UnarySubtype(rq::EntityKind::SY_INFERENCED_COUNT_ARRAY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
  }
};

struct CountedSubtype : public rq::Symbol {
  using Self = rq::CountedSubtype;

  explicit CountedSubtype(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsCountedSubtype(rq::dereferencePtr(entity).getKind());
  }
};

struct Array : public rq::CountedSubtype {
  using Self = rq::Array;

  Array() : CountedSubtype(rq::EntityKind::SY_ARRAY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ARRAY;
  }
};

struct CompositeSubtype : public rq::Symbol {
  using Self = rq::CompositeSubtype;

  explicit CompositeSubtype(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsCompositeSubtype(rq::dereferencePtr(entity).getKind());
  }
};

struct Layout : public rq::CompositeSubtype {
  using Self = rq::Layout;

  Layout() : CompositeSubtype(rq::EntityKind::SY_LAYOUT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_LAYOUT;
  }
};

struct Signature : public rq::CompositeSubtype {
  using Self = rq::Signature;

  Signature() : CompositeSubtype(rq::EntityKind::SY_SIGNATURE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SIGNATURE;
  }
};

struct Extension : public rq::CompositeSubtype {
  using Self = rq::Extension;

  Extension() : CompositeSubtype(rq::EntityKind::SY_EXTENSION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_EXTENSION;
  }
};

struct ArithmeticSequence : public rq::Symbol {
  using Self = rq::ArithmeticSequence;

  explicit ArithmeticSequence(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsArithmeticSequence(rq::dereferencePtr(entity).getKind());
  }
};

struct ArithmeticInterval : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;

  ArithmeticInterval()
      : ArithmeticSequence(rq::EntityKind::SY_ARITHMETIC_INTERVAL) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ARITHMETIC_INTERVAL;
  }
};

struct FiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticProgression;

  FiniteArithmeticProgression()
      : ArithmeticSequence(rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
};

struct InfiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticProgression;

  InfiniteArithmeticProgression()
      : ArithmeticSequence(rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION) {
  }

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
};

struct Synonym : public rq::Symbol {
  using Self = rq::Synonym;

  Synonym() : Symbol(rq::EntityKind::SY_SYNONYM) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SYNONYM;
  }
};

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind kind) {
  using M = rq::ModuleKind;
  switch (kind) {
  case M::NONE:
    return "none";
  case M::SOURCE:
    return "source";
  case M::IMPORT:
    return "import";
  }
  RQ_UNREACHABLE();
}

struct Module : public rq::Symbol, public rq::ReplacableExpression {
  using Self = rq::Module;

  rq::ModuleKind _module_kind;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;

  Module(rq::ModuleKind kind, llvm::StringRef path,
         llvm::MemoryBufferRef &&buffer)
      : Symbol(rq::EntityKind::SY_MODULE), _module_kind(kind), _path(path),
        _buffer(buffer) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const {
    return this->_module_kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getPath() const {
    return this->_path;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
    return this->_buffer.getBuffer();
  }
  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_MODULE;
  }
};

struct Import : public rq::Symbol {
  using Self = rq::Import;

  Import() : Symbol(rq::EntityKind::SY_IMPORT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_IMPORT;
  }
};

struct Code : public rq::Symbol {
  using Self = rq::Code;

  Code() : Symbol(rq::EntityKind::SY_CODE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CODE;
  }
};

struct CategoryDiscriminant : public rq::Symbol {
  using Self = rq::CategoryDiscriminant;

  CategoryDiscriminant() : Symbol(rq::EntityKind::SY_CATEGORY_DISCRIMINANT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CATEGORY_DISCRIMINANT;
  }
};

struct Label : public rq::Symbol {
  using Self = rq::Label;

  Label() : Symbol(rq::EntityKind::SY_LABEL) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_LABEL;
  }
};

struct DynamicVariable : public rq::Symbol {
  using Self = rq::DynamicVariable;

  DynamicVariable() : Symbol(rq::EntityKind::SY_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_DYNAMIC_VARIABLE;
  }
};

struct StaticVariable : public rq::Symbol {
  using Self = rq::StaticVariable;

  StaticVariable() : Symbol(rq::EntityKind::SY_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_STATIC_VARIABLE;
  }
};

struct Enumerator : public rq::Symbol {
  using Self = rq::Enumerator;

  Enumerator() : Symbol(rq::EntityKind::SY_ENUMERATOR) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ENUMERATOR;
  }
};

struct CategoryAlternative : public rq::Symbol {
  using Self = rq::CategoryAlternative;

  CategoryAlternative() : Symbol(rq::EntityKind::SY_CATEGORY_ALTERNATIVE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CATEGORY_ALTERNATIVE;
  }
};

struct ClassParameter : public rq::Symbol {
  using Self = rq::ClassParameter;

  ClassParameter() : Symbol(rq::EntityKind::SY_CLASS_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CLASS_PARAMETER;
  }
};

struct LayoutParameter : public rq::Symbol {
  using Self = rq::LayoutParameter;

  LayoutParameter() : Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_LAYOUT_PARAMETER;
  }
};

struct TemplateParameter : public rq::Symbol {
  using Self = rq::TemplateParameter;

  TemplateParameter() : Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_PARAMETER;
  }
};

struct SignatureParameter : public rq::Symbol {
  using Self = rq::SignatureParameter;

  SignatureParameter() : Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SIGNATURE_PARAMETER;
  }
};

struct SymbolTable : public rq::Symbol {
  using Self = rq::SymbolTable;

  explicit SymbolTable(rq::EntityKind k) : Symbol(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsSymbolTable(rq::dereferencePtr(entity).getKind());
  }
};

struct Top : public rq::SymbolTable {
  using Self = rq::Top;

  Top() : SymbolTable(rq::EntityKind::SY_TOP) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_TOP;
  }
};

struct Scope : public rq::SymbolTable {
  using Self = rq::Scope;

  Scope() : SymbolTable(rq::EntityKind::SY_SCOPE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SCOPE;
  }
};

struct Table : public rq::SymbolTable {
  using Self = rq::Table;

  Table() : SymbolTable(rq::EntityKind::SY_TABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_TABLE;
  }
};

struct Class : public rq::SymbolTable {
  using Self = rq::Class;

  Class() : SymbolTable(rq::EntityKind::SY_CLASS) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CLASS;
  }
};

struct Enumeration : public rq::SymbolTable {
  using Self = rq::Enumeration;

  Enumeration() : SymbolTable(rq::EntityKind::SY_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ENUMERATION;
  }
};

struct Category : public rq::SymbolTable {
  using Self = rq::Category;

  Category() : SymbolTable(rq::EntityKind::SY_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CATEGORY;
  }
};

struct Procedure : public rq::SymbolTable {
  using Self = rq::Procedure;

  explicit Procedure(rq::EntityKind k) : SymbolTable(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsProcedure(rq::dereferencePtr(entity).getKind());
  }
};

struct Entry : public rq::Procedure {
  using Self = rq::Entry;

  Entry() : Procedure(rq::EntityKind::SY_ENTRY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ENTRY;
  }
};

struct Function : public rq::Procedure {
  using Self = rq::Function;

  Function() : Procedure(rq::EntityKind::SY_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_FUNCTION;
  }
};

struct Method : public rq::Procedure {
  using Self = rq::Method;

  Method() : Procedure(rq::EntityKind::SY_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_METHOD;
  }
};

struct Ranger : public rq::Procedure {
  using Self = rq::Ranger;

  Ranger() : Procedure(rq::EntityKind::SY_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_RANGER;
  }
};

struct ExtensionFunction : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  ExtensionFunction() : Procedure(rq::EntityKind::SY_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_FUNCTION;
  }
};

struct ExtensionMethod : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  ExtensionMethod() : Procedure(rq::EntityKind::SY_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_METHOD;
  }
};

struct ExtensionRanger : public rq::Procedure {
  using Self = rq::ExtensionRanger;

  ExtensionRanger() : Procedure(rq::EntityKind::SY_EXTENSION_RANGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_RANGER;
  }
};

struct Template : public rq::Entity {
  using Self = rq::Template;

  explicit Template(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsTemplate(rq::dereferencePtr(entity).getKind());
  }
};

struct TemplateClass : public rq::Template {
  using Self = rq::TemplateClass;

  TemplateClass() : Template(rq::EntityKind::SY_TEMPLATE_CLASS) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_CLASS;
  }
};

struct TemplateEnumeration : public rq::Template {
  using Self = rq::TemplateEnumeration;

  TemplateEnumeration() : Template(rq::EntityKind::SY_TEMPLATE_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_ENUMERATION;
  }
};

struct TemplateCategory : public rq::Template {
  using Self = rq::TemplateCategory;

  TemplateCategory() : Template(rq::EntityKind::SY_TEMPLATE_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_CATEGORY;
  }
};

struct TemplateDynamicVariable : public rq::Template {
  using Self = rq::TemplateDynamicVariable;

  TemplateDynamicVariable()
      : Template(rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
  }
};

struct TemplateStaticVariable : public rq::Template {
  using Self = rq::TemplateStaticVariable;

  TemplateStaticVariable()
      : Template(rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
  }
};

struct TemplateFunction : public rq::Template {
  using Self = rq::TemplateFunction;

  TemplateFunction() : Template(rq::EntityKind::SY_TEMPLATE_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_FUNCTION;
  }
};

struct TemplateMethod : public rq::Template {
  using Self = rq::TemplateMethod;

  TemplateMethod() : Template(rq::EntityKind::SY_TEMPLATE_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_METHOD;
  }
};

struct TemplateRanger : public rq::Template {
  using Self = rq::TemplateRanger;

  TemplateRanger() : Template(rq::EntityKind::SY_TEMPLATE_RANGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_RANGER;
  }
};

struct TemplateExtensionFunction : public rq::Template {
  using Self = rq::TemplateExtensionFunction;

  TemplateExtensionFunction()
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
};

struct TemplateExtensionMethod : public rq::Template {
  using Self = rq::TemplateExtensionMethod;

  TemplateExtensionMethod()
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
  }
};

struct TemplateExtensionRanger : public rq::Template {
  using Self = rq::TemplateExtensionRanger;

  TemplateExtensionRanger()
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_RANGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_RANGER;
  }
};

struct Partial : public rq::Entity {
  using Self = rq::Partial;

  explicit Partial(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsPartial(rq::dereferencePtr(entity).getKind());
  }
};

struct PartialClass : public rq::Partial {
  using Self = rq::PartialClass;

  PartialClass() : Partial(rq::EntityKind::SY_PARTIAL_CLASS) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_CLASS;
  }
};

struct PartialEnumeration : public rq::Partial {
  using Self = rq::PartialEnumeration;

  PartialEnumeration() : Partial(rq::EntityKind::SY_PARTIAL_ENUMERATION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_ENUMERATION;
  }
};

struct PartialCategory : public rq::Partial {
  using Self = rq::PartialCategory;

  PartialCategory() : Partial(rq::EntityKind::SY_PARTIAL_CATEGORY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_CATEGORY;
  }
};

struct PartialDynamicVariable : public rq::Partial {
  using Self = rq::PartialDynamicVariable;

  PartialDynamicVariable()
      : Partial(rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
  }
};

struct PartialStaticVariable : public rq::Partial {
  using Self = rq::PartialStaticVariable;

  PartialStaticVariable()
      : Partial(rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
  }
};

struct PartialFunction : public rq::Partial {
  using Self = rq::PartialFunction;

  PartialFunction() : Partial(rq::EntityKind::SY_PARTIAL_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_FUNCTION;
  }
};

struct PartialMethod : public rq::Partial {
  using Self = rq::PartialMethod;

  PartialMethod() : Partial(rq::EntityKind::SY_PARTIAL_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct PartialRanger : public rq::Partial {
  using Self = rq::PartialRanger;

  PartialRanger() : Partial(rq::EntityKind::SY_PARTIAL_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct PartialExtensionFunction : public rq::Partial {
  using Self = rq::PartialExtensionFunction;

  PartialExtensionFunction()
      : Partial(rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
  }
};

struct PartialExtensionMethod : public rq::Partial {
  using Self = rq::PartialExtensionMethod;

  PartialExtensionMethod()
      : Partial(rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
  }
};

struct PartialExtensionRanger : public rq::Partial {
  using Self = rq::PartialExtensionRanger;

  PartialExtensionRanger() : Partial(rq::EntityKind::SY_PARTIAL_METHOD) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  explicit Constant(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsConstant(rq::dereferencePtr(entity).getKind());
  }
};

struct TypeConstant : public rq::Constant {
  using Self = rq::TypeConstant;

  TypeConstant() : Constant(rq::EntityKind::CT_TYPE) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_TYPE;
  }
};

struct ExpressionConstant : public rq::Constant {
  using Self = rq::ExpressionConstant;

  ExpressionConstant() : Constant(rq::EntityKind::CT_EXPRESSION) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::CT_EXPRESSION;
  }
};

struct BooleanConstant : public rq::Constant {
  using Self = rq::BooleanConstant;

  BooleanConstant() : Constant(rq::EntityKind::CT_BOOLEAN) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_BOOLEAN;
  }
};

struct IntegerConstant : public rq::Constant {
  using Self = rq::IntegerConstant;

  IntegerConstant() : Constant(rq::EntityKind::CT_INTEGER) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_INTEGER;
  }
};

struct FloatConstant : public rq::Constant {
  using Self = rq::FloatConstant;

  FloatConstant() : Constant(rq::EntityKind::CT_FLOAT) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_FLOAT;
  }
};

struct StringConstant : public rq::Constant {
  using Self = rq::StringConstant;

  StringConstant() : Constant(rq::EntityKind::CT_STRING) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_STRING;
  }
};

struct ArrayConstant : public rq::Constant {
  using Self = rq::ArrayConstant;

  ArrayConstant() : Constant(rq::EntityKind::CT_ARRAY) {}

  static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_ARRAY;
  }
};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  explicit Instruction(rq::EntityKind k) : Entity(k) {}

  static bool classof(const Entity *entity) {
    return rq::getIsOpcode(rq::dereferencePtr(entity).getKind());
  }
};

} // namespace rq