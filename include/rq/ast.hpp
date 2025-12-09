#pragma once

#include <rq/utility.hpp>

#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstdint>

namespace rq {

// NOTE:
//  two underscores at the front means that the keyword is for internal use only

// NOTE:
//  one underscore at the front means that the keyword is not meant to be
//  normally used directly. Might be used by a standard library, or might be
//  useful only in symbolic requite. There should be other ways to write these
//  things most of the time that are more concise.

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
  _BINDING,
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
  _LAYOUT_TYPE,
  _NULL,
  _NULL_TYPE,
  _STRUCTURED_BINDING,
  _IGNORE,
  _SPECIALIZATION,

  // PROCEDURES
  _CALL,
  _NAMED_ARGUMENT,
  _INDEX,
  _SIGNATURE_TYPE,
  _DEFAULT_VALUE_PARAMETER,
  DESTROY,
  _DESTROY_VALUE,
  DROP,
  _DROP_VALUE,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  CONSTRUCTOR,
  _CONSTRUCTOR_OF,
  LAYOUT_CONSTRUCTOR,
  _LAYOUT_CONSTRUCTOR_OF,
  DESTRUCTOR,
  _DESTRUCTOR_OF,
  CALLER,
  _CALLER_OF,
  RANGER,
  _RANGER_OF,
  DEEP_COPIER,
  _DEEP_COPIER_OF,
  RETAIN_MOVER,
  _RETAIN_MOVER_OF,
  DROP_MOVER,
  _DROP_MOVER_OF,
  SWAPER,
  _SWAPER_OF,
  INDEXER,
  _INDEXER_OF,
  _ANONYMOUS_FUNCTION,
  _DYNAMIC_CAPTURE,

  // CONTROL FLOW
  RETURN,
  RETURN_RESULT,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  RANGE_OVER,

  // DECLARED TYPES
  OBJECT,
  _OBJECT_OF,
  ENUMERATION,
  _ENUMERATION_OF,
  _ENUMERATOR_WITH_DISCRIMINANT,

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
  _SCOPE_OF,
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

  // TABLE GRAPH
  IMPORT,
  USE,
  USE_TABLE,
  TABLE,
  _TABLE_OF,
  MODULE,
  _MODULE_OF,
  _MODULE_ROOT,

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
  _MANGLED_NAME_OF,
  PACK,
  USER,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  MAY_SHALLOW_COPY,

  // EXPRESSIONS
  QUOTE,
  EXPRESSION,
  EXPAND,
  _EXPAND_TOP_STATEMENT,
  _EXPAND_TABLE_STATEMENT,
  _EXPAND_OBJECT_STATEMENT,
  _EXPAND_LOCAL_STATEMENT,
  _EXPAND_LVALUE,
  _EXPAND_RVALUE,
  _EXPAND_REFLECTION,
  _EXPAND_ARGUMENT,
  _EXPAND_PARAMETER,
  _EXPAND_ENUMERATOR,
  _EXPAND_PATH,
  _EXPAND_NAME,
  _EXPAND_SEQUENCE_STAGE,
  _EXPAND_ARM,
  _EXPAND_DYNAMIC_CAPTURE,

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
  LAYOUT,
  _LAYOUT_OF,
  SIGNATURE,
  _SIGNATURE_OF,

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
  case K::_BINDING:
    return "_binding";
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
  case K::_LAYOUT_TYPE:
    return "_layout_type";
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
  case K::_SIGNATURE_TYPE:
    return "_signature_type";
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
  case K::METHOD:
    return "method";
  case K::CONSTRUCTOR:
    return "constructor";
  case K::_CONSTRUCTOR_OF:
    return "_constructor_of";
  case K::LAYOUT_CONSTRUCTOR:
    return "layout_constructor";
  case K::_LAYOUT_CONSTRUCTOR_OF:
    return "_layout_constructor_of";
  case K::DESTRUCTOR:
    return "destructor";
  case K::_DESTRUCTOR_OF:
    return "_destructor_of";
  case K::CALLER:
    return "caller";
  case K::_CALLER_OF:
    return "_caller_of";
  case K::RANGER:
    return "ranger";
  case K::_RANGER_OF:
    return "_ranger_of";
  case K::DEEP_COPIER:
    return "deep_copier";
  case K::_DEEP_COPIER_OF:
    return "_deep_copier_of";
  case K::RETAIN_MOVER:
    return "retain_mover";
  case K::_RETAIN_MOVER_OF:
    return "_retain_mover_of";
  case K::DROP_MOVER:
    return "drop_mover";
  case K::_DROP_MOVER_OF:
    return "_drop_mover_of";
  case K::SWAPER:
    return "swaper";
  case K::_SWAPER_OF:
    return "_swaper_of";
  case K::INDEXER:
    return "indexer";
  case K::_INDEXER_OF:
    return "_indexer_of";
  case K::_ANONYMOUS_FUNCTION:
    return "_anonymous_function";
  case K::_DYNAMIC_CAPTURE:
    return "_dynamic_capture";

  // CONTROL FLOW
  case K::RETURN:
    return "return";
  case K::RETURN_RESULT:
    return "return_result";
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
  case K::_OBJECT_OF:
    return "_object_of";
  case K::ENUMERATION:
    return "enumeration";
  case K::_ENUMERATION_OF:
    return "_enumeration_of";
  case K::_ENUMERATOR_WITH_DISCRIMINANT:
    return "_enumerator_with_discriminant";

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
  case K::_SCOPE_OF:
    return "_scope_of";
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

  // TABLE GRAPH
  case K::IMPORT:
    return "import";
  case K::USE:
    return "use";
  case K::USE_TABLE:
    return "use_table";
  case K::TABLE:
    return "table";
  case K::_TABLE_OF:
    return "_table_of";
  case K::MODULE:
    return "module";
  case K::_MODULE_OF:
    return "_module_of";
  case K::_MODULE_ROOT:
    return "_module_root";

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
  case K::MAY_SHALLOW_COPY:
    return "may_shallow_copy";

  // EXPRESSIONS
  case K::QUOTE:
    return "quote";
  case K::EXPRESSION:
    return "expression";
  case K::EXPAND:
    return "expand";
  case K::_EXPAND_TOP_STATEMENT:
    return "_expand_top_statement";
  case K::_EXPAND_TABLE_STATEMENT:
    return "_expand_table_statement";
  case K::_EXPAND_OBJECT_STATEMENT:
    return "_expand_object_statement";
  case K::_EXPAND_LOCAL_STATEMENT:
    return "_expand_local_statement";
  case K::_EXPAND_LVALUE:
    return "_expand_lvalue";
  case K::_EXPAND_RVALUE:
    return "_expand_rvalue";
  case K::_EXPAND_REFLECTION:
    return "_expand_reflection";
  case K::_EXPAND_ARGUMENT:
    return "_expand_argument";
  case K::_EXPAND_PARAMETER:
    return "_expand_parameter";
  case K::_EXPAND_ENUMERATOR:
    return "_expand_enumerator";
  case K::_EXPAND_PATH:
    return "_expand_path";
  case K::_EXPAND_NAME:
    return "_expand_name";
  case K::_EXPAND_SEQUENCE_STAGE:
    return "_expand_sequence_stage";
  case K::_EXPAND_ARM:
    return "_expand_arm";
  case K::_EXPAND_DYNAMIC_CAPTURE:
    return "_expand_dynamic_capture";

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
  case K::SIGNATURE:
    return "signature";
  case K::_SIGNATURE_OF:
    return "_signature_of";
  case K::LAYOUT:
    return "layout";
  case K::_LAYOUT_OF:
    return "_layout_of";

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
  INTERNAL = rq::getBit(27),
  HAS_SEMICOLON_SEPARATED_BRANCHES = rq::getBit(26),
  NULLARY_WHEN_NO_BRANCHES = rq::getBit(25),
  // ROOT_STATEMENT
  TOP_STATEMENT = rq::getBit(24),
  TABLE_STATEMENT = rq::getBit(23),
  OBJECT_STATEMENT = rq::getBit(22),
  LOCAL_STATEMENT = rq::getBit(21),
  RVALUE = rq::getBit(20),
  LVALUE = rq::getBit(19),
  REFLECTION = rq::getBit(18),
  ARGUMENT = rq::getBit(17),
  PARAMETER = rq::getBit(16),
  ENUMERATOR = rq::getBit(15),
  PATH = rq::getBit(14),
  NAME = rq::getBit(13),
  ASCRIPTION = rq::getBit(12),
  TYPE_ATTRIBUTE = rq::getBit(11),
  STATEMENT_ATTRIBUTE = rq::getBit(10),
  SEQUENCE_STAGE = rq::getBit(9),
  ARM = rq::getBit(8),
  DYNAMIC_CAPTURE = rq::getBit(7),
  IF_CHUNK_NOT_START = rq::getBit(6),
  IF_CHUNK_NOT_END = rq::getBit(5),
  COMMA_BRANCH_COUNT_MASK = 0x3,
  ALL = TOP_STATEMENT | TABLE_STATEMENT | OBJECT_STATEMENT | LOCAL_STATEMENT |
        RVALUE | LVALUE | REFLECTION | ARGUMENT | PARAMETER | ENUMERATOR |
        PATH | NAME | ASCRIPTION | TYPE_ATTRIBUTE | STATEMENT_ATTRIBUTE |
        SEQUENCE_STAGE | ARM | DYNAMIC_CAPTURE
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
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT | KF::LVALUE | KF::PATH |
           KF::NAME | KF::SEQUENCE_STAGE;
  case K::_EQUAL_OPERATOR:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | KF::ARGUMENT | KF::PARAMETER | KF::ENUMERATOR;
  case K::_COLON_OPERATOR:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::ENUMERATOR;
  case K::_INFERENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_TACIT_COMMA_EXPRESSION:
    return KF::RVALUE;

  // LOGICAL
  case K::_LOGICAL_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LOGICAL_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LOGICAL_COMPLEMENT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // COMPARISON
  case K::_GREATER:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_GREATER_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LESS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_LESS_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_NOT_EQUAL:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // APPLY
  case K::_EXTEND:
    return KF::RVALUE;
  case K::_BINDING:
    return KF::LVALUE | KF::PARAMETER | KF::ARGUMENT | KF::ENUMERATOR;
  case K::_ASCRIBE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION |
           KF::ASCRIPTION;
  case K::_ASCRIBE_STATEMENT:
    return KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::PARAMETER | KF::ASCRIPTION;
  case K::_ASCRIBE_TYPE_OF_ELEMENTS:
    return KF::RVALUE | KF::ASCRIPTION;
  case K::_CAST:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_IDENTIFY:
    return KF::PATH | KF::NAME | KF::RVALUE | KF::ARGUMENT;

  // ARITHMETIC
  case K::_ADD:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_SUBTRACT:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_MULTIPLY:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_DIVIDE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_MODULUS:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_NEGATE:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;

  // BITWISE
  case K::_BITWISE_CAST:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_OR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_AND:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_XOR:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_COMPLEMENT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_SHIFT_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_BITWISE_SHIFT_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_LEFT:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BITWISE_ROTATE_RIGHT:
    return KF::RVALUE | KF::ARGUMENT;

  // MEMORY
  case K::SINGLETON:
    return KF::REFLECTION;
  case K::_SINGLETON_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::CONTENT:
    return KF::REFLECTION;
  case K::_CONTENT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ADDRESS:
    return KF::REFLECTION;
  case K::_ADDRESS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::BORROW:
    return KF::REFLECTION;
  case K::_BORROW_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VIEW:
    return KF::REFLECTION;
  case K::_VIEW_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SLICE:
    return KF::REFLECTION;
  case K::_SLICE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::ARRAY_SLICE:
    return KF::REFLECTION;
  case K::_ARRAY_SLICE_OF:
    return KF::RVALUE | KF::ARGUMENT;

  // ASSIGNMENT
  case K::_ASSIGN:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_ASSIGN_ADD:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_ASSIGN_SUBTRACT:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_ASSIGN_MULTIPLY:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_ASSIGN_DIVIDE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_ASSIGN_MODULUS:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

  // MOVE SEMANTICS
  case K::DEEP_COPY:
    return KF::REFLECTION;
  case K::_DEEP_COPY_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::RETAIN_MOVE:
    return KF::REFLECTION;
  case K::_RETAIN_MOVE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DROP_MOVE:
    return KF::REFLECTION;
  case K::_DROP_MOVE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LINEAR_ASSIGN:
    return KF::REFLECTION;
  case K::_LINEAR_ASSIGN_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SWAP:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

  // SUBTYPE
  case K::_ARRAY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_REFERENCE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_FAT_POINTER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

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
    return KF::PARAMETER;
  case K::_NAMED_PARAMETERS_BEGIN:
    return KF::PARAMETER;

  // BRACES
  case K::_TUPLE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_LAYOUT_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_NULL:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_NULL_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_STRUCTURED_BINDING:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_IGNORE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::_SPECIALIZATION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::_CALL:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::_NAMED_ARGUMENT:
    return KF::ARGUMENT;
  case K::_INDEX:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::_SIGNATURE_TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::_DEFAULT_VALUE_PARAMETER:
    return KF::PARAMETER;
  case K::DESTROY:
    return KF::REFLECTION;
  case K::_DESTROY_VALUE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::DROP:
    return KF::REFLECTION;
  case K::_DROP_VALUE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::ENTRY_POINT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::RVALUE;
  case K::FUNCTION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 2;
  case K::METHOD:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 2;
  case K::CONSTRUCTOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE | 2;
  case K::_CONSTRUCTOR_OF:
    return KF::RVALUE;
  case K::LAYOUT_CONSTRUCTOR:
    return KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_LAYOUT_CONSTRUCTOR_OF:
    return KF::RVALUE;
  case K::DESTRUCTOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_DESTRUCTOR_OF:
    return KF::RVALUE;
  case K::CALLER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE | 1;
  case K::_CALLER_OF:
    return KF::RVALUE;
  case K::RANGER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE | 1;
  case K::_RANGER_OF:
    return KF::RVALUE;
  case K::DEEP_COPIER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_DEEP_COPIER_OF:
    return KF::RVALUE;
  case K::RETAIN_MOVER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_RETAIN_MOVER_OF:
    return KF::RVALUE;
  case K::DROP_MOVER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_DROP_MOVER_OF:
    return KF::RVALUE;
  case K::SWAPER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE;
  case K::_SWAPER_OF:
    return KF::RVALUE;
  case K::INDEXER:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::NULLARY_WHEN_NO_BRANCHES |
           KF::OBJECT_STATEMENT | KF::REFLECTION | KF::RVALUE | 1;
  case K::_INDEXER_OF:
    return KF::RVALUE;
  case K::_ANONYMOUS_FUNCTION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | KF::ARGUMENT | 2;
  case K::_DYNAMIC_CAPTURE:
    return KF::DYNAMIC_CAPTURE;

  // CONTROL FLOW
  case K::RETURN:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::RETURN_RESULT:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::BREAK:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::CONTINUE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::FALLTHROUGH:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::GOTO:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::RANGE_OVER:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

  // DECLARED TYPES
  case K::OBJECT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT |
           KF::REFLECTION | KF::RVALUE | 2;
  case K::_OBJECT_OF:
    return KF::RVALUE;
  case K::ENUMERATION:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | KF::LOCAL_STATEMENT |
           KF::REFLECTION | KF::RVALUE | 2;
  case K::_ENUMERATION_OF:
    return KF::RVALUE;
  case K::_ENUMERATOR_WITH_DISCRIMINANT:
    return KF::ENUMERATOR;

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
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NEXT_VARIADIC_ARGUMENT:
    return KF::REFLECTION;
  case K::_NEXT_VARIADIC_ARGUMENT_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // SCOPES
  case K::IF:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::IF_CHUNK_NOT_END | 1;
  case K::ELSE_IF:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::IF_CHUNK_NOT_START | KF::IF_CHUNK_NOT_END | 1;
  case K::ELSE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::IF_CHUNK_NOT_START;
  case K::MATCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT | 1;
  case K::INLINE_MATCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;
  case K::SWITCH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::ARM |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::CASE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::ARM | 1;
  case K::DEFAULT:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::ARM;
  case K::FOR:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::FOR_EACH:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 2;
  case K::WHILE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT | 1;
  case K::SCOPE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::REFLECTION | KF::RVALUE;
  case K::INLINE_SCOPE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;
  case K::_SCOPE_OF:
    return KF::RVALUE;
  case K::BLOCK:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::LOCAL_STATEMENT |
           KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT;
  case K::INLINE_BLOCK:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::RVALUE | 1;

  // RANGES
  case K::RANGE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CONTAINS:
    return KF::REFLECTION;
  case K::_CONTAINS_VALUE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_SEQUENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::_SEQUENCE_STEP_ADD:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_SUBTRACT:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_MULTIPLY:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_DIVIDE:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_STEP_MODULUS:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_LESS:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_GREATER:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_LESS_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_GREATER_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_EQUAL:
    return KF::SEQUENCE_STAGE;
  case K::_SEQUENCE_WHILE_NOT_EQUAL:
    return KF::SEQUENCE_STAGE;

  // ACCESS MODIFIERS
  case K::PRIVATE:
    return KF::STATEMENT_ATTRIBUTE;
  case K::PROTECTED:
    return KF::STATEMENT_ATTRIBUTE;
  case K::EXPORT:
    return KF::STATEMENT_ATTRIBUTE;

  // TABLE GRAPH
  case K::IMPORT:
    return KF::TOP_STATEMENT;
  case K::USE:
    return KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::USE_TABLE:
    return KF::TOP_STATEMENT | KF::TABLE_STATEMENT | KF::OBJECT_STATEMENT |
           KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::TABLE:
    return KF::HAS_SEMICOLON_SEPARATED_BRANCHES | KF::TOP_STATEMENT |
           KF::TABLE_STATEMENT | KF::RVALUE | KF::REFLECTION | 1;
  case K::_TABLE_OF:
    return KF::RVALUE;
  case K::MODULE:
    return KF::REFLECTION | KF::RVALUE;
  case K::_MODULE_OF:
    return KF::RVALUE;
  case K::_MODULE_ROOT:
    return KF::NONE; // KF::ROOT_STATEMENT

  // ERROR HANDLING AND DEBUGGING
  case K::PANIC_TRAP:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::DEBUG_TRAP:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

  // HINTS
  case K::UNREACHABLE:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;
  case K::ASSUME:
    return KF::LOCAL_STATEMENT | KF::TOP_STATEMENT | KF::TABLE_STATEMENT |
           KF::OBJECT_STATEMENT;

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
  case K::_MANGLED_NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
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
  case K::MAY_SHALLOW_COPY:
    return KF::STATEMENT_ATTRIBUTE;

  // EXPRESSIONS
  case K::QUOTE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPRESSION:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPAND:
    return KF::REFLECTION;
  case K::_EXPAND_TOP_STATEMENT:
    return KF::TOP_STATEMENT;
  case K::_EXPAND_TABLE_STATEMENT:
    return KF::TABLE_STATEMENT;
  case K::_EXPAND_OBJECT_STATEMENT:
    return KF::OBJECT_STATEMENT;
  case K::_EXPAND_LOCAL_STATEMENT:
    return KF::LOCAL_STATEMENT;
  case K::_EXPAND_LVALUE:
    return KF::LVALUE;
  case K::_EXPAND_RVALUE:
    return KF::RVALUE;
  case K::_EXPAND_REFLECTION:
    return KF::REFLECTION;
  case K::_EXPAND_ARGUMENT:
    return KF::ARGUMENT;
  case K::_EXPAND_PARAMETER:
    return KF::PARAMETER;
  case K::_EXPAND_ENUMERATOR:
    return KF::ENUMERATOR;
  case K::_EXPAND_PATH:
    return KF::PATH;
  case K::_EXPAND_NAME:
    return KF::NAME;
  case K::_EXPAND_SEQUENCE_STAGE:
    return KF::SEQUENCE_STAGE;
  case K::_EXPAND_ARM:
    return KF::ARM;
  case K::_EXPAND_DYNAMIC_CAPTURE:
    return KF::DYNAMIC_CAPTURE;

  // REFLECTIONS
  case K::_REFLECT:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER | KF::PATH;
  case K::_MEMBER_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PATH;
  case K::SIZE:
    return KF::REFLECTION;
  case K::_SIZE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DEPTH:
    return KF::REFLECTION;
  case K::_DEPTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COUNT:
    return KF::REFLECTION;
  case K::_COUNT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LENGTH:
    return KF::REFLECTION;
  case K::_LENGTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SPAN_SIZE:
    return KF::REFLECTION;
  case K::_SPAN_SIZE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SPAN_DEPTH:
    return KF::REFLECTION;
  case K::_SPAN_DEPTH_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NAME:
    return KF::REFLECTION;
  case K::_NAME_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LINE:
    return KF::REFLECTION;
  case K::_LINE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COLUMN:
    return KF::REFLECTION;
  case K::_COLUMN_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::IS:
    return KF::REFLECTION;
  case K::_VALUE_IS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::HOLDS:
    return KF::REFLECTION;
  case K::_VALUE_HOLDS:
    return KF::RVALUE | KF::ARGUMENT;
  case K::TYPE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER | KF::REFLECTION;
  case K::_TYPE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SYMBOL:
    return KF::REFLECTION;
  case K::_SYMBOL_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DISCRIMINANT:
    return KF::REFLECTION;
  case K::_DISCRIMINANT_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SIGNATURE:
    return KF::REFLECTION;
  case K::_SIGNATURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::LAYOUT:
    return KF::REFLECTION;
  case K::_LAYOUT_OF:
    return KF::RVALUE | KF::ARGUMENT;

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
getIsInternal(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::INTERNAL);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getHasSemicolonSeparatedBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags,
                       rq::KeywordFlags::HAS_SEMICOLON_SEPARATED_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsNullaryWhenNoBranches(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NULLARY_WHEN_NO_BRANCHES);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsIfChunkNotStart(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHUNK_NOT_START);
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getIsIfChunkNotEnd(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::IF_CHUNK_NOT_END);
}

enum class Situation : std::uint_fast8_t {
  NONE,
  ROOT_STATEMENT,
  TOP_STATEMENT,
  TABLE_STATEMENT,
  OBJECT_STATEMENT,
  LOCAL_STATEMENT,
  LVALUE,
  RVALUE,
  REFLECTION,
  ARGUMENT,
  PARAMETER,
  ENUMERATOR,
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
  case S::ROOT_STATEMENT:
    return "root statement";
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
  case S::ENUMERATOR:
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

template <rq::Situation SITUATION_PARAM>
[[nodiscard]] RQ_ALWAYS_INLINE constexpr rq::Keyword
getUniversalized(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using S = Situation;
  constexpr S SP = SITUATION_PARAM;
  if constexpr (SP == S::RVALUE || SP == S::ARGUMENT) {
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
    case K::DESTROY:
      return K::_DESTROY_VALUE;
    case K::DROP:
      return K::_DROP_VALUE;
    case K::CONSTRUCTOR:
      return K::_CONSTRUCTOR_OF;
    case K::LAYOUT_CONSTRUCTOR:
      return K::_LAYOUT_CONSTRUCTOR_OF;
    case K::DESTRUCTOR:
      return K::_DESTRUCTOR_OF;
    case K::CALLER:
      return K::_CALLER_OF;
    case K::RANGER:
      return K::_RANGER_OF;
    case K::DEEP_COPIER:
      return K::_DEEP_COPY_OF;
    case K::RETAIN_MOVER:
      return K::_RETAIN_MOVER_OF;
    case K::DROP_MOVER:
      return K::_DROP_MOVER_OF;
    case K::SWAPER:
      return K::_SWAPER_OF;
    case K::INDEXER:
      return K::_INDEXER_OF;
    case K::OBJECT:
      return K::_OBJECT_OF;
    case K::ENUMERATION:
      return K::_ENUMERATION_OF;
    case K::FIRST_VARIADIC_ARGUMENT:
      return K::_FIRST_VARIADIC_ARGUMENT_OF;
    case K::NEXT_VARIADIC_ARGUMENT:
      return K::_NEXT_VARIADIC_ARGUMENT_OF;
    case K::SCOPE:
      return K::_SCOPE_OF;
    case K::TABLE:
      return K::_TABLE_OF;
    case K::MODULE:
      return K::_MODULE_OF;
    case K::MANGLED_NAME:
      return K::_MANGLED_NAME_OF;
    case K::EXPAND:
      if constexpr (SP == S::RVALUE) {
        return K::_EXPAND_RVALUE;
      } else if constexpr (SP == S::ARGUMENT) {
        return K::_EXPAND_ARGUMENT;
      } else {
        static_assert(false, "invalid situtation");
      }
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
    case K::SIGNATURE:
      return K::_SIGNATURE_OF;
    case K::LAYOUT:
      return K::_LAYOUT_OF;
    default:
      break;
    }
  } else if constexpr (SP == S::TOP_STATEMENT) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_TOP_STATEMENT;
    default:
      break;
    }
  } else if constexpr (SP == S::TABLE_STATEMENT) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_TABLE_STATEMENT;
    default:
      break;
    }
  } else if constexpr (SP == S::OBJECT_STATEMENT) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_OBJECT_STATEMENT;
    default:
      break;
    }
  } else if constexpr (SP == S::LOCAL_STATEMENT) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_LOCAL_STATEMENT;
    default:
      break;
    }
  } else if constexpr (SP == S::LVALUE) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_LVALUE;
    default:
      break;
    }
  } else if constexpr (SP == S::RVALUE) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_RVALUE;
    default:
      break;
    }
  } else if constexpr (SP == S::REFLECTION) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_REFLECTION;
    default:
      break;
    }
  } else if constexpr (SP == S::PARAMETER) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_PARAMETER;
    default:
      break;
    }
  } else if constexpr (SP == S::ENUMERATOR) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_ENUMERATOR;
    default:
      break;
    }
  } else if constexpr (SP == S::PATH) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_PATH;
    default:
      break;
    }
  } else if constexpr (SP == S::NAME) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_NAME;
    default:
      break;
    }
  } else if constexpr (SP == S::SEQUENCE_STAGE) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_SEQUENCE_STAGE;
    default:
      break;
    }
  } else if constexpr (SP == S::ARM) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_ARM;
    default:
      break;
    }
  } else if constexpr (SP == S::DYNAMIC_CAPTURE) {
    switch (keyword) {
    case K::EXPAND:
      return K::_EXPAND_DYNAMIC_CAPTURE;
    default:
      break;
    }
  } else {
    static_assert(false, "invalid situation");
  }
  return K::__ERROR;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::__NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE constexpr bool
getCanBeRootStatement(rq::Keyword keyword) {
  return keyword == rq::Keyword::_MODULE_ROOT;
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
getCanBeEnumerator(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ENUMERATOR);
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
  } else if constexpr (SITUATION_PARAM == rq::Situation::ROOT_STATEMENT) {
    return rq::getCanBeRootStatement(keyword);
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
  } else if constexpr (SITUATION_PARAM == rq::Situation::ENUMERATOR) {
    return rq::getCanBeEnumerator(keyword);
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
  USER,
  LABEL,
  TEMPLATE,
  LIKELY,
  UNLIKELY,
  DEPRECIATED,
  EXPORT,
  PRIVATE,
  PROTECTED,
  MAY_SHALLOW_COPY
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
  case SA::MAY_SHALLOW_COPY:
    return "may_shallow_copy";
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
  case K::MAY_SHALLOW_COPY:
    return SA::MAY_SHALLOW_COPY;
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
  USER = rq::getBit(18),
  LABEL = rq::getBit(17),
  TEMPLATE = rq::getBit(16),
  LIKELY = rq::getBit(15),
  UNLIKELY = rq::getBit(14),
  DEPRECIATED = rq::getBit(13),
  EXPORT = rq::getBit(12),
  PRIVATE = rq::getBit(11),
  PROTECTED = rq::getBit(10),
  MAY_SHALLOW_COPY = rq::getBit(9)
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
  case SA::MAY_SHALLOW_COPY:
    return SF::MAY_SHALLOW_COPY;
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

enum class ExpressionFlags : std::uint8_t {
  NONE = 0,
  INSERTED = rq::getBit(0),
  COMMA_TERMINATED = rq::getBit(1)
};

template <> struct is_flags<rq::ExpressionFlags> final : std::true_type {};

struct Expression final {
  using Self = rq::Expression;

  rq::Keyword _keyword{rq::Keyword::__NONE};
#if defined(_NDEBUG)
  llvm::PointerIntPair<rq::Expression *, 2,
                       std::underlying_type_t<rq::ExpressionFlags>>
      _next_ptr{nullptr};
#else
  // set these properties up seperatly on debug builds to make it easier to
  // debug
  rq::Expression *_next_ptr{nullptr};
  bool _is_inserted{false};
  bool _is_comma_terminated{false};
#endif
  rq::Expression *_branch_ptr{nullptr};
  const char *_source_text_ptr{nullptr};
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
  template <typename rq::Situation SITUATION_PARAM>
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getUniversalized() const {
    return rq::getUniversalized<SITUATION_PARAM>(this->_keyword);
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInternal() const {
    return rq::getIsInternal(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSemicolonSeparatedBranches() const {
    return rq::getHasSemicolonSeparatedBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsNullaryWhenNoBranches() const {
    return rq::getIsNullaryWhenNoBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIfChunkNotStart() const {
    return rq::getIsIfChunkNotStart(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsIfChunkNotEnd() const {
    return rq::getIsIfChunkNotEnd(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRootStatement() const {
    return rq::getCanBeRootStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTopStatement() const {
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeEnumerator() const {
    return rq::getCanBeEnumerator(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsInserted() const {
    RQ_ASSERT(this->getHasSourceText(), "expression source was not set");
#if defined(_NDEBUG)
    return rq::getHasAll(
        static_cast<rq::ExpressionFlags>(this->_next_ptr.getInt()),
        rq::ExpressionFlags::INSERTED);
#else
    return this->_is_inserted;
#endif
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsCommaTerminated() const {
#if defined(_NDEBUG)
    return rq::getHasAll(
        static_cast<rq::ExpressionFlags>(this->_next_ptr.getInt()),
        rq::ExpressionFlags::COMMA_TERMINATED);
#else
    return this->_is_comma_terminated;
#endif
  }
  RQ_ALWAYS_INLINE void setIsCommaTerminated() {
#if defined(_NDEBUG)
    this->_next_ptr.setInt(
        static_cast<std::underlying_type_t<rq::ExpressionFlags>>(
            rq::ExpressionFlags::COMMA_TERMINATED));
#else
    this->_is_comma_terminated = true;
#endif
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasSourceText() const {
    return this->_source_text_ptr != nullptr;
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
  [[nodiscard]] RQ_ALWAYS_INLINE llvm::SMLoc getLlvmSourceBefore() const {
    return llvm::SMLoc::getFromPointer(this->_source_text_ptr - 1);
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
  RQ_ALWAYS_INLINE void setSourceTextInserted(llvm::StringRef source) {
#if defined(_NDEBUG)
    this->_next_ptr.setInt(true);
#else
    this->_is_inserted = true;
#endif
    this->setSourceText(source);
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSource(const SourceParam &source) {
    this->setSourceText(source.getSourceText());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceInserted(const SourceParam &source) {
#if defined(_NDEBUG)
    this->_next_ptr.setInt(true);
#else
    this->_is_inserted = true;
#endif
    this->setSource(source);
  }
  template <typename FirstSourceParam, typename LastSourceParam>
  inline void setSource(const FirstSourceParam &first,
                        const LastSourceParam &last) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
    rq::assignSingleValue(this->_source_text_ptr, first.getSourceTextPtr());
    this->_source_text_length = rq::getSourceLengthBetween(first, last);
  }
  template <typename FirstSourceParam, typename LastSourceParam>
  inline void setSourceInserted(const FirstSourceParam &first,
                                const LastSourceParam &last) {
#if defined(_NDEBUG)
    this->_next_ptr.setInt(true);
#else
    this->_is_inserted = true;
#endif
    this->setSource(first, last);
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
  [[nodiscard]] RQ_ALWAYS_INLINE const char *getEndSourceTextPtr() const {
    if (this->getSourceTextLength() == 0) {
      return this->getSourceTextPtr();
    }
    return this->getSourceTextPtr() + this->getSourceTextLength() - 1;
  }
  RQ_ALWAYS_INLINE void setSourceInsertedAt(const char *source_ptr) {
    RQ_ASSERT(!this->getHasSourceText(), "expression source already set");
#if defined(_NDEBUG)
    this->_next_ptr.setInt(true);
#else
    this->_is_inserted = true;
#endif
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
  template <typename SourceParam>
  inline void setSourceInsertedAtEnd(const SourceParam &source) {
    this->setSourceInsertedAt(source.getEndSourceTextPtr());
  }
  // NOTE: no getBranchCount and getNextCount because bad performance!
  RQ_ALWAYS_INLINE rq::Expression &getBranch() {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE const rq::Expression &getBranch() const {
    return rq::dereferencePtr(this->_branch_ptr);
  }
  RQ_ALWAYS_INLINE rq::Expression &getNext() {
#if defined(_NDEBUG)
    return rq::dereferencePtr(this->_next_ptr.getPointer());
#else
    return rq::dereferencePtr(this->_next_ptr);
#endif
  }
  RQ_ALWAYS_INLINE const rq::Expression &getNext() const {
#if defined(_NDEBUG)
    return rq::dereferencePtr(this->_next_ptr.getPointer());
#else
    return rq::dereferencePtr(this->_next_ptr);
#endif
  }
  RQ_ALWAYS_INLINE rq::Expression *getBranchPtr() { return this->_branch_ptr; }
  RQ_ALWAYS_INLINE const rq::Expression *getBranchPtr() const {
    return this->_branch_ptr;
  }
  RQ_ALWAYS_INLINE rq::Expression *getNextPtr() {
#if defined(_NDEBUG)
    return this->_next_ptr.getPointer();
#else
    return this->_next_ptr;
#endif
  }
  RQ_ALWAYS_INLINE const rq::Expression *getNextPtr() const {
#if defined(_NDEBUG)
    return this->_next_ptr.getPointer();
#else
    return this->_next_ptr;
#endif
  }
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
    RQ_ASSERT(!this->getHasNext(), "single value reassignment");
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(&next);
#else
    rq::assignSingleValue(this->_next_ptr, &next);
#endif
  }
  RQ_ALWAYS_INLINE void setBranch(rq::Expression *branch_ptr) {
    rq::assignSingleValue(this->_branch_ptr, branch_ptr);
  }
  RQ_ALWAYS_INLINE void setNext(rq::Expression *next_ptr) {
    RQ_ASSERT(!this->getHasNext(), "single value reassignment");
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(next_ptr);
#else
    rq::assignSingleValue(this->_next_ptr, next_ptr);
#endif
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
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(&next);
#else
    this->_next_ptr = &next;
#endif
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
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(next_ptr);
#else
    this->_next_ptr = next_ptr;
#endif
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
#if defined(_NDEBUG)
    rq::Expression *replaced_next_ptr = this->_next_ptr.getPointer();
#else
    rq::Expression *replaced_next_ptr = this->_next_ptr;
#endif
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(&next);
#else
    this->_next_ptr = &next;
#endif
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
#if defined(_NDEBUG)
    rq::Expression *replaced_next_ptr = this->_next_ptr.getPointer();
#else
    rq::Expression *replaced_next_ptr = this->_next_ptr;
#endif
#if defined(_NDEBUG)
    this->_next_ptr.setPointer(next_ptr);
#else
    this->_next_ptr = next_ptr;
#endif
    return replaced_next_ptr;
  }
  RQ_ALWAYS_INLINE bool getHasBranch() const {
    return this->_branch_ptr != nullptr;
  }
  RQ_ALWAYS_INLINE bool getHasNext() const {
#if defined(_NDEBUG)
    return this->_next_ptr.getPointer() != nullptr;
#else
    return this->_next_ptr != nullptr;
#endif
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
#if defined(_NDEBUG)
    rq::Expression *old_next_ptr = this->_next_ptr.getPointer();
    this->_next_ptr.setPointer(nullptr);
#else
    rq::Expression *old_next_ptr = this->_next_ptr;
    this->_next_ptr = nullptr;
#endif
    return *old_next_ptr;
  }
  inline rq::Expression *popNextPtr() {
#if defined(_NDEBUG)
    rq::Expression *old_next_ptr = this->_next_ptr.getPointer();
    this->_next_ptr.setPointer(nullptr);
#else
    rq::Expression *old_next_ptr = this->_next_ptr;
    this->_next_ptr = nullptr;
#endif
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
