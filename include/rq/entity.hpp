#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
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
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ranges>
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
  SY_GENERIC_Bfloat,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_Bfloat16,
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

  // COUNTED SUBTYPE
  SY_ARRAY,

  // PARAMETER LIST SUBTYPE
  SY_LAYOUT,
  SY_CLASS_LAYOUT,
  SY_TEMPLATE_LAYOUT,
  SY_SIGNATURE,
  SY_EXTENSION,

  // PARAMETER
  SY_CLASS_PARAMETER,
  SY_LAYOUT_PARAMETER,
  SY_TEMPLATE_PARAMETER,
  SY_SIGNATURE_PARAMETER,

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
  SY_SYNONYM,

  // BINDING
  SY_DYNAMIC_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE, // entry within a category referencing a code and
                           // maybe a value type

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
  case E::SY_GENERIC_Bfloat:
    return "sy_generic_Bfloat";
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
  case E::SY_Bfloat16:
    return "sy_Bfloat16";
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
  case E::SY_CLASS_LAYOUT:
    return "sy_class_layout";
  case E::SY_TEMPLATE_LAYOUT:
    return "sy_template_layout";
  case E::SY_SIGNATURE:
    return "sy_signature";
  case E::SY_EXTENSION:
    return "sy_extension";
  case E::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case E::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case E::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case E::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
  case E::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case E::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case E::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";
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
  case E::SY_SYNONYM:
    return "sy_synonym";
  case E::SY_DYNAMIC_VARIABLE:
    return "sy_dynamic_variable";
  case E::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case E::SY_ENUMERATOR:
    return "sy_enumerator";
  case E::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
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
  SY_PARAMETER = rq::getBit(6),
  SY_PARAMETER_LIST_SUBTYPE = rq::getBit(7),
  SY_ARITHMETIC_SEQUENCE = rq::getBit(8),
  SY_SYMBOL_TABLE = rq::getBit(9),
  SY_PROCEDURE = rq::getBit(10),
  SY_TEMPLATE = rq::getBit(11),
  SY_PARTIAL = rq::getBit(12),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(13),
  SY_TYPE = rq::getBit(14),
  SY_SUBTYPE = rq::getBit(15),
  SY_GENERIC = rq::getBit(16),
  SY_CONCRETE = rq::getBit(17),
  SY_PLATFORM_CHANGING = rq::getBit(18),
  SY_INTEGER = rq::getBit(19),
  SY_FLOAT = rq::getBit(20),
  SY_BINARY = rq::getBit(21),
  SY_CODEUNIT = rq::getBit(22),
  SY_SIGNED = rq::getBit(23),
  SY_UNSIGNED = rq::getBit(24),
  SY_TOP_OF_FRAME = rq::getBit(25)

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
  case E::SY_GENERIC_Bfloat:
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
  case E::SY_Bfloat16:
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
  case E::SY_ARRAY:
    return EF::SYMBOL | EF::SY_COUNTED_SUBTYPE | EF::SY_TYPE | EF::SY_SUBTYPE |
           EF::SY_CONCRETE;
  case E::SY_LAYOUT:
    return EF::SYMBOL | EF::SY_PARAMETER_LIST_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_CLASS_LAYOUT:
    return EF::SYMBOL | EF::SY_PARAMETER_LIST_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_LAYOUT:
    return EF::SYMBOL | EF::SY_PARAMETER_LIST_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_SIGNATURE:
    return EF::SYMBOL | EF::SY_PARAMETER_LIST_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_EXTENSION:
    return EF::SYMBOL | EF::SY_PARAMETER_LIST_SUBTYPE | EF::SY_TYPE |
           EF::SY_SUBTYPE | EF::SY_CONCRETE;
  case E::SY_CLASS_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_LAYOUT_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_TEMPLATE_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
  case E::SY_SIGNATURE_PARAMETER:
    return EF::SYMBOL | EF::SY_CONCRETE;
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
  case E::SY_SYNONYM:
    return EF::SYMBOL | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_DYNAMIC_VARIABLE:
    return EF::SYMBOL | EF::SY_CONCRETE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_STATIC_VARIABLE:
    return EF::SYMBOL | EF::SY_CONCRETE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENUMERATOR:
    return EF::SYMBOL | EF::SY_TYPE | EF::SY_CONCRETE;
  case E::SY_CATEGORY_ALTERNATIVE:
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
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENUMERATION:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_CATEGORY:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_TYPE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENTRY:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME;
  case E::SY_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_EXTENSION_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_CONCRETE |
           EF::SY_TOP_OF_FRAME | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
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
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PARAMETER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterListSubtype(rq::EntityKind kind) {
  RQ_ASSERT_SYMBOL(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::SY_PARAMETER_LIST_SUBTYPE);
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
// NOTE: SCALED_SIGNED_INTEGER and SCALED_UNSIGNED_INTEGER is platform changing
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
struct GenericBfloat;
struct Half;
struct Single;
struct Double;
struct Quadruple;
struct Binary16;
struct Binary32;
struct Binary64;
struct Binary128;
struct Bfloat16;
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
struct Parameter;
struct ClassParameter;
struct LayoutParameter;
struct TemplateParameter;
struct SignatureParameter;
struct ParameterListSubtype;
struct Layout;
struct ClassLayout;
struct TemplateLayout;
struct Signature;
struct Extension;
struct ArithmeticSequence;
struct ArithmeticInterval;
struct FiniteArithmeticProgression;
struct InfiniteArithmeticProgression;
struct Module;
struct Import;
struct Code;
struct CategoryDiscriminant;
struct Label;
struct Synonym;
struct DynamicVariable;
struct StaticVariable;
struct Enumerator;
struct CategoryAlternative;
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

  const rq::Expression *_expression_ptr{nullptr};

  InitialExpression() = default;
  inline explicit InitialExpression(const rq::Expression &expression)
      : _expression_ptr(&expression) {}
  InitialExpression(const Self &) = delete;
  InitialExpression(Self &&) = delete;
  ~InitialExpression() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};
struct LateExpression : public rq::InitialExpression {
  using Self = rq::LateExpression;

  LateExpression() = default;
  inline explicit LateExpression(const rq::Expression &expression)
      : InitialExpression(expression) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() {
    return this->_expression_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_expression_ptr, expression_ptr);
  }
};
struct ReplacableExpression : public rq::LateExpression {
  using Self = rq::ReplacableExpression;

  ReplacableExpression() = default;
  inline explicit ReplacableExpression(const rq::Expression &expression)
      : LateExpression(expression) {}
  RQ_ALWAYS_INLINE const rq::Expression &
  replaceExpression(const rq::Expression &expression) {
    return rq::replaceValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE const rq::Expression &
  replaceExpression(const rq::Expression *expression) {
    return rq::replaceValue(this->_expression_ptr, expression);
  }
  RQ_ALWAYS_INLINE const rq::Expression *
  replaceExpressionPtr(const rq::Expression &expression) {
    return rq::replaceValuPtr(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE const rq::Expression *
  replaceExpressionPtr(const rq::Expression *expression) {
    return rq::replaceValuPtr(this->_expression_ptr, expression);
  }
};
struct InitialExpressionFlags {
  using Self = InitialExpressionFlags;

  rq::ExpressionFlags _expression_flags;

  InitialExpressionFlags(rq::ExpressionFlags flags)
      : _expression_flags(flags) {}
  InitialExpressionFlags(const Self &) = delete;
  InitialExpressionFlags(Self &&) = delete;
  virtual ~InitialExpressionFlags() {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionFlags
  getExpressionFlags() const {
    return this->_expression_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque() const {
    return rq::getHasOpaque(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOutside() const {
    return rq::getHasOutside(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatic() const {
    return rq::getHasStatic(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapture() const {
    return rq::getHasCapture(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasEager() const {
    return rq::getHasEager(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayParent() const {
    return rq::getHasMayParent(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasParent() const {
    return rq::getHasParent(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAbstract() const {
    return rq::getHasAbstract(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVirtual() const {
    return rq::getHasVirtual(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOverride() const {
    return rq::getHasOverride(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPosition() const {
    return rq::getHasPosition(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangle() const {
    return rq::getHasMangle(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPack() const {
    return rq::getHasPack(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLabel() const {
    return rq::getHasLabel(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate() const {
    return rq::getHasTemplate(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikely() const {
    return rq::getHasLikely(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnlikely() const {
    return rq::getHasUnlikely(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasDepreciated() const {
    return rq::getHasDepreciated(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExport() const {
    return rq::getHasExport(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPublic() const {
    return rq::getHasPublic(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasProtected() const {
    return rq::getHasProtected(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayCopy() const {
    return rq::getHasMayCopy(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayMove() const {
    return rq::getHasMayMove(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasOk() const {
    return rq::getHasOk(this->_expression_flags);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::ExpressionAttribute attribute) const {
    return rq::getHasAttribute(this->_expression_flags, attribute);
  }
};
struct InitialModuleMember {
  using Self = rq::InitialModuleMember;

  rq::Module *_containing_module_ptr;

  inline explicit InitialModuleMember(rq::Module &module)
      : _containing_module_ptr(&module) {}
  InitialModuleMember(const Self &) = delete;
  InitialModuleMember(Self &&) = delete;
  ~InitialModuleMember() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  const rq::Module &getContainingModule() const {
    return rq::dereferencePtr(this->_containing_module_ptr);
  }
  rq::Module &getContainingModule() {
    return rq::dereferencePtr(this->_containing_module_ptr);
  }
};
struct InitialSymbolTableMember {
  using Self = rq::InitialSymbolTableMember;

  rq::SymbolTable *_containing_symbol_table_ptr;

  inline explicit InitialSymbolTableMember(rq::SymbolTable &symbol_table)
      : _containing_symbol_table_ptr(&symbol_table) {}
  InitialSymbolTableMember(const Self &) = delete;
  InitialSymbolTableMember(Self &&) = delete;
  ~InitialSymbolTableMember() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  const rq::SymbolTable &getContainingSymbolTable() const {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
  rq::SymbolTable &getContainingSymbolTable() {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
};
struct InitialMaybeSymbolTableMember {
  using Self = rq::InitialMaybeSymbolTableMember;

  rq::SymbolTable *_containing_symbol_table_ptr{nullptr};

  inline explicit InitialMaybeSymbolTableMember() = default;
  inline explicit InitialMaybeSymbolTableMember(rq::SymbolTable &symbol_table)
      : _containing_symbol_table_ptr(&symbol_table) {}
  InitialMaybeSymbolTableMember(const Self &) = delete;
  InitialMaybeSymbolTableMember(Self &&) = delete;
  ~InitialMaybeSymbolTableMember() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasContainingSymbolTable() const {
    return this->_containing_symbol_table_ptr != nullptr;
  }
  const rq::SymbolTable &getContainingSymbolTable() const {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
  rq::SymbolTable &getContainingSymbolTable() {
    return rq::dereferencePtr(this->_containing_symbol_table_ptr);
  }
};
struct InitialNamed {
  using Self = rq::InitialNamed;

  llvm::StringRef _name;

  inline explicit InitialNamed(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "name is empty");
  }
  InitialNamed(const Self &) = delete;
  InitialNamed(Self &&) = delete;
  ~InitialNamed() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct InitialMaybeNamed {
  using Self = rq::InitialMaybeNamed;

  llvm::StringRef _name{};

  InitialMaybeNamed() = default;
  inline explicit InitialMaybeNamed(llvm::StringRef name) : _name(name) {
    RQ_ASSERT(!name.empty(), "name is empty");
  }
  InitialMaybeNamed(const Self &) = delete;
  InitialMaybeNamed(Self &&) = delete;
  ~InitialMaybeNamed() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasName() const {
    return !this->_name.empty();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return this->_name;
  }
};
struct Entity {
  using Self = rq::Entity;

  rq::EntityKind _kind;

  inline explicit Entity(rq::EntityKind k) : _kind(k) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter() const {
    return rq::getIsParameter(this->getKind());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterListSubtype() const {
    return rq::getIsParameterListSubtype(this->getKind());
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

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    std::ignore = entity;
    return true;
  }
};

template <> struct is_parent_only<rq::Entity> final : std::true_type {};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  inline explicit Symbol(rq::EntityKind k) : Entity(k) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSymbol(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::Symbol> final : std::true_type {};

struct SimpleBuiltin : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit SimpleBuiltin(rq::EntityKind k) : Symbol(k) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSimpleBuiltin(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_acquired<rq::SimpleBuiltin> final : std::true_type {};

struct Inference final : public rq::SimpleBuiltin {
  using Self = rq::Inference;

  inline explicit Inference() : SimpleBuiltin(rq::EntityKind::SY_INFERENCE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_INFERENCE;
  }
};

template <> struct is_acquired<rq::Inference> final : std::true_type {};

struct GenericSymbol final : public rq::SimpleBuiltin {
  using Self = rq::GenericSymbol;

  inline explicit GenericSymbol()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SYMBOL) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SYMBOL;
  }
};

template <> struct is_acquired<rq::GenericSymbol> final : std::true_type {};

struct GenericType final : public rq::SimpleBuiltin {
  using Self = rq::GenericType;

  inline explicit GenericType()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_TYPE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_TYPE;
  }
};

template <> struct is_acquired<rq::GenericType> final : std::true_type {};

struct Void final : public rq::SimpleBuiltin {
  using Self = rq::Void;

  inline explicit Void() : SimpleBuiltin(rq::EntityKind::SY_VOID) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_VOID;
  }
};

template <> struct is_acquired<rq::Void> final : std::true_type {};

struct Null final : public rq::SimpleBuiltin {
  using Self = rq::Null;

  inline explicit Null() : SimpleBuiltin(rq::EntityKind::SY_NULL) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_NULL;
  }
};

template <> struct is_acquired<rq::Null> final : std::true_type {};

struct NoReturn final : public rq::SimpleBuiltin {
  using Self = rq::NoReturn;

  inline explicit NoReturn() : SimpleBuiltin(rq::EntityKind::SY_NO_RETURN) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_NO_RETURN;
  }
};

template <> struct is_acquired<rq::NoReturn> final : std::true_type {};

struct VariadicArguments final : public rq::SimpleBuiltin {
  using Self = rq::VariadicArguments;

  inline explicit VariadicArguments()
      : SimpleBuiltin(rq::EntityKind::SY_VARIADIC_ARGUMENTS) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_VARIADIC_ARGUMENTS;
  }
};

template <> struct is_acquired<rq::VariadicArguments> final : std::true_type {};

struct Boolean final : public rq::SimpleBuiltin {
  using Self = rq::Boolean;

  inline explicit Boolean() : SimpleBuiltin(rq::EntityKind::SY_BOOLEAN) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BOOLEAN;
  }
};

template <> struct is_acquired<rq::Boolean> final : std::true_type {};

struct GenericSigned final : public rq::SimpleBuiltin {
  using Self = rq::GenericSigned;

  inline explicit GenericSigned()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SIGNED) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SIGNED;
  }
};

template <> struct is_acquired<rq::GenericSigned> final : std::true_type {};

struct GenericUnsigned final : public rq::SimpleBuiltin {
  using Self = rq::GenericUnsigned;

  inline explicit GenericUnsigned()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_UNSIGNED) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_UNSIGNED;
  }
};

template <> struct is_acquired<rq::GenericUnsigned> final : std::true_type {};

struct GenericFloat final : public rq::SimpleBuiltin {
  using Self = rq::GenericFloat;

  inline explicit GenericFloat()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_FLOAT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_FLOAT;
  }
};

template <> struct is_acquired<rq::GenericFloat> final : std::true_type {};

struct GenericBinary final : public rq::SimpleBuiltin {
  using Self = rq::GenericBinary;

  inline explicit GenericBinary()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_BINARY) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_BINARY;
  }
};

template <> struct is_acquired<rq::GenericBinary> final : std::true_type {};

struct GenericBfloat final : public rq::SimpleBuiltin {
  using Self = rq::GenericBfloat;

  inline explicit GenericBfloat()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_Bfloat) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_Bfloat;
  }
};

template <> struct is_acquired<rq::GenericBfloat> final : std::true_type {};

struct Half final : public rq::SimpleBuiltin {
  using Self = rq::Half;

  inline explicit Half() : SimpleBuiltin(rq::EntityKind::SY_HALF) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_HALF;
  }
};

template <> struct is_acquired<rq::Half> final : std::true_type {};

struct Single final : public rq::SimpleBuiltin {
  using Self = rq::Single;

  inline explicit Single() : SimpleBuiltin(rq::EntityKind::SY_SINGLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SINGLE;
  }
};

template <> struct is_acquired<rq::Single> final : std::true_type {};

struct Double final : public rq::SimpleBuiltin {
  using Self = rq::Double;

  inline explicit Double() : SimpleBuiltin(rq::EntityKind::SY_DOUBLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_DOUBLE;
  }
};

template <> struct is_acquired<rq::Double> final : std::true_type {};

struct Quadruple final : public rq::SimpleBuiltin {
  using Self = rq::Quadruple;

  inline explicit Quadruple() : SimpleBuiltin(rq::EntityKind::SY_QUADRUPLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_QUADRUPLE;
  }
};

template <> struct is_acquired<rq::Quadruple> final : std::true_type {};

struct Binary16 final : public rq::SimpleBuiltin {
  using Self = rq::Binary16;

  inline explicit Binary16() : SimpleBuiltin(rq::EntityKind::SY_BINARY16) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY16;
  }
};

template <> struct is_acquired<rq::Binary16> final : std::true_type {};

struct Binary32 final : public rq::SimpleBuiltin {
  using Self = rq::Binary32;

  inline explicit Binary32() : SimpleBuiltin(rq::EntityKind::SY_BINARY32) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY32;
  }
};

template <> struct is_acquired<rq::Binary32> final : std::true_type {};

struct Binary64 final : public rq::SimpleBuiltin {
  using Self = rq::Binary64;

  inline explicit Binary64() : SimpleBuiltin(rq::EntityKind::SY_BINARY64) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY64;
  }
};

template <> struct is_acquired<rq::Binary64> final : std::true_type {};

struct Binary128 final : public rq::SimpleBuiltin {
  using Self = rq::Binary128;

  inline explicit Binary128() : SimpleBuiltin(rq::EntityKind::SY_BINARY128) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_BINARY128;
  }
};

template <> struct is_acquired<rq::Binary128> final : std::true_type {};

struct Bfloat16 final : public rq::SimpleBuiltin {
  using Self = rq::Bfloat16;

  inline explicit Bfloat16() : SimpleBuiltin(rq::EntityKind::SY_Bfloat16) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_Bfloat16;
  }
};

template <> struct is_acquired<rq::Bfloat16> final : std::true_type {};

struct GenericInteger final : public rq::SimpleBuiltin {
  using Self = rq::GenericInteger;

  inline explicit GenericInteger()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_INTEGER) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_INTEGER;
  }
};

template <> struct is_acquired<rq::GenericInteger> final : std::true_type {};

struct GenericSignedInteger final : public rq::SimpleBuiltin {
  using Self = rq::GenericSignedInteger;

  inline explicit GenericSignedInteger()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_SIGNED_INTEGER) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_SIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::GenericSignedInteger> final : std::true_type {};

struct GenericUnsignedInteger final : public rq::SimpleBuiltin {
  using Self = rq::GenericUnsignedInteger;

  inline explicit GenericUnsignedInteger()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_UNSIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::GenericUnsignedInteger> final : std::true_type {};

struct GenericCodeunit final : public rq::SimpleBuiltin {
  using Self = rq::GenericCodeunit;

  inline explicit GenericCodeunit()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_CODEUNIT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_CODEUNIT;
  }
};

template <> struct is_acquired<rq::GenericCodeunit> final : std::true_type {};

struct GenericString final : public rq::SimpleBuiltin {
  using Self = rq::GenericString;

  inline explicit GenericString()
      : SimpleBuiltin(rq::EntityKind::SY_GENERIC_STRING) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_GENERIC_STRING;
  }
};

template <> struct is_acquired<rq::GenericString> final : std::true_type {};

struct Ascii final : public rq::SimpleBuiltin {
  using Self = rq::Ascii;

  inline explicit Ascii() : SimpleBuiltin(rq::EntityKind::SY_ASCII) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ASCII;
  }
};

template <> struct is_acquired<rq::Ascii> final : std::true_type {};

struct Utf8 final : public rq::SimpleBuiltin {
  using Self = rq::Utf8;

  inline explicit Utf8() : SimpleBuiltin(rq::EntityKind::SY_UTF8) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_UTF8;
  }
};

template <> struct is_acquired<rq::Utf8> final : std::true_type {};

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

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPlatformScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::PLATFORM_SCALAR);
}

void RQ_ALWAYS_INLINE profileScaledBuiltin(llvm::FoldingSetNodeID &id,
                                           rq::EntityKind kind,
                                           std::uint16_t scalar,
                                           std::uint16_t uid,
                                           rq::ScaledBuiltinFlags flags) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddInteger(static_cast<unsigned>(scalar));
  id.AddInteger(static_cast<unsigned>(uid));
  id.AddInteger(static_cast<unsigned>(flags));
}

struct ScaledBuiltin : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledBuiltin;

  std::uint16_t _scalar;
  std::uint16_t _uid;
  rq::ScaledBuiltinFlags _flags;

  inline explicit ScaledBuiltin(rq::EntityKind k, std::uint16_t scalar,
                                std::uint16_t uid, rq::ScaledBuiltinFlags flags)
      : Symbol(k), _scalar(scalar), _uid(uid), _flags(flags) {}
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
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsScaledBuiltin(rq::dereferencePtr(entity).getKind());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileScaledBuiltin(id, this->_kind, this->_scalar, this->_uid,
                             this->_flags);
  }
};

template <> struct is_acquired<rq::ScaledBuiltin> final : std::true_type {};

struct ScaledSignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledSignedInteger;

  inline explicit ScaledSignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                      rq::ScaledBuiltinFlags flags)
      : ScaledBuiltin(rq::EntityKind::SY_SCALED_SIGNED_INTEGER, scalar, uid,
                      flags) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SCALED_SIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::ScaledSignedInteger> final : std::true_type {};

struct ScaledUnsignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledUnsignedInteger;

  inline explicit ScaledUnsignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                        rq::ScaledBuiltinFlags flags)
      : ScaledBuiltin(rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER, scalar, uid,
                      flags) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SCALED_UNSIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::ScaledUnsignedInteger> final : std::true_type {};

void RQ_ALWAYS_INLINE profileUnarySubtype(llvm::FoldingSetNodeID &id,
                                          rq::EntityKind kind,
                                          const rq::TypeConstant &descendent) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&descendent);
}

struct UnarySubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtype;

  rq::TypeConstant *_descendent_ptr;

  inline explicit UnarySubtype(rq::EntityKind k, rq::TypeConstant &descendent)
      : Symbol(k), _descendent_ptr(&descendent) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent() {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsUnarySubtype(rq::dereferencePtr(entity).getKind());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileUnarySubtype(id, this->getKind(), this->getDescendent());
  }
};

template <> struct is_acquired<rq::UnarySubtype> final : std::true_type {};

struct Reference final : public rq::UnarySubtype {
  using Self = rq::Reference;

  inline explicit Reference(rq::TypeConstant &descendent)
      : UnarySubtype(rq::EntityKind::SY_REFERENCE, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_REFERENCE;
  }
};

template <> struct is_acquired<rq::Reference> final : std::true_type {};

struct Pointer final : public rq::UnarySubtype {
  using Self = rq::Pointer;

  inline explicit Pointer(rq::TypeConstant &descendent)
      : UnarySubtype(rq::EntityKind::SY_POINTER, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_POINTER;
  }
};

template <> struct is_acquired<rq::Pointer> final : std::true_type {};

struct FatPointer final : public rq::UnarySubtype {
  using Self = rq::FatPointer;

  inline explicit FatPointer(rq::TypeConstant &descendent)
      : UnarySubtype(rq::EntityKind::SY_FAT_POINTER, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_FAT_POINTER;
  }
};

template <> struct is_acquired<rq::FatPointer> final : std::true_type {};

struct InferencedCountArray final : public rq::UnarySubtype {
  using Self = rq::InferencedCountArray;

  inline explicit InferencedCountArray(rq::TypeConstant &descendent)
      : UnarySubtype(rq::EntityKind::SY_INFERENCED_COUNT_ARRAY, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
  }
};

template <>
struct is_acquired<rq::InferencedCountArray> final : std::true_type {};

void RQ_ALWAYS_INLINE profileCountedSubtype(llvm::FoldingSetNodeID &id,
                                            rq::EntityKind kind,
                                            const rq::TypeConstant &descendent,
                                            unsigned count) {
  id.AddInteger(static_cast<unsigned>(kind));
  id.AddPointer(&descendent);
  id.AddInteger(count);
}

struct CountedSubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::CountedSubtype;

  rq::TypeConstant *_descendent_ptr;
  unsigned _count;

  inline explicit CountedSubtype(rq::EntityKind k, rq::TypeConstant &descendent,
                                 unsigned count)
      : Symbol(k), _descendent_ptr(&descendent), _count(count) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent() {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getCount() const {
    return this->_count;
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsCountedSubtype(rq::dereferencePtr(entity).getKind());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileCountedSubtype(id, this->getKind(), this->getDescendent(),
                              this->getCount());
  }
};

template <> struct is_acquired<rq::CountedSubtype> final : std::true_type {};

struct Array final : public rq::CountedSubtype {
  using Self = rq::Array;

  inline explicit Array(rq::TypeConstant &descendent, unsigned count)
      : CountedSubtype(rq::EntityKind::SY_ARRAY, descendent, count) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ARRAY;
  }
};

template <> struct is_acquired<rq::Array> final : std::true_type {};

struct ParameterListSubtype : public rq::Symbol,
                              public rq::InitialExpression,
                              public rq::InitialExpressionFlags,
                              public rq::InitialModuleMember,
                              public rq::InitialSymbolTableMember {
  using Self = rq::ParameterListSubtype;

  llvm::ArrayRef<rq::Parameter> _named_parameter_map{};
  rq::Parameter *_positional_parameter_list_begin_ptr{nullptr};
  unsigned _parameter_count{};

  inline explicit ParameterListSubtype(rq::BumpPtrAllocator &allocator,
                                       unsigned map_bucket_count,
                                       rq::EntityKind k,
                                       rq::Expression &expression,
                                       rq::ExpressionFlags attributes,
                                       rq::Module &module,
                                       rq::SymbolTable &symbol_table)
      : Symbol(k), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table),
        _named_parameter_map(
            allocator.allocateAcquiredZeroedArray<rq::Parameter>(
                map_bucket_count)) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsParameterListSubtype(rq::dereferencePtr(entity).getKind());
  }
};

template <>
struct is_parent_only<rq::ParameterListSubtype> final : std::true_type {};

struct Layout final : public rq::ParameterListSubtype {
  using Self = rq::Layout;

  inline explicit Layout(rq::BumpPtrAllocator &allocator,
                         unsigned parameter_bucket_count,
                         rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::SymbolTable &symbol_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::EntityKind::SY_LAYOUT, expression, attributes,
                             module, symbol_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_LAYOUT;
  }
};

template <> struct is_acquired<rq::Layout> final : std::true_type {};

struct ClassLayout final : public rq::ParameterListSubtype {
  using Self = rq::ClassLayout;

  inline explicit ClassLayout(rq::BumpPtrAllocator &allocator,
                              unsigned parameter_bucket_count,
                              rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module, rq::SymbolTable &symbol_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::EntityKind::SY_CLASS_LAYOUT, expression,
                             attributes, module, symbol_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CLASS_LAYOUT;
  }
};

template <> struct is_acquired<rq::ClassLayout> final : std::true_type {};

struct TemplateLayout final : public rq::ParameterListSubtype {
  using Self = rq::TemplateLayout;

  inline explicit TemplateLayout(rq::BumpPtrAllocator &allocator,
                                 unsigned parameter_bucket_count,
                                 rq::Expression &expression, rq::Module &module,
                                 rq::SymbolTable &symbol_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::EntityKind::SY_TEMPLATE_LAYOUT, expression, {},
                             module, symbol_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_LAYOUT;
  }
};

template <> struct is_acquired<rq::TemplateLayout> final : std::true_type {};

struct Signature final : public rq::ParameterListSubtype {
  using Self = rq::Signature;

  rq::TypeConstant *_return_type;

  inline explicit Signature(rq::BumpPtrAllocator &allocator,
                            unsigned parameter_bucket_count,
                            rq::TypeConstant &return_type,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &symbol_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::EntityKind::SY_SIGNATURE, expression,
                             attributes, module, symbol_table),
        _return_type(&return_type) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getReturnType() const {
    return rq::dereferencePtr(this->_return_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getReturnType() {
    return rq::dereferencePtr(this->_return_type);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SIGNATURE;
  }
};

template <> struct is_acquired<rq::Signature> final : std::true_type {};

struct Extension final : public rq::ParameterListSubtype {
  using Self = rq::Extension;

  rq::TypeConstant *_return_type;
  rq::TypeConstant *_extended_type;

  inline explicit Extension(rq::BumpPtrAllocator &allocator,
                            unsigned parameter_bucket_count,
                            rq::TypeConstant &return_type,
                            rq::TypeConstant &extended_type,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &symbol_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::EntityKind::SY_EXTENSION, expression,
                             attributes, module, symbol_table),
        _return_type(&return_type), _extended_type(&extended_type) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getReturnType() const {
    return rq::dereferencePtr(this->_return_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getReturnType() {
    return rq::dereferencePtr(this->_return_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
  getExtendedType() const {
    return rq::dereferencePtr(this->_extended_type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getExtendedType() {
    return rq::dereferencePtr(this->_extended_type);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_EXTENSION;
  }
};

template <> struct is_acquired<rq::Extension> final : std::true_type {};

struct Parameter : public rq::Symbol,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::InitialSymbolTableMember,
                   public rq::InitialMaybeNamed {
  using Self = rq::Parameter;

  std::size_t _hash{0};
  rq::TypeConstant *_type_ptr{nullptr};
  rq::ParameterListSubtype *_parameter_list_subtype_ptr;
  Self *_left_ptr{nullptr};
  Self *_right_ptr{nullptr};
  Self *_next_ptr{nullptr};

  inline explicit Parameter(rq::EntityKind k, llvm::StringRef name,
                            rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &symbol_table)
      : Symbol(k), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialMaybeNamed(name),
        _parameter_list_subtype_ptr(&list) {}
  inline explicit Parameter(rq::EntityKind k, rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &symbol_table)
      : Symbol(k), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialMaybeNamed(),
        _parameter_list_subtype_ptr(&list) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const {
    return this->_type_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setType(rq::TypeConstant &type) {
    rq::assignSingleValue(this->_type_ptr, &type);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getType() {
    return rq::dereferencePtr(this->_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ParameterListSubtype &
  getParameterListSubtype() const {
    return rq::dereferencePtr(this->_parameter_list_subtype_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ParameterListSubtype &
  getParameterListSubtype() {
    return rq::dereferencePtr(this->_parameter_list_subtype_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsParameter(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_acquired<rq::Parameter> final : std::true_type {};

// NOTE: rq::Parameter is not is_parent_only so it can be allocated for
// rq::ParameterListSubtype map. as a consequence of this, no child types of
// rq::Parameter can have properties beyond what is inherited from Parameter.
// also, all child types must only inherit rq::Parameter and nothing else.

struct ClassParameter : public rq::Parameter {
  using Self = rq::ClassParameter;

  inline explicit ClassParameter(llvm::StringRef name, rq::ClassLayout &layout,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_CLASS_PARAMETER, name, layout, expression,
                  attributes, module, symbol_table) {}
  inline explicit ClassParameter(rq::ClassLayout &layout,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_CLASS_PARAMETER, layout, expression,
                  attributes, module, symbol_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassLayout &getClassLayout() const {
    return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassLayout &getClassLayout() {
    return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CLASS_PARAMETER;
  }
};

template <> struct is_acquired<rq::ClassParameter> final : std::true_type {};

struct LayoutParameter : public rq::Parameter {
  using Self = rq::LayoutParameter;

  inline explicit LayoutParameter(llvm::StringRef name, rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_LAYOUT_PARAMETER, name, layout, expression,
                  attributes, module, symbol_table) {}
  inline explicit LayoutParameter(rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_LAYOUT_PARAMETER, layout, expression,
                  attributes, module, symbol_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getLayout() const {
    return llvm::cast<rq::Layout>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getLayout() {
    return llvm::cast<rq::Layout>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_LAYOUT_PARAMETER;
  }
};

template <> struct is_acquired<rq::LayoutParameter> final : std::true_type {};

struct TemplateParameter : public rq::Parameter {
  using Self = rq::TemplateParameter;

  inline explicit TemplateParameter(llvm::StringRef name,
                                    rq::TemplateLayout &template_layout,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_TEMPLATE_PARAMETER, name, template_layout,
                  expression, attributes, module, symbol_table) {}
  inline explicit TemplateParameter(rq::TemplateLayout &template_layout,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_TEMPLATE_PARAMETER, template_layout,
                  expression, attributes, module, symbol_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &getTemplate() const {
    return llvm::cast<rq::Template>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template &getTemplate() {
    return llvm::cast<rq::Template>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_PARAMETER;
  }
};

template <> struct is_acquired<rq::TemplateParameter> final : std::true_type {};

struct SignatureParameter : public rq::Parameter {
  using Self = rq::SignatureParameter;

  inline explicit SignatureParameter(llvm::StringRef name,
                                     rq::Signature &signature,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_SIGNATURE_PARAMETER, name, signature,
                  expression, attributes, module, symbol_table) {}
  inline explicit SignatureParameter(rq::Signature &signature,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::SymbolTable &symbol_table)
      : Parameter(rq::EntityKind::SY_SIGNATURE_PARAMETER, signature, expression,
                  attributes, module, symbol_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &getSignature() const {
    return llvm::cast<rq::Signature>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature &getSignature() {
    return llvm::cast<rq::Signature>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_SIGNATURE_PARAMETER;
  }
};

template <>
struct is_acquired<rq::SignatureParameter> final : std::true_type {};

void RQ_ALWAYS_INLINE profileArithmeticSequence(
    llvm::FoldingSetNodeID &id, const rq::TypeConstant &descendent,
    rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  // no need to fold kind
  id.AddPointer(&descendent);
  id.AddInteger(static_cast<unsigned>(condition));
  id.AddInteger(static_cast<unsigned>(step));
}

struct ArithmeticSequence : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequence;

  rq::TypeConstant *_descendent_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  inline explicit ArithmeticSequence(rq::EntityKind k,
                                     rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition,
                                     rq::ArithmeticSequenceStep step)
      : Symbol(k), _descendent_ptr(&descendent), _condition(condition),
        _step(step) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent() {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceCondition
  getCondition() const {
    return this->_condition;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ArithmeticSequenceStep getStep() const {
    return this->_step;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsArithmeticSequence(rq::dereferencePtr(entity).getKind());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileArithmeticSequence(id, this->getDescendent(),
                                  this->getCondition(), this->getStep());
  }
};

template <>
struct is_acquired<rq::ArithmeticSequence> final : std::true_type {};

struct ArithmeticInterval : public rq::ArithmeticSequence {
  using Self = rq::ArithmeticInterval;
  inline explicit ArithmeticInterval(rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition)
      : ArithmeticSequence(rq::EntityKind::SY_ARITHMETIC_INTERVAL, descendent,
                           condition, rq::ArithmeticSequenceStep::NONE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ARITHMETIC_INTERVAL;
  }
};

template <>
struct is_acquired<rq::ArithmeticInterval> final : std::true_type {};

struct FiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticProgression;

  inline explicit FiniteArithmeticProgression(
      rq::TypeConstant &descendent, rq::ArithmeticSequenceCondition condition,
      rq::ArithmeticSequenceStep step)
      : ArithmeticSequence(rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION,
                           descendent, condition, step) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
};

template <>
struct is_acquired<rq::FiniteArithmeticProgression> final : std::true_type {};

struct InfiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticProgression;

  inline explicit InfiniteArithmeticProgression(rq::TypeConstant &descendent,
                                                rq::ArithmeticSequenceStep step)
      : ArithmeticSequence(rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION,
                           descendent, rq::ArithmeticSequenceCondition::NONE,
                           step) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
};

template <>
struct is_acquired<rq::InfiniteArithmeticProgression> final : std::true_type {};

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

struct Module final : public rq::Symbol {
  using Self = rq::Module;

  rq::Expression *_expression_ptr{nullptr};
  rq::ModuleKind _module_kind;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;

  inline explicit Module(rq::ModuleKind kind, llvm::StringRef path,
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
  RQ_ALWAYS_INLINE void setExpression(rq::Expression *expression) {
    rq::assignSingleValue(this->_expression_ptr, expression);
  }
  RQ_ALWAYS_INLINE rq::Expression &
  replaceExpression(rq::Expression &expression) {
    return rq::replaceValue(this->_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE rq::Expression *
  replaceExpressionPtr(rq::Expression &expression) {
    return rq::replaceValuPtr(this->_expression_ptr, &expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_MODULE;
  }
};

template <> struct is_acquired<rq::Module> final : std::true_type {};

struct Import final : public rq::Symbol,
                      public rq::InitialExpression,
                      public rq::InitialExpressionFlags,
                      public rq::InitialModuleMember {
  using Self = rq::Import;

  inline explicit Import(const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module)
      : Symbol(rq::EntityKind::SY_IMPORT), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_IMPORT;
  }
};

struct Code : public rq::Symbol,
              public rq::InitialExpression,
              public rq::InitialExpressionFlags,
              public rq::InitialModuleMember,
              public rq::InitialSymbolTableMember,
              public rq::InitialNamed {
  using Self = rq::Code;

  inline explicit Code(llvm::StringRef name, const rq::Expression &expression,
                       rq::ExpressionFlags attributes, rq::Module &module,
                       rq::SymbolTable &symbol_table)
      : Symbol(rq::EntityKind::SY_CODE), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CODE;
  }
};

struct CategoryDiscriminant : public rq::Symbol {
  using Self = rq::CategoryDiscriminant;

  rq::Category *_category_ptr;

  inline explicit CategoryDiscriminant(rq::Category &category)
      : Symbol(rq::EntityKind::SY_CATEGORY_DISCRIMINANT),
        _category_ptr(&category) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CATEGORY_DISCRIMINANT;
  }
};

template <>
struct is_acquired<rq::CategoryDiscriminant> final : std::true_type {};

struct Label : public rq::Symbol,
               public rq::InitialExpression,
               public rq::InitialModuleMember,
               public rq::InitialSymbolTableMember,
               public rq::InitialNamed {
  using Self = rq::Label;

  const rq::Expression *_ascription_ptr;
  rq::Entity *_subject_ptr;

  inline explicit Label(llvm::StringRef name, rq::Expression &expression,
                        const rq::Expression &ascription, rq::Entity &subject,
                        rq::Module &module, rq::SymbolTable &symbol_table)
      : Symbol(rq::EntityKind::SY_LABEL), InitialExpression(expression),
        InitialModuleMember(module), InitialSymbolTableMember(symbol_table),
        InitialNamed(name), _ascription_ptr(&ascription),
        _subject_ptr(&subject) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getAscription() const {
    return rq::dereferencePtr(this->_ascription_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getSubject() const {
    return rq::dereferencePtr(this->_subject_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getSubject() {
    return rq::dereferencePtr(this->_subject_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_LABEL;
  }
};

struct Synonym : public rq::Symbol {
  using Self = rq::Synonym;

  rq::Symbol *_original_ptr;

  inline explicit Synonym(rq::Symbol &original)
      : Symbol(rq::EntityKind::SY_SYNONYM), _original_ptr(&original) {
    RQ_ASSERT(
        !llvm::isa<rq::ScaledBuiltin>(original),
        "must use internal uid to differentiate synonyms of scaled builtins");
    RQ_ASSERT(!llvm::isa<rq::Synonym>(original),
              "must not make synonym of synonym");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getOriginal() const {
    return rq::dereferencePtr(this->_original_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getOriginal() {
    return rq::dereferencePtr(this->_original_ptr);
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SYNONYM;
  }
};

template <> struct is_acquired<rq::Synonym> final : std::true_type {};

struct DynamicVariable : public rq::Symbol,
                         public rq::InitialExpression,
                         public rq::InitialExpressionFlags,
                         public rq::InitialModuleMember,
                         public rq::InitialSymbolTableMember,
                         public rq::InitialNamed {
  using Self = rq::DynamicVariable;

  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit DynamicVariable(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &symbol_table)
      : Symbol(rq::EntityKind::SY_DYNAMIC_VARIABLE),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), InitialSymbolTableMember(symbol_table),
        InitialNamed(name) {}
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression& expression) {
    rq::assignSingleValue(this->_type_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void setValueExpression(const rq::Expression* expression_ptr) {
    rq::assignSingleValue(this->_value_expression_ptr, expression_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_DYNAMIC_VARIABLE;
  }
};

struct StaticVariable : public rq::Symbol,
                        public rq::InitialExpression,
                        public rq::InitialExpressionFlags,
                        public rq::InitialModuleMember,
                        public rq::InitialSymbolTableMember,
                        public rq::InitialNamed {
  using Self = rq::StaticVariable;

  inline explicit StaticVariable(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &symbol_table)
      : Symbol(rq::EntityKind::SY_STATIC_VARIABLE),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), InitialSymbolTableMember(symbol_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_STATIC_VARIABLE;
  }
};

template <> struct is_acquired<rq::StaticVariable> final : std::true_type {};

struct Enumerator : public rq::Symbol,
                    public rq::InitialExpression,
                    public rq::InitialExpressionFlags,
                    public rq::InitialModuleMember,
                    public rq::InitialSymbolTableMember,
                    public rq::InitialNamed {
  using Self = rq::Enumerator;

  inline explicit Enumerator(llvm::StringRef name, rq::Expression &expression,
                             rq::ExpressionFlags attributes, rq::Module &module,
                             rq::SymbolTable &symbol_table)
      : Symbol(rq::EntityKind::SY_ENUMERATOR), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ENUMERATOR;
  }
};

template <> struct is_acquired<rq::Enumerator> final : std::true_type {};

struct CategoryAlternative : public rq::Symbol,
                             public rq::InitialExpression,
                             public rq::InitialExpressionFlags,
                             public rq::InitialModuleMember {
  using Self = rq::CategoryAlternative;

  rq::Category *_category_ptr;
  rq::Code *_code_ptr;

  inline explicit CategoryAlternative(rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module)
      : Symbol(rq::EntityKind::SY_CATEGORY_ALTERNATIVE),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_CATEGORY_ALTERNATIVE;
  }
};

template <>
struct is_acquired<rq::CategoryAlternative> final : std::true_type {};

struct SymbolTableNode final {
  using Self = rq::SymbolTableNode;

  llvm::StringRef name{};
  rq::BumpPtrList<rq::Symbol> list{};
  std::size_t hash{0};
  Self *left_ptr{nullptr};
  Self *right_ptr{nullptr};
  Self *next_ptr{nullptr};

  inline explicit SymbolTableNode() = default;
};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTableNode;
  using reference = rq::SymbolTableNode &;
  using pointer = rq::SymbolTableNode *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTableNode *_node_ptr{nullptr};

  SymbolTableIterator() = default;
  explicit SymbolTableIterator(rq::SymbolTableNode *node_ptr)
      : _node_ptr(node_ptr) {}
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_node_ptr == it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_node_ptr != it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableNode &operator*() {
    return rq::dereferencePtr(this->_node_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableNode &operator*() const {
    return rq::dereferencePtr(this->_node_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTableNode *operator->() {
    return this->_node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableNode *operator->() const {
    return this->_node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_node_ptr == nullptr;
  }
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = rq::SymbolTableNode;
  using reference = const rq::SymbolTableNode &;
  using pointer = const rq::SymbolTableNode *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTableNode *_node_ptr = nullptr;

  ConstSymbolTableIterator() = default;
  explicit ConstSymbolTableIterator(const rq::SymbolTableNode *node_ptr)
      : _node_ptr(node_ptr) {}
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_node_ptr == it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_node_ptr != it._node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableNode &operator*() const {
    return rq::dereferencePtr(this->_node_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTableNode *operator->() const {
    return this->_node_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_node_ptr == nullptr;
  }
};

struct SymbolTable : public rq::Symbol,
                     public rq::InitialMaybeSymbolTableMember {
  using Self = rq::SymbolTable;
  using Node = rq::SymbolTableNode;

  Node *_named_map_begin_ptr{nullptr};
  std::span<Node> _named_symbols_map{};
  rq::BumpPtrList<rq::Symbol> _unamed_symbols_list{};

  inline explicit SymbolTable(rq::EntityKind k, rq::BumpPtrAllocator &allocator,
                              unsigned bucket_count)
      : Symbol(k),
        _named_symbols_map(allocator.allocateZeroedArray<Node>(bucket_count)) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::BumpPtrList<rq::Symbol> &
  getUnamedSymbolsList() const {
    return this->_unamed_symbols_list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrList<rq::Symbol> &
  getUnamedSymbolsList() {
    return this->_unamed_symbols_list;
  }
  inline void addNamedSymbol(rq::BumpPtrAllocator &allocator,
                             llvm::StringRef name, rq::Symbol &symbol) {
    const std::size_t hash = llvm::hash_value(name);
    const std::size_t bucket_i = hash % this->_named_symbols_map.size();
    Node *node_ptr = &this->_named_symbols_map[bucket_i];
    while (true) {
      Node &node = rq::dereferencePtr(node_ptr);
      if (node.hash == hash) {
        if (node.name == name) {
          node.list.insertFront(allocator, symbol);
          return;
        }
      }
      if (node.hash == 0) {
        node.hash = hash;
        node.name = name;
        node.list.insertFront(allocator, symbol);
        return;
      }
      Node *&next_ptr = node.hash < hash ? node.left_ptr : node.right_ptr;
      if (next_ptr == nullptr) {
        Node &new_node = allocator.allocateValue<Node>();
        node.next_ptr = this->_named_map_begin_ptr;
        this->_named_map_begin_ptr = &node;
        new_node.hash = hash;
        new_node.name = name;
        new_node.list.insertFront(allocator, symbol);
        next_ptr = &new_node;
        return;
      }
      node_ptr = next_ptr;
    }
  }
  [[nodiscard]] rq::ConstBumpPtrList<rq::Symbol>
  getNamedListPtr(llvm::StringRef name) const {
    const std::size_t hash = llvm::hash_value(name);
    const std::size_t bucket_i = hash % this->_named_symbols_map.size();
    Node *node_ptr = &this->_named_map_begin_ptr[bucket_i];
    while (node_ptr != nullptr) {
      Node &node = rq::dereferencePtr(node_ptr);
      if (node.hash == hash) {
        if (node.name == name) {
          return rq::ConstBumpPtrList<rq::Symbol>(node.list);
        }
      }
      node_ptr = node.hash < hash ? node.left_ptr : node.right_ptr;
    }
    return rq::ConstBumpPtrList<Symbol>();
  }
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstSymbolTableIterator, rq::ConstSymbolTableIterator,
      std::ranges::subrange_kind::unsized>
  getNamedSymbolsRange() const {
    return std::ranges::subrange<rq::ConstSymbolTableIterator,
                                 rq::ConstSymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::ConstSymbolTableIterator(this->_named_map_begin_ptr),
        rq::ConstSymbolTableIterator());
  }
  std::ranges::subrange<rq::SymbolTableIterator, rq::SymbolTableIterator,
                        std::ranges::subrange_kind::unsized>
  getNamedSymbolsRange() {
    return std::ranges::subrange<rq::SymbolTableIterator,
                                 rq::SymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::SymbolTableIterator(this->_named_map_begin_ptr),
        rq::SymbolTableIterator());
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSymbolTable(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::SymbolTable> final : std::true_type {};

RQ_ALWAYS_INLINE rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  this->_node_ptr = rq::dereferencePtr(this->_node_ptr).next_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::SymbolTableIterator SymbolTableIterator::operator++(int) {
  rq::SymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator &
ConstSymbolTableIterator::operator++() {
  this->_node_ptr = rq::dereferencePtr(this->_node_ptr).next_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator
ConstSymbolTableIterator::operator++(int) {
  rq::ConstSymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

struct Top : public rq::SymbolTable {
  using Self = rq::Top;

  inline explicit Top(rq::BumpPtrAllocator &allocator, unsigned bucket_count)
      : SymbolTable(rq::EntityKind::SY_TOP, allocator, bucket_count) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_TOP;
  }
};

struct Scope : public rq::SymbolTable,
               rq::InitialExpression,
               rq::InitialModuleMember {
  using Self = rq::Scope;

  inline explicit Scope(rq::BumpPtrAllocator &allocator, unsigned bucket_count,
                        rq::Expression &expression, rq::Module &module)
      : SymbolTable(rq::EntityKind::SY_SCOPE, allocator, bucket_count),
        InitialExpression(expression), InitialModuleMember(module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_SCOPE;
  }
};

struct Table : public rq::SymbolTable, rq::InitialNamed {
  using Self = rq::Table;

  inline explicit Table(llvm::StringRef name, rq::BumpPtrAllocator &allocator,
                        unsigned bucket_count)
      : SymbolTable(rq::EntityKind::SY_TABLE, allocator, bucket_count),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_TABLE;
  }
};

struct Class : public rq::SymbolTable,
               public rq::InitialExpression,
               public rq::InitialExpressionFlags,
               public rq::InitialModuleMember,
               public rq::InitialNamed {
  using Self = rq::Class;

  const rq::Expression *_class_layout_expression_ptr{nullptr};
  rq::ClassLayout *_class_layout_ptr{nullptr};

  inline explicit Class(llvm::StringRef name, rq::BumpPtrAllocator &allocator,
                        unsigned bucket_count, const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module)
      : SymbolTable(rq::EntityKind::SY_CLASS, allocator, bucket_count),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), InitialNamed(name) {}
  void setClassLayoutExpression(const rq::Expression &layout_expression) {
    rq::assignSingleValue(this->_class_layout_expression_ptr,
                          &layout_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CLASS;
  }
};

struct Enumeration : public rq::SymbolTable,
                     public rq::InitialExpression,
                     public rq::InitialExpressionFlags,
                     public rq::InitialModuleMember,
                     public rq::InitialNamed {
  using Self = rq::Enumeration;

  const rq::Expression *_underlying_type_expression_ptr{nullptr};

  inline explicit Enumeration(llvm::StringRef name,
                              rq::BumpPtrAllocator &allocator,
                              unsigned bucket_count,
                              const rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module)
      : SymbolTable(rq::EntityKind::SY_ENUMERATION, allocator, bucket_count),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), InitialNamed(name) {}
  RQ_ALWAYS_INLINE void setUnderlyingTypeExpression(
      const rq::Expression &underlying_type_expression) {
    rq::assignSingleValue(this->_underlying_type_expression_ptr,
                          &underlying_type_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_ENUMERATION;
  }
};

struct Category : public rq::SymbolTable,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::InitialNamed {
  using Self = rq::Category;

  const rq::Expression *_discriminant_type_expression_ptr{nullptr};

  inline explicit Category(llvm::StringRef name,
                           rq::BumpPtrAllocator &allocator,
                           unsigned bucket_count,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module)
      : SymbolTable(rq::EntityKind::SY_CATEGORY, allocator, bucket_count),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), InitialNamed(name) {}
  RQ_ALWAYS_INLINE void
  setDiscriminantTypeExpression(const rq::Expression &type_expression) {
    rq::assignSingleValue(this->_discriminant_type_expression_ptr,
                          &type_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_CATEGORY;
  }
};

struct Procedure : public rq::SymbolTable,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::InitialMaybeNamed {
  using Self = rq::Procedure;

  rq::TypeConstant *_signature_ptr{nullptr};
  const rq::Expression *_signature_expression_ptr{nullptr};

  inline explicit Procedure(rq::EntityKind k, llvm::StringRef name,
                            rq::BumpPtrAllocator &allocator,
                            unsigned bucket_count,
                            const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module)
      : SymbolTable(k, allocator, bucket_count), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialMaybeNamed(name) {}
  inline explicit Procedure(rq::EntityKind k, rq::BumpPtrAllocator &allocator,
                            unsigned bucket_count,
                            const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module)
      : SymbolTable(k, allocator, bucket_count), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module) {}
  RQ_ALWAYS_INLINE void
  setSignatureExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_signature_expression_ptr, &expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsProcedure(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::Procedure> final : std::true_type {};

struct Entry : public rq::Procedure {
  using Self = rq::Entry;

  inline explicit Entry(rq::BumpPtrAllocator &allocator, unsigned bucket_count,
                        const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module)
      : Procedure(rq::EntityKind::SY_ENTRY, allocator, bucket_count, expression,
                  attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_ENTRY;
  }
};

struct Function : public rq::Procedure {
  using Self = rq::Function;

  inline explicit Function(llvm::StringRef name,
                           rq::BumpPtrAllocator &allocator,
                           unsigned bucket_count,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module)
      : Procedure(rq::EntityKind::SY_FUNCTION, name, allocator, bucket_count,
                  expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_FUNCTION;
  }
};

struct Method : public rq::Procedure {
  using Self = rq::Method;

  inline explicit Method(llvm::StringRef name, rq::BumpPtrAllocator &allocator,
                         unsigned bucket_count,
                         const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module)
      : Procedure(rq::EntityKind::SY_METHOD, name, allocator, bucket_count,
                  expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_METHOD;
  }
};

struct Ranger : public rq::Procedure {
  using Self = rq::Ranger;

  inline explicit Ranger(llvm::StringRef name, rq::BumpPtrAllocator &allocator,
                         unsigned bucket_count,
                         const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module)
      : Procedure(rq::EntityKind::SY_METHOD, name, allocator, bucket_count,
                  expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::SY_RANGER;
  }
};

struct ExtensionFunction : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  inline explicit ExtensionFunction(llvm::StringRef name,
                                    rq::BumpPtrAllocator &allocator,
                                    unsigned bucket_count,
                                    const rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module)
      : Procedure(rq::EntityKind::SY_EXTENSION_FUNCTION, name, allocator,
                  bucket_count, expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_FUNCTION;
  }
};

struct ExtensionMethod : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  inline explicit ExtensionMethod(llvm::StringRef name,
                                  rq::BumpPtrAllocator &allocator,
                                  unsigned bucket_count,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module)
      : Procedure(rq::EntityKind::SY_EXTENSION_METHOD, name, allocator,
                  bucket_count, expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_METHOD;
  }
};

struct ExtensionRanger : public rq::Procedure {
  using Self = rq::ExtensionRanger;

  inline explicit ExtensionRanger(llvm::StringRef name,
                                  rq::BumpPtrAllocator &allocator,
                                  unsigned bucket_count,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module)
      : Procedure(rq::EntityKind::SY_EXTENSION_RANGER, name, allocator,
                  bucket_count, expression, attributes, module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_EXTENSION_RANGER;
  }
};

struct Template : public rq::Symbol,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::InitialSymbolTableMember,
                  public rq::InitialNamed {
  using Self = rq::Template;

  rq::TemplateLayout *_template_layout_ptr;

  inline explicit Template(rq::EntityKind k, llvm::StringRef name,
                           rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::SymbolTable &symbol_table,
                           rq::TemplateLayout &template_layout)
      : Symbol(k), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialNamed(name),
        _template_layout_ptr(&template_layout) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateLayout &
  getTemplateLayout() const {
    return rq::dereferencePtr(this->_template_layout_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateLayout &getTemplateLayout() {
    return rq::dereferencePtr(this->_template_layout_ptr);
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsTemplate(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::Template> final : std::true_type {};

struct TemplateClass : public rq::Template {
  using Self = rq::TemplateClass;

  inline explicit TemplateClass(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &symbol_table,
                                rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_CLASS, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_CLASS;
  }
};

struct TemplateEnumeration : public rq::Template {
  using Self = rq::TemplateEnumeration;

  inline explicit TemplateEnumeration(llvm::StringRef name,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::SymbolTable &symbol_table,
                                      rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_ENUMERATION, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_ENUMERATION;
  }
};

struct TemplateCategory : public rq::Template {
  using Self = rq::TemplateCategory;

  inline explicit TemplateCategory(llvm::StringRef name,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::SymbolTable &symbol_table,
                                   rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_CATEGORY, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_CATEGORY;
  }
};

struct TemplateDynamicVariable : public rq::Template {
  using Self = rq::TemplateDynamicVariable;

  inline explicit TemplateDynamicVariable(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::SymbolTable &symbol_table,
                                          rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
  }
};

struct TemplateStaticVariable : public rq::Template {
  using Self = rq::TemplateStaticVariable;

  inline explicit TemplateStaticVariable(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &symbol_table,
                                         rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
  }
};

struct TemplateFunction : public rq::Template {
  using Self = rq::TemplateFunction;

  inline explicit TemplateFunction(llvm::StringRef name,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::SymbolTable &symbol_table,
                                   rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_FUNCTION, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_FUNCTION;
  }
};

struct TemplateMethod : public rq::Template {
  using Self = rq::TemplateMethod;

  inline explicit TemplateMethod(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &symbol_table,
                                 rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_METHOD, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_METHOD;
  }
};

struct TemplateRanger : public rq::Template {
  using Self = rq::TemplateRanger;

  inline explicit TemplateRanger(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &symbol_table,
                                 rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_RANGER, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_RANGER;
  }
};

struct TemplateExtensionFunction : public rq::Template {
  using Self = rq::TemplateExtensionFunction;

  inline explicit TemplateExtensionFunction(llvm::StringRef name,
                                            rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::SymbolTable &symbol_table,
                                            rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION, name,
                 expression, attributes, module, symbol_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
};

struct TemplateExtensionMethod : public rq::Template {
  using Self = rq::TemplateExtensionMethod;

  inline explicit TemplateExtensionMethod(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::SymbolTable &symbol_table,
                                          rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
  }
};

struct TemplateExtensionRanger : public rq::Template {
  using Self = rq::TemplateExtensionRanger;

  inline explicit TemplateExtensionRanger(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::SymbolTable &symbol_table,
                                          rq::TemplateLayout &template_layout)
      : Template(rq::EntityKind::SY_TEMPLATE_EXTENSION_RANGER, name, expression,
                 attributes, module, symbol_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_TEMPLATE_EXTENSION_RANGER;
  }
};

struct Partial : public rq::Symbol,
                 public rq::InitialExpression,
                 public rq::InitialExpressionFlags,
                 public rq::InitialModuleMember,
                 public rq::InitialSymbolTableMember,
                 public rq::InitialNamed {
  using Self = rq::Partial;

  inline explicit Partial(rq::EntityKind k, llvm::StringRef name,
                          rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::SymbolTable &symbol_table)
      : Symbol(k), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        InitialSymbolTableMember(symbol_table), InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsPartial(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::Partial> final : std::true_type {};

struct PartialClass : public rq::Partial {
  using Self = rq::PartialClass;

  inline explicit PartialClass(llvm::StringRef name, rq::Expression &expression,
                               rq::ExpressionFlags attributes,
                               rq::Module &module,
                               rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_CLASS, name, expression, attributes,
                module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_CLASS;
  }
};

struct PartialEnumeration : public rq::Partial {
  using Self = rq::PartialEnumeration;

  inline explicit PartialEnumeration(llvm::StringRef name,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_ENUMERATION, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_ENUMERATION;
  }
};

struct PartialCategory : public rq::Partial {
  using Self = rq::PartialCategory;

  inline explicit PartialCategory(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_CATEGORY, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_CATEGORY;
  }
};

struct PartialDynamicVariable : public rq::Partial {
  using Self = rq::PartialDynamicVariable;

  inline explicit PartialDynamicVariable(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
  }
};

struct PartialStaticVariable : public rq::Partial {
  using Self = rq::PartialStaticVariable;

  inline explicit PartialStaticVariable(llvm::StringRef name,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
  }
};

struct PartialFunction : public rq::Partial {
  using Self = rq::PartialFunction;

  inline explicit PartialFunction(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_FUNCTION, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_FUNCTION;
  }
};

struct PartialMethod : public rq::Partial {
  using Self = rq::PartialMethod;

  inline explicit PartialMethod(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_METHOD, name, expression, attributes,
                module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct PartialRanger : public rq::Partial {
  using Self = rq::PartialRanger;

  inline explicit PartialRanger(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_METHOD, name, expression, attributes,
                module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct PartialExtensionFunction : public rq::Partial {
  using Self = rq::PartialExtensionFunction;

  inline explicit PartialExtensionFunction(llvm::StringRef name,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags attributes,
                                           rq::Module &module,
                                           rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
  }
};

struct PartialExtensionMethod : public rq::Partial {
  using Self = rq::PartialExtensionMethod;

  inline explicit PartialExtensionMethod(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD, name, expression,
                attributes, module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
  }
};

struct PartialExtensionRanger : public rq::Partial {
  using Self = rq::PartialExtensionRanger;

  inline explicit PartialExtensionRanger(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &symbol_table)
      : Partial(rq::EntityKind::SY_PARTIAL_METHOD, name, expression, attributes,
                module, symbol_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::SY_PARTIAL_METHOD;
  }
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  inline explicit Constant(rq::EntityKind k) : Entity(k) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsConstant(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_parent_only<rq::Constant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileTypeConstant(llvm::FoldingSetNodeID &id,
                                          const rq::Symbol &symbol,
                                          rq::TypeFlags attributes) {
  id.AddPointer(&symbol);
  id.AddInteger(static_cast<unsigned>(attributes));
}

struct TypeConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::TypeConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  inline explicit TypeConstant(rq::Symbol &symbol, rq::TypeFlags flags)
      : Constant(rq::EntityKind::CT_TYPE), _symbol_ptr(&symbol),
        _type_flags(flags) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Symbol &getSymbol() const {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Symbol &getSymbol() {
    return rq::dereferencePtr(this->_symbol_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeFlags getTypeFlags() const {
    return this->_type_flags;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutable() const {
    return rq::getHasMutable(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstant() const {
    return rq::getHasConstant(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasPartiallyMutable() const {
    return rq::getHasPartiallyMutable(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatile() const {
    return rq::getHasVolatile(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomic() const {
    return rq::getHasAtomic(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminated() const {
    return rq::getHasNullTerminated(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayDiscard() const {
    return rq::getHasMayDiscard(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndeterminate() const {
    return rq::getHasIndeterminate(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasRanging() const {
    return rq::getHasRanging(this->getTypeFlags());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getHasAttribute(rq::TypeAttribute attribute) const {
    return rq::getHasAttribute(this->getTypeFlags(), attribute);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutability() const {
    return rq::getHasMutability(this->getTypeFlags());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_TYPE;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileTypeConstant(id, this->getSymbol(), this->getTypeFlags());
  }
};

template <> struct is_acquired<rq::TypeConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileExpressionConstant(
    llvm::FoldingSetNodeID &id, const rq::Expression &expression) {
  id.AddPointer(&expression);
}

struct ExpressionConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::ExpressionConstant;

  const rq::Expression *_expression_ptr;

  inline explicit ExpressionConstant(const rq::Expression &expression)
      : Constant(rq::EntityKind::CT_EXPRESSION), _expression_ptr(&expression) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() ==
           rq::EntityKind::CT_EXPRESSION;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileExpressionConstant(id, this->getExpression());
  }
};

template <>
struct is_acquired<rq::ExpressionConstant> final : std::true_type {};

struct BooleanConstant : public rq::Constant {
  using Self = rq::BooleanConstant;

  bool _value;

  inline explicit BooleanConstant(bool value)
      : Constant(rq::EntityKind::CT_BOOLEAN), _value(value) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getValue() const { return this->_value; }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_BOOLEAN;
  }
};

template <> struct is_acquired<rq::BooleanConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileIntegerConstant(llvm::FoldingSetNodeID &id,
                                             const llvm::APInt &int_) {
  int_.Profile(id);
}

struct IntegerConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::IntegerConstant;

  llvm::APInt _ap_int;

  inline explicit IntegerConstant(const llvm::APInt &int_)
      : Constant(rq::EntityKind::CT_INTEGER), _ap_int(int_) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getInt() const {
    return this->_ap_int;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_INTEGER;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileIntegerConstant(id, this->getInt());
  }
};

template <> struct is_acquired<rq::IntegerConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileFloatConstant(llvm::FoldingSetNodeID &id,
                                           const llvm::APFloat &float_) {
  float_.Profile(id);
}

struct FloatConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::FloatConstant;

  llvm::APFloat _ap_float;

  inline explicit FloatConstant(const llvm::APFloat &float_)
      : Constant(rq::EntityKind::CT_FLOAT), _ap_float(float_) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getFloat() const {
    return this->_ap_float;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_FLOAT;
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileFloatConstant(id, this->getFloat());
  }
};

template <> struct is_acquired<rq::FloatConstant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileStringConstant(llvm::FoldingSetNodeID &id,
                                            llvm::StringRef string) {
  id.AddPointer(string.data());
  id.AddInteger(string.size());
}

struct StringConstant : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::StringConstant;

  llvm::StringRef _string;

  inline explicit StringConstant(llvm::StringRef string)
      : Constant(rq::EntityKind::CT_STRING), _string(string) {}
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getString() const {
    return this->_string;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_STRING;
  }

  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileStringConstant(id, this->getString());
  }
};

template <> struct is_acquired<rq::StringConstant> final : std::true_type {};

struct ArrayConstant : public rq::Constant {
  using Self = rq::ArrayConstant;

  inline explicit ArrayConstant() : Constant(rq::EntityKind::CT_ARRAY) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getKind() == rq::EntityKind::CT_ARRAY;
  }
};

template <> struct is_acquired<rq::ArrayConstant> final : std::true_type {};

struct InstructionNode;

using InstructionNext = llvm::PointerUnion<rq::Entity *, rq::InstructionNode *>;

struct InstructionNode final {
  using Self = rq::InstructionNode;

  rq::InstructionNext _car{nullptr};
  rq::InstructionNext _cdr{nullptr};

  InstructionNode() = default;
  InstructionNode(const Self &) = delete;
  InstructionNode(Self &&) = delete;
  ~InstructionNode() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionNext &getCar() {
    return this->_car;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionNext &getCdr() {
    return this->_cdr;
  }

  RQ_ALWAYS_INLINE void clear() {
    this->_car = nullptr;
    this->_cdr = nullptr;
  }
};

template <> struct is_acquired<rq::InstructionNode> final : std::true_type {};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  rq::InstructionNext _cdr;

  inline explicit Instruction() : Entity(rq::EntityKind::OP_NONE) {}

  RQ_ALWAYS_INLINE void clear() {
    this->_kind = rq::EntityKind::OP_NONE;
    this->_cdr = nullptr;
  }

  RQ_ALWAYS_INLINE void setOpcode(rq::EntityKind kind) {
    RQ_ASSERT(rq::getIsOpcode(kind), "not opcode");
    RQ_ASSERT(this->_kind == rq::EntityKind::OP_NONE, "modify opcode");
    this->_kind = kind;
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::InstructionNext &getCdr() {
    return this->_cdr;
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsOpcode(rq::dereferencePtr(entity).getKind());
  }
};

template <> struct is_acquired<rq::Instruction> final : std::true_type {};

} // namespace rq