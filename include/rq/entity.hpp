#pragma once

#include <rq/bump_ptr_list.hpp>
#include <rq/codeunits.hpp>
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

enum class EntityKind : std::uint16_t {
  NONE = 0,

  // =====KEYWORDS=====

  // Nodes represent individual expressions in the AST. Each is identified by a
  // keyword.

  // this is the initial keyword set for expressions. it must be overwritten
  // later!
  KW_NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  KW_INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  KW_FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  KW_STRING_LITERAL,
  // left string interpolation (string literal with no end quote mark).
  KW_LEFT_INTERPOLATION_LITERAL,
  // middle string interpolation (string literal with no quote marks).
  KW_MIDDLE_INTERPOLATION_LITERAL,
  // right string interpolation (string literal with start quote mark).
  KW_RIGHT_INTERPOLATION_LITERAL,
  // a literal that represents a single text character.
  KW_CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  KW_IDENTIFIER_LITERAL,

  // ERRORS
  KW_ERROR,

  // SITUATIONAL
  KW_UNSITUATED_PARENTHESIS_GROUP,
  KW_UNSITUATED_EQUAL_OPERATOR,
  KW_UNSITUATED_ASCRIBE_STATEMENT,
  KW_UNSITUATED_ASCRIBE_TYPE,

  // LOGICAL
  KW_LOGICAL_AND,
  KW_LOGICAL_OR,
  KW_LOGICAL_COMPLEMENT,

  // COMPARISON
  KW_GREATER,
  KW_GREATER_EQUAL,
  KW_LESS,
  KW_LESS_EQUAL,
  KW_EQUAL,
  KW_NOT_EQUAL,

  // APPLY
  KW_EXTEND,
  KW_EXTENSION,
  KW_BINDING,
  KW_ASCRIBE_TYPE,
  KW_ASCRIBE_STATEMENT,
  KW_ASCRIBE_ROOT_OF_VALUE,
  KW_IDENTIFY,

  // ARITHMETIC
  KW_ADD,
  KW_SUBTRACT,
  KW_MULTIPLY,
  KW_DIVIDE,
  KW_MODULUS,
  KW_NEGATE,

  // CASTS
  KW_AS,
  KW_LITERAL_AS,
  KW_OF,
  KW_ELEMENTS_OF,
  KW_CAST,
  KW_CAST_OF,
  KW_DOWN_CAST,
  KW_DOWN_CAST_OF,
  KW_FLOAT_CAST,
  KW_FLOAT_CAST_OF,
  KW_PLATFORM_CAST,
  KW_PLATFORM_CAST_OF,
  KW_STATIC_CAST,
  KW_STATIC_CAST_OF,
  KW_BITWISE_CAST,
  KW_BITWISE_CAST_OF,

  // BITWISE
  KW_BITWISE_OR,
  KW_BITWISE_AND,
  KW_BITWISE_XOR,
  KW_BITWISE_COMPLEMENT,
  KW_BITWISE_SHIFT_LEFT,
  KW_BITWISE_SHIFT_RIGHT,
  KW_BITWISE_ROTATE_LEFT,
  KW_BITWISE_ROTATE_RIGHT,

  // MEMORY
  KW_CONTENT,
  KW_CONTENT_OF,
  KW_ADDRESS,
  KW_ADDRESS_OF,
  KW_BORROW,
  KW_BORROW_OF,
  KW_DATA_ADDRESS,
  KW_DATA_ADDRESS_OF,

  // ASSIGNMENT
  KW_ASSIGN,

  // SUBTYPE
  KW_ARRAY,
  KW_REFERENCE,
  KW_POINTER,
  KW_FAT_POINTER,

  // TYPE ATTRIBUTES
  KW_MUTABLE,
  KW_CONSTANT,
  KW_PARTIALLY_MUTABLE,
  KW_VOLATILE,
  KW_ATOMIC,
  KW_NULL_TERMINATED,
  KW_MAY_DISCARD,
  KW_DEBUG_TRAP_ON_PANIC,

  // PARAMETER RULES
  KW_POSITIONAL_PARAMETERS_END,
  KW_NAMED_PARAMETERS_BEGIN,

  // BRACES
  KW_TUPLE,
  KW_LAYOUT_TYPE,
  KW_NULL,
  KW_NULL_TYPE,
  KW_STRUCTURED_BINDING,
  KW_IGNORE,
  KW_SPECIALIZATION,

  // PROCEDURES
  KW_CALL,
  KW_NAMED_ARGUMENT,
  KW_INDEX_INTO,
  KW_SIGNATURE_TYPE,
  KW_DEFAULT_VALUE_PARAMETER,
  KW_DESTROY,
  KW_DESTROY_VALUE,
  KW_DROP,
  KW_DROP_VALUE,
  KW_MOVE,
  KW_MOVE_VALUE,
  KW_ENTRY,
  KW_FUNCTION,
  KW_METHOD,
  KW_EXTENSION_FUNCTION,
  KW_EXTENSION_METHOD,
  KW_CONSTRUCTOR,
  KW_LAYOUT_CONSTRUCTOR,
  KW_DESTRUCTOR,
  KW_RANGER,

  // CONTROL FLOW
  KW_RETURN,
  KW_BREAK,
  KW_CONTINUE,
  KW_FALLTHROUGH,
  KW_GOTO,
  KW_RANGE_OVER,

  // DECLARED TYPES
  KW_CLASS,
  KW_ENUMERATION,
  KW_MUTATION,

  // VALUES
  KW_INITIALIZER_LIST,
  KW_TRUE,
  KW_FALSE,
  KW_INDETERMINATE,
  // vignette value.
  KW_VALUE,
  // vignette index.
  KW_INDEX,
  // vignette or reflected enumerator index.
  KW_DISCRIMINANT,
  // vignette value returned from a block.
  KW_OUT,
  // reference to extended value of method or extension_method.
  KW_THIS,
  // value returned from a function.
  KW_RESULT,
  // retrieve command line arguments within entry.
  KW_COMMAND_LINE_ARGUMENTS,
  // resulting exit code within entry.
  KW_EXIT_CODE,

  // BUILTIN TYPES
  KW_INFERENCE,
  KW_VOID,
  KW_NO_RETURN,
  KW_BOOLEAN,
  KW_FLOAT,
  KW_HALF,
  KW_SINGLE,
  KW_DOUBLE,
  KW_QUADRUPLE,
  KW_BINARY,
  KW_BFLOAT,
  KW_BINARY16,
  KW_BINARY32,
  KW_BINARY64,
  KW_BINARY128,
  KW_BFLOAT16,
  KW_INTEGER,
  KW_SIGNED,
  KW_SIGNED_OF,
  KW_UNSIGNED,
  KW_UNSIGNED_OF,
  KW_SIGNED_FAST_BITS,
  KW_SIGNED_FAST_BYTES,
  KW_SIGNED_LEAST_BITS,
  KW_SIGNED_LEAST_BYTES,
  KW_SIGNED_EXACT_BITS,
  KW_SIGNED_EXACT_BYTES,
  KW_SIGNED_INDEX,
  KW_SIGNED_ADDRESS,
  KW_UNSIGNED_FAST_BITS,
  KW_UNSIGNED_FAST_BYTES,
  KW_UNSIGNED_LEAST_BITS,
  KW_UNSIGNED_LEAST_BTYES,
  KW_UNSIGNED_EXACT_BITS,
  KW_UNSIGNED_EXACT_BYTES,
  KW_UNSIGNED_INDEX,
  KW_UNSIGNED_ADDRESS,
  KW_ASCII,
  KW_UTF8,

  // VARIADIC ARGUMENTS
  KW_VARIADIC_ARGUMENTS,
  KW_FIRST_VARIADIC_ARGUMENT,
  KW_FIRST_VARIADIC_ARGUMENT_OF,
  KW_NEXT_VARIADIC_ARGUMENT,
  KW_NEXT_VARIADIC_ARGUMENT_OF,

  // SCOPES
  KW_IF,
  KW_ELSE_IF,
  KW_ELSE,
  KW_MATCH,
  KW_INLINE_MATCH,
  KW_SWITCH,
  KW_INLINE_SWITCH,
  KW_CASE,
  KW_WITH,
  KW_DEFAULT,
  KW_FOR,
  KW_WHILE,
  KW_SCOPE,
  KW_INLINE_SCOPE,
  KW_BLOCK,
  KW_INLINE_BLOCK,

  // RANGES
  KW_RANGE,
  KW_ARITHMETIC_SEQUENCE,
  KW_ARITHMETIC_SEQUENCE_CONDITION_LESS,
  KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER,
  KW_ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL,
  KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL,
  KW_ARITHMETIC_SEQUENCE_CONDITION_EQUAL,
  KW_ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL,
  KW_ARITHMETIC_SEQUENCE_STEP_ADD,
  KW_ARITHMETIC_SEQUENCE_STEP_SUBTRACT,
  KW_ARITHMETIC_SEQUENCE_STEP_MULTIPLY,
  KW_ARITHMETIC_SEQUENCE_STEP_DIVIDE,
  KW_ARITHMETIC_SEQUENCE_STEP_MODULUS,

  // ARITHMETIC INTERVAL TYPES
  KW_ARITHMETIC_INTERVAL_LESS,
  KW_ARITHMETIC_INTERVAL_LESS_EQUAL,
  KW_ARITHMETIC_INTERVAL_GREATER,
  KW_ARITHMETIC_INTERVAL_GREATER_EQUAL,
  KW_ARITHMETIC_INTERVAL_EQUAL,
  KW_ARITHMETIC_INTERVAL_NOT_EQUAL,

  // FINITE ARITHMETIC PROGRESSION TYPES
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS,
  KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS,
  KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD,
  KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT,
  KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY,
  KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE,
  KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS,

  // INFINITE ARITHMETIC PROGRESSION TYPES
  KW_INFINITE_ARITHMETIC_PROGRESSION_ADD,
  KW_INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT,
  KW_INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY,
  KW_INFINITE_ARITHMETIC_PROGRESSION_DIVIDE,
  KW_INFINITE_ARITHMETIC_PROGRESSION_MODULUS,

  // ACCESS MODIFIERS
  KW_PUBLIC,
  KW_PROTECTED,
  KW_EXPORT,

  // TABLE GRAPH
  KW_IMPORT,
  KW_USE,
  KW_FACADE,
  KW_TABLE,
  KW_MODULE_TRUNK,

  // ERROR HANDLING AND DEBUGGING
  KW_PANIC_TRAP,
  KW_DEBUG_TRAP,

  // HINTS
  KW_UNREACHABLE,
  KW_ASSUME,

  // SYMBOL ATTRIBUTES
  KW_OPAQUE,
  KW_OUTSIDE,
  KW_STATIC,
  KW_CAPTURE,
  KW_CAPTURE_OF,
  KW_EAGER,
  KW_MAY_PARENT,
  KW_PARENT,
  KW_ABSTRACT,
  KW_VIRTUAL,
  KW_OVERRIDE,
  KW_POSITION,
  KW_INLINE,
  KW_MANGLE,
  KW_PACK,
  KW_LABEL,
  KW_TEMPLATE,
  KW_LIKELY,
  KW_UNLIKELY,
  KW_DEPRECIATED,
  KW_MAY_COPY,
  KW_MAY_MOVE,
  KW_MUTATE_WITH,

  // MACROS
  KW_QUOTE,
  KW_EXPRESSION,
  KW_EXPAND,
  KW_EXPAND_STATEMENT,
  KW_EXPAND_LVALUE,
  KW_EXPAND_RVALUE,
  KW_EXPAND_REFLECTION,
  KW_EXPAND_ARGUMENT,
  KW_EXPAND_PARAMETER,
  KW_EXPAND_SYMBOL_PATH,
  KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE,

  // REFLECTIONS
  KW_REFLECT,
  KW_MEMBER_OF,
  KW_MEMBER_OF_TOP,
  KW_ASCEND_FRAME,
  KW_ASCEND_FRAME_OF,
  KW_BYTE_SIZE,
  KW_BYTE_SIZE_OF,
  KW_BIT_DEPTH,
  KW_BIT_DEPTH_OF,
  KW_ELEMENT_COUNT,
  KW_ELEMENT_COUNT_OF,
  KW_NAME,
  KW_NAME_OF,
  KW_LINE,
  KW_LINE_OF,
  KW_COLUMN,
  KW_COLUMN_OF,
  KW_IS,
  KW_IS_TYPE,
  KW_HOLDS,
  KW_HOLDS_ENUMERATOR,
  KW_TYPE,
  KW_TYPE_OF,
  KW_SYMBOL,
  KW_SYMBOL_OF,
  KW_HAS_MEMBER,
  KW_HAS_MEMBER_OF,
  KW_HAS,
  KW_HAS_OF,
  KW_GET,
  KW_GET_OF,
  KW_SIGNATURE,
  KW_SIGNATURE_OF,
  KW_LAYOUT,
  KW_LAYOUT_OF,
  // make a unique clone of a type that is not implicitly convertable
  // can use platform specific values for bit depth only if type is a synonym
  KW_SYNONYM,
  KW_SYNONYM_OF,

  // =====SYMBOLS=====

  // all symbols have unique instances. you can always test if two symbols
  // are exactly the same (including type attriubutes, etc) by comparing their
  // pointers.

  // TYPE DEFINITION
  SY_TYPE_DEFINITION,

  // SIMPLE BUILTIN
  SY_INFERENCE,
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

  // =====OPCODES=====

  // Instructions are built in stage 6 and contain baked instructions, including
  // evaluations from see (Symbolic Evolution Engine). In stage 7, the
  // insturctions are then used to build LLVM IR.

  // this is the initial keyword set for expressions. it must be overwritten
  // later!
  OP_NONE,

  // TODO

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

    // =====KEYWORDS=====

  case E::KW_NONE:
    return "none";

  // LITERALS
  case E::KW_INTEGER_LITERAL:
    return "__integer_literal";
  case E::KW_FLOAT_LITERAL:
    return "__float_literal";
  case E::KW_STRING_LITERAL:
    return "__string_literal";
  case E::KW_LEFT_INTERPOLATION_LITERAL:
    return "__left_interpolation_literal";
  case E::KW_MIDDLE_INTERPOLATION_LITERAL:
    return "__middle_interpolation_literal";
  case E::KW_RIGHT_INTERPOLATION_LITERAL:
    return "__right_interpolation_literal";
  case E::KW_CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case E::KW_IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case E::KW_ERROR:
    return "__error";

  // SITUATIONAL
  case E::KW_UNSITUATED_PARENTHESIS_GROUP:
    return "_unsituated_parenthesis_group";
  case E::KW_UNSITUATED_EQUAL_OPERATOR:
    return "_unsituated_equal_operator";
  case E::KW_UNSITUATED_ASCRIBE_STATEMENT:
    return "_unsituated_ascribe_statement";
  case E::KW_UNSITUATED_ASCRIBE_TYPE:
    return "_unsituated_ascribe_type";

  // LOGICAL
  case E::KW_LOGICAL_AND:
    return "_logical_and";
  case E::KW_LOGICAL_OR:
    return "_logical_or";
  case E::KW_LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case E::KW_GREATER:
    return "_greater";
  case E::KW_GREATER_EQUAL:
    return "_greater_equal";
  case E::KW_LESS:
    return "_less";
  case E::KW_LESS_EQUAL:
    return "_less_equal";
  case E::KW_EQUAL:
    return "_equal";
  case E::KW_NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case E::KW_EXTEND:
    return "_extend";
  case E::KW_EXTENSION:
    return "_extension";
  case E::KW_BINDING:
    return "_binding";
  case E::KW_ASCRIBE_TYPE:
    return "_ascribe_type";
  case E::KW_ASCRIBE_STATEMENT:
    return "_ascribe_statement";
  case E::KW_ASCRIBE_ROOT_OF_VALUE:
    return "_ascribe_root_of_value";
  case E::KW_IDENTIFY:
    return "_identify";

  // ARITHMETIC
  case E::KW_ADD:
    return "_add";
  case E::KW_SUBTRACT:
    return "_subtract";
  case E::KW_MULTIPLY:
    return "_multiply";
  case E::KW_DIVIDE:
    return "_divide";
  case E::KW_MODULUS:
    return "_modulus";
  case E::KW_NEGATE:
    return "_negate";

  // CASTS
  case E::KW_AS:
    return "as";
  case E::KW_LITERAL_AS:
    return "_literal_as";
  case E::KW_OF:
    return "of";
  case E::KW_ELEMENTS_OF:
    return "_elements_of";
  case E::KW_CAST:
    return "cast";
  case E::KW_CAST_OF:
    return "_cast_of";
  case E::KW_DOWN_CAST:
    return "down_cast";
  case E::KW_DOWN_CAST_OF:
    return "_down_cast_of";
  case E::KW_FLOAT_CAST:
    return "float_cast";
  case E::KW_FLOAT_CAST_OF:
    return "_float_cast_of";
  case E::KW_PLATFORM_CAST:
    return "platform_cast";
  case E::KW_PLATFORM_CAST_OF:
    return "_platform_cast_of";
  case E::KW_STATIC_CAST:
    return "static_cast";
  case E::KW_STATIC_CAST_OF:
    return "_static_cast_of";
  case E::KW_BITWISE_CAST:
    return "bitwise_cast";
  case E::KW_BITWISE_CAST_OF:
    return "_bitwise_cast_of";

  // BITWISE
  case E::KW_BITWISE_OR:
    return "_bitwise_or";
  case E::KW_BITWISE_AND:
    return "_bitwise_and";
  case E::KW_BITWISE_XOR:
    return "_bitwise_xor";
  case E::KW_BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case E::KW_BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case E::KW_BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case E::KW_BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case E::KW_BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case E::KW_CONTENT:
    return "content";
  case E::KW_CONTENT_OF:
    return "_content_of";
  case E::KW_ADDRESS:
    return "address";
  case E::KW_ADDRESS_OF:
    return "_address_of";
  case E::KW_BORROW:
    return "borrow";
  case E::KW_BORROW_OF:
    return "_borrow_of";
  case E::KW_DATA_ADDRESS:
    return "data_address";
  case E::KW_DATA_ADDRESS_OF:
    return "_data_address_of";

  // ASSIGNMENT
  case E::KW_ASSIGN:
    return "_assign";

  // SUBTYPE
  case E::KW_ARRAY:
    return "_array";
  case E::KW_REFERENCE:
    return "_reference";
  case E::KW_POINTER:
    return "_pointer";
  case E::KW_FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case E::KW_MUTABLE:
    return "_mutable";
  case E::KW_CONSTANT:
    return "";
  case E::KW_PARTIALLY_MUTABLE:
    return "partially_mutable";
  case E::KW_VOLATILE:
    return "volatile";
  case E::KW_ATOMIC:
    return "atomic";
  case E::KW_NULL_TERMINATED:
    return "null_terminated";
  case E::KW_MAY_DISCARD:
    return "may_discard";
  case E::KW_DEBUG_TRAP_ON_PANIC:
    return "debug_trap_on_panic";

  // PARAMETER RULES
  case E::KW_POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case E::KW_NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";

  // BRACES
  case E::KW_TUPLE:
    return "_tuple";
  case E::KW_LAYOUT_TYPE:
    return "_layout_type";
  case E::KW_NULL:
    return "_null";
  case E::KW_NULL_TYPE:
    return "_null_type";
  case E::KW_STRUCTURED_BINDING:
    return "_structured_binding";
  case E::KW_IGNORE:
    return "_ignore";
  case E::KW_SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case E::KW_CALL:
    return "_call";
  case E::KW_NAMED_ARGUMENT:
    return "_named_argument";
  case E::KW_INDEX_INTO:
    return "_index_into";
  case E::KW_SIGNATURE_TYPE:
    return "_signature_type";
  case E::KW_DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case E::KW_DESTROY:
    return "destroy";
  case E::KW_DESTROY_VALUE:
    return "_destroy_value";
  case E::KW_DROP:
    return "drop";
  case E::KW_DROP_VALUE:
    return "_drop_value";
  case E::KW_MOVE:
    return "move";
  case E::KW_MOVE_VALUE:
    return "_move_value";
  case E::KW_ENTRY:
    return "entry";
  case E::KW_FUNCTION:
    return "function";
  case E::KW_METHOD:
    return "method";
  case E::KW_EXTENSION_FUNCTION:
    return "extension_function";
  case E::KW_EXTENSION_METHOD:
    return "extension_method";
  case E::KW_CONSTRUCTOR:
    return "constructor";
  case E::KW_LAYOUT_CONSTRUCTOR:
    return "layout_constructor";
  case E::KW_DESTRUCTOR:
    return "destructor";
  case E::KW_RANGER:
    return "ranger";

  // CONTROL FLOW
  case E::KW_RETURN:
    return "return";
  case E::KW_BREAK:
    return "break";
  case E::KW_CONTINUE:
    return "continue";
  case E::KW_FALLTHROUGH:
    return "fallthrough";
  case E::KW_GOTO:
    return "goto";
  case E::KW_RANGE_OVER:
    return "range_over";

  // DECLARED TYPES
  case E::KW_CLASS:
    return "class";
  case E::KW_ENUMERATION:
    return "enumeration";
  case E::KW_MUTATION:
    return "mutation";

  // VALUES
  case E::KW_INITIALIZER_LIST:
    return "_initializer_list";
  case E::KW_TRUE:
    return "true";
  case E::KW_FALSE:
    return "false";
  case E::KW_INDETERMINATE:
    return "indeterminate";
  case E::KW_VALUE:
    return "value";
  case E::KW_INDEX:
    return "index";
  case E::KW_DISCRIMINANT:
    return "discriminant";
  case E::KW_OUT:
    return "out";
  case E::KW_THIS:
    return "this";
  case E::KW_RESULT:
    return "result";
  case E::KW_COMMAND_LINE_ARGUMENTS:
    return "command_line_arguments";
  case E::KW_EXIT_CODE:
    return "exit_code";

  // BUILTIN TYPES
  case E::KW_INFERENCE:
    return "_inference";
  case E::KW_VOID:
    return "void";
  case E::KW_NO_RETURN:
    return "no_return";
  case E::KW_BOOLEAN:
    return "boolean";
  case E::KW_FLOAT:
    return "float";
  case E::KW_HALF:
    return "half";
  case E::KW_SINGLE:
    return "single";
  case E::KW_DOUBLE:
    return "double";
  case E::KW_QUADRUPLE:
    return "quadruple";
  case E::KW_BINARY:
    return "binary";
  case E::KW_BFLOAT:
    return "bfloat";
  case E::KW_BINARY16:
    return "binary16";
  case E::KW_BINARY32:
    return "binary32";
  case E::KW_BINARY64:
    return "binary64";
  case E::KW_BINARY128:
    return "binary128";
  case E::KW_BFLOAT16:
    return "bfloat16";
  case E::KW_INTEGER:
    return "integer";
  case E::KW_SIGNED:
    return "signed";
  case E::KW_SIGNED_OF:
    return "_signed_of";
  case E::KW_UNSIGNED:
    return "unsigned";
  case E::KW_UNSIGNED_OF:
    return "_unsigned_of";
  case E::KW_SIGNED_FAST_BITS:
    return "signed_fast_bits";
  case E::KW_SIGNED_FAST_BYTES:
    return "signed_fast_bytes";
  case E::KW_SIGNED_LEAST_BITS:
    return "signed_least_bits";
  case E::KW_SIGNED_LEAST_BYTES:
    return "signed_least_bytes";
  case E::KW_SIGNED_EXACT_BITS:
    return "signed_exact_bits";
  case E::KW_SIGNED_EXACT_BYTES:
    return "signed_exact_bytes";
  case E::KW_SIGNED_INDEX:
    return "signed_index";
  case E::KW_SIGNED_ADDRESS:
    return "signed_address";
  case E::KW_UNSIGNED_FAST_BITS:
    return "unsigned_fast_bits";
  case E::KW_UNSIGNED_FAST_BYTES:
    return "unsigned_fast_bytes";
  case E::KW_UNSIGNED_LEAST_BITS:
    return "unsigned_least_bits";
  case E::KW_UNSIGNED_LEAST_BTYES:
    return "unsigned_least_btyes";
  case E::KW_UNSIGNED_EXACT_BITS:
    return "unsigned_exact_bits";
  case E::KW_UNSIGNED_EXACT_BYTES:
    return "unsigned_exact_bytes";
  case E::KW_UNSIGNED_INDEX:
    return "unsigned_index";
  case E::KW_UNSIGNED_ADDRESS:
    return "unsigned_address";
  case E::KW_ASCII:
    return "ascii";
  case E::KW_UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case E::KW_VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case E::KW_FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case E::KW_FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case E::KW_NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case E::KW_NEXT_VARIADIC_ARGUMENT_OF:
    return "_next_variadic_argument_of";

  // SCOPES
  case E::KW_IF:
    return "if";
  case E::KW_ELSE_IF:
    return "else_if";
  case E::KW_ELSE:
    return "else";
  case E::KW_MATCH:
    return "match";
  case E::KW_INLINE_MATCH:
    return "inline_match";
  case E::KW_SWITCH:
    return "switch";
  case E::KW_INLINE_SWITCH:
    return "inline_switch";
  case E::KW_CASE:
    return "case";
  case E::KW_WITH:
    return "with";
  case E::KW_DEFAULT:
    return "default";
  case E::KW_FOR:
    return "for";
  case E::KW_WHILE:
    return "while";
  case E::KW_SCOPE:
    return "scope";
  case E::KW_INLINE_SCOPE:
    return "inline_scope";
  case E::KW_BLOCK:
    return "block";
  case E::KW_INLINE_BLOCK:
    return "inline_block";

  // RANGES
  case E::KW_RANGE:
    return "range";
  case E::KW_ARITHMETIC_SEQUENCE:
    return "_arithmetic_sequence";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return "_arithmetic_sequence_condition_less";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return "_arithmetic_sequence_condition_greater";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return "_arithmetic_sequence_condition_less_equal";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return "_arithmetic_sequence_condition_greater_equal";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return "_arithmetic_sequence_condition_equal";
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return "_arithmetic_sequence_condition_not_equal";
  case E::KW_ARITHMETIC_SEQUENCE_STEP_ADD:
    return "_arithmetic_sequence_step_add";
  case E::KW_ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return "_arithmetic_sequence_step_subtract";
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return "_arithmetic_sequence_step_multiply";
  case E::KW_ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return "_arithmetic_sequence_step_divide";
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return "_arithmetic_sequence_step_modulus";

  // ARITHMETIC INTERVAL
  case E::KW_ARITHMETIC_INTERVAL_LESS:
    return "_arithmetic_interval_less";
  case E::KW_ARITHMETIC_INTERVAL_LESS_EQUAL:
    return "_arithmetic_interval_less_equal";
  case E::KW_ARITHMETIC_INTERVAL_GREATER:
    return "_arithmetic_interval_greater";
  case E::KW_ARITHMETIC_INTERVAL_GREATER_EQUAL:
    return "_arithmetic_interval_greater_equal";
  case E::KW_ARITHMETIC_INTERVAL_EQUAL:
    return "_arithmetic_interval_equal";
  case E::KW_ARITHMETIC_INTERVAL_NOT_EQUAL:
    return "_arithmetic_interval_not_equal";

  // FINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    return "_finite_arithmetic_progression_less_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    return "_finite_arithmetic_progression_less_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    return "_finite_arithmetic_progression_less_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    return "_finite_arithmetic_progression_less_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    return "_finite_arithmetic_progression_less_modulus";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    return "_finite_arithmetic_progression_less_equal_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_less_equal_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_less_equal_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_less_equal_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_less_equal_modulus";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    return "_finite_arithmetic_progression_greater_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    return "_finite_arithmetic_progression_greater_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    return "_finite_arithmetic_progression_greater_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    return "_finite_arithmetic_progression_greater_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    return "_finite_arithmetic_progression_greater_modulus";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    return "_finite_arithmetic_progression_greater_equal_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_greater_equal_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_greater_equal_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_greater_equal_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_greater_equal_modulus";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    return "_finite_arithmetic_progression_equal_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_equal_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_equal_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_equal_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_equal_modulus";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    return "_finite_arithmetic_progression_not_equal_add";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_not_equal_subtract";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_not_equal_multiply";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_not_equal_divide";
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_not_equal_modulus";

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_ADD:
    return "_infinite_arithmetic_progression_add";
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    return "_infinite_arithmetic_progression_subtract";
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    return "_infinite_arithmetic_progression_multiply";
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    return "_infinite_arithmetic_progression_divide";
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    return "_infinite_arithmetic_progression_modulus";

  // ACCESS MODIFIERS
  case E::KW_PUBLIC:
    return "public";
  case E::KW_PROTECTED:
    return "protected";
  case E::KW_EXPORT:
    return "export";

  // TABLE GRAPH
  case E::KW_IMPORT:
    return "import";
  case E::KW_USE:
    return "use";
  case E::KW_FACADE:
    return "facade";
  case E::KW_TABLE:
    return "table";
  case E::KW_MODULE_TRUNK:
    return "_module_trunk";

  // ERROR HANDLING AND DEBUGGING
  case E::KW_PANIC_TRAP:
    return "panic_trap";
  case E::KW_DEBUG_TRAP:
    return "debug_trap";

  // HINTS
  case E::KW_UNREACHABLE:
    return "unreachable";
  case E::KW_ASSUME:
    return "assume";

  // SYMBOL ATTRIBUTES
  case E::KW_OPAQUE:
    return "opaque";
  case E::KW_OUTSIDE:
    return "outside";
  case E::KW_STATIC:
    return "static";
  case E::KW_CAPTURE:
    return "capture";
  case E::KW_CAPTURE_OF:
    return "_capture_of";
  case E::KW_EAGER:
    return "eager";
  case E::KW_MAY_PARENT:
    return "may_parent";
  case E::KW_PARENT:
    return "parent";
  case E::KW_ABSTRACT:
    return "abstract";
  case E::KW_VIRTUAL:
    return "virtual";
  case E::KW_OVERRIDE:
    return "override";
  case E::KW_POSITION:
    return "position";
  case E::KW_INLINE:
    return "inline";
  case E::KW_MANGLE:
    return "mangle";
  case E::KW_PACK:
    return "pack";
  case E::KW_LABEL:
    return "label";
  case E::KW_TEMPLATE:
    return "template";
  case E::KW_LIKELY:
    return "likely";
  case E::KW_UNLIKELY:
    return "unlikely";
  case E::KW_DEPRECIATED:
    return "depreciated";
  case E::KW_MAY_COPY:
    return "may_copy";
  case E::KW_MAY_MOVE:
    return "may_move";
  case E::KW_MUTATE_WITH:
    return "mutate_with";

  // NODES
  case E::KW_QUOTE:
    return "quote";
  case E::KW_EXPRESSION:
    return "expression";
  case E::KW_EXPAND:
    return "expand";
  case E::KW_EXPAND_STATEMENT:
    return "_expand_statement";
  case E::KW_EXPAND_LVALUE:
    return "_expand_lvalue";
  case E::KW_EXPAND_RVALUE:
    return "_expand_rvalue";
  case E::KW_EXPAND_REFLECTION:
    return "_expand_reflection";
  case E::KW_EXPAND_ARGUMENT:
    return "_expand_argument";
  case E::KW_EXPAND_PARAMETER:
    return "_expand_parameter";
  case E::KW_EXPAND_SYMBOL_PATH:
    return "_expand_symbol_path";
  case E::KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return "_expand_arithmetic_sequence_stage";

  // REFLECTIONS
  case E::KW_REFLECT:
    return "_reflect";
  case E::KW_MEMBER_OF:
    return "_member_of";
  case E::KW_MEMBER_OF_TOP:
    return "_member_of_top";
  case E::KW_ASCEND_FRAME:
    return "_ascend_frame";
  case E::KW_ASCEND_FRAME_OF:
    return "_ascend_frame_of";
  case E::KW_BYTE_SIZE:
    return "byte_size";
  case E::KW_BYTE_SIZE_OF:
    return "_byte_size_of";
  case E::KW_BIT_DEPTH:
    return "bit_depth";
  case E::KW_BIT_DEPTH_OF:
    return "_bit_depth_of";
  case E::KW_ELEMENT_COUNT:
    return "element_count";
  case E::KW_ELEMENT_COUNT_OF:
    return "_element_count_of";
  case E::KW_NAME:
    return "name";
  case E::KW_NAME_OF:
    return "_name_of";
  case E::KW_LINE:
    return "line";
  case E::KW_LINE_OF:
    return "_line_of";
  case E::KW_COLUMN:
    return "column";
  case E::KW_COLUMN_OF:
    return "_column_of";
  case E::KW_IS:
    return "is";
  case E::KW_IS_TYPE:
    return "_is_type";
  case E::KW_HOLDS:
    return "holds";
  case E::KW_HOLDS_ENUMERATOR:
    return "_holds_enumerator";
  case E::KW_TYPE:
    return "type";
  case E::KW_TYPE_OF:
    return "_type_of";
  case E::KW_SYMBOL:
    return "symbol";
  case E::KW_SYMBOL_OF:
    return "_symbol_of";
  case E::KW_HAS_MEMBER:
    return "has_member";
  case E::KW_HAS_MEMBER_OF:
    return "_has_member_of";
  case E::KW_HAS:
    return "has";
  case E::KW_HAS_OF:
    return "_has_of";
  case E::KW_GET:
    return "get";
  case E::KW_GET_OF:
    return "_get_of";
  case E::KW_SIGNATURE:
    return "signature";
  case E::KW_SIGNATURE_OF:
    return "_signature_of";
  case E::KW_LAYOUT:
    return "layout";
  case E::KW_LAYOUT_OF:
    return "_layout_of";
  case E::KW_SYNONYM:
    return "synonym";
  case E::KW_SYNONYM_OF:
    return "_synonym_of";

  // =====SYMBOLS=====

  // TYPE DEFINITION
  case E::SY_TYPE_DEFINITION:
    return "sy_type_definition";

  // SIMPLE BUILTIN
  case E::SY_INFERENCE:
    return "sy_inference";
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

    // =====OPCODES=====

  case E::OP_NONE:
    return "op_none";

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
  KEYWORD = 0,  // 00
  SYMBOL = 1,   // 01
  CONSTANT = 2, // 10
  OPCODE = 3,   // 11
  // mask for the low two bits that encode the entity category
  CATEGORY_MASK = (rq::getBit(0) | rq::getBit(1)),

  // KEYWORD FLAGS
  // these flags are valid only when working with nodes
  KW_CONVERGING = rq::getBit(2),
  KW_LITERAL = rq::getBit(3),
  KW_UNQUOTED_LEFT = rq::getBit(4),
  KW_UNQUOTED_RIGHT = rq::getBit(5),
  KW_INTERNAL = rq::getBit(6),
  KW_UNIVERSALIZABLE = rq::getBit(7),
  KW_STATEMENT_BRANCHES = rq::getBit(8),
  KW_STARTING_CHAINLINK = rq::getBit(9),
  KW_CONTINUING_CHAINLINK = rq::getBit(10),
  KW_FINISHING_CHAINLINK = rq::getBit(11),
  KW_IF_CHAINLINK = rq::getBit(12),
  KW_ARM_CHAINLINK = rq::getBit(13),
  // TRUNK
  KW_STATEMENT = rq::getBit(14),
  KW_RVALUE = rq::getBit(15),
  KW_LVALUE = rq::getBit(16),
  KW_REFLECTION = rq::getBit(17),
  KW_ARGUMENT = rq::getBit(18),
  KW_PARAMETER = rq::getBit(19),
  KW_BINDING = rq::getBit(20),
  KW_SYMBOL_PATH = rq::getBit(21),
  KW_ASCRIPTION = rq::getBit(22),
  KW_TYPE_ATTRIBUTE = rq::getBit(23),
  KW_EXPRESSION_ATTRIBUTE = rq::getBit(24),
  KW_ARITHMETIC_SEQUENCE_STEP = rq::getBit(25),
  KW_ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(26),
  KW_ALL_SITUATIONS = KW_STATEMENT | KW_RVALUE | KW_LVALUE | KW_REFLECTION |
                      KW_ARGUMENT | KW_PARAMETER | KW_BINDING | KW_SYMBOL_PATH |
                      KW_ASCRIPTION | KW_TYPE_ATTRIBUTE |
                      KW_EXPRESSION_ATTRIBUTE | KW_ARITHMETIC_SEQUENCE_STEP |
                      KW_ARITHMETIC_SEQUENCE_CONDITION,

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

  // CONSTANT FLAGS
  // TODO

  // INSTRUCTION FLAGS
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

  case E::KW_NONE:
    return EF::KEYWORD;

    // LITERALS
  case E::KW_INTEGER_LITERAL:
    return EF::KEYWORD | EF::KW_LITERAL | EF::KW_INTERNAL | EF::KW_RVALUE |
           EF::KW_ARGUMENT;
  case E::KW_FLOAT_LITERAL:
    return EF::KEYWORD | EF::KW_LITERAL | EF::KW_INTERNAL | EF::KW_RVALUE |
           EF::KW_ARGUMENT;
  case E::KW_STRING_LITERAL:
    return EF::KEYWORD | EF::KW_LITERAL | EF::KW_INTERNAL | EF::KW_RVALUE |
           EF::KW_ARGUMENT;
  case E::KW_LEFT_INTERPOLATION_LITERAL:
    return EF::KEYWORD | EF::KW_UNQUOTED_RIGHT | EF::KW_LITERAL |
           EF::KW_INTERNAL | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_MIDDLE_INTERPOLATION_LITERAL:
    return EF::KEYWORD | EF::KW_UNQUOTED_LEFT | EF::KW_UNQUOTED_RIGHT |
           EF::KW_LITERAL | EF::KW_INTERNAL | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_RIGHT_INTERPOLATION_LITERAL:
    return EF::KEYWORD | EF::KW_UNQUOTED_LEFT | EF::KW_LITERAL |
           EF::KW_INTERNAL | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_CODEUNIT_LITERAL:
    return EF::KEYWORD | EF::KW_LITERAL | EF::KW_INTERNAL | EF::KW_RVALUE |
           EF::KW_ARGUMENT;
  case E::KW_IDENTIFIER_LITERAL:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_LITERAL | EF::KW_INTERNAL |
           EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_REFLECTION | EF::KW_ARGUMENT |
           EF::KW_PARAMETER | EF::KW_SYMBOL_PATH;

  // ERRORS
  case E::KW_ERROR:
    return EF::KEYWORD | EF::KW_INTERNAL;

  // SITUATIONAL
  case E::KW_UNSITUATED_PARENTHESIS_GROUP:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT |
           EF::KW_LVALUE | EF::KW_SYMBOL_PATH |
           EF::KW_ARITHMETIC_SEQUENCE_STEP |
           EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_UNSITUATED_EQUAL_OPERATOR:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSITUATED_ASCRIBE_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER |
           EF::KW_REFLECTION | EF::KW_ASCRIPTION;
  case E::KW_UNSITUATED_ASCRIBE_STATEMENT:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_PARAMETER | EF::KW_ARGUMENT |
           EF::KW_ASCRIPTION;

  // LOGICAL
  case E::KW_LOGICAL_AND:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LOGICAL_OR:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LOGICAL_COMPLEMENT:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // COMPARISON
  case E::KW_GREATER:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_GREATER_EQUAL:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LESS:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LESS_EQUAL:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_EQUAL:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_NOT_EQUAL:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // APPLY
  case E::KW_EXTEND:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_EXTENSION:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINDING:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_LVALUE | EF::KW_PARAMETER |
           EF::KW_ARGUMENT | EF::KW_BINDING;
  case E::KW_ASCRIBE_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER |
           EF::KW_REFLECTION | EF::KW_ASCRIPTION;
  case E::KW_ASCRIBE_STATEMENT:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_PARAMETER | EF::KW_ARGUMENT |
           EF::KW_ASCRIPTION;
  case E::KW_ASCRIBE_ROOT_OF_VALUE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_ASCRIPTION;
  case E::KW_IDENTIFY:
    return EF::KEYWORD | EF::KW_SYMBOL_PATH | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // ARITHMETIC
  case E::KW_ADD:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_SUBTRACT:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_MULTIPLY:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_DIVIDE:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_MODULUS:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_NEGATE:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // CASTS
  case E::KW_AS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_LITERAL_AS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_OF:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_ELEMENTS_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_DOWN_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_DOWN_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_FLOAT_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_FLOAT_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_PLATFORM_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_PLATFORM_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_STATIC_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_STATIC_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_CAST:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_BITWISE_CAST_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // BITWISE
  case E::KW_BITWISE_OR:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_AND:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_XOR:
    return EF::KEYWORD | EF::KW_CONVERGING | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_COMPLEMENT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_SHIFT_LEFT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_SHIFT_RIGHT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_ROTATE_LEFT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BITWISE_ROTATE_RIGHT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // MEMORY
  case E::KW_CONTENT:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_CONTENT_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT;
  case E::KW_ADDRESS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_ADDRESS_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BORROW:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_BORROW_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_DATA_ADDRESS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_DATA_ADDRESS_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // ASSIGNMENT
  case E::KW_ASSIGN:
    return EF::KEYWORD | EF::KW_STATEMENT;

  // SUBTYPE
  case E::KW_ARRAY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_REFERENCE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_POINTER:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FAT_POINTER:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // TYPE MODIFIER
  case E::KW_MUTABLE:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_CONSTANT:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_PARTIALLY_MUTABLE:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_VOLATILE:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_ATOMIC:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_NULL_TERMINATED:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_MAY_DISCARD:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;
  case E::KW_DEBUG_TRAP_ON_PANIC:
    return EF::KEYWORD | EF::KW_TYPE_ATTRIBUTE;

  // PARAMETER RULES
  case E::KW_POSITIONAL_PARAMETERS_END:
    return EF::KEYWORD | EF::KW_PARAMETER;
  case E::KW_NAMED_PARAMETERS_BEGIN:
    return EF::KEYWORD | EF::KW_PARAMETER;

  // BRACES
  case E::KW_TUPLE:
    return EF::KEYWORD | EF::KW_LVALUE | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LAYOUT_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_NULL:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_NULL_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_STRUCTURED_BINDING:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_IGNORE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_SPECIALIZATION:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // PROCEDURES
  case E::KW_CALL:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_RVALUE | EF::KW_LVALUE |
           EF::KW_ARGUMENT;
  case E::KW_NAMED_ARGUMENT:
    return EF::KEYWORD | EF::KW_ARGUMENT;
  case E::KW_INDEX_INTO:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT;
  case E::KW_SIGNATURE_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_DEFAULT_VALUE_PARAMETER:
    return EF::KEYWORD | EF::KW_PARAMETER;
  case E::KW_DESTROY:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_DESTROY_VALUE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_DROP:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_DROP_VALUE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_MOVE:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_MOVE_VALUE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_ENTRY:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;
  case E::KW_FUNCTION:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_METHOD:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_EXTENSION_FUNCTION:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_EXTENSION_METHOD:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_CONSTRUCTOR:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;
  case E::KW_LAYOUT_CONSTRUCTOR:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_RVALUE;
  case E::KW_DESTRUCTOR:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;
  case E::KW_RANGER:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;

  // CONTROL FLOW
  case E::KW_RETURN:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_BREAK:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_CONTINUE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_FALLTHROUGH:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_GOTO:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_RANGE_OVER:
    return EF::KEYWORD | EF::KW_STATEMENT;

  // DECLARED TYPES
  case E::KW_CLASS:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_ENUMERATION:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_MUTATION:
    return EF::KEYWORD | EF::KW_STATEMENT;

  // VALUES
  case E::KW_INITIALIZER_LIST:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_TRUE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_FALSE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_INDETERMINATE:
    return EF::KEYWORD | EF::KW_RVALUE;
  case E::KW_VALUE:
    return EF::KEYWORD | EF::KW_RVALUE;
  case E::KW_INDEX:
    return EF::KEYWORD | EF::KW_RVALUE;
  case E::KW_DISCRIMINANT:
    return EF::KEYWORD | EF::KW_RVALUE;
  case E::KW_OUT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT;
  case E::KW_THIS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT;
  case E::KW_RESULT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT;
  case E::KW_COMMAND_LINE_ARGUMENTS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_EXIT_CODE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE;

  // BUILTIN TYPES
  case E::KW_INFERENCE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_VOID:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_NO_RETURN:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BOOLEAN:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FLOAT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_HALF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SINGLE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_DOUBLE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_QUADRUPLE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINARY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BFLOAT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINARY16:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINARY32:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINARY64:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BINARY128:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_BFLOAT16:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_INTEGER:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ASCII:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UTF8:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SYNONYM:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SYNONYM_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_FAST_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_FAST_BYTES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_LEAST_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_LEAST_BYTES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_EXACT_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_EXACT_BYTES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_INDEX:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SIGNED_ADDRESS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_FAST_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_FAST_BYTES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_LEAST_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_LEAST_BTYES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_EXACT_BITS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_EXACT_BYTES:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_INDEX:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_UNSIGNED_ADDRESS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // VARIADIC ARGUMENTS
  case E::KW_VARIADIC_ARGUMENTS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FIRST_VARIADIC_ARGUMENT:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_FIRST_VARIADIC_ARGUMENT_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_NEXT_VARIADIC_ARGUMENT:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_NEXT_VARIADIC_ARGUMENT_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;

  // SCOPES
  case E::KW_IF:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_STARTING_CHAINLINK | EF::KW_IF_CHAINLINK;
  case E::KW_ELSE_IF:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_CONTINUING_CHAINLINK | EF::KW_FINISHING_CHAINLINK |
           EF::KW_IF_CHAINLINK;
  case E::KW_ELSE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_FINISHING_CHAINLINK | EF::KW_IF_CHAINLINK;
  case E::KW_MATCH:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_INLINE_MATCH:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_RVALUE;
  case E::KW_SWITCH:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_INLINE_SWITCH:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_RVALUE;
  case E::KW_CASE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_STARTING_CHAINLINK | EF::KW_CONTINUING_CHAINLINK |
           EF::KW_FINISHING_CHAINLINK | EF::KW_ARM_CHAINLINK;
  case E::KW_WITH:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_STARTING_CHAINLINK | EF::KW_CONTINUING_CHAINLINK |
           EF::KW_FINISHING_CHAINLINK | EF::KW_ARM_CHAINLINK;
  case E::KW_DEFAULT:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_STARTING_CHAINLINK | EF::KW_CONTINUING_CHAINLINK |
           EF::KW_FINISHING_CHAINLINK | EF::KW_ARM_CHAINLINK;
  case E::KW_FOR:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_WHILE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_SCOPE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;
  case E::KW_INLINE_SCOPE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_RVALUE;
  case E::KW_BLOCK:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT;
  case E::KW_INLINE_BLOCK:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_RVALUE;

  // RANGES
  case E::KW_RANGE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_SEQUENCE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_ADD:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP;

  // ARITHMETIC INTERVAL
  case E::KW_ARITHMETIC_INTERVAL_LESS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_INTERVAL_LESS_EQUAL:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_INTERVAL_GREATER:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_INTERVAL_GREATER_EQUAL:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_INTERVAL_EQUAL:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_ARITHMETIC_INTERVAL_NOT_EQUAL:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // FINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_ADD:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // ACCESS MODIFIERS
  case E::KW_PUBLIC:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_PROTECTED:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_EXPORT:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;

  // TABLE GRAPH
  case E::KW_IMPORT:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_USE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_FACADE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_TABLE:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::KW_STATEMENT |
           EF::KW_RVALUE;
  case E::KW_MODULE_TRUNK:
    return EF::KEYWORD | EF::KW_STATEMENT_BRANCHES | EF::NONE; // TRUNK

  // ERROR HANDLING AND DEBUGGING
  case E::KW_PANIC_TRAP:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_DEBUG_TRAP:
    return EF::KEYWORD | EF::KW_STATEMENT;

  // HINTS
  case E::KW_UNREACHABLE:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_ASSUME:
    return EF::KEYWORD | EF::KW_STATEMENT;

  // SYMBOL ATTRIBUTES
  case E::KW_OPAQUE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_OUTSIDE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_STATIC:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_CAPTURE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE | EF::KW_REFLECTION |
           EF::KW_UNIVERSALIZABLE | EF::KW_RVALUE;
  case E::KW_CAPTURE_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_EAGER:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_MAY_PARENT:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_PARENT:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_ABSTRACT:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_VIRTUAL:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_OVERRIDE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_POSITION:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_INLINE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_MANGLE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_PACK:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_LABEL:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_TEMPLATE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_LIKELY:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_UNLIKELY:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_DEPRECIATED:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_MAY_COPY:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_MAY_MOVE:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;
  case E::KW_MUTATE_WITH:
    return EF::KEYWORD | EF::KW_EXPRESSION_ATTRIBUTE;

  // NODES
  case E::KW_QUOTE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_EXPRESSION:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_EXPAND:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_EXPAND_STATEMENT:
    return EF::KEYWORD | EF::KW_STATEMENT;
  case E::KW_EXPAND_LVALUE:
    return EF::KEYWORD | EF::KW_LVALUE;
  case E::KW_EXPAND_RVALUE:
    return EF::KEYWORD | EF::KW_RVALUE;
  case E::KW_EXPAND_REFLECTION:
    return EF::KEYWORD | EF::KW_REFLECTION;
  case E::KW_EXPAND_ARGUMENT:
    return EF::KEYWORD | EF::KW_ARGUMENT;
  case E::KW_EXPAND_PARAMETER:
    return EF::KEYWORD | EF::KW_PARAMETER;
  case E::KW_EXPAND_SYMBOL_PATH:
    return EF::KEYWORD | EF::KW_SYMBOL_PATH;
  case E::KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return EF::KEYWORD | EF::KW_ARITHMETIC_SEQUENCE_STEP |
           EF::KW_ARITHMETIC_SEQUENCE_CONDITION;

  // REFLECTIONS
  case E::KW_REFLECT:
    return EF::KEYWORD | EF::KW_STATEMENT | EF::KW_RVALUE | EF::KW_LVALUE |
           EF::KW_REFLECTION | EF::KW_ARGUMENT | EF::KW_PARAMETER |
           EF::KW_SYMBOL_PATH | EF::KW_ARITHMETIC_SEQUENCE_STEP |
           EF::KW_ARITHMETIC_SEQUENCE_CONDITION;
  case E::KW_MEMBER_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT |
           EF::KW_PARAMETER | EF::KW_SYMBOL_PATH | EF::KW_STATEMENT;
  case E::KW_MEMBER_OF_TOP:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT |
           EF::KW_PARAMETER | EF::KW_SYMBOL_PATH | EF::KW_STATEMENT;
  case E::KW_ASCEND_FRAME:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT |
           EF::KW_PARAMETER | EF::KW_SYMBOL_PATH;
  case E::KW_ASCEND_FRAME_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_LVALUE | EF::KW_ARGUMENT |
           EF::KW_PARAMETER | EF::KW_SYMBOL_PATH;
  case E::KW_BYTE_SIZE:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_BYTE_SIZE_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_BIT_DEPTH:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_BIT_DEPTH_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_ELEMENT_COUNT:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_ELEMENT_COUNT_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_NAME:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_NAME_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LINE:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_LINE_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_COLUMN:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_COLUMN_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_IS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_IS_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_HOLDS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_HOLDS_ENUMERATOR:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_TYPE:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER |
           EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_TYPE_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;
  case E::KW_SYMBOL:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_SYMBOL_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_HAS_MEMBER:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_HAS_MEMBER_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_HAS:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_HAS_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_GET:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_GET_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_SIGNATURE:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_SIGNATURE_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT;
  case E::KW_LAYOUT:
    return EF::KEYWORD | EF::KW_REFLECTION | EF::KW_UNIVERSALIZABLE;
  case E::KW_LAYOUT_OF:
    return EF::KEYWORD | EF::KW_RVALUE | EF::KW_ARGUMENT | EF::KW_PARAMETER;

  // TYPE DEFINITION SYMBOL
  case E::SY_TYPE_DEFINITION:
    return EF::SYMBOL | EF::SY_TYPE;

  // SIMPLE BUILTIN SYMBOL
  case E::SY_INFERENCE:
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
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE;
  case E::SY_TABLE:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE;
  case E::SY_CLASS:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_ENUMERATION:
    return EF::SYMBOL | EF::SY_SYMBOL_TABLE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;

  // PROCEDURE SYMBOL
  case E::SY_ENTRY:
    return EF::SYMBOL | EF::SY_PROCEDURE;
  case E::SY_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_EXTENSION_FUNCTION:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_EXTENSION_METHOD:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_CONSTRUCTOR:
    return EF::SYMBOL | EF::SY_PROCEDURE | EF::SY_HAS_TEMPLATE_ALTERNATIVE;
  case E::SY_DESTRUCTOR:
    return EF::SYMBOL | EF::SY_PROCEDURE;
  case E::SY_RANGER:
    return EF::SYMBOL | EF::SY_PROCEDURE;

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

  // INSTRUCTION
  case E::OP_NONE:
    return EF::OPCODE;

  case E::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsKeyword(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  using EF = rq::EntityFlags;
  using Underlying = std::underlying_type_t<EF>;
  const Underlying masked = rq::getMaskValue(flags, EF::CATEGORY_MASK);
  return masked == static_cast<Underlying>(EF::KEYWORD);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  using EF = rq::EntityFlags;
  using Underlying = std::underlying_type_t<EF>;
  const Underlying masked = rq::getMaskValue(flags, EF::CATEGORY_MASK);
  return masked == static_cast<Underlying>(EF::SYMBOL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  using EF = rq::EntityFlags;
  using Underlying = std::underlying_type_t<EF>;
  const Underlying masked = rq::getMaskValue(flags, EF::CATEGORY_MASK);
  return masked == static_cast<Underlying>(EF::CONSTANT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsOpcode(rq::EntityKind kind) {
  const rq::EntityFlags flags = rq::getFlags(kind);
  using EF = rq::EntityFlags;
  using Underlying = std::underlying_type_t<EF>;
  const Underlying masked = rq::getMaskValue(flags, EF::CATEGORY_MASK);
  return masked == static_cast<Underlying>(EF::OPCODE);
}

#define RQ_ASSERT_KEYWORD(kind)                                                \
  RQ_ASSERT(rq::getIsKeyword((kind)), "not keyword")

#define RQ_ASSERT_SYMBOL(kind) RQ_ASSERT(rq::getIsSymbol((kind)), "not symbol")

#define RQ_ASSERT_CONSTANT(kind)                                               \
  RQ_ASSERT(rq::getIsConstant((kind)), "not constant")

#define RQ_ASSERT_OPCODE(kind) RQ_ASSERT(rq::getIsOpcode((kind)), "not opcode")

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterMarkKeyword(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  return kind == rq::EntityKind::KW_NAMED_PARAMETERS_BEGIN ||
         kind == rq::EntityKind::KW_POSITIONAL_PARAMETERS_END;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralKeyword(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_LITERAL);
}

[[nodiscard]] inline rq::EntityKind
getSituatedAscribeKeyword(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  switch (kind) {
  case rq::EntityKind::KW_UNSITUATED_ASCRIBE_STATEMENT:
    return rq::EntityKind::KW_ASCRIBE_STATEMENT;
  case rq::EntityKind::KW_UNSITUATED_ASCRIBE_TYPE:
    return rq::EntityKind::KW_ASCRIBE_TYPE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

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

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_UNQUOTED_LEFT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_UNQUOTED_RIGHT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_CONVERGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasStatementBranches(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_STATEMENT_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionBranches(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasNone(flags, rq::EntityFlags::KW_STATEMENT_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUniversalizable(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::EntityFlags::KW_UNIVERSALIZABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags, rq::EntityFlags::KW_STARTING_CHAINLINK |
                                   rq::EntityFlags::KW_CONTINUING_CHAINLINK |
                                   rq::EntityFlags::KW_FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAllChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_IF_CHAINLINK |
                                  rq::EntityFlags::KW_ARM_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeIfChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_IF_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArmChainLink(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_ARM_CHAINLINK);
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpression() const {
    return rq::getIsKeyword(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbol() const {
    return rq::getIsSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstant() const {
    return rq::getIsConstant(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInstruction() const {
    return rq::getIsOpcode(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeDefinition() const {
    return this->_kind == rq::EntityKind::SY_TYPE_DEFINITION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSimpleBuiltin() const {
    return rq::getIsSimpleBuiltinSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInference() const {
    return this->_kind == rq::EntityKind::SY_INFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericType() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_TYPE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSymbol() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_SYMBOL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVoid() const {
    return this->_kind == rq::EntityKind::SY_VOID;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNull() const {
    return this->_kind == rq::EntityKind::SY_NULL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNoReturn() const {
    return this->_kind == rq::EntityKind::SY_NO_RETURN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsVariadicArguments() const {
    return this->_kind == rq::EntityKind::SY_VARIADIC_ARGUMENTS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBoolean() const {
    return this->_kind == rq::EntityKind::SY_BOOLEAN;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericFloat() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_FLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHalf() const {
    return this->_kind == rq::EntityKind::SY_HALF;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSingle() const {
    return this->_kind == rq::EntityKind::SY_SINGLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDouble() const {
    return this->_kind == rq::EntityKind::SY_DOUBLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsQuadruple() const {
    return this->_kind == rq::EntityKind::SY_QUADRUPLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBinary() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_BINARY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericBfloat() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_BFLOAT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary16() const {
    return this->_kind == rq::EntityKind::SY_BINARY16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary32() const {
    return this->_kind == rq::EntityKind::SY_BINARY32;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary64() const {
    return this->_kind == rq::EntityKind::SY_BINARY64;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBinary128() const {
    return this->_kind == rq::EntityKind::SY_BINARY128;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsBfloat16() const {
    return this->_kind == rq::EntityKind::SY_BFLOAT16;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericInteger() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_INTEGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericSigned() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericUnsigned() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_UNSIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGenericCodeunit() const {
    return this->_kind == rq::EntityKind::SY_GENERIC_CODEUNIT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsAscii() const {
    return this->_kind == rq::EntityKind::SY_ASCII;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUtf8() const {
    return this->_kind == rq::EntityKind::SY_UTF8;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledBuiltin() const {
    return rq::getIsScaledBuiltinSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledSigned() const {
    return this->_kind == rq::EntityKind::SY_SCALED_SIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsScaledUnsigned() const {
    return this->_kind == rq::EntityKind::SY_SCALED_UNSIGNED;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnarySubtype() const {
    return rq::getIsUnarySubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRange() const {
    return this->_kind == rq::EntityKind::SY_RANGE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsReference() const {
    return this->_kind == rq::EntityKind::SY_REFERENCE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPointer() const {
    return this->_kind == rq::EntityKind::SY_POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFatPointer() const {
    return this->_kind == rq::EntityKind::SY_FAT_POINTER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInferencedCountArray() const {
    return this->_kind == rq::EntityKind::SY_INFERENCED_COUNT_ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCountedSubtype() const {
    return rq::getIsCountedSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArray() const {
    return this->_kind == rq::EntityKind::SY_ARRAY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCompositeSubtype() const {
    return rq::getIsCompositeSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayout() const {
    return this->_kind == rq::EntityKind::SY_LAYOUT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignature() const {
    return this->_kind == rq::EntityKind::SY_SIGNATURE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticSequence() const {
    return rq::getIsArithmeticSequenceSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsArithmeticInterval() const {
    return this->_kind == rq::EntityKind::SY_ARITHMETIC_INTERVAL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFiniteArithmeticProgression() const {
    return this->_kind == rq::EntityKind::SY_FINITE_ARITHMETIC_PROGRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getIsInfiniteArithmeticProgression() const {
    return this->_kind == rq::EntityKind::SY_INFINITE_ARITHMETIC_PROGRESSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsModule() const {
    return this->_kind == rq::EntityKind::SY_MODULE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsImport() const {
    return this->_kind == rq::EntityKind::SY_IMPORT;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFacade() const {
    return this->_kind == rq::EntityKind::SY_FACADE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMutation() const {
    return this->_kind == rq::EntityKind::SY_MUTATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtension() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDynamicVariable() const {
    return this->_kind == rq::EntityKind::SY_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsStaticVariable() const {
    return this->_kind == rq::EntityKind::SY_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumerator() const {
    return this->_kind == rq::EntityKind::SY_ENUMERATOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProperty() const {
    return this->_kind == rq::EntityKind::SY_PROPERTY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClassParameter() const {
    return this->_kind == rq::EntityKind::SY_CLASS_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLayoutParameter() const {
    return this->_kind == rq::EntityKind::SY_LAYOUT_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateParameter() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSignatureParameter() const {
    return this->_kind == rq::EntityKind::SY_SIGNATURE_PARAMETER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLabel() const {
    return this->_kind == rq::EntityKind::SY_LABEL;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolTable() const {
    return rq::getIsSymbolTableSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTop() const {
    return this->_kind == rq::EntityKind::SY_TOP;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTable() const {
    return this->_kind == rq::EntityKind::SY_TABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsClass() const {
    return this->_kind == rq::EntityKind::SY_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEnumeration() const {
    return this->_kind == rq::EntityKind::SY_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsProcedure() const {
    return rq::getIsProcedureSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEntry() const {
    return this->_kind == rq::EntityKind::SY_ENTRY;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFunction() const {
    return this->_kind == rq::EntityKind::SY_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsMethod() const {
    return this->_kind == rq::EntityKind::SY_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionFunction() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExtensionMethod() const {
    return this->_kind == rq::EntityKind::SY_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConstructor() const {
    return this->_kind == rq::EntityKind::SY_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDestructor() const {
    return this->_kind == rq::EntityKind::SY_DESTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsRanger() const {
    return this->_kind == rq::EntityKind::SY_RANGER;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplate() const {
    return rq::getIsTemplateSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateClass() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateEnumeration() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateDynamicVariable() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateStaticVariable() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateFunction() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateMethod() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateExtensionFunction() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateExtensionMethod() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTemplateConstructor() const {
    return this->_kind == rq::EntityKind::SY_TEMPLATE_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartial() const {
    return rq::getIsPartialSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialClass() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_CLASS;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialEnumeration() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_ENUMERATION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialDynamicVariable() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_DYNAMIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialStaticVariable() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_STATIC_VARIABLE;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialFunction() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialMethod() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialExtensionFunction() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_FUNCTION;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialExtensionMethod() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_EXTENSION_METHOD;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPartialConstructor() const {
    return this->_kind == rq::EntityKind::SY_PARTIAL_CONSTRUCTOR;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplateAlternative() const {
    return rq::getHasTemplateAlternativeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsType() const {
    return rq::getIsTypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsGeneric() const {
    return rq::getIsGenericSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConcrete() const {
    return rq::getIsConcreteSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSubtype() const {
    return rq::getIsSubtypeSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsPlatformChanging() const {
    return rq::getIsPlatformChangingSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInteger() const {
    return rq::getIsIntegerSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsFloat() const {
    return rq::getIsFloatSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCodeunit() const {
    return rq::getIsCodeunitSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSigned() const {
    return rq::getIsSignedSymbol(this->_kind);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUnsigned() const {
    return rq::getIsUnsignedSymbol(this->_kind);
  }
};

enum class Situation : std::uint_fast8_t {
  NONE,
  TRUNK,
  STATEMENT,
  LVALUE,
  RVALUE,
  REFLECTION,
  ARGUMENT,
  PARAMETER,
  BINDING,
  SYMBOL_PATH,
  ASCRIPTION,
  TYPE_ATTRIBUTE,
  EXPRESSION_ATTRIBUTE,
  ARITHMETIC_SEQUENCE_STAGE
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
getDescription(rq::Situation situation) {
  using namespace rq;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    return "no expression";
  case S::TRUNK:
    return "trunk expression";
  case S::STATEMENT:
    return "statement";
  case S::LVALUE:
    return "lvalue expression";
  case S::RVALUE:
    return "rvalue expression";
  case S::REFLECTION:
    return "reflection expression";
  case S::ARGUMENT:
    return "argument expression";
  case S::PARAMETER:
    return "parameter expression";
  case S::BINDING:
    return "binding expression";
  case S::SYMBOL_PATH:
    return "symbol path expression";
  case S::ASCRIPTION:
    return "ascription expression";
  case S::TYPE_ATTRIBUTE:
    return "type attribute";
  case S::EXPRESSION_ATTRIBUTE:
    return "symbol attribute";
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return "sequence stage expression";
  }
  return "error expression";
}

[[nodiscard]] inline rq::EntityKind
getExpandOfSituation(rq::Situation situation) {
  using namespace rq;
  using E = EntityKind;
  using S = Situation;
  switch (situation) {
  case S::NONE:
  case S::TRUNK:
    break;
  case S::STATEMENT:
    return E::KW_EXPAND_STATEMENT;
  case S::LVALUE:
    return E::KW_EXPAND_LVALUE;
  case S::RVALUE:
    return E::KW_EXPAND_RVALUE;
  case S::REFLECTION:
    return E::KW_EXPAND_REFLECTION;
  case S::ARGUMENT:
    return E::KW_EXPAND_ARGUMENT;
  case S::PARAMETER:
    return E::KW_EXPAND_PARAMETER;
  case S::BINDING:
    break;
  case S::SYMBOL_PATH:
    return E::KW_EXPAND_SYMBOL_PATH;
  case S::ASCRIPTION:
  case S::TYPE_ATTRIBUTE:
  case S::EXPRESSION_ATTRIBUTE:
    break;
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return E::KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation getSituationOfExpand(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  using S = Situation;
  switch (kind) {
  case E::KW_EXPAND:
    return S::NONE;
  case E::KW_EXPAND_STATEMENT:
    return S::STATEMENT;
  case E::KW_EXPAND_LVALUE:
    return S::LVALUE;
  case E::KW_EXPAND_RVALUE:
    return S::RVALUE;
  case E::KW_EXPAND_REFLECTION:
    return S::REFLECTION;
  case E::KW_EXPAND_ARGUMENT:
    return S::ARGUMENT;
  case E::KW_EXPAND_PARAMETER:
    return S::PARAMETER;
  case E::KW_EXPAND_SYMBOL_PATH:
    return S::SYMBOL_PATH;
  case E::KW_EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return S::ARITHMETIC_SEQUENCE_STAGE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::EntityKind getUniversalized(rq::EntityKind kind,
                                                     rq::Situation situation) {
  using namespace rq;
  using E = EntityKind;
  switch (kind) {
  // CASTS
  case E::KW_AS:
    return E::KW_LITERAL_AS;
  case E::KW_OF:
    return E::KW_ELEMENTS_OF;
  case E::KW_CAST:
    return E::KW_CAST_OF;
  case E::KW_DOWN_CAST:
    return E::KW_DOWN_CAST_OF;
  case E::KW_FLOAT_CAST:
    return E::KW_FLOAT_CAST_OF;
  case E::KW_PLATFORM_CAST:
    return E::KW_PLATFORM_CAST_OF;
  case E::KW_STATIC_CAST:
    return E::KW_STATIC_CAST_OF;
  case E::KW_BITWISE_CAST:
    return E::KW_BITWISE_CAST_OF;
  // MEMORY
  case E::KW_CONTENT:
    return E::KW_CONTENT_OF;
  case E::KW_ADDRESS:
    return E::KW_ADDRESS_OF;
  case E::KW_BORROW:
    return E::KW_BORROW_OF;
  case E::KW_DATA_ADDRESS:
    return E::KW_DATA_ADDRESS_OF;
  // PROCEDURES
  case E::KW_DESTROY:
    return E::KW_DESTROY_VALUE;
  case E::KW_DROP:
    return E::KW_DROP_VALUE;
  case E::KW_MOVE:
    return E::KW_MOVE_VALUE;
  // BUILTIN TYPES
  case E::KW_SIGNED:
    return E::KW_SIGNED_OF;
  case E::KW_UNSIGNED:
    return E::KW_UNSIGNED_OF;
  // VARIADIC ARGUMENTS
  case E::KW_FIRST_VARIADIC_ARGUMENT:
    return E::KW_FIRST_VARIADIC_ARGUMENT_OF;
  case E::KW_NEXT_VARIADIC_ARGUMENT:
    return E::KW_NEXT_VARIADIC_ARGUMENT_OF;
  // EXPANSIONS
  case E::KW_EXPAND:
    return rq::getExpandOfSituation(situation);
  // SYMBOL ATTRIBUTES
  case E::KW_CAPTURE:
    return E::KW_CAPTURE_OF;
  // REFLECTIONS
  case E::KW_BYTE_SIZE:
    return E::KW_BYTE_SIZE_OF;
  case E::KW_BIT_DEPTH:
    return E::KW_BIT_DEPTH_OF;
  case E::KW_ELEMENT_COUNT:
    return E::KW_ELEMENT_COUNT_OF;
  case E::KW_NAME:
    return E::KW_NAME_OF;
  case E::KW_LINE:
    return E::KW_LINE_OF;
  case E::KW_COLUMN:
    return E::KW_COLUMN_OF;
  case E::KW_IS:
    return E::KW_IS_TYPE;
  case E::KW_HOLDS:
    return E::KW_HOLDS_ENUMERATOR;
  case E::KW_TYPE:
    return E::KW_TYPE_OF;
  case E::KW_SYMBOL:
    return E::KW_SYMBOL_OF;
  case E::KW_HAS_MEMBER:
    return E::KW_HAS_MEMBER_OF;
  case E::KW_HAS:
    return E::KW_HAS_OF;
  case E::KW_GET:
    return E::KW_GET_OF;
  case E::KW_SIGNATURE:
    return E::KW_SIGNATURE_OF;
  case E::KW_LAYOUT:
    return E::KW_LAYOUT_OF;
  case E::KW_SYNONYM:
    return E::KW_SYNONYM_OF;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation getAttributeSituation(rq::EntityKind kind) {
  switch (kind) {
  case rq::EntityKind::KW_UNSITUATED_ASCRIBE_STATEMENT:
    [[fallthrough]];
  case rq::EntityKind::KW_ASCRIBE_STATEMENT:
    return rq::Situation::EXPRESSION_ATTRIBUTE;
  case rq::EntityKind::KW_UNSITUATED_ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::EntityKind::KW_ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::EntityKind::KW_ASCRIBE_ROOT_OF_VALUE:
    return rq::Situation::TYPE_ATTRIBUTE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone(rq::EntityKind kind) {
  return kind == rq::EntityKind::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTrunk(rq::EntityKind kind) {
  return kind == rq::EntityKind::KW_MODULE_TRUNK;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLValue(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRValue(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeReflection(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_REFLECTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_BINDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSymbolPath(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_SYMBOL_PATH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTypeAttribute(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_TYPE_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeExpressionAttribute(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_EXPRESSION_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStage(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasSome(flags,
                        rq::EntityFlags::KW_ARITHMETIC_SEQUENCE_CONDITION |
                            rq::EntityFlags::KW_ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceCondition(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags,
                       rq::EntityFlags::KW_ARITHMETIC_SEQUENCE_CONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStep(rq::EntityKind kind) {
  RQ_ASSERT_KEYWORD(kind);
  const rq::EntityFlags flags = rq::getFlags(kind);
  return rq::getHasAll(flags, rq::EntityFlags::KW_ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] inline bool getCanBeSituation(rq::EntityKind kind,
                                            rq::Situation situation) {
  switch (situation) {
  case rq::Situation::NONE:
    return rq::getIsNone(kind);
  case rq::Situation::TRUNK:
    return rq::getCanBeTrunk(kind);
  case rq::Situation::STATEMENT:
    return rq::getCanBeStatement(kind);
  case rq::Situation::LVALUE:
    return rq::getCanBeLValue(kind);
  case rq::Situation::RVALUE:
    return rq::getCanBeRValue(kind);
  case rq::Situation::REFLECTION:
    return rq::getCanBeReflection(kind);
  case rq::Situation::ARGUMENT:
    return rq::getCanBeArgument(kind);
  case rq::Situation::PARAMETER:
    return rq::getCanBeParameter(kind);
  case rq::Situation::BINDING:
    return rq::getCanBeBinding(kind);
  case rq::Situation::SYMBOL_PATH:
    return rq::getCanBeSymbolPath(kind);
  case rq::Situation::ASCRIPTION:
    return rq::getCanBeAscription(kind);
  case rq::Situation::TYPE_ATTRIBUTE:
    return rq::getCanBeTypeAttribute(kind);
  case rq::Situation::EXPRESSION_ATTRIBUTE:
    return rq::getCanBeExpressionAttribute(kind);
  case rq::Situation::ARITHMETIC_SEQUENCE_STAGE:
    return rq::getCanBeArithmeticSequenceStage(kind);
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName(rq::EntityKind kind) {
  return kind == rq::EntityKind::KW_IDENTIFY ||
         kind == rq::EntityKind::KW_IDENTIFIER_LITERAL;
}

enum class ChainKind : std::uint_fast8_t { NONE, UNKNOWN, IF, ARM };

[[nodiscard]] inline llvm::StringRef getDescription(rq::ChainKind chainKind) {
  using namespace rq;
  using CK = ChainKind;
  switch (chainKind) {
  case CK::NONE:
    return "no chain";
  case CK::UNKNOWN:
    return "unknown chain";
  case CK::IF:
    return "if chain";
  case CK::ARM:
    return "arm chain";
  }
  return "error chain";
}

[[nodiscard]] inline rq::ChainKind getChainKind(rq::EntityKind kind) {
  if (!rq::getCanBeChainLink(kind)) [[likely]] {
    return rq::ChainKind::NONE;
  } else if (rq::getCanBeAllChainLink(kind)) {
    return rq::ChainKind::UNKNOWN;
  } else if (rq::getCanBeIfChainLink(kind)) {
    return rq::ChainKind::IF;
  }
  RQ_UNREACHABLE();
}

enum class ExpressionAttribute : std::uint_fast8_t {
  NONE,
  OPAQUE,
  OUTSIDE,
  STATIC,
  CAPTURE,
  EAGER,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  MANGLE,
  PACK,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  EXPORT,
  PUBLIC,
  PROTECTED,
  MAY_COPY,
  MAY_MOVE,
  MUTATE_WITH,
};

[[nodiscard]] inline llvm::StringRef
getName(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using SA = ExpressionAttribute;
  switch (attribute) {
  case SA::NONE:
    return "none";
  case SA::OPAQUE:
    return "opaque";
  case SA::OUTSIDE:
    return "outside";
  case SA::STATIC:
    return "static";
  case SA::CAPTURE:
    return "capture";
  case SA::EAGER:
    return "eager";
  case SA::MAY_PARENT:
    return "may_parent";
  case SA::PARENT:
    return "parent";
  case SA::ABSTRACT:
    return "abstract";
  case SA::VIRTUAL:
    return "virtual";
  case SA::OVERRIDE:
    return "override";
  case SA::POSITION:
    return "position";
  case SA::MANGLE:
    return "mangle";
  case SA::PACK:
    return "pack";
  case SA::LABEL:
    return "label";
  case SA::TEMPLATE:
    return "template";
  case SA::LIKELY:
    return "likely";
  case SA::UNLIKELY:
    return "unlikely";
  case SA::DEPRECIATED:
    return "depreciated";
  case SA::EXPORT:
    return "export";
  case SA::PUBLIC:
    return "public";
  case SA::PROTECTED:
    return "protected";
  case SA::MAY_COPY:
    return "may_copy";
  case SA::MAY_MOVE:
    return "may_move";
  case SA::MUTATE_WITH:
    return "mutate_with";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionAttribute
getExpressionAttribute(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  using SA = ExpressionAttribute;
  switch (kind) {
  case E::KW_OPAQUE:
    return SA::OPAQUE;
  case E::KW_OUTSIDE:
    return SA::OUTSIDE;
  case E::KW_STATIC:
    return SA::STATIC;
  case E::KW_CAPTURE:
    return SA::CAPTURE;
  case E::KW_EAGER:
    return SA::EAGER;
  case E::KW_MAY_PARENT:
    return SA::MAY_PARENT;
  case E::KW_PARENT:
    return SA::PARENT;
  case E::KW_ABSTRACT:
    return SA::ABSTRACT;
  case E::KW_VIRTUAL:
    return SA::VIRTUAL;
  case E::KW_OVERRIDE:
    return SA::OVERRIDE;
  case E::KW_POSITION:
    return SA::POSITION;
  case E::KW_MANGLE:
    return SA::MANGLE;
  case E::KW_PACK:
    return SA::PACK;
  case E::KW_LABEL:
    return SA::LABEL;
  case E::KW_TEMPLATE:
    return SA::TEMPLATE;
  case E::KW_LIKELY:
    return SA::LIKELY;
  case E::KW_UNLIKELY:
    return SA::UNLIKELY;
  case E::KW_DEPRECIATED:
    return SA::DEPRECIATED;
  case E::KW_EXPORT:
    return SA::EXPORT;
  case E::KW_PUBLIC:
    return SA::PUBLIC;
  case E::KW_PROTECTED:
    return SA::PROTECTED;
  case E::KW_MAY_COPY:
    return SA::MAY_COPY;
  case E::KW_MAY_MOVE:
    return SA::MAY_MOVE;
  case E::KW_MUTATE_WITH:
    return SA::MUTATE_WITH;
  default:
    break;
  }
  return SA::NONE;
}

enum class ExpressionAttributeFlags : std::uint32_t {
  NONE = 0,
  OPAQUE = rq::getBit(31),
  OUTSIDE = rq::getBit(30),
  STATIC = rq::getBit(29),
  CAPTURE = rq::getBit(28),
  EAGER = rq::getBit(27),
  MAY_PARENT = rq::getBit(26),
  PARENT = rq::getBit(25),
  ABSTRACT = rq::getBit(24),
  VIRTUAL = rq::getBit(23),
  OVERRIDE = rq::getBit(22),
  POSITION = rq::getBit(21),
  MANGLE = rq::getBit(20),
  PACK = rq::getBit(19),
  LABEL = rq::getBit(18),
  TEMPLATE = rq::getBit(17),
  LIKELY = rq::getBit(16),
  UNLIKELY = rq::getBit(15),
  DEPRECIATED = rq::getBit(14),
  EXPORT = rq::getBit(13),
  PUBLIC = rq::getBit(12),
  PROTECTED = rq::getBit(11),
  MAY_COPY = rq::getBit(11),
  MAY_MOVE = rq::getBit(10),
  MUTATE_WITH = rq::getBit(9)
};

template <> struct is_flags<ExpressionAttributeFlags> : std::true_type {};

[[nodiscard]] inline rq::ExpressionAttributeFlags
getFlags(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using SA = ExpressionAttribute;
  using SF = ExpressionAttributeFlags;
  switch (attribute) {
  case SA::NONE:
    return SF::NONE;
  case SA::OPAQUE:
    return SF::OPAQUE;
  case SA::OUTSIDE:
    return SF::OUTSIDE;
  case SA::STATIC:
    return SF::STATIC;
  case SA::CAPTURE:
    return SF::CAPTURE;
  case SA::EAGER:
    return SF::EAGER;
  case SA::MAY_PARENT:
    return SF::MAY_PARENT;
  case SA::PARENT:
    return SF::PARENT;
  case SA::ABSTRACT:
    return SF::ABSTRACT;
  case SA::VIRTUAL:
    return SF::VIRTUAL;
  case SA::OVERRIDE:
    return SF::OVERRIDE;
  case SA::POSITION:
    return SF::POSITION;
  case SA::MANGLE:
    return SF::POSITION;
  case SA::PACK:
    return SF::PACK;
  case SA::LABEL:
    return SF::LABEL;
  case SA::TEMPLATE:
    return SF::TEMPLATE;
  case SA::LIKELY:
    return SF::LIKELY;
  case SA::UNLIKELY:
    return SF::UNLIKELY;
  case SA::DEPRECIATED:
    return SF::DEPRECIATED;
  case SA::EXPORT:
    return SF::EXPORT;
  case SA::PUBLIC:
    return SF::PUBLIC;
  case SA::PROTECTED:
    return SF::PROTECTED;
  case SA::MAY_COPY:
    return SF::MAY_COPY;
  case SA::MAY_MOVE:
    return SF::MAY_MOVE;
  case SA::MUTATE_WITH:
    return SF::MUTATE_WITH;
  }
  return SF::NONE;
}

[[nodiscard]] inline bool getHasOpaque(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::OPAQUE);
}

[[nodiscard]] inline bool getHasOutside(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::OUTSIDE);
}

[[nodiscard]] inline bool getHasStatic(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::STATIC);
}

[[nodiscard]] inline bool getHasCapture(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::CAPTURE);
}

[[nodiscard]] inline bool getHasEager(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::EAGER);
}

[[nodiscard]] inline bool getHasMayParent(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::MAY_PARENT);
}

[[nodiscard]] inline bool getHasParent(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::PARENT);
}

[[nodiscard]] inline bool getHasAbstract(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::ABSTRACT);
}

[[nodiscard]] inline bool getHasVirtual(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::VIRTUAL);
}

[[nodiscard]] inline bool getHasOverride(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::OVERRIDE);
}

[[nodiscard]] inline bool getHasPosition(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::POSITION);
}

[[nodiscard]] inline bool getHasMangle(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::MANGLE);
}

[[nodiscard]] inline bool getHasPack(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::PACK);
}

[[nodiscard]] inline bool getHasLabel(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::LABEL);
}

[[nodiscard]] inline bool getHasTemplate(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::TEMPLATE);
}

[[nodiscard]] inline bool getHasLikely(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::LIKELY);
}

[[nodiscard]] inline bool getHasUnlikely(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::UNLIKELY);
}

[[nodiscard]] inline bool
getHasDepreciated(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::DEPRECIATED);
}

[[nodiscard]] inline bool getHasExport(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::EXPORT);
}

[[nodiscard]] inline bool getHasPublic(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::PUBLIC);
}

[[nodiscard]] inline bool getHasProtected(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::PROTECTED);
}

[[nodiscard]] inline bool getHasMayCopy(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::MAY_COPY);
}

[[nodiscard]] inline bool getHasMayMove(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::MAY_MOVE);
}

[[nodiscard]] inline bool getHasMutateWith(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::MUTATE_WITH);
}

[[nodiscard]] inline bool getHasAttribute(rq::ExpressionAttributeFlags flags,
                                          rq::ExpressionAttribute attribute) {
  rq::ExpressionAttributeFlags attribute_flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, attribute_flags);
}

struct Expression;

struct ExpressionAttributeFlagsFactory final {
  using Self = rq::ExpressionAttributeFlagsFactory;

  rq::ExpressionAttributeFlags _flags{};
  const rq::Expression *_capture_ptr{nullptr};
  const rq::Expression *_override_ptr{nullptr};
  const rq::Expression *_position_ptr{nullptr};
  const rq::Expression *_mangle_ptr{nullptr};
  const rq::Expression *_label_ptr{nullptr};
  const rq::Expression *_template_ptr{nullptr};
  const rq::Expression *_depreciated_ptr{nullptr};
  const rq::Expression *_mutate_with_ptr{nullptr};

  ExpressionAttributeFlagsFactory() = default;
  inline void addAttribute(const rq::Expression &expression);
  inline void addAllAttributres(const rq::Expression &ascribed);
  [[nodiscard]] rq::ExpressionAttributeFlags getFlags() const {
    return this->_flags;
  }
  [[nodiscard]] bool getHasOpaque() const {
    return rq::getHasOpaque(this->_flags);
  }
  [[nodiscard]] bool getHasOutside() const {
    return rq::getHasOutside(this->_flags);
  }
  [[nodiscard]] bool getHasStatic() const {
    return rq::getHasStatic(this->_flags);
  }
  [[nodiscard]] bool getHasCapture() const {
    return rq::getHasCapture(this->_flags);
  }
  [[nodiscard]] bool getHasEager() const {
    return rq::getHasEager(this->_flags);
  }
  [[nodiscard]] bool getHasMayParent() const {
    return rq::getHasMayParent(this->_flags);
  }
  [[nodiscard]] bool getHasParent() const {
    return rq::getHasParent(this->_flags);
  }
  [[nodiscard]] bool getHasAbstract() const {
    return rq::getHasAbstract(this->_flags);
  }
  [[nodiscard]] bool getHasVirtual() const {
    return rq::getHasVirtual(this->_flags);
  }
  [[nodiscard]] bool getHasOverride() const {
    return rq::getHasOverride(this->_flags);
  }
  [[nodiscard]] bool getHasPosition() const {
    return rq::getHasPosition(this->_flags);
  }
  [[nodiscard]] bool getHasMangle() const {
    return rq::getHasMangle(this->_flags);
  }
  [[nodiscard]] bool getHasPack() const { return rq::getHasPack(this->_flags); }
  [[nodiscard]] bool getHasLabel() const {
    return rq::getHasLabel(this->_flags);
  }
  [[nodiscard]] bool getHasTemplate() const {
    return rq::getHasTemplate(this->_flags);
  }
  [[nodiscard]] bool getHasLikely() const {
    return rq::getHasLikely(this->_flags);
  }
  [[nodiscard]] bool getHasUnlikely() const {
    return rq::getHasUnlikely(this->_flags);
  }
  [[nodiscard]] bool getHasDepreciated() const {
    return rq::getHasDepreciated(this->_flags);
  }
  [[nodiscard]] bool getHasExport() const {
    return rq::getHasExport(this->_flags);
  }
  [[nodiscard]] bool getHasPublic() const {
    return rq::getHasPublic(this->_flags);
  }
  [[nodiscard]] bool getHasProtected() const {
    return rq::getHasProtected(this->_flags);
  }
  [[nodiscard]] bool getHasMayCopy() const {
    return rq::getHasMayCopy(this->_flags);
  }
  [[nodiscard]] bool getHasMayMove() const {
    return rq::getHasMayMove(this->_flags);
  }
  [[nodiscard]] bool getHasMutateWith() const {
    return rq::getHasMutateWith(this->_flags);
  }
  [[nodiscard]] const rq::Expression &getCapture() const {
    RQ_ASSERT(this->getHasCapture(), "no capture");
    return rq::dereferencePtr(this->_capture_ptr);
  }
  [[nodiscard]] const rq::Expression &getOverride() const {
    RQ_ASSERT(this->getHasOverride(), "no override");
    return rq::dereferencePtr(this->_override_ptr);
  }
  [[nodiscard]] const rq::Expression &getPosition() const {
    RQ_ASSERT(this->getHasPosition(), "no position");
    return rq::dereferencePtr(this->_position_ptr);
  }
  [[nodiscard]] const rq::Expression &getMangle() const {
    RQ_ASSERT(this->getHasMangle(), "no mangle");
    return rq::dereferencePtr(this->_mangle_ptr);
  }
  [[nodiscard]] const rq::Expression &getLabel() const {
    RQ_ASSERT(this->getHasLabel(), "no label");
    return rq::dereferencePtr(this->_label_ptr);
  }
  [[nodiscard]] const rq::Expression &getTemplate() const {
    RQ_ASSERT(this->getHasTemplate(), "no template");
    return rq::dereferencePtr(this->_template_ptr);
  }
  [[nodiscard]] const rq::Expression &getDepreciated() const {
    RQ_ASSERT(this->getHasDepreciated(), "no depreciated");
    return rq::dereferencePtr(this->_depreciated_ptr);
  }
  [[nodiscard]] const rq::Expression &getMutateWith() const {
    RQ_ASSERT(this->getHasMutateWith(), "no mutate_with");
    return rq::dereferencePtr(this->_mutate_with_ptr);
  }
};

enum class TypeAttribute : std::uint_fast8_t {
  NONE,
  MUTABLE,
  CONSTANT,
  PARTIALLY_MUTABLE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  MAY_DISCARD,
  DEBUG_TRAP_ON_PANIC
};

[[nodiscard]] inline llvm::StringRef getName(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  switch (attribute) {
  case TA::NONE:
    return "none";
  case TA::MUTABLE:
    return "mutable";
  case TA::CONSTANT:
    return "constant";
  case TA::PARTIALLY_MUTABLE:
    return "partially_mutable";
  case TA::VOLATILE:
    return "volatile";
  case TA::ATOMIC:
    return "atomic";
  case TA::NULL_TERMINATED:
    return "null_terminated";
  case TA::MAY_DISCARD:
    return "may_discard";
  case TA::DEBUG_TRAP_ON_PANIC:
    return "debug_trap_on_panic";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::TypeAttribute getTypeAttribute(rq::EntityKind kind) {
  using namespace rq;
  using E = EntityKind;
  using TA = TypeAttribute;
  switch (kind) {
  case E::KW_MUTABLE:
    return TA::MUTABLE;
  case E::KW_CONSTANT:
    return TA::CONSTANT;
  case E::KW_PARTIALLY_MUTABLE:
    return TA::PARTIALLY_MUTABLE;
  case E::KW_VOLATILE:
    return TA::VOLATILE;
  case E::KW_ATOMIC:
    return TA::ATOMIC;
  case E::KW_NULL_TERMINATED:
    return TA::NULL_TERMINATED;
  case E::KW_MAY_DISCARD:
    return TA::MAY_DISCARD;
  case E::KW_DEBUG_TRAP_ON_PANIC:
    return TA::DEBUG_TRAP_ON_PANIC;
  default:
    break;
  }
  return TA::NONE;
}

enum class TypeAttributeFlags : std::uint32_t {
  NONE = 0,
  MUTABLE = rq::getBit(15),
  CONSTANT = rq::getBit(14),
  PARTIALLY_MUTABLE = rq::getBit(13),
  VOLATILE = rq::getBit(12),
  ATOMIC = rq::getBit(11),
  NULL_TERMINATED = rq::getBit(10),
  MAY_DISCARD = rq::getBit(9),
  DEBUG_TRAP_ON_PANIC = rq::getBit(8),
  MUTATION_MASK = 0xFFFF
};

template <> struct is_flags<TypeAttributeFlags> : std::true_type {};

enum class MutationFlags : std::uint16_t {
  NONE = 0,
  INDEPENDENT_CLASS = rq::getBit(0),
  USER_CLASS_MASK = 0xFFFE
};

template <> struct is_flags<MutationFlags> : std::true_type {};

static constexpr unsigned MAX_MUTATION_COUNT = 16;

[[nodiscard]] inline rq::TypeAttributeFlags
getFlags(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  using TF = TypeAttributeFlags;
  switch (attribute) {
  case TA::NONE:
    return TF::NONE;
  case TA::MUTABLE:
    return TF::MUTABLE;
  case TA::CONSTANT:
    return TF::CONSTANT;
  case TA::PARTIALLY_MUTABLE:
    return TF::PARTIALLY_MUTABLE;
  case TA::VOLATILE:
    return TF::VOLATILE;
  case TA::ATOMIC:
    return TF::ATOMIC;
  case TA::NULL_TERMINATED:
    return TF::NULL_TERMINATED;
  case TA::MAY_DISCARD:
    return TF::MAY_DISCARD;
  case TA::DEBUG_TRAP_ON_PANIC:
    return TF::DEBUG_TRAP_ON_PANIC;
  }
  return TF::NONE;
}

[[nodiscard]] inline bool getHasMutable(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::MUTABLE);
}

[[nodiscard]] inline bool getHasConstant(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::CONSTANT);
}

[[nodiscard]] inline bool getHasPartiallyMutable(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::PARTIALLY_MUTABLE);
}

[[nodiscard]] inline bool getHasVolatile(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::VOLATILE);
}

[[nodiscard]] inline bool getHasAtomic(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::ATOMIC);
}

[[nodiscard]] inline bool getHasNullTerminated(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::NULL_TERMINATED);
}

[[nodiscard]] inline bool getHasMayDiscard(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::MAY_DISCARD);
}

[[nodiscard]] inline bool getHasDebugTrapOnPanic(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::DEBUG_TRAP_ON_PANIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAttribute(rq::TypeAttributeFlags flags, rq::TypeAttribute attribute) {
  return rq::getHasAll(flags, rq::getFlags(attribute));
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasMutability(rq::TypeAttributeFlags flags) {
  return rq::getHasSome(flags, rq::TypeAttributeFlags::MUTABLE |
                                   rq::TypeAttributeFlags::CONSTANT |
                                   rq::TypeAttributeFlags::PARTIALLY_MUTABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE rq::MutationFlags
getMutationFlags(rq::TypeAttributeFlags flags) {
  RQ_ASSERT(rq::getHasAll(flags, rq::TypeAttributeFlags::PARTIALLY_MUTABLE),
            "not partially mutable");
  return static_cast<rq::MutationFlags>(
      rq::getMaskValue(flags, rq::TypeAttributeFlags::MUTATION_MASK));
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsValidMutability(rq::TypeAttributeFlags flags) {
  if (rq::getHasMutability(flags)) {
    unsigned mutability_count = 0;
    if (rq::getHasAll(flags, rq::TypeAttributeFlags::MUTABLE)) {
      mutability_count++;
    }
    if (rq::getHasAll(flags, rq::TypeAttributeFlags::CONSTANT)) {
      mutability_count++;
    }
    if (rq::getHasAll(flags, rq::TypeAttributeFlags::PARTIALLY_MUTABLE)) {
      mutability_count++;
    }
    if (mutability_count != 1) {
      return false;
    }
  }
  return true;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsValidMutabilityClass(rq::TypeAttributeFlags flags) {
  if (!rq::getHasAll(flags, rq::TypeAttributeFlags::PARTIALLY_MUTABLE)) {
    rq::MutationFlags classes = rq::getMutationFlags(flags);
    if (classes != rq::MutationFlags::NONE) {
      return false;
    }
  }
  return true;
}

enum class ArithmeticSequenceStep : std::uint_fast8_t {
  NONE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::ArithmeticSequenceStep step) {
  using namespace rq;
  using ASS = ArithmeticSequenceStep;
  switch (step) {
  case ASS::NONE:
    return "no step";
  case ASS::ADD:
    return "add step";
  case ASS::SUBTRACT:
    return "subtract step";
  case ASS::MULTIPLY:
    return "multiply step";
  case ASS::DIVIDE:
    return "divide step";
  case ASS::MODULUS:
    return "modulus step";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ArithmeticSequenceStep
getArithmeticSequenceStep(rq::EntityKind kind) {
  using namespace rq;
  using ASS = ArithmeticSequenceStep;
  using E = EntityKind;
  switch (kind) {
  case E::KW_ARITHMETIC_SEQUENCE_STEP_ADD:
    return ASS::ADD;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return ASS::SUBTRACT;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return ASS::MULTIPLY;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return ASS::DIVIDE;
  case E::KW_ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return ASS::MODULUS;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class ArithmeticSequenceCondition : std::uint_fast8_t {
  NONE,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  EQUAL,
  NOT_EQUAL
};

[[nodiscard]] inline llvm::StringRef
getDescription(rq::ArithmeticSequenceCondition condition) {
  using namespace rq;
  using ASC = ArithmeticSequenceCondition;
  switch (condition) {
  case ASC::NONE:
    return "no condition";
  case ASC::LESS:
    return "less condition";
  case ASC::LESS_EQUAL:
    return "less equal condition";
  case ASC::GREATER:
    return "greater condition";
  case ASC::GREATER_EQUAL:
    return "greater equal condition";
  case ASC::EQUAL:
    return "equal condition";
  case ASC::NOT_EQUAL:
    return "not equal condition";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ArithmeticSequenceCondition
getArithmeticSequenceCondition(rq::EntityKind kind) {
  using namespace rq;
  using ASC = ArithmeticSequenceCondition;
  using E = EntityKind;
  switch (kind) {
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return ASC::LESS;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return ASC::LESS_EQUAL;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return ASC::GREATER;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return ASC::GREATER_EQUAL;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return ASC::EQUAL;
  case E::KW_ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return ASC::NOT_EQUAL;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

template <typename SourceAParam, typename SourceBParam>
[[nodiscard]] inline unsigned getSourceLengthBetween(const SourceAParam &first,
                                                     const SourceBParam &last) {
  RQ_ASSERT(first.getSourceTextPtr() != nullptr, "first must have source");
  RQ_ASSERT(last.getSourceTextPtr() != nullptr, "last must have source");
  RQ_ASSERT(first.getSourceTextPtr() <= last.getSourceTextPtr(),
            "last source must start after first source");
  return static_cast<unsigned>(last.getSourceTextPtr() -
                               first.getSourceTextPtr()) +
         last.getSourceTextLength();
}

struct Expression;

struct ExpressionIterator final {
  using Self = rq::ExpressionIterator;
  using value_type = rq::Expression;
  using reference = rq::Expression &;
  using pointer = rq::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  rq::Expression *_expression_ptr = nullptr;

  ExpressionIterator() = default;
  ExpressionIterator(rq::Expression *expression_ptr)
      : _expression_ptr(expression_ptr) {}
  ExpressionIterator(const Self &) = default;
  ExpressionIterator(Self &&) = default;
  ~ExpressionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_expression_ptr == it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_expression_ptr != it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &operator*() {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &operator*() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *operator->() {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *operator->() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_expression_ptr == nullptr;
  }
};

struct ConstExpressionIterator final {
  using Self = rq::ConstExpressionIterator;
  using value_type = const rq::Expression;
  using reference = const rq::Expression &;
  using pointer = rq::Expression *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  const rq::Expression *_expression_ptr = nullptr;

  ConstExpressionIterator() = default;
  ConstExpressionIterator(const rq::Expression *expression_ptr)
      : _expression_ptr(expression_ptr) {}
  ConstExpressionIterator(const Self &) = default;
  ConstExpressionIterator(Self &&) = default;
  ~ConstExpressionIterator() = default;
  Self &operator=(const Self &) = default;
  Self &operator=(Self &&) = default;
  RQ_ALWAYS_INLINE Self &operator++();
  RQ_ALWAYS_INLINE Self operator++(int);
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &it) const {
    return this->_expression_ptr == it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &it) const {
    return this->_expression_ptr != it._expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &operator*() const {
    return rq::dereferencePtr(this->_expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression *operator->() const {
    return this->_expression_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDone() const {
    return this->_expression_ptr == nullptr;
  }
};

enum class ExpressionNextFlags : std::uint8_t {
  NONE = 0,
  // NOTE: a "chain-link" expression is a statement that is linked to the next
  // and not seperated with semicolons. Used in things like if->else_if->else
  // chains.
  CHAINLINK = rq::getBit(0),
  // NOTE: a "header" expression is one that terminates with a comma in a
  // semicolon terminating context (usually before all statements)
  HEADER = rq::getBit(1)
};

template <> struct is_flags<ExpressionNextFlags> : std::true_type {};

enum class ExpressionSourceFlags : std::uint8_t {
  NONE = 0,
  // NOTE: an "inserted" node is one that was not present in the source
  // and was inserted into the AST by the compiler
  INSERTED = rq::getBit(0),
  // NOTE: this flag is set if there is an error with situating this node
  SITUATOR_ERROR = rq::getBit(1)
};

template <> struct is_flags<ExpressionSourceFlags> : std::true_type {};

struct Expression final : public rq::Entity {
  using Self = rq::Expression;

  rq::PtrWithFlags<rq::Expression, 2, rq::ExpressionNextFlags>
      _next_ptr_flags{};
  rq::Expression *_branch_ptr{nullptr};
  rq::PtrWithFlags<const char, 1, rq::ExpressionSourceFlags>
      _source_ptr_flags{};
  unsigned _source_text_length{0};

  Expression() : rq::Entity(rq::EntityKind::KW_NONE) {}
  Expression(Self &) = delete;
  Expression(Self &&) = delete;
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;
  ~Expression() = default;
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator==(const Self &rhs) const {
    return this == &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool operator!=(const Self &rhs) const {
    return this != &rhs;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getKeyword() const {
    return this->getKind();
  }
  RQ_ALWAYS_INLINE void clear() {
    this->_kind = rq::EntityKind::KW_NONE;
    this->_next_ptr_flags = {};
    this->_branch_ptr = nullptr;
    this->_source_ptr_flags = {};
    this->_source_text_length = 0;
  }
  RQ_ALWAYS_INLINE void setKeyword(rq::EntityKind kind) {
    RQ_ASSERT(this->_kind == rq::EntityKind::KW_NONE,
              "keyword must not already be set");
    RQ_ASSERT_KEYWORD(kind);
    this->_kind = kind;
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::EntityKind kind) {
    RQ_ASSERT(this->_kind != rq::EntityKind::KW_NONE,
              "keyword must already be set");
    RQ_ASSERT_KEYWORD(kind);
    this->_kind = kind;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterMark() const {
    return rq::getIsParameterMarkKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return rq::getIsLiteralKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft() const {
    return rq::getHasUnquotedLeft(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight() const {
    return rq::getHasUnquotedRight(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging() const {
    return rq::getIsConverging(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal() const {
    return rq::getIsInternal(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatementBranches() const {
    return rq::getHasStatementBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasExpressionBranches() const {
    return rq::getHasExpressionBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind getSituatedAscribe() const {
    return rq::getSituatedAscribeKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Situation getAttributeSituation() const {
    return rq::getAttributeSituation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ChainKind getChainKind() const {
    return rq::getChainKind(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink() const {
    return rq::getCanBeChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStartingChainLink() const {
    return rq::getCanBeStartingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeContinuingChainLink() const {
    return rq::getCanBeContinuingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeFinishingChainLink() const {
    return rq::getCanBeFinishingChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAllChainLink() const {
    return rq::getCanBeAllChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeIfChainLink() const {
    return rq::getCanBeIfChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArmChainLink() const {
    return rq::getCanBeArmChainLink(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::EntityKind
  getUniversalized(rq::Situation situation) const {
    return rq::getUniversalized(this->getKeyword(), situation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsUniversalizable() const {
    return rq::getIsUniversalizable(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone() const {
    return rq::getIsNone(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTrunk() const {
    return rq::getCanBeTrunk(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement() const {
    return rq::getCanBeStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRValue() const {
    return rq::getCanBeRValue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLValue() const {
    return rq::getCanBeLValue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeReflection() const {
    return rq::getCanBeReflection(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument() const {
    return rq::getCanBeArgument(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter() const {
    return rq::getCanBeParameter(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding() const {
    return rq::getCanBeBinding(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSymbolPath() const {
    return rq::getCanBeSymbolPath(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription() const {
    return rq::getCanBeAscription(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTypeAttribute() const {
    return rq::getCanBeTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeExpressionAttribute() const {
    return rq::getCanBeExpressionAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticSequenceStage() const {
    return rq::getCanBeArithmeticSequenceStage(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeArithmeticSequenceCondition() const {
    return rq::getCanBeArithmeticSequenceCondition(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArithmeticSequenceStep() const {
    return rq::getCanBeArithmeticSequenceStep(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeSituation(rq::Situation situation) const {
    return rq::getCanBeSituation(this->getKeyword(), situation);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName() const {
    return rq::getIsEvaluatableName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::ExpressionAttribute
  getExpressionAttribute() const {
    return rq::getExpressionAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::TypeAttribute getTypeAttribute() const {
    return rq::getTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInserted() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceFlags::INSERTED);
  }
  RQ_ALWAYS_INLINE void setIsInserted() {
    this->_source_ptr_flags.addFlags(rq::ExpressionSourceFlags::INSERTED);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSituatorError() const {
    return rq::getHasAll(this->_source_ptr_flags.getFlags(),
                         rq::ExpressionSourceFlags::SITUATOR_ERROR);
  }
  RQ_ALWAYS_INLINE void setHasSituatorError() {
    this->_source_ptr_flags.addFlags(rq::ExpressionSourceFlags::SITUATOR_ERROR);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsHeader() const {
    return rq::getHasAll(this->_next_ptr_flags.getFlags(),
                         rq::ExpressionNextFlags::HEADER);
  }
  void RQ_ALWAYS_INLINE setIsHeader() {
    this->_next_ptr_flags.addFlags(rq::ExpressionNextFlags::HEADER);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsChainLink() const {
    return rq::getHasAll(this->_next_ptr_flags.getFlags(),
                         rq::ExpressionNextFlags::CHAINLINK);
  }
  void RQ_ALWAYS_INLINE setIsChainLink() {
    this->_next_ptr_flags.addFlags(rq::ExpressionNextFlags::CHAINLINK);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_source_ptr_flags.getPtr() != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return llvm::StringRef(this->_source_ptr_flags.getPtr(),
                           this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getSourceTextPtr() const {
    return this->_source_ptr_flags.getPtr();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getSourceTextLength() const {
    return this->_source_text_length;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceBegin() const {
    return llvm::SMLoc::getFromPointer(this->_source_ptr_flags.getPtr());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceEnd() const {
    return llvm::SMLoc::getFromPointer(this->_source_ptr_flags.getPtr() +
                                       this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMRange getLlvmSourceRange() const {
    return llvm::SMRange(this->getLlvmSourceBegin(), this->getLlvmSourceEnd());
  }
  RQ_ALWAYS_INLINE void setSource(llvm::StringRef source) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    RQ_ASSERT(!source.empty(), "source text is empty");
    this->_source_ptr_flags.setPtr(source.data());
    this->_source_text_length = static_cast<unsigned>(source.size());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSource(const SourceParam &source) {
    this->setSource(source.getSourceText());
  }
  template <typename FirstSourceParam, typename LastSourceParam>
  inline void setSource(const FirstSourceParam &first,
                        const LastSourceParam &last) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    rq::assignSingleValue(this->_source_ptr_flags.getPtr(),
                          first.getSourceTextPtr());
    this->_source_text_length = rq::getSourceLengthBetween(first, last);
  }
  template <typename SourceParam>
  inline void extendSourceOver(const SourceParam &source) {
    RQ_ASSERT(this->getHasSourceText(), "expression source not set");
    if (source.getSourceTextPtr() >= this->_source_ptr_flags.getPtr()) {
      this->_source_text_length = rq::getSourceLengthBetween(*this, source);
    } else {
      this->_source_ptr_flags.setPtr(source.getSourceTextPtr());
      this->_source_text_length = rq::getSourceLengthBetween(source, *this);
    }
  }
  RQ_ALWAYS_INLINE const char *getBeforeSourceTextPtr() const {
    return this->getSourceTextPtr();
  }
  RQ_ALWAYS_INLINE const char *getAfterSourceTextPtr() const {
    return this->getSourceTextPtr() + this->getSourceTextLength();
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getEndSourceTextPtr() const {
    if (this->getSourceTextLength() == 0) {
      return this->getSourceTextPtr();
    }
    return this->getSourceTextPtr() + this->getSourceTextLength() - 1;
  }
  RQ_ALWAYS_INLINE void setSourceAt(const char *source_ptr) {
    rq::assignSingleValue(this->_source_ptr_flags.getPtr(), source_ptr);
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceBefore(const SourceParam &source) {
    this->setSourceAt(source.getBeforeSourceTextPtr());
  }
  template <typename SourceParam>
  inline void setSourceAfter(const SourceParam &source) {
    this->setSourceAt(source.getAfterSourceTextPtr());
  }
  template <typename SourceParam>
  inline void setSourceAtEnd(const SourceParam &source) {
    this->setSourceAt(source.getEndSourceTextPtr());
  }
  // NOTE: no getBranchCount and getNextCount because bad performance!
  RQ_ALWAYS_INLINE rq::Expression &getBranch() {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE const rq::Expression &getBranch() const {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE rq::Expression &getNext() {
    return rq::dereferencePtr(this->_next_ptr_flags.getPtr());
  }
  RQ_ALWAYS_INLINE const rq::Expression &getNext() const {
    return rq::dereferencePtr(this->_next_ptr_flags.getPtr());
  }
  RQ_ALWAYS_INLINE rq::Expression *getBranchPtr() { return this->_branch_ptr; }
  RQ_ALWAYS_INLINE const rq::Expression *getBranchPtr() const {
    return this->_branch_ptr;
  }
  RQ_ALWAYS_INLINE rq::Expression *getNextPtr() {
    return this->_next_ptr_flags.getPtr();
  }
  RQ_ALWAYS_INLINE const rq::Expression *getNextPtr() const {
    return this->_next_ptr_flags.getPtr();
  }
  // TODO: get rid of the getLastNext and getLastBranch functions to avoid
  // excessive pointer chasing
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastNext() {
    rq::Expression *expression_ptr = this;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastNext() const {
    const rq::Expression *expression_ptr = this;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastBranch() {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    rq::Expression *expression_ptr = this->_branch_ptr;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastBranch() const {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    rq::Expression *expression_ptr = this->_branch_ptr;
    while (expression_ptr->getNextPtr() != nullptr) {
      expression_ptr = expression_ptr->getNextPtr();
    }
    return rq::dereferencePtr(expression_ptr);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression &branch) {
    rq::assignSingleValue(this->_branch_ptr, &branch);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression &next) {
    rq::assignSingleValue(this->_next_ptr_flags.getPtr(), &next);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression *branch_ptr) {
    rq::assignSingleValue(this->_branch_ptr, branch_ptr);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression *next_ptr) {
    rq::assignSingleValue(this->_next_ptr_flags.getPtr(), next_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceBranch(rq::Expression &branch) {
    rq::Expression &replaced_branch = this->getBranch();
    this->_branch_ptr = &branch;
    return replaced_branch;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceNext(rq::Expression &next) {
    rq::Expression &replaced_next = this->getNext();
    this->_next_ptr_flags.setPtr(&next);
    return replaced_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceBranch(rq::Expression *branch_ptr) {
    rq::Expression &replaced_branch = this->getBranch();
    this->_branch_ptr = branch_ptr;
    return replaced_branch;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &
  replaceNext(rq::Expression *next_ptr) {
    rq::Expression &replaced_next = this->getNext();
    this->_next_ptr_flags.setPtr(next_ptr);
    return replaced_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceBranchPtr(rq::Expression &branch) {
    rq::Expression *replaced_branch = this->_branch_ptr;
    this->_branch_ptr = &branch;
    return replaced_branch;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceNextPtr(rq::Expression &next) {
    rq::Expression *replaced_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(&next);
    return replaced_next_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceBranchPtr(rq::Expression *branch_ptr) {
    rq::Expression *replaced_branch = this->_branch_ptr;
    this->_branch_ptr = branch_ptr;
    return replaced_branch;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceNextPtr(rq::Expression *next_ptr) {
    rq::Expression *replaced_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(next_ptr);
    return replaced_next_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasBranch() const {
    return this->_branch_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNext() const {
    return this->_next_ptr_flags.getPtr() != nullptr;
  }
  [[nodiscard]] inline rq::Expression &popBranch() {
    RQ_ASSERT(this->_branch_ptr != nullptr, "does not have branch");
    rq::Expression *old_branch_ptr = this->_branch_ptr;
    this->_branch_ptr = nullptr;
    return rq::dereferencePtr(old_branch_ptr);
  }
  [[nodiscard]] inline rq::Expression *popBranchPtr() {
    rq::Expression *old_branch_ptr = this->_branch_ptr;
    this->_branch_ptr = nullptr;
    return old_branch_ptr;
  }
  [[nodiscard]] inline rq::Expression &popNext() {
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(nullptr);
    return rq::dereferencePtr(old_next_ptr);
  }
  [[nodiscard]] inline rq::Expression *popNextPtr() {
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(nullptr);
    return old_next_ptr;
  }
  [[nodiscard]] inline rq::Expression &changeNext(rq::Expression &new_next) {
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(&new_next);
    return rq::dereferencePtr(old_next_ptr);
  }
  [[nodiscard]] inline rq::Expression &
  changeNext(rq::Expression *new_next_ptr) {
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(new_next_ptr);
    return rq::dereferencePtr(old_next_ptr);
  }
  [[nodiscard]] inline rq::Expression *changeNextPtr(rq::Expression &new_next) {
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(&new_next);
    return old_next_ptr;
  }
  [[nodiscard]] inline rq::Expression *
  changeNextPtr(rq::Expression *new_next_ptr) {
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(new_next_ptr);
    return old_next_ptr;
  }
  [[nodiscard]] inline rq::Expression &mergeAndPopBranch() {
    rq::Expression &branch = this->popBranch();
    if (this->getHasNext()) {
      rq::Expression &branch_last_next = branch.getLastNext();
      branch_last_next.setNext(this->popNext());
    }
    this->clear();
    this->setKeyword(branch.getKeyword());
    if (branch.getHasBranch()) {
      this->setBranch(branch.popBranch());
    }
    if (branch.getHasNext()) {
      this->setNext(branch.popNext());
    }
    this->setSource(branch);
    branch.clear();
    return branch;
  }
  [[nodiscard]] inline rq::Expression &mergeAndPopNext() {
    rq::Expression &next = this->popNext();
    RQ_ASSERT(!this->getHasBranch(), "has branch");
    this->clear();
    this->setKeyword(next.getKeyword());
    if (next.getHasBranch()) {
      this->setBranch(next.popBranch());
    }
    if (next.getHasNext()) {
      this->setNext(next.popNext());
    }
    this->setSource(next);
    next.clear();
    return next;
  }
  [[nodiscard]] inline rq::Expression &getUnascribed() {
    if (this->getCanBeAscription()) {
      return this->getBranch().getUnascribed();
    }
    return *this;
  }
  [[nodiscard]] inline const rq::Expression &getUnascribed() const {
    if (this->getCanBeAscription()) {
      return this->getBranch().getUnascribed();
    }
    return *this;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getInclusiveNextSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getInclusiveNextSubrange() const {
    return std::ranges::subrange(rq::ConstExpressionIterator(this),
                                 rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getNextSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this->getNextPtr()),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getNextSubrange() const {
    return std::ranges::subrange(
        rq::ConstExpressionIterator(this->getNextPtr()),
        rq::ConstExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ExpressionIterator, rq::ExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getBranchSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this->getBranchPtr()),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getBranchSubrange() const {
    return std::ranges::subrange(
        rq::ConstExpressionIterator(this->getBranchPtr()),
        rq::ConstExpressionIterator());
  }
};

rq::ExpressionIterator &ExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
}

inline void ExpressionAttributeFlagsFactory::addAttribute(
    const rq::Expression &expression) {
  RQ_ASSERT(expression.getCanBeExpressionAttribute(), "not symbol attribute");
  const rq::ExpressionAttribute attribute = expression.getExpressionAttribute();
  RQ_ASSERT(!rq::getHasAttribute(this->_flags, attribute),
            "duplicate attribute");
  this->_flags |= rq::getFlags(attribute);
  switch (expression.getKeyword()) {
  case rq::EntityKind::KW_CAPTURE:
    this->_capture_ptr = &expression;
    break;
  case rq::EntityKind::KW_OVERRIDE:
    this->_override_ptr = &expression;
    break;
  case rq::EntityKind::KW_POSITION:
    this->_position_ptr = &expression;
    break;
  case rq::EntityKind::KW_MANGLE:
    this->_mangle_ptr = &expression;
    break;
  case rq::EntityKind::KW_LABEL:
    this->_label_ptr = &expression;
    break;
  case rq::EntityKind::KW_TEMPLATE:
    this->_template_ptr = &expression;
    break;
  case rq::EntityKind::KW_DEPRECIATED:
    this->_depreciated_ptr = &expression;
    break;
  case rq::EntityKind::KW_MUTATE_WITH:
    this->_mutate_with_ptr = &expression;
    break;
  default:
    break;
  }
}

inline void ExpressionAttributeFlagsFactory::addAllAttributres(
    const rq::Expression &ascribed) {
  for (const rq::Expression &attribute : ascribed.getNextSubrange()) {
    this->addAttribute(attribute);
  }
}

rq::ExpressionIterator ExpressionIterator::operator++(int) { return ++*this; }

rq::ConstExpressionIterator &ConstExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
}

rq::ConstExpressionIterator ConstExpressionIterator::operator++(int) {
  return ++*this;
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
    RQ_ASSERT(root.getIsType(), "not type");
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
  unsigned _count;

  CountedSubtypeSymbol(rq::EntityKind kind, rq::Symbol &root, unsigned count)
      : rq::Symbol(kind), _root_ptr(&root), _count(count) {
    RQ_ASSERT(rq::getIsCountedSubtypeSymbol(kind), "not counted subtype");
    RQ_ASSERT(root.getIsType(), "not type");
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

  ArithmeticSequenceSymbol(rq::EntityKind kind, rq::Symbol &root,
                           rq::ArithmeticSequenceCondition condition,
                           rq::ArithmeticSequenceStep step)
      : rq::Symbol(kind), _root_ptr(&root), _condition(condition), _step(step) {
    RQ_ASSERT(root.getIsType(), "not type");
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

  rq::SymbolTableSymbol *_scope_ptr;

  SymbolTableMemberSymbol(rq::SymbolTableSymbol &scope) : _scope_ptr(&scope) {}

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
struct HasTypeSymbol {
  using Self = rq::detail::HasTypeSymbol;

  rq::TypeSymbol *_type_ptr{nullptr};

  HasTypeSymbol() {}

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

struct SymbolTableSymbol : public rq::Symbol {
  using Self = rq::SymbolTableSymbol;

  llvm::SmallDenseMap<llvm::StringRef, rq::BumpPtrList<rq::Symbol>>
      _named_values{};
  rq::BumpPtrList<rq::Symbol> _unamed_values{};

  SymbolTableSymbol(rq::EntityKind kind) : rq::Symbol(kind) {}

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

  ProcedureSymbol(rq::EntityKind kind, rq::Expression &expression,
                  rq::ModuleSymbol &module, rq::SymbolTableSymbol &scope,
                  rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(kind), rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
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

template <> struct isa_impl<rq::GenericFloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericFloat();
  }
};

template <> struct isa_impl<rq::GenericFloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericFloat();
  }
};

template <> struct isa_impl<rq::HalfSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsHalf(); }
};

template <> struct isa_impl<rq::HalfSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsHalf();
  }
};

template <> struct isa_impl<rq::SingleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsSingle(); }
};

template <> struct isa_impl<rq::SingleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsSingle();
  }
};

template <> struct isa_impl<rq::DoubleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsDouble(); }
};

template <> struct isa_impl<rq::DoubleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsDouble();
  }
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsQuadruple();
  }
};

template <> struct isa_impl<rq::QuadrupleSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsQuadruple();
  }
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericBinary();
  }
};

template <> struct isa_impl<rq::GenericBinarySymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericBinary();
  }
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericBfloat();
  }
};

template <> struct isa_impl<rq::GenericBfloatSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericBfloat();
  }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary16(); }
};

template <> struct isa_impl<rq::Binary16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary16();
  }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary32(); }
};

template <> struct isa_impl<rq::Binary32Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary32();
  }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBinary64(); }
};

template <> struct isa_impl<rq::Binary64Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary64();
  }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsBinary128();
  }
};

template <> struct isa_impl<rq::Binary128Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBinary128();
  }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsBfloat16(); }
};

template <> struct isa_impl<rq::Bfloat16Symbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsBfloat16();
  }
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericInteger();
  }
};

template <> struct isa_impl<rq::GenericIntegerSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericInteger();
  }
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericSigned();
  }
};

template <> struct isa_impl<rq::GenericSignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericSigned();
  }
};

template <> struct isa_impl<rq::GenericUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericUnsigned();
  }
};

template <>
struct isa_impl<rq::GenericUnsignedSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericUnsigned();
  }
};

template <> struct isa_impl<rq::GenericCodeunitSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsGenericCodeunit();
  }
};

template <>
struct isa_impl<rq::GenericCodeunitSymbol, rq::SimpleBuiltinSymbol> {
  static inline bool doit(const rq::SimpleBuiltinSymbol &val) {
    return val.getIsGenericCodeunit();
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

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledSigned();
  }
};

template <> struct isa_impl<rq::ScaledSignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsScaledSigned();
  }
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsScaledUnsigned();
  }
};

template <> struct isa_impl<rq::ScaledUnsignedSymbol, rq::ScaledBuiltinSymbol> {
  static inline bool doit(const rq::ScaledBuiltinSymbol &val) {
    return val.getIsScaledUnsigned();
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
template <> struct isa_impl<rq::PartialSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) { return val.getIsPartial(); }
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialClass();
  }
};

template <> struct isa_impl<rq::PartialClassSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialClass();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialEnumeration();
  }
};

template <> struct isa_impl<rq::PartialEnumerationSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialEnumeration();
  }
};

template <> struct isa_impl<rq::PartialDynamicVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialDynamicVariable();
  }
};

template <>
struct isa_impl<rq::PartialDynamicVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialDynamicVariable();
  }
};

template <> struct isa_impl<rq::PartialStaticVariableSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialStaticVariable();
  }
};

template <>
struct isa_impl<rq::PartialStaticVariableSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialStaticVariable();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialFunction();
  }
};

template <> struct isa_impl<rq::PartialFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialFunction();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialMethod();
  }
};

template <> struct isa_impl<rq::PartialMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialMethod();
  }
};

template <> struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionFunction();
  }
};

template <>
struct isa_impl<rq::PartialExtensionFunctionSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialExtensionFunction();
  }
};

template <> struct isa_impl<rq::PartialExtensionMethodSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialExtensionMethod();
  }
};

template <>
struct isa_impl<rq::PartialExtensionMethodSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialExtensionMethod();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::Symbol> {
  static inline bool doit(const rq::Symbol &val) {
    return val.getIsPartialConstructor();
  }
};

template <> struct isa_impl<rq::PartialConstructorSymbol, rq::PartialSymbol> {
  static inline bool doit(const rq::PartialSymbol &val) {
    return val.getIsPartialConstructor();
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
  ArraySymbol(rq::Symbol &root, unsigned count)
      : rq::CountedSubtypeSymbol(rq::EntityKind::SY_ARRAY, root, count) {}

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
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_DYNAMIC_VARIABLE),
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
                       rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_STATIC_VARIABLE),
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
                   rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_ENUMERATOR),
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
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_PROPERTY),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  PropertySymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                 rq::SymbolTableSymbol &scope, llvm::StringRef name,
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_PROPERTY),
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
                           rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  SignatureParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                           rq::SymbolTableSymbol &scope, llvm::StringRef name,
                           rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_SIGNATURE_PARAMETER),
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
                          rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  TemplateParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                          rq::SymbolTableSymbol &scope, llvm::StringRef name,
                          rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_TEMPLATE_PARAMETER),
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
                       rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_CLASS_PARAMETER),
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
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
        rq::detail::HasLocationSymbol(expression),
        rq::detail::ModuleMemberSymbol(module),
        rq::detail::SymbolTableMemberSymbol(scope),
        rq::detail::HasAttributesSymbol(attributes),
        rq::detail::MaybeHasNameSymbol() {}
  LayoutParameterSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
                        rq::SymbolTableSymbol &scope, llvm::StringRef name,
                        rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_LAYOUT_PARAMETER),
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
                 rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_MUTATION),
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
      : rq::Symbol(rq::EntityKind::SY_LABEL),
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
                     public rq::detail::SymbolTableMemberSymbol,
                     public rq::detail::HasAttributesSymbol,
                     public rq::detail::HasNameSymbol {
  using Self = rq::ClassSymbol;

  rq::BumpPtrList<rq::PropertySymbol> _class_properties;

  ClassSymbol(rq::Expression &expression, rq::ModuleSymbol &module,
              rq::SymbolTableSymbol &scope, llvm::StringRef name,
              rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::EntityKind::SY_CLASS),
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
                    rq::ExpressionAttributeFlags attributes)
      : rq::SymbolTableSymbol(rq::EntityKind::SY_ENUMERATION),
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
              rq::SymbolTableSymbol &scope,
              rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_ENTRY, expression, module, scope,
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
                 rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_FUNCTION, expression, module,
                            scope, attributes),
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
               rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_METHOD, expression, module,
                            scope, attributes),
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
                          rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_FUNCTION, expression,
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
                        rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_EXTENSION_METHOD, expression,
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
                    rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_CONSTRUCTOR, expression, module,
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
                   rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_DESTRUCTOR, expression, module,
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
               rq::ExpressionAttributeFlags attributes)
      : rq::ProcedureSymbol(rq::EntityKind::SY_RANGER, expression, module,
                            scope, attributes) {}
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
               rq::SymbolTableSymbol &scope,
               rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_IMPORT),
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
               rq::ExpressionAttributeFlags attributes)
      : rq::Symbol(rq::EntityKind::SY_FACADE),
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

struct Instruction : public rq::Entity {
  using Self = Instruction;

  rq::Expression *_source_ptr{nullptr};
  rq::Entity *_a_ptr{nullptr};
  rq::Entity *_b_ptr{nullptr};
  rq::Entity *_c_ptr{nullptr};
  rq::Instruction *_next_ptr{nullptr};

  Instruction() : rq::Entity(rq::EntityKind::OP_NONE) {}
  Instruction(const Self &) = delete;
  ~Instruction() override {}
  Self &operator=(const Self &) = delete;
  Self &operator=(Self &&) = delete;

  // TODO
};

} // namespace rq
