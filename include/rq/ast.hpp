#pragma once

#include <rq/bump_ptr_allocator.hpp>
#include <rq/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>

#include <cstddef>
#include <cstdint>
#include <ranges>

namespace rq {

// Expression represents individual expressions in the AST. Each is identified
// by a keyword.

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
  // a literal that represents a single text character.
  CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  IDENTIFIER_LITERAL,

  // ERRORS
  ERROR,

  // SITUATIONAL
  UNSITUATED_PARENTHESIS_GROUP,
  UNSITUATED_EQUAL_OPERATOR,
  UNSITUATED_ASCRIBE_EXPRESSION,
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
  INITIALIZE_RECIEVER,
  BINDING,
  ASCRIBE_TYPE,
  ASCRIBE_EXPRESSION,
  ASCRIBE_ROOT_OF_VALUE,
  INSTANTIATE_EXPRESSION_ATTRIBUTE,
  INSTANTIATE_TYPE_ATTRIBUTE,
  // turn a string into an identifier
  IDENTIFY,
  IDENTIFY_OF,

  // ARITHMETIC
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  NEGATE,

  // CASTS
  AS,
  AS_OF,
  OF,
  OF_OF,
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
  ASSIGN,
  UNSAFE_CONTENT,
  UNSAFE_CONTENT_OF,
  ADDRESS,
  ADDRESS_OF,
  BORROW,
  BORROW_OF,
  DATA_ADDRESS,
  DATA_ADDRESS_OF,
  UNSAFE_AT,
  UNSAFE_AT_OF,
  MOVE,
  MOVE_OF,
  COMPOSE,
  COMPOSE_OF,
  DESTRUCTOR,
  DESTROY,
  DESTROY_OF,

  // SUBTYPE
  ARRAY,
  REFERENCE,
  POINTER,
  FAT_POINTER,

  // PARAMETER RULES
  POSITIONAL_PARAMETERS_END,
  NAMED_PARAMETERS_BEGIN,
  UNSETTABLE_PARAMETERS_BEGIN,

  // BRACES
  TUPLE,
  INSTANTIATE_LAYOUT,
  INITIALIZE_CONCATENATED_STRING,
  INSTANTIATE_TEMPLATE,

  // PROCEDURES
  CALL,
  NAMED_ARGUMENT,
  INSTANTIATE_SIGNATURE,
  DEFAULT_VALUE_PARAMETER,
  DROP,
  DROP_OF,
  DROP_EACH,
  DROP_EACH_OF,
  FORWARD_RANGER,
  BACKWARD_RANGER,
  ENTRY,
  FUNCTION,
  METHOD,
  EXTENSION_FUNCTION,
  EXTENSION_METHOD,
  IMPLEMENT_FUNCTION,
  IMPLEMENT_METHOD,
  IMPLEMENT_EXTENSION_FUNCTION,
  IMPLEMENT_EXTENSION_METHOD,
  USE_FUNCTION,
  USE_METHOD,

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
  INTERFACE,
  IMPLEMENT_INTERFACE,

  // VALUES
  INITIALIZE_ARRAY,
  NULL_,
  TRUE,
  FALSE,
  // vignette value.
  VALUE,
  // vignette index.
  INDEX,
  // vignette or reflected enumerator index.
  DISCRIMINANT,
  // value returned from a block.
  OUT,
  // reference to extended value of method or extension_method.
  THIS,
  // value returned from a function.
  RESULT,
  // retrieve command line arguments within entry.
  COMMAND_LINE_ARGUMENTS,
  // get information about location of function call
  CALLSITE,

  // BUILTIN TYPES
  INFERENCE,
  VOID,
  NO_RETURN,
  BOOLEAN,
  HALF,
  SINGLE,
  DOUBLE,
  QUADRUPLE,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  BFLOAT16,
  SIGNED_INTEGER,
  UNSIGNED_INTEGER,
  FAST_SIGNED_INTEGER,
  FAST_UNSIGNED_INTEGER,
  LEAST_SIGNED_INTEGER,
  LEAST_UNSIGNED_INTEGER,
  SIGNED_INDEX,
  UNSIGNED_INDEX,
  SIGNED_ADDRESS,
  UNSIGNED_ADDRESS,
  CHAR,
  ASCII,
  UTF8,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS,
  FIRST_VARIADIC_ARGUMENT,
  FIRST_VARIADIC_ARGUMENT_OF,
  NEXT_VARIADIC_ARGUMENT,
  NEXT_VARIADIC_ARGUMENT_OF,
  INITIALIZE_VARIADIC_ARGUMENTS,

  // CONSTRAINTS
  TYPE_CONSTRAINT,
  RANGE_CONSTRAINT,
  NUMERIC_CONSTRAINT,
  SIGNED_CONSTRAINT,
  UNSIGNED_CONSTRAINT,
  INTEGER_CONSTRAINT,
  SIGNED_INTEGER_CONSTRAINT,
  UNSIGNED_INTEGER_CONSTRAINT,
  FLOAT_CONSTRAINT,
  BINARY_CONSTRAINT,
  BFLOAT_CONSTRAINT,
  STRING_CONSTRAINT,
  CODEUNIT_CONSTRAINT,
  EXPRESSION_ATTRIBUTE_CONSTRAINT,
  TYPE_ATTRIBUTE_CONSTRAINT,

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
  SPIN,
  WEAVE,
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

  // TABLE GRAPH
  IMPORT,
  NAMESPACE,
  C,
  TOP,
  NO_NAME,

  // HINTS
  DEBUG_BREAK,
  ABORT,
  ASSERT,
  UNREACHABLE,
  ASSUME,

  // EXPRESSION ATTRIBUTES
  // labeling
  NO_LABEL, // default
  LABEL,
  // visibility
  TRANSPARENT, // default
  OPAQUE,
  // scope_location
  INSIDE_SCOPE, // default
  OUTSIDE_SCOPE,
  // availability
  LOCAL, // default
  GLOBAL,
  // accessibility
  PRIVATE, // default
  PUBLIC,
  PROTECTED,
  // property mutability
  NO_PARTIAL_MUTATE, // default
  PARTIAL_MUTATE,
  // exporting
  NO_EXPORT, // default
  EXPORT,
  // generation_time
  DYNAMIC, // default
  STATIC,
  // capturing
  NO_CAPTURE, // default
  CAPTURE,
  // evaluation_time
  LAZY, // default
  EAGER,
  // inlining
  NO_INLINE, // default
  INLINE,
  // mangling
  IMPLICIT_MANGLE, // default
  EXPLICIT_MANGLE,
  // packing
  NO_PACK, // default
  PACK,
  // templating
  NO_TEMPLATE, // default
  TEMPLATE,
  SPECIALIZE,
  // likelyhood
  EQUIVOCAL, // default
  LIKELY,
  UNLIKELY,
  // support
  SUPPORTED, // default
  DEPRECIATED,
  EXPERIMENTAL,
  // copyability
  NO_COPY, // default
  MAY_COPY,
  // address_stability
  UNSTABLE_ADDRESS, // default
  STABLE_ADDRESS,
  // cleanup
  IMPLICIT_DROP, // default
  EXPLICIT_DROP,

  // TYPE ATTRIBUTES
  // mutability
  CONSTANT, // default
  VAR,
  PARTIALLY_VAR,
  // volatility
  NO_VOLATILE, // default
  VOLATILE,
  // determinicity
  DETERMINATE, // default
  INDETERMINATE,
  // atomicity
  NO_ATOMIC, // default
  ATOMIC,
  // null_termination
  NO_NULL_TERMINATE, // default
  NULL_TERMINATE,
  // precondition
  NO_ASSERT_BEFORE,
  ASSERT_BEFORE,
  // postcondition
  NO_ASSERT_AFTER,
  ASSERT_AFTER,

  // EXPRESSION ATTRIBUTE TYPES
  LABELING,            // no_label vs label
  VISIBILITY,          // transparent vs opaque
  SCOPING,             // inside_scope vs outside_scope
  AVAILABILITY,        // local vs global
  ACCESSIBILITY,       // private vs public vs protected
  PROPERTY_MUTABILITY, // no_partial_mutate vs partial_mutate
  EXPORTING,           // no_export vs export
  GENERATION_TIME,     // dynamic vs static
  CAPTURING,           // no_capture vs capture
  EVALUATION_TIME,     // lazy vs eager
  INLINING,            // no_inline vs inline
  MANGLING,            // implicit_mangle vs explicit_mangle
  PACKING,             // no_pack vs pack
  TEMPLATING,          // no_template vs template vs specialize
  LIKELYHOOD,          // equivocal vs likely vs unlikely
  SUPPORT,             // supported vs depreciated vs experimental
  COPYABILITY,         // no_copy vs may_copy
  ADDRESS_STABILITY,   // unstable_address vs stable_address
  CLEANUP,             // explicit_drop vs implicit_drop

  // TYPE ATTRIBUTE TYPES
  MUTABILITY,       // any_mutability var vs constant vs partially_var
  VOLATILITY,       // maybe_volatile vs volatile vs no_volatile
  DETERMINICITY,    // determinate vs indeterminate
  ATOMICITY,        // no_atomic vs atomic
  NULL_TERMINATION, // no_null_terminate vs null_terminate
  PRECONDITION,     // no_assert_before vs assert_before
  POSTCONDITION,    // no_assert_after vs assert_after

  // MACROS
  QUOTE,
  EXPRESSION,
  EXPAND,
  EXPAND_STATEMENT,
  EXPAND_LVALUE,
  EXPAND_RVALUE,
  EXPAND_TUPLE,
  EXPAND_LAYOUT,
  EXPAND_SIGNATURE,
  EXPAND_REFLECTION,
  EXPAND_ARGUMENT,
  EXPAND_PARAMETER,
  EXPAND_BINDING,
  EXPAND_NAME,
  EXPAND_NAMESPACE,
  EXPAND_ASCRIPTION,
  EXPAND_EXPRESSION_ATTRIBUTE_INSTANTIATION,
  EXPAND_TYPE_ATTRIBUTE_INSTANTIATION,
  EXPAND_ARITHMETIC_SEQUENCE_STAGE,

  // REFLECTIONS
  REFLECT,
  MEMBER_OF,
  MEMBER_OF_TOP,
  IGNORE,
  IGNORE_OF,
  BYTE_SIZE,
  BYTE_SIZE_OF,
  BIT_DEPTH,
  BIT_DEPTH_OF,
  ELEMENT_COUNT,
  ELEMENT_COUNT_OF,
  SNIPPET,
  SNIPPET_OF,
  NAME,
  NAME_OF,
  LINE,
  LINE_OF,
  COLUMN,
  COLUMN_OF,
  IS,
  IS_TYPE,
  HOLDS,
  HOLDS_OF,
  TYPE,
  TYPE_OF,
  SYMBOL,
  SYMBOL_OF,
  HAS_MEMBER,
  HAS_MEMBER_OF,
  HAS_CAPTURE,
  HAS_CAPTURE_OF,
  GET_CAPTURE,
  GET_CAPTURE_OF,
  SIGNATURE,
  SIGNATURE_OF,
  // make a unique clone of a type that is not implicitly convertable
  // can use platform specific values for bit depth only if type is a synonym
  SYNONYM,
  SYNONYM_OF,
  IS_OK,
  IS_OK_OF,
  AS_EXTENSION,
  AS_EXTENSION_OF,
  REVERSE,
  REVERSE_OF,

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
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
    return "_unsituated_ascribe_expression";
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
  case K::INITIALIZE_RECIEVER:
    return "_initialize_reciever";
  case K::BINDING:
    return "_binding";
  case K::ASCRIBE_TYPE:
    return "_ascribe_type";
  case K::ASCRIBE_EXPRESSION:
    return "_ascribe_expression";
  case K::ASCRIBE_ROOT_OF_VALUE:
    return "_ascribe_root_of_value";
  case K::INSTANTIATE_EXPRESSION_ATTRIBUTE:
    return "_instantiate_expression_attribute";
  case K::INSTANTIATE_TYPE_ATTRIBUTE:
    return "_instantiate_type_attribute";
  case K::IDENTIFY:
    return "identify";
  case K::IDENTIFY_OF:
    return "_identify_of";

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
  case K::AS_OF:
    return "_as_of";
  case K::OF:
    return "of";
  case K::OF_OF:
    return "_of_of";
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
  case K::ASSIGN:
    return "_assign";
  case K::UNSAFE_CONTENT:
    return "unsafe_content";
  case K::UNSAFE_CONTENT_OF:
    return "_unsafe_content_of";
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
  case K::UNSAFE_AT:
    return "unsafe_at";
  case K::UNSAFE_AT_OF:
    return "_unsafe_at_of";
  case K::MOVE:
    return "move";
  case K::MOVE_OF:
    return "_move_of";
  case K::COMPOSE:
    return "compose";
  case K::COMPOSE_OF:
    return "_compose_of";
  case K::DESTRUCTOR:
    return "destructor";
  case K::DESTROY:
    return "destroy";
  case K::DESTROY_OF:
    return "_destroy_of";

  // SUBTYPE
  case K::ARRAY:
    return "_array";
  case K::REFERENCE:
    return "_reference";
  case K::POINTER:
    return "_pointer";
  case K::FAT_POINTER:
    return "_fat_pointer";

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case K::NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";
  case K::UNSETTABLE_PARAMETERS_BEGIN:
    return "_unsettable_parameters_begin";

  // BRACES
  case K::TUPLE:
    return "_tuple";
  case K::INSTANTIATE_LAYOUT:
    return "_instantiate_layout";
  case K::INITIALIZE_CONCATENATED_STRING:
    return "_initialize_concatenated_string";
  case K::INSTANTIATE_TEMPLATE:
    return "_instantiate_template";

  // PROCEDURES
  case K::CALL:
    return "_call";
  case K::NAMED_ARGUMENT:
    return "_named_argument";
  case K::INSTANTIATE_SIGNATURE:
    return "_instantiate_signature";
  case K::DEFAULT_VALUE_PARAMETER:
    return "_default_value_parameter";
  case K::DROP:
    return "drop";
  case K::DROP_OF:
    return "_drop_of";
  case K::DROP_EACH:
    return "drop_each";
  case K::DROP_EACH_OF:
    return "_drop_each_of";
  case K::FORWARD_RANGER:
    return "forward_ranger";
  case K::BACKWARD_RANGER:
    return "backward_ranger";
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
  case K::IMPLEMENT_FUNCTION:
    return "implement_function";
  case K::IMPLEMENT_METHOD:
    return "implement_method";
  case K::IMPLEMENT_EXTENSION_FUNCTION:
    return "implement_extension_function";
  case K::IMPLEMENT_EXTENSION_METHOD:
    return "implement_extension_method";
  case K::USE_FUNCTION:
    return "use_function";
  case K::USE_METHOD:
    return "use_method";

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
  case K::INTERFACE:
    return "interface";
  case K::IMPLEMENT_INTERFACE:
    return "implement_interface";

  // VALUES
  case K::INITIALIZE_ARRAY:
    return "initialize_array";
  case K::NULL_:
    return "null";
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
  case K::CALLSITE:
    return "callsite";

  // BUILTIN TYPES
  case K::INFERENCE:
    return "_inference";
  case K::VOID:
    return "void";
  case K::NO_RETURN:
    return "no_return";
  case K::BOOLEAN:
    return "boolean";
  case K::HALF:
    return "half";
  case K::SINGLE:
    return "single";
  case K::DOUBLE:
    return "double";
  case K::QUADRUPLE:
    return "quadruple";
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
  case K::SIGNED_INTEGER:
    return "signed_integer";
  case K::UNSIGNED_INTEGER:
    return "unsigned_integer";
  case K::FAST_SIGNED_INTEGER:
    return "fast_signed_integer";
  case K::FAST_UNSIGNED_INTEGER:
    return "fast_unsigned_integer";
  case K::LEAST_SIGNED_INTEGER:
    return "least_signed_integer";
  case K::LEAST_UNSIGNED_INTEGER:
    return "least_unsigned_integer";
  case K::SIGNED_INDEX:
    return "signed_index";
  case K::UNSIGNED_INDEX:
    return "unsigned_index";
  case K::SIGNED_ADDRESS:
    return "signed_address";
  case K::UNSIGNED_ADDRESS:
    return "unsigned_address";
  case K::CHAR:
    return "char";
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
  case K::INITIALIZE_VARIADIC_ARGUMENTS:
    return "initialize_variadic_arguments";

  // CONSTRAINTS
  case K::TYPE_CONSTRAINT:
    return "type_constraint";
  case K::NUMERIC_CONSTRAINT:
    return "numeric_constraint";
  case K::SIGNED_CONSTRAINT:
    return "signed_constraint";
  case K::UNSIGNED_CONSTRAINT:
    return "unsigned_constraint";
  case K::INTEGER_CONSTRAINT:
    return "integer_constraint";
  case K::SIGNED_INTEGER_CONSTRAINT:
    return "signed_integer_constraint";
  case K::UNSIGNED_INTEGER_CONSTRAINT:
    return "unsigned_integer_constraint";
  case K::FLOAT_CONSTRAINT:
    return "float_constraint";
  case K::BINARY_CONSTRAINT:
    return "binary_constraint";
  case K::BFLOAT_CONSTRAINT:
    return "bfloat_constraint";
  case K::STRING_CONSTRAINT:
    return "string_constraint";
  case K::CODEUNIT_CONSTRAINT:
    return "codeunit_constraint";
  case K::EXPRESSION_ATTRIBUTE_CONSTRAINT:
    return "expression_attribute_constraint";
  case K::TYPE_ATTRIBUTE_CONSTRAINT:
    return "type_attribute_constraint";

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
  case K::SPIN:
    return "spin";
  case K::WEAVE:
    return "weave";
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

  // TABLE GRAPH
  case K::IMPORT:
    return "import";
  case K::NAMESPACE:
    return "namespace";
  case K::C:
    return "c";
  case K::TOP:
    return "_top";
  case K::NO_NAME:
    return "no_name";

  // HINTS
  case K::DEBUG_BREAK:
    return "debug_break";
  case K::ABORT:
    return "abort";
  case K::ASSERT:
    return "assert";
  case K::UNREACHABLE:
    return "unreachable";
  case K::ASSUME:
    return "assume";

  // EXPRESSION ATTRIBUTES
  case K::NO_LABEL:
    return "no_label";
  case K::OPAQUE:
    return "opaque";
  case K::TRANSPARENT:
    return "transparent";
  case K::OUTSIDE_SCOPE:
    return "outside_scope";
  case K::INSIDE_SCOPE:
    return "inside_scope";
  case K::LOCAL:
    return "local";
  case K::GLOBAL:
    return "global";
  case K::PUBLIC:
    return "public";
  case K::PRIVATE:
    return "private";
  case K::PROTECTED:
    return "protected";
  case K::NO_PARTIAL_MUTATE:
    return "no_partial_mutate";
  case K::PARTIAL_MUTATE:
    return "partial_mutate";
  case K::EXPORT:
    return "export";
  case K::NO_EXPORT:
    return "no_export";
  case K::STATIC:
    return "static";
  case K::DYNAMIC:
    return "dynamic";
  case K::CAPTURE:
    return "capture";
  case K::NO_CAPTURE:
    return "no_capture";
  case K::EAGER:
    return "eager";
  case K::LAZY:
    return "lazy";
  case K::INLINE:
    return "inline";
  case K::NO_INLINE:
    return "no_inline";
  case K::EXPLICIT_MANGLE:
    return "explicit_mangle";
  case K::IMPLICIT_MANGLE:
    return "implicit_mangle";
  case K::PACK:
    return "pack";
  case K::NO_PACK:
    return "no_pack";
  case K::LABEL:
    return "label";
  case K::TEMPLATE:
    return "template";
  case K::NO_TEMPLATE:
    return "no_template";
  case K::SPECIALIZE:
    return "specialize";
  case K::LIKELY:
    return "likely";
  case K::UNLIKELY:
    return "unlikely";
  case K::EQUIVOCAL:
    return "equivocal";
  case K::SUPPORTED:
    return "supported";
  case K::DEPRECIATED:
    return "depreciated";
  case K::EXPERIMENTAL:
    return "experimental";
  case K::MAY_COPY:
    return "may_copy";
  case K::NO_COPY:
    return "no_copy";
  case K::STABLE_ADDRESS:
    return "stable_address";
  case K::UNSTABLE_ADDRESS:
    return "unstable_address";
  case K::IMPLICIT_DROP:
    return "implicit_drop";
  case K::EXPLICIT_DROP:
    return "explicit_drop";

  // TYPE ATTRIBUTES
  case K::CONSTANT:
    return "constant";
  case K::VAR:
    return "var";
  case K::PARTIALLY_VAR:
    return "partially_var";
  case K::NO_VOLATILE:
    return "no_volatile";
  case K::VOLATILE:
    return "volatile";
  case K::DETERMINATE:
    return "determinate";
  case K::INDETERMINATE:
    return "indeterminate";
  case K::NO_ATOMIC:
    return "no_atomic";
  case K::ATOMIC:
    return "atomic";
  case K::NO_NULL_TERMINATE:
    return "no_null_terminate";
  case K::NULL_TERMINATE:
    return "null_terminate";
  case K::NO_ASSERT_BEFORE:
    return "no_assert_before";
  case K::ASSERT_BEFORE:
    return "assert_before";
  case K::NO_ASSERT_AFTER:
    return "no_assert_after";
  case K::ASSERT_AFTER:
    return "assert_after";

  // EXPRESSION ATTRIBUTE TYPES
  case K::LABELING:
    return "labeling";
  case K::VISIBILITY:
    return "visibility";
  case K::SCOPING:
    return "scoping";
  case K::AVAILABILITY:
    return "availability";
  case K::ACCESSIBILITY:
    return "accessibility";
  case K::PROPERTY_MUTABILITY:
    return "property_mutability";
  case K::EXPORTING:
    return "exporting";
  case K::GENERATION_TIME:
    return "generation_time";
  case K::CAPTURING:
    return "capturing";
  case K::EVALUATION_TIME:
    return "evaluation_time";
  case K::INLINING:
    return "inlining";
  case K::MANGLING:
    return "mangling";
  case K::PACKING:
    return "packing";
  case K::TEMPLATING:
    return "templating";
  case K::LIKELYHOOD:
    return "likelyhood";
  case K::SUPPORT:
    return "support";
  case K::COPYABILITY:
    return "copyability";
  case K::ADDRESS_STABILITY:
    return "address_stability";
  case K::CLEANUP:
    return "cleanup";

  // TYPE ATTRIBUTE TYPES
  case K::MUTABILITY:
    return "mutability";
  case K::VOLATILITY:
    return "volatility";
  case K::DETERMINICITY:
    return "determinicity";
  case K::ATOMICITY:
    return "atomicity";
  case K::NULL_TERMINATION:
    return "null_termination";
  case K::PRECONDITION:
    return "precondition";
  case K::POSTCONDITION:
    return "postcondition";

  // EXPRESSIONS
  case K::QUOTE:
    return "quote";
  case K::EXPRESSION:
    return "expression";
  case K::EXPAND:
    return "expand";
  case K::EXPAND_STATEMENT:
    return "_expand_statement";
  case K::EXPAND_LVALUE:
    return "_expand_lvalue";
  case K::EXPAND_RVALUE:
    return "_expand_rvalue";
  case K::EXPAND_TUPLE:
    return "_expand_tuple";
  case K::EXPAND_LAYOUT:
    return "_expand_layout";
  case K::EXPAND_SIGNATURE:
    return "_expand_signature";
  case K::EXPAND_REFLECTION:
    return "_expand_reflection";
  case K::EXPAND_ARGUMENT:
    return "_expand_argument";
  case K::EXPAND_PARAMETER:
    return "_expand_parameter";
  case K::EXPAND_BINDING:
    return "_expand_binding";
  case K::EXPAND_NAME:
    return "_expand_name";
  case K::EXPAND_NAMESPACE:
    return "_expand_namespace";
  case K::EXPAND_ASCRIPTION:
    return "_expand_ascription";
  case K::EXPAND_EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return "_expand_expression_attribute_instantiation";
  case K::EXPAND_TYPE_ATTRIBUTE_INSTANTIATION:
    return "_expand_type_attribute_instantiation";
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return "_expand_arithmetic_sequence_stage";

  // REFLECTIONS
  case K::REFLECT:
    return "_reflect";
  case K::MEMBER_OF:
    return "_member_of";
  case K::MEMBER_OF_TOP:
    return "_member_of_top";
  case K::IGNORE:
    return "ignore";
  case K::IGNORE_OF:
    return "_ignore_of";
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
  case K::SNIPPET:
    return "snippet";
  case K::SNIPPET_OF:
    return "_snippet_of";
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
  case K::HOLDS_OF:
    return "_holds_of";
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
  case K::HAS_CAPTURE:
    return "has_capture";
  case K::HAS_CAPTURE_OF:
    return "_has_capture_of";
  case K::GET_CAPTURE:
    return "get_capture";
  case K::GET_CAPTURE_OF:
    return "_get_capture_of";
  case K::SIGNATURE:
    return "signature";
  case K::SIGNATURE_OF:
    return "_signature_of";
  case K::SYNONYM:
    return "synonym";
  case K::SYNONYM_OF:
    return "_synonym_of";
  case K::IS_OK:
    return "is_ok";
  case K::IS_OK_OF:
    return "_is_ok_of";
  case K::AS_EXTENSION:
    return "_as_extension";
  case K::AS_EXTENSION_OF:
    return "_as_extension_of";
  case K::REVERSE:
    return "reverse";
  case K::REVERSE_OF:
    return "_reverse_of";

  case K::LAST:
    break;

    // NOTE: don't add default case so compiler will warn if cases are missing!
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
  EXPANSION = rq::getBit(10),
  // TOP
  STATEMENT = rq::getBit(11),
  RVALUE = rq::getBit(12),
  LVALUE = rq::getBit(13),
  TUPLE = rq::getBit(14),
  LAYOUT = rq::getBit(15),
  SIGNATURE = rq::getBit(16),
  REFLECTION = rq::getBit(17),
  ARGUMENT = rq::getBit(18),
  PARAMETER = rq::getBit(19),
  BINDING = rq::getBit(20),
  NAME = rq::getBit(21),
  NAMESPACE = rq::getBit(22),
  ASCRIPTION = rq::getBit(23),
  EXPRESSION_ATTRIBUTE = rq::getBit(24),
  TYPE_ATTRIBUTE = rq::getBit(25),
  ARITHMETIC_SEQUENCE_STEP = rq::getBit(26),
  ARITHMETIC_SEQUENCE_CONDITION = rq::getBit(27),
  ALL_SITUATIONS = STATEMENT | RVALUE | LVALUE | TUPLE | LAYOUT | SIGNATURE |
                   REFLECTION | ARGUMENT | PARAMETER | BINDING | NAME |
                   NAMESPACE | ASCRIPTION | TYPE_ATTRIBUTE |
                   EXPRESSION_ATTRIBUTE | ARITHMETIC_SEQUENCE_STEP |
                   ARITHMETIC_SEQUENCE_CONDITION,

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
  case K::CODEUNIT_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::ARGUMENT;
  case K::IDENTIFIER_LITERAL:
    return KF::LITERAL | KF::INTERNAL | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER | KF::NAME |
           KF::NAMESPACE;

  // ERRORS
  case K::ERROR:
    return KF::INTERNAL;

  // SITUATIONAL
  case K::UNSITUATED_PARENTHESIS_GROUP:
    return KF::CONVERGING | KF::RVALUE | KF::ARGUMENT | KF::LVALUE | KF::NAME |
           KF::NAMESPACE | KF::ARITHMETIC_SEQUENCE_STEP |
           KF::ARITHMETIC_SEQUENCE_CONDITION;
  case K::UNSITUATED_EQUAL_OPERATOR:
    return KF::STATEMENT | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSITUATED_ASCRIBE_TYPE:
    return KF::SIGNATURE | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::REFLECTION | KF::ASCRIPTION;
  case K::UNSITUATED_ASCRIBE_EXPRESSION:
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
  case K::INITIALIZE_RECIEVER:
    return KF::SIGNATURE | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINDING:
    return KF::LVALUE | KF::PARAMETER | KF::ARGUMENT | KF::BINDING;
  case K::ASCRIBE_TYPE:
    return KF::SIGNATURE | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER |
           KF::REFLECTION | KF::ASCRIPTION;
  case K::ASCRIBE_EXPRESSION:
    return KF::STATEMENT | KF::PARAMETER | KF::ARGUMENT | KF::ASCRIPTION;
  case K::ASCRIBE_ROOT_OF_VALUE:
    return KF::RVALUE | KF::ARGUMENT | KF::ASCRIPTION;
  case K::INSTANTIATE_EXPRESSION_ATTRIBUTE:
    return KF::NONE; // EXPRESSION_ATTRIBUTE_INSTANTIATION
  case K::INSTANTIATE_TYPE_ATTRIBUTE:
    return KF::NONE; // TYPE_ATTRIBUTE_INSTANTIATION
  case K::IDENTIFY:
    return KF::REFLECTION | KF::ASCRIPTION;
  case K::IDENTIFY_OF:
    return KF::NAME | KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::NAMESPACE;

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
  case K::AS_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::OF:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::OF_OF:
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
  case K::ASSIGN:
    return KF::STATEMENT;
  case K::UNSAFE_CONTENT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::UNSAFE_CONTENT_OF:
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
  case K::UNSAFE_AT:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::UNSAFE_AT_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::MOVE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::MOVE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::COMPOSE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::COMPOSE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::DESTRUCTOR:
    return KF::STATEMENT | KF::STATEMENT_BRANCHES;
  case K::DESTROY:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DESTROY_OF:
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

  // PARAMETER RULES
  case K::POSITIONAL_PARAMETERS_END:
    return KF::PARAMETER;
  case K::NAMED_PARAMETERS_BEGIN:
    return KF::PARAMETER;
  case K::UNSETTABLE_PARAMETERS_BEGIN:
    return KF::PARAMETER;

  // BRACES
  case K::TUPLE:
    return KF::TUPLE;
  case K::INSTANTIATE_LAYOUT:
    return KF::LAYOUT;
  case K::INITIALIZE_CONCATENATED_STRING:
    return KF::RVALUE | KF::ARGUMENT;
  case K::INSTANTIATE_TEMPLATE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // PROCEDURES
  case K::CALL:
    return KF::STATEMENT | KF::RVALUE | KF::LVALUE | KF::ARGUMENT;
  case K::NAMED_ARGUMENT:
    return KF::ARGUMENT;
  case K::INSTANTIATE_SIGNATURE:
    return KF::SIGNATURE | KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DEFAULT_VALUE_PARAMETER:
    return KF::PARAMETER;
  case K::DROP:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DROP_OF:
    return KF::STATEMENT;
  case K::DROP_EACH:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::DROP_EACH_OF:
    return KF::RVALUE;
  case K::FORWARD_RANGER:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::BACKWARD_RANGER:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::ENTRY:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::EXTENSION_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::IMPLEMENT_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::IMPLEMENT_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::IMPLEMENT_EXTENSION_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::IMPLEMENT_EXTENSION_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::USE_FUNCTION:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::USE_METHOD:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;

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
  case K::INTERFACE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::IMPLEMENT_INTERFACE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;

  // VALUES
  case K::INITIALIZE_ARRAY:
    return KF::RVALUE | KF::ARGUMENT;
  case K::NULL_:
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
  case K::CALLSITE:
    return KF::RVALUE;

  // BUILTIN TYPES
  case K::INFERENCE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::VOID:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NO_RETURN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BOOLEAN:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::HALF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SINGLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DOUBLE:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::QUADRUPLE:
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
  case K::SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FAST_SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FAST_UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LEAST_SIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LEAST_UNSIGNED_INTEGER:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INDEX:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_ADDRESS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CHAR:
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
  case K::INITIALIZE_VARIADIC_ARGUMENTS:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // CONSTRAINTS
  case K::TYPE_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NUMERIC_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INTEGER_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SIGNED_INTEGER_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::UNSIGNED_INTEGER_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::FLOAT_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BINARY_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::BFLOAT_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::STRING_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CODEUNIT_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EXPRESSION_ATTRIBUTE_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::TYPE_ATTRIBUTE_CONSTRAINT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // SCOPES
  case K::IF:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK;
  case K::ELSE_IF:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::CONTINUING_CHAINLINK |
           KF::FINISHING_CHAINLINK;
  case K::ELSE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::FINISHING_CHAINLINK;
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
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK;
  case K::WITH:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK;
  case K::DEFAULT:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK |
           KF::CONTINUING_CHAINLINK | KF::FINISHING_CHAINLINK;
  case K::FOR:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::WHILE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT;
  case K::SPIN:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::STARTING_CHAINLINK;
  case K::WEAVE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::CONTINUING_CHAINLINK |
           KF::FINISHING_CHAINLINK;
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

  // TABLE GRAPH
  case K::IMPORT:
    return KF::STATEMENT;
  case K::NAMESPACE:
    return KF::STATEMENT_BRANCHES | KF::STATEMENT | KF::RVALUE;
  case K::C:
    return KF::RVALUE;
  case K::TOP:
    return KF::STATEMENT_BRANCHES | KF::NONE; // TOP
  case K::NO_NAME:
    return KF::NAME;

  // HINTS
  case K::DEBUG_BREAK:
    return KF::STATEMENT;
  case K::ABORT:
    return KF::STATEMENT;
  case K::ASSERT:
    return KF::STATEMENT;
  case K::UNREACHABLE:
    return KF::STATEMENT;
  case K::ASSUME:
    return KF::STATEMENT;

  // EXPRESSION ATTRIBUTES
  case K::NO_LABEL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::OPAQUE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::TRANSPARENT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::OUTSIDE_SCOPE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::INSIDE_SCOPE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LOCAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::GLOBAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PUBLIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PRIVATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PROTECTED:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_PARTIAL_MUTATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PARTIAL_MUTATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPORT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_EXPORT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::STATIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::DYNAMIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::CAPTURE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_CAPTURE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EAGER:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LAZY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::INLINE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_INLINE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPLICIT_MANGLE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::IMPLICIT_MANGLE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PACK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_PACK:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LABEL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::TEMPLATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_TEMPLATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::SPECIALIZE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::LIKELY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::UNLIKELY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EQUIVOCAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::SUPPORTED:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::DEPRECIATED:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPERIMENTAL:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::MAY_COPY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_COPY:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::STABLE_ADDRESS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::UNSTABLE_ADDRESS:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::IMPLICIT_DROP:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::EXPLICIT_DROP:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;

  // TYPE ATTRIBUTES
  case K::CONSTANT:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::VAR:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::PARTIALLY_VAR:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_VOLATILE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::VOLATILE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::DETERMINATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::INDETERMINATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ATOMIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ATOMIC:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NULL_TERMINATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_NULL_TERMINATE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ASSERT_BEFORE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ASSERT_BEFORE:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::NO_ASSERT_AFTER:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;
  case K::ASSERT_AFTER:
    return KF::EXPRESSION_ATTRIBUTE | KF::RVALUE | KF::ARGUMENT;

  // EXPRESSION ATTRIBUTE TYPES
  case K::LABELING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::VISIBILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SCOPING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::AVAILABILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ACCESSIBILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PROPERTY_MUTABILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EXPORTING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::GENERATION_TIME:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CAPTURING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EVALUATION_TIME:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::INLINING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::MANGLING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PACKING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::TEMPLATING:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::LIKELYHOOD:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::SUPPORT:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::COPYABILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ADDRESS_STABILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::CLEANUP:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // TYPE ATTRIBUTE TYPES
  case K::MUTABILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::VOLATILITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::DETERMINICITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::ATOMICITY:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::NULL_TERMINATION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::PRECONDITION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::POSTCONDITION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;

  // EXPRESSIONS
  case K::QUOTE:
    return KF::RVALUE | KF::ARGUMENT;
  case K::EXPRESSION:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::EXPAND:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::EXPAND_STATEMENT:
    return KF::STATEMENT | KF::EXPANSION;
  case K::EXPAND_LVALUE:
    return KF::LVALUE | KF::EXPANSION;
  case K::EXPAND_RVALUE:
    return KF::RVALUE | KF::EXPANSION;
  case K::EXPAND_TUPLE:
    return KF::TUPLE | KF::EXPANSION;
  case K::EXPAND_LAYOUT:
    return KF::LAYOUT | KF::EXPANSION;
  case K::EXPAND_SIGNATURE:
    return KF::SIGNATURE | KF::EXPANSION;
  case K::EXPAND_REFLECTION:
    return KF::REFLECTION | KF::EXPANSION;
  case K::EXPAND_ARGUMENT:
    return KF::ARGUMENT | KF::EXPANSION;
  case K::EXPAND_PARAMETER:
    return KF::PARAMETER | KF::EXPANSION;
  case K::EXPAND_BINDING:
    return KF::BINDING | KF::EXPANSION;
  case K::EXPAND_NAME:
    return KF::NAME | KF::EXPANSION;
  case K::EXPAND_NAMESPACE:
    return KF::NAMESPACE | KF::EXPANSION;
  case K::EXPAND_ASCRIPTION:
    return KF::ASCRIPTION | KF::EXPANSION;
  case K::EXPAND_EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return KF::EXPANSION;
  case K::EXPAND_TYPE_ATTRIBUTE_INSTANTIATION:
    return KF::EXPANSION;
  case K::EXPAND_ARITHMETIC_SEQUENCE_STAGE:
    return KF::ARITHMETIC_SEQUENCE_STEP | KF::ARITHMETIC_SEQUENCE_CONDITION |
           KF::EXPANSION;

  // REFLECTIONS
  case K::REFLECT:
    return KF::CONVERGING | KF::STATEMENT | KF::RVALUE | KF::LVALUE |
           KF::REFLECTION | KF::ARGUMENT | KF::PARAMETER |
           KF::ARITHMETIC_SEQUENCE_STEP | KF::ARITHMETIC_SEQUENCE_CONDITION |
           KF::NAMESPACE;
  case K::MEMBER_OF:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::MEMBER_OF_TOP:
    return KF::RVALUE | KF::LVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::IGNORE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IGNORE_OF:
    return KF::STATEMENT;
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
  case K::SNIPPET:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SNIPPET_OF:
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
  case K::HOLDS_OF:
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
  case K::HAS_CAPTURE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::HAS_CAPTURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::GET_CAPTURE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::GET_CAPTURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SIGNATURE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SIGNATURE_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::SYNONYM:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::SYNONYM_OF:
    return KF::RVALUE | KF::ARGUMENT | KF::PARAMETER;
  case K::IS_OK:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::IS_OK_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::AS_EXTENSION:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::AS_EXTENSION_OF:
    return KF::RVALUE | KF::ARGUMENT;
  case K::REVERSE:
    return KF::REFLECTION | KF::UNIVERSALIZABLE;
  case K::REVERSE_OF:
    return KF::RVALUE;

  case K::LAST:
    break;

    // NOTE: don't add default case so compiler will warn if cases are missing!
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsParameterMarkKeyword(rq::Keyword keyword) {
  return keyword == rq::Keyword::NAMED_PARAMETERS_BEGIN ||
         keyword == rq::Keyword::POSITIONAL_PARAMETERS_END ||
         keyword == rq::Keyword::UNSETTABLE_PARAMETERS_BEGIN;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsLiteralKeyword(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LITERAL);
}

[[nodiscard]] inline rq::Keyword
getSituatedAscribeKeyword(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_EXPRESSION:
    return rq::Keyword::ASCRIBE_EXPRESSION;
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasValueBranches(rq::Keyword keyword) {
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpansion(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::EXPANSION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TYPE_ATTRIBUTE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getIsExpressionAttribute(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::EXPRESSION_ATTRIBUTE);
}

enum class Situation : std::uint_fast8_t {
  NONE,
  TOP,
  STATEMENT,
  LVALUE,
  RVALUE,
  TUPLE,
  LAYOUT,
  SIGNATURE,
  REFLECTION,
  ARGUMENT,
  PARAMETER,
  BINDING,
  NAME,
  NAMESPACE,
  ASCRIPTION,
  EXPRESSION_ATTRIBUTE_INSTANTIATION,
  TYPE_ATTRIBUTE_INSTANTIATION,
  ARITHMETIC_SEQUENCE_STAGE
};

[[nodiscard]] RQ_ALWAYS_INLINE llvm::StringRef
getDescription(rq::Situation situation) {
  using namespace rq;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    return "no expression";
  case S::TOP:
    return "top expression";
  case S::STATEMENT:
    return "statement";
  case S::LVALUE:
    return "lvalue expression";
  case S::RVALUE:
    return "rvalue expression";
  case S::TUPLE:
    return "tuplo";
  case S::LAYOUT:
    return "layout";
  case S::SIGNATURE:
    return "signature";
  case S::REFLECTION:
    return "reflection expression";
  case S::ARGUMENT:
    return "argument expression";
  case S::PARAMETER:
    return "parameter expression";
  case S::BINDING:
    return "binding expression";
  case S::NAME:
    return "name expression";
  case S::NAMESPACE:
    return "namespace expression";
  case S::ASCRIPTION:
    return "ascription expression";
  case S::EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return "expression attribute instantiation";
  case S::TYPE_ATTRIBUTE_INSTANTIATION:
    return "type attribute instantiation";
  case S::ARITHMETIC_SEQUENCE_STAGE:
    return "sequence stage expression";
  }
  return "error expression";
}

[[nodiscard]] inline rq::Keyword getExpandOfSituation(rq::Situation situation) {
  using namespace rq;
  using K = Keyword;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    break;
  case S::TOP:
    break;
  case S::STATEMENT:
    return K::EXPAND_STATEMENT;
  case S::LVALUE:
    return K::EXPAND_LVALUE;
  case S::RVALUE:
    return K::EXPAND_RVALUE;
  case S::TUPLE:
    return K::EXPAND_TUPLE;
  case S::LAYOUT:
    return K::EXPAND_LAYOUT;
  case S::SIGNATURE:
    return K::EXPAND_SIGNATURE;
  case S::REFLECTION:
    return K::EXPAND_REFLECTION;
  case S::ARGUMENT:
    return K::EXPAND_ARGUMENT;
  case S::PARAMETER:
    return K::EXPAND_PARAMETER;
  case S::BINDING:
    return K::EXPAND_BINDING;
  case S::NAME:
    return K::EXPAND_NAME;
  case S::NAMESPACE:
    return K::EXPAND_NAME;
  case S::ASCRIPTION:
    return K::EXPAND_ASCRIPTION;
  case S::EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return K::EXPAND_EXPRESSION_ATTRIBUTE_INSTANTIATION;
  case S::TYPE_ATTRIBUTE_INSTANTIATION:
    return K::EXPAND_TYPE_ATTRIBUTE_INSTANTIATION;
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
  case K::EXPAND_STATEMENT:
    return S::STATEMENT;
  case K::EXPAND_LVALUE:
    return S::LVALUE;
  case K::EXPAND_RVALUE:
    return S::RVALUE;
  case K::EXPAND_TUPLE:
    return S::TUPLE;
  case K::EXPAND_LAYOUT:
    return S::LAYOUT;
  case K::EXPAND_SIGNATURE:
    return S::SIGNATURE;
  case K::EXPAND_REFLECTION:
    return S::REFLECTION;
  case K::EXPAND_ARGUMENT:
    return S::ARGUMENT;
  case K::EXPAND_PARAMETER:
    return S::PARAMETER;
  case K::EXPAND_BINDING:
    return S::BINDING;
  case K::EXPAND_NAME:
    return S::NAME;
  case K::EXPAND_NAMESPACE:
    return S::NAMESPACE;
  case K::EXPAND_ASCRIPTION:
    return S::ASCRIPTION;
  case K::EXPAND_EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return S::EXPRESSION_ATTRIBUTE_INSTANTIATION;
  case K::EXPAND_TYPE_ATTRIBUTE_INSTANTIATION:
    return S::TYPE_ATTRIBUTE_INSTANTIATION;
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
  // APPLY
  case K::IDENTIFY:
    return K::IDENTIFY_OF;
  // CASTS
  case K::AS:
    return K::AS_OF;
  case K::OF:
    return K::OF_OF;
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
  case K::UNSAFE_CONTENT:
    return K::UNSAFE_CONTENT_OF;
  case K::ADDRESS:
    return K::ADDRESS_OF;
  case K::BORROW:
    return K::BORROW_OF;
  case K::DATA_ADDRESS:
    return K::DATA_ADDRESS_OF;
  case K::UNSAFE_AT:
    return K::UNSAFE_AT_OF;
  case K::MOVE:
    return K::MOVE_OF;
  case K::COMPOSE:
    return K::COMPOSE_OF;
  case K::DESTROY:
    return K::DESTROY_OF;
  // PROCEDURES
  case K::DROP:
    return K::DROP_OF;
  case K::DROP_EACH:
    return K::DROP_EACH_OF;
  // VARIADIC ARGUMENTS
  case K::FIRST_VARIADIC_ARGUMENT:
    return K::FIRST_VARIADIC_ARGUMENT_OF;
  case K::NEXT_VARIADIC_ARGUMENT:
    return K::NEXT_VARIADIC_ARGUMENT_OF;
  // EXPANSIONS
  case K::EXPAND:
    return rq::getExpandOfSituation(situation);
  // REFLECTIONS
  case K::IGNORE:
    return K::IGNORE_OF;
  case K::BYTE_SIZE:
    return K::BYTE_SIZE_OF;
  case K::BIT_DEPTH:
    return K::BIT_DEPTH_OF;
  case K::ELEMENT_COUNT:
    return K::ELEMENT_COUNT_OF;
  case K::SNIPPET:
    return K::SNIPPET_OF;
  case K::NAME:
    return K::NAME_OF;
  case K::LINE:
    return K::LINE_OF;
  case K::COLUMN:
    return K::COLUMN_OF;
  case K::IS:
    return K::IS_TYPE;
  case K::HOLDS:
    return K::HOLDS_OF;
  case K::TYPE:
    return K::TYPE_OF;
  case K::SYMBOL:
    return K::SYMBOL_OF;
  case K::HAS_MEMBER:
    return K::HAS_MEMBER_OF;
  case K::HAS_CAPTURE:
    return K::HAS_CAPTURE_OF;
  case K::GET_CAPTURE:
    return K::GET_CAPTURE_OF;
  case K::SIGNATURE:
    return K::SIGNATURE_OF;
  case K::SYNONYM:
    return K::SYNONYM_OF;
  case K::IS_OK:
    return K::IS_OK_OF;
  case K::AS_EXTENSION:
    return K::AS_EXTENSION_OF;
  case K::REVERSE:
    return K::REVERSE_OF;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::Situation
getAttributeInstantiationSituation(rq::Keyword keyword) {
  switch (keyword) {
  case rq::Keyword::UNSITUATED_ASCRIBE_EXPRESSION:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_EXPRESSION:
    return rq::Situation::EXPRESSION_ATTRIBUTE_INSTANTIATION;
  case rq::Keyword::UNSITUATED_ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_TYPE:
    [[fallthrough]];
  case rq::Keyword::ASCRIBE_ROOT_OF_VALUE:
    return rq::Situation::TYPE_ATTRIBUTE_INSTANTIATION;
  default:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsNone(rq::Keyword keyword) {
  return keyword == rq::Keyword::NONE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTop(rq::Keyword keyword) {
  return keyword == rq::Keyword::TOP;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::STATEMENT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLvalue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRvalue(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::RVALUE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTuple(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::TUPLE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLayout(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::LAYOUT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSignature(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::SIGNATURE);
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

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NAME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeNamespace(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::NAMESPACE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ASCRIPTION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeExpressionAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_EXPRESSION_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeTypeAttributeInstantiation(rq::Keyword keyword) {
  return keyword == rq::Keyword::INSTANTIATE_TYPE_ATTRIBUTE;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceStage(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasSome(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION |
                                   rq::KeywordFlags::ARITHMETIC_SEQUENCE_STEP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getCanBeArithmeticSequenceCondition(rq::Keyword keyword) {
  const rq::KeywordFlags flags = rq::getFlags(keyword);
  return rq::getHasAll(flags, rq::KeywordFlags::ARITHMETIC_SEQUENCE_CONDITION);
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
  case rq::Situation::TOP:
    return rq::getCanBeTop(keyword);
  case rq::Situation::STATEMENT:
    return rq::getCanBeStatement(keyword);
  case rq::Situation::LVALUE:
    return rq::getCanBeLvalue(keyword);
  case rq::Situation::RVALUE:
    return rq::getCanBeRvalue(keyword);
  case rq::Situation::TUPLE:
    return rq::getCanBeTuple(keyword);
  case rq::Situation::LAYOUT:
    return rq::getCanBeLayout(keyword);
  case rq::Situation::SIGNATURE:
    return rq::getCanBeSignature(keyword);
  case rq::Situation::REFLECTION:
    return rq::getCanBeReflection(keyword);
  case rq::Situation::ARGUMENT:
    return rq::getCanBeArgument(keyword);
  case rq::Situation::PARAMETER:
    return rq::getCanBeParameter(keyword);
  case rq::Situation::BINDING:
    return rq::getCanBeBinding(keyword);
  case rq::Situation::NAME:
    return rq::getCanBeName(keyword);
  case rq::Situation::NAMESPACE:
    return rq::getCanBeNamespace(keyword);
  case rq::Situation::ASCRIPTION:
    return rq::getCanBeAscription(keyword);
  case rq::Situation::TYPE_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeTypeAttributeInstantiation(keyword);
  case rq::Situation::EXPRESSION_ATTRIBUTE_INSTANTIATION:
    return rq::getCanBeExpressionAttributeInstantiation(keyword);
  case rq::Situation::ARITHMETIC_SEQUENCE_STAGE:
    return rq::getCanBeArithmeticSequenceStage(keyword);
  }
  return false;
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsEvaluatableName(rq::Keyword keyword) {
  return keyword == rq::Keyword::IDENTIFY_OF ||
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

enum class ExpressionAttribute : std::uint_fast8_t {
  NONE,
  TRANSPARENT,
  OPAQUE,
  INSIDE_SCOPE,
  OUTSIDE_SCOPE,
  LOCAL,
  GLOBAL,
  PRIVATE,
  PUBLIC,
  PROTECTED,
  NO_PARTIAL_MUTATE,
  PARTIAL_MUTATE,
  NO_EXPORT,
  EXPORT,
  DYNAMIC,
  STATIC,
  NO_CAPTURE,
  CAPTURE,
  LAZY,
  EAGER,
  NO_INLINE,
  INLINE,
  IMPLICIT_MANGLE,
  EXPLICIT_MANGLE,
  NO_PACK,
  PACK,
  LABEL,
  NO_TEMPLATE,
  TEMPLATE,
  SPECIALIZE,
  EQUIVOCAL,
  LIKELY,
  UNLIKELY,
  SUPPORTED,
  DEPRECIATED,
  EXPERIMENTAL,
  NO_COPY,
  MAY_COPY,
  UNSTABLE_ADDRESS,
  STABLE_ADDRESS,
  EXPLICIT_DROP,
  IMPLICIT_DROP,
  LAST
};

[[nodiscard]] inline llvm::StringRef
getName(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using EA = ExpressionAttribute;
  switch (attribute) {
  case EA::NONE:
    return "none";
  case EA::TRANSPARENT:
    return "transparent";
  case EA::OPAQUE:
    return "opaque";
  case EA::INSIDE_SCOPE:
    return "inside_scope";
  case EA::OUTSIDE_SCOPE:
    return "outside_scope";
  case EA::GLOBAL:
    return "global";
  case EA::LOCAL:
    return "local";
  case EA::PRIVATE:
    return "private";
  case EA::PUBLIC:
    return "public";
  case EA::PROTECTED:
    return "protected";
  case EA::NO_PARTIAL_MUTATE:
    return "no_partial_mutate";
  case EA::PARTIAL_MUTATE:
    return "partial_mutate";
  case EA::NO_EXPORT:
    return "no_export";
  case EA::EXPORT:
    return "export";
  case EA::DYNAMIC:
    return "dynamic";
  case EA::STATIC:
    return "static";
  case EA::NO_CAPTURE:
    return "no_capture";
  case EA::CAPTURE:
    return "capture";
  case EA::LAZY:
    return "lazy";
  case EA::EAGER:
    return "eager";
  case EA::NO_INLINE:
    return "no_inline";
  case EA::INLINE:
    return "inline";
  case EA::IMPLICIT_MANGLE:
    return "implicit_mangle";
  case EA::EXPLICIT_MANGLE:
    return "explicit_mangle";
  case EA::NO_PACK:
    return "no_pack";
  case EA::PACK:
    return "pack";

  case EA::LABEL:
    return "label";
  case EA::NO_TEMPLATE:
    return "no_template";
  case EA::TEMPLATE:
    return "template";
  case EA::SPECIALIZE:
    return "specialize";
  case EA::EQUIVOCAL:
    return "equivocal";
  case EA::LIKELY:
    return "likely";
  case EA::UNLIKELY:
    return "unlikely";

  case EA::SUPPORTED:
    return "supported";
  case EA::DEPRECIATED:
    return "depreciated";
  case EA::EXPERIMENTAL:
    return "experimental";
  case EA::NO_COPY:
    return "no_copy";
  case EA::MAY_COPY:
    return "may_copy";
  case EA::UNSTABLE_ADDRESS:
    return "unstable_address";

  case EA::STABLE_ADDRESS:
    return "stable_address";
  case EA::EXPLICIT_DROP:
    return "explicit_drop";

  case EA::IMPLICIT_DROP:
    return "implicit_drop";

  case EA::LAST:
    break;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::ExpressionAttribute
getExpressionAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using EA = ExpressionAttribute;
  switch (keyword) {
  case K::NO_LABEL:
    return EA::NONE;
  case K::OPAQUE:
    return EA::OPAQUE;
  case K::TRANSPARENT:
    return EA::TRANSPARENT;
  case K::OUTSIDE_SCOPE:
    return EA::OUTSIDE_SCOPE;
  case K::INSIDE_SCOPE:
    return EA::INSIDE_SCOPE;
  case K::LOCAL:
    return EA::LOCAL;
  case K::GLOBAL:
    return EA::GLOBAL;
  case K::PUBLIC:
    return EA::PUBLIC;
  case K::PRIVATE:
    return EA::PRIVATE;
  case K::PROTECTED:
    return EA::PROTECTED;
  case K::EXPORT:
    return EA::EXPORT;
  case K::NO_EXPORT:
    return EA::NO_EXPORT;
  case K::STATIC:
    return EA::STATIC;
  case K::DYNAMIC:
    return EA::DYNAMIC;
  case K::CAPTURE:
    return EA::CAPTURE;
  case K::NO_CAPTURE:
    return EA::NO_CAPTURE;
  case K::EAGER:
    return EA::EAGER;
  case K::LAZY:
    return EA::LAZY;
  case K::INLINE:
    return EA::INLINE;
  case K::NO_INLINE:
    return EA::NO_INLINE;
  case K::EXPLICIT_MANGLE:
    return EA::EXPLICIT_MANGLE;
  case K::IMPLICIT_MANGLE:
    return EA::IMPLICIT_MANGLE;
  case K::PACK:
    return EA::PACK;
  case K::NO_PACK:
    return EA::NO_PACK;
  case K::LABEL:
    return EA::LABEL;
  case K::TEMPLATE:
    return EA::TEMPLATE;
  case K::NO_TEMPLATE:
    return EA::NO_TEMPLATE;
  case K::SPECIALIZE:
    return EA::SPECIALIZE;
  case K::LIKELY:
    return EA::LIKELY;
  case K::UNLIKELY:
    return EA::UNLIKELY;
  case K::EQUIVOCAL:
    return EA::EQUIVOCAL;
  case K::SUPPORTED:
    return EA::SUPPORTED;
  case K::DEPRECIATED:
    return EA::DEPRECIATED;
  case K::EXPERIMENTAL:
    return EA::EXPERIMENTAL;
  case K::MAY_COPY:
    return EA::MAY_COPY;
  case K::NO_COPY:
    return EA::NO_COPY;
  case K::STABLE_ADDRESS:
    return EA::STABLE_ADDRESS;
  case K::UNSTABLE_ADDRESS:
    return EA::UNSTABLE_ADDRESS;
  case K::IMPLICIT_DROP:
    return EA::IMPLICIT_DROP;
  case K::EXPLICIT_DROP:
    return EA::EXPLICIT_DROP;
  default:
    break;
  }
  return EA::NONE;
}

enum class ExpressionFlags : std::uint64_t {
  NONE = 0,
  // labeling
  // no_label (default)
  LABEL = rq::getBit(0),

  // visibility
  // transparent (default)
  OPAQUE = rq::getBit(1),

  // scope_location
  // inside_scope (default)
  OUTSIDE_SCOPE = rq::getBit(2),

  // availability
  // local (default)
  GLOBAL = rq::getBit(3),

  // accessibility
  // private (default)
  PUBLIC = rq::getBit(4),
  PROTECTED = rq::getBit(5),

  // property mutability
  // no_partial_mutate (default)
  PARTIAL_MUTATE = rq::getBit(6),

  // exporting
  // no_export (default)
  EXPORT = rq::getBit(7),

  // generation_time
  // dynamic (default)
  STATIC = rq::getBit(8),

  // capturing
  // no_capture (default)
  CAPTURE = rq::getBit(9),

  // evaluation_time
  // lazy (default)
  EAGER = rq::getBit(10),

  // inlining
  // no_inline (default)
  INLINE = rq::getBit(11),

  // mangling
  // implicit_mangle (default)
  EXPLICIT_MANGLE = rq::getBit(12),

  // packing
  // no_pack (default)
  PACK = rq::getBit(13),

  // templating
  // no_template (default)
  TEMPLATE = rq::getBit(14),
  SPECIALIZE = rq::getBit(15),

  // likelihood
  // equivocal (default)
  LIKELY = rq::getBit(16),
  UNLIKELY = rq::getBit(17),

  // support
  // supported (default)
  DEPRECIATED = rq::getBit(18),
  EXPERIMENTAL = rq::getBit(19),

  // copyability
  // no_copy (default)
  MAY_COPY = rq::getBit(20),

  // address_stability
  // unstable_address (default)
  STABLE_ADDRESS = rq::getBit(21),

  // cleanup
  // implicit_drop (default)
  EXPLICIT_DROP = rq::getBit(22),

  LABELING = LABEL,                     // no_label vs label
  VISIBILITY = OPAQUE,                  // transparent vs opaque
  SCOPE_LOCATION = OUTSIDE_SCOPE,       // inside_scope vs outside_scope
  AVAILABILITY = GLOBAL,                // local vs global
  ACCESSIBILITY = PUBLIC | PROTECTED,   // private vs public vs protected
  PROPERTY_MUTABILITY = PARTIAL_MUTATE, // no_partial_mutate vs partial_mutate
  EXPORTING = EXPORT,                   // no_export vs export
  GENERATION_TIME = STATIC,             // dynamic vs static
  CAPTURING = CAPTURE,                  // no_capture vs capture
  EVALUATION_TIME = EAGER,              // lazy vs eager
  INLINING = INLINE,                    // no_inline vs inline
  MANGLING = EXPLICIT_MANGLE,           // implicit_mangle vs explicit_mangle
  PACKING = PACK,                       // no_pack vs pack
  TEMPLATING = TEMPLATE | SPECIALIZE,   // no_template vs template vs specialize
  LIKELYHOOD = LIKELY | UNLIKELY,       // equivocal vs likely vs unlikely
  SUPPORT =
      DEPRECIATED | EXPERIMENTAL, // supported vs depreciated vs experimental
  COPYABILITY = MAY_COPY,         // no_copy vs may_copy
  ADDRESS_STABILITY = STABLE_ADDRESS, // unstable_address vs stable_address
  CLEANUP = EXPLICIT_DROP             // implicit_drop vs explicit_drop
};

template <> struct is_flags<ExpressionFlags> : std::true_type {};

[[nodiscard]] inline rq::ExpressionFlags
getFlags(rq::ExpressionAttribute attribute) {
  using namespace rq;
  using EA = ExpressionAttribute;
  using EF = ExpressionFlags;
  switch (attribute) {
  case EA::NONE:
    return EF::NONE;

  // labeling
  case EA::LABEL:
    return EF::LABEL;

  // visibility
  case EA::TRANSPARENT:
    return EF::NONE;
  case EA::OPAQUE:
    return EF::OPAQUE;

  // scope_location
  case EA::INSIDE_SCOPE:
    return EF::NONE;
  case EA::OUTSIDE_SCOPE:
    return EF::OUTSIDE_SCOPE;

  // availability
  case EA::LOCAL:
    return EF::NONE;
  case EA::GLOBAL:
    return EF::GLOBAL;

  // accessibility
  case EA::PRIVATE:
    return EF::NONE;
  case EA::PUBLIC:
    return EF::PUBLIC;
  case EA::PROTECTED:
    return EF::PROTECTED;

  // property mutability
  case EA::NO_PARTIAL_MUTATE:
    return EF::NONE;
  case EA::PARTIAL_MUTATE:
    return EF::PARTIAL_MUTATE;

  // exporting
  case EA::NO_EXPORT:
    return EF::NONE;
  case EA::EXPORT:
    return EF::EXPORT;

  // generation_time
  case EA::DYNAMIC:
    return EF::NONE;
  case EA::STATIC:
    return EF::STATIC;

  // capturing
  case EA::NO_CAPTURE:
    return EF::NONE;
  case EA::CAPTURE:
    return EF::CAPTURE;

  // evaluation_time
  case EA::LAZY:
    return EF::NONE;
  case EA::EAGER:
    return EF::EAGER;

  // inlining
  case EA::NO_INLINE:
    return EF::NONE;
  case EA::INLINE:
    return EF::INLINE;

  // mangling
  case EA::IMPLICIT_MANGLE:
    return EF::NONE;
  case EA::EXPLICIT_MANGLE:
    return EF::EXPLICIT_MANGLE;

  // packing
  case EA::NO_PACK:
    return EF::NONE;
  case EA::PACK:
    return EF::PACK;

  // templating
  case EA::NO_TEMPLATE:
    return EF::NONE;
  case EA::TEMPLATE:
    return EF::TEMPLATE;
  case EA::SPECIALIZE:
    return EF::SPECIALIZE;

  // likelyhood
  case EA::EQUIVOCAL:
    return EF::NONE;
  case EA::LIKELY:
    return EF::LIKELY;
  case EA::UNLIKELY:
    return EF::UNLIKELY;

  // support
  case EA::SUPPORTED:
    return EF::NONE;
  case EA::DEPRECIATED:
    return EF::DEPRECIATED;
  case EA::EXPERIMENTAL:
    return EF::EXPERIMENTAL;

  // copyability
  case EA::NO_COPY:
    return EF::NONE;
  case EA::MAY_COPY:
    return EF::MAY_COPY;

  // address_stability
  case EA::UNSTABLE_ADDRESS:
    return EF::NONE;
  case EA::STABLE_ADDRESS:
    return EF::STABLE_ADDRESS;

  // cleanup
  case EA::IMPLICIT_DROP:
    return EF::NONE;
  case EA::EXPLICIT_DROP:
    return EF::EXPLICIT_DROP;

  case EA::LAST:
    break;
  }
  return EF::NONE;
}

// labeling
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoLabel(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::LABELING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLabel(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::LABEL);
}

// visibility
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasTransparent(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::VISIBILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasOpaque(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::OPAQUE);
}

// scope_location
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasInsideScope(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::SCOPE_LOCATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasOutsideScope(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::OUTSIDE_SCOPE);
}

// availability
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLocal(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::AVAILABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasGlobal(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::GLOBAL);
}

// accessibility
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPrivate(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::ACCESSIBILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPublic(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::PUBLIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasProtected(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::PROTECTED);
}

// property mutability
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasNoPartialMutate(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::PROPERTY_MUTABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasPartialMutate(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::PARTIAL_MUTATE);
}

// exporting
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoExport(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::EXPORTING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasExport(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::EXPORT);
}

// generation_time
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasDynamic(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::GENERATION_TIME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasStatic(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::STATIC);
}

// capturing
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoCapture(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::CAPTURING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasCapture(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::CAPTURE);
}

// evaluation_time
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLazy(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::EVALUATION_TIME);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasEager(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::EAGER);
}

// inlining
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoInline(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::INLINING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasInline(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::INLINE);
}

// mangling
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasImplicitMangle(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::MANGLING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExplicitMangle(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::EXPLICIT_MANGLE);
}

// packing
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoPack(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::PACKING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPack(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::PACK);
}

// templating
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasNoTemplate(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::TEMPLATING);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasTemplate(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::TEMPLATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasSpecialize(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::SPECIALIZE);
}

// likelyhood
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasEquivocal(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::LIKELYHOOD);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasLikely(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::LIKELY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasUnlikely(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::UNLIKELY);
}

// support
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasSupported(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::SUPPORT);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasDepreciated(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::DEPRECIATED);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExperimental(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::EXPERIMENTAL);
}

// copyability
[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoCopy(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::COPYABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasMayCopy(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::MAY_COPY);
}

// address_stability
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasUnstableAddress(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::ADDRESS_STABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasStableAddress(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::STABLE_ADDRESS);
}

// cleanup
[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasImplicitDrop(rq::ExpressionFlags flags) {
  return rq::getHasNone(flags, rq::ExpressionFlags::CLEANUP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasExplicitDrop(rq::ExpressionFlags flags) {
  return rq::getHasAll(flags, rq::ExpressionFlags::EXPLICIT_DROP);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAttribute(rq::ExpressionFlags flags, rq::ExpressionAttribute attribute) {
  rq::ExpressionFlags attribute_flags = rq::getFlags(attribute);
  return rq::getHasAll(flags, attribute_flags);
}

struct Expression;
struct TableSymbol;

enum class TypeAttribute : std::uint_fast8_t {
  NONE,
  CONSTANT,
  VAR,
  PARTIALLY_VAR,
  NO_VOLATILE,
  VOLATILE,
  DETERMINATE,
  INDETERMINATE,
  NO_ATOMIC,
  ATOMIC,
  NO_NULL_TERMINATE,
  NULL_TERMINATE,
  NO_ASSERT_BEFORE,
  ASSERT_BEFORE,
  NO_ASSERT_AFTER,
  ASSERT_AFTER
};

[[nodiscard]] inline llvm::StringRef getName(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  switch (attribute) {
  case TA::NONE:
    return "none";
  case TA::CONSTANT:
    return "constant";
  case TA::VAR:
    return "var";
  case TA::PARTIALLY_VAR:
    return "partially_var";
  case TA::NO_VOLATILE:
    return "no_volatile";
  case TA::VOLATILE:
    return "volatile";
  case TA::DETERMINATE:
    return "determinate";
  case TA::INDETERMINATE:
    return "indeterminate";
  case TA::NO_ATOMIC:
    return "no_atomic";
  case TA::ATOMIC:
    return "atomic";
  case TA::NO_NULL_TERMINATE:
    return "no_null_terminate";
  case TA::NULL_TERMINATE:
    return "null_terminate";
  case TA::NO_ASSERT_BEFORE:
    return "no_assert_before";
  case TA::ASSERT_BEFORE:
    return "assert_before";
  case TA::NO_ASSERT_AFTER:
    return "no_assert_after";
  case TA::ASSERT_AFTER:
    return "assert_after";
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] inline rq::TypeAttribute getTypeAttribute(rq::Keyword keyword) {
  using namespace rq;
  using K = Keyword;
  using TA = TypeAttribute;
  switch (keyword) {
  case K::CONSTANT:
    return TA::CONSTANT;
  case K::VAR:
    return TA::VAR;
  case K::PARTIALLY_VAR:
    return TA::PARTIALLY_VAR;
  case K::NO_VOLATILE:
    return TA::NO_VOLATILE;
  case K::VOLATILE:
    return TA::VOLATILE;
  case K::DETERMINATE:
    return TA::DETERMINATE;
  case K::INDETERMINATE:
    return TA::INDETERMINATE;
  case K::NO_ATOMIC:
    return TA::NO_ATOMIC;
  case K::ATOMIC:
    return TA::ATOMIC;
  case K::NO_NULL_TERMINATE:
    return TA::NO_NULL_TERMINATE;
  case K::NULL_TERMINATE:
    return TA::NULL_TERMINATE;
  case K::NO_ASSERT_BEFORE:
    return TA::NO_ASSERT_BEFORE;
  case K::ASSERT_BEFORE:
    return TA::ASSERT_BEFORE;
  case K::NO_ASSERT_AFTER:
    return TA::NO_ASSERT_AFTER;
  case K::ASSERT_AFTER:
    return TA::ASSERT_AFTER;
  default:
    break;
  }
  return TA::NONE;
}

enum class TypeFlags : std::uint8_t {
  NONE = 0,
  // mutability
  // constant (default)
  VAR = rq::getBit(0),
  PARTIALLY_VAR = rq::getBit(1),

  // volatility
  // no_volatile (default)
  VOLATILE = rq::getBit(2),

  // determinicity
  // determinate (default)
  INDETERMINATE = rq::getBit(3),

  // atomicity
  // no_atomic (default)
  ATOMIC = rq::getBit(4),

  // null_termination
  // no_null_terminate (default)
  NULL_TERMINATE = rq::getBit(5),

  // precondition
  // no_assert_before (default)
  ASSERT_BEFORE = rq::getBit(6),

  // postcondition
  // no_assert_after (default)
  ASSERT_AFTER = rq::getBit(7)

      MUTABILITY = VAR | PARTIALLY_VAR,
  VOLATILITY = VOLATILE,
  DETERMINICITY = INDETERMINATE,
  ATOMICITY = ATOMIC,
  NULL_TERMINATION = NULL_TERMINATE,
  PRECONDITION = ASSERT_BEFORE,
  POSTCONDITION = ASSERT_AFTER
};

template <> struct is_flags<TypeFlags> : std::true_type {};

[[nodiscard]] inline rq::TypeFlags getFlags(rq::TypeAttribute attribute) {
  using namespace rq;
  using TA = TypeAttribute;
  using TF = TypeFlags;
  switch (attribute) {
  case TA::NONE:
    return TF::NONE;
  case TA::CONSTANT:
    return TF::NONE;
  case TA::VAR:
    return TF::VAR;
  case TA::PARTIALLY_VAR:
    return TF::NONE;
  case TA::NO_VOLATILE:
    return TF::NONE;
  case TA::VOLATILE:
    return TF::VOLATILE;
  case TA::DETERMINATE:
    return TF::NONE;
  case TA::INDETERMINATE:
    return TF::INDETERMINATE;
  case TA::NO_ATOMIC:
    return TF::NONE;
  case TA::ATOMIC:
    return TF::ATOMIC;
  case TA::NO_NULL_TERMINATE:
    return TF::NONE;
  case TA::NULL_TERMINATE:
    return TF::NULL_TERMINATE;
  case TA::NO_ASSERT_BEFORE:
    return TF::NONE;
  case TA::ASSERT_BEFORE:
    return TF::ASSERT_BEFORE;
  case TA::NO_ASSERT_AFTER:
    return TF::NONE;
  case TA::ASSERT_AFTER:
    return TF::ASSERT_AFTER;
  }
  RQ_UNREACHABLE();
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasConstant(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::VAR);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVar(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::VAR);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasPartiallyVar(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::PARTIALLY_VAR);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatile(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::VOLATILE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoVolatile(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::VOLATILE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasDeterminate(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::INDETERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasIndeterminate(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::INDETERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoAtomic(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::ATOMIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomic(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::ATOMIC);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoNullTerminate(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::NULL_TERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTerminate(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::NULL_TERMINATE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoAssertBefore(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::PRECONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAssertBefore(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::ASSERT_BEFORE);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNoAssertAfter(rq::TypeFlags flags) {
  return rq::getHasNone(flags, rq::TypeFlags::POSTCONDITION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAssertAfter(rq::TypeFlags flags) {
  return rq::getHasAll(flags, rq::TypeFlags::ASSERT_AFTER);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool
getHasAttribute(rq::TypeFlags flags, rq::TypeAttribute attribute) {
  return rq::getHasAll(flags, rq::getFlags(attribute));
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasMutability(rq::TypeFlags flags) {
  return rq::getHasSome(flags, rq::TypeFlags::MUTABILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasVolatility(rq::TypeFlags flags) {
  return rq::getHasSome(flags, rq::TypeFlags::VOLATILITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasDeterminicity(rq::TypeFlags flags) {
  return rq::getHasSome(flags, rq::TypeFlags::DETERMINICITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasAtomicity(rq::TypeFlags flags) {
  return rq::getHasSome(flags, rq::TypeFlags::ATOMICITY);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getHasNullTermination(rq::TypeFlags flags) {
  return rq::getHasSome(flags, rq::TypeFlags::NULL_TERMINATION);
}

[[nodiscard]] RQ_ALWAYS_INLINE bool getIsValidMutability(rq::TypeFlags flags) {
  if (rq::getHasMutability(flags)) {
    unsigned mutability_count = 0;
    if (rq::getHasAll(flags, rq::TypeFlags::VAR)) {
      mutability_count++;
    }
    if (rq::getHasAll(flags, rq::TypeFlags::PARTIALLY_VAR)) {
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
  explicit ExpressionIterator(rq::Expression *expression_ptr)
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
  explicit ConstExpressionIterator(const rq::Expression *expression_ptr)
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
  rq::PtrWithFlags<const char, 2, rq::ExpressionSourceFlags>
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getHasValueBranches() const {
    return rq::getHasValueBranches(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Keyword getSituatedAscribe() const {
    return rq::getSituatedAscribeKeyword(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE rq::Situation getAttributeSituation() const {
    return rq::getAttributeInstantiationSituation(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpansion() const {
    return rq::getIsExpansion(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsTypeAttribute() const {
    return rq::getIsTypeAttribute(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getIsExpressionAttribute() const {
    return rq::getIsExpressionAttribute(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTop() const {
    return rq::getCanBeTop(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeStatement() const {
    return rq::getCanBeStatement(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLvalue() const {
    return rq::getCanBeLvalue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeRvalue() const {
    return rq::getCanBeRvalue(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeTuple() const {
    return rq::getCanBeTuple(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeLayout() const {
    return rq::getCanBeLayout(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeSignature() const {
    return rq::getCanBeSignature(this->getKeyword());
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
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeName() const {
    return rq::getCanBeName(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeNamespace() const {
    return rq::getCanBeNamespace(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool getCanBeAscription() const {
    return rq::getCanBeAscription(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeExpressionAttributeInstantiation() const {
    return rq::getCanBeExpressionAttributeInstantiation(this->getKeyword());
  }
  [[nodiscard]] RQ_ALWAYS_INLINE bool
  getCanBeTypeAttributeInstantiation() const {
    return rq::getCanBeTypeAttributeInstantiation(this->getKeyword());
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
  RQ_ALWAYS_INLINE void setSourceAfter(const SourceParam &source) {
    this->setSourceAt(source.getAfterSourceTextPtr());
  }
  template <typename SourceParam>
  RQ_ALWAYS_INLINE void setSourceAtEnd(const SourceParam &source) {
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

template <> struct is_acquired<rq::Expression> final : std::true_type {};

rq::ExpressionIterator &ExpressionIterator::operator++() {
  this->_expression_ptr =
      rq::dereferencePtr(this->_expression_ptr).getNextPtr();
  return *this;
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