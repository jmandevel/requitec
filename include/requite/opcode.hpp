// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <limits>
#include <string_view>

namespace requite {

// NOTE:
//  one underscore at the front means that the opcode is for intermediate use
//  only, and can not normally be written in source files at the head of an
//  operation unless -intermediate-form compiler flag is set.

// NOTE:
//  two underscores at the front means that the opcode is for internal use
//  only, and is a compile error if it is written in source files at the head of
//  an operation.

enum class Opcode : unsigned {
  // this should never occur.
  __NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  __INTEGER_LITERAL,
  // a literal that represents a real value with a decimal point.
  __REAL_LITERAL,
  // a literal that represents a string of text characters.
  __STRING_LITERAL,
  // a literal that represents a single text character.
  __CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  __IDENTIFIER_LITERAL,

  // ERRORS
  // this opcode is used whenever something goes wrong to denote that an error
  // occured.
  __ERROR,

  // SITUATIONAL
  _CALL_OR_SIGNATURE,
  _BIND_VALUE_OR_DEFAULT_VALUE,
  _BIND_SYMBOL_OR_DEFAULT_SYMBOL,
  _TRIP,
  _CONDUIT,
  _INFERENCED_TYPE_OR_INDETERMINATE,

  // LOGICAL
  // a nary logical operation that returns if all branches are true or not zero.
  _LOGICAL_AND,
  // a nary logical operation that returns if any branch is true or not zero.
  _LOGICAL_OR,
  // a unary logical operation that returns if the branch is false or zero.
  _LOGICAL_COMPLEMENT,

  // COMPARISON
  // a nary comparison operation that returns if each branch is less than the
  // previous.
  _GREATER,
  // a nary comparison operation that returns if each branch is less than or
  // equal to the previous.
  _GREATER_EQUAL,
  // a nary comparison operation that returns if each branch is greater than the
  // previous.
  _LESS,
  // a nary comparison operation that returns if each branch is greater than or
  // equal to the previous.
  _LESS_EQUAL,
  // a nary comparison operation that returns if all branches are equal.
  _EQUAL,
  // a nary comparison operation that returns if all branches are not equal.
  _NOT_EQUAL,

  // REFLECT
  // a nary reflect operation that reflects on values to get a resulting symbol
  // or value.
  _REFLECT_VALUE,
  // a nary reflect operation that reflects on symbols to get a resulting symbol
  // or value.
  _REFLECT_SYMBOL,

  // MEMBERS
  _MEMBER_VALUE_OF_VALUE,
  _MEMBER_SYMBOL_OF_VALUE,
  _MEMBER_VALUE_OF_SYMBOL,
  _MEMBER_SYMBOL_OF_SYMBOL,

  // BIND
  _BIND_VALUE,
  _BIND_SYMBOL,
  _DEFAULT_VALUE,
  _DEFAULT_SYMBOL,

  // APPLY
  _ASCRIBE,
  _CAST,
  STRINGIFY,
  CODEUNIFY,
  _IDENTIFY,
  _BAKE,

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
  _BITWISE_ROTATE_LEFT,
  _BITWISE_ROTATE_RIGHT,

  // MEMORY
  _CONCATINATE,
  FROM_FRONT,
  _FROM_FRONT_OF_VALUE,
  FROM_BACK,
  _FROM_BACK_OF_VALUE,
  TRUNCATE_FRONT,
  _TRUNCATE_FRONT_OF_VALUE,
  TRUNCATE_BACK,
  _TRUNCATE_BACK_OF_VALUE,
  AT,
  _AT_OFFSET_FROM_VALUE,
  ADDRESS,
  _ADDRESS_OF_VALUE,

  // ASSIGNMENT
  _ASSIGN,
  _ASSIGN_ADD,
  _ASSIGN_SUBTRACT,
  _ASSIGN_MULTIPLY,
  _ASSIGN_DIVIDE,
  _ASSIGN_MODULUS,

  // MOVE SEMANTICS
  COPY,
  _COPY_VALUE,
  MOVE,
  _MOVE_VALUE,
  SWAP,

  // SUBTYPE
  ARRAY,
  _REFERENCE,
  _STOLEN_REFERENCE,
  _POINTER,
  _FAT_POINTER,

  // TYPE MODIFIER
  MUTABLE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,

  // FIELD RULES
  _POSITIONAL_FIELDS_END,
  _NAMED_FIELDS_BEGIN,
  _POSITIONAL_FIELDS_END_AND_NAMED_FIELDS_BEGIN,

  // TRIPS
  _TUPLE_VALUE,
  _TUPLE_TYPE,
  _NULL_VALUE,
  _NULL_TYPE,
  _STRUCTURED_BINDING,
  _IGNORE,

  // TEMPLATES
  TEMPLATE,
  _SPECIALIZATION,

  // PROCEDURES
  _CALL,
  _SIGNATURE,
  DESTROY,
  _DESTROY_VALUE,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  CONSTRUCTOR,
  DESTRUCTOR,
  _ANONYMOUS_FUNCTION,
  _CAPTURE,

  // CONTROL FLOW
  RETURN,
  BREAK,
  CONTINUE,
  FALLTHROUGH,
  EXIT,
  GOTO,
  LABEL,

  // SYMBOLS
  OBJECT,
  TABLE,
  ALIAS,
  _LOCAL,
  GLOBAL,
  PROPERTY,
  CONSTANT,

  // VALUES
  // for variables inserted during circuiting stage. situates into
  // __temporary_with_data_id.
  _TEMPORARY,
  // a temporary with its id stored in the expression data.
  __TEMPORARY_WITH_DATA_ID,
  // the boolean value of true.
  TRUE,
  // the boolean value of false.
  FALSE,
  // a value that contains the iterated value of a for_each block.
  VALUE,
  // a value that contains the iteration index of for, while, for_each, and loop
  // blocks.
  INDEX,
  // a value that contains the caught exception within a catch block.
  EXCEPTION,
  // a value that can be assigned to any variable on declaration to denote that
  // its memory should not be initialized and should be left containing garbage
  // memory. This is implicitly inserted into local and global operations when
  // no value branch is provided.
  _INDETERMINATE,
  // a value to be assigned to a property, denoting that it has no default value
  // and needs to be explicitly initialized in each constructor. This is
  // implicitly inserted into property operations when no value branch is
  // provided.
  NO_DEFAULT_VALUE,
  // an element count for casted arrays, denoting that the count should be
  // inferenced from the casted value. This is implicitly inserted into array
  // operations when no count branch is provided.
  INFERENCED_COUNT,
  // a variable for accessing the instance an object from within members.
  THIS,
  // a reference to the return value of a procedure.
  RESULT,
  // retrieve an argument of the current procedure at an index.
  ARGUMENT,
  // value returned into a circuit.
  INPUT,
  // value returned from an circuit.
  OUTPUT,
  // the byte size of memory addresses on the current architecture.
  ADDRESS_SIZE,
  // the bit size of memory addresses on the current architecture.
  ADDRESS_DEPTH,
  // the amount of bits per byte on the current architecture. (almost always 8)
  BITS_PER_BYTE,

  // BUILTIN TYPES
  _INFERENCED_TYPE,
  VOID,
  BOOLEAN,
  WORD,
  SIGNED_INTEGER,
  UNSIGNED_INTEGER,
  BINARY_HALF_FLOAT,
  BINARY_SINGLE_FLOAT,
  BINARY_DOUBLE_FLOAT,
  BINARY_QUAD_FLOAT,
  CHARACTER,
  UTF8,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS,
  FIRST_VARIADIC_ARGUMENT,
  _FIRST_VARIADIC_ARGUMENT_OF_VALUE,
  NEXT_VARIADIC_ARGUMENT,
  _NEXT_VARIADIC_ARGUMENT_OF_VALUE,

  // SCOPES
  IF,
  ELSE_IF,
  ELSE,
  SWITCH,
  CASE,
  DEFAULT_CASE,
  FOR,
  WHILE,
  DO_WHILE,
  FOR_EACH,
  LOOP,
  SCOPE,
  _VALUE_CONDUIT,
  _JUNCTION_CONDUIT,
  _DESTINATION_CONDUIT,

  // ACCESS MODIFIERS
  PRIVATE,
  PROTECTED,
  EXPORT,

  // SYMBOL GRAPH
  IMPORT,
  USE,

  // SOURCES
  MODULE,

  // ERROR HANDLING AND DEBUGGING
  ASSERT,
  TRY,
  CATCH,
  THROW,
  MAY_THROW,

  // ATTRIBUTES
  EXTERNAL,
  C,
  NOT_FINAL,
  MAY_DISCARD,
  NO_SHORT_CIRCUIT,
  NO_REMAINDER,
  INLINE,
  MANGLED_NAME,
  _MANGLED_NAME_OF_SYMBOL,
  PACK,

  // REFLECTED VALUES
  SIZE,
  _SIZE_OF_VALUE,
  _SIZE_OF_TYPE,
  DEPTH,
  _DEPTH_OF_VALUE,
  _DEPTH_OF_TYPE,
  COUNT,
  _COUNT_OF_VALUE,
  _COUNT_OF_TYPE,
  NAME,
  _NAME_OF_VALUE,
  _NAME_OF_SYMBOL,
  LINE,
  _LINE_OF_VALUE,
  _LINE_OF_SYMBOL,
  COLUMN,
  _COLUMN_OF_VALUE,
  _COLUMN_OF_SYMBOL,
  IS,
  ARE_SAME,

  // REFLECTED SYMBOLS
  TYPE,
  _TYPE_OF_VALUE,
  UNDERLYING,
  _UNDERLYING_OF_TYPE,

  __LAST
};

static constexpr unsigned OPCODE_COUNT =
    static_cast<std::size_t>(requite::Opcode::__LAST);

[[nodiscard]] constexpr std::string_view getName(requite::Opcode opcode);

[[nodiscard]] constexpr requite::Opcode
getUniversalizedValue(requite::Opcode opcode);

[[nodiscard]] constexpr requite::Opcode
getUniversalizedSymbol(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsValid(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsInternalUseOnly(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsIntermediateOperation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsConverging(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasTextData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasUnsignedIntegerData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasIntegerData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasScopeData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasObjectData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasOverloadData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasLabelData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasAliasData(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasVariableData(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getHasAnonymousFunctionData(requite::Opcode opcode);

} // namespace requite

#include <requite/detail/opcode.hpp>
