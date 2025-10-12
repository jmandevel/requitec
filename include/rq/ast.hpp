#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstdint>

namespace rq {

// NOTE:
//  two underscores at the front means that the keyword is for internal use only

// NOTE:
//  one underscore at the front means that the keyword is for symbolic requite
//  source code only and cannot be used in normative requite.

enum class Keyword : std::uint32_t {
  // this should never occur.
  __NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  __INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  __FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  __STRING_LITERAL,
  // left string interpolation (string literal with no end quote mark).
  __LEFT_INTERPOLATION_LITERAL,
  // middle string interpolation (string literal with no quote marks).
  __MIDDLE_INTERPOLATION_LITERAL,
  // right string interpolation (string literal with start quote mark).
  __RIGHT_INTERPOLATION_LITERAL,
  // a literal that represents a single text character.
  __CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  __IDENTIFIER_LITERAL,

  // ERRORS
  __ERROR,

  // SITUATIONAL
  _PARENTHESIS_GROUP,
  _EQUAL_OPERATOR,
  _COLON_OPERATOR,
  _DOUBLE_COLON_OPERATOR,
  _INFERENCE,
  _TACIT_COMMA_EXPRESSION,

  // LOGICAL
  _LOGICAL_AND,
  _LOGICAL_OR,
  _LOGICAL_COMPLEMENT,

  // COMPARISON
  _GREATER,
  _GREATER_EQUAL,
  _LESS,
  _LESS_EQUAL,
  _EQUAL,
  _NOT_EQUAL,

  // APPLY
  _EXTEND,
  _ORDERED_BINDING,
  _UNORDERED_BINDING,
  _ASCRIBE_TYPE,
  _ASCRIBE_STATEMENT,
  _ASCRIBE_TYPE_OF_ELEMENTS,
  _CAST,
  _IDENTIFY,

  // ARITHMETIC
  _ADD,
  _SUBTRACT,
  _MULTIPLY,
  _DIVIDE,
  _MODULUS,
  _NEGATE,

  // BITWISE
  _BITWISE_CAST,
  _BITWISE_OR,
  _BITWISE_AND,
  _BITWISE_XOR,
  _BITWISE_COMPLEMENT,
  _BITWISE_SHIFT_LEFT,
  _BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  // MEMORY
  _CONCATINATE,
  SINGLETON,
  _SINGLETON_OF,
  CONTENT,
  _CONTENT_OF,
  ADDRESS,
  _ADDRESS_OF,
  BORROW,
  _BORROW_OF,
  VIEW,
  _VIEW_OF,
  SLICE,
  _SLICE_OF,
  ARRAY_SLICE,
  _ARRAY_SLICE_OF,

  // ASSIGNMENT
  _ASSIGN,
  _ASSIGN_ADD,
  _ASSIGN_SUBTRACT,
  _ASSIGN_MULTIPLY,
  _ASSIGN_DIVIDE,
  _ASSIGN_MODULUS,

  // MOVE SEMANTICS
  DEEP_COPY,
  _DEEP_COPY_OF,
  RETAIN_MOVE,
  _RETAIN_MOVE_OF,
  DROP_MOVE,
  _DROP_MOVE_OF,
  LINEAR_ASSIGN,
  _LINEAR_ASSIGN_OF,
  SWAP,

  // SUBTYPE
  _ARRAY,
  _REFERENCE,
  _POINTER,
  _FAT_POINTER,

  // TYPE MODIFIER
  MUTABLE,
  CONSTANT,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  MAY_DISCARD,
  DEBUG_TRAP_ON_PANIC,
  LINEAR,
  DYNAMIC_CAPTURE_LAYOUT,

  // PARAMETER RULES
  _POSITIONAL_PARAMETERS_END,
  _NAMED_PARAMETERS_BEGIN,

  // BRACES
  _TUPLE,
  _LAYOUT,
  _NULL,
  _NULL_TYPE,
  _STRUCTURED_BINDING,
  _IGNORE,
  _SPECIALIZATION,

  // PROCEDURES
  _CALL,
  _NAMED_ARGUMENT,
  _INDEX,
  _SIGNATURE,
  _DEFAULT_VALUE_PARAMETER,
  DESTROY,
  _DESTROY_VALUE,
  DROP,
  _DROP_VALUE,
  ENTRY_POINT,
  FUNCTION,
  CONSTRUCTOR,
  DESTRUCTOR,
  RANGER,
  DEEP_COPIER,
  RETAIN_MOVER,
  DROP_MOVER,
  SWAPER,
  INDEXER,
  _ANONYMOUS_FUNCTION,
  _DYNAMIC_CAPTURE,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  RANGE_OVER,

  // SYMBOLS
  OBJECT,
  ENUMERATION,
  _ENUMERATION_VALUE_WITH_DISCRIMINANT,

  // VALUES
  TRUE,
  FALSE,
  INDETERMINATE,
  THIS,
  // value returned from a function.
  RESULT,
  // retrieve command line arguments within entry_point.
  COMMAND_LINE_ARGUMENTS,
  // value returned into a block.
  IN,
  // value returned from a block.
  OUT,
  // the byte size of memory addresses on the current architecture.
  ADDRESS_SIZE,
  // the bit size of memory addresses on the current architecture.
  ADDRESS_DEPTH,
  // the amount of bits per byte on the current architecture. (almost always 8)
  BITS_PER_BYTE,

  // BUILTIN TYPES
  SELF,
  VOID,
  NO_RETURN,
  BOOLEAN,
  // has branch with depth. if none specified, it uses bits_per_byte.
  WORD,
  // has branch with depth. if none is specified, it uses address_depth.
  SIGNED,
  // has branch with depth. if none is specified, it uses address_depth.
  UNSIGNED,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  UTF8,
  // the rest are C analogs for portability
  C_CHAR,
  C_UNSIGNED_CHAR,
  C_SIGNED_CHAR,
  C_WCHAR_T,
  C_SHORT_INT,
  C_UNSIGNED_SHORT_INT,
  C_INT,
  C_UNSIGNED_INT,
  C_LONG_INT,
  C_UNSIGNED_LONG_INT,
  C_LONG_LONG_INT,
  C_UNSIGNED_LONG_LONG_INT,
  C_FLOAT,
  C_DOUBLE,
  C_LONG_DOUBLE,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS,
  FIRST_VARIADIC_ARGUMENT,
  _FIRST_VARIADIC_ARGUMENT_OF,
  NEXT_VARIADIC_ARGUMENT,
  _NEXT_VARIADIC_ARGUMENT_OF,

  // SCOPES
  IF,
  ELSE_IF,
  ELSE,
  MATCH,
  INLINE_MATCH,
  SWITCH,
  CASE,
  DEFAULT,
  FOR,
  FOR_EACH,
  WHILE,
  SCOPE,
  INLINE_SCOPE,
  BLOCK,
  INLINE_BLOCK,

  // RANGES
  RANGE,
  CONTAINS,
  _CONTAINS_VALUE,
  _SEQUENCE,
  _SEQUENCE_STEP_ADD,
  _SEQUENCE_STEP_SUBTRACT,
  _SEQUENCE_STEP_MULTIPLY,
  _SEQUENCE_STEP_DIVIDE,
  _SEQUENCE_STEP_MODULUS,
  _SEQUENCE_WHILE_LESS,
  _SEQUENCE_WHILE_GREATER,
  _SEQUENCE_WHILE_LESS_EQUAL,
  _SEQUENCE_WHILE_GREATER_EQUAL,
  _SEQUENCE_WHILE_EQUAL,
  _SEQUENCE_WHILE_NOT_EQUAL,

  // ACCESS MODIFIERS
  PRIVATE,
  PROTECTED,
  EXPORT,

  // SYMBOL GRAPH
  IMPORT,
  USE,
  USE_TABLE,
  TABLE,

  // ERROR HANDLING AND DEBUGGING
  PANIC_TRAP,
  DEBUG_TRAP,

  // HINTS
  UNREACHABLE,
  ASSUME,

  // ATTRIBUTES
  BAKE,
  STATIC_CAPTURE,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  INLINE,
  MANGLED_NAME,
  _MANGLED_NAME_OF,
  PACK,
  USER,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  NO_SHALLOW_COPY,

  // REFLECTIONS
  _REFLECT,
  _MEMBER_OF,
  SIZE,
  _SIZE_OF,
  DEPTH,
  _DEPTH_OF,
  COUNT,
  _COUNT_OF,
  LENGTH,
  _LENGTH_OF,
  SPAN_SIZE,
  _SPAN_SIZE_OF,
  SPAN_DEPTH,
  _SPAN_DEPTH_OF,
  NAME,
  _NAME_OF,
  LINE,
  _LINE_OF,
  COLUMN,
  _COLUMN_OF,
  IS,
  _VALUE_IS,
  HOLDS,
  _VALUE_HOLDS,
  TYPE,
  _TYPE_OF,
  SYMBOL,
  _SYMBOL_OF,
  DISCRIMINANT,
  _DISCRIMINANT_OF,

  __LAST
};

constexpr std::size_t KEYWORD_COUNT =
    static_cast<std::size_t>(rq::Keyword::__LAST);

[[nodiscard]] inline constexpr llvm::StringRef getName(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::__NONE:
    return "__none";

  // LITERALS
  case K::__INTEGER_LITERAL:
    return "__integer_literal";
  case K::__FLOAT_LITERAL:
    return "__float_literal";
  case K::__STRING_LITERAL:
    return "__string_literal";
  case K::__LEFT_INTERPOLATION_LITERAL:
    return "__left_interpolation_literal";
  case K::__MIDDLE_INTERPOLATION_LITERAL:
    return "__middle_interpolation_literal";
  case K::__RIGHT_INTERPOLATION_LITERAL:
    return "__right_interpolation_literal";
  case K::__CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case K::__IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case K::__ERROR:
    return "__error";

  // SITUATIONAL
  case K::_PARENTHESIS_GROUP:
    return "_parenthesis_group";
  case K::_EQUAL_OPERATOR:
    return "_equal_operator";
  case K::_COLON_OPERATOR:
    return "_colon_operator";
  case K::_DOUBLE_COLON_OPERATOR:
    return "_double_colon_operator";
  case K::_INFERENCE:
    return "_inference";
  case K::_TACIT_COMMA_EXPRESSION:
    return "_tacit_comma_expression";

  // LOGICAL
  case K::_LOGICAL_AND:
    return "_logical_and";
  case K::_LOGICAL_OR:
    return "_logical_or";
  case K::_LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case K::_GREATER:
    return "_greater";
  case K::_GREATER_EQUAL:
    return "_greater_equal";
  case K::_LESS:
    return "_less";
  case K::_LESS_EQUAL:
    return "_less_equal";
  case K::_EQUAL:
    return "_equal";
  case K::_NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case K::_EXTEND:
    return "_extend";
  case K::_ORDERED_BINDING:
    return "_ordered_binding";
  case K::_UNORDERED_BINDING:
    return "_unordered_binding";
  case K::_ASCRIBE_TYPE:
    return "_ascribe_type";
  case K::_ASCRIBE_STATEMENT:
    return "_ascribe_statement";
  case K::_ASCRIBE_TYPE_OF_ELEMENTS:
    return "_ascribe_type_of_elements";
  case K::_CAST:
    return "_cast";
  case K::_IDENTIFY:
    return "_identify";

  // ARITHMETIC
  case K::_ADD:
    return "_add";
  case K::_SUBTRACT:
    return "_subtract";
  case K::_MULTIPLY:
    return "_multiply";
  case K::_DIVIDE:
    return "_divide";
  case K::_MODULUS:
    return "_modulus";
  case K::_NEGATE:
    return "_negate";

  // BITWISE
  case K::_BITWISE_CAST:
    return "_bitwise_cast";
  case K::_BITWISE_OR:
    return "_bitwise_or";
  case K::_BITWISE_AND:
    return "_bitwise_and";
  case K::_BITWISE_XOR:
    return "_bitwise_xor";
  case K::_BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case K::_BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case K::_BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case K::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case K::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case K::_CONCATINATE:
    return "_concatinate";
  case K::SINGLETON:
    return "singleton";
  case K::_SINGLETON_OF:
    return "_singleton_of";
  case K::CONTENT:
    return "content";
  case K::_CONTENT_OF:
    return "_content_of";
  case K::ADDRESS:
    return "address";
  case K::_ADDRESS_OF:
    return "_address_of";
  case K::BORROW:
    return "borrow";
  case K::_BORROW_OF:
    return "_borrow_of";
  case K::VIEW:
    return "view";
  case K::_VIEW_OF:
    return "_view_of";
  case K::SLICE:
    return "slice";
  case K::_SLICE_OF:
    return "_slice_of";
  case K::ARRAY_SLICE:
    return "array_slice";
  case K::_ARRAY_SLICE_OF:
    return "_array_slice_of";

  // ASSIGNMENT
  case K::_ASSIGN:
    return "_assign";
  case K::_ASSIGN_ADD:
    return "_assign_add";
  case K::_ASSIGN_SUBTRACT:
    return "_assign_subtract";
  case K::_ASSIGN_MULTIPLY:
    return "_assign_multiply";
  case K::_ASSIGN_DIVIDE:
    return "_assign_divide";
  case K::_ASSIGN_MODULUS:
    return "_assign_modulus";

  // MOVE SEMANTICS
  case K::DEEP_COPY:
    return "deep_copy";
  case K::_DEEP_COPY_OF:
    return "_deep_copy_of";
  case K::RETAIN_MOVE:
    return "retain_move";
  case K::_RETAIN_MOVE_OF:
    return "_move_of";
  case K::DROP_MOVE:
    return "drop_move";
  case K::_DROP_MOVE_OF:
    return "_drop_move_of";
  case K::LINEAR_ASSIGN:
    return "linear_assign";
  case K::_LINEAR_ASSIGN_OF:
    return "_linear_assign_of";
  case K::SWAP:
    return "swap";

  // SUBTYPE
  case K::_ARRAY:
    return "_array";
  case K::_REFERENCE:
    return "_reference";
  case K::_POINTER:
    return "_pointer";
  case K::_FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case K::MUTABLE:
    return "mutable";
  case K::CONSTANT:
    return "constant";
  case K::VOLATILE:
    return "volatile";
  case K::ATOMIC:
    return "atomic";
  case K::NULL_TERMINATED:
    return "null_terminated";
  case K::MAY_DISCARD:
    return "may_discard";
  case K::DEBUG_TRAP_ON_PANIC:
    return "debug_trap_on_panic";
  case K::LINEAR:
    return "linear";
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return "dynamic_capture_layout";

  // PARAMETER RULES
  case K::_POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case K::_NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";

  // BRACES
  case K::_TUPLE:
    return "_tuple";
  case K::_LAYOUT:
    return "_layout";
  case K::_NULL:
    return "_null";
  case K::_NULL_TYPE:
    return "_null_type";
  case K::_STRUCTURED_BINDING:
    return "_structured_binding";
  case K::_IGNORE:
    return "_ignore";
  case K::_SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case K::_CALL:
    return "_call";
  case K::_NAMED_ARGUMENT:
    return "_named_argument";
  case K::_INDEX:
    return "_index";
  case K::_SIGNATURE:
    return "_signature";
  case K::_DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case K::DESTROY:
    return "destroy";
  case K::_DESTROY_VALUE:
    return "_destroy_value";
  case K::DROP:
    return "drop";
  case K::_DROP_VALUE:
    return "_drop_value";
  case K::ENTRY_POINT:
    return "entry_point";
  case K::FUNCTION:
    return "function";
  case K::CONSTRUCTOR:
    return "constructor";
  case K::DESTRUCTOR:
    return "destructor";
  case K::RANGER:
    return "ranger";
  case K::DEEP_COPIER:
    return "deep_copier";
  case K::RETAIN_MOVER:
    return "retain_mover";
  case K::DROP_MOVER:
    return "drop_mover";
  case K::SWAPER:
    return "swaper";
  case K::INDEXER:
    return "indexer";
  case K::_ANONYMOUS_FUNCTION:
    return "_anonymous_function";
  case K::_DYNAMIC_CAPTURE:
    return "_dynamic_capture";

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

  // SYMBOLS
  case K::OBJECT:
    return "object";
  case K::ENUMERATION:
    return "enumeration";
  case K::_ENUMERATION_VALUE_WITH_DISCRIMINANT:
    return "_enumeration_value_with_discriminant";

  // VALUES
  case K::TRUE:
    return "true";
  case K::FALSE:
    return "false";
  case K::INDETERMINATE:
    return "indeterminate";
  case K::THIS:
    return "this";
  case K::RESULT:
    return "result";
  case K::COMMAND_LINE_ARGUMENTS:
    return "command_line_arguments";
  case K::IN:
    return "in";
  case K::OUT:
    return "out";
  case K::ADDRESS_SIZE:
    return "address_size";
  case K::ADDRESS_DEPTH:
    return "address_depth";
  case K::BITS_PER_BYTE:
    return "bits_per_byte";

  // BUILTIN TYPES
  case K::SELF:
    return "self";
  case K::VOID:
    return "void";
  case K::NO_RETURN:
    return "no_return";
  case K::BOOLEAN:
    return "boolean";
  case K::WORD:
    return "word";
  case K::SIGNED:
    return "signed";
  case K::UNSIGNED:
    return "unsigned";
  case K::BFLOAT16:
    return "bfloat16";
  case K::BINARY16:
    return "binary16";
  case K::BINARY32:
    return "binary32";
  case K::BINARY64:
    return "binary64";
  case K::BINARY128:
    return "binary128";
  case K::UTF8:
    return "utf8";
  case K::C_CHAR:
    return "c_char";
  case K::C_UNSIGNED_CHAR:
    return "c_unsigned_char";
  case K::C_SIGNED_CHAR:
    return "c_signed_char";
  case K::C_WCHAR_T:
    return "c_wchar_t";
  case K::C_SHORT_INT:
    return "c_short_int";
  case K::C_UNSIGNED_SHORT_INT:
    return "c_unsigned_short_int";
  case K::C_INT:
    return "c_int";
  case K::C_UNSIGNED_INT:
    return "c_unsigned_int";
  case K::C_LONG_INT:
    return "c_long_int";
  case K::C_UNSIGNED_LONG_INT:
    return "c_unsigned_long_int";
  case K::C_LONG_LONG_INT:
    return "c_long_long_int";
  case K::C_UNSIGNED_LONG_LONG_INT:
    return "c_unsigned_long_long_int";
  case K::C_FLOAT:
    return "c_float";
  case K::C_DOUBLE:
    return "c_double";
  case K::C_LONG_DOUBLE:
    return "c_long_double";

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case K::FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case K::_FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case K::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case K::_NEXT_VARIADIC_ARGUMENT_OF:
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
  case K::CASE:
    return "case";
  case K::DEFAULT:
    return "default";
  case K::FOR:
    return "for";
  case K::FOR_EACH:
    return "for_each";
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
  case K::CONTAINS:
    return "contains";
  case K::_CONTAINS_VALUE:
    return "_contains_value";
  case K::_SEQUENCE:
    return "_sequence";
  case K::_SEQUENCE_STEP_ADD:
    return "_sequence_step_add";
  case K::_SEQUENCE_STEP_SUBTRACT:
    return "_sequence_step_subtract";
  case K::_SEQUENCE_STEP_MULTIPLY:
    return "_sequence_step_multiply";
  case K::_SEQUENCE_STEP_DIVIDE:
    return "_sequence_step_divide";
  case K::_SEQUENCE_STEP_MODULUS:
    return "_sequence_step_modulus";
  case K::_SEQUENCE_WHILE_LESS:
    return "_sequence_while_less";
  case K::_SEQUENCE_WHILE_GREATER:
    return "_sequence_while_greater";
  case K::_SEQUENCE_WHILE_LESS_EQUAL:
    return "_sequence_while_less_equal";
  case K::_SEQUENCE_WHILE_GREATER_EQUAL:
    return "_sequence_while_greater_equal";
  case K::_SEQUENCE_WHILE_EQUAL:
    return "_sequence_while_equal";
  case K::_SEQUENCE_WHILE_NOT_EQUAL:
    return "_sequence_while_not_equal";

  // ACCESS MODIFIERS
  case K::PRIVATE:
    return "private";
  case K::PROTECTED:
    return "protected";
  case K::EXPORT:
    return "export";

  // SYMBOL GRAPH
  case K::IMPORT:
    return "import";
  case K::USE:
    return "use";
  case K::USE_TABLE:
    return "use_table";
  case K::TABLE:
    return "table";

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    return "panic_trap";
  case K::DEBUG_TRAP:
    return "debug_trap";

  // HINTS
  case K::UNREACHABLE:
    return "unreachable";
  case K::ASSUME:
    return "assume";

  // ATTRIBUTES
  case K::BAKE:
    return "bake";
  case K::STATIC_CAPTURE:
    return "static_capture";
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
  case K::MANGLED_NAME:
    return "mangled_name";
  case K::_MANGLED_NAME_OF:
    return "_mangled_name_of";
  case K::PACK:
    return "pack";
  case K::USER:
    return "user";
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
  case K::NO_SHALLOW_COPY:
    return "no_shallow_copy";

  // REFLECTIONS
  case K::_REFLECT:
    return "_reflect";
  case K::_MEMBER_OF:
    return "_member_of";
  case K::SIZE:
    return "size";
  case K::_SIZE_OF:
    return "_size_of";
  case K::DEPTH:
    return "depth";
  case K::_DEPTH_OF:
    return "_depth_of";
  case K::COUNT:
    return "count";
  case K::_COUNT_OF:
    return "_count_of";
  case K::LENGTH:
    return "length";
  case K::_LENGTH_OF:
    return "_length_of";
  case K::SPAN_SIZE:
    return "span_size";
  case K::_SPAN_SIZE_OF:
    return "_span_size_of";
  case K::SPAN_DEPTH:
    return "span_depth";
  case K::_SPAN_DEPTH_OF:
    return "_span_depth_of";
  case K::NAME:
    return "name";
  case K::_NAME_OF:
    return "_name_of";
  case K::LINE:
    return "line";
  case K::_LINE_OF:
    return "_line_of";
  case K::COLUMN:
    return "column";
  case K::_COLUMN_OF:
    return "_column_of";
  case K::IS:
    return "is";
  case K::_VALUE_IS:
    return "_value_is";
  case K::HOLDS:
    return "holds";
  case K::_VALUE_HOLDS:
    return "_value_holds";
  case K::TYPE:
    return "type";
  case K::_TYPE_OF:
    return "_type_of";
  case K::SYMBOL:
    return "symbol";
  case K::_SYMBOL_OF:
    return "_symbol_of";
  case K::DISCRIMINANT:
    return "discriminant";
  case K::_DISCRIMINANT_OF:
    return "_discriminant_of";

  case K::__LAST:
    return "__last";
  }

  return "_unknown";
}

enum class KeywordFlags : std::uint32_t {
  NONE = 0,
  CONVERGING = rq::getBit(31),
  LITERAL = rq::getBit(30),
  UNQUOTED_LEFT = rq::getBit(29),
  UNQUOTED_RIGHT = rq::getBit(28),
  SYMBOLIC = rq::getBit(27),
  INTERNAL = rq::getBit(26),
  CAN_HAVE_NO_SEMICOLON = rq::getBit(25),
  HAS_SEMICOLON_SEPARATED_BRANCHES = rq::getBit(24),
  TOP_STATEMENT = rq::getBit(23),
  TABLE_STATEMENT = rq::getBit(22),
  OBJECT_STATEMENT = rq::getBit(21),
  LOCAL_STATEMENT = rq::getBit(20),
  RVALUE = rq::getBit(19),
  LVALUE = rq::getBit(18),
  REFLECTION = rq::getBit(17),
  ARGUMENT = rq::getBit(15),
  PARAMETER = rq::getBit(14),
  ENUMERATION_VALUE = rq::getBit(13),
  PATH = rq::getBit(12),
  NAME = rq::getBit(11),
  ASCRIPTION = rq::getBit(10),
  TYPE_ATTRIBUTE = rq::getBit(9),
  STATEMENT_ATTRIBUTE = rq::getBit(8),
  SEQUENCE_STAGE = rq::getBit(7),
  ARM = rq::getBit(6),
  DYNAMIC_CAPTURE = rq::getBit(5),
  COMMA_BRANCH_COUNT_MASK = 0x3,
  ALL = TOP_STATEMENT | TABLE_STATEMENT | OBJECT_STATEMENT | LOCAL_STATEMENT |
        RVALUE | LVALUE | REFLECTION | ARGUMENT | PARAMETER |
        ENUMERATION_VALUE | PATH | NAME | ASCRIPTION | TYPE_ATTRIBUTE |
        STATEMENT_ATTRIBUTE | SEQUENCE_STAGE | ARM | DYNAMIC_CAPTURE
};

template <> struct is_flags<rq::KeywordFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::KeywordFlags
getFlags(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using KF = KeywordFlags;
  switch (keyword) {
  case K::__NONE:
    return KF::NONE;

  // LITERALS
  case K::__INTEGER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::__FLOAT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::__STRING_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::__LEFT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::__MIDDLE_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL |
           KF::RVALUE | KF::ARGUMENT;
  case K::__RIGHT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::__CODEUNIT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::__IDENTIFIER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER | KF::PATH | KF::NAME;

  // ERRORS
  case K::__ERROR:
    return KF::INTERNAL;

  // SITUATIONAL
  case K::_PARENTHESIS_GROUP:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT |
           KF::LVALUE | KF::PATH | KF::NAME | KF::SEQUENCE_STAGE;
  case K::_EQUAL_OPERATOR:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::ARGUMENT |
           KF::PARAMETER | KF::ENUMERATION_VALUE;
  case K::_COLON_OPERATOR:
    return KF::SYMBOLIC | KF::RVALUE | KF::LVALUE | KF::ARGUMENT |
           KF::PARAMETER | KF::ENUMERATION_VALUE;
  case K::_DOUBLE_COLON_OPERATOR:
    return KF::SYMBOLIC | KF::RVALUE | KF::LVALUE | KF::ARGUMENT |
           KF::PARAMETER;
  case K::_INFERENCE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_TACIT_COMMA_EXPRESSION:
    return KF::RVALUE;

  // LOGICAL
  case K::_LOGICAL_AND:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LOGICAL_OR:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LOGICAL_COMPLEMENT:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // COMPARISON
  case K::_GREATER:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_GREATER_EQUAL:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LESS:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LESS_EQUAL:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_EQUAL:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_NOT_EQUAL:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // APPLY
  case K::_EXTEND:
    return KF::SYMBOLIC | KF::RVALUE;
  case K::_ORDERED_BINDING:
    return KF::SYMBOLIC | KF::LVALUE | KF::PARAMETER | KF::ARGUMENT |
           KF::ENUMERATION_VALUE;
  case K::_UNORDERED_BINDING:
    return KF::SYMBOLIC | KF::LVALUE;
  case K::_ASCRIBE_TYPE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::REFLECTION | KF::ASCRIPTION;
  case K::_ASCRIBE_STATEMENT:
    return KF::SYMBOLIC | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::PARAMETER | KF::ASCRIPTION;
  case K::_ASCRIBE_TYPE_OF_ELEMENTS:
    return KF::SYMBOLIC | KF::RVALUE | KF::ASCRIPTION;
  case K::_CAST:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_IDENTIFY:
    return KF::SYMBOLIC | KF::PATH | KF::NAME | KF::RVALUE | KF::ARGUMENT;

  // ARITHMETIC
  case K::_ADD:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_SUBTRACT:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_MULTIPLY:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_DIVIDE:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_MODULUS:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_NEGATE:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // BITWISE
  case K::_BITWISE_CAST:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_OR:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_AND:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_XOR:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_COMPLEMENT:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_SHIFT_LEFT:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_SHIFT_RIGHT:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;

  // MEMORY
  case K::_CONCATINATE:
    return KF::SYMBOLIC | KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::SINGLETON:
    return KF::REFLECTION;
  case K::_SINGLETON_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::CONTENT:
    return KF::REFLECTION;
  case K::_CONTENT_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::ADDRESS:
    return KF::REFLECTION;
  case K::_ADDRESS_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::BORROW:
    return KF::REFLECTION;
  case K::_BORROW_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::VIEW:
    return KF::REFLECTION;
  case K::_VIEW_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::SLICE:
    return KF::REFLECTION;
  case K::_SLICE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::ARRAY_SLICE:
    return KF::REFLECTION;
  case K::_ARRAY_SLICE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;

  // ASSIGNMENT
  case K::_ASSIGN:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_ASSIGN_ADD:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_ASSIGN_SUBTRACT:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_ASSIGN_MULTIPLY:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_ASSIGN_DIVIDE:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_ASSIGN_MODULUS:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;

  // MOVE SEMANTICS
  case K::DEEP_COPY:
    return KF::REFLECTION;
  case K::_DEEP_COPY_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::RETAIN_MOVE:
    return KF::REFLECTION;
  case K::_RETAIN_MOVE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::DROP_MOVE:
    return KF::REFLECTION;
  case K::_DROP_MOVE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::LINEAR_ASSIGN:
    return KF::REFLECTION;
  case K::_LINEAR_ASSIGN_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::SWAP:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

  // SUBTYPE
  case K::_ARRAY:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_REFERENCE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_POINTER:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_FAT_POINTER:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // TYPE MODIFIER
  case K::MUTABLE:
    return KF::TYPE_ATTRIBUTE;
  case K::CONSTANT:
    return KF::TYPE_ATTRIBUTE;
  case K::VOLATILE:
    return KF::TYPE_ATTRIBUTE;
  case K::ATOMIC:
    return KF::TYPE_ATTRIBUTE;
  case K::NULL_TERMINATED:
    return KF::TYPE_ATTRIBUTE;
  case K::MAY_DISCARD:
    return KF::TYPE_ATTRIBUTE;
  case K::DEBUG_TRAP_ON_PANIC:
    return KF::TYPE_ATTRIBUTE;
  case K::LINEAR:
    return KF::TYPE_ATTRIBUTE;
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return KF::TYPE_ATTRIBUTE;

  // PARAMETER RULES
  case K::_POSITIONAL_PARAMETERS_END:
    return KF::SYMBOLIC | KF::PARAMETER;
  case K::_NAMED_PARAMETERS_BEGIN:
    return KF::SYMBOLIC | KF::PARAMETER;

  // BRACES
  case K::_TUPLE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_LAYOUT:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_NULL:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_NULL_TYPE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_STRUCTURED_BINDING:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_IGNORE:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::_SPECIALIZATION:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::_CALL:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::RVALUE |
           KF::LVALUE | KF::ARGUMENT;
  case K::_NAMED_ARGUMENT:
    return KF::SYMBOLIC | KF::ARGUMENT;
  case K::_INDEX:
    return KF::SYMBOLIC | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::_SIGNATURE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_DEFAULT_VALUE_PARAMETER:
    return KF::SYMBOLIC | KF::PARAMETER;
  case K::DESTROY:
    return KF::REFLECTION;
  case K::_DESTROY_VALUE:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::DROP:
    return KF::REFLECTION;
  case K::_DROP_VALUE:
    return KF::SYMBOLIC | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::ENTRY_POINT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::TOP_STATEMENT;
  case K::FUNCTION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 2;
  case K::CONSTRUCTOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT | 2;
  case K::DESTRUCTOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT;
  case K::RANGER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT | 1;
  case K::DEEP_COPIER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT;
  case K::RETAIN_MOVER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT;
  case K::DROP_MOVER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT;
  case K::SWAPER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT;
  case K::INDEXER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::OBJECT_STATEMENT | 1;
  case K::_ANONYMOUS_FUNCTION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | KF::ARGUMENT | 2;
  case K::_DYNAMIC_CAPTURE:
    return KF::DYNAMIC_CAPTURE;

  // CONTROL FLOW
  case K::RETURN:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::BREAK:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::CONTINUE:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::FALLTHROUGH:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::GOTO:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::RANGE_OVER:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;

  // SYMBOLS
  case K::OBJECT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | 2;
  case K::ENUMERATION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | 2;
  case K::_ENUMERATION_VALUE_WITH_DISCRIMINANT:
    return KF::SYMBOLIC | KF::ENUMERATION_VALUE;

  // VALUES;
  case K::TRUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::FALSE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INDETERMINATE:
    return KF::RVALUE;
  case K::THIS:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::RESULT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::COMMAND_LINE_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IN:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::OUT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::ADDRESS_SIZE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ADDRESS_DEPTH:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITS_PER_BYTE:
    return KF::RVALUE | KF::ARGUMENT;

  // BUILTIN TYPES
  case K::VOID:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NO_RETURN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SELF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BOOLEAN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::WORD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BFLOAT16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY16:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY32:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY64:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY128:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UTF8:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_CHAR:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_UNSIGNED_CHAR:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_SIGNED_CHAR:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_WCHAR_T:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_SHORT_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_UNSIGNED_SHORT_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_UNSIGNED_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_LONG_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_UNSIGNED_LONG_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_LONG_LONG_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_UNSIGNED_LONG_LONG_INT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_FLOAT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_DOUBLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::C_LONG_DOUBLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // VARIADIC ARGUMENTS
  case K::VARIADIC_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FIRST_VARIADIC_ARGUMENT:
    return KF::REFLECTION;
  case K::_FIRST_VARIADIC_ARGUMENT_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KF::REFLECTION;
  case K::_NEXT_VARIADIC_ARGUMENT_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // SCOPES
  case K::IF:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::ELSE_IF:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::ELSE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::MATCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::INLINE_MATCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;
  case K::SWITCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::ARM | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 1;
  case K::CASE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::ARM | 1;
  case K::DEFAULT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::ARM;
  case K::FOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 1;
  case K::FOR_EACH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 2;
  case K::WHILE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 1;
  case K::SCOPE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::INLINE_SCOPE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;
  case K::BLOCK:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::INLINE_BLOCK:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;

  // RANGES
  case K::RANGE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CONTAINS:
    return KF::REFLECTION;
  case K::_CONTAINS_VALUE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_SEQUENCE:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::_SEQUENCE_STEP_ADD:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_SUBTRACT:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_MULTIPLY:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_DIVIDE:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_MODULUS:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_LESS:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_GREATER:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_LESS_EQUAL:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_GREATER_EQUAL:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_EQUAL:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_NOT_EQUAL:
    return KF::SYMBOLIC | KF::SEQUENCE_STAGE;

  // ACCESS MODIFIERS
  case K::PRIVATE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::PROTECTED:
    return KF::STATEMENT_ATTRIBUTE;
  case K::EXPORT:
    return KF::STATEMENT_ATTRIBUTE;

  // SYMBOL GRAPH
  case K::IMPORT:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::TOP_STATEMENT;
  case K::USE:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::USE_TABLE:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::TABLE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::CAN_HAVE_NO_SEMICOLON |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | 1;

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::DEBUG_TRAP:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;

  // HINTS
  case K::UNREACHABLE:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::ASSUME:
    return KF::CAN_HAVE_NO_SEMICOLON | KF::LOCAL_STATEMENT | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;

  // ATTRIBUTES
  case K::BAKE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::STATIC_CAPTURE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::MAY_PARENT:
    return KF::STATEMENT_ATTRIBUTE;
  case K::PARENT:
    return KF::STATEMENT_ATTRIBUTE;
  case K::ABSTRACT:
    return KF::STATEMENT_ATTRIBUTE;
  case K::VIRTUAL:
    return KF::STATEMENT_ATTRIBUTE;
  case K::OVERRIDE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::POSITION:
    return KF::STATEMENT_ATTRIBUTE;
  case K::INLINE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::MANGLED_NAME:
    return KF::REFLECTION | KF::STATEMENT_ATTRIBUTE;
  case K::_MANGLED_NAME_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::PACK:
    return KF::STATEMENT_ATTRIBUTE;
  case K::USER:
    return KF::STATEMENT_ATTRIBUTE;
  case K::LABEL:
    return KF::STATEMENT_ATTRIBUTE;
  case K::TEMPLATE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::LIKELY:
    return KF::STATEMENT_ATTRIBUTE;
  case K::UNLIKELY:
    return KF::STATEMENT_ATTRIBUTE;
  case K::DEPRECIATED:
    return KF::STATEMENT_ATTRIBUTE;
  case K::NO_SHALLOW_COPY:
    return KF::STATEMENT_ATTRIBUTE;

  // REFLECTIONS
  case K::_REFLECT:
    return KF::SYMBOLIC | KF::RVALUE | KF::LVALUE | KF::ARGUMENT |
           KF::PARAMETER | KF::PATH;
  case K::_MEMBER_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PATH;
  case K::SIZE:
    return KF::REFLECTION;
  case K::_SIZE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::DEPTH:
    return KF::REFLECTION;
  case K::_DEPTH_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::COUNT:
    return KF::REFLECTION;
  case K::_COUNT_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::LENGTH:
    return KF::REFLECTION;
  case K::_LENGTH_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::SPAN_SIZE:
    return KF::REFLECTION;
  case K::_SPAN_SIZE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::SPAN_DEPTH:
    return KF::REFLECTION;
  case K::_SPAN_DEPTH_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::NAME:
    return KF::REFLECTION;
  case K::_NAME_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::LINE:
    return KF::REFLECTION;
  case K::_LINE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::COLUMN:
    return KF::REFLECTION;
  case K::_COLUMN_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::IS:
    return KF::REFLECTION;
  case K::_VALUE_IS:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::HOLDS:
    return KF::REFLECTION;
  case K::_VALUE_HOLDS:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION;
  case K::_TYPE_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::SYMBOL:
    return KF::REFLECTION;
  case K::_SYMBOL_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;
  case K::DISCRIMINANT:
    return KF::REFLECTION;
  case K::_DISCRIMINANT_OF:
    return KF::SYMBOLIC | KF::RVALUE | KF::ARGUMENT;

  case K::__LAST:
    break;
  }
  return KF::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsLiteral(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LITERAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasUnquotedLeft(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_LEFT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasUnquotedRight(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::UNQUOTED_RIGHT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsConverging(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONVERGING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsSymbolic(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::SYMBOLIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsInternal(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanHaveNoSemicolon(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CAN_HAVE_NO_SEMICOLON);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasSemicolonSeparatedBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags,
                       rq::KeywordFlags::HAS_SEMICOLON_SEPARATED_BRANCHES);
}

enum class Situation : std::uint_fast32_t {
  NONE,
  TOP_STATEMENT,
  TABLE_STATEMENT,
  OBJECT_STATEMENT,
  LOCAL_STATEMENT,
  LVALUE,
  RVALUE,
  REFLECTION,
  ARGUMENT,
  PARAMETER,
  ENUMERATION_VALUE,
  PATH,
  NAME,
  ASCRIPTION,
  TYPE_ATTRIBUTE,
  STATEMENT_ATTRIBUTE,
  SEQUENCE_STAGE,
  ARM,
  DYNAMIC_CAPTURE
};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
getDescription(rq::Situation situation) {
  using namespace rq;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    return "no expression";
  case S::TOP_STATEMENT:
    return "top level statement";
  case S::TABLE_STATEMENT:
    return "table scoped statement";
  case S::OBJECT_STATEMENT:
    return "object scoped statement";
  case S::LOCAL_STATEMENT:
    return "local scoped statement";
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
  case S::ENUMERATION_VALUE:
    return "enumeration value expression";
  case S::PATH:
    return "path expression";
  case S::NAME:
    return "name expression";
  case S::ASCRIPTION:
    return "path expression";
  case S::TYPE_ATTRIBUTE:
    return "type attribute";
  case S::STATEMENT_ATTRIBUTE:
    return "statement attribute";
  case S::SEQUENCE_STAGE:
    return "short range stage expression";
  case S::ARM:
    return "arm expression";
  case S::DYNAMIC_CAPTURE:
    return "dynamic capture expression";
  }
  return "error expression";
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsValueRelated(rq::Situation situation) {
  return situation == rq::Situation::LVALUE ||
         situation == rq::Situation::RVALUE ||
         situation == rq::Situation::ARGUMENT;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::Keyword
getUniversalized(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::CONTAINS:
    return K::_CONTAINS_VALUE;
  case K::SINGLETON:
    return K::_SINGLETON_OF;
  case K::CONTENT:
    return K::_CONTENT_OF;
  case K::ADDRESS:
    return K::_ADDRESS_OF;
  case K::BORROW:
    return K::_BORROW_OF;
  case K::VIEW:
    return K::_VIEW_OF;
  case K::SLICE:
    return K::_SLICE_OF;
  case K::ARRAY_SLICE:
    return K::_ARRAY_SLICE_OF;
  case K::DEEP_COPY:
    return K::_DEEP_COPY_OF;
  case K::RETAIN_MOVE:
    return K::_RETAIN_MOVE_OF;
  case K::DROP_MOVE:
    return K::_DROP_MOVE_OF;
  case K::LINEAR_ASSIGN:
    return K::_LINEAR_ASSIGN_OF;
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::_FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::_NEXT_VARIADIC_ARGUMENT_OF;
  case K::MANGLED_NAME:
    return K::_MANGLED_NAME_OF;
  case K::SIZE:
    return K::_SIZE_OF;
  case K::DEPTH:
    return K::_DEPTH_OF;
  case K::COUNT:
    return K::_COUNT_OF;
  case K::LENGTH:
    return K::_LENGTH_OF;
  case K::SPAN_SIZE:
    return K::_SPAN_SIZE_OF;
  case K::NAME:
    return K::_NAME_OF;
  case K::LINE:
    return K::_LINE_OF;
  case K::COLUMN:
    return K::_COLUMN_OF;
  case K::IS:
    return K::_VALUE_IS;
  case K::HOLDS:
    return K::_VALUE_HOLDS;
  case K::TYPE:
    return K::_TYPE_OF;
  case K::SYMBOL:
    return K::_SYMBOL_OF;
  case K::DISCRIMINANT:
    return K::_DISCRIMINANT_OF;
  default:
    break;
  }
  return K::__ERROR;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::__NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeTopStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TOP_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeTableStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TABLE_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeObjectStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::OBJECT_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeLocalStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LOCAL_STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeLValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeRValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeReflection(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::REFLECTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeArgument(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARGUMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeParameter(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::PARAMETER);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeEnumerationValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ENUMERATION_VALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBePath(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::PATH);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeName(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeAscription(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeTypeAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TYPE_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeStatementAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeSequenceStage(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::SEQUENCE_STAGE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getCanBeArm(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARM);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeDynamicCapture(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::DYNAMIC_CAPTURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr unsigned
getCommaBranchCount(rq::Keyword keyword) {
  RQ_ASSERT(rq::getHasSemicolonSeparatedBranches(keyword),
            "does not have semicolon separated branches");
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  unsigned comma_branch_count =
      rq::getMaskValue(flags, rq::KeywordFlags::COMMA_BRANCH_COUNT_MASK);
  return comma_branch_count;
}

template <rq::Situation SITUATION_PARAM>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeSituation(rq::Keyword keyword) {
  if constexpr (SITUATION_PARAM == rq::Situation::NONE) {
    return rq::getIsNone(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::TOP_STATEMENT) {
    return rq::getCanBeTopStatement(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::TABLE_STATEMENT) {
    return rq::getCanBeTableStatement(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::OBJECT_STATEMENT) {
    return rq::getCanBeObjectStatement(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::LOCAL_STATEMENT) {
    return rq::getCanBeLocalStatement(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::LVALUE) {
    return rq::getCanBeLValue(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::RVALUE) {
    return rq::getCanBeRValue(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::REFLECTION) {
    return rq::getCanBeReflection(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::ARGUMENT) {
    return rq::getCanBeArgument(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::PARAMETER) {
    return rq::getCanBeParameter(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::ENUMERATION_VALUE) {
    return rq::getCanBeEnumerationValue(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::PATH) {
    return rq::getCanBePath(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::NAME) {
    return rq::getCanBeName(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::ASCRIPTION) {
    return rq::getCanBeAscription(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::TYPE_ATTRIBUTE) {
    return rq::getCanBeTypeAttribute(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::STATEMENT_ATTRIBUTE) {
    return rq::getCanBeStatementAttribute(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::SEQUENCE_STAGE) {
    return rq::getCanBeSequenceStage(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::ARM) {
    return rq::getCanBeArm(keyword);
  } else if constexpr (SITUATION_PARAM == rq::Situation::DYNAMIC_CAPTURE) {
    return rq::getCanBeDynamicCapture(keyword);
  } else {
    static_assert(
        false, "invalid situation or function not implemented for situation");
  }
}

enum class StatementAttribute : std::uint_fast8_t {
  NONE,
  BAKE,
  STATIC_CAPTURE,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  MANGLED_NAME,
  PACK,
  USER,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  EXPORT,
  PRIVATE,
  PROTECTED,
  NO_SHALLOW_COPY
};

[[nodiscard]] inline constexpr llvm::StringRef
getName(rq::StatementAttribute attribute) {
  using namespace rq;
  using SA = StatementAttribute;
  switch (attribute) {
  case SA::NONE:
    return "none";
  case SA::BAKE:
    return "bake";
  case SA::STATIC_CAPTURE:
    return "static_capture";
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
  case SA::MANGLED_NAME:
    return "mangled_name";
  case SA::PACK:
    return "pack";
  case SA::USER:
    return "user";
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
  case SA::PRIVATE:
    return "private";
  case SA::PROTECTED:
    return "protected";
  case SA::NO_SHALLOW_COPY:
    return "no_shallow_copy";
  }
  return "error";
}

[[nodiscard]] inline constexpr rq::StatementAttribute
getStatementAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using SA = StatementAttribute;
  switch (keyword) {
  case K::BAKE:
    return SA::BAKE;
  case K::STATIC_CAPTURE:
    return SA::STATIC_CAPTURE;
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
  case K::MANGLED_NAME:
    return SA::MANGLED_NAME;
  case K::PACK:
    return SA::PACK;
  case K::USER:
    return SA::USER;
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
  case K::PRIVATE:
    return SA::PRIVATE;
  case K::PROTECTED:
    return SA::PROTECTED;
  case K::NO_SHALLOW_COPY:
    return SA::NO_SHALLOW_COPY;
  default:
    break;
  }
  return SA::NONE;
}

enum class StatementFlags : std::uint32_t {
  NONE = 0,
  BAKE = rq::getBit(31),
  STATIC_CAPTURE = rq::getBit(30),
  MAY_PARENT = rq::getBit(29),
  PARENT = rq::getBit(28),
  ABSTRACT = rq::getBit(27),
  VIRTUAL = rq::getBit(26),
  OVERRIDE = rq::getBit(25),
  POSITION = rq::getBit(24),
  MANGLED_NAME = rq::getBit(23),
  PACK = rq::getBit(22),
  USER = rq::getBit(21),
  LABEL = rq::getBit(20),
  TEMPLATE = rq::getBit(19),
  LIKELY = rq::getBit(18),
  UNLIKELY = rq::getBit(17),
  DEPRECIATED = rq::getBit(16),
  EXPORT = rq::getBit(15),
  PRIVATE = rq::getBit(14),
  PROTECTED = rq::getBit(13),
  NO_SHALLOW_COPY = rq::getBit(12)
};

template <> struct is_flags<rq::StatementFlags> final : std::true_type {};

[[nodiscard]] inline constexpr rq::StatementFlags
getFlags(rq::StatementAttribute attribute) {
  using namespace rq;
  using SA = StatementAttribute;
  using SF = StatementFlags;
  switch (attribute) {
  case SA::NONE:
    return SF::NONE;
  case SA::BAKE:
    return SF::BAKE;
  case SA::STATIC_CAPTURE:
    return SF::STATIC_CAPTURE;
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
  case SA::MANGLED_NAME:
    return SF::POSITION;
  case SA::PACK:
    return SF::PACK;
  case SA::USER:
    return SF::USER;
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
  case SA::PRIVATE:
    return SF::PRIVATE;
  case SA::PROTECTED:
    return SF::PROTECTED;
  case SA::NO_SHALLOW_COPY:
    return SF::NO_SHALLOW_COPY;
  }
  return SF::NONE;
}

enum class TypeAttribute : std::uint_fast8_t {
  NONE,
  MUTABLE,
  CONSTANT,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  MAY_DISCARD,
  DEBUG_TRAP_ON_PANIC,
  LINEAR,
  DYNAMIC_CAPTURE_LAYOUT
};

[[nodiscard]] inline constexpr llvm::StringRef
getName(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  switch (attribute) {
  case TA::NONE:
    return "none";
  case TA::MUTABLE:
    return "mutable";
  case TA::CONSTANT:
    return "constant";
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
  case TA::LINEAR:
    return "linear";
  case TA::DYNAMIC_CAPTURE_LAYOUT:
    return "dynamic_capture_layout";
  }
  return "none";
}

[[nodiscard]] inline constexpr rq::TypeAttribute
getTypeAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using TA = TypeAttribute;
  switch (keyword) {
  case K::MUTABLE:
    return TA::MUTABLE;
  case K::CONSTANT:
    return TA::CONSTANT;
  case K::VOLATILE:
    return TA::VOLATILE;
  case K::ATOMIC:
    return TA::ATOMIC;
  case K::NULL_TERMINATED:
    return TA::NULL_TERMINATED;
  case K::MAY_DISCARD:
    return TA::MAY_DISCARD;
  case K::DEBUG_TRAP_ON_PANIC:
    return TA::DEBUG_TRAP_ON_PANIC;
  case K::LINEAR:
    return TA::LINEAR;
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return TA::DYNAMIC_CAPTURE_LAYOUT;
  default:
    break;
  }
  return TA::NONE;
}

enum class TypeFlags : std::uint16_t {
  NONE = 0,
  MUTABLE = rq::getBit(15),
  CONSTANT = rq::getBit(14),
  VOLATILE = rq::getBit(13),
  ATOMIC = rq::getBit(12),
  NULL_TERMINATED = rq::getBit(11),
  MAY_DISCARD = rq::getBit(10),
  DEBUG_TRAP_ON_PANIC = rq::getBit(9),
  LINEAR = rq::getBit(8),
  DYNAMIC_CAPTURE_LAYOUT = rq::getBit(7)
};

template <> struct is_flags<rq::TypeFlags> final : std::true_type {};

[[nodiscard]] inline constexpr rq::TypeFlags
getFlags(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  using TF = TypeFlags;
  switch (attribute) {
  case TA::NONE:
    return TF::NONE;
  case TA::MUTABLE:
    return TF::MUTABLE;
  case TA::CONSTANT:
    return TF::CONSTANT;
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
  case TA::LINEAR:
    return TF::LINEAR;
  case TA::DYNAMIC_CAPTURE_LAYOUT:
    return TF::DYNAMIC_CAPTURE_LAYOUT;
  default:
    break;
  }
  return TF::NONE;
}

template <typename SourceAParam, typename SourceBParam>
[[nodiscard]] inline constexpr unsigned
getSourceLengthBetween(const SourceAParam &first, const SourceBParam &last) {
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

struct Expression final {
  using Self = rq::Expression;

  rq::Keyword _keyword = rq::Keyword::__NONE;
  rq::Expression *_next_ptr = nullptr;
  rq::Expression *_branch_ptr = nullptr;
  const char *_source_text_ptr = nullptr;
  unsigned _source_text_length = 0;

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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getUniversalized() const {
    return rq::getUniversalized(this->_keyword);
  }
  RQ_ALWAYS_INLINE void clear() { std::memset(this, 0, sizeof(*this)); }
  RQ_ALWAYS_INLINE void setKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword == rq::Keyword::__NONE,
              "keyword must not already be set");
    this->_keyword = keyword;
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword != rq::Keyword::__NONE,
              "keyword must already be set");
    this->_keyword = keyword;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteral() const {
    return rq::getIsLiteral(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsSymbolic() const {
    return rq::getIsSymbolic(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal() const {
    return rq::getIsInternal(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanHaveNoSemicolon() const {
    return rq::getCanHaveNoSemicolon(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSemicolonSeparatedBranches() const {
    return rq::getHasSemicolonSeparatedBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTopStatement() const {
    return rq::getCanBeTopStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTableStatement() const {
    return rq::getCanBeTableStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeObjectStatement() const {
    return rq::getCanBeObjectStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLocalStatement() const {
    return rq::getCanBeLocalStatement(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeEnumerationValue() const {
    return rq::getCanBeEnumerationValue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBePath() const {
    return rq::getCanBePath(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName() const {
    return rq::getCanBeName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription() const {
    return rq::getCanBeAscription(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTypeAttribute() const {
    return rq::getCanBeTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatementAttribute() const {
    return rq::getCanBeStatementAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSequenceStage() const {
    return rq::getCanBeSequenceStage(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeArm() const {
    return rq::getCanBeArm(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeDynamicCapture() const {
    return rq::getCanBeDynamicCapture(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getCommaBranchCount() const {
    return rq::getCommaBranchCount(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_source_text_ptr != nullptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInserted() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return this->_source_text_length == 0;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef getSourceText() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
    return llvm::StringRef(this->_source_text_ptr, this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getSourceTextPtr() const {
    return this->_source_text_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE unsigned getSourceTextLength() const {
    return this->_source_text_length;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceStart() const {
    return llvm::SMLoc::getFromPointer(this->_source_text_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceEnd() const {
    return llvm::SMLoc::getFromPointer(this->_source_text_ptr +
                                       this->_source_text_length);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMRange getLlvmSourceRange() const {
    return llvm::SMRange(this->getLlvmSourceStart(), this->getLlvmSourceEnd());
  }
  RQ_ALWAYS_INLINE void setSourceText(llvm::StringRef source) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    RQ_ASSERT(!source.empty(), "source text is empty");
    this->_source_text_ptr = source.data();
    this->_source_text_length = source.size();
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSource(const SourceParam &source) {
    this->setSourceText(source.getSourceText());
  }
  template <typename FirstSourceParam, typename LastSourceParam>
  inline void setSource(const FirstSourceParam &first,
                        const LastSourceParam &last) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    rq::assignSingleValue(this->_source_text_ptr, first.getSourceTextPtr());
    this->_source_text_length = rq::getSourceLengthBetween(first, last);
  }
  template <typename SourceParam>
  inline void extendSourceOver(const SourceParam &source) {
    RQ_ASSERT(this->getHasSourceText(), "expression source not set");
    if (source.getSourceTextPtr() >= this->_source_text_ptr) {
      this->_source_text_length = rq::getSourceLengthBetween(*this, source);
    } else {
      this->_source_text_ptr = source.getSourceTextPtr();
      this->_source_text_length = rq::getSourceLengthBetween(source, *this);
    }
  }
  RQ_ALWAYS_INLINE const char *getBeforeSourceTextPtr() const {
    return this->getSourceTextPtr();
  }
  RQ_ALWAYS_INLINE const char *getAfterSourceTextPtr() const {
    return this->getSourceTextPtr() + this->getSourceTextLength();
  }
  RQ_ALWAYS_INLINE void setSourceInsertedAt(const char *source_ptr) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    rq::assignSingleValue(this->_source_text_ptr, source_ptr);
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceInsertedBefore(const SourceParam &source) {
    this->setSourceInsertedAt(source.getBeforeSourceTextPtr());
  }
  template <typename SourceParam>
  inline void setSourceInsertedAfter(const SourceParam &source) {
    this->setSourceInsertedAt(source.getAfterSourceTextPtr());
  }
  // NOTE: no getBranchCount and getNextCount because bad performance!
  RQ_ALWAYS_INLINE rq::Expression &getBranch() {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE const rq::Expression &getBranch() const {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE rq::Expression &getNext() {
    return rq::dereferencePtr(this->_next_ptr);
  }
  RQ_ALWAYS_INLINE const rq::Expression &getNext() const {
    return rq::dereferencePtr(this->_next_ptr);
  }
  RQ_ALWAYS_INLINE rq::Expression *getBranchPtr() { return this->_branch_ptr; }
  RQ_ALWAYS_INLINE const rq::Expression *getBranchPtr() const {
    return this->_branch_ptr;
  }
  RQ_ALWAYS_INLINE rq::Expression *getNextPtr() { return this->_next_ptr; }
  RQ_ALWAYS_INLINE const rq::Expression *getNextPtr() const {
    return this->_next_ptr;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastNext() {
    rq::Expression *expression_ptr = this;
    while (expression_ptr->_next_ptr != nullptr) {
      expression_ptr = expression_ptr->_next_ptr;
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastNext() const {
    const rq::Expression *expression_ptr = this;
    while (expression_ptr->_next_ptr != nullptr) {
      expression_ptr = expression_ptr->_next_ptr;
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression &getLastBranch() {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    rq::Expression *expression_ptr = this->_branch_ptr;
    while (expression_ptr->_next_ptr != nullptr) {
      expression_ptr = expression_ptr->_next_ptr;
    }
    return rq::dereferencePtr(expression_ptr);
  }
  [[nodiscard]] RQ_ALWAYS_INLINE const rq::Expression &getLastBranch() const {
    RQ_ASSERT(this->getHasBranch(), "does not have branch");
    rq::Expression *expression_ptr = this->_branch_ptr;
    while (expression_ptr->_next_ptr != nullptr) {
      expression_ptr = expression_ptr->_next_ptr;
    }
    return rq::dereferencePtr(expression_ptr);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression &branch) {
    rq::assignSingleValue(this->_branch_ptr, &branch);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression &next) {
    rq::assignSingleValue(this->_next_ptr, &next);
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression *branch_ptr) {
    rq::assignSingleValue(this->_branch_ptr, branch_ptr);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression *next_ptr) {
    rq::assignSingleValue(this->_next_ptr, next_ptr);
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
    this->_next_ptr = &next;
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
    this->_next_ptr = next_ptr;
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
    rq::Expression *replaced_next = this->_next_ptr;
    this->_next_ptr = &next;
    return replaced_next;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceBranchPtr(rq::Expression *branch_ptr) {
    rq::Expression *replaced_branch = this->_branch_ptr;
    this->_branch_ptr = branch_ptr;
    return replaced_branch;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Expression *
  replaceNextPtr(rq::Expression *next_ptr) {
    rq::Expression *replaced_next = this->_next_ptr;
    this->_next_ptr = next_ptr;
    return replaced_next;
  }
  RQ_ALWAYS_INLINE bool getHasBranch() const {
    return this->_branch_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE bool getHasNext() const {
    return this->_next_ptr != nullptr;
  }

  inline rq::Expression &popBranch() {
    RQ_ASSERT(this->_branch_ptr != nullptr, "does not have branch");
    rq::Expression *old_branch_ptr = this->_branch_ptr;
    this->_branch_ptr = nullptr;
    return *old_branch_ptr;
  }
  inline rq::Expression *popBranchPtr() {
    rq::Expression *old_branch_ptr = this->_branch_ptr;
    this->_branch_ptr = nullptr;
    return old_branch_ptr;
  }
  inline rq::Expression &popNext() {
    RQ_ASSERT(this->_next_ptr != nullptr, "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr;
    this->_next_ptr = nullptr;
    return *old_next_ptr;
  }
  inline rq::Expression *popNextPtr() {
    rq::Expression *old_next_ptr = this->_next_ptr;
    this->_next_ptr = nullptr;
    return old_next_ptr;
  }
  inline rq::Expression &mergeAndPopBranch() {
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
      getHorizontalSubrange() {
    return std::ranges::subrange(rq::ExpressionIterator(this),
                                 rq::ExpressionIterator());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE
      std::ranges::subrange<rq::ConstExpressionIterator,
                            rq::ConstExpressionIterator,
                            std::ranges::subrange_kind::unsized>
      getHorizontalSubrange() const {
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

rq::ExpressionIterator ExpressionIterator::operator++(int) {
  return rq::ExpressionIterator(++this->_expression_ptr);
}

rq::ConstExpressionIterator &ConstExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
}

rq::ConstExpressionIterator ConstExpressionIterator::operator++(int) {
  return rq::ConstExpressionIterator(++this->_expression_ptr);
}

} // namespace rq
