#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstddef>
#include <cstdint>

namespace rq {

// Nodes represent individual expressions in the AST. Each is identified by a
// keyword.

enum class Keyword : std::uint32_t {
  // this is the initial keyword set for expressions. it must be overwritten
  // later!
  NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  STRING_LITERAL,
  // left string interpolation (string literal with no end quote mark).
  LEFT_INTERPOLATION_LITERAL,
  // middle string interpolation (string literal with no quote marks).
  MIDDLE_INTERPOLATION_LITERAL,
  // right string interpolation (string literal with start quote mark).
  RIGHT_INTERPOLATION_LITERAL,
  // a literal that represents a single text character.
  CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  IDENTIFIER_LITERAL,

  // ERRORS
  ERROR,

  // SITUATIONAL
  UNSITUATED_PARENTHESIS_GROUP,
  UNSITUATED_EQUAL_OPERATOR,
  UNSITUATED_ASCRIBE_STATEMENT,
  UNSITUATED_ASCRIBE_TYPE,

  // LOGICAL
  LOGICAL_AND,
  LOGICAL_OR,
  LOGICAL_COMPLEMENT,

  // COMPARISON
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  EQUAL,
  NOT_EQUAL,

  // APPLY
  EXTEND,
  EXTENSION,
  BINDING,
  ASCRIBE_TYPE,
  ASCRIBE_STATEMENT,
  ASCRIBE_ROOT_OF_VALUE,
  // turn a string into an identifier
  IDENTIFY,
  // turn an identifier into a keyword
  KEWORDIFY,

  // ARITHMETIC
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  NEGATE,

  // CASTS
  AS,
  LITERAL_AS,
  OF,
  ELEMENTS_OF,
  CAST,
  CAST_OF,
  DOWN_CAST,
  DOWN_CAST_OF,
  FLOAT_CAST,
  FLOAT_CAST_OF,
  PLATFORM_CAST,
  PLATFORM_CAST_OF,
  STATIC_CAST,
  STATIC_CAST_OF,
  BITWISE_CAST,
  BITWISE_CAST_OF,

  // BITWISE
  BITWISE_OR,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_COMPLEMENT,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  // MEMORY
  CONTENT,
  CONTENT_OF,
  ADDRESS,
  ADDRESS_OF,
  BORROW,
  BORROW_OF,
  DATA_ADDRESS,
  DATA_ADDRESS_OF,

  // ASSIGNMENT
  ASSIGN,

  // SUBTYPE
  ARRAY,
  REFERENCE,
  POINTER,
  FAT_POINTER,

  // TYPE ATTRIBUTES
  MUTABLE,
  CONSTANT,
  PARTIALLY_MUTABLE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  MAY_DISCARD,
  INDETERMINATE,

  // PARAMETER RULES
  POSITIONAL_PARAMETERS_END,
  NAMED_PARAMETERS_BEGIN,

  // BRACES
  TUPLE,
  LAYOUT_TYPE,
  NULL_,
  NULL_TYPE,
  STRUCTURED_BINDING,
  IGNORE,
  SPECIALIZATION,

  // PROCEDURES
  CALL,
  NAMED_ARGUMENT,
  INDEX_INTO,
  SIGNATURE_TYPE,
  DEFAULT_VALUE_PARAMETER,
  DESTROY,
  DESTROY_VALUE,
  DROP,
  DROP_VALUE,
  MOVE,
  MOVE_VALUE,
  ENTRY,
  FUNCTION,
  METHOD,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  RANGER,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  RANGE_OVER,

  // DECLARED TYPES
  CLASS,
  ENUMERATION,

  // VALUES
  INITIALIZER_LIST,
  TRUE,
  FALSE,
  // vignette value.
  VALUE,
  // vignette index.
  INDEX,
  // vignette or reflected enumerator index.
  DISCRIMINANT,
  // vignette value returned from a block.
  OUT,
  // reference to extended value of method or extension_method.
  THIS,
  // value returned from a function.
  RESULT,
  // retrieve command line arguments within entry.
  COMMAND_LINE_ARGUMENTS,

  // BUILTIN TYPES
  INFERENCE,
  VOID,
  NO_RETURN,
  BOOLEAN,
  FLOAT,
  HALF,
  SINGLE,
  DOUBLE,
  QUADRUPLE,
  BINARY,
  BFLOAT,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  BFLOAT16,
  INTEGER,
  SIGNED,
  SIGNED_OF,
  UNSIGNED,
  UNSIGNED_OF,
  SIGNED_FAST_BITS,
  SIGNED_FAST_BYTES,
  SIGNED_LEAST_BITS,
  SIGNED_LEAST_BYTES,
  SIGNED_EXACT_BITS,
  SIGNED_EXACT_BYTES,
  SIGNED_INDEX,
  SIGNED_ADDRESS,
  UNSIGNED_FAST_BITS,
  UNSIGNED_FAST_BYTES,
  UNSIGNED_LEAST_BITS,
  UNSIGNED_LEAST_BTYES,
  UNSIGNED_EXACT_BITS,
  UNSIGNED_EXACT_BYTES,
  UNSIGNED_INDEX,
  UNSIGNED_ADDRESS,
  STRING,
  CODEUNIT,
  ASCII,
  UTF8,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS,
  FIRST_VARIADIC_ARGUMENT,
  FIRST_VARIADIC_ARGUMENT_OF,
  NEXT_VARIADIC_ARGUMENT,
  NEXT_VARIADIC_ARGUMENT_OF,

  // SCOPES
  IF,
  ELSE_IF,
  ELSE,
  MATCH,
  INLINE_MATCH,
  SWITCH,
  INLINE_SWITCH,
  CASE,
  WITH,
  DEFAULT,
  FOR,
  WHILE,
  SCOPE,
  INLINE_SCOPE,
  BLOCK,
  INLINE_BLOCK,

  // RANGES
  RANGE,
  ARITHMETIC_SEQUENCE,
  ARITHMETIC_SEQUENCE_CONDITION_LESS,
  ARITHMETIC_SEQUENCE_CONDITION_GREATER,
  ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_EQUAL,
  ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL,
  ARITHMETIC_SEQUENCE_STEP_ADD,
  ARITHMETIC_SEQUENCE_STEP_SUBTRACT,
  ARITHMETIC_SEQUENCE_STEP_MULTIPLY,
  ARITHMETIC_SEQUENCE_STEP_DIVIDE,
  ARITHMETIC_SEQUENCE_STEP_MODULUS,

  // ARITHMETIC INTERVAL TYPES
  ARITHMETIC_INTERVAL_LESS,
  ARITHMETIC_INTERVAL_LESS_EQUAL,
  ARITHMETIC_INTERVAL_GREATER,
  ARITHMETIC_INTERVAL_GREATER_EQUAL,
  ARITHMETIC_INTERVAL_EQUAL,
  ARITHMETIC_INTERVAL_NOT_EQUAL,

  // FINITE ARITHMETIC PROGRESSION TYPES
  FINITE_ARITHMETIC_PROGRESSION_LESS_ADD,
  FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS,
  FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD,
  FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS,
  FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD,
  FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS,
  FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD,
  FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT,
  FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY,
  FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE,
  FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS,

  // INFINITE ARITHMETIC PROGRESSION TYPES
  INFINITE_ARITHMETIC_PROGRESSION_ADD,
  INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT,
  INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY,
  INFINITE_ARITHMETIC_PROGRESSION_DIVIDE,
  INFINITE_ARITHMETIC_PROGRESSION_MODULUS,

  // ACCESS MODIFIERS
  PUBLIC,
  PROTECTED,
  EXPORT,

  // TABLE GRAPH
  IMPORT,
  USE,
  FACADE,
  TABLE,
  MODULE_TRUNK,

  // ERROR HANDLING AND DEBUGGING
  DEBUG_TRAP,

  // HINTS
  UNREACHABLE,
  ASSUME,

  // EXPRESSION ATTRIBUTES
  OPAQUE,
  OUTSIDE,
  STATIC,
  CAPTURE,
  CAPTURE_OF,
  EAGER,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  INLINE,
  MANGLE,
  PACK,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  MAY_COPY,
  MAY_MOVE,
  OK,

  // MACROS
  QUOTE,
  EXPRESSION,
  ENTITY_KIND,
  EXPAND,
  EXPAND_STATEMENT,
  EXPAND_LVALUE,
  EXPAND_RVALUE,
  EXPAND_REFLECTION,
  EXPAND_ARGUMENT,
  EXPAND_PARAMETER,
  EXPAND_SYMBOL_PATH,
  EXPAND_ARITHMETIC_SEQUENCE_STAGE,

  // REFLECTIONS
  REFLECT,
  MEMBER_OF,
  MEMBER_OF_TOP,
  ASCEND_FRAME,
  ASCEND_FRAME_OF,
  BYTE_SIZE,
  BYTE_SIZE_OF,
  BIT_DEPTH,
  BIT_DEPTH_OF,
  ELEMENT_COUNT,
  ELEMENT_COUNT_OF,
  NAME,
  NAME_OF,
  LINE,
  LINE_OF,
  COLUMN,
  COLUMN_OF,
  IS,
  IS_TYPE,
  HOLDS,
  HOLDS_ENUMERATOR,
  TYPE,
  TYPE_OF,
  SYMBOL,
  SYMBOL_OF,
  HAS_MEMBER,
  HAS_MEMBER_OF,
  HAS,
  HAS_OF,
  GET,
  GET_OF,
  SIGNATURE,
  SIGNATURE_OF,
  LAYOUT,
  LAYOUT_OF,
  // make a unique clone of a type that is not implicitly convertable
  // can use platform specific values for bit depth only if type is a synonym
  SYNONYM,
  SYNONYM_OF,
  IS_OK,
  IS_OK_OF,

  LAST
};

static constexpr std::size_t KEYWORD_COUNT =
    static_cast<std::size_t>(rq::Keyword::LAST) - 1;

[[nodiscard]] inline llvm::StringRef getName(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::NONE:
    return "none";

    // LITERALS
  case K::INTEGER_LITERAL:
    return "__integer_literal";
  case K::FLOAT_LITERAL:
    return "__float_literal";
  case K::STRING_LITERAL:
    return "__string_literal";
  case K::LEFT_INTERPOLATION_LITERAL:
    return "__left_interpolation_literal";
  case K::MIDDLE_INTERPOLATION_LITERAL:
    return "__middle_interpolation_literal";
  case K::RIGHT_INTERPOLATION_LITERAL:
    return "__right_interpolation_literal";
  case K::CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case K::IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case K::ERROR:
    return "__error";

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return "_unsituated_parenthesis_group";
  case K::UNSITUATED_EQUAL_OPERATOR:
    return "_unsituated_equal_operator";
  case K::UNSITUATED_ASCRIBE_STATEMENT:
    return "_unsituated_ascribe_statement";
  case K::UNSITUATED_ASCRIBE_TYPE:
    return "_unsituated_ascribe_type";

  // LOGICAL
  case K::LOGICAL_AND:
    return "_logical_and";
  case K::LOGICAL_OR:
    return "_logical_or";
  case K::LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case K::GREATER:
    return "_greater";
  case K::GREATER_EQUAL:
    return "_greater_equal";
  case K::LESS:
    return "_less";
  case K::LESS_EQUAL:
    return "_less_equal";
  case K::EQUAL:
    return "_equal";
  case K::NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case K::EXTEND:
    return "_extend";
  case K::EXTENSION:
    return "_extension";
  case K::BINDING:
    return "_binding";
  case K::ASCRIBE_TYPE:
    return "_ascribe_type";
  case K::ASCRIBE_STATEMENT:
    return "_ascribe_statement";
  case K::ASCRIBE_ROOT_OF_VALUE:
    return "_ascribe_root_of_value";
  case K::IDENTIFY:
    return "_identify";
  case K::KEWORDIFY:
    return "kewordify";

  // ARITHMETIC
  case K::ADD:
    return "_add";
  case K::SUBTRACT:
    return "_subtract";
  case K::MULTIPLY:
    return "_multiply";
  case K::DIVIDE:
    return "_divide";
  case K::MODULUS:
    return "_modulus";
  case K::NEGATE:
    return "_negate";

  // CASTS
  case K::AS:
    return "as";
  case K::LITERAL_AS:
    return "_literal_as";
  case K::OF:
    return "of";
  case K::ELEMENTS_OF:
    return "_elements_of";
  case K::CAST:
    return "cast";
  case K::CAST_OF:
    return "_cast_of";
  case K::DOWN_CAST:
    return "down_cast";
  case K::DOWN_CAST_OF:
    return "_down_cast_of";
  case K::FLOAT_CAST:
    return "float_cast";
  case K::FLOAT_CAST_OF:
    return "_float_cast_of";
  case K::PLATFORM_CAST:
    return "platform_cast";
  case K::PLATFORM_CAST_OF:
    return "_platform_cast_of";
  case K::STATIC_CAST:
    return "static_cast";
  case K::STATIC_CAST_OF:
    return "_static_cast_of";
  case K::BITWISE_CAST:
    return "bitwise_cast";
  case K::BITWISE_CAST_OF:
    return "_bitwise_cast_of";

  // BITWISE
  case K::BITWISE_OR:
    return "_bitwise_or";
  case K::BITWISE_AND:
    return "_bitwise_and";
  case K::BITWISE_XOR:
    return "_bitwise_xor";
  case K::BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case K::BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case K::BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case K::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case K::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case K::CONTENT:
    return "content";
  case K::CONTENT_OF:
    return "_content_of";
  case K::ADDRESS:
    return "address";
  case K::ADDRESS_OF:
    return "_address_of";
  case K::BORROW:
    return "borrow";
  case K::BORROW_OF:
    return "_borrow_of";
  case K::DATA_ADDRESS:
    return "data_address";
  case K::DATA_ADDRESS_OF:
    return "_data_address_of";

  // ASSIGNMENT
  case K::ASSIGN:
    return "_assign";

  // SUBTYPE
  case K::ARRAY:
    return "_array";
  case K::REFERENCE:
    return "_reference";
  case K::POINTER:
    return "_pointer";
  case K::FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case K::MUTABLE:
    return "_mutable";
  case K::CONSTANT:
    return "";
  case K::PARTIALLY_MUTABLE:
    return "partially_mutable";
  case K::VOLATILE:
    return "volatile";
  case K::ATOMIC:
    return "atomic";
  case K::NULL_TERMINATED:
    return "null_terminated";
  case K::MAY_DISCARD:
    return "may_discard";
  case K::INDETERMINATE:
    return "indeterminate";

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case K::NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";

  // BRACES
  case K::TUPLE:
    return "_tuple";
  case K::LAYOUT_TYPE:
    return "_layout_type";
  case K::NULL_:
    return "_null";
  case K::NULL_TYPE:
    return "_null_type";
  case K::STRUCTURED_BINDING:
    return "_structured_binding";
  case K::IGNORE:
    return "_ignore";
  case K::SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case K::CALL:
    return "_call";
  case K::NAMED_ARGUMENT:
    return "_named_argument";
  case K::INDEX_INTO:
    return "_index_into";
  case K::SIGNATURE_TYPE:
    return "_signature_type";
  case K::DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case K::DESTROY:
    return "destroy";
  case K::DESTROY_VALUE:
    return "_destroy_value";
  case K::DROP:
    return "drop";
  case K::DROP_VALUE:
    return "_drop_value";
  case K::MOVE:
    return "move";
  case K::MOVE_VALUE:
    return "_move_value";
  case K::ENTRY:
    return "entry";
  case K::FUNCTION:
    return "function";
  case K::METHOD:
    return "method";
  case K::EXTENSION_FUNCTION:
    return "extension_function";
  case K::EXTENSION_METHOD:
    return "extension_method";
  case K::RANGER:
    return "ranger";

  // CONTROL FLOW
  case K::RETURN:
    return "return";
  case K::BREAK:
    return "break";
  case K::CONTINUE:
    return "continue";
  case K::FALLTHROUGH:
    return "fallthrough";
  case K::GOTO:
    return "goto";
  case K::RANGE_OVER:
    return "range_over";

  // DECLARED TYPES
  case K::CLASS:
    return "class";
  case K::ENUMERATION:
    return "enumeration";

  // VALUES
  case K::INITIALIZER_LIST:
    return "_initializer_list";
  case K::TRUE:
    return "true";
  case K::FALSE:
    return "false";
  case K::VALUE:
    return "value";
  case K::INDEX:
    return "index";
  case K::DISCRIMINANT:
    return "discriminant";
  case K::OUT:
    return "out";
  case K::THIS:
    return "this";
  case K::RESULT:
    return "result";
  case K::COMMAND_LINE_ARGUMENTS:
    return "command_line_arguments";

  // BUILTIN TYPES
  case K::INFERENCE:
    return "_inference";
  case K::VOID:
    return "void";
  case K::NO_RETURN:
    return "no_return";
  case K::BOOLEAN:
    return "boolean";
  case K::FLOAT:
    return "float";
  case K::HALF:
    return "half";
  case K::SINGLE:
    return "single";
  case K::DOUBLE:
    return "double";
  case K::QUADRUPLE:
    return "quadruple";
  case K::BINARY:
    return "binary";
  case K::BFLOAT:
    return "bfloat";
  case K::BINARY16:
    return "binary16";
  case K::BINARY32:
    return "binary32";
  case K::BINARY64:
    return "binary64";
  case K::BINARY128:
    return "binary128";
  case K::BFLOAT16:
    return "bfloat16";
  case K::INTEGER:
    return "integer";
  case K::SIGNED:
    return "signed";
  case K::SIGNED_OF:
    return "_signed_of";
  case K::UNSIGNED:
    return "unsigned";
  case K::UNSIGNED_OF:
    return "_unsigned_of";
  case K::SIGNED_FAST_BITS:
    return "signed_fast_bits";
  case K::SIGNED_FAST_BYTES:
    return "signed_fast_bytes";
  case K::SIGNED_LEAST_BITS:
    return "signed_least_bits";
  case K::SIGNED_LEAST_BYTES:
    return "signed_least_bytes";
  case K::SIGNED_EXACT_BITS:
    return "signed_exact_bits";
  case K::SIGNED_EXACT_BYTES:
    return "signed_exact_bytes";
  case K::SIGNED_INDEX:
    return "signed_index";
  case K::SIGNED_ADDRESS:
    return "signed_address";
  case K::UNSIGNED_FAST_BITS:
    return "unsigned_fast_bits";
  case K::UNSIGNED_FAST_BYTES:
    return "unsigned_fast_bytes";
  case K::UNSIGNED_LEAST_BITS:
    return "unsigned_least_bits";
  case K::UNSIGNED_LEAST_BTYES:
    return "unsigned_least_btyes";
  case K::UNSIGNED_EXACT_BITS:
    return "unsigned_exact_bits";
  case K::UNSIGNED_EXACT_BYTES:
    return "unsigned_exact_bytes";
  case K::UNSIGNED_INDEX:
    return "unsigned_index";
  case K::UNSIGNED_ADDRESS:
    return "unsigned_address";
  case K::STRING:
    return "string";
  case K::CODEUNIT:
    return "codeunit";
  case K::ASCII:
    return "ascii";
  case K::UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case K::FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case K::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    return "_next_variadic_argument_of";

  // SCOPES
  case K::IF:
    return "if";
  case K::ELSE_IF:
    return "else_if";
  case K::ELSE:
    return "else";
  case K::MATCH:
    return "match";
  case K::INLINE_MATCH:
    return "inline_match";
  case K::SWITCH:
    return "switch";
  case K::INLINE_SWITCH:
    return "inline_switch";
  case K::CASE:
    return "case";
  case K::WITH:
    return "with";
  case K::DEFAULT:
    return "default";
  case K::FOR:
    return "for";
  case K::WHILE:
    return "while";
  case K::SCOPE:
    return "scope";
  case K::INLINE_SCOPE:
    return "inline_scope";
  case K::BLOCK:
    return "block";
  case K::INLINE_BLOCK:
    return "inline_block";

  // RANGES
  case K::RANGE:
    return "range";
  case K::ARITHMETIC_SEQUENCE:
    return "_arithmetic_sequence";
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return "_arithmetic_sequence_condition_less";
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return "_arithmetic_sequence_condition_greater";
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return "_arithmetic_sequence_condition_less_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return "_arithmetic_sequence_condition_greater_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return "_arithmetic_sequence_condition_equal";
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return "_arithmetic_sequence_condition_not_equal";
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return "_arithmetic_sequence_step_add";
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return "_arithmetic_sequence_step_subtract";
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return "_arithmetic_sequence_step_multiply";
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return "_arithmetic_sequence_step_divide";
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return "_arithmetic_sequence_step_modulus";

  // ARITHMETIC INTERVAL
  case K::ARITHMETIC_INTERVAL_LESS:
    return "_arithmetic_interval_less";
  case K::ARITHMETIC_INTERVAL_LESS_EQUAL:
    return "_arithmetic_interval_less_equal";
  case K::ARITHMETIC_INTERVAL_GREATER:
    return "_arithmetic_interval_greater";
  case K::ARITHMETIC_INTERVAL_GREATER_EQUAL:
    return "_arithmetic_interval_greater_equal";
  case K::ARITHMETIC_INTERVAL_EQUAL:
    return "_arithmetic_interval_equal";
  case K::ARITHMETIC_INTERVAL_NOT_EQUAL:
    return "_arithmetic_interval_not_equal";

  // FINITE ARITHMETIC PROGRESSION TYPES
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    return "_finite_arithmetic_progression_less_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    return "_finite_arithmetic_progression_less_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    return "_finite_arithmetic_progression_less_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    return "_finite_arithmetic_progression_less_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    return "_finite_arithmetic_progression_less_modulus";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    return "_finite_arithmetic_progression_less_equal_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_less_equal_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_less_equal_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_less_equal_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_less_equal_modulus";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    return "_finite_arithmetic_progression_greater_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    return "_finite_arithmetic_progression_greater_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    return "_finite_arithmetic_progression_greater_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    return "_finite_arithmetic_progression_greater_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    return "_finite_arithmetic_progression_greater_modulus";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    return "_finite_arithmetic_progression_greater_equal_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_greater_equal_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_greater_equal_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_greater_equal_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_greater_equal_modulus";
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    return "_finite_arithmetic_progression_equal_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_equal_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_equal_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_equal_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_equal_modulus";
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    return "_finite_arithmetic_progression_not_equal_add";
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    return "_finite_arithmetic_progression_not_equal_subtract";
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    return "_finite_arithmetic_progression_not_equal_multiply";
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    return "_finite_arithmetic_progression_not_equal_divide";
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    return "_finite_arithmetic_progression_not_equal_modulus";

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case K::INFINITE_ARITHMETIC_PROGRESSION_ADD:
    return "_infinite_arithmetic_progression_add";
  case K::INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    return "_infinite_arithmetic_progression_subtract";
  case K::INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    return "_infinite_arithmetic_progression_multiply";
  case K::INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    return "_infinite_arithmetic_progression_divide";
  case K::INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    return "_infinite_arithmetic_progression_modulus";

  // ACCESS MODIFIERS
  case K::PUBLIC:
    return "public";
  case K::PROTECTED:
    return "protected";
  case K::EXPORT:
    return "export";

  // TABLE GRAPH
  case K::IMPORT:
    return "import";
  case K::USE:
    return "use";
  case K::FACADE:
    return "facade";
  case K::TABLE:
    return "table";
  case K::MODULE_TRUNK:
    return "_module_trunk";

  // HINTS
  case K::DEBUG_TRAP:
    return "debug_trap";
  case K::UNREACHABLE:
    return "unreachable";
  case K::ASSUME:
    return "assume";

  // EXPRESSION ATTRIBUTES
  case K::OPAQUE:
    return "opaque";
  case K::OUTSIDE:
    return "outside";
  case K::STATIC:
    return "static";
  case K::CAPTURE:
    return "capture";
  case K::CAPTURE_OF:
    return "_capture_of";
  case K::EAGER:
    return "eager";
  case K::MAY_PARENT:
    return "may_parent";
  case K::PARENT:
    return "parent";
  case K::ABSTRACT:
    return "abstract";
  case K::VIRTUAL:
    return "virtual";
  case K::OVERRIDE:
    return "override";
  case K::POSITION:
    return "position";
  case K::INLINE:
    return "inline";
  case K::MANGLE:
    return "mangle";
  case K::PACK:
    return "pack";
  case K::LABEL:
    return "label";
  case K::TEMPLATE:
    return "template";
  case K::LIKELY:
    return "likely";
  case K::UNLIKELY:
    return "unlikely";
  case K::DEPRECIATED:
    return "depreciated";
  case K::MAY_COPY:
    return "may_copy";
  case K::MAY_MOVE:
    return "may_move";

  // NODES
  case K::QUOTE:
    return "quote";
  case K::EXPRESSION:
    return "expression";
  case K::ENTITY_KIND:
    return "entity_keyword";
  case K::EXPAND:
    return "expand";
  case K::EXPAND_STATEMENT:
    return "_expand_statement";
  case K::EXPAND_LVALUE:
    return "_expand_lvalue";
  case K::EXPAND_RVALUE:
    return "_expand_rvalue";
  case K::EXPAND_REFLECTION:
    return "_expand_reflection";
  case K::EXPAND_ARGUMENT:
    return "_expand_argument";
  case K::EXPAND_PARAMETER:
    return "_expand_parameter";
  case K::EXPAND_SYMBOL_PATH:
    return "_expand_symbol_path";
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return "_expand_arithmetic_sequence_stage";

  // REFLECTIONS
  case K::REFLECT:
    return "_reflect";
  case K::MEMBER_OF:
    return "_member_of";
  case K::MEMBER_OF_TOP:
    return "_member_of_top";
  case K::ASCEND_FRAME:
    return "_ascend_frame";
  case K::ASCEND_FRAME_OF:
    return "_ascend_frame_of";
  case K::BYTE_SIZE:
    return "byte_size";
  case K::BYTE_SIZE_OF:
    return "_byte_size_of";
  case K::BIT_DEPTH:
    return "bit_depth";
  case K::BIT_DEPTH_OF:
    return "_bit_depth_of";
  case K::ELEMENT_COUNT:
    return "element_count";
  case K::ELEMENT_COUNT_OF:
    return "_element_count_of";
  case K::NAME:
    return "name";
  case K::NAME_OF:
    return "_name_of";
  case K::LINE:
    return "line";
  case K::LINE_OF:
    return "_line_of";
  case K::COLUMN:
    return "column";
  case K::COLUMN_OF:
    return "_column_of";
  case K::IS:
    return "is";
  case K::IS_TYPE:
    return "_is_type";
  case K::HOLDS:
    return "holds";
  case K::HOLDS_ENUMERATOR:
    return "_holds_enumerator";
  case K::TYPE:
    return "type";
  case K::TYPE_OF:
    return "_type_of";
  case K::SYMBOL:
    return "symbol";
  case K::SYMBOL_OF:
    return "_symbol_of";
  case K::HAS_MEMBER:
    return "has_member";
  case K::HAS_MEMBER_OF:
    return "_has_member_of";
  case K::HAS:
    return "has";
  case K::HAS_OF:
    return "_has_of";
  case K::GET:
    return "get";
  case K::GET_OF:
    return "_get_of";
  case K::SIGNATURE:
    return "signature";
  case K::SIGNATURE_OF:
    return "_signature_of";
  case K::LAYOUT:
    return "layout";
  case K::LAYOUT_OF:
    return "_layout_of";
  case K::SYNONYM:
    return "synonym";
  case K::SYNONYM_OF:
    return "_synonym_of";
  case K::IS_OK:
    return "is_ok";
  case K::IS_OK_OF:
    return "_is_ok_of";
  default:
    break;
  }
  RQ_UNREACHABLE();
}

enum class KeywordFlags : std::uint32_t {
  NONE = 0,
  CONVERGING = rq::getBit(0),
  LITERAL = rq::getBit(1),
  UNQUOTED_LEFT = rq::getBit(2),
  UNQUOTED_RIGHT = rq::getBit(3),
  INTERNAL = rq::getBit(4),
  UNIVERSALIZABLE = rq::getBit(5),
  STATEMENT_BRANCHES = rq::getBit(6),
  STARTING_CHAINLINK = rq::getBit(7),
  CONTINUING_CHAINLINK = rq::getBit(8),
  FINISHING_CHAINLINK = rq::getBit(9),
  IF_CHAINLINK = rq::getBit(10),
  ARM_CHAINLINK = rq::getBit(11),
  // TRUNK
  STATEMENT = rq::getBit(12),
  RVALUE = rq::getBit(13),
  LVALUE = rq::getBit(14),
  REFLECTION = rq::getBit(15),
  ARGUMENT = rq::getBit(16),
  PARAMETER = rq::getBit(17),
  BINDING = rq::getBit(18),
  SYMBOL_PATH = rq::getBit(19),
  ASCRIPTION = rq::getBit(20),
  TYPE_ATTRIBUTE = rq::getBit(21),
  EXPRESSION_ATTRIBUTE = rq::getBit(22),
  ARITHMETIC_SEQUENCE_STEP = rq::getBit(23),
  ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(24),
  ALL_SITUATIONS = STATEMENT | RVALUE | LVALUE | REFLECTION | ARGUMENT |
                   PARAMETER | BINDING | SYMBOL_PATH | ASCRIPTION |
                   TYPE_ATTRIBUTE | EXPRESSION_ATTRIBUTE |
                   ARITHMETIC_SEQUENCE_STEP | ARITHMETIC_SEQUENCE_CONDITION,

};

template <> struct is_flags<KeywordFlags> : std::true_type {};

[[nodiscard]] inline rq::KeywordFlags getFlags(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using KF = KeywordFlags;
  switch (keyword) {
  case K::NONE:
    return KF::NONE;

    // LITERALS
  case K::INTEGER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::FLOAT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::STRING_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::LEFT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::MIDDLE_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL |
           KF::RVALUE | KF::ARGUMENT;
  case K::RIGHT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::CODEUNIT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::IDENTIFIER_LITERAL:
    return KF::STATEMENT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::LVALUE | KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER |
           KF::SYMBOL_PATH;

  // ERRORS
  case K::ERROR:
    return KF::INTERNAL;

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT | KF::LVALUE |
           KF::SYMBOL_PATH | KF::ARITHMETIC_SEQUENCE_STEP |
           KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::UNSITUATED_EQUAL_OPERATOR:
    return KF::STATEMENT | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSITUATED_ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::UNSITUATED_ASCRIBE_STATEMENT:
    return KF::STATEMENT | KF::PARAMETER | KF::ARGUMENT | KF::ASCRIPTION;

  // LOGICAL
  case K::LOGICAL_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LOGICAL_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LOGICAL_COMPLEMENT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // COMPARISON
  case K::GREATER:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::GREATER_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LESS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::LESS_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::NOT_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // APPLY
  case K::EXTEND:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EXTENSION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINDING:
    return KF::STATEMENT | KF::LVALUE | KF::PARAMETER | KF::ARGUMENT |
           KF::BINDING;
  case K::ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::ASCRIBE_STATEMENT:
    return KF::STATEMENT | KF::PARAMETER | KF::ARGUMENT | KF::ASCRIPTION;
  case K::ASCRIBE_ROOT_OF_VALUE:
    return KF::RVALUE | KF::ARGUMENT | KF::ASCRIPTION;
  case K::IDENTIFY:
    return KF::SYMBOL_PATH | KF::RVALUE | KF::ARGUMENT;
  case K::KEWORDIFY:
    return KF::RVALUE | KF::ARGUMENT;

  // ARITHMETIC
  case K::ADD:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::SUBTRACT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::MULTIPLY:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::DIVIDE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::MODULUS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::NEGATE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // CASTS
  case K::AS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::LITERAL_AS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::OF:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ELEMENTS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DOWN_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DOWN_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::FLOAT_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::FLOAT_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::PLATFORM_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::PLATFORM_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::STATIC_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::STATIC_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_CAST:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BITWISE_CAST_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // BITWISE
  case K::BITWISE_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_XOR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_COMPLEMENT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_SHIFT_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_SHIFT_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;

  // MEMORY
  case K::CONTENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::CONTENT_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::ADDRESS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BORROW:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BORROW_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DATA_ADDRESS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DATA_ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // ASSIGNMENT
  case K::ASSIGN:
    return KF::STATEMENT;

  // SUBTYPE
  case K::ARRAY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::REFERENCE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FAT_POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // TYPE MODIFIER
  case K::MUTABLE:
    return KF::TYPE_ATTRIBUTE;
  case K::CONSTANT:
    return KF::TYPE_ATTRIBUTE;
  case K::PARTIALLY_MUTABLE:
    return KF::TYPE_ATTRIBUTE;
  case K::VOLATILE:
    return KF::TYPE_ATTRIBUTE;
  case K::ATOMIC:
    return KF::TYPE_ATTRIBUTE;
  case K::NULL_TERMINATED:
    return KF::TYPE_ATTRIBUTE;
  case K::MAY_DISCARD:
    return KF::TYPE_ATTRIBUTE;
  case K::INDETERMINATE:
    return KF::TYPE_ATTRIBUTE;

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return KF::PARAMETER;
  case K::NAMED_PARAMETERS_BEGIN:
    return KF::PARAMETER;

  // BRACES
  case K::TUPLE:
    return KF::LVALUE | KF::RVALUE | KF::ARGUMENT;
  case K::LAYOUT_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NULL_:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NULL_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::STRUCTURED_BINDING:
    return KF::STATEMENT;
  case K::IGNORE:
    return KF::STATEMENT;
  case K::SPECIALIZATION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::CALL:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::NAMED_ARGUMENT:
    return KF::ARGUMENT;
  case K::INDEX_INTO:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::SIGNATURE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DEFAULT_VALUE_PARAMETER:
    return KF::PARAMETER;
  case K::DESTROY:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DESTROY_VALUE:
    return KF::STATEMENT;
  case K::DROP:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DROP_VALUE:
    return KF::STATEMENT;
  case K::MOVE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::MOVE_VALUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ENTRY:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;
  case K::FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::RANGER:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;

  // CONTROL FLOW
  case K::RETURN:
    return KF::STATEMENT;
  case K::BREAK:
    return KF::STATEMENT;
  case K::CONTINUE:
    return KF::STATEMENT;
  case K::FALLTHROUGH:
    return KF::STATEMENT;
  case K::GOTO:
    return KF::STATEMENT;
  case K::RANGE_OVER:
    return KF::STATEMENT;

  // DECLARED TYPES
  case K::CLASS:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::ENUMERATION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;

  // VALUES
  case K::INITIALIZER_LIST:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TRUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::FALSE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VALUE:
    return KF::RVALUE;
  case K::INDEX:
    return KF::RVALUE;
  case K::DISCRIMINANT:
    return KF::RVALUE;
  case K::OUT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::THIS:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::RESULT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::COMMAND_LINE_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT;

  // BUILTIN TYPES
  case K::INFERENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VOID:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NO_RETURN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BOOLEAN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FLOAT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::HALF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SINGLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DOUBLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::QUADRUPLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BFLOAT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY32:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY64:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY128:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BFLOAT16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_FAST_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_FAST_BYTES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_LEAST_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_LEAST_BYTES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_EXACT_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_EXACT_BYTES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_FAST_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_FAST_BYTES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_LEAST_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_LEAST_BTYES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_EXACT_BITS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_EXACT_BYTES:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::STRING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CODEUNIT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ASCII:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UTF8:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FIRST_VARIADIC_ARGUMENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::FIRST_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::NEXT_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // SCOPES
  case K::IF:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::IF_CHAINLINK;
  case K::ELSE_IF:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::CONTINUING_CHAINLINK |
           KF::FINISHING_CHAINLINK | KF::IF_CHAINLINK;
  case K::ELSE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::FINISHING_CHAINLINK |
           KF::IF_CHAINLINK;
  case K::MATCH:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::INLINE_MATCH:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;
  case K::SWITCH:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::INLINE_SWITCH:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;
  case K::CASE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK |
           KF::ARM_CHAINLINK;
  case K::WITH:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK |
           KF::ARM_CHAINLINK;
  case K::DEFAULT:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK |
           KF::ARM_CHAINLINK;
  case K::FOR:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::WHILE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::SCOPE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;
  case K::INLINE_SCOPE:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;
  case K::BLOCK:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::INLINE_BLOCK:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;

  // RANGES
  case K::RANGE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_SEQUENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
    return KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return KF::ARITHMETIC_SEQUENCE_STEP;
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
    return KF::ARITHMETIC_SEQUENCE_STEP;

  // ARITHMETIC INTERVAL
  case K::ARITHMETIC_INTERVAL_LESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_INTERVAL_LESS_EQUAL:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_INTERVAL_GREATER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_INTERVAL_GREATER_EQUAL:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_INTERVAL_EQUAL:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ARITHMETIC_INTERVAL_NOT_EQUAL:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // FINITE ARITHMETIC PROGRESSION TYPES
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_LESS_EQUAL_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_GREATER_EQUAL_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_EQUAL_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FINITE_ARITHMETIC_PROGRESSION_NOT_EQUAL_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // INFINITE ARITHMETIC PROGRESSION TYPES
  case K::INFINITE_ARITHMETIC_PROGRESSION_ADD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INFINITE_ARITHMETIC_PROGRESSION_SUBTRACT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INFINITE_ARITHMETIC_PROGRESSION_MULTIPLY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INFINITE_ARITHMETIC_PROGRESSION_DIVIDE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INFINITE_ARITHMETIC_PROGRESSION_MODULUS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // ACCESS MODIFIERS
  case K::PUBLIC:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::PROTECTED:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::EXPORT:
    return KF::EXPRESSION_ATTRIBUTE;

  // TABLE GRAPH
  case K::IMPORT:
    return KF::STATEMENT;
  case K::USE:
    return KF::STATEMENT;
  case K::FACADE:
    return KF::STATEMENT;
  case K::TABLE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;
  case K::MODULE_TRUNK:
    return KF::STATEMENT_BRANCHES | KF::NONE; // TRUNK

  // HINTS
  case K::DEBUG_TRAP:
    return KF::STATEMENT;
  case K::UNREACHABLE:
    return KF::STATEMENT;
  case K::ASSUME:
    return KF::STATEMENT;

  // EXPRESSION ATTRIBUTES
  case K::OPAQUE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::OUTSIDE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::STATIC:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::CAPTURE:
    return KF::EXPRESSION_ATTRIBUTE | KF::REFLECTION | KF::UNIVERSALIZABLE |
           KF::RVALUE;
  case K::CAPTURE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EAGER:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::MAY_PARENT:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::PARENT:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::ABSTRACT:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::VIRTUAL:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::OVERRIDE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::POSITION:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::INLINE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::MANGLE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::PACK:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::LABEL:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::TEMPLATE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::LIKELY:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::UNLIKELY:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::DEPRECIATED:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::MAY_COPY:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::MAY_MOVE:
    return KF::EXPRESSION_ATTRIBUTE;
  case K::OK:
    return KF::EXPRESSION_ATTRIBUTE;

  // NODES
  case K::QUOTE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPRESSION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ENTITY_KIND:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EXPAND:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::EXPAND_STATEMENT:
    return KF::STATEMENT;
  case K::EXPAND_LVALUE:
    return KF::LVALUE;
  case K::EXPAND_RVALUE:
    return KF::RVALUE;
  case K::EXPAND_REFLECTION:
    return KF::REFLECTION;
  case K::EXPAND_ARGUMENT:
    return KF::ARGUMENT;
  case K::EXPAND_PARAMETER:
    return KF::PARAMETER;
  case K::EXPAND_SYMBOL_PATH:
    return KF::SYMBOL_PATH;
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return KF::ARITHMETIC_SEQUENCE_STEP | KF::ARITHMETIC_SEQUENCE_CONDITION;

  // REFLECTIONS
  case K::REFLECT:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::REFLECTION |
           KF::ARGUMENT | KF::PARAMETER | KF::SYMBOL_PATH |
           KF::ARITHMETIC_SEQUENCE_STEP | KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::MEMBER_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::SYMBOL_PATH | KF::STATEMENT;
  case K::MEMBER_OF_TOP:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::SYMBOL_PATH | KF::STATEMENT;
  case K::ASCEND_FRAME:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::SYMBOL_PATH;
  case K::ASCEND_FRAME_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::SYMBOL_PATH;
  case K::BYTE_SIZE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BYTE_SIZE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BIT_DEPTH:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::BIT_DEPTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ELEMENT_COUNT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::ELEMENT_COUNT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NAME:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LINE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::LINE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COLUMN:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::COLUMN_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_TYPE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HOLDS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HOLDS_ENUMERATOR:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::UNIVERSALIZABLE;
  case K::TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SYMBOL:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SYMBOL_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HAS_MEMBER:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HAS_MEMBER_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HAS:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HAS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::GET:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::GET_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SIGNATURE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SIGNATURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LAYOUT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::LAYOUT_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterMarkKeyword(rq::Keyword keyword) {
  return keyword == rq::Keyword::NAMED_PARAMETERS_BEGIN ||
         keyword == rq::Keyword::POSITIONAL_PARAMETERS_END;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralKeyword(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LITERAL);
}

[[nodiscard]] inline rq::Keyword
getSituatedAscribeKeyword(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_STATEMENT:
    return rq::Keyword::ASCRIBE_STATEMENT;
  case rq::Keyword::UNSITUATED_ASCRIBE_TYPE:
    return rq::Keyword::ASCRIBE_TYPE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedLeft(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_LEFT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnquotedRight(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_RIGHT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsConverging(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONVERGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasStatementBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExpressionBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasNone(flags, rq::KeywordFlags::STATEMENT_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsUniversalizable(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::UNIVERSALIZABLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::STARTING_CHAINLINK |
                                   rq::KeywordFlags::CONTINUING_CHAINLINK |
                                   rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeStartingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeContinuingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeFinishingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAllChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHAINLINK |
                                  rq::KeywordFlags::ARM_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeIfChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArmChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARM_CHAINLINK);
}


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

[[nodiscard]] inline rq::Keyword
getExpandOfSituation(rq::Situation situation) {
  using namespace rq;
  using K = Keyword;
  using S = Situation;
  switch (situation) {
  case S::NONE:
  case S::TRUNK:
    break;
  case S::STATEMENT:
    return K::EXPAND_STATEMENT;
  case S::LVALUE:
    return K::EXPAND_LVALUE;
  case S::RVALUE:
    return K::EXPAND_RVALUE;
  case S::REFLECTION:
    return K::EXPAND_REFLECTION;
  case S::ARGUMENT:
    return K::EXPAND_ARGUMENT;
  case S::PARAMETER:
    return K::EXPAND_PARAMETER;
  case S::BINDING:
    break;
  case S::SYMBOL_PATH:
    return K::EXPAND_SYMBOL_PATH;
  case S::ASCRIPTION:
  case S::TYPE_ATTRIBUTE:
  case S::EXPRESSION_ATTRIBUTE:
    break;
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return K::EXPAND_ARITHMETIC_SEQUENCE_STAGE;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation getSituationOfExpand(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using S = Situation;
  switch (keyword) {
  case K::EXPAND:
    return S::NONE;
  case K::EXPAND_STATEMENT:
    return S::STATEMENT;
  case K::EXPAND_LVALUE:
    return S::LVALUE;
  case K::EXPAND_RVALUE:
    return S::RVALUE;
  case K::EXPAND_REFLECTION:
    return S::REFLECTION;
  case K::EXPAND_ARGUMENT:
    return S::ARGUMENT;
  case K::EXPAND_PARAMETER:
    return S::PARAMETER;
  case K::EXPAND_SYMBOL_PATH:
    return S::SYMBOL_PATH;
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return S::ARITHMETIC_SEQUENCE_STAGE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Keyword getUniversalized(rq::Keyword keyword,
                                                     rq::Situation situation) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  // CASTS
  case K::AS:
    return K::LITERAL_AS;
  case K::OF:
    return K::ELEMENTS_OF;
  case K::CAST:
    return K::CAST_OF;
  case K::DOWN_CAST:
    return K::DOWN_CAST_OF;
  case K::FLOAT_CAST:
    return K::FLOAT_CAST_OF;
  case K::PLATFORM_CAST:
    return K::PLATFORM_CAST_OF;
  case K::STATIC_CAST:
    return K::STATIC_CAST_OF;
  case K::BITWISE_CAST:
    return K::BITWISE_CAST_OF;
  // MEMORY
  case K::CONTENT:
    return K::CONTENT_OF;
  case K::ADDRESS:
    return K::ADDRESS_OF;
  case K::BORROW:
    return K::BORROW_OF;
  case K::DATA_ADDRESS:
    return K::DATA_ADDRESS_OF;
  // PROCEDURES
  case K::DESTROY:
    return K::DESTROY_VALUE;
  case K::DROP:
    return K::DROP_VALUE;
  case K::MOVE:
    return K::MOVE_VALUE;
  // BUILTIN TYPES
  case K::SIGNED:
    return K::SIGNED_OF;
  case K::UNSIGNED:
    return K::UNSIGNED_OF;
  // VARIADIC ARGUMENTS
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::NEXT_VARIADIC_ARGUMENT_OF;
  // EXPANSIONS
  case K::EXPAND:
    return rq::getExpandOfSituation(situation);
  // EXPRESSION ATTRIBUTES
  case K::CAPTURE:
    return K::CAPTURE_OF;
  // REFLECTIONS
  case K::BYTE_SIZE:
    return K::BYTE_SIZE_OF;
  case K::BIT_DEPTH:
    return K::BIT_DEPTH_OF;
  case K::ELEMENT_COUNT:
    return K::ELEMENT_COUNT_OF;
  case K::NAME:
    return K::NAME_OF;
  case K::LINE:
    return K::LINE_OF;
  case K::COLUMN:
    return K::COLUMN_OF;
  case K::IS:
    return K::IS_TYPE;
  case K::HOLDS:
    return K::HOLDS_ENUMERATOR;
  case K::TYPE:
    return K::TYPE_OF;
  case K::SYMBOL:
    return K::SYMBOL_OF;
  case K::HAS_MEMBER:
    return K::HAS_MEMBER_OF;
  case K::HAS:
    return K::HAS_OF;
  case K::GET:
    return K::GET_OF;
  case K::SIGNATURE:
    return K::SIGNATURE_OF;
  case K::LAYOUT:
    return K::LAYOUT_OF;
  case K::SYNONYM:
    return K::SYNONYM_OF;
  case K::IS_OK:
    return K::IS_OK_OF;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation getAttributeSituation(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_STATEMENT:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_STATEMENT:
    return rq::Situation::EXPRESSION_ATTRIBUTE;
  case rq::Keyword::UNSITUATED_ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_ROOT_OF_VALUE:
    return rq::Situation::TYPE_ATTRIBUTE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTrunk(rq::Keyword keyword) {
  return keyword == rq::Keyword::MODULE_TRUNK;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeReflection(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::REFLECTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArgument(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeParameter(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeBinding(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::BINDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSymbolPath(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::SYMBOL_PATH);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTypeAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TYPE_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeExpressionAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::EXPRESSION_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStage(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags,
                        rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION |
                            rq::KeywordFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceCondition(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags,
                       rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStep(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] inline bool getCanBeSituation(rq::Keyword keyword,
                                            rq::Situation situation) {
  switch (situation) {
  case rq::Situation::NONE:
    return rq::getIsNone(keyword);
  case rq::Situation::TRUNK:
    return rq::getCanBeTrunk(keyword);
  case rq::Situation::STATEMENT:
    return rq::getCanBeStatement(keyword);
  case rq::Situation::LVALUE:
    return rq::getCanBeLValue(keyword);
  case rq::Situation::RVALUE:
    return rq::getCanBeRValue(keyword);
  case rq::Situation::REFLECTION:
    return rq::getCanBeReflection(keyword);
  case rq::Situation::ARGUMENT:
    return rq::getCanBeArgument(keyword);
  case rq::Situation::PARAMETER:
    return rq::getCanBeParameter(keyword);
  case rq::Situation::BINDING:
    return rq::getCanBeBinding(keyword);
  case rq::Situation::SYMBOL_PATH:
    return rq::getCanBeSymbolPath(keyword);
  case rq::Situation::ASCRIPTION:
    return rq::getCanBeAscription(keyword);
  case rq::Situation::TYPE_ATTRIBUTE:
    return rq::getCanBeTypeAttribute(keyword);
  case rq::Situation::EXPRESSION_ATTRIBUTE:
    return rq::getCanBeExpressionAttribute(keyword);
  case rq::Situation::ARITHMETIC_SEQUENCE_STAGE:
    return rq::getCanBeArithmeticSequenceStage(keyword);
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName(rq::Keyword keyword) {
  return keyword == rq::Keyword::IDENTIFY ||
         keyword == rq::Keyword::IDENTIFIER_LITERAL;
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

[[nodiscard]] inline rq::ChainKind getChainKind(rq::Keyword keyword) {
  if (!rq::getCanBeChainLink(keyword)) [[likely]] {
    return rq::ChainKind::NONE;
  } else if (rq::getCanBeAllChainLink(keyword)) {
    return rq::ChainKind::UNKNOWN;
  } else if (rq::getCanBeIfChainLink(keyword)) {
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
  OK
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
  case SA::OK:
    return "ok";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionAttribute
getExpressionAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using SA = ExpressionAttribute;
  switch (keyword) {
  case K::OPAQUE:
    return SA::OPAQUE;
  case K::OUTSIDE:
    return SA::OUTSIDE;
  case K::STATIC:
    return SA::STATIC;
  case K::CAPTURE:
    return SA::CAPTURE;
  case K::EAGER:
    return SA::EAGER;
  case K::MAY_PARENT:
    return SA::MAY_PARENT;
  case K::PARENT:
    return SA::PARENT;
  case K::ABSTRACT:
    return SA::ABSTRACT;
  case K::VIRTUAL:
    return SA::VIRTUAL;
  case K::OVERRIDE:
    return SA::OVERRIDE;
  case K::POSITION:
    return SA::POSITION;
  case K::MANGLE:
    return SA::MANGLE;
  case K::PACK:
    return SA::PACK;
  case K::LABEL:
    return SA::LABEL;
  case K::TEMPLATE:
    return SA::TEMPLATE;
  case K::LIKELY:
    return SA::LIKELY;
  case K::UNLIKELY:
    return SA::UNLIKELY;
  case K::DEPRECIATED:
    return SA::DEPRECIATED;
  case K::EXPORT:
    return SA::EXPORT;
  case K::PUBLIC:
    return SA::PUBLIC;
  case K::PROTECTED:
    return SA::PROTECTED;
  case K::MAY_COPY:
    return SA::MAY_COPY;
  case K::MAY_MOVE:
    return SA::MAY_MOVE;
  case K::OK:
    return SA::OK;
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
  OK = rq::getBit(9)
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
  case SA::OK:
    return SF::OK;
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

[[nodiscard]] inline bool getHasOk(rq::ExpressionAttributeFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionAttributeFlags::OK);
}

[[nodiscard]] inline bool getHasAttribute(rq::ExpressionAttributeFlags flags,
                                          rq::ExpressionAttribute attribute) {
  rq::ExpressionAttributeFlags attribute_flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, attribute_flags);
}

struct Expression;
struct SymbolTableSymbol;

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
  [[nodiscard]] bool getHasOk() const {
    return rq::getHasOk(this->_flags);
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
  INDETERMINATE
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
  case TA::INDETERMINATE:
    return "indeterminate";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::TypeAttribute getTypeAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using TA = TypeAttribute;
  switch (keyword) {
  case K::MUTABLE:
    return TA::MUTABLE;
  case K::CONSTANT:
    return TA::CONSTANT;
  case K::PARTIALLY_MUTABLE:
    return TA::PARTIALLY_MUTABLE;
  case K::VOLATILE:
    return TA::VOLATILE;
  case K::ATOMIC:
    return TA::ATOMIC;
  case K::NULL_TERMINATED:
    return TA::NULL_TERMINATED;
  case K::MAY_DISCARD:
    return TA::MAY_DISCARD;
  case K::INDETERMINATE:
    return TA::INDETERMINATE;
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
  INDETERMINATE = rq::getBit(8)
};

template <> struct is_flags<TypeAttributeFlags> : std::true_type {};

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
  case TA::INDETERMINATE:
    return TF::INDETERMINATE;
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

[[nodiscard]] inline bool getHasIndeterminate(rq::TypeAttribute attribute) {
  rq::TypeAttributeFlags flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, rq::TypeAttributeFlags::INDETERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAttribute(rq::TypeAttributeFlags flags, rq::TypeAttribute attribute) {
  return rq::getHasAll(flags, rq::getFlags(attribute));
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasMutability(rq::TypeAttributeFlags flags) {
  return rq::getHasSome(flags, rq::TypeAttributeFlags::MUTABLE |
                                   rq::TypeAttributeFlags::CONSTANT);
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
getArithmeticSequenceStep(rq::Keyword keyword) {
  using namespace rq;
  using ASS = ArithmeticSequenceStep;
  using K = Keyword;
  switch (keyword) {
  case K::ARITHMETIC_SEQUENCE_STEP_ADD:
    return ASS::ADD;
  case K::ARITHMETIC_SEQUENCE_STEP_SUBTRACT:
    return ASS::SUBTRACT;
  case K::ARITHMETIC_SEQUENCE_STEP_MULTIPLY:
    return ASS::MULTIPLY;
  case K::ARITHMETIC_SEQUENCE_STEP_DIVIDE:
    return ASS::DIVIDE;
  case K::ARITHMETIC_SEQUENCE_STEP_MODULUS:
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
getArithmeticSequenceCondition(rq::Keyword keyword) {
  using namespace rq;
  using ASC = ArithmeticSequenceCondition;
  using K = Keyword;
  switch (keyword) {
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS:
    return ASC::LESS;
  case K::ARITHMETIC_SEQUENCE_CONDITION_LESS_EQUAL:
    return ASC::LESS_EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER:
    return ASC::GREATER;
  case K::ARITHMETIC_SEQUENCE_CONDITION_GREATER_EQUAL:
    return ASC::GREATER_EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_EQUAL:
    return ASC::EQUAL;
  case K::ARITHMETIC_SEQUENCE_CONDITION_NOT_EQUAL:
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

struct Expression final {
  using Self = rq::Expression;

  rq::Keyword _keyword = rq::Keyword::NONE;
  rq::PtrWithFlags<rq::Expression, 2, rq::ExpressionNextFlags>
      _next_ptr_flags{};
  rq::Expression *_branch_ptr{nullptr};
  rq::PtrWithFlags<const char, 1, rq::ExpressionSourceFlags>
      _source_ptr_flags{};
  unsigned _source_text_length{0};

  Expression() = default;
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getKeyword() const {
    return this->_keyword;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getName() const {
    return rq::getName(this->_keyword);
  }
  RQ_ALWAYS_INLINE void clear() {
    this->_keyword = rq::Keyword::NONE;
    this->_next_ptr_flags = {};
    this->_branch_ptr = nullptr;
    this->_source_ptr_flags = {};
    this->_source_text_length = 0;
  }
  RQ_ALWAYS_INLINE void setKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword == rq::Keyword::NONE,
              "keyword must not already be set");
    this->_keyword = keyword;
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword != rq::Keyword::NONE,
              "keyword must already be set");
    this->_keyword = keyword;
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getSituatedAscribe() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword
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
  case rq::Keyword::CAPTURE:
    this->_capture_ptr = &expression;
    break;
  case rq::Keyword::OVERRIDE:
    this->_override_ptr = &expression;
    break;
  case rq::Keyword::POSITION:
    this->_position_ptr = &expression;
    break;
  case rq::Keyword::MANGLE:
    this->_mangle_ptr = &expression;
    break;
  case rq::Keyword::LABEL:
    this->_label_ptr = &expression;
    break;
  case rq::Keyword::TEMPLATE:
    this->_template_ptr = &expression;
    break;
  case rq::Keyword::DEPRECIATED:
    this->_depreciated_ptr = &expression;
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

} // namespace rq