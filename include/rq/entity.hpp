#pragma once

#include <rq/ast.hpp>
#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
#include <rq/see.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/PointerUnion.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/StringSaver.h>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

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

enum class Opcode : std::uint16_t {
  NONE = 0,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same by comparing their pointers.

  // SPCEIAL
  SY_RESULT,
  SY_IN,
  SY_OUT,

  // SIMPLE BUILTIN
  SY_INFERENCE,
  SY_SYMBOL_CONSTRAINT,
  SY_TYPE_CONSTRAINT,
  SY_EXPRESSION,
  SY_VOID,
  SY_NULL,
  SY_NO_RETURN,
  SY_VARIADIC_ARGUMENTS,
  SY_BOOLEAN,
  SY_SIGNED_CONSTRAINT,
  SY_UNSIGNED_CONSTRAINT,
  SY_FLOAT_CONSTRAINT,
  SY_BINARY_CONSTRAINT,
  SY_BFLOAT_CONSTRAINT,
  SY_HALF,
  SY_SINGLE,
  SY_DOUBLE,
  SY_QUADRUPLE,
  SY_BINARY16,
  SY_BINARY32,
  SY_BINARY64,
  SY_BINARY128,
  SY_BFLOAT16,
  SY_INTEGER_CONSTRAINT,
  SY_SIGNED_INTEGER_CONSTRAINT,
  SY_UNSIGNED_INTEGER_CONSTRAINT,
  SY_CODEUNIT_CONSTRAINT,
  SY_STRING_CONSTRAINT,
  SY_CHAR,
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

  // SYMBOL TABLE
  SY_TOP,
  SY_SCOPE,
  SY_NAMESPACE,
  SY_CLASS,
  SY_ENUMERATION,
  SY_CATEGORY,

  // BINDING
  SY_GLOBAL_VARIABLE,
  SY_LOCAL_VARIABLE,
  SY_STATIC_VARIABLE,
  SY_ENUMERATOR,
  SY_CATEGORY_ALTERNATIVE, // entry within a category referencing a code and
                           // maybe a value type

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
  SY_TEMPLATE_GLOBAL_VARIABLE,
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
  SY_PARTIAL_GLOBAL_VARIABLE,
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

  // =====INSTRUCTIONS=====

  // none | 0:statement 1:statement
  IN_EXECUTE,
  // 0 | 0:type 1:value
  IN_CAST,

  // boolean | 0:boolean 1:boolean
  IN_LOGICAL_AND,
  // boolean | 0:boolean 1:boolean
  IN_LOGICAL_OR,
  // boolean | 0:boolean
  IN_LOGICAL_COMPLEMENT,

  // boolean | 0:value 1:value
  IN_EQ,
  // boolean | 0:value 1:value
  IN_NE,
  // boolean | 0:numeric 1:numeric
  IN_GT,
  // boolean | 0:numeric 1:numeric
  IN_GE,
  // boolean | 0:numeric 1:numeric
  IN_LT,
  // boolean | 0:numeric 1:numeric
  IN_LE,

  // int | 0:int 1:int
  IN_AND,
  // int | 0:int 1:int
  IN_OR,
  // int | 0:int 1:int
  IN_XOR,
  // int | 0:int
  IN_COMPLEMENT,
  // int | 0:int 1:int
  IN_SHIFT_LEFT,
  // int | 0:int 1:int
  IN_SHIFT_RIGHT,
  // int | 0:int 1:int
  IN_ROTATE_LEFT,
  // int | 0:int 1:int
  IN_ROTATE_RIGHT,

  // numeric | 0:numeric 1:numeric
  IN_ADD,
  // numeric | 0:numeric 1:numeric
  IN_SUBTRACT,
  // numeric | 0:numeric 1:numeric
  IN_MULTIPLY,
  // numeric | 0:numeric 1:numeric
  IN_DIVIDE,
  // numeric | 0:numeric 1:numeric
  IN_MODULUS,
  // numeric | 0:numeric
  IN_NEGATE,

  // value | 0:pointer
  IN_CONTENT,
  // pointer | 0:value
  IN_ADDRESS,
  // value | 0:location
  IN_VALUE,
  // value | 0:location
  IN_REF,
  // value | 0:location 1:index
  IN_INDEX,
  // value | 0:value
  IN_DATA_COUNT,
  // none | 0:value 1:next arg (or last value)
  IN_ARG,
  // none | 0:procedure 1:arg0
  IN_CALL,
  // none | 0:location 1:location
  IN_MOVE,
  // none | 0:location 1:value
  IN_COPY,

  // none |
  IN_RET,
  // none | 0:boolean 1:fork
  IN_COND,
  // none | 0:if 1:else
  IN_FORK,

  // none |
  IN_DEBUG_TRAP,
  // none |
  IN_UNREACHABLE,
  // none | 0:boolean
  IN_ASSUME,

  LAST
};

static constexpr std::size_t OPCODE_COUNT =
    static_cast<std::size_t>(rq::Opcode::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::Opcode opcode) {
  using O = rq::Opcode;
  switch (opcode) {
  case O::NONE:
    return "none";
  case O::SY_RESULT:
    return "sy_result";
  case O::SY_IN:
    return "sy_in";
  case O::SY_OUT:
    return "sy_out";
  case O::SY_INFERENCE:
    return "sy_inference";
  case O::SY_SYMBOL_CONSTRAINT:
    return "sy_symbol_constraint";
  case O::SY_TYPE_CONSTRAINT:
    return "sy_type_constraint";
  case O::SY_EXPRESSION:
    return "sy_expression";
  case O::SY_VOID:
    return "sy_void";
  case O::SY_NULL:
    return "sy_null";
  case O::SY_NO_RETURN:
    return "sy_no_return";
  case O::SY_VARIADIC_ARGUMENTS:
    return "sy_variadic_arguments";
  case O::SY_BOOLEAN:
    return "sy_boolean";
  case O::SY_SIGNED_CONSTRAINT:
    return "sy_signed_constraint";
  case O::SY_UNSIGNED_CONSTRAINT:
    return "sy_unsigned_constraint";
  case O::SY_FLOAT_CONSTRAINT:
    return "sy_float_constraint";
  case O::SY_BINARY_CONSTRAINT:
    return "sy_binary_constraint";
  case O::SY_BFLOAT_CONSTRAINT:
    return "sy_bfloat_constraint";
  case O::SY_HALF:
    return "sy_half";
  case O::SY_SINGLE:
    return "sy_single";
  case O::SY_DOUBLE:
    return "sy_double";
  case O::SY_QUADRUPLE:
    return "sy_quadruple";
  case O::SY_BINARY16:
    return "sy_binary16";
  case O::SY_BINARY32:
    return "sy_binary32";
  case O::SY_BINARY64:
    return "sy_binary64";
  case O::SY_BINARY128:
    return "sy_binary128";
  case O::SY_BFLOAT16:
    return "sy_Bfloat16";
  case O::SY_INTEGER_CONSTRAINT:
    return "sy_integer_constraint";
  case O::SY_SIGNED_INTEGER_CONSTRAINT:
    return "sy_signed_integer_constraint";
  case O::SY_UNSIGNED_INTEGER_CONSTRAINT:
    return "sy_unsigned_integer_constraint";
  case O::SY_CODEUNIT_CONSTRAINT:
    return "sy_codeunit_constraint";
  case O::SY_STRING_CONSTRAINT:
    return "sy_string_constraint";
  case O::SY_CHAR:
    return "sy_char";
  case O::SY_ASCII:
    return "sy_ascii";
  case O::SY_UTF8:
    return "sy_utf8";
  case O::SY_SCALED_SIGNED_INTEGER:
    return "sy_scaled_signed_integer";
  case O::SY_SCALED_UNSIGNED_INTEGER:
    return "sy_scaled_unsigned_integer";
  case O::SY_REFERENCE:
    return "sy_reference";
  case O::SY_POINTER:
    return "sy_pointer";
  case O::SY_FAT_POINTER:
    return "sy_fat_pointer";
  case O::SY_INFERENCED_COUNT_ARRAY:
    return "sy_inferenced_count_array";
  case O::SY_ARRAY:
    return "sy_array";
  case O::SY_LAYOUT:
    return "sy_layout";
  case O::SY_CLASS_LAYOUT:
    return "sy_class_layout";
  case O::SY_TEMPLATE_LAYOUT:
    return "sy_template_layout";
  case O::SY_SIGNATURE:
    return "sy_signature";
  case O::SY_CLASS_PARAMETER:
    return "sy_class_parameter";
  case O::SY_LAYOUT_PARAMETER:
    return "sy_layout_parameter";
  case O::SY_TEMPLATE_PARAMETER:
    return "sy_template_parameter";
  case O::SY_SIGNATURE_PARAMETER:
    return "sy_signature_parameter";
  case O::SY_ARITHMETIC_INTERVAL:
    return "sy_arithmetic_interval";
  case O::SY_FINITE_ARITHMETIC_PROGRESSION:
    return "sy_finite_arithmetic_progression";
  case O::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return "sy_infinite_arithmetic_progression";
  case O::SY_MODULE:
    return "sy_module";
  case O::SY_IMPORT:
    return "sy_import";
  case O::SY_CODE:
    return "sy_code";
  case O::SY_CATEGORY_DISCRIMINANT:
    return "sy_category_discriminant";
  case O::SY_LABEL:
    return "sy_label";
  case O::SY_SYNONYM:
    return "sy_synonym";
  case O::SY_TOP:
    return "sy_top";
  case O::SY_SCOPE:
    return "sy_scope";
  case O::SY_NAMESPACE:
    return "sy_namespace";
  case O::SY_CLASS:
    return "sy_class";
  case O::SY_ENUMERATION:
    return "sy_enumeration";
  case O::SY_CATEGORY:
    return "sy_category";
  case O::SY_GLOBAL_VARIABLE:
    return "sy_global_variable";
  case O::SY_LOCAL_VARIABLE:
    return "sy_local_variable";
  case O::SY_STATIC_VARIABLE:
    return "sy_static_variable";
  case O::SY_ENUMERATOR:
    return "sy_enumerator";
  case O::SY_CATEGORY_ALTERNATIVE:
    return "sy_category_alternative";
  case O::SY_ENTRY:
    return "sy_entry";
  case O::SY_FUNCTION:
    return "sy_function";
  case O::SY_METHOD:
    return "sy_method";
  case O::SY_RANGER:
    return "sy_ranger";
  case O::SY_EXTENSION_FUNCTION:
    return "sy_extension_function";
  case O::SY_EXTENSION_METHOD:
    return "sy_extension_method";
  case O::SY_EXTENSION_RANGER:
    return "sy_extension_ranger";
  case O::SY_TEMPLATE_CLASS:
    return "sy_template_class";
  case O::SY_TEMPLATE_ENUMERATION:
    return "sy_template_enumeration";
  case O::SY_TEMPLATE_CATEGORY:
    return "sy_template_category";
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    return "sy_template_global_variable";
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    return "sy_template_static_variable";
  case O::SY_TEMPLATE_FUNCTION:
    return "sy_template_function";
  case O::SY_TEMPLATE_METHOD:
    return "sy_template_method";
  case O::SY_TEMPLATE_RANGER:
    return "sy_template_ranger";
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return "sy_template_extension_function";
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return "sy_template_extension_method";
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    return "sy_template_extension_ranger";
  case O::SY_PARTIAL_CLASS:
    return "sy_partial_class";
  case O::SY_PARTIAL_ENUMERATION:
    return "sy_partial_enumeration";
  case O::SY_PARTIAL_CATEGORY:
    return "sy_partial_category";
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return "sy_partial_global_variable";
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return "sy_partial_static_variable";
  case O::SY_PARTIAL_FUNCTION:
    return "sy_partial_function";
  case O::SY_PARTIAL_METHOD:
    return "sy_partial_method";
  case O::SY_PARTIAL_RANGER:
    return "sy_partial_ranger";
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return "sy_partial_extension_function";
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return "sy_partial_extension_method";
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return "sy_partial_extension_ranger";
  case O::CT_TYPE:
    return "ct_type";
  case O::CT_EXPRESSION:
    return "ct_expression";
  case O::CT_BOOLEAN:
    return "ct_boolean";
  case O::CT_INTEGER:
    return "ct_integer";
  case O::CT_FLOAT:
    return "ct_float";
  case O::CT_STRING:
    return "ct_string";
  case O::CT_ARRAY:
    return "ct_array";
  case O::IN_EXECUTE:
    return "in_execute";
  case O::IN_CAST:
    return "in_cast";
  case O::IN_LOGICAL_AND:
    return "in_logical_and";
  case O::IN_LOGICAL_OR:
    return "in_logical_or";
  case O::IN_LOGICAL_COMPLEMENT:
    return "in_logical_complement";
  case O::IN_EQ:
    return "in_eq";
  case O::IN_NE:
    return "in_ne";
  case O::IN_GT:
    return "in_gt";
  case O::IN_GE:
    return "in_ge";
  case O::IN_LT:
    return "in_lt";
  case O::IN_LE:
    return "in_le";
  case O::IN_AND:
    return "in_and";
  case O::IN_OR:
    return "in_or";
  case O::IN_XOR:
    return "in_xor";
  case O::IN_COMPLEMENT:
    return "in_complement";
  case O::IN_SHIFT_LEFT:
    return "in_shift_left";
  case O::IN_SHIFT_RIGHT:
    return "in_shift_right";
  case O::IN_ROTATE_LEFT:
    return "in_rotate_left";
  case O::IN_ROTATE_RIGHT:
    return "in_rotate_right";
  case O::IN_ADD:
    return "in_add";
  case O::IN_SUBTRACT:
    return "in_subtract";
  case O::IN_MULTIPLY:
    return "in_multiply";
  case O::IN_DIVIDE:
    return "in_divide";
  case O::IN_MODULUS:
    return "in_modulus";
  case O::IN_NEGATE:
    return "in_negate";
  case O::IN_CONTENT:
    return "in_content";
  case O::IN_ADDRESS:
    return "in_address";
  case O::IN_VALUE:
    return "in_value";
  case O::IN_REF:
    return "in_ref";
  case O::IN_INDEX:
    return "in_index";
  case O::IN_DATA_COUNT:
    return "in_data_count";
  case O::IN_ARG:
    return "in_arg";
  case O::IN_CALL:
    return "in_call";
  case O::IN_MOVE:
    return "in_move";
  case O::IN_COPY:
    return "in_copy";
  case O::IN_RET:
    return "in_ret";
  case O::IN_COND:
    return "in_cond";
  case O::IN_FORK:
    return "in_fork";
  case O::IN_DEBUG_TRAP:
    return "in_debug_trap";
  case O::IN_UNREACHABLE:
    return "in_unreachable";
  case O::IN_ASSUME:
    return "in_assume";

  case O::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

enum class OpcodeFlags : std::uint32_t {
  NONE = 0,

  // CATEGORIES
  // the first two bits used to determine category
  // the rest of the bits mean different things depending on the
  // category.
  SYMBOL = rq::getBit(0),
  CONSTANT = rq::getBit(1),
  NOT_INSTRUCTION_MASK = (rq::getBit(0) | rq::getBit(1)),

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
  SY_DYNAMIC_VARIABLE = rq::getBit(9),
  SY_SYMBOL_TABLE = rq::getBit(10),
  SY_PROCEDURE = rq::getBit(11),
  SY_TEMPLATE = rq::getBit(12),
  SY_PARTIAL = rq::getBit(13),
  // SYMBOL INFO PROPERTIES - have no data associated
  SY_HAS_TEMPLATE_ALTERNATIVE = rq::getBit(14),
  SY_TYPE = rq::getBit(15),
  SY_SUBTYPE = rq::getBit(16),
  SY_CONSTRAINT = rq::getBit(17),
  SY_PLATFORM_CHANGING = rq::getBit(18),
  SY_INTEGER = rq::getBit(19),
  SY_FLOAT = rq::getBit(20),
  SY_BINARY = rq::getBit(21),
  SY_CODEUNIT = rq::getBit(22),
  SY_SIGNED = rq::getBit(23),
  SY_UNSIGNED = rq::getBit(24),
  SY_TIN_OF_FRAME = rq::getBit(25),

  // CONSTANT FLAGS
  // TODO

  // INSTRUCTION FLAGS
  IN_NULLARY = rq::getBit(2),
  IN_UNARY = rq::getBit(3),
  IN_BINARY = rq::getBit(4),
};
template <> struct is_flags<OpcodeFlags> : std::true_type {};
[[nodiscard]] inline rq::OpcodeFlags getFlags(rq::Opcode opcode) {
  using O = rq::Opcode;
  using OF = rq::OpcodeFlags;
  switch (opcode) {
  case O::NONE:
    return OF::NONE;
  case O::SY_RESULT:
    return OF::SYMBOL;
  case O::SY_IN:
    return OF::SYMBOL;
  case O::SY_OUT:
    return OF::SYMBOL;
  case O::SY_INFERENCE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_SYMBOL_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_TYPE_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_EXPRESSION:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_VOID:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_NULL:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_NO_RETURN:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_VARIADIC_ARGUMENTS:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_BOOLEAN:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE;
  case O::SY_SIGNED_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_SIGNED;
  case O::SY_UNSIGNED_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_FLOAT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_BINARY_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BFLOAT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_HALF:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_SINGLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_DOUBLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_QUADRUPLE:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_PLATFORM_CHANGING | OF::SY_FLOAT | OF::SY_SIGNED;
  case O::SY_BINARY16:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY32:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY64:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BINARY128:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_BINARY | OF::SY_SIGNED;
  case O::SY_BFLOAT16:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_FLOAT |
           OF::SY_SIGNED;
  case O::SY_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER;
  case O::SY_SIGNED_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER | OF::SY_SIGNED;
  case O::SY_UNSIGNED_INTEGER_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_INTEGER | OF::SY_UNSIGNED;
  case O::SY_CODEUNIT_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE |
           OF::SY_CONSTRAINT | OF::SY_CODEUNIT;
  case O::SY_STRING_CONSTRAINT:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CONSTRAINT;
  case O::SY_CHAR:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT |
           OF::SY_PLATFORM_CHANGING;
  case O::SY_ASCII:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_UTF8:
    return OF::SYMBOL | OF::SY_SIMPLE_BUILTIN | OF::SY_TYPE | OF::SY_CODEUNIT;
  case O::SY_SCALED_SIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_BUILTIN | OF::SY_TYPE | OF::SY_INTEGER |
           OF::SY_SIGNED;
  case O::SY_SCALED_UNSIGNED_INTEGER:
    return OF::SYMBOL | OF::SY_SCALED_BUILTIN | OF::SY_TYPE | OF::SY_INTEGER |
           OF::SY_UNSIGNED;
  case O::SY_REFERENCE:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_POINTER:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_FAT_POINTER:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_INFERENCED_COUNT_ARRAY:
    return OF::SYMBOL | OF::SY_UNARY_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_ARRAY:
    return OF::SYMBOL | OF::SY_COUNTED_SUBTYPE | OF::SY_TYPE | OF::SY_SUBTYPE;
  case O::SY_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_CLASS_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_TEMPLATE_LAYOUT:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_SIGNATURE:
    return OF::SYMBOL | OF::SY_PARAMETER_LIST_SUBTYPE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_CLASS_PARAMETER:
    return OF::SYMBOL;
  case O::SY_LAYOUT_PARAMETER:
    return OF::SYMBOL;
  case O::SY_TEMPLATE_PARAMETER:
    return OF::SYMBOL;
  case O::SY_SIGNATURE_PARAMETER:
    return OF::SYMBOL;
  case O::SY_ARITHMETIC_INTERVAL:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_FINITE_ARITHMETIC_PROGRESSION:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_INFINITE_ARITHMETIC_PROGRESSION:
    return OF::SYMBOL | OF::SY_ARITHMETIC_SEQUENCE | OF::SY_TYPE |
           OF::SY_SUBTYPE;
  case O::SY_MODULE:
    return OF::SYMBOL;
  case O::SY_IMPORT:
    return OF::SYMBOL;
  case O::SY_CODE:
    return OF::SYMBOL;
  case O::SY_CATEGORY_DISCRIMINANT:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_LABEL:
    return OF::SYMBOL;
  case O::SY_SYNONYM:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_TOP:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TIN_OF_FRAME;
  case O::SY_SCOPE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE;
  case O::SY_NAMESPACE:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TIN_OF_FRAME;
  case O::SY_CLASS:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_ENUMERATION:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_CATEGORY:
    return OF::SYMBOL | OF::SY_SYMBOL_TABLE | OF::SY_TYPE |
           OF::SY_TIN_OF_FRAME | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_DYNAMIC_VARIABLE |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_LOCAL_VARIABLE:
    return OF::SYMBOL | OF::SY_DYNAMIC_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_ENUMERATOR:
    return OF::SYMBOL | OF::SY_TYPE;
  case O::SY_CATEGORY_ALTERNATIVE:
    return OF::SYMBOL;
  case O::SY_ENTRY:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME;
  case O::SY_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_RANGER:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_PROCEDURE | OF::SY_TIN_OF_FRAME |
           OF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case O::SY_TEMPLATE_CLASS:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_ENUMERATION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_CATEGORY:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_RANGER:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_TEMPLATE;
  case O::SY_PARTIAL_CLASS:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_ENUMERATION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_CATEGORY:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_FUNCTION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_METHOD:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_RANGER:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return OF::SYMBOL | OF::SY_PARTIAL;
  case O::CT_TYPE:
    return OF::CONSTANT;
  case O::CT_EXPRESSION:
    return OF::CONSTANT;
  case O::CT_BOOLEAN:
    return OF::CONSTANT;
  case O::CT_INTEGER:
    return OF::CONSTANT;
  case O::CT_FLOAT:
    return OF::CONSTANT;
  case O::CT_STRING:
    return OF::CONSTANT;
  case O::CT_ARRAY:
    return OF::CONSTANT;
  case O::IN_EXECUTE:
    return OF::IN_BINARY;
  case O::IN_CAST:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_AND:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_OR:
    return OF::IN_BINARY;
  case O::IN_LOGICAL_COMPLEMENT:
    return OF::IN_UNARY;
  case O::IN_EQ:
    return OF::IN_BINARY;
  case O::IN_NE:
    return OF::IN_BINARY;
  case O::IN_GT:
    return OF::IN_BINARY;
  case O::IN_GE:
    return OF::IN_BINARY;
  case O::IN_LT:
    return OF::IN_BINARY;
  case O::IN_LE:
    return OF::IN_BINARY;
  case O::IN_AND:
    return OF::IN_BINARY;
  case O::IN_OR:
    return OF::IN_BINARY;
  case O::IN_XOR:
    return OF::IN_BINARY;
  case O::IN_COMPLEMENT:
    return OF::IN_UNARY;
  case O::IN_SHIFT_LEFT:
    return OF::IN_BINARY;
  case O::IN_SHIFT_RIGHT:
    return OF::IN_BINARY;
  case O::IN_ROTATE_LEFT:
    return OF::IN_BINARY;
  case O::IN_ROTATE_RIGHT:
    return OF::IN_BINARY;
  case O::IN_ADD:
    return OF::IN_BINARY;
  case O::IN_SUBTRACT:
    return OF::IN_BINARY;
  case O::IN_MULTIPLY:
    return OF::IN_BINARY;
  case O::IN_DIVIDE:
    return OF::IN_BINARY;
  case O::IN_MODULUS:
    return OF::IN_BINARY;
  case O::IN_NEGATE:
    return OF::IN_UNARY;
  case O::IN_CONTENT:
    return OF::IN_UNARY;
  case O::IN_ADDRESS:
    return OF::IN_UNARY;
  case O::IN_VALUE:
    return OF::IN_UNARY;
  case O::IN_REF:
    return OF::IN_UNARY;
  case O::IN_INDEX:
    return OF::IN_BINARY;
  case O::IN_DATA_COUNT:
    return OF::IN_UNARY;
  case O::IN_ARG:
    return OF::IN_BINARY;
  case O::IN_CALL:
    return OF::IN_BINARY;
  case O::IN_MOVE:
    return OF::IN_BINARY;
  case O::IN_COPY:
    return OF::IN_BINARY;
  case O::IN_RET:
    return OF::IN_NULLARY;
  case O::IN_COND:
    return OF::IN_BINARY;
  case O::IN_FORK:
    return OF::IN_BINARY;
  case O::IN_DEBUG_TRAP:
    return OF::IN_NULLARY;
  case O::IN_UNREACHABLE:
    return OF::IN_NULLARY;
  case O::IN_ASSUME:
    return OF::IN_UNARY;
  case O::LAST:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SYMBOL);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::CONSTANT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction(rq::Opcode opcode) {
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasNone(flags, rq::OpcodeFlags::NOT_INSTRUCTION_MASK);
}

#define RQ_ASSERT_SYMBOL(opcode)                                               \
  RQ_ASSERT(rq::getIsSymbol((opcode)), "not symbol")
#define RQ_ASSERT_CONSTANT(opcode)                                             \
  RQ_ASSERT(rq::getIsConstant((opcode)), "not constant")

#define RQ_ASSERT_INSTRUCTION(opcode)                                          \
  RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction")

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIMPLE_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SCALED_BUILTIN);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNARY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_COUNTED_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterListSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARAMETER_LIST_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_ARITHMETIC_SEQUENCE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_DYNAMIC_VARIABLE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SYMBOL_TABLE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PROCEDURE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TEMPLATE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PARTIAL);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTemplateAlternative(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_HAS_TEMPLATE_ALTERNATIVE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsType(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SUBTYPE);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CONSTRAINT);
}
// NOTE: SCALED_SIGNED_INTEGER and SCALED_UNSIGNED_INTEGER is platform changing
// only if depth is not exact that is checked in member function of Entity, not
// here.
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_PLATFORM_CHANGING);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags,
                       rq::OpcodeFlags::SY_INTEGER | rq::OpcodeFlags::SY_FLOAT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_FLOAT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_BINARY);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_CODEUNIT);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNSIGNED);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_SIGNED |
                                  rq::OpcodeFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_UNSIGNED |
                                  rq::OpcodeFlags::SY_INTEGER);
}
[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::SY_TIN_OF_FRAME);
}
[[nodiscard]] inline rq::Opcode getTemplate(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_TEMPLATE_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_TEMPLATE_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_TEMPLATE_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_TEMPLATE_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_TEMPLATE_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_TEMPLATE_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_TEMPLATE_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_TEMPLATE_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_TEMPLATE_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_TEMPLATE_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_TEMPLATE_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] inline rq::Opcode getPartial(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_PARTIAL_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_PARTIAL_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_PARTIAL_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_PARTIAL_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_PARTIAL_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_PARTIAL_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_PARTIAL_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_PARTIAL_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_PARTIAL_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_PARTIAL_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_PARTIAL_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}
[[nodiscard]] inline rq::Opcode getFull(rq::Opcode opcode) {
  RQ_ASSERT_SYMBOL(opcode);
  RQ_ASSERT(rq::getHasTemplateAlternative(opcode), "no template alternative");
  using O = rq::Opcode;
  switch (opcode) {
  case O::SY_CLASS:
    [[fallthrough]];
  case O::SY_TEMPLATE_CLASS:
    [[fallthrough]];
  case O::SY_PARTIAL_CLASS:
    return O::SY_CLASS;
  case O::SY_ENUMERATION:
    [[fallthrough]];
  case O::SY_TEMPLATE_ENUMERATION:
    [[fallthrough]];
  case O::SY_PARTIAL_ENUMERATION:
    return O::SY_ENUMERATION;
  case O::SY_CATEGORY:
    [[fallthrough]];
  case O::SY_TEMPLATE_CATEGORY:
    [[fallthrough]];
  case O::SY_PARTIAL_CATEGORY:
    return O::SY_CATEGORY;
  case O::SY_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_GLOBAL_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_GLOBAL_VARIABLE:
    return O::SY_GLOBAL_VARIABLE;
  case O::SY_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_TEMPLATE_STATIC_VARIABLE:
    [[fallthrough]];
  case O::SY_PARTIAL_STATIC_VARIABLE:
    return O::SY_STATIC_VARIABLE;
  case O::SY_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_FUNCTION:
    return O::SY_FUNCTION;
  case O::SY_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_METHOD:
    return O::SY_METHOD;
  case O::SY_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_RANGER:
    return O::SY_RANGER;
  case O::SY_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_FUNCTION:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_FUNCTION:
    return O::SY_EXTENSION_FUNCTION;
  case O::SY_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_METHOD:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_METHOD:
    return O::SY_EXTENSION_METHOD;
  case O::SY_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_TEMPLATE_EXTENSION_RANGER:
    [[fallthrough]];
  case O::SY_PARTIAL_EXTENSION_RANGER:
    return O::SY_EXTENSION_RANGER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_NULLARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_UNARY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinaryInstruction(rq::Opcode opcode) {
  RQ_ASSERT_INSTRUCTION(opcode);
  const rq::OpcodeFlags flags = rq::getFlags(opcode);
  return rq::getHasAll(flags, rq::OpcodeFlags::IN_BINARY);
}

struct Entity;
struct Symbol;
struct Result;
struct In;
struct Out;
struct SimpleBuiltin;
struct Inference;
struct SymbolConstraint;
struct TypeConstraint;
struct Void;
struct Null;
struct NoReturn;
struct VariadicArguments;
struct Boolean;
struct SignedConstraint;
struct UnsignedConstraint;
struct FloatConstraint;
struct BinaryConstraint;
struct BfloatConstraint;
struct Half;
struct Single;
struct Double;
struct Quadruple;
struct Binary16;
struct Binary32;
struct Binary64;
struct Binary128;
struct Bfloat16;
struct IntegerConstraint;
struct SignedIntegerConstraint;
struct UnsignedIntegerConstraint;
struct CodeunitConstraint;
struct StringConstraint;
struct Char;
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
struct GlobalVariable;
struct LocalVariable;
struct StaticVariable;
struct Enumerator;
struct CategoryAlternative;
struct SymbolTable;
struct Top;
struct Scope;
struct Namespace;
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
struct TemplateGlobalVariable;
struct TemplateLocalVariable;
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
struct PartialGlobalVariable;
struct PartialLocalVariable;
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
struct UnaryInstruction;
struct BinaryInstruction;
struct TernaryInstruction;

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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocation() const {
    return rq::getHasLocation(this->_expression_flags);
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
struct SymbolTableMember {
  using Self = rq::SymbolTableMember;

  rq::SymbolTable *_containing_symbol_table_ptr{nullptr};

  inline explicit SymbolTableMember() = default;
  inline explicit SymbolTableMember(rq::SymbolTable &containing_table)
      : _containing_symbol_table_ptr(&containing_table) {}
  SymbolTableMember(const Self &) = delete;
  SymbolTableMember(Self &&) = delete;
  ~SymbolTableMember() = default;
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
struct SymbolTableHosted {
  using Self = rq::SymbolTableHosted;

  rq::SymbolTable *_hosting_symbol_table_ptr{nullptr};

  inline explicit SymbolTableHosted(rq::SymbolTable &hosting_table)
      : _hosting_symbol_table_ptr(&hosting_table) {}
  SymbolTableHosted(const Self &) = delete;
  SymbolTableHosted(Self &&) = delete;
  ~SymbolTableHosted() = default;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  const rq::SymbolTable &getHostingSymbolTable() const {
    return rq::dereferencePtr(this->_hosting_symbol_table_ptr);
  }
  rq::SymbolTable &getHostingSymbolTable() {
    return rq::dereferencePtr(this->_hosting_symbol_table_ptr);
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

  rq::Opcode _opcode;

  inline explicit Entity(rq::Opcode opcode) : _opcode(opcode) {}
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

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getOpcode() const {
    return this->_opcode;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const {
    return rq::getHasTemplateAlternative(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return rq::getIsType(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const {
    return rq::getIsSubtype(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameter() const {
    return rq::getIsParameter(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterListSubtype() const {
    return rq::getIsParameterListSubtype(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstraint() const {
    return rq::getIsConstraint(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging() const {
    if (rq::getIsScaledBuiltin(this->getOpcode())) {
      // TODO
    }
    return rq::getIsPlatformChanging(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNumeric() const {
    return rq::getIsNumeric(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsInteger(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloat(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary() const {
    return rq::getIsBinary(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunit(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return rq::getIsSigned(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return rq::getIsUnsigned(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignedInteger() const {
    return rq::getIsSignedInteger(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsignedInteger() const {
    return rq::getIsUnsignedInteger(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopOfFrame() const {
    return rq::getIsTopOfFrame(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getTemplateopcode() const {
    return rq::getTemplate(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getPartialopcode() const {
    return rq::getPartial(this->getOpcode());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Opcode getFullopcode() const {
    return rq::getFull(this->getOpcode());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    std::ignore = entity;
    return true;
  }
};

template <> struct is_parent_only<rq::Entity> final : std::true_type {};

struct Symbol : public rq::Entity {
  using Self = rq::Symbol;

  inline explicit Symbol(rq::Opcode opcode) : Entity(opcode) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSymbol(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::Symbol> final : std::true_type {};

struct Result : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit Result() : Symbol(rq::Opcode::SY_RESULT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_RESULT;
  }
};

template <> struct is_acquired<rq::Result> final : std::true_type {};

struct In : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit In() : Symbol(rq::Opcode::SY_IN) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_IN;
  }
};

template <> struct is_acquired<rq::In> final : std::true_type {};

struct Out : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit Out() : Symbol(rq::Opcode::SY_OUT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_OUT;
  }
};

template <> struct is_acquired<rq::Out> final : std::true_type {};

struct SimpleBuiltin : public rq::Symbol {
  using Self = rq::SimpleBuiltin;

  inline explicit SimpleBuiltin(rq::Opcode opcode) : Symbol(opcode) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSimpleBuiltin(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_acquired<rq::SimpleBuiltin> final : std::true_type {};

struct Inference final : public rq::SimpleBuiltin {
  using Self = rq::Inference;

  inline explicit Inference() : SimpleBuiltin(rq::Opcode::SY_INFERENCE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_INFERENCE;
  }
};

template <> struct is_acquired<rq::Inference> final : std::true_type {};

struct SymbolConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SymbolConstraint;

  inline explicit SymbolConstraint()
      : SimpleBuiltin(rq::Opcode::SY_SYMBOL_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SYMBOL_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::SymbolConstraint> final : std::true_type {};

struct TypeConstraint final : public rq::SimpleBuiltin {
  using Self = rq::TypeConstraint;

  inline explicit TypeConstraint()
      : SimpleBuiltin(rq::Opcode::SY_TYPE_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TYPE_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::TypeConstraint> final : std::true_type {};

struct Void final : public rq::SimpleBuiltin {
  using Self = rq::Void;

  inline explicit Void() : SimpleBuiltin(rq::Opcode::SY_VOID) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_VOID;
  }
};

template <> struct is_acquired<rq::Void> final : std::true_type {};

struct Null final : public rq::SimpleBuiltin {
  using Self = rq::Null;

  inline explicit Null() : SimpleBuiltin(rq::Opcode::SY_NULL) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NULL;
  }
};

template <> struct is_acquired<rq::Null> final : std::true_type {};

struct NoReturn final : public rq::SimpleBuiltin {
  using Self = rq::NoReturn;

  inline explicit NoReturn() : SimpleBuiltin(rq::Opcode::SY_NO_RETURN) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NO_RETURN;
  }
};

template <> struct is_acquired<rq::NoReturn> final : std::true_type {};

struct VariadicArguments final : public rq::SimpleBuiltin {
  using Self = rq::VariadicArguments;

  inline explicit VariadicArguments()
      : SimpleBuiltin(rq::Opcode::SY_VARIADIC_ARGUMENTS) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_VARIADIC_ARGUMENTS;
  }
};

template <> struct is_acquired<rq::VariadicArguments> final : std::true_type {};

struct Boolean final : public rq::SimpleBuiltin {
  using Self = rq::Boolean;

  inline explicit Boolean() : SimpleBuiltin(rq::Opcode::SY_BOOLEAN) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BOOLEAN;
  }
};

template <> struct is_acquired<rq::Boolean> final : std::true_type {};

struct SignedConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SignedConstraint;

  inline explicit SignedConstraint()
      : SimpleBuiltin(rq::Opcode::SY_SIGNED_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SIGNED_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::SignedConstraint> final : std::true_type {};

struct UnsignedConstraint final : public rq::SimpleBuiltin {
  using Self = rq::UnsignedConstraint;

  inline explicit UnsignedConstraint()
      : SimpleBuiltin(rq::Opcode::SY_UNSIGNED_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_UNSIGNED_CONSTRAINT;
  }
};

template <>
struct is_acquired<rq::UnsignedConstraint> final : std::true_type {};

struct FloatConstraint final : public rq::SimpleBuiltin {
  using Self = rq::FloatConstraint;

  inline explicit FloatConstraint()
      : SimpleBuiltin(rq::Opcode::SY_FLOAT_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_FLOAT_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::FloatConstraint> final : std::true_type {};

struct BinaryConstraint final : public rq::SimpleBuiltin {
  using Self = rq::BinaryConstraint;

  inline explicit BinaryConstraint()
      : SimpleBuiltin(rq::Opcode::SY_BINARY_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_BINARY_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::BinaryConstraint> final : std::true_type {};

struct BfloatConstraint final : public rq::SimpleBuiltin {
  using Self = rq::BfloatConstraint;

  inline explicit BfloatConstraint()
      : SimpleBuiltin(rq::Opcode::SY_BFLOAT_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_BFLOAT_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::BfloatConstraint> final : std::true_type {};

struct Half final : public rq::SimpleBuiltin {
  using Self = rq::Half;

  inline explicit Half() : SimpleBuiltin(rq::Opcode::SY_HALF) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_HALF;
  }
};

template <> struct is_acquired<rq::Half> final : std::true_type {};

struct Single final : public rq::SimpleBuiltin {
  using Self = rq::Single;

  inline explicit Single() : SimpleBuiltin(rq::Opcode::SY_SINGLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SINGLE;
  }
};

template <> struct is_acquired<rq::Single> final : std::true_type {};

struct Double final : public rq::SimpleBuiltin {
  using Self = rq::Double;

  inline explicit Double() : SimpleBuiltin(rq::Opcode::SY_DOUBLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_DOUBLE;
  }
};

template <> struct is_acquired<rq::Double> final : std::true_type {};

struct Quadruple final : public rq::SimpleBuiltin {
  using Self = rq::Quadruple;

  inline explicit Quadruple() : SimpleBuiltin(rq::Opcode::SY_QUADRUPLE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_QUADRUPLE;
  }
};

template <> struct is_acquired<rq::Quadruple> final : std::true_type {};

struct Binary16 final : public rq::SimpleBuiltin {
  using Self = rq::Binary16;

  inline explicit Binary16() : SimpleBuiltin(rq::Opcode::SY_BINARY16) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY16;
  }
};

template <> struct is_acquired<rq::Binary16> final : std::true_type {};

struct Binary32 final : public rq::SimpleBuiltin {
  using Self = rq::Binary32;

  inline explicit Binary32() : SimpleBuiltin(rq::Opcode::SY_BINARY32) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY32;
  }
};

template <> struct is_acquired<rq::Binary32> final : std::true_type {};

struct Binary64 final : public rq::SimpleBuiltin {
  using Self = rq::Binary64;

  inline explicit Binary64() : SimpleBuiltin(rq::Opcode::SY_BINARY64) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY64;
  }
};

template <> struct is_acquired<rq::Binary64> final : std::true_type {};

struct Binary128 final : public rq::SimpleBuiltin {
  using Self = rq::Binary128;

  inline explicit Binary128() : SimpleBuiltin(rq::Opcode::SY_BINARY128) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BINARY128;
  }
};

template <> struct is_acquired<rq::Binary128> final : std::true_type {};

struct Bfloat16 final : public rq::SimpleBuiltin {
  using Self = rq::Bfloat16;

  inline explicit Bfloat16() : SimpleBuiltin(rq::Opcode::SY_BFLOAT16) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_BFLOAT16;
  }
};

template <> struct is_acquired<rq::Bfloat16> final : std::true_type {};

struct IntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::IntegerConstraint;

  inline explicit IntegerConstraint()
      : SimpleBuiltin(rq::Opcode::SY_INTEGER_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_INTEGER_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::IntegerConstraint> final : std::true_type {};

struct SignedIntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::SignedIntegerConstraint;

  inline explicit SignedIntegerConstraint()
      : SimpleBuiltin(rq::Opcode::SY_SIGNED_INTEGER_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SIGNED_INTEGER_CONSTRAINT;
  }
};

template <>
struct is_acquired<rq::SignedIntegerConstraint> final : std::true_type {};

struct UnsignedIntegerConstraint final : public rq::SimpleBuiltin {
  using Self = rq::UnsignedIntegerConstraint;

  inline explicit UnsignedIntegerConstraint()
      : SimpleBuiltin(rq::Opcode::SY_UNSIGNED_INTEGER_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_UNSIGNED_INTEGER_CONSTRAINT;
  }
};

template <>
struct is_acquired<rq::UnsignedIntegerConstraint> final : std::true_type {};

struct CodeunitConstraint final : public rq::SimpleBuiltin {
  using Self = rq::CodeunitConstraint;

  inline explicit CodeunitConstraint()
      : SimpleBuiltin(rq::Opcode::SY_CODEUNIT_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_CODEUNIT_CONSTRAINT;
  }
};

template <>
struct is_acquired<rq::CodeunitConstraint> final : std::true_type {};

struct StringConstraint final : public rq::SimpleBuiltin {
  using Self = rq::StringConstraint;

  inline explicit StringConstraint()
      : SimpleBuiltin(rq::Opcode::SY_STRING_CONSTRAINT) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_STRING_CONSTRAINT;
  }
};

template <> struct is_acquired<rq::StringConstraint> final : std::true_type {};

struct Char final : public rq::SimpleBuiltin {
  using Self = rq::Char;

  inline explicit Char() : SimpleBuiltin(rq::Opcode::SY_CHAR) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CHAR;
  }
};

template <> struct is_acquired<rq::Char> final : std::true_type {};

struct Ascii final : public rq::SimpleBuiltin {
  using Self = rq::Ascii;

  inline explicit Ascii() : SimpleBuiltin(rq::Opcode::SY_ASCII) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ASCII;
  }
};

template <> struct is_acquired<rq::Ascii> final : std::true_type {};

struct Utf8 final : public rq::SimpleBuiltin {
  using Self = rq::Utf8;

  inline explicit Utf8() : SimpleBuiltin(rq::Opcode::SY_UTF8) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_UTF8;
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
getHasPlatformScalar(rq::ScaledBuiltinFlags flags) {
  return rq::getHasAll(flags, rq::ScaledBuiltinFlags::PLATFORM_SCALAR);
}

void RQ_ALWAYS_INLINE profileScaledBuiltin(llvm::FoldingSetNodeID &id,
                                           rq::Opcode opcode,
                                           std::uint16_t scalar,
                                           std::uint16_t uid,
                                           rq::ScaledBuiltinFlags flags) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddInteger(static_cast<unsigned>(scalar));
  id.AddInteger(static_cast<unsigned>(uid));
  id.AddInteger(static_cast<unsigned>(flags));
}

struct ScaledBuiltin : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ScaledBuiltin;

  std::uint16_t _scalar;
  std::uint16_t _uid;
  rq::ScaledBuiltinFlags _flags;

  inline explicit ScaledBuiltin(rq::Opcode opcode, std::uint16_t scalar,
                                std::uint16_t uid, rq::ScaledBuiltinFlags flags)
      : Symbol(opcode), _scalar(scalar), _uid(uid), _flags(flags) {}
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSynonym() const {
    return this->_uid != 0;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsScaledBuiltin(rq::dereferencePtr(entity).getOpcode());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileScaledBuiltin(id, this->_opcode, this->_scalar, this->_uid,
                             this->_flags);
  }
};

template <> struct is_acquired<rq::ScaledBuiltin> final : std::true_type {};

struct ScaledSignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledSignedInteger;

  inline explicit ScaledSignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                      rq::ScaledBuiltinFlags flags)
      : ScaledBuiltin(rq::Opcode::SY_SCALED_SIGNED_INTEGER, scalar, uid,
                      flags) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SCALED_SIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::ScaledSignedInteger> final : std::true_type {};

struct ScaledUnsignedInteger final : public rq::ScaledBuiltin {
  using Self = rq::ScaledUnsignedInteger;

  inline explicit ScaledUnsignedInteger(std::uint16_t scalar, std::uint16_t uid,
                                        rq::ScaledBuiltinFlags flags)
      : ScaledBuiltin(rq::Opcode::SY_SCALED_UNSIGNED_INTEGER, scalar, uid,
                      flags) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SCALED_UNSIGNED_INTEGER;
  }
};

template <>
struct is_acquired<rq::ScaledUnsignedInteger> final : std::true_type {};

void RQ_ALWAYS_INLINE profileUnarySubtype(llvm::FoldingSetNodeID &id,
                                          rq::Opcode opcode,
                                          const rq::TypeConstant &descendent) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddPointer(&descendent);
}

struct UnarySubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::UnarySubtype;

  rq::TypeConstant *_descendent_ptr;

  inline explicit UnarySubtype(rq::Opcode opcode, rq::TypeConstant &descendent)
      : Symbol(opcode), _descendent_ptr(&descendent) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getDescendent() const {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getDescendent() {
    return rq::dereferencePtr(this->_descendent_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsUnarySubtype(rq::dereferencePtr(entity).getOpcode());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileUnarySubtype(id, this->getOpcode(), this->getDescendent());
  }
};

template <> struct is_acquired<rq::UnarySubtype> final : std::true_type {};

struct Reference final : public rq::UnarySubtype {
  using Self = rq::Reference;

  inline explicit Reference(rq::TypeConstant &descendent)
      : UnarySubtype(rq::Opcode::SY_REFERENCE, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_REFERENCE;
  }
};

template <> struct is_acquired<rq::Reference> final : std::true_type {};

struct Pointer final : public rq::UnarySubtype {
  using Self = rq::Pointer;

  inline explicit Pointer(rq::TypeConstant &descendent)
      : UnarySubtype(rq::Opcode::SY_POINTER, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_POINTER;
  }
};

template <> struct is_acquired<rq::Pointer> final : std::true_type {};

struct FatPointer final : public rq::UnarySubtype {
  using Self = rq::FatPointer;

  inline explicit FatPointer(rq::TypeConstant &descendent)
      : UnarySubtype(rq::Opcode::SY_FAT_POINTER, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_FAT_POINTER;
  }
};

template <> struct is_acquired<rq::FatPointer> final : std::true_type {};

struct InferencedCountArray final : public rq::UnarySubtype {
  using Self = rq::InferencedCountArray;

  inline explicit InferencedCountArray(rq::TypeConstant &descendent)
      : UnarySubtype(rq::Opcode::SY_INFERENCED_COUNT_ARRAY, descendent) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_INFERENCED_COUNT_ARRAY;
  }
};

template <>
struct is_acquired<rq::InferencedCountArray> final : std::true_type {};

void RQ_ALWAYS_INLINE profileCountedSubtype(llvm::FoldingSetNodeID &id,
                                            rq::Opcode opcode,
                                            const rq::TypeConstant &descendent,
                                            unsigned count) {
  id.AddInteger(static_cast<unsigned>(opcode));
  id.AddPointer(&descendent);
  id.AddInteger(count);
}

struct CountedSubtype : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::CountedSubtype;

  rq::TypeConstant *_descendent_ptr;
  unsigned _count;

  inline explicit CountedSubtype(rq::Opcode opcode,
                                 rq::TypeConstant &descendent, unsigned count)
      : Symbol(opcode), _descendent_ptr(&descendent), _count(count) {}

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
    return rq::getIsCountedSubtype(rq::dereferencePtr(entity).getOpcode());
  }
  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileCountedSubtype(id, this->getOpcode(), this->getDescendent(),
                              this->getCount());
  }
};

template <> struct is_acquired<rq::CountedSubtype> final : std::true_type {};

struct Array final : public rq::CountedSubtype {
  using Self = rq::Array;

  inline explicit Array(rq::TypeConstant &descendent, unsigned count)
      : CountedSubtype(rq::Opcode::SY_ARRAY, descendent, count) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ARRAY;
  }
};

template <> struct is_acquired<rq::Array> final : std::true_type {};

struct ParameterListSubtype : public rq::Symbol,
                              public rq::InitialExpressionFlags,
                              public rq::InitialModuleMember,
                              public rq::SymbolTableHosted {
  using Self = rq::ParameterListSubtype;

  llvm::ArrayRef<rq::Parameter> _named_parameter_map{};
  rq::Parameter *_positional_parameter_list_begin_ptr{nullptr};
  unsigned _parameter_count{};

  inline explicit ParameterListSubtype(rq::BumpPtrAllocator &allocator,
                                       unsigned map_bucket_count,
                                       rq::Opcode opcode,
                                       rq::ExpressionFlags attributes,
                                       rq::Module &module,
                                       rq::SymbolTable &hosting_table)
      : Symbol(opcode), InitialExpressionFlags(attributes),
        InitialModuleMember(module), SymbolTableHosted(hosting_table),
        _named_parameter_map(
            allocator.allocateAcquiredZeroedArray<rq::Parameter>(
                map_bucket_count)) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsParameterListSubtype(
        rq::dereferencePtr(entity).getOpcode());
  }
};

template <>
struct is_parent_only<rq::ParameterListSubtype> final : std::true_type {};

struct Layout final : public rq::ParameterListSubtype {
  using Self = rq::Layout;

  inline explicit Layout(rq::BumpPtrAllocator &allocator,
                         unsigned parameter_bucket_count,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::SymbolTable &hosting_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::Opcode::SY_LAYOUT, attributes, module,
                             hosting_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LAYOUT;
  }
};

template <> struct is_acquired<rq::Layout> final : std::true_type {};

struct ClassLayout final : public rq::ParameterListSubtype {
  using Self = rq::ClassLayout;

  inline explicit ClassLayout(rq::BumpPtrAllocator &allocator,
                              unsigned parameter_bucket_count,
                              rq::ExpressionFlags attributes,
                              rq::Module &module,
                              rq::SymbolTable &hosting_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::Opcode::SY_CLASS_LAYOUT, attributes, module,
                             hosting_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_CLASS_LAYOUT;
  }
};

template <> struct is_acquired<rq::ClassLayout> final : std::true_type {};

struct TemplateLayout final : public rq::ParameterListSubtype {
  using Self = rq::TemplateLayout;

  inline explicit TemplateLayout(rq::BumpPtrAllocator &allocator,
                                 unsigned parameter_bucket_count,
                                 rq::Module &module,
                                 rq::SymbolTable &hosting_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::Opcode::SY_TEMPLATE_LAYOUT, {}, module,
                             hosting_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_LAYOUT;
  }
};

template <> struct is_acquired<rq::TemplateLayout> final : std::true_type {};

struct Signature final : public rq::ParameterListSubtype {
  using Self = rq::Signature;

  rq::TypeConstant *_return_type_ptr{nullptr};
  rq::TypeConstant *_extended_type_ptr{nullptr};

  inline explicit Signature(rq::BumpPtrAllocator &allocator,
                            unsigned parameter_bucket_count,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &hosting_table)
      : ParameterListSubtype(allocator, parameter_bucket_count,
                             rq::Opcode::SY_SIGNATURE, attributes, module,
                             hosting_table) {}
  RQ_ALWAYS_INLINE void setReturnType(rq::TypeConstant &type) {
    this->_return_type_ptr = &type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasReturnType() const {
    return this->_return_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getReturnType() const {
    return rq::dereferencePtr(this->_return_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getReturnType() {
    return rq::dereferencePtr(this->_return_type_ptr);
  }
  RQ_ALWAYS_INLINE void setExtendedType(rq::TypeConstant &type) {
    this->_extended_type_ptr = &type;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExtendedType() const {
    return this->_extended_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &
  getExtendedType() const {
    return rq::dereferencePtr(this->_extended_type_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getExtendedType() {
    return rq::dereferencePtr(this->_extended_type_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SIGNATURE;
  }
};

struct Parameter : public rq::Symbol,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::SymbolTableHosted,
                   public rq::InitialMaybeNamed {
  using Self = rq::Parameter;

  std::size_t _hash{0};
  rq::TypeConstant *_type_ptr{nullptr};
  rq::ParameterListSubtype *_parameter_list_subtype_ptr;
  Self *_left_ptr{nullptr};
  Self *_right_ptr{nullptr};
  Self *_next_ptr{nullptr};

  inline explicit Parameter(rq::Opcode opcode, llvm::StringRef name,
                            rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &hosting_table)
      : Symbol(opcode), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableHosted(hosting_table), InitialMaybeNamed(name),
        _parameter_list_subtype_ptr(&list) {}
  inline explicit Parameter(rq::Opcode opcode, rq::ParameterListSubtype &list,
                            rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &hosting_table)
      : Symbol(opcode), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableHosted(hosting_table), InitialMaybeNamed(),
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
    return rq::getIsParameter(rq::dereferencePtr(entity).getOpcode());
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
                                 rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_CLASS_PARAMETER, name, layout, expression,
                  attributes, module, hosting_table) {}
  inline explicit ClassParameter(rq::ClassLayout &layout,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_CLASS_PARAMETER, layout, expression,
                  attributes, module, hosting_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::ClassLayout &getClassLayout() const {
    return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ClassLayout &getClassLayout() {
    return llvm::cast<rq::ClassLayout>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_CLASS_PARAMETER;
  }
};

template <> struct is_acquired<rq::ClassParameter> final : std::true_type {};

struct LayoutParameter : public rq::Parameter {
  using Self = rq::LayoutParameter;

  inline explicit LayoutParameter(llvm::StringRef name, rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_LAYOUT_PARAMETER, name, layout, expression,
                  attributes, module, hosting_table) {}
  inline explicit LayoutParameter(rq::Layout &layout,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_LAYOUT_PARAMETER, layout, expression,
                  attributes, module, hosting_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Layout &getLayout() const {
    return llvm::cast<rq::Layout>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Layout &getLayout() {
    return llvm::cast<rq::Layout>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_LAYOUT_PARAMETER;
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
                                    rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_TEMPLATE_PARAMETER, name, template_layout,
                  expression, attributes, module, hosting_table) {}
  inline explicit TemplateParameter(rq::TemplateLayout &template_layout,
                                    rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_TEMPLATE_PARAMETER, template_layout,
                  expression, attributes, module, hosting_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Template &getTemplate() const {
    return llvm::cast<rq::Template>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Template &getTemplate() {
    return llvm::cast<rq::Template>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_PARAMETER;
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
                                     rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_SIGNATURE_PARAMETER, name, signature,
                  expression, attributes, module, hosting_table) {}
  inline explicit SignatureParameter(rq::Signature &signature,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::SymbolTable &hosting_table)
      : Parameter(rq::Opcode::SY_SIGNATURE_PARAMETER, signature, expression,
                  attributes, module, hosting_table) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Signature &getSignature() const {
    return llvm::cast<rq::Signature>(this->getParameterListSubtype());
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::Signature &getSignature() {
    return llvm::cast<rq::Signature>(this->getParameterListSubtype());
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_SIGNATURE_PARAMETER;
  }
};

template <>
struct is_acquired<rq::SignatureParameter> final : std::true_type {};

void RQ_ALWAYS_INLINE profileArithmeticSequence(
    llvm::FoldingSetNodeID &id, const rq::TypeConstant &descendent,
    rq::ArithmeticSequenceCondition condition,
    rq::ArithmeticSequenceStep step) {
  // no need to fold opcode
  id.AddPointer(&descendent);
  id.AddInteger(static_cast<unsigned>(condition));
  id.AddInteger(static_cast<unsigned>(step));
}

struct ArithmeticSequence : public rq::Symbol, public llvm::FoldingSetNode {
  using Self = rq::ArithmeticSequence;

  rq::TypeConstant *_descendent_ptr;
  rq::ArithmeticSequenceCondition _condition;
  rq::ArithmeticSequenceStep _step;

  inline explicit ArithmeticSequence(rq::Opcode opcode,
                                     rq::TypeConstant &descendent,
                                     rq::ArithmeticSequenceCondition condition,
                                     rq::ArithmeticSequenceStep step)
      : Symbol(opcode), _descendent_ptr(&descendent), _condition(condition),
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
    return rq::getIsArithmeticSequence(rq::dereferencePtr(entity).getOpcode());
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
      : ArithmeticSequence(rq::Opcode::SY_ARITHMETIC_INTERVAL, descendent,
                           condition, rq::ArithmeticSequenceStep::NONE) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_ARITHMETIC_INTERVAL;
  }
};

template <>
struct is_acquired<rq::ArithmeticInterval> final : std::true_type {};

struct FiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::FiniteArithmeticProgression;

  inline explicit FiniteArithmeticProgression(
      rq::TypeConstant &descendent, rq::ArithmeticSequenceCondition condition,
      rq::ArithmeticSequenceStep step)
      : ArithmeticSequence(rq::Opcode::SY_FINITE_ARITHMETIC_PROGRESSION,
                           descendent, condition, step) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
};

template <>
struct is_acquired<rq::FiniteArithmeticProgression> final : std::true_type {};

struct InfiniteArithmeticProgression : public rq::ArithmeticSequence {
  using Self = rq::InfiniteArithmeticProgression;

  inline explicit InfiniteArithmeticProgression(rq::TypeConstant &descendent,
                                                rq::ArithmeticSequenceStep step)
      : ArithmeticSequence(rq::Opcode::SY_INFINITE_ARITHMETIC_PROGRESSION,
                           descendent, rq::ArithmeticSequenceCondition::NONE,
                           step) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
};

template <>
struct is_acquired<rq::InfiniteArithmeticProgression> final : std::true_type {};

static constexpr llvm::StringRef REQUITE_EXTENSION = ".rq";

enum class ModuleKind : std::uint_fast8_t { NONE, SOURCE, IMPORT };

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName(rq::ModuleKind opcode) {
  using M = rq::ModuleKind;
  switch (opcode) {
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
  rq::ModuleKind _module_opcode;
  llvm::StringRef _path;
  llvm::MemoryBufferRef _buffer;

  inline explicit Module(rq::ModuleKind opcode, llvm::StringRef path,
                         llvm::MemoryBufferRef &&buffer)
      : Symbol(rq::Opcode::SY_MODULE), _module_opcode(opcode), _path(path),
        _buffer(buffer) {}

  [[nodiscard]] RQ_ALWAYS_INLINE rq::ModuleKind getModuleKind() const {
    return this->_module_opcode;
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
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_MODULE;
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
      : Symbol(rq::Opcode::SY_IMPORT), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_IMPORT;
  }
};

struct Code : public rq::Symbol,
              public rq::InitialExpression,
              public rq::InitialExpressionFlags,
              public rq::InitialModuleMember,
              public rq::SymbolTableMember,
              public rq::SymbolTableHosted,
              public rq::InitialNamed {
  using Self = rq::Code;

  inline explicit Code(llvm::StringRef name, const rq::Expression &expression,
                       rq::ExpressionFlags attributes, rq::Module &module,
                       rq::SymbolTable &containing_table,
                       rq::SymbolTable &hosting_table)
      : Symbol(rq::Opcode::SY_CODE), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CODE;
  }
};

struct CategoryDiscriminant : public rq::Symbol {
  using Self = rq::CategoryDiscriminant;

  rq::Category *_category_ptr;

  inline explicit CategoryDiscriminant(rq::Category &category)
      : Symbol(rq::Opcode::SY_CATEGORY_DISCRIMINANT), _category_ptr(&category) {
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_CATEGORY_DISCRIMINANT;
  }
};

template <>
struct is_acquired<rq::CategoryDiscriminant> final : std::true_type {};

struct Label : public rq::Symbol,
               public rq::InitialExpression,
               public rq::InitialModuleMember,
               public rq::SymbolTableMember,
               public rq::InitialNamed {
  using Self = rq::Label;

  const rq::Expression *_ascription_ptr;
  rq::Entity *_subject_ptr;

  inline explicit Label(llvm::StringRef name, rq::Expression &expression,
                        const rq::Expression &ascription, rq::Entity &subject,
                        rq::Module &module, rq::SymbolTable &containing_table)
      : Symbol(rq::Opcode::SY_LABEL), InitialExpression(expression),
        InitialModuleMember(module), SymbolTableMember(containing_table),
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
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_LABEL;
  }
};

struct Synonym : public rq::Symbol {
  using Self = rq::Synonym;

  rq::Symbol *_original_ptr;

  inline explicit Synonym(rq::Symbol &original)
      : Symbol(rq::Opcode::SY_SYNONYM), _original_ptr(&original) {
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
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SYNONYM;
  }
};

template <> struct is_acquired<rq::Synonym> final : std::true_type {};

template <>
struct is_acquired<rq::CategoryAlternative> final : std::true_type {};

struct SymbolTableIterator final {
  using Self = rq::SymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = rq::SymbolTable &;
  using pointer = rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTable *_table_ptr{nullptr};

  SymbolTableIterator() = default;
  explicit SymbolTableIterator(rq::SymbolTable *table_ptr)
      : _table_ptr(table_ptr) {}
  SymbolTableIterator(const Self &) = default;
  SymbolTableIterator(Self &&) = default;
  ~SymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_table_ptr == it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_table_ptr != it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &operator*() {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *operator->() {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_table_ptr == nullptr;
  }
};

struct ConstSymbolTableIterator final {
  using Self = rq::ConstSymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = const rq::SymbolTable &;
  using pointer = const rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTable *_table_ptr = nullptr;

  ConstSymbolTableIterator() = default;
  explicit ConstSymbolTableIterator(const rq::SymbolTable *table_ptr)
      : _table_ptr(table_ptr) {}
  ConstSymbolTableIterator(const Self &) = default;
  ConstSymbolTableIterator(Self &&) = default;
  ~ConstSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_table_ptr == it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_table_ptr != it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_table_ptr == nullptr;
  }
};

struct MemberSymbolTableIterator final {
  using Self = rq::MemberSymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = rq::SymbolTable &;
  using pointer = rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::SymbolTable *_table_ptr{nullptr};

  MemberSymbolTableIterator() = default;
  explicit MemberSymbolTableIterator(rq::SymbolTable *table_ptr)
      : _table_ptr(table_ptr) {}
  MemberSymbolTableIterator(const Self &) = default;
  MemberSymbolTableIterator(Self &&) = default;
  ~MemberSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_table_ptr == it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_table_ptr != it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable &operator*() {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::SymbolTable *operator->() {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_table_ptr == nullptr;
  }
};

struct ConstMemberSymbolTableIterator final {
  using Self = rq::ConstMemberSymbolTableIterator;
  using value_type = rq::SymbolTable;
  using reference = const rq::SymbolTable &;
  using pointer = const rq::SymbolTable *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::SymbolTable *_table_ptr = nullptr;

  ConstMemberSymbolTableIterator() = default;
  explicit ConstMemberSymbolTableIterator(const rq::SymbolTable *table_ptr)
      : _table_ptr(table_ptr) {}
  ConstMemberSymbolTableIterator(const Self &) = default;
  ConstMemberSymbolTableIterator(Self &&) = default;
  ~ConstMemberSymbolTableIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_table_ptr == it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_table_ptr != it._table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable &operator*() const {
    return rq::dereferencePtr(this->_table_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::SymbolTable *operator->() const {
    return this->_table_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_table_ptr == nullptr;
  }
};

struct SymbolTable : public rq::Symbol, public rq::SymbolTableMember {
  using Self = rq::SymbolTable;

  rq::SymbolTable *_next_table_ptr{nullptr};
  rq::SymbolTable *_first_member_table_ptr{nullptr};
  llvm::DenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_symbols_map{};
  rq::BumpPtrList<rq::Symbol> _unamed_symbols_list{};

  inline explicit SymbolTable(rq::Opcode opcode) : Symbol(opcode) {}
  inline explicit SymbolTable(rq::Opcode opcode,
                              rq::SymbolTable &containing_table)
      : Symbol(opcode), SymbolTableMember(containing_table) {}
  inline void release();
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ConstBumpPtrListRef<rq::Symbol>
  getUnamedSymbolsListRef() const {
    return this->_unamed_symbols_list;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::BumpPtrListRef<rq::Symbol>
  getUnamedSymbolsListRef() {
    return this->_unamed_symbols_list;
  }
  inline void _addMember(rq::Symbol &symbol);
  inline void addNamedSymbol(rq::BumpPtrAllocator &allocator,
                             llvm::StringRef name, rq::Symbol &symbol) {
    rq::BumpPtrList<rq::Symbol> &list = this->_named_symbols_map[name];
    list.insertFront(allocator, symbol);
    this->_addMember(symbol);
  }
  inline void addUnamedSymbol(rq::BumpPtrAllocator &allocator,
                              rq::Symbol &symbol) {
    this->_unamed_symbols_list.insertFront(allocator, symbol);
    this->_addMember(symbol);
  }
  [[nodiscard]] rq::ConstBumpPtrListRef<rq::Symbol>
  getNamedListRef(llvm::StringRef name) const {
    auto it = this->_named_symbols_map.find(name);
    if (it == this->_named_symbols_map.end()) {
      return rq::ConstBumpPtrListRef<rq::Symbol>();
    }
    return it->getSecond();
  }
  [[nodiscard]] rq::BumpPtrListRef<rq::Symbol>
  getNamedListRef(llvm::StringRef name) {
    auto it = this->_named_symbols_map.find(name);
    if (it == this->_named_symbols_map.end()) {
      return rq::BumpPtrListRef<rq::Symbol>();
    }
    return it->getSecond();
  }
  auto getNamedListsSubrange() const {
    auto begin_it = this->_named_symbols_map.begin();
    return std::ranges::subrange<decltype(begin_it), decltype(begin_it),
                                 std::ranges::subrange_kind::unsized>(
        begin_it, this->_named_symbols_map.end());
  }
  std::ranges::subrange<
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
      std::ranges::subrange_kind::unsized>
  getNamedListsSubrange() {
    return std::ranges::subrange<
        llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
        llvm::DenseMapIterator<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>,
        std::ranges::subrange_kind::unsized>(this->_named_symbols_map.begin(),
                                             this->_named_symbols_map.end());
  }
  [[nodiscard]] inline std::ranges::subrange<
      rq::ConstSymbolTableIterator, rq::ConstSymbolTableIterator,
      std::ranges::subrange_kind::unsized>
  getInclusiveFrameSubrange() const {
    return std::ranges::subrange<rq::ConstSymbolTableIterator,
                                 rq::ConstSymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::ConstSymbolTableIterator(this), rq::ConstSymbolTableIterator());
  }
  std::ranges::subrange<rq::SymbolTableIterator, rq::SymbolTableIterator,
                        std::ranges::subrange_kind::unsized>
  getInclusiveFrameSubrange() {
    return std::ranges::subrange<rq::SymbolTableIterator,
                                 rq::SymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::SymbolTableIterator(this), rq::SymbolTableIterator());
  }
  std::ranges::subrange<rq::MemberSymbolTableIterator,
                        rq::MemberSymbolTableIterator,
                        std::ranges::subrange_kind::unsized>
  getMemberSymbolTableSubrange() {
    return std::ranges::subrange<rq::MemberSymbolTableIterator,
                                 rq::MemberSymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::MemberSymbolTableIterator(this->_first_member_table_ptr),
        rq::MemberSymbolTableIterator());
  }
  std::ranges::subrange<rq::ConstMemberSymbolTableIterator,
                        rq::ConstMemberSymbolTableIterator,
                        std::ranges::subrange_kind::unsized>
  getMemberSymbolTableSubrange() const {
    return std::ranges::subrange<rq::ConstMemberSymbolTableIterator,
                                 rq::ConstMemberSymbolTableIterator,
                                 std::ranges::subrange_kind::unsized>(
        rq::ConstMemberSymbolTableIterator(this->_first_member_table_ptr),
        rq::ConstMemberSymbolTableIterator());
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsSymbolTable(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::SymbolTable> final : std::true_type {};

RQ_ALWAYS_INLINE rq::SymbolTableIterator &SymbolTableIterator::operator++() {
  this->_table_ptr =
      rq::dereferencePtr(this->_table_ptr)._containing_symbol_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::SymbolTableIterator SymbolTableIterator::operator++(int) {
  rq::SymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator &
ConstSymbolTableIterator::operator++() {
  this->_table_ptr =
      rq::dereferencePtr(this->_table_ptr)._containing_symbol_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstSymbolTableIterator
ConstSymbolTableIterator::operator++(int) {
  rq::ConstSymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

RQ_ALWAYS_INLINE rq::MemberSymbolTableIterator &
MemberSymbolTableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._next_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::MemberSymbolTableIterator
MemberSymbolTableIterator::operator++(int) {
  rq::MemberSymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

RQ_ALWAYS_INLINE rq::ConstMemberSymbolTableIterator &
ConstMemberSymbolTableIterator::operator++() {
  this->_table_ptr = rq::dereferencePtr(this->_table_ptr)._next_table_ptr;
  return *this;
}

RQ_ALWAYS_INLINE rq::ConstMemberSymbolTableIterator
ConstMemberSymbolTableIterator::operator++(int) {
  rq::ConstMemberSymbolTableIterator temp = *this;
  ++(*this);
  return temp;
}

inline void SymbolTable::_addMember(rq::Symbol &symbol) {
  if (llvm::isa<rq::SymbolTable>(symbol)) {
    rq::SymbolTable &symbol_table = llvm::cast<rq::SymbolTable>(symbol);
    symbol_table._next_table_ptr = this->_first_member_table_ptr;
    this->_first_member_table_ptr = &symbol_table;
  }
}

inline void SymbolTable::release() {
  this->_named_symbols_map.clear();
  for (rq::SymbolTable &member : this->getMemberSymbolTableSubrange()) {
    member.release();
  }
}

struct Top : public rq::SymbolTable {
  using Self = rq::Top;

  inline explicit Top() : SymbolTable(rq::Opcode::SY_TOP) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_TOP;
  }
};

struct Scope : public rq::SymbolTable,
               rq::InitialExpression,
               rq::InitialModuleMember {
  using Self = rq::Scope;

  inline explicit Scope(rq::Expression &expression, rq::Module &module,
                        rq::SymbolTable &containing_table)
      : SymbolTable(rq::Opcode::SY_SCOPE, containing_table),
        InitialExpression(expression), InitialModuleMember(module) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_SCOPE;
  }
};

struct Namespace : public rq::SymbolTable, rq::InitialNamed {
  using Self = rq::Namespace;

  inline explicit Namespace(llvm::StringRef name,

                            rq::SymbolTable &containing_table)
      : SymbolTable(rq::Opcode::SY_NAMESPACE, containing_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_NAMESPACE;
  }
};

struct Class : public rq::SymbolTable,
               public rq::InitialExpression,
               public rq::InitialExpressionFlags,
               public rq::InitialModuleMember,
               public rq::SymbolTableHosted,
               public rq::InitialNamed {
  using Self = rq::Class;

  bool _is_implemented : 1 {false};
  const rq::Expression *_class_layout_expression_ptr{nullptr};
  rq::ClassLayout *_class_layout_ptr{nullptr};

  inline explicit Class(llvm::StringRef name, const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module,
                        rq::SymbolTable &containing_table,
                        rq::SymbolTable &hosting_table)
      : SymbolTable(rq::Opcode::SY_CLASS, containing_table),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const {
    return this->_is_implemented;
  }
  RQ_ALWAYS_INLINE void setIsImplemented() {
    RQ_ASSERT(!this->_is_implemented, "already implemented");
    this->_is_implemented = true;
  }
  void setClassLayoutExpression(const rq::Expression &layout_expression) {
    rq::assignSingleValue(this->_class_layout_expression_ptr,
                          &layout_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CLASS;
  }
};

struct Enumeration : public rq::SymbolTable,
                     public rq::InitialExpression,
                     public rq::InitialExpressionFlags,
                     public rq::InitialModuleMember,
                     public rq::SymbolTableHosted,
                     public rq::InitialNamed {
  using Self = rq::Enumeration;

  bool _is_implemented : 1 {false};
  const rq::Expression *_underlying_type_expression_ptr{nullptr};

  inline explicit Enumeration(llvm::StringRef name,
                              const rq::Expression &expression,
                              rq::ExpressionFlags attributes,
                              rq::Module &module,
                              rq::SymbolTable &containing_table,
                              rq::SymbolTable &hosting_table)
      : SymbolTable(rq::Opcode::SY_ENUMERATION, containing_table),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const {
    return this->_is_implemented;
  }
  RQ_ALWAYS_INLINE void setIsImplemented() {
    RQ_ASSERT(!this->_is_implemented, "already implemented");
    this->_is_implemented = true;
  }
  RQ_ALWAYS_INLINE void setUnderlyingTypeExpression(
      const rq::Expression &underlying_type_expression) {
    rq::assignSingleValue(this->_underlying_type_expression_ptr,
                          &underlying_type_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENUMERATION;
  }
};

struct Category : public rq::SymbolTable,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::SymbolTableHosted,
                  public rq::InitialNamed {
  using Self = rq::Category;

  bool _is_implemented : 1 {false};
  const rq::Expression *_discriminant_type_expression_ptr{nullptr};

  inline explicit Category(llvm::StringRef name,

                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::SymbolTable &containing_table,
                           rq::SymbolTable &hosting_table)
      : SymbolTable(rq::Opcode::SY_CATEGORY, containing_table),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const {
    return this->_is_implemented;
  }
  RQ_ALWAYS_INLINE void setIsImplemented() {
    RQ_ASSERT(!this->_is_implemented, "already implemented");
    this->_is_implemented = true;
  }
  RQ_ALWAYS_INLINE void
  setDiscriminantTypeExpression(const rq::Expression &type_expression) {
    rq::assignSingleValue(this->_discriminant_type_expression_ptr,
                          &type_expression);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_CATEGORY;
  }
};

struct DynamicVariable : public rq::Symbol,
                         public rq::InitialExpression,
                         public rq::InitialExpressionFlags,
                         public rq::InitialModuleMember,
                         public rq::SymbolTableMember,
                         public rq::InitialNamed {
  using Self = rq::DynamicVariable;

  rq::TypeConstant *_type_ptr{nullptr};
  const rq::Expression *_type_expression_ptr{nullptr};
  const rq::Expression *_value_expression_ptr{nullptr};

  inline explicit DynamicVariable(rq::Opcode opcode, llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &containing_table)
      : Symbol(opcode), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), InitialNamed(name) {
    RQ_ASSERT(rq::getIsDynamicVariable(opcode), "not dynamic variable");
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasType() const {
    return this->_type_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getType() const {
    return rq::dereferencePtr(this->_type_ptr);
  }
  RQ_ALWAYS_INLINE void setTypeExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_type_expression_ptr, &expression);
  }
  RQ_ALWAYS_INLINE void
  setValueExpression(const rq::Expression *expression_ptr) {
    rq::assignSingleValue(this->_value_expression_ptr, expression_ptr);
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsDynamicVariable(rq::dereferencePtr(entity).getOpcode());
  }
};

template <>
struct is_parent_only<rq::DynamicVariable> final : std::true_type {};

struct LocalVariable : public rq::DynamicVariable {
  using Self = rq::LocalVariable;

  bool _is_indeterminate : 1 {true};

  inline explicit LocalVariable(llvm::StringRef name,
                                const rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &containing_table)
      : DynamicVariable(rq::Opcode::SY_LOCAL_VARIABLE, name, expression,
                        attributes, module, containing_table) {}
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_LOCAL_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIndeterminate() const {
    return this->_is_indeterminate;
  }
  RQ_ALWAYS_INLINE void setNotIndeterminate() {
    RQ_ASSERT(this->getHasType(), "does not have type");
    //RQ_ASSERT(this->getType().getIsComplete(), "does not have complete type");
    this->_is_indeterminate = false;
  }
};

struct GlobalVariable : public rq::DynamicVariable,
                        public rq::SymbolTableHosted {
  using Self = rq::GlobalVariable;

  bool _is_implemented : 1 {false};
  rq::TypeConstant *_type_ptr;

  inline explicit GlobalVariable(llvm::StringRef name,
                                 const rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table)
      : DynamicVariable(rq::Opcode::SY_GLOBAL_VARIABLE, name, expression,
                        attributes, module, containing_table),
        SymbolTableHosted(hosting_table) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() const {
    return this->_is_implemented;
  }
  RQ_ALWAYS_INLINE void setIsImplemented() {
    RQ_ASSERT(!this->_is_implemented, "already implemented");
    this->_is_implemented = true;
  }
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
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_GLOBAL_VARIABLE;
  }
};

struct StaticVariable : public rq::Symbol,
                        public rq::InitialExpression,
                        public rq::InitialExpressionFlags,
                        public rq::InitialModuleMember,
                        public rq::SymbolTableMember,
                        public rq::SymbolTableHosted,
                        public rq::InitialNamed {
  using Self = rq::StaticVariable;

  inline explicit StaticVariable(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table)
      : Symbol(rq::Opcode::SY_STATIC_VARIABLE), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_STATIC_VARIABLE;
  }
};

template <> struct is_acquired<rq::StaticVariable> final : std::true_type {};

struct Enumerator : public rq::Symbol,
                    public rq::InitialExpression,
                    public rq::InitialExpressionFlags,
                    public rq::InitialModuleMember,
                    public rq::SymbolTableMember,
                    public rq::SymbolTableHosted,
                    public rq::InitialNamed {
  using Self = rq::Enumerator;

  inline explicit Enumerator(llvm::StringRef name, rq::Expression &expression,
                             rq::ExpressionFlags attributes, rq::Module &module,
                             rq::SymbolTable &containing_table,
                             rq::SymbolTable &hosting_table)
      : Symbol(rq::Opcode::SY_ENUMERATOR), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENUMERATOR;
  }
};

template <> struct is_acquired<rq::Enumerator> final : std::true_type {};

struct CategoryAlternative : public rq::Symbol,
                             public rq::InitialExpression,
                             public rq::InitialExpressionFlags,
                             public rq::InitialModuleMember,
                             public rq::SymbolTableMember {
  using Self = rq::CategoryAlternative;

  rq::Code *_code_ptr;

  inline explicit CategoryAlternative(rq::Code &code, rq::Category &category,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module)
      : Symbol(rq::Opcode::SY_CATEGORY_ALTERNATIVE),
        InitialExpression(expression), InitialExpressionFlags(attributes),
        InitialModuleMember(module), SymbolTableMember(category),
        _code_ptr(&code) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_CATEGORY_ALTERNATIVE;
  }
};

struct Procedure : public rq::SymbolTable,
                   public rq::InitialExpression,
                   public rq::InitialExpressionFlags,
                   public rq::InitialModuleMember,
                   public rq::SymbolTableHosted,
                   public rq::InitialMaybeNamed {
  using Self = rq::Procedure;

  bool _is_implemented : 1 {false};
  rq::TypeConstant *_signature_ptr{nullptr};
  const rq::Expression *_signature_expression_ptr{nullptr};
  const rq::Expression *_body_start_ptr{nullptr};
  rq::Instruction *_instruction_ptr{nullptr};
  llvm::StringRef _mangled_name{};
  llvm::Function *_llvm_function_ptr{nullptr};

  inline explicit Procedure(rq::Opcode opcode, llvm::StringRef name,

                            const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &containing_table,
                            rq::SymbolTable &hosting_table)
      : SymbolTable(opcode, containing_table), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableHosted(hosting_table), InitialMaybeNamed(name) {}
  inline explicit Procedure(rq::Opcode opcode, const rq::Expression &expression,
                            rq::ExpressionFlags attributes, rq::Module &module,
                            rq::SymbolTable &containing_table,
                            rq::SymbolTable &hosting_table)
      : SymbolTable(opcode, containing_table), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableHosted(hosting_table) {}
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImplemented() {
    return this->_is_implemented;
  }
  RQ_ALWAYS_INLINE void setIsImplemented() {
    RQ_ASSERT(!this->_is_implemented, "already implemented");
    this->_is_implemented = true;
  }
  RQ_ALWAYS_INLINE void
  setSignatureExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_signature_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSignature() const {
    return this->_signature_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setSignature(rq::TypeConstant &signature) {
    this->_signature_ptr = &signature;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TypeConstant &getSignature() const {
    return rq::dereferencePtr(this->_signature_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeConstant &getSignature() {
    return rq::dereferencePtr(this->_signature_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasInstruction() const {
    return this->_instruction_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction *instruction_ptr) {
    rq::assignSingleValue(this->_instruction_ptr, instruction_ptr);
  }
  RQ_ALWAYS_INLINE void setInstruction(rq::Instruction &instruction) {
    rq::assignSingleValue(this->_instruction_ptr, &instruction);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Instruction &getInstruction() const {
    return rq::dereferencePtr(this->_instruction_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Instruction &getInstruction() {
    return rq::dereferencePtr(this->_instruction_ptr);
  }
  RQ_ALWAYS_INLINE bool getHasSignatureExpression() const {
    return this->_signature_expression_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getSignatureExpression() const {
    return rq::dereferencePtr(this->_signature_expression_ptr);
  }
  RQ_ALWAYS_INLINE bool getHasBodyStartExpression() const {
    return this->_body_start_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void
  setBodyStartExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_body_start_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &
  getBodyStartExpression() const {
    return rq::dereferencePtr(this->_body_start_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasMangledName() const {
    return !this->_mangled_name.empty();
  }
  RQ_ALWAYS_INLINE void setMangledName(llvm::StringRef name) {
    RQ_ASSERT(!this->getHasMangledName(), "mangled name already set");
    this->_mangled_name = name;
  }
  [[nodiscard]] llvm::StringRef getMangledName() const {
    return this->_mangled_name;
  }
  RQ_ALWAYS_INLINE void setLlvmFunctionPtr(llvm::Function *llvm_function_ptr) {
    rq::assignSingleValue(this->_llvm_function_ptr, llvm_function_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::Function *
  getLlvmFunctionPtr() const {
    return this->_llvm_function_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::Function *getLlvmFunctionPtr() {
    return this->_llvm_function_ptr;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsProcedure(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::Procedure> final : std::true_type {};

struct Entry : public rq::Procedure {
  using Self = rq::Entry;

  inline explicit Entry(const rq::Expression &expression,
                        rq::ExpressionFlags attributes, rq::Module &module,
                        rq::SymbolTable &containing_table,
                        rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_ENTRY, expression, attributes, module,
                  containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_ENTRY;
  }
};

struct Function : public rq::Procedure {
  using Self = rq::Function;

  inline explicit Function(llvm::StringRef name,
                           const rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::SymbolTable &containing_table,
                           rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_FUNCTION, name, expression, attributes, module,
                  containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_FUNCTION;
  }
};

struct Method : public rq::Procedure {
  using Self = rq::Method;

  inline explicit Method(llvm::StringRef name, const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::SymbolTable &containing_table,
                         rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_METHOD, name, expression, attributes, module,
                  containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_METHOD;
  }
};

struct Ranger : public rq::Procedure {
  using Self = rq::Ranger;

  inline explicit Ranger(llvm::StringRef name, const rq::Expression &expression,
                         rq::ExpressionFlags attributes, rq::Module &module,
                         rq::SymbolTable &containing_table,
                         rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_METHOD, name, expression, attributes, module,
                  containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::SY_RANGER;
  }
};

struct ExtensionFunction : public rq::Procedure {
  using Self = rq::ExtensionFunction;

  inline explicit ExtensionFunction(llvm::StringRef name,
                                    const rq::Expression &expression,
                                    rq::ExpressionFlags attributes,
                                    rq::Module &module,
                                    rq::SymbolTable &containing_table,
                                    rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_EXTENSION_FUNCTION, name, expression,
                  attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_EXTENSION_FUNCTION;
  }
};

struct ExtensionMethod : public rq::Procedure {
  using Self = rq::ExtensionMethod;

  inline explicit ExtensionMethod(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &containing_table,
                                  rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_EXTENSION_METHOD, name, expression, attributes,
                  module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_EXTENSION_METHOD;
  }
};

struct ExtensionRanger : public rq::Procedure {
  using Self = rq::ExtensionRanger;

  inline explicit ExtensionRanger(llvm::StringRef name,
                                  const rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &containing_table,
                                  rq::SymbolTable &hosting_table)
      : Procedure(rq::Opcode::SY_EXTENSION_RANGER, name, expression, attributes,
                  module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_EXTENSION_RANGER;
  }
};

struct Template : public rq::Symbol,
                  public rq::InitialExpression,
                  public rq::InitialExpressionFlags,
                  public rq::InitialModuleMember,
                  public rq::SymbolTableMember,
                  public rq::SymbolTableHosted,
                  public rq::InitialNamed {
  using Self = rq::Template;

  rq::TemplateLayout *_template_layout_ptr;

  inline explicit Template(rq::Opcode opcode, llvm::StringRef name,
                           rq::Expression &expression,
                           rq::ExpressionFlags attributes, rq::Module &module,
                           rq::SymbolTable &containing_table,
                           rq::SymbolTable &hosting_table,
                           rq::TemplateLayout &template_layout)
      : Symbol(opcode), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), SymbolTableHosted(hosting_table),
        InitialNamed(name), _template_layout_ptr(&template_layout) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::TemplateLayout &
  getTemplateLayout() const {
    return rq::dereferencePtr(this->_template_layout_ptr);
  }

  [[nodiscard]] RQ_ALWAYS_INLINE rq::TemplateLayout &getTemplateLayout() {
    return rq::dereferencePtr(this->_template_layout_ptr);
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsTemplate(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::Template> final : std::true_type {};

struct TemplateClass : public rq::Template {
  using Self = rq::TemplateClass;

  inline explicit TemplateClass(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &containing_table,
                                rq::SymbolTable &hosting_table,
                                rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_CLASS, name, expression, attributes,
                 module, containing_table, hosting_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_CLASS;
  }
};

struct TemplateEnumeration : public rq::Template {
  using Self = rq::TemplateEnumeration;

  inline explicit TemplateEnumeration(llvm::StringRef name,
                                      rq::Expression &expression,
                                      rq::ExpressionFlags attributes,
                                      rq::Module &module,
                                      rq::SymbolTable &containing_table,
                                      rq::SymbolTable &hosting_table,
                                      rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_ENUMERATION, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_ENUMERATION;
  }
};

struct TemplateCategory : public rq::Template {
  using Self = rq::TemplateCategory;

  inline explicit TemplateCategory(llvm::StringRef name,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::SymbolTable &containing_table,
                                   rq::SymbolTable &hosting_table,
                                   rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_CATEGORY, name, expression, attributes,
                 module, containing_table, hosting_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_CATEGORY;
  }
};

struct TemplateGlobalVariable : public rq::Template {
  using Self = rq::TemplateGlobalVariable;

  inline explicit TemplateGlobalVariable(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &containing_table,
                                         rq::SymbolTable &hosting_table,
                                         rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_GLOBAL_VARIABLE, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_GLOBAL_VARIABLE;
  }
};

struct TemplateStaticVariable : public rq::Template {
  using Self = rq::TemplateStaticVariable;

  inline explicit TemplateStaticVariable(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &containing_table,
                                         rq::SymbolTable &hosting_table,
                                         rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_STATIC_VARIABLE, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_STATIC_VARIABLE;
  }
};

struct TemplateFunction : public rq::Template {
  using Self = rq::TemplateFunction;

  inline explicit TemplateFunction(llvm::StringRef name,
                                   rq::Expression &expression,
                                   rq::ExpressionFlags attributes,
                                   rq::Module &module,
                                   rq::SymbolTable &containing_table,
                                   rq::SymbolTable &hosting_table,
                                   rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_FUNCTION, name, expression, attributes,
                 module, containing_table, hosting_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_FUNCTION;
  }
};

struct TemplateMethod : public rq::Template {
  using Self = rq::TemplateMethod;

  inline explicit TemplateMethod(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table,
                                 rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_METHOD, name, expression, attributes,
                 module, containing_table, hosting_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_METHOD;
  }
};

struct TemplateRanger : public rq::Template {
  using Self = rq::TemplateRanger;

  inline explicit TemplateRanger(llvm::StringRef name,
                                 rq::Expression &expression,
                                 rq::ExpressionFlags attributes,
                                 rq::Module &module,
                                 rq::SymbolTable &containing_table,
                                 rq::SymbolTable &hosting_table,
                                 rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_RANGER, name, expression, attributes,
                 module, containing_table, hosting_table, template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_RANGER;
  }
};

struct TemplateExtensionFunction : public rq::Template {
  using Self = rq::TemplateExtensionFunction;

  inline explicit TemplateExtensionFunction(llvm::StringRef name,
                                            rq::Expression &expression,
                                            rq::ExpressionFlags attributes,
                                            rq::Module &module,
                                            rq::SymbolTable &containing_table,
                                            rq::SymbolTable &hosting_table,
                                            rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_EXTENSION_FUNCTION, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
};

struct TemplateExtensionMethod : public rq::Template {
  using Self = rq::TemplateExtensionMethod;

  inline explicit TemplateExtensionMethod(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::SymbolTable &containing_table,
                                          rq::SymbolTable &hosting_table,
                                          rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_EXTENSION_METHOD, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_EXTENSION_METHOD;
  }
};

struct TemplateExtensionRanger : public rq::Template {
  using Self = rq::TemplateExtensionRanger;

  inline explicit TemplateExtensionRanger(llvm::StringRef name,
                                          rq::Expression &expression,
                                          rq::ExpressionFlags attributes,
                                          rq::Module &module,
                                          rq::SymbolTable &containing_table,
                                          rq::SymbolTable &hosting_table,
                                          rq::TemplateLayout &template_layout)
      : Template(rq::Opcode::SY_TEMPLATE_EXTENSION_RANGER, name, expression,
                 attributes, module, containing_table, hosting_table,
                 template_layout) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_TEMPLATE_EXTENSION_RANGER;
  }
};

struct Partial : public rq::Symbol,
                 public rq::InitialExpression,
                 public rq::InitialExpressionFlags,
                 public rq::InitialModuleMember,
                 public rq::SymbolTableMember,
                 public rq::SymbolTableHosted,
                 public rq::InitialNamed {
  using Self = rq::Partial;

  inline explicit Partial(rq::Opcode opcode, llvm::StringRef name,
                          rq::Expression &expression,
                          rq::ExpressionFlags attributes, rq::Module &module,
                          rq::SymbolTable &containing_table,
                          rq::SymbolTable &hosting_table)
      : Symbol(opcode), InitialExpression(expression),
        InitialExpressionFlags(attributes), InitialModuleMember(module),
        SymbolTableMember(containing_table), SymbolTableHosted(hosting_table),
        InitialNamed(name) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsPartial(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::Partial> final : std::true_type {};

struct PartialClass : public rq::Partial {
  using Self = rq::PartialClass;

  inline explicit PartialClass(llvm::StringRef name, rq::Expression &expression,
                               rq::ExpressionFlags attributes,
                               rq::Module &module,
                               rq::SymbolTable &containing_table,
                               rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_CLASS, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_CLASS;
  }
};

struct PartialEnumeration : public rq::Partial {
  using Self = rq::PartialEnumeration;

  inline explicit PartialEnumeration(llvm::StringRef name,
                                     rq::Expression &expression,
                                     rq::ExpressionFlags attributes,
                                     rq::Module &module,
                                     rq::SymbolTable &containing_table,
                                     rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_ENUMERATION, name, expression,
                attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_ENUMERATION;
  }
};

struct PartialCategory : public rq::Partial {
  using Self = rq::PartialCategory;

  inline explicit PartialCategory(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &containing_table,
                                  rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_CATEGORY, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_CATEGORY;
  }
};

struct PartialGlobalVariable : public rq::Partial {
  using Self = rq::PartialGlobalVariable;

  inline explicit PartialGlobalVariable(llvm::StringRef name,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::SymbolTable &containing_table,
                                        rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_GLOBAL_VARIABLE, name, expression,
                attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_GLOBAL_VARIABLE;
  }
};

struct PartialStaticVariable : public rq::Partial {
  using Self = rq::PartialStaticVariable;

  inline explicit PartialStaticVariable(llvm::StringRef name,
                                        rq::Expression &expression,
                                        rq::ExpressionFlags attributes,
                                        rq::Module &module,
                                        rq::SymbolTable &containing_table,
                                        rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_STATIC_VARIABLE, name, expression,
                attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_STATIC_VARIABLE;
  }
};

struct PartialFunction : public rq::Partial {
  using Self = rq::PartialFunction;

  inline explicit PartialFunction(llvm::StringRef name,
                                  rq::Expression &expression,
                                  rq::ExpressionFlags attributes,
                                  rq::Module &module,
                                  rq::SymbolTable &containing_table,
                                  rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_FUNCTION, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_FUNCTION;
  }
};

struct PartialMethod : public rq::Partial {
  using Self = rq::PartialMethod;

  inline explicit PartialMethod(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &containing_table,
                                rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_METHOD, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_METHOD;
  }
};

struct PartialRanger : public rq::Partial {
  using Self = rq::PartialRanger;

  inline explicit PartialRanger(llvm::StringRef name,
                                rq::Expression &expression,
                                rq::ExpressionFlags attributes,
                                rq::Module &module,
                                rq::SymbolTable &containing_table,
                                rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_METHOD, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_METHOD;
  }
};

struct PartialExtensionFunction : public rq::Partial {
  using Self = rq::PartialExtensionFunction;

  inline explicit PartialExtensionFunction(llvm::StringRef name,
                                           rq::Expression &expression,
                                           rq::ExpressionFlags attributes,
                                           rq::Module &module,
                                           rq::SymbolTable &containing_table,
                                           rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_EXTENSION_FUNCTION, name, expression,
                attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_EXTENSION_FUNCTION;
  }
};

struct PartialExtensionMethod : public rq::Partial {
  using Self = rq::PartialExtensionMethod;

  inline explicit PartialExtensionMethod(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &containing_table,
                                         rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_EXTENSION_METHOD, name, expression,
                attributes, module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_EXTENSION_METHOD;
  }
};

struct PartialExtensionRanger : public rq::Partial {
  using Self = rq::PartialExtensionRanger;

  inline explicit PartialExtensionRanger(llvm::StringRef name,
                                         rq::Expression &expression,
                                         rq::ExpressionFlags attributes,
                                         rq::Module &module,
                                         rq::SymbolTable &containing_table,
                                         rq::SymbolTable &hosting_table)
      : Partial(rq::Opcode::SY_PARTIAL_METHOD, name, expression, attributes,
                module, containing_table, hosting_table) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() ==
           rq::Opcode::SY_PARTIAL_METHOD;
  }
};

struct Constant : public rq::Entity {
  using Self = rq::Constant;

  inline explicit Constant(rq::Opcode opcode) : Entity(opcode) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsConstant(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_parent_only<rq::Constant> final : std::true_type {};

void RQ_ALWAYS_INLINE profileTypeConstant(llvm::FoldingSetNodeID &id,
                                          const rq::Symbol &symbol,
                                          rq::TypeFlags attributes) {
  id.AddPointer(&symbol);
  id.AddInteger(static_cast<unsigned>(attributes));
}

struct TypeConstant final : public rq::Constant, public llvm::FoldingSetNode {
  using Self = rq::TypeConstant;

  rq::Symbol *_symbol_ptr;
  rq::TypeFlags _type_flags;

  inline explicit TypeConstant(rq::Symbol &symbol, rq::TypeFlags flags)
      : Constant(rq::Opcode::CT_TYPE), _symbol_ptr(&symbol),
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
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_TYPE;
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
      : Constant(rq::Opcode::CT_EXPRESSION), _expression_ptr(&expression) {}

  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_EXPRESSION;
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
      : Constant(rq::Opcode::CT_BOOLEAN), _value(value) {}

  [[nodiscard]] RQ_ALWAYS_INLINE bool getValue() const { return this->_value; }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_BOOLEAN;
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
      : Constant(rq::Opcode::CT_INTEGER), _ap_int(int_) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APInt &getInt() const {
    return this->_ap_int;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_INTEGER;
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
      : Constant(rq::Opcode::CT_FLOAT), _ap_float(float_) {}
  [[nodiscard]] RQ_ALWAYS_INLINE const llvm::APFloat &getFloat() const {
    return this->_ap_float;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_FLOAT;
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
      : Constant(rq::Opcode::CT_STRING), _string(string) {}
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getString() const {
    return this->_string;
  }
  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_STRING;
  }

  void Profile(llvm::FoldingSetNodeID &id) const {
    rq::profileStringConstant(id, this->getString());
  }
};

template <> struct is_acquired<rq::StringConstant> final : std::true_type {};

struct ArrayConstant : public rq::Constant {
  using Self = rq::ArrayConstant;

  inline explicit ArrayConstant() : Constant(rq::Opcode::CT_ARRAY) {}

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::dereferencePtr(entity).getOpcode() == rq::Opcode::CT_ARRAY;
  }
};

template <> struct is_acquired<rq::ArrayConstant> final : std::true_type {};

struct Instruction : public rq::Entity {
  using Self = rq::Instruction;

  const rq::Expression *_expression_ptr{nullptr};

  inline explicit Instruction(rq::Opcode opcode) : Entity(opcode) {
    RQ_ASSERT(rq::getIsInstruction(opcode), "not instruction");
  }
  RQ_ALWAYS_INLINE void setExpression(const rq::Expression &expression) {
    rq::assignSingleValue(this->_expression_ptr, &expression);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpression() const {
    return this->_expression_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getExpression() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
};

template <> struct is_parent_only<rq::Instruction> final : std::true_type {};

struct NullaryInstruction : public rq::Instruction {
  using Self = rq::NullaryInstruction;

  inline explicit NullaryInstruction(rq::Opcode opcode) : Instruction(opcode) {
    RQ_ASSERT(rq::getIsNullaryInstruction(opcode), "not nullary instruction");
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsNullaryInstruction(rq::dereferencePtr(entity).getOpcode());
  }
};

template <>
struct is_acquired<rq::NullaryInstruction> final : std::true_type {};

struct UnaryInstruction : public rq::Instruction {
  using Self = rq::UnaryInstruction;

  rq::Entity *_address0_ptr{nullptr};

  inline explicit UnaryInstruction(rq::Opcode opcode) : Instruction(opcode) {
    RQ_ASSERT(rq::getIsUnaryInstruction(opcode), "not unary instruction");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress0() const {
    return this->_address0_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity &address0) {
    rq::assignSingleValue(this->_address0_ptr, &address0);
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity *address0_ptr) {
    rq::assignSingleValue(this->_address0_ptr, address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity &address0) {
    return rq::replaceValue(this->_address0_ptr, &address0);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity *address0_ptr) {
    return rq::replaceValue(this->_address0_ptr, address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0() {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress0() {
    rq::Entity &address0 = this->getAddress0();
    this->_address0_ptr = nullptr;
    return address0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress0Ptr() {
    rq::Entity *address0_ptr = this->_address0_ptr;
    this->_address0_ptr = nullptr;
    return address0_ptr;
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsUnaryInstruction(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_acquired<rq::UnaryInstruction> final : std::true_type {};

struct BinaryInstruction : public rq::Instruction {
  using Self = rq::BinaryInstruction;

  rq::Entity *_address0_ptr{nullptr};
  rq::Entity *_address1_ptr{nullptr};

  inline explicit BinaryInstruction(rq::Opcode opcode) : Instruction(opcode) {
    RQ_ASSERT(rq::getIsBinaryInstruction(opcode), "not binary instruction");
  }

  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress0() const {
    return this->_address0_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasAddress1() const {
    return this->_address1_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity &address0) {
    rq::assignSingleValue(this->_address0_ptr, &address0);
  }
  RQ_ALWAYS_INLINE void setAddress0(rq::Entity *address0_ptr) {
    rq::assignSingleValue(this->_address0_ptr, address0_ptr);
  }
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity &address1) {
    rq::assignSingleValue(this->_address1_ptr, &address1);
  }
  RQ_ALWAYS_INLINE void setAddress1(rq::Entity *address1_ptr) {
    rq::assignSingleValue(this->_address1_ptr, address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity &address0) {
    return rq::replaceValue(this->_address0_ptr, &address0);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress0(rq::Entity *address0_ptr) {
    return rq::replaceValue(this->_address0_ptr, address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity &address1) {
    return rq::replaceValue(this->_address1_ptr, &address1);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &
  replaceAddress1(rq::Entity *address1_ptr) {
    return rq::replaceValue(this->_address1_ptr, address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress0() const {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress0() {
    return rq::dereferencePtr(this->_address0_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Entity &getAddress1() const {
    return rq::dereferencePtr(this->_address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &getAddress1() {
    return rq::dereferencePtr(this->_address1_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress0() {
    rq::Entity &address0 = this->getAddress0();
    this->_address0_ptr = nullptr;
    return address0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress0Ptr() {
    rq::Entity *address0_ptr = this->_address0_ptr;
    this->_address0_ptr = nullptr;
    return address0_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity &popAddress1() {
    rq::Entity &address1 = this->getAddress1();
    this->_address1_ptr = nullptr;
    return address1;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Entity *popAddress1Ptr() {
    rq::Entity *address1_ptr = this->_address1_ptr;
    this->_address1_ptr = nullptr;
    return address1_ptr;
  }

  [[nodiscard]] inline static bool classof(const Entity *entity) {
    return rq::getIsBinaryInstruction(rq::dereferencePtr(entity).getOpcode());
  }
};

template <> struct is_acquired<rq::BinaryInstruction> final : std::true_type {};

} // namespace rq