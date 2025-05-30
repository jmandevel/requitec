// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <limits>
#include <string_view>

namespace requite {

// NOTE:
//  an underscore at the front means that the opcode is for internal use
//  only, and is a compile error if it is written in Requite source code.

// NOTE:
//  underscores are added to the end of opcode names to prevent name collisions
//  with c++ keywords. Trailing underscores are not included in the opcode name
//  in the source code

enum class Opcode : unsigned {
  // this should never occur.
  _NONE,

  // LITERALS
  // a literal that represents an integer value. May have a base.
  _INTEGER_LITERAL,
  // a literal that represents a real value with a decimal point.
  _REAL_LITERAL,
  // a literal that represents a string of text characters.
  _STRING_LITERAL,
  // a literal that represents a single text character.
  _CODEUNIT_LITERAL,
  // a literal that is used to refeer to user defined symbols.
  _IDENTIFIER_LITERAL,

  // ERRORS
  // this opcode is used whenever something goes wrong to denote that an error
  // occured.
  _ERROR,

  // SITUATIONAL
  // a situational opcode that occurs when a DOUBLE_AMBERSAND_OPERATOR token is
  // used as a prefix notation opcode. in VALUE sitation, it is situated to
  // logical_and. in SYMBOL situation, it is situated to two reference
  // operations
  // with one nested inside the other.
  SITUATIONAL_LOGICAL_AND_OR_DOUBLE_REFERENCE,
  // a situational opcode that occurs when a PLUS_OPERATOR token is used as a
  // prefix notation opcode. in VALUE situation, it is situated to add. in
  // ATTRIBUTE situation, it is situated to null_terminated.
  SITUATIONAL_ADD_OR_NULL_TERMINATED,
  // a situational opcode that occurs when a DASH_OPERATOR token is used as a
  // prefix notation opcode. this opcode is only valid in VALUE situation. if it
  // has one branch, it is situated to negate. if it has two or more branches,
  // it
  // is situated to subtract.
  SITUATIONAL_SUBTRACT_OR_NEGATE,
  // a situational opcode that occurs when a STAR_OPERATOR token is used as a
  // prefix notation opcode. in SYMBOL situation, it is situated to pointer. in
  // VALUE situation, it is situated to multiply.
  SITUATIONAL_MULTIPLY_OR_POINTER,
  // a situational opcode that occurs when a SLASH_OPERATOR token is used as a
  // prefix notation opcode. in SYMBOL situation, it is situated to slice. in
  // VALUE situation, it is situated to divide.
  SITUATIONAL_DIVIDE_OR_FAT_POINTER,
  // a situational opcode that occurs when a PERCENT_OPERATOR token is used as
  // a prefix notation opcode. in SYMBOL situation, it is situated to array. in
  // VALUE situation, it is situated to modulus.
  SITUATIONAL_MODULUS_OR_ARRAY,
  // a situational opcode that occurs when a AMBERSAND_OPERATOR token is used
  // as a prefix notation opcode. in SYMBOL situation, it is situated to
  // reference.
  // in VALUE situation, it is situated to bitwise_and.
  SITUATIONAL_BITWISE_AND_OR_REFERENCE,
  SITUATIONAL_CALL_OR_SIGNATURE,
  SITUATIONAL_BIND_VALUE_OR_DEFAULT_VALUE,
  SITUATIONAL_BIND_SYMBOL_OR_DEFAULT_SYMBOL,
  SITUATIONAL_TRIP,
  SITUATIONAL_INFERENCE_OR_INDETERMINATE,

  // LOGICAL
  // a nary logical operation that returns if all branches are true or not zero.
  LOGICAL_AND,
  // a nary logical operation that returns if any branch is true or not zero.
  LOGICAL_OR,
  // a unary logical operation that returns if the branch is false or zero.
  LOGICAL_COMPLEMENT,

  // COMPARISON
  // a nary comparison operation that returns if each branch is less than the
  // previous.
  GREATER,
  // a nary comparison operation that returns if each branch is less than or
  // equal to the previous.
  GREATER_EQUAL,
  // a nary comparison operation that returns if each branch is greater than the
  // previous.
  LESS,
  // a nary comparison operation that returns if each branch is greater than or
  // equal to the previous.
  LESS_EQUAL,
  // a nary comparison operation that returns if all branches are equal.
  EQUAL,
  // a nary comparison operation that returns if all branches are not equal.
  NOT_EQUAL,

  // REFLECT
  // a nary reflect operation that reflects on values to get a resulting symbol
  // or value.
  REFLECT_VALUE,
  // a nary reflect operation that reflects on symbols to get a resulting symbol
  // or value.
  REFLECT_SYMBOL,

  // BIND
  BIND_VALUE,
  BIND_SYMBOL,
  DEFAULT_VALUE,
  DEFAULT_SYMBOL,

  // APPLY
  ASCRIBE,
  CAST,
  STRINGIFY,
  CODEUNIFY,
  IDENTIFY,
  BAKE,

  // ARITHMETIC
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  MODULUS,
  NEGATE,

  // BITWISE
  BITWISE_CAST,
  BITWISE_OR,
  BITWISE_AND,
  BITWISE_XOR,
  BITWISE_COMPLEMENT,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT,
  BITWISE_ROTATE_LEFT,
  BITWISE_ROTATE_RIGHT,

  // MEMORY
  CONCATINATE,
  FROM_FRONT,
  FROM_BACK,
  TRUNCATE_FRONT,
  TRUNCATE_BACK,
  AT,
  DEREFERENCE,

  // ASSIGNMENT
  ASSIGN,
  ASSIGN_ADD,
  ASSIGN_SUBTRACT,
  ASSIGN_MULTIPLY,
  ASSIGN_DIVIDE,
  ASSIGN_MODULUS,

  // MOVE SEMANTICS
  COPY,
  MOVE,
  SWAP,

  // SUBTYPE
  ARRAY,
  REFERENCE,
  STOLEN_REFERENCE,
  POINTER,
  FAT_POINTER,

  // TYPE MODIFIER
  MUTABLE,
  VOLATILE,
  ATOMIC,
  NULL_TERMINATED,

  // TRIPS
  TUPLE,
  ANONYMOUS_OBJECT,
  NULL_,
  IGNORE,

  // TEMPLATES
  TEMPLATE,
  SPECIALIZATION,

  // PROCEDURES
  NO_PARAMETER_SIGNATURE,
  NAMED_PARAMETER_SIGNATURE,
  POSITIONAL_PARAMETER_SIGNATURE,
  NO_ARGUMENT_CALL,
  NAMED_ARGUMENT_CALL,
  POSITIONAL_ARGUMENT_CALL,
  DESTROY,
  ENTRY_POINT,
  FUNCTION,
  METHOD,
  CONSTRUCTOR,
  DESTRUCTOR,
  ANONYMOUS_FUNCTION,
  CAPTURE,

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
  LOCAL,
  GLOBAL,
  PROPERTY,
  CONSTANT,

  // VALUES
  // a special variable that is replaced every time it is assigned to. its
  // previous value is destroyed. its type can change on each assignment. a
  // positive numeric integer literal can be passed into the first branch to
  // differentiate between multiple temp values. if no index is provided, this
  // temp is sitauted to default_temp.
  TEMP,
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
  INDETERMINATE,
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
  // value returned into a inlet.
  INPUT,
  // value returned from an outlet.
  OUTPUT,
  // the byte size of memory addresses on the current architecture.
  ADDRESS_SIZE,
  // the bit size of memory addresses on the current architecture.
  ADDRESS_DEPTH,
  // the amount of bits per byte on the current architecture. (almost always 8)
  BITS_PER_BYTE,

  // BUILTIN TYPES
  INFERENCE,
  VOID,
  BOOLEAN,
  WORD,
  SIGNED_INTEGER,
  UNSIGNED_INTEGER,
  BINARY_HALF_FLOAT,
  BINARY_SINGLE_FLOAT,
  BINARY_DOUBLE_FLOAT,
  BINARY_QUAD_FLOAT,
  C_CHAR,
  UTF8,

  // VARIADIC ARGUMENTS
  VARIADIC_ARGUMENTS,
  START_VARIADIC_ARGUMENT,
  NEXT_VARIADIC_ARGUMENT,

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
  CONDUIT,

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
  THROWS,

  // ATTRIBUTES
  EXTERNAL,
  C,
  POSITIONAL_ARGUMENTS,
  NOT_FINAL,
  MAY_DISCARD,
  NO_SHORT_CIRCUIT,
  NO_REMAINDER,
  INLINE,
  MANGLED_NAME,
  PACK,

  // REFLECTION
  SIZE,
  DEPTH,
  COUNT,
  TYPE,
  NAME,
  LINE,
  COLUMN,
  UNDERLYING,
  IS,

  _LAST
};

static constexpr unsigned OPCODE_COUNT =
    static_cast<std::size_t>(requite::Opcode::_LAST);

[[nodiscard]] constexpr std::string_view getName(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsValid(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsInternalUseOnly(requite::Opcode opcode);

[[nodiscard]] constexpr bool getIsConverging(requite::Opcode opcode);

[[nodiscard]] constexpr bool getHasTextData(requite::Opcode opcode);

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
