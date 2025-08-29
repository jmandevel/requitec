// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <limits>
#include <string_view>

namespace requite {

// NOTE:
//  two underscores at the front means that the opcode is for internal use only

// NOTE:
//  one underscore at the front means that the opcode is for intermediate requite only

enum class  Opcode : unsigned {
  // this should never occur.
  __NONE,

  // HANDLES
  __LOCAL_HANDLE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  __INTEGER_LITERAL,
  // a literal that represents a float value with a decimal point.
  __FLOAT_LITERAL,
  // a literal that represents a string of text characters.
  __STRING_LITERAL,
  // a literal that represents a single text character.
  __CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  __IDENTIFIER_LITERAL,

  // ERRORS
  __ERROR,

  // SITUATIONAL
  _CLOVEN,
  _COLON,
  _INFERENCE,

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
  BAKE,
  _EXTEND,
  _BINDING,
  _ASCRIBE_TYPE,
  _ASCRIBE_STATEMENT,
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
  _SINGLETON_OF_ASCRIBED,
  CONTENT,
  _CONTENT_OF,
  _CONTENT_OF_ASCRIBED,
  ADDRESS,
  _ADDRESS_OF,
  _ADDRESS_OF_ASCRIBED,
  REFER,
  _REFERENCE_OF,
  _REFERENCE_OF_ASCRIBED,
  VIEW,
  _VIEW_OF,
  _VIEW_OF_ASCRIBED, 
  SLICE,
  _SLICE_OF,
  _SLICE_OF_ASCRIBED,
  ARRAY_SLICE,
  _ARRAY_SLICE_OF,
  _ARRAY_SLICE_OF_ASCRIBED,

  // ASSIGNMENT
  _ASSIGN,
  _ASSIGN_ADD,
  _ASSIGN_SUBTRACT,
  _ASSIGN_MULTIPLY,
  _ASSIGN_DIVIDE,
  _ASSIGN_MODULUS,

  // MOVE SEMANTICS
  COPY,
  _COPY_OF,
  MOVE,
  _MOVE_OF,
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
  OWNING,
  MAY_DISCARD,

  // PARAMETER RULES
  _POSITIONAL_PARAMETERS_END,
  _NAMED_PARAMETERS_BEGIN,

  // TRIPS
  _TUPLE,
  _LAYOUT,
  _NULL,
  _NULL_TYPE,
  _STRUCTURED_BINDING,
  _IGNORE,
  _SPECIALIZATION,

  // PROCEDURES
  _CALL,
  _INDEX,
  _SIGNATURE,
  DESTROY,
  _DESTROY_VALUE,
  DROP,
  _DROP_VALUE,
  ENTRY_POINT,
  FUNCTION,
  CONSTRUCTOR,
  DESTRUCTOR,
  RANGER,
  INDEXER,
  _ANONYMOUS_FUNCTION,
  CAPTURE,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  GOTO,
  EXIT,
  LOOP_BODY,

  // SYMBOLS
  OBJECT,
  VARIANT,
  TABLE,
  USE,

  // VALUES
  TRUE,
  FALSE,
  INDETERMINATE,
  THIS,
  RESULT,
  // retrieve command line arguments within entry_point.
  ARGUMENTS,
  // value returned into a circuit.
  IN,
  // value returned from an circuit.
  OUT,
  // the byte size of memory addresses on the current architecture.
  ADDRESS_SIZE,
  // the bit size of memory addresses on the current architecture.
  ADDRESS_DEPTH,
  // the amount of bits per byte on the current architecture. (almost always 8)
  BITS_PER_BYTE,

  // BUILTIN TYPES
  VOID,
  BOOLEAN,
  WORD,
  SIGNED,
  UNSIGNED,
  BFLOAT16,
  BINARY16,
  BINARY32,
  BINARY64,
  BINARY128,
  UTF8,

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
  SWITCH,
  MATCH,
  CASE,
  DEFAULT,
  LOOP,
  SCOPE,
  BLOCK,
  _INLINE_SCOPE,
  _INLINE_BLOCK,

  // RANGES
  RANGE,
  _LIMIT_RANGE_EQUAL,
  _LIMIT_RANGE_NOT_EQUAL,
  _LIMIT_RANGE_GREATER,
  _LIMIT_RANGE_GREATER_EQUAL,
  _LIMIT_RANGE_LESS,
  _LIMIT_RANGE_LESS_EQUAL,
  _LONG_RANGE,
  FOR,
  DO,
  WHILE,
  UNTIL,
  FOREVER,
  STEP,
  WHEN,
  _SHORT_RANGE,
  _SHORT_STEP_ADD,
  _SHORT_STEP_SUBTRACT,
  _SHORT_STEP_MULTIPLY,
  _SHORT_STEP_DIVIDE,
  _SHORT_STEP_MODULUS,
  _SHORT_WHILE_LESS,
  _SHORT_WHILE_GREATER,
  _SHORT_WHILE_LESS_EQUAL,
  _SHORT_WHILE_GREATER_EQUAL,
  _SHORT_WHILE_EQUAL,
  _SHORT_WHILE_NOT_EQUAL,

  // ACCESS MODIFIERS
  PRIVATE,
  PROTECTED,
  EXPORT,

  // SYMBOL GRAPH
  IMPORT,
  USE_TABLE,

  // SOURCES
  _MODULE_ROOT,

  // ERROR HANDLING AND DEBUGGING
  ASSERT,
  UNREACHABLE,

  // ATTRIBUTES
  MAY_PARENT,
  PARENT,
  POSITION,
  INLINE,
  MANGLED_NAME,
  _MANGLED_NAME_OF,
  PACK,
  USER,
  LABEL,
  TEMPLATE,

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
  GET,
  _GET_VALUE_OF_VARIANT,
  TYPE,
  _TYPE_OF,
  SYMBOL,
  _SYMBOL_OF,
  DISCRIMINANT,
  _DISCRIMINANT_OF,

  __LAST
};

static constexpr unsigned OPCODE_COUNT =
    static_cast<std::size_t>(requite::Opcode::__LAST);

[[nodiscard]] constexpr std::string_view getName(requite::Opcode opcode);
[[nodiscard]] constexpr requite::Opcode
getUniversalized(requite::Opcode opcode);
[[nodiscard]] constexpr requite::Opcode
getUniversalizedAscribed(requite::Opcode opcode);
[[nodiscard]] constexpr bool getIsValid(requite::Opcode opcode);
[[nodiscard]] constexpr bool getIsInternal(requite::Opcode opcode);
[[nodiscard]] constexpr bool getIsIntermediate(requite::Opcode opcode);
[[nodiscard]] constexpr bool
getBranchCanHaveNoSemicolon(requite::Opcode opcode);
[[nodiscard]] constexpr bool
getHasSemicolonSeperatedBranches(requite::Opcode opcode);
[[nodiscard]] constexpr unsigned
getCommaTerminatingBranchCount(requite::Opcode opcode);
[[nodiscard]] constexpr bool
getFirstCommaBranchCanBeTacit(requite::Opcode opcode);
[[nodiscard]] constexpr bool
getLastCommaBranchCanBeTacit(requite::Opcode opcode);
[[nodiscard]] constexpr bool getAllCommaBranchesCanBeTacit(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanHaveTacitCommaBranch(requite::Opcode opcode);
[[nodiscard]] constexpr bool getIsConverging(requite::Opcode opcode);
[[nodiscard]] constexpr bool getHasTextData(requite::Opcode opcode);

} // namespace requite

#include <requite/detail/opcode.hpp>
