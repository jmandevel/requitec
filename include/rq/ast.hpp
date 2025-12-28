#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstdint>

namespace rq {

// NOTE:
//  two underscores at the front of name and I_ in C++ means that the keyword is
//  for internal use only

// NOTE:
//  one underscore at the front of name and S_ means that the keyword is not
//  meant to be normally used directly. Might be used by a standard library, or
//  might be useful only in symbolic requite. There should be other ways to
//  write these things most of the time that are more concise.

enum class Keyword : std::uint32_t {
  // this should never occur.
  I_NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  I_INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  I_FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  I_STRING_LITERAL,
  // left string interpolation (string literal with no end quote mark).
  I_LEFT_INTERPOLATION_LITERAL,
  // middle string interpolation (string literal with no quote marks).
  I_MIDDLE_INTERPOLATION_LITERAL,
  // right string interpolation (string literal with start quote mark).
  I_RIGHT_INTERPOLATION_LITERAL,
  // a literal that represents a single text character.
  I_CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  I_IDENTIFIER_LITERAL,

  // ERRORS
  I_ERROR,

  // SITUATIONAL
  S_PARENTHESIS_GROUP,
  S_EQUAL_OPERATOR,
  S_COLON_OPERATOR,
  S_INFERENCE,
  S_UNSITUATED_ASCRIBE_STATEMENT,
  S_UNSITUATED_ASCRIBE_TYPE,

  // LOGICAL
  S_LOGICAL_AND,
  S_LOGICAL_OR,
  S_LOGICAL_COMPLEMENT,

  // COMPARISON
  S_GREATER,
  S_GREATER_EQUAL,
  S_LESS,
  S_LESS_EQUAL,
  S_EQUAL,
  S_NOT_EQUAL,

  // APPLY
  S_EXTEND,
  S_EXTENSION,
  S_BINDING,
  S_ASCRIBE_TYPE,
  S_ASCRIBE_STATEMENT,
  S_CAST,
  S_IDENTIFY,

  // ARITHMETIC
  S_ADD,
  S_SUBTRACT,
  S_MULTIPLY,
  S_DIVIDE,
  S_MODULUS,
  S_NEGATE,

  // BITWISE
  S_BITWISE_CAST,
  S_BITWISE_OR,
  S_BITWISE_AND,
  S_BITWISE_XOR,
  S_BITWISE_COMPLEMENT,
  S_BITWISE_SHIFT_LEFT,
  S_BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  // MEMORY
  SINGLETON,
  S_SINGLETON_OF,
  CONTENT,
  S_CONTENT_OF,
  ADDRESS,
  S_ADDRESS_OF,
  BORROW,
  S_BORROW_OF,

  // ASSIGNMENT
  S_ASSIGN,
  S_ASSIGN_ADD,
  S_ASSIGN_SUBTRACT,
  S_ASSIGN_MULTIPLY,
  S_ASSIGN_DIVIDE,
  S_ASSIGN_MODULUS,

  // SUBTYPE
  S_ARRAY,
  S_REFERENCE,
  S_POINTER,
  S_FAT_POINTER,

  // TYPE MODIFIER
  MUTABLE,
  CONSTANT,
  PARTIALLY_MUTABLE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,
  MAY_DISCARD,
  DEBUG_TRAP_ON_PANIC,
  DYNAMIC_CAPTURE_LAYOUT,

  // PARAMETER RULES
  S_POSITIONAL_PARAMETERS_END,
  S_NAMED_PARAMETERS_BEGIN,

  // BRACES
  S_TUPLE,
  S_LAYOUT_TYPE,
  S_NULL,
  S_NULL_TYPE,
  S_STRUCTURED_BINDING,
  S_IGNORE,
  S_SPECIALIZATION,

  // PROCEDURES
  S_CALL,
  CONSTRUCT_FUNCTOR,
  S_CONSTRUCT_FUNCTOR_OF,
  S_NAMED_ARGUMENT,
  S_INDEX_INTO,
  S_SIGNATURE_TYPE,
  S_DEFAULT_VALUE_PARAMETER,
  DESTROY,
  S_DESTROY_VALUE,
  DROP,
  S_DROP_VALUE,
  MOVE,
  S_MOVE_VALUE,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  CONSTRUCTOR,
  LAYOUT_CONSTRUCTOR,
  DESTRUCTOR,
  RANGER,
  S_ANONYMOUS_FUNCTION,
  S_DYNAMIC_CAPTURE,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  RANGE_OVER,

  // DECLARED TYPES
  OBJECT,
  ENUMERATION,

  // VALUES
  TRUE,
  FALSE,
  INDETERMINATE,
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
  // type of object extending or member of.
  THIS_TYPE,
  // value returned from a function.
  RESULT,
  // retrieve command line arguments within entry_point.
  COMMAND_LINE_ARGUMENTS,
  // resulting exit code within entry_point.
  EXIT_CODE,
  // the byte size of memory addresses on the current architecture.
  ADDRESS_BYTE_SIZE,
  // the bit size of memory addresses on the current architecture.
  ADDRESS_BIT_DEPTH,
  // the amount of bits per byte on the current architecture. (almost always 8)
  BITS_PER_BYTE,

  // BUILTIN TYPES
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
  S_FIRST_VARIADIC_ARGUMENT_OF,
  NEXT_VARIADIC_ARGUMENT,
  S_NEXT_VARIADIC_ARGUMENT_OF,

  // SCOPES
  IF,
  ELSE_IF,
  ELSE,
  MATCH,
  INLINE_MATCH,
  SWITCH,
  INLINE_SWITCH,
  CASE,
  DEFAULT,
  FOR,
  WHILE,
  SCOPE,
  INLINE_SCOPE,
  BLOCK,
  INLINE_BLOCK,

  // RANGES
  RANGE,
  S_SEQUENCE,
  S_SEQUENCE_STEP_ADD,
  S_SEQUENCE_STEP_SUBTRACT,
  S_SEQUENCE_STEP_MULTIPLY,
  S_SEQUENCE_STEP_DIVIDE,
  S_SEQUENCE_STEP_MODULUS,
  S_SEQUENCE_WHILE_LESS,
  S_SEQUENCE_WHILE_GREATER,
  S_SEQUENCE_WHILE_LESS_EQUAL,
  S_SEQUENCE_WHILE_GREATER_EQUAL,
  S_SEQUENCE_WHILE_EQUAL,
  S_SEQUENCE_WHILE_NOT_EQUAL,

  // ACCESS MODIFIERS
  PUBLIC,
  PROTECTED,
  EXPORT,

  // TABLE GRAPH
  IMPORT,
  USE,
  USE_TABLE,
  FACADE,
  TABLE,
  S_MODULE_TRUNK,

  // ERROR HANDLING AND DEBUGGING
  PANIC_TRAP,
  DEBUG_TRAP,

  // HINTS
  UNREACHABLE,
  ASSUME,

  // STATEMENT ATTRIBUTES
  OPAQUE,
  GLOBAL,
  STATIC,
  STATIC_CAPTURE,
  EVALUATE_IMMEDIATELY,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  INLINE,
  MANGLED_NAME,
  S_MANGLED_NAME_OF,
  PACK,
  USER_ATTRIBUTE,
  S_USER_ATTRIBUTE_OF,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  MAY_COPY,
  MAY_MOVE,
  MUTABILITY_CLASS,

  // EXPRESSIONS
  QUOTE,
  EXPRESSION,
  EXPAND,
  S_EXPAND_STATEMENT,
  S_EXPAND_LVALUE,
  S_EXPAND_RVALUE,
  S_EXPAND_REFLECTION,
  S_EXPAND_ARGUMENT,
  S_EXPAND_PARAMETER,
  S_EXPAND_SYMBOL_PATH,
  S_EXPAND_SEQUENCE_STAGE,
  S_EXPAND_DYNAMIC_CAPTURE,
  S_EXPAND_VIGNETTE,
  S_EXPAND_VIGNETTE_RVALUE,

  // REFLECTIONS
  S_REFLECT,
  BYTE_SIZE,
  S_BYTE_SIZE_OF,
  BIT_DEPTH,
  S_BIT_DEPTH_OF,
  ELEMENT_COUNT,
  S_ELEMENT_COUNT_OF,
  NAME,
  S_NAME_OF,
  LINE,
  S_LINE_OF,
  COLUMN,
  S_COLUMN_OF,
  IS,
  S_IS_TYPE,
  HOLDS,
  S_HOLDS_ENUMERATOR,
  TYPE,
  S_TYPE_OF,
  SYMBOL,
  S_SYMBOL_OF,
  SIGNATURE,
  S_SIGNATURE_OF,
  LAYOUT,
  S_LAYOUT_OF,

  I_LAST
};

constexpr std::size_t KEYWORD_COUNT =
    static_cast<std::size_t>(rq::Keyword::I_LAST);

[[nodiscard]] inline constexpr llvm::StringRef getName(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  case K::I_NONE:
    return "__none";

  // LITERALS
  case K::I_INTEGER_LITERAL:
    return "__integer_literal";
  case K::I_FLOAT_LITERAL:
    return "__float_literal";
  case K::I_STRING_LITERAL:
    return "__string_literal";
  case K::I_LEFT_INTERPOLATION_LITERAL:
    return "__left_interpolation_literal";
  case K::I_MIDDLE_INTERPOLATION_LITERAL:
    return "__middle_interpolation_literal";
  case K::I_RIGHT_INTERPOLATION_LITERAL:
    return "__right_interpolation_literal";
  case K::I_CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case K::I_IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case K::I_ERROR:
    return "__error";

  // SITUATIONAL
  case K::S_PARENTHESIS_GROUP:
    return "_parenthesis_group";
  case K::S_EQUAL_OPERATOR:
    return "_equal_operator";
  case K::S_COLON_OPERATOR:
    return "_colon_operator";
  case K::S_INFERENCE:
    return "_inference";
  case K::S_UNSITUATED_ASCRIBE_STATEMENT:
    return "_unsituated_ascribe_statement";
  case K::S_UNSITUATED_ASCRIBE_TYPE:
    return "_unsituated_ascribe_type";

  // LOGICAL
  case K::S_LOGICAL_AND:
    return "_logical_and";
  case K::S_LOGICAL_OR:
    return "_logical_or";
  case K::S_LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case K::S_GREATER:
    return "_greater";
  case K::S_GREATER_EQUAL:
    return "_greater_equal";
  case K::S_LESS:
    return "_less";
  case K::S_LESS_EQUAL:
    return "_less_equal";
  case K::S_EQUAL:
    return "_equal";
  case K::S_NOT_EQUAL:
    return "_not_equal";

  // APPLY
  case K::S_EXTEND:
    return "_extend";
  case K::S_EXTENSION:
    return "_extension";
  case K::S_BINDING:
    return "_binding";
  case K::S_ASCRIBE_TYPE:
    return "_ascribe_type";
  case K::S_ASCRIBE_STATEMENT:
    return "_ascribe_statement";
  case K::S_CAST:
    return "_cast";
  case K::S_IDENTIFY:
    return "_identify";

  // ARITHMETIC
  case K::S_ADD:
    return "_add";
  case K::S_SUBTRACT:
    return "_subtract";
  case K::S_MULTIPLY:
    return "_multiply";
  case K::S_DIVIDE:
    return "_divide";
  case K::S_MODULUS:
    return "_modulus";
  case K::S_NEGATE:
    return "_negate";

  // BITWISE
  case K::S_BITWISE_CAST:
    return "_bitwise_cast";
  case K::S_BITWISE_OR:
    return "_bitwise_or";
  case K::S_BITWISE_AND:
    return "_bitwise_and";
  case K::S_BITWISE_XOR:
    return "_bitwise_xor";
  case K::S_BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case K::S_BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case K::S_BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case K::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case K::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case K::SINGLETON:
    return "singleton";
  case K::S_SINGLETON_OF:
    return "_singleton_of";
  case K::CONTENT:
    return "content";
  case K::S_CONTENT_OF:
    return "_content_of";
  case K::ADDRESS:
    return "address";
  case K::S_ADDRESS_OF:
    return "_address_of";
  case K::BORROW:
    return "borrow";
  case K::S_BORROW_OF:
    return "_borrow_of";

  // ASSIGNMENT
  case K::S_ASSIGN:
    return "_assign";
  case K::S_ASSIGN_ADD:
    return "_assign_add";
  case K::S_ASSIGN_SUBTRACT:
    return "_assign_subtract";
  case K::S_ASSIGN_MULTIPLY:
    return "_assign_multiply";
  case K::S_ASSIGN_DIVIDE:
    return "_assign_divide";
  case K::S_ASSIGN_MODULUS:
    return "_assign_modulus";

  // SUBTYPE
  case K::S_ARRAY:
    return "_array";
  case K::S_REFERENCE:
    return "_reference";
  case K::S_POINTER:
    return "_pointer";
  case K::S_FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case K::MUTABLE:
    return "mutable";
  case K::CONSTANT:
    return "constant";
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
  case K::DEBUG_TRAP_ON_PANIC:
    return "debug_trap_on_panic";
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return "dynamic_capture_layout";

  // PARAMETER RULES
  case K::S_POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case K::S_NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";

  // BRACES
  case K::S_TUPLE:
    return "_tuple";
  case K::S_LAYOUT_TYPE:
    return "_layout_type";
  case K::S_NULL:
    return "_null";
  case K::S_NULL_TYPE:
    return "_null_type";
  case K::S_STRUCTURED_BINDING:
    return "_structured_binding";
  case K::S_IGNORE:
    return "_ignore";
  case K::S_SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case K::S_CALL:
    return "_call";
  case K::CONSTRUCT_FUNCTOR:
    return "construct_functor";
  case K::S_CONSTRUCT_FUNCTOR_OF:
    return "_construct_functor_of";
  case K::S_NAMED_ARGUMENT:
    return "_named_argument";
  case K::S_INDEX_INTO:
    return "_index_into";
  case K::S_SIGNATURE_TYPE:
    return "_signature_type";
  case K::S_DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case K::DESTROY:
    return "destroy";
  case K::S_DESTROY_VALUE:
    return "_destroy_value";
  case K::DROP:
    return "drop";
  case K::S_DROP_VALUE:
    return "_drop_value";
  case K::MOVE:
    return "move";
  case K::S_MOVE_VALUE:
    return "_move_value";
  case K::ENTRY_POINT:
    return "entry_point";
  case K::FUNCTION:
    return "function";
  case K::METHOD:
    return "method";
  case K::EXTENSION_FUNCTION:
    return "extension_function";
  case K::EXTENSION_METHOD:
    return "extension_method";
  case K::CONSTRUCTOR:
    return "constructor";
  case K::LAYOUT_CONSTRUCTOR:
    return "layout_constructor";
  case K::DESTRUCTOR:
    return "destructor";
  case K::RANGER:
    return "ranger";
  case K::S_ANONYMOUS_FUNCTION:
    return "_anonymous_function";
  case K::S_DYNAMIC_CAPTURE:
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

  // DECLARED TYPES
  case K::OBJECT:
    return "object";
  case K::ENUMERATION:
    return "enumeration";

  // VALUES
  case K::TRUE:
    return "true";
  case K::FALSE:
    return "false";
  case K::INDETERMINATE:
    return "indeterminate";
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
  case K::THIS_TYPE:
    return "this_type";
  case K::RESULT:
    return "result";
  case K::COMMAND_LINE_ARGUMENTS:
    return "command_line_arguments";
  case K::EXIT_CODE:
    return "exit_code";
  case K::ADDRESS_BYTE_SIZE:
    return "address_byte_size";
  case K::ADDRESS_BIT_DEPTH:
    return "address_bit_depth";
  case K::BITS_PER_BYTE:
    return "bits_per_byte";

  // BUILTIN TYPES
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
  case K::S_FIRST_VARIADIC_ARGUMENT_OF:
    return "_first_variadic_argument_of";
  case K::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case K::S_NEXT_VARIADIC_ARGUMENT_OF:
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
  case K::S_SEQUENCE:
    return "_sequence";
  case K::S_SEQUENCE_STEP_ADD:
    return "_sequence_step_add";
  case K::S_SEQUENCE_STEP_SUBTRACT:
    return "_sequence_step_subtract";
  case K::S_SEQUENCE_STEP_MULTIPLY:
    return "_sequence_step_multiply";
  case K::S_SEQUENCE_STEP_DIVIDE:
    return "_sequence_step_divide";
  case K::S_SEQUENCE_STEP_MODULUS:
    return "_sequence_step_modulus";
  case K::S_SEQUENCE_WHILE_LESS:
    return "_sequence_while_less";
  case K::S_SEQUENCE_WHILE_GREATER:
    return "_sequence_while_greater";
  case K::S_SEQUENCE_WHILE_LESS_EQUAL:
    return "_sequence_while_less_equal";
  case K::S_SEQUENCE_WHILE_GREATER_EQUAL:
    return "_sequence_while_greater_equal";
  case K::S_SEQUENCE_WHILE_EQUAL:
    return "_sequence_while_equal";
  case K::S_SEQUENCE_WHILE_NOT_EQUAL:
    return "_sequence_while_not_equal";

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
  case K::USE_TABLE:
    return "use_table";
  case K::FACADE:
    return "facade";
  case K::TABLE:
    return "table";
  case K::S_MODULE_TRUNK:
    return "_module_trunk";

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

  // STATEMENT ATTRIBUTES
  case K::OPAQUE:
    return "opaque";
  case K::GLOBAL:
    return "global";
  case K::STATIC:
    return "static";
  case K::STATIC_CAPTURE:
    return "static_capture";
  case K::EVALUATE_IMMEDIATELY:
    return "evaluate_immediately";
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
  case K::S_MANGLED_NAME_OF:
    return "_mangled_name_of";
  case K::PACK:
    return "pack";
  case K::USER_ATTRIBUTE:
    return "user_attribute";
  case K::S_USER_ATTRIBUTE_OF:
    return "_user_attribute_of";
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
  case K::MUTABILITY_CLASS:
    return "mutability_class";

  // EXPRESSIONS
  case K::QUOTE:
    return "quote";
  case K::EXPRESSION:
    return "expression";
  case K::EXPAND:
    return "expand";
  case K::S_EXPAND_STATEMENT:
    return "_expand_statement";
  case K::S_EXPAND_LVALUE:
    return "_expand_lvalue";
  case K::S_EXPAND_RVALUE:
    return "_expand_rvalue";
  case K::S_EXPAND_REFLECTION:
    return "_expand_reflection";
  case K::S_EXPAND_ARGUMENT:
    return "_expand_argument";
  case K::S_EXPAND_PARAMETER:
    return "_expand_parameter";
  case K::S_EXPAND_SYMBOL_PATH:
    return "_expand_symbol_path";
  case K::S_EXPAND_SEQUENCE_STAGE:
    return "_expand_sequence_stage";
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    return "_expand_dynamic_capture";
  case K::S_EXPAND_VIGNETTE:
    return "_expand_vignette";
  case K::S_EXPAND_VIGNETTE_RVALUE:
    return "_expand_vignette_rvalue";

  // REFLECTIONS
  case K::S_REFLECT:
    return "_reflect";
  case K::BYTE_SIZE:
    return "byte_size";
  case K::S_BYTE_SIZE_OF:
    return "_byte_size_of";
  case K::BIT_DEPTH:
    return "bit_depth";
  case K::S_BIT_DEPTH_OF:
    return "_bit_depth_of";
  case K::ELEMENT_COUNT:
    return "element_count";
  case K::S_ELEMENT_COUNT_OF:
    return "_element_count_of";
  case K::NAME:
    return "name";
  case K::S_NAME_OF:
    return "_name_of";
  case K::LINE:
    return "line";
  case K::S_LINE_OF:
    return "_line_of";
  case K::COLUMN:
    return "column";
  case K::S_COLUMN_OF:
    return "_column_of";
  case K::IS:
    return "is";
  case K::S_IS_TYPE:
    return "_is_type";
  case K::HOLDS:
    return "holds";
  case K::S_HOLDS_ENUMERATOR:
    return "_holds_enumerator";
  case K::TYPE:
    return "type";
  case K::S_TYPE_OF:
    return "_type_of";
  case K::SYMBOL:
    return "symbol";
  case K::S_SYMBOL_OF:
    return "_symbol_of";
  case K::SIGNATURE:
    return "signature";
  case K::S_SIGNATURE_OF:
    return "_signature_of";
  case K::LAYOUT:
    return "layout";
  case K::S_LAYOUT_OF:
    return "_layout_of";

  case K::I_LAST:
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
  INTERNAL = rq::getBit(27),
  STATEMENT_BRANCHES = rq::getBit(26),
  STARTING_CHAINLINK = rq::getBit(25),
  CONTINUING_CHAINLINK = rq::getBit(24),
  FINISHING_CHAINLINK = rq::getBit(23),
  IF_CHAINLINK = rq::getBit(22),
  ARM_CHAINLINK = rq::getBit(21),
  // TRUNK
  STATEMENT = rq::getBit(20),
  RVALUE = rq::getBit(19),
  LVALUE = rq::getBit(18),
  REFLECTION = rq::getBit(17),
  ARGUMENT = rq::getBit(16),
  PARAMETER = rq::getBit(15),
  BINDING = rq::getBit(14),
  SYMBOL_PATH = rq::getBit(13),
  ASCRIPTION = rq::getBit(12),
  TYPE_ATTRIBUTE = rq::getBit(11),
  STATEMENT_ATTRIBUTE = rq::getBit(10),
  SEQUENCE_STAGE = rq::getBit(9),
  DYNAMIC_CAPTURE = rq::getBit(8),
  VIGNETTE = rq::getBit(7),
  VIGNETTE_RVALUE = rq::getBit(6),
  ALL = STATEMENT | RVALUE | LVALUE | REFLECTION | ARGUMENT | PARAMETER |
        BINDING | SYMBOL_PATH | ASCRIPTION | TYPE_ATTRIBUTE |
        STATEMENT_ATTRIBUTE | SEQUENCE_STAGE | DYNAMIC_CAPTURE | VIGNETTE |
        VIGNETTE_RVALUE
};

template <> struct is_flags<rq::KeywordFlags> : std::true_type {};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::KeywordFlags
getFlags(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using KF = KeywordFlags;
  switch (keyword) {
  case K::I_NONE:
    return KF::NONE;

  // LITERALS
  case K::I_INTEGER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::I_FLOAT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::I_STRING_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::I_LEFT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::I_MIDDLE_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::UNQUOTED_RIGHT | KF::LITERAL | KF::INTERNAL |
           KF::RVALUE | KF::ARGUMENT;
  case K::I_RIGHT_INTERPOLATION_LITERAL:
    return KF::UNQUOTED_LEFT | KF::LITERAL | KF::INTERNAL | KF::RVALUE |
           KF::ARGUMENT;
  case K::I_CODEUNIT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::I_IDENTIFIER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER | KF::SYMBOL_PATH;

  // ERRORS
  case K::I_ERROR:
    return KF::INTERNAL;

  // SITUATIONAL
  case K::S_PARENTHESIS_GROUP:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT | KF::LVALUE |
           KF::SYMBOL_PATH | KF::SEQUENCE_STAGE;
  case K::S_EQUAL_OPERATOR:
    return KF::STATEMENT | KF::ARGUMENT | KF::PARAMETER | KF::BINDING;
  case K::S_COLON_OPERATOR:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_INFERENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_UNSITUATED_ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::S_UNSITUATED_ASCRIBE_STATEMENT:
    return KF::STATEMENT | KF::PARAMETER | KF::VIGNETTE_RVALUE | KF::ASCRIPTION;

  // LOGICAL
  case K::S_LOGICAL_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_LOGICAL_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_LOGICAL_COMPLEMENT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // COMPARISON
  case K::S_GREATER:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_GREATER_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_LESS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_LESS_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_NOT_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // APPLY
  case K::S_EXTEND:
    return KF::RVALUE;
  case K::S_EXTENSION:
    return KF::RVALUE;
  case K::S_BINDING:
    return KF::STATEMENT | KF::LVALUE | KF::PARAMETER | KF::ARGUMENT |
           KF::BINDING;
  case K::S_ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::S_ASCRIBE_STATEMENT:
    return KF::STATEMENT | KF::PARAMETER | KF::VIGNETTE_RVALUE | KF::ASCRIPTION;
  case K::S_CAST:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_IDENTIFY:
    return KF::SYMBOL_PATH | KF::RVALUE | KF::ARGUMENT;

  // ARITHMETIC
  case K::S_ADD:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_SUBTRACT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_MULTIPLY:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_DIVIDE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_MODULUS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_NEGATE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // BITWISE
  case K::S_BITWISE_CAST:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_XOR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_COMPLEMENT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_SHIFT_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_BITWISE_SHIFT_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;

  // MEMORY
  case K::SINGLETON:
    return KF::REFLECTION;
  case K::S_SINGLETON_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CONTENT:
    return KF::REFLECTION;
  case K::S_CONTENT_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::ADDRESS:
    return KF::REFLECTION;
  case K::S_ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BORROW:
    return KF::REFLECTION;
  case K::S_BORROW_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // ASSIGNMENT
  case K::S_ASSIGN:
    return KF::STATEMENT | KF::VIGNETTE_RVALUE;
  case K::S_ASSIGN_ADD:
    return KF::STATEMENT;
  case K::S_ASSIGN_SUBTRACT:
    return KF::STATEMENT;
  case K::S_ASSIGN_MULTIPLY:
    return KF::STATEMENT;
  case K::S_ASSIGN_DIVIDE:
    return KF::STATEMENT;
  case K::S_ASSIGN_MODULUS:
    return KF::STATEMENT;

  // SUBTYPE
  case K::S_ARRAY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_REFERENCE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_FAT_POINTER:
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
  case K::DEBUG_TRAP_ON_PANIC:
    return KF::TYPE_ATTRIBUTE;
  case K::DYNAMIC_CAPTURE_LAYOUT:
    return KF::TYPE_ATTRIBUTE;

  // PARAMETER RULES
  case K::S_POSITIONAL_PARAMETERS_END:
    return KF::PARAMETER;
  case K::S_NAMED_PARAMETERS_BEGIN:
    return KF::PARAMETER;

  // BRACES
  case K::S_TUPLE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_LAYOUT_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_NULL:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_NULL_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_STRUCTURED_BINDING:
    return KF::STATEMENT;
  case K::S_IGNORE:
    return KF::STATEMENT;
  case K::S_SPECIALIZATION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::S_CALL:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::CONSTRUCT_FUNCTOR:
    return KF::REFLECTION;
  case K::S_CONSTRUCT_FUNCTOR_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_NAMED_ARGUMENT:
    return KF::ARGUMENT;
  case K::S_INDEX_INTO:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::S_SIGNATURE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_DEFAULT_VALUE_PARAMETER:
    return KF::PARAMETER;
  case K::DESTROY:
    return KF::REFLECTION;
  case K::S_DESTROY_VALUE:
    return KF::STATEMENT;
  case K::DROP:
    return KF::REFLECTION;
  case K::S_DROP_VALUE:
    return KF::STATEMENT;
  case K::MOVE:
    return KF::REFLECTION;
  case K::S_MOVE_VALUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ENTRY_POINT:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;
  case K::FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::CONSTRUCTOR:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::LAYOUT_CONSTRUCTOR:
    return KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::DESTRUCTOR:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::RANGER:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::S_ANONYMOUS_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::RVALUE | KF::ARGUMENT;
  case K::S_DYNAMIC_CAPTURE:
    return KF::DYNAMIC_CAPTURE | KF::RVALUE | KF::ARGUMENT;

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
  case K::OBJECT:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::ENUMERATION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;

  // VALUES;
  case K::TRUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::FALSE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INDETERMINATE:
    return KF::RVALUE;
  case K::VALUE:
    return KF::VIGNETTE_RVALUE;
  case K::INDEX:
    return KF::VIGNETTE_RVALUE;
  case K::DISCRIMINANT:
    return KF::VIGNETTE_RVALUE;
  case K::OUT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::THIS:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::THIS_TYPE:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::RESULT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::COMMAND_LINE_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXIT_CODE:
    return KF::RVALUE | KF::LVALUE;
  case K::ADDRESS_BYTE_SIZE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ADDRESS_BIT_DEPTH:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITS_PER_BYTE:
    return KF::RVALUE | KF::ARGUMENT;

  // BUILTIN TYPES
  case K::VOID:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NO_RETURN:
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
  case K::S_FIRST_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KF::REFLECTION;
  case K::S_NEXT_VARIADIC_ARGUMENT_OF:
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
  case K::DEFAULT:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK |
           KF::ARM_CHAINLINK;
  case K::FOR:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::WHILE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::SCOPE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::INLINE_SCOPE:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;
  case K::BLOCK:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::INLINE_BLOCK:
    return KF::STATEMENT_BRANCHES | KF::RVALUE;

  // RANGES
  case K::RANGE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::S_SEQUENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::S_SEQUENCE_STEP_ADD:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_STEP_SUBTRACT:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_STEP_MULTIPLY:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_STEP_DIVIDE:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_STEP_MODULUS:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_LESS:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_GREATER:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_LESS_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_GREATER_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::S_SEQUENCE_WHILE_NOT_EQUAL:
    return KF::SEQUENCE_STAGE;

  // ACCESS MODIFIERS
  case K::PUBLIC:
    return KF::STATEMENT_ATTRIBUTE;
  case K::PROTECTED:
    return KF::STATEMENT_ATTRIBUTE;
  case K::EXPORT:
    return KF::STATEMENT_ATTRIBUTE;

  // TABLE GRAPH
  case K::IMPORT:
    return KF::STATEMENT;
  case K::USE:
    return KF::STATEMENT;
  case K::USE_TABLE:
    return KF::STATEMENT;
  case K::FACADE:
    return KF::STATEMENT;
  case K::TABLE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE | KF::REFLECTION;
  case K::S_MODULE_TRUNK:
    return KF::STATEMENT_BRANCHES | KF::NONE; // TRUNK

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    return KF::STATEMENT;
  case K::DEBUG_TRAP:
    return KF::STATEMENT;

  // HINTS
  case K::UNREACHABLE:
    return KF::STATEMENT;
  case K::ASSUME:
    return KF::STATEMENT;

  // STATEMENT ATTRIBUTES
  case K::OPAQUE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::GLOBAL:
    return KF::STATEMENT_ATTRIBUTE;
  case K::STATIC:
    return KF::STATEMENT_ATTRIBUTE;
  case K::STATIC_CAPTURE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::EVALUATE_IMMEDIATELY:
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
  case K::S_MANGLED_NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::PACK:
    return KF::STATEMENT_ATTRIBUTE;
  case K::USER_ATTRIBUTE:
    return KF::REFLECTION | KF::STATEMENT_ATTRIBUTE;
  case K::S_USER_ATTRIBUTE_OF:
    return KF::RVALUE | KF::ARGUMENT;
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
  case K::MAY_COPY:
    return KF::STATEMENT_ATTRIBUTE;
  case K::MAY_MOVE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::MUTABILITY_CLASS:
    return KF::STATEMENT_ATTRIBUTE;

  // EXPRESSIONS
  case K::QUOTE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPRESSION:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPAND:
    return KF::REFLECTION;
  case K::S_EXPAND_STATEMENT:
    return KF::STATEMENT;
  case K::S_EXPAND_LVALUE:
    return KF::LVALUE;
  case K::S_EXPAND_RVALUE:
    return KF::RVALUE;
  case K::S_EXPAND_REFLECTION:
    return KF::REFLECTION;
  case K::S_EXPAND_ARGUMENT:
    return KF::ARGUMENT;
  case K::S_EXPAND_PARAMETER:
    return KF::PARAMETER;
  case K::S_EXPAND_SYMBOL_PATH:
    return KF::SYMBOL_PATH;
  case K::S_EXPAND_SEQUENCE_STAGE:
    return KF::SEQUENCE_STAGE;
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    return KF::DYNAMIC_CAPTURE;
  case K::S_EXPAND_VIGNETTE:
    return KF::VIGNETTE;
  case K::S_EXPAND_VIGNETTE_RVALUE:
    return KF::VIGNETTE_RVALUE;

  // REFLECTIONS
  case K::S_REFLECT:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::REFLECTION |
           KF::ARGUMENT | KF::PARAMETER | KF::SYMBOL_PATH | KF::SEQUENCE_STAGE |
           KF::DYNAMIC_CAPTURE | KF::VIGNETTE | KF::VIGNETTE_RVALUE;
  case K::BYTE_SIZE:
    return KF::REFLECTION;
  case K::S_BYTE_SIZE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BIT_DEPTH:
    return KF::REFLECTION;
  case K::S_BIT_DEPTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ELEMENT_COUNT:
    return KF::REFLECTION;
  case K::S_ELEMENT_COUNT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NAME:
    return KF::REFLECTION;
  case K::S_NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LINE:
    return KF::REFLECTION;
  case K::S_LINE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COLUMN:
    return KF::REFLECTION;
  case K::S_COLUMN_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS:
    return KF::REFLECTION;
  case K::S_IS_TYPE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HOLDS:
    return KF::REFLECTION;
  case K::S_HOLDS_ENUMERATOR:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION;
  case K::S_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SYMBOL:
    return KF::REFLECTION;
  case K::S_SYMBOL_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SIGNATURE:
    return KF::REFLECTION;
  case K::S_SIGNATURE_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LAYOUT:
    return KF::REFLECTION;
  case K::S_LAYOUT_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  case K::I_LAST:
    break;
  }
  return KF::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsParameterMark(rq::Keyword keyword) {
  return keyword == rq::Keyword::S_NAMED_PARAMETERS_BEGIN ||
         keyword == rq::Keyword::S_POSITIONAL_PARAMETERS_END;
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
getIsInternal(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasStatementBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasNonStatementBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasNone(flags, rq::KeywordFlags::STATEMENT_BRANCHES);
}

[[nodiscard]] inline constexpr rq::Keyword
getDeuniversalized(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  switch (keyword) {
  // MEMORY
  case K::SINGLETON:
    return K::S_SINGLETON_OF;
  case K::CONTENT:
    return K::S_CONTENT_OF;
  case K::ADDRESS:
    return K::S_ADDRESS_OF;
  case K::BORROW:
    return K::S_BORROW_OF;
  // PROCEDURES
  case K::DESTROY:
    return K::S_DESTROY_VALUE;
  case K::DROP:
    return K::S_DROP_VALUE;
  case K::MOVE:
    return K::S_MOVE_VALUE;
  case K::CONSTRUCT_FUNCTOR:
    return K::S_CONSTRUCT_FUNCTOR_OF;
  // VARIADIC ARGUMENTS
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::S_FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::S_NEXT_VARIADIC_ARGUMENT_OF;
  // STATEMENT ATTRIBUTES
  case K::MANGLED_NAME:
    return K::S_MANGLED_NAME_OF;
  case K::USER_ATTRIBUTE:
    return K::S_USER_ATTRIBUTE_OF;
  // REFLECTIONS
  case K::BYTE_SIZE:
    return K::S_BYTE_SIZE_OF;
  case K::BIT_DEPTH:
    return K::S_BIT_DEPTH_OF;
  case K::ELEMENT_COUNT:
    return K::S_ELEMENT_COUNT_OF;
  case K::NAME:
    return K::S_NAME_OF;
  case K::LINE:
    return K::S_LINE_OF;
  case K::COLUMN:
    return K::S_COLUMN_OF;
  case K::IS:
    return K::S_IS_TYPE;
  case K::HOLDS:
    return K::S_HOLDS_ENUMERATOR;
  case K::TYPE:
    return K::S_TYPE_OF;
  case K::SYMBOL:
    return K::S_SYMBOL_OF;
  case K::SIGNATURE:
    return K::S_SIGNATURE_OF;
  case K::LAYOUT:
    return K::S_LAYOUT_OF;
  default:
    break;
  }
  return K::I_NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsDeuniversalizable(rq::Keyword keyword) {
  return rq::getDeuniversalized(keyword) != rq::Keyword::I_NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::STARTING_CHAINLINK |
                                   rq::KeywordFlags::CONTINUING_CHAINLINK |
                                   rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeStartingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STARTING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeContinuingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::CONTINUING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeFinishingChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::FINISHING_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeAllChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHAINLINK |
                                  rq::KeywordFlags::ARM_CHAINLINK;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeIfChainLink(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHAINLINK);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeArmChainLink(rq::Keyword keyword) {
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
  STATEMENT_ATTRIBUTE,
  SEQUENCE_STAGE,
  DYNAMIC_CAPTURE,
  VIGNETTE,
  VIGNETTE_RVALUE
};

[[nodiscard]] RQ_ALWAYS_INLINE constexpr llvm::StringRef
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
  case S::STATEMENT_ATTRIBUTE:
    return "statement attribute";
  case S::SEQUENCE_STAGE:
    return "sequence stage expression";
  case S::DYNAMIC_CAPTURE:
    return "dynamic capture expression";
  case S::VIGNETTE:
    return "vignette expression";
  case S::VIGNETTE_RVALUE:
    return "vignette rvalue expression";
  }
  return "error expression";
}

[[nodiscard]] inline rq::Keyword getExpandOfSituation(rq::Situation situation) {
  using namespace rq;
  using K = Keyword;
  using S = Situation;
  switch (situation) {
  case S::NONE:
  case S::TRUNK:
    break;
  case S::STATEMENT:
    return K::S_EXPAND_STATEMENT;
  case S::LVALUE:
    return K::S_EXPAND_LVALUE;
  case S::RVALUE:
    return K::S_EXPAND_RVALUE;
  case S::REFLECTION:
    return K::S_EXPAND_REFLECTION;
  case S::ARGUMENT:
    return K::S_EXPAND_ARGUMENT;
  case S::PARAMETER:
    return K::S_EXPAND_PARAMETER;
  case S::BINDING:
    break;
  case S::SYMBOL_PATH:
    return K::S_EXPAND_SYMBOL_PATH;
  case S::ASCRIPTION:
  case S::TYPE_ATTRIBUTE:
  case S::STATEMENT_ATTRIBUTE:
    break;
  case S::SEQUENCE_STAGE:
    return K::S_EXPAND_SEQUENCE_STAGE;
  case S::DYNAMIC_CAPTURE:
    return K::S_EXPAND_DYNAMIC_CAPTURE;
  case S::VIGNETTE:
    return K::S_EXPAND_VIGNETTE;
  case S::VIGNETTE_RVALUE:
    return K::S_EXPAND_VIGNETTE_RVALUE;
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
  case K::S_EXPAND_STATEMENT:
    return S::STATEMENT;
  case K::S_EXPAND_LVALUE:
    return S::LVALUE;
  case K::S_EXPAND_RVALUE:
    return S::RVALUE;
  case K::S_EXPAND_REFLECTION:
    return S::REFLECTION;
  case K::S_EXPAND_ARGUMENT:
    return S::ARGUMENT;
  case K::S_EXPAND_PARAMETER:
    return S::PARAMETER;
  case K::S_EXPAND_SYMBOL_PATH:
    return S::SYMBOL_PATH;
  case K::S_EXPAND_SEQUENCE_STAGE:
    return S::SEQUENCE_STAGE;
  case K::S_EXPAND_DYNAMIC_CAPTURE:
    return S::DYNAMIC_CAPTURE;
  case K::S_EXPAND_VIGNETTE:
    return S::VIGNETTE;
  case K::S_EXPAND_VIGNETTE_RVALUE:
    return S::VIGNETTE_RVALUE;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::I_NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeTrunk(rq::Keyword keyword) {
  return keyword == rq::Keyword::S_MODULE_TRUNK;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT);
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
getCanBeBinding(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::BINDING);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeSymbolPath(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::SYMBOL_PATH);
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

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeDynamicCapture(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::DYNAMIC_CAPTURE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeVignette(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::VIGNETTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeVignetteRValue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::VIGNETTE_RVALUE);
}

[[nodiscard]] constexpr bool getCanBeSituation(rq::Keyword keyword,
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
  case rq::Situation::STATEMENT_ATTRIBUTE:
    return rq::getCanBeStatementAttribute(keyword);
  case rq::Situation::SEQUENCE_STAGE:
    return rq::getCanBeSequenceStage(keyword);
  case rq::Situation::DYNAMIC_CAPTURE:
    return rq::getCanBeDynamicCapture(keyword);
  case rq::Situation::VIGNETTE:
    return rq::getCanBeVignette(keyword);
  case rq::Situation::VIGNETTE_RVALUE:
    return rq::getCanBeVignetteRValue(keyword);
  }
  return false;
}

enum class ChainKind : std::uint_fast8_t { NONE, UNKNOWN, IF, ARM };

[[nodiscard]] inline constexpr llvm::StringRef
getDescription(rq::ChainKind chainKind) {
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

enum class StatementAttribute : std::uint_fast8_t {
  NONE,
  OPAQUE,
  GLOBAL,
  STATIC,
  STATIC_CAPTURE,
  EVALUATE_IMMEDIATELY,
  MAY_PARENT,
  PARENT,
  ABSTRACT,
  VIRTUAL,
  OVERRIDE,
  POSITION,
  MANGLED_NAME,
  PACK,
  USER_ATTRIBUTE,
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
  MUTABILITY_CLASS
};

[[nodiscard]] inline constexpr llvm::StringRef
getName(rq::StatementAttribute attribute) {
  using namespace rq;
  using SA = StatementAttribute;
  switch (attribute) {
  case SA::NONE:
    return "none";
  case SA::OPAQUE:
    return "opaque";
  case SA::GLOBAL:
    return "global";
  case SA::STATIC:
    return "static";
  case SA::STATIC_CAPTURE:
    return "static_capture";
  case SA::EVALUATE_IMMEDIATELY:
    return "evaluate_immediately";
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
  case SA::USER_ATTRIBUTE:
    return "user_attribute";
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
  case SA::MUTABILITY_CLASS:
    return "mutability_class";
  }
  return "error";
}

[[nodiscard]] inline constexpr rq::StatementAttribute
getStatementAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using SA = StatementAttribute;
  switch (keyword) {
  case K::OPAQUE:
    return SA::OPAQUE;
  case K::GLOBAL:
    return SA::GLOBAL;
  case K::STATIC:
    return SA::STATIC;
  case K::STATIC_CAPTURE:
    return SA::STATIC_CAPTURE;
  case K::EVALUATE_IMMEDIATELY:
    return SA::EVALUATE_IMMEDIATELY;
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
  case K::USER_ATTRIBUTE:
    return SA::USER_ATTRIBUTE;
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
  case K::MUTABILITY_CLASS:
    return SA::MUTABILITY_CLASS;
  default:
    break;
  }
  return SA::NONE;
}

enum class StatementFlags : std::uint32_t {
  NONE = 0,
  OPAQUE = rq::getBit(31),
  GLOBAL = rq::getBit(30),
  STATIC = rq::getBit(29),
  STATIC_CAPTURE = rq::getBit(28),
  EVALUATE_IMMEDIATELY = rq::getBit(27),
  MAY_PARENT = rq::getBit(26),
  PARENT = rq::getBit(25),
  ABSTRACT = rq::getBit(24),
  VIRTUAL = rq::getBit(23),
  OVERRIDE = rq::getBit(22),
  POSITION = rq::getBit(21),
  MANGLED_NAME = rq::getBit(20),
  PACK = rq::getBit(19),
  USER_ATTRIBUTE = rq::getBit(18),
  LABEL = rq::getBit(17),
  TEMPLATE = rq::getBit(16),
  LIKELY = rq::getBit(15),
  UNLIKELY = rq::getBit(14),
  DEPRECIATED = rq::getBit(13),
  EXPORT = rq::getBit(12),
  PUBLIC = rq::getBit(11),
  PROTECTED = rq::getBit(10),
  MAY_COPY = rq::getBit(9),
  MAY_MOVE = rq::getBit(8),
  MUTABILITY_CLASS = rq::getBit(7)
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
  case SA::OPAQUE:
    return SF::OPAQUE;
  case SA::GLOBAL:
    return SF::GLOBAL;
  case SA::STATIC:
    return SF::STATIC;
  case SA::STATIC_CAPTURE:
    return SF::STATIC_CAPTURE;
  case SA::EVALUATE_IMMEDIATELY:
    return SF::EVALUATE_IMMEDIATELY;
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
  case SA::USER_ATTRIBUTE:
    return SF::USER_ATTRIBUTE;
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
  case SA::MUTABILITY_CLASS:
    return SF::MUTABILITY_CLASS;
  }
  return SF::NONE;
}

enum class TypeAttribute : std::uint_fast8_t {
  NONE,
  MUTABLE,
  CONSTANT,
  PARTIALLY_MUTABLE,
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
  case TA::PARTIALLY_MUTABLE:
    return "PARTIALLY_MUTABLE";
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
  case K::DEBUG_TRAP_ON_PANIC:
    return TA::DEBUG_TRAP_ON_PANIC;
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
  PARTIALLY_MUTABLE = rq::getBit(13),
  VOLATILE = rq::getBit(12),
  ATOMIC = rq::getBit(11),
  NULL_TERMINATED = rq::getBit(10),
  MAY_DISCARD = rq::getBit(9),
  DEBUG_TRAP_ON_PANIC = rq::getBit(8),
  LINEAR = rq::getBit(7),
  DYNAMIC_CAPTURE_LAYOUT = rq::getBit(6)
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
  case TA::LINEAR:
    return TF::LINEAR;
  case TA::DYNAMIC_CAPTURE_LAYOUT:
    return TF::DYNAMIC_CAPTURE_LAYOUT;
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

template <> struct is_flags<rq::ExpressionNextFlags> final : std::true_type {};

enum class ExpressionSourceFlags : std::uint8_t {
  NONE = 0,
  // NOTE: an "inserted" expression is one that was not present in the source
  // and was inserted into the AST by the compiler
  INSERTED = rq::getBit(0),
  // NOTE: this flag is set if there is an error with situating this expression
  SITUATOR_ERROR = rq::getBit(1)
};

template <>
struct is_flags<rq::ExpressionSourceFlags> final : std::true_type {};

struct Expression final {
  using Self = rq::Expression;

  rq::Keyword _keyword{rq::Keyword::I_NONE};
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
    this->_keyword = rq::Keyword::I_NONE;
    this->_next_ptr_flags = {};
    this->_branch_ptr = nullptr;
    this->_source_ptr_flags = {};
    this->_source_text_length = 0;
  }
  RQ_ALWAYS_INLINE void setKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword == rq::Keyword::I_NONE,
              "keyword must not already be set");
    this->_keyword = keyword;
  }
  RQ_ALWAYS_INLINE void changeKeyword(rq::Keyword keyword) {
    RQ_ASSERT(this->_keyword != rq::Keyword::I_NONE,
              "keyword must already be set");
    this->_keyword = keyword;
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsParameterMark() const {
    return rq::getIsParameterMark(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal() const {
    return rq::getIsInternal(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatementBranches() const {
    return rq::getHasStatementBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasNonStatementBranches() const {
    return rq::getHasNonStatementBranches(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getDeuniversalized() const {
    return rq::getDeuniversalized(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsDeuniversalizable() const {
    return rq::getIsDeuniversalizable(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatementAttribute() const {
    return rq::getCanBeStatementAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSequenceStage() const {
    return rq::getCanBeSequenceStage(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeDynamicCapture() const {
    return rq::getCanBeDynamicCapture(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeSituation(rq::Situation situation) const {
    return rq::getCanBeSituation(this->getKeyword(), situation);
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
  RQ_ALWAYS_INLINE bool getHasBranch() const {
    return this->_branch_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE bool getHasNext() const {
    return this->_next_ptr_flags.getPtr() != nullptr;
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
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(nullptr);
    return *old_next_ptr;
  }
  inline rq::Expression *popNextPtr() {
    RQ_ASSERT(this->getHasNext(), "does not have next");
    rq::Expression *old_next_ptr = this->_next_ptr_flags.getPtr();
    this->_next_ptr_flags.setPtr(nullptr);
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
