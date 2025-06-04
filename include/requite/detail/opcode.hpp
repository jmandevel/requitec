// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <magic_enum.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

namespace requite {

namespace _opcode {
enum _OpcodeFlags : std::uint32_t {
  _NONE = 0,
  _CONVERGING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(0)),
  _INTERNAL_USE_ONLY =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(1)),
  _INTERMEDIATE_OPERATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(2)),
  _ROOT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(3)),
  _BASE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(4)),
  _GLOBAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(5)),
  _MATTE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(6)),
  _VALUE_REFLECTIVE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(7)),
  _SYMBOL_REFLECTIVE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(8)),
  _OBJECT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(9)),
  _MATTE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(10)),
  _VALUE_REFLECTIVE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(11)),
  _SYMBOL_REFLECTIVE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(12)),
  _MATTE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(13)),
  _VALUE_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(14)),
  _SYMBOL_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(15)),
  _MATTE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(16)),
  _VALUE_REFLECTIVE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(17)),
  _SYMBOL_REFLECTIVE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(18)),
  _MATTE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(19)),
  _VALUE_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(20)),
  _SYMBOL_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(21)),
  _ATTRIBUTE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(22)),
  _VALUE_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(23)),
  _SYMBOL_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(24)),
  _NAMED_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(25)),
  _POSITIONAL_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(26)),
  _TEMPLATE_PARAMETER =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(27)),
  _SYMBOL_NAME =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(28)),
  _SYMBOL_PATH =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(29)),
  _SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(30)),
  _LAST_SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(31))
};
}

[[nodiscard]] constexpr requite::_opcode::_OpcodeFlags
operator|(requite::_opcode::_OpcodeFlags lhs,
          requite::_opcode::_OpcodeFlags rhs) {
  using Underlying = std::underlying_type_t<requite::_opcode::_OpcodeFlags>;
  return static_cast<requite::_opcode::_OpcodeFlags>(
      static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

[[nodiscard]] constexpr requite::_opcode::_OpcodeFlags
_getFlags(requite::Opcode opcode) {
  using namespace requite;
  using namespace requite::_opcode;
  switch (opcode) {
  case Opcode::__NONE:
    return _INTERNAL_USE_ONLY;

  // LITERALS
  case Opcode::__INTEGER_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::__REAL_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::__STRING_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::__CODEUNIT_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::__IDENTIFIER_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL | _SYMBOL_NAME |
           _SYMBOL_PATH;

  // ERROR
  case Opcode::__ERROR:
    return _NONE;

  // SITUATIONAL
  case Opcode::_CALL_OR_SIGNATURE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE | _MATTE_SYMBOL |
           _POSITIONAL_FIELD;
  case Opcode::_BIND_VALUE_OR_DEFAULT_VALUE:
    return _INTERMEDIATE_OPERATION | _VALUE_BINDING | _NAMED_FIELD |
           _POSITIONAL_FIELD | _TEMPLATE_PARAMETER;
  case Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return _INTERMEDIATE_OPERATION | _SYMBOL_BINDING | _NAMED_FIELD |
           _TEMPLATE_PARAMETER;
  case Opcode::_TRIP:
    return _INTERMEDIATE_OPERATION | _MATTE_JUNCTION | _MATTE_SYMBOL |
           _POSITIONAL_FIELD;
  case Opcode::_CONDUIT:
    return _INTERMEDIATE_OPERATION | _MATTE_JUNCTION;
  case Opcode::_INFERENCED_TYPE_OR__INDETERMINATE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE | _MATTE_SYMBOL |
           _POSITIONAL_FIELD;

  // LOGICAL
  case Opcode::_LOGICAL_AND:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_LOGICAL_OR:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_LOGICAL_COMPLEMENT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // COMPAIRSON
  case Opcode::_GREATER:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_GREATER_EQUAL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_LESS:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_LESS_EQUAL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_EQUAL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_NOT_EQUAL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // REFLECT
  case Opcode::_REFLECT_VALUE:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL |
           _POSITIONAL_FIELD;
  case Opcode::_REFLECT_SYMBOL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL |
           _POSITIONAL_FIELD | _SYMBOL_PATH;

  // MEMBERS
  case Opcode::_MEMBER_VALUE_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::_MEMBER_SYMBOL_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_MEMBER_VALUE_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::_MEMBER_SYMBOL_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // BIND
  case Opcode::_BIND_VALUE:
    return _INTERMEDIATE_OPERATION | _VALUE_BINDING | _TEMPLATE_PARAMETER;
  case Opcode::_BIND_SYMBOL:
    return _INTERMEDIATE_OPERATION | _SYMBOL_BINDING | _NAMED_FIELD |
           _TEMPLATE_PARAMETER;
  case Opcode::_DEFAULT_VALUE:
    return _INTERMEDIATE_OPERATION | _NAMED_FIELD | _POSITIONAL_FIELD |
           _TEMPLATE_PARAMETER;
  case Opcode::_DEFAULT_SYMBOL:
    return _INTERMEDIATE_OPERATION | _TEMPLATE_PARAMETER;

  // APPLY
  case Opcode::_ASCRIBE:
    return _INTERMEDIATE_OPERATION | _BASE_STATEMENT | _GLOBAL_STATEMENT |
           _OBJECT_STATEMENT | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_CAST:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::STRINGIFY:
    return _MATTE_VALUE;
  case Opcode::CODEUNIFY:
    return _MATTE_VALUE;
  case Opcode::_IDENTIFY:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL | _SYMBOL_NAME |
           _SYMBOL_PATH | _POSITIONAL_FIELD;
  case Opcode::_BAKE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE |
           _SYMBOL_REFLECTIVE_VALUE | _MATTE_SYMBOL | _VALUE_REFLECTIVE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_PATH | _POSITIONAL_FIELD;

  // ARITHMETIC
  case Opcode::_ADD:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_SUBTRACT:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_MULTIPLY:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_DIVIDE:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_MODULUS:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_NEGATE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // BITWISE
  case Opcode::_BITWISE_CAST:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_OR:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_AND:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_XOR:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_COMPLEMENT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_SHIFT_LEFT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_SHIFT_RIGHT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_ROTATE_LEFT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_BITWISE_ROTATE_RIGHT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // MEMORY
  case Opcode::_CONCATINATE:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::FROM_FRONT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_FROM_FRONT_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::FROM_BACK:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_FROM_BACK_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::TRUNCATE_FRONT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_TRUNCATE_FRONT_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::TRUNCATE_BACK:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_TRUNCATE_BACK_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::AT:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::_AT_OFFSET_FROM_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::ADDRESS:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::_ADDRESS_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;

  // ASSIGNMENT
  case Opcode::_ASSIGN:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_DESTINATION |
           _MATTE_VALUE | _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_ASSIGN_ADD:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_ASSIGN_SUBTRACT:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_ASSIGN_MULTIPLY:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_ASSIGN_DIVIDE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_ASSIGN_MODULUS:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_LOCAL_STATEMENT;

  // MOVE SEMANTICS
  case Opcode::COPY:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_COPY_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::MOVE:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_MOVE_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::SWAP:
    return _MATTE_LOCAL_STATEMENT;

  // SUBTYPE
  case Opcode::ARRAY:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_REFERENCE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_STOLEN_REFERENCE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_POINTER:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_FAT_POINTER:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // TYPE MODIFIER
  case Opcode::MUTABLE:
    return _ATTRIBUTE;
  case Opcode::VOLATILE:
    return _ATTRIBUTE;
  case Opcode::ATOMIC:
    return _ATTRIBUTE;
  case Opcode::NULL_TERMINATED:
    return _ATTRIBUTE;

  // TRIPS
  case Opcode::_TUPLE_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_TUPLE_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_NULL_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_NULL_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::_IGNORE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION;

  // TEMPLATES
  case Opcode::TEMPLATE:
    return _ATTRIBUTE;
  case Opcode::_SPECIALIZATION:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE | _MATTE_SYMBOL |
           _POSITIONAL_FIELD;

  // PROCEDURES
  case Opcode::_CALL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_SIGNATURE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_POSITIONAL_PARAMETERS_END:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_NAMED_PARAMETERS_BEGIN:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_POSITIONAL_PARAMETERS_END_AND_NAMED_PARAMETERS_BEGIN:
    return _INTERMEDIATE_OPERATION;
  case Opcode::DESTROY:
    return _VALUE_REFLECTIVE_LOCAL_STATEMENT;
  case Opcode::_DESTROY_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::ENTRY_POINT:
    return _BASE_STATEMENT;
  case Opcode::FUNCTION:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::METHOD:
    return _OBJECT_STATEMENT;
  case Opcode::CONSTRUCTOR:
    return _OBJECT_STATEMENT;
  case Opcode::DESTRUCTOR:
    return _OBJECT_STATEMENT;
  case Opcode::_ANONYMOUS_FUNCTION:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_CAPTURE:
    return _NONE;

  // CONTROL FOLW
  case Opcode::RETURN:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::BREAK:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::CONTINUE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::FALLTHROUGH:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::EXIT:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::GOTO:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::LABEL:
    return _MATTE_LOCAL_STATEMENT;

  // SYMBOLS
  case Opcode::OBJECT:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::TABLE:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ALIAS:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::_LOCAL:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::GLOBAL:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::PROPERTY:
    return _OBJECT_STATEMENT;
  case Opcode::CONSTANT:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;

  // VALUES
  case Opcode::TEMP:
    return _MATTE_JUNCTION;
  case Opcode::TRUE:
    return _MATTE_VALUE;
  case Opcode::FALSE:
    return _MATTE_VALUE;
  case Opcode::VALUE:
    return _MATTE_JUNCTION;
  case Opcode::INDEX:
    return _MATTE_VALUE;
  case Opcode::EXCEPTION:
    return _MATTE_VALUE;
  case Opcode::_INDETERMINATE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::NO_DEFAULT_VALUE:
    return _MATTE_VALUE;
  case Opcode::INFERENCED_COUNT:
    return _MATTE_VALUE;
  case Opcode::THIS:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::RESULT:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::INPUT:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::OUTPUT:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::ADDRESS_SIZE:
    return _MATTE_VALUE;
  case Opcode::ADDRESS_DEPTH:
    return _MATTE_VALUE;
  case Opcode::BITS_PER_BYTE:
    return _MATTE_VALUE;

  // BUILTIN TYPES
  case Opcode::_INFERENCED_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::VOID:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::BOOLEAN:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::WORD:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SIGNED_INTEGER:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::UNSIGNED_INTEGER:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::BINARY_HALF_FLOAT:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::BINARY_SINGLE_FLOAT:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::BINARY_DOUBLE_FLOAT:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::BINARY_QUAD_FLOAT:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::CHARACTER:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::UTF8:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // VARIADIC ARGUMENTS
  case Opcode::VARIADIC_ARGUMENTS:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::FIRST_VARIADIC_ARGUMENT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::NEXT_VARIADIC_ARGUMENT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // SCOPES
  case Opcode::IF:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::ELSE_IF:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::ELSE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::SWITCH:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::CASE:
    return _SWITCH_CASE | _LAST_SWITCH_CASE;
  case Opcode::DEFAULT_CASE:
    return _LAST_SWITCH_CASE;
  case Opcode::FOR:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::WHILE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::DO_WHILE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::FOR_EACH:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::LOOP:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::SCOPE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::_VALUE_CONDUIT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_JUNCTION_CONDUIT:
    return _INTERMEDIATE_OPERATION | _MATTE_JUNCTION;
  case Opcode::_DESTINATION_CONDUIT:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION;

  // ACCESS MODIFIERS
  case Opcode::PRIVATE:
    return _ATTRIBUTE;
  case Opcode::PROTECTED:
    return _ATTRIBUTE;
  case Opcode::EXPORT:
    return _ATTRIBUTE;

  // SYMBOL GRAPH
  case Opcode::IMPORT:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::USE:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;

  // SOURCES
  case Opcode::MODULE:
    return _ROOT_STATEMENT;

  // ERROR HANDLING AND DEBUGGING
  case Opcode::ASSERT:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::TRY:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::CATCH:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::THROW:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::MAY_THROW:
    return _ATTRIBUTE;
  case Opcode::THROWS:
    return _ATTRIBUTE;

  // ATTRIBUTES
  case Opcode::EXTERNAL:
    return _ATTRIBUTE;
  case Opcode::C:
    return _ATTRIBUTE;
  case Opcode::NOT_FINAL:
    return _ATTRIBUTE;
  case Opcode::MAY_DISCARD:
    return _ATTRIBUTE;
  case Opcode::NO_SHORT_CIRCUIT:
    return _ATTRIBUTE;
  case Opcode::NO_REMAINDER:
    return _ATTRIBUTE;
  case Opcode::INLINE:
    return _ATTRIBUTE;
  case Opcode::MANGLED_NAME:
    return _SYMBOL_REFLECTIVE_VALUE | _ATTRIBUTE;
  case Opcode::_MANGLED_NAME_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::PACK:
    return _ATTRIBUTE;

  // REFLECTED VALUES
  case Opcode::SIZE:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_SIZE_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_SIZE_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::DEPTH:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_DEPTH_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_DEPTH_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::COUNT:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_COUNT_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_COUNT_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::NAME:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_NAME_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_NAME_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::LINE:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_LINE_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_LINE_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::COLUMN:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_COLUMN_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_COLUMN_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::IS:
    return _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::ARE_SAME:
    return _MATTE_VALUE;

  // REFLECTED SYMBOLS
  case Opcode::TYPE:
    return _VALUE_REFLECTIVE_SYMBOL;
  case Opcode::_TYPE_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::UNDERLYING:
    return _SYMBOL_REFLECTIVE_SYMBOL;
  case Opcode::_UNDERLYING_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;

  case Opcode::__LAST:
    break;
  }
  return _NONE;
}

[[nodiscard]] constexpr bool
_getHasFlags(requite::Opcode opcode, requite::_opcode::_OpcodeFlags flags) {
  requite::_opcode::_OpcodeFlags opcode_flags = requite::_getFlags(opcode);
  const bool has_flags = (opcode_flags & flags) == flags;
  return has_flags;
}

constexpr std::string_view getName(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::__NONE:
    return "__none";

  // LITERALS
  case requite::Opcode::__INTEGER_LITERAL:
    return "__integer_literal";
  case requite::Opcode::__REAL_LITERAL:
    return "__real_literal";
  case requite::Opcode::__STRING_LITERAL:
    return "__string_literal";
  case requite::Opcode::__CODEUNIT_LITERAL:
    return "__codeunit_literal";
  case requite::Opcode::__IDENTIFIER_LITERAL:
    return "__identifier_literal";

  // ERRORS
  case requite::Opcode::__ERROR:
    return "__error";

  // SITUATIONAL
  case requite::Opcode::_CALL_OR_SIGNATURE:
    return "_call_or_signature";
  case requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE:
    return "_bind_value_or_default_value";
  case requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return "_bind_symbol_or_default_symbol";
  case requite::Opcode::_TRIP:
    return "_trip";
  case requite::Opcode::_CONDUIT:
    return "_conduit";
  case requite::Opcode::_INFERENCED_TYPE_OR__INDETERMINATE:
    return "_inferenced_type_or__indeterminate";

  // LOGICAL
  case requite::Opcode::_LOGICAL_AND:
    return "_logical_and";
  case requite::Opcode::_LOGICAL_OR:
    return "_logical_or";
  case requite::Opcode::_LOGICAL_COMPLEMENT:
    return "_logical_complement";

  // COMPARISON
  case requite::Opcode::_GREATER:
    return "_greater";
  case requite::Opcode::_GREATER_EQUAL:
    return "_greater_equal";
  case requite::Opcode::_LESS:
    return "_less";
  case requite::Opcode::_LESS_EQUAL:
    return "_less_equal";
  case requite::Opcode::_EQUAL:
    return "_equal";
  case requite::Opcode::_NOT_EQUAL:
    return "_not_equal";

  // REFLECT
  case requite::Opcode::_REFLECT_VALUE:
    return "_reflect_value";
  case requite::Opcode::_REFLECT_SYMBOL:
    return "_reflect_symbol";

  // MEMBERS
  case requite::Opcode::_MEMBER_VALUE_OF_VALUE:
    return "_member_value_of_value";
  case requite::Opcode::_MEMBER_SYMBOL_OF_VALUE:
    return "_member_symbol_of_value";
  case requite::Opcode::_MEMBER_VALUE_OF_SYMBOL:
    return "_member_value_of_symbol";
  case requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL:
    return "_member_symbol_of_symbol";

  // BIND
  case requite::Opcode::_BIND_VALUE:
    return "_bind_value";
  case requite::Opcode::_BIND_SYMBOL:
    return "_bind_symbol";
  case requite::Opcode::_DEFAULT_VALUE:
    return "default_value";
  case requite::Opcode::_DEFAULT_SYMBOL:
    return "_default_symbol";

  // APPLY
  case requite::Opcode::_ASCRIBE:
    return "_ascribe";
  case requite::Opcode::_CAST:
    return "_cast";
  case requite::Opcode::STRINGIFY:
    return "stringify";
  case requite::Opcode::CODEUNIFY:
    return "codeunify";
  case requite::Opcode::_IDENTIFY:
    return "_identify";
  case requite::Opcode::_BAKE:
    return "_bake";

  // ARITHMETIC
  case requite::Opcode::_ADD:
    return "_add";
  case requite::Opcode::_SUBTRACT:
    return "_subtract";
  case requite::Opcode::_MULTIPLY:
    return "_multiply";
  case requite::Opcode::_DIVIDE:
    return "_divide";
  case requite::Opcode::_MODULUS:
    return "_modulus";
  case requite::Opcode::_NEGATE:
    return "_negate";

  // BITWISE
  case requite::Opcode::_BITWISE_CAST:
    return "_bitwise_cast";
  case requite::Opcode::_BITWISE_OR:
    return "_bitwise_or";
  case requite::Opcode::_BITWISE_AND:
    return "_bitwise_and";
  case requite::Opcode::_BITWISE_XOR:
    return "_bitwise_xor";
  case requite::Opcode::_BITWISE_COMPLEMENT:
    return "_bitwise_complement";
  case requite::Opcode::_BITWISE_SHIFT_LEFT:
    return "_bitwise_shift_left";
  case requite::Opcode::_BITWISE_SHIFT_RIGHT:
    return "_bitwise_shift_right";
  case requite::Opcode::_BITWISE_ROTATE_LEFT:
    return "_bitwise_rotate_left";
  case requite::Opcode::_BITWISE_ROTATE_RIGHT:
    return "_bitwise_rotate_right";

  // MEMORY
  case requite::Opcode::_CONCATINATE:
    return "_concatinate";
  case requite::Opcode::FROM_FRONT:
    return "from_front";
  case requite::Opcode::_FROM_FRONT_OF_VALUE:
    return "_from_front_of_value";
  case requite::Opcode::FROM_BACK:
    return "from_back";
  case requite::Opcode::_FROM_BACK_OF_VALUE:
    return "_from_back_of_value";
  case requite::Opcode::TRUNCATE_FRONT:
    return "truncate_front";
  case requite::Opcode::_TRUNCATE_FRONT_OF_VALUE:
    return "_truncate_front_of_value";
  case requite::Opcode::TRUNCATE_BACK:
    return "truncate_back";
  case requite::Opcode::_TRUNCATE_BACK_OF_VALUE:
    return "_truncate_back_of_value";
  case requite::Opcode::AT:
    return "at";
  case requite::Opcode::_AT_OFFSET_FROM_VALUE:
    return "_at_offset_from_value";
  case requite::Opcode::ADDRESS:
    return "address";
  case requite::Opcode::_ADDRESS_OF_VALUE:
    return "_address_of_value";

  // ASSIGNMENT
  case requite::Opcode::_ASSIGN:
    return "_assign";
  case requite::Opcode::_ASSIGN_ADD:
    return "_assign_add";
  case requite::Opcode::_ASSIGN_SUBTRACT:
    return "assign_subtract";
  case requite::Opcode::_ASSIGN_MULTIPLY:
    return "_assign_multiply";
  case requite::Opcode::_ASSIGN_DIVIDE:
    return "_assign_divide";
  case requite::Opcode::_ASSIGN_MODULUS:
    return "_assign_modulus";

  // MOVE SEMANTICS
  case requite::Opcode::COPY:
    return "copy";
  case requite::Opcode::_COPY_VALUE:
    return "_copy_value";
  case requite::Opcode::MOVE:
    return "move";
  case requite::Opcode::_MOVE_VALUE:
    return "_move_value";
  case requite::Opcode::SWAP:
    return "swap";

  // SUBTYPE
  case requite::Opcode::ARRAY:
    return "array";
  case requite::Opcode::_REFERENCE:
    return "_reference";
  case requite::Opcode::_STOLEN_REFERENCE:
    return "_stolen_reference";
  case requite::Opcode::_POINTER:
    return "_pointer";
  case requite::Opcode::_FAT_POINTER:
    return "_fat_pointer";

  // TYPE MODIFIER
  case requite::Opcode::MUTABLE:
    return "MUTABLE";
  case requite::Opcode::VOLATILE:
    return "volatile";
  case requite::Opcode::ATOMIC:
    return "atomic";
  case requite::Opcode::NULL_TERMINATED:
    return "null_terminated";

  // TRIPS
  case requite::Opcode::_TUPLE_VALUE:
    return "_tuple_value";
  case requite::Opcode::_TUPLE_TYPE:
    return "_tuple_type";
  case requite::Opcode::_NULL_VALUE:
    return "_null_value";
  case requite::Opcode::_NULL_TYPE:
    return "_null_type";
  case requite::Opcode::_IGNORE:
    return "_ignore";

  // TEMPLATES
  case requite::Opcode::TEMPLATE:
    return "template";
  case requite::Opcode::_SPECIALIZATION:
    return "_specialization";

  // PROCEDURES
  case requite::Opcode::_CALL:
    return "_call";
  case requite::Opcode::_SIGNATURE:
    return "_signature";
  case requite::Opcode::_POSITIONAL_PARAMETERS_END:
    return "_positional_parameters_end";
  case requite::Opcode::_NAMED_PARAMETERS_BEGIN:
    return "_named_parameters_begin";
  case requite::Opcode::_POSITIONAL_PARAMETERS_END_AND_NAMED_PARAMETERS_BEGIN:
    return "_positional_parameters_end_and_named_parameters_begin";
  case requite::Opcode::DESTROY:
    return "destroy";
  case requite::Opcode::_DESTROY_VALUE:
    return "_destroy_value";
  case requite::Opcode::ENTRY_POINT:
    return "entry_point";
  case requite::Opcode::FUNCTION:
    return "function";
  case requite::Opcode::METHOD:
    return "method";
  case requite::Opcode::CONSTRUCTOR:
    return "constructor";
  case requite::Opcode::DESTRUCTOR:
    return "destructor";
  case requite::Opcode::_ANONYMOUS_FUNCTION:
    return "_anonymous_function";
  case requite::Opcode::_CAPTURE:
    return "_capture";

  // CONTROL FLOW
  case requite::Opcode::RETURN:
    return "return";
  case requite::Opcode::BREAK:
    return "break";
  case requite::Opcode::CONTINUE:
    return "continue";
  case requite::Opcode::FALLTHROUGH:
    return "fallthrough";
  case requite::Opcode::EXIT:
    return "exit";
  case requite::Opcode::GOTO:
    return "goto";
  case requite::Opcode::LABEL:
    return "label";

  // SYMBOLS
  case requite::Opcode::OBJECT:
    return "object";
  case requite::Opcode::TABLE:
    return "table";
  case requite::Opcode::ALIAS:
    return "alias";
  case requite::Opcode::_LOCAL:
    return "_local";
  case requite::Opcode::GLOBAL:
    return "global";
  case requite::Opcode::PROPERTY:
    return "property";
  case requite::Opcode::CONSTANT:
    return "constant";

  // VALUES
  case requite::Opcode::TEMP:
    return "temp";
  case requite::Opcode::TRUE:
    return "true";
  case requite::Opcode::FALSE:
    return "false";
  case requite::Opcode::VALUE:
    return "value";
  case requite::Opcode::INDEX:
    return "index";
  case requite::Opcode::EXCEPTION:
    return "exception";
  case requite::Opcode::_INDETERMINATE:
    return "_indeterminate";
  case requite::Opcode::NO_DEFAULT_VALUE:
    return "no_default_value";
  case requite::Opcode::INFERENCED_COUNT:
    return "inferenced_count";
  case requite::Opcode::THIS:
    return "this";
  case requite::Opcode::RESULT:
    return "result";
  case requite::Opcode::INPUT:
    return "input";
  case requite::Opcode::OUTPUT:
    return "output";
  case requite::Opcode::ADDRESS_SIZE:
    return "address_size";
  case requite::Opcode::ADDRESS_DEPTH:
    return "address_depth";
  case requite::Opcode::BITS_PER_BYTE:
    return "bits_per_byte";

  // BUILTIN TYPES
  case requite::Opcode::_INFERENCED_TYPE:
    return "_inferenced_type";
  case requite::Opcode::VOID:
    return "void";
  case requite::Opcode::BOOLEAN:
    return "boolean";
  case requite::Opcode::WORD:
    return "word";
  case requite::Opcode::SIGNED_INTEGER:
    return "signed_integer";
  case requite::Opcode::UNSIGNED_INTEGER:
    return "unsigned_integer";
  case requite::Opcode::BINARY_HALF_FLOAT:
    return "binary_half_float";
  case requite::Opcode::BINARY_SINGLE_FLOAT:
    return "binary_single_float";
  case requite::Opcode::BINARY_DOUBLE_FLOAT:
    return "binary_double_float";
  case requite::Opcode::BINARY_QUAD_FLOAT:
    return "binary_quad_float";
  case requite::Opcode::CHARACTER:
    return "character";
  case requite::Opcode::UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case requite::Opcode::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT:
    return "first_variadic_argument";
  case requite::Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE:
    return "_first_variadic_argument_of_value";
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case requite::Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE:
    return "_next_variadic_argument_of_value";

  // SCOPES
  case requite::Opcode::IF:
    return "if";
  case requite::Opcode::ELSE_IF:
    return "else_if";
  case requite::Opcode::ELSE:
    return "else";
  case requite::Opcode::SWITCH:
    return "switch";
  case requite::Opcode::CASE:
    return "case";
  case requite::Opcode::DEFAULT_CASE:
    return "default_case";
  case requite::Opcode::FOR:
    return "for";
  case requite::Opcode::WHILE:
    return "while";
  case requite::Opcode::DO_WHILE:
    return "do_while";
  case requite::Opcode::FOR_EACH:
    return "for_each";
  case requite::Opcode::LOOP:
    return "loop";
  case requite::Opcode::SCOPE:
    return "scope";
  case requite::Opcode::_VALUE_CONDUIT:
    return "_value_conduit";
  case requite::Opcode::_JUNCTION_CONDUIT:
    return "_junction_conduit";
  case requite::Opcode::_DESTINATION_CONDUIT:
    return "_destination_conduit";

  // ACCESS MODIFIERS
  case requite::Opcode::PRIVATE:
    return "private";
  case requite::Opcode::PROTECTED:
    return "protected";
  case requite::Opcode::EXPORT:
    return "export";

  // SYMBOL GRAPH
  case requite::Opcode::IMPORT:
    return "import";
  case requite::Opcode::USE:
    return "use";

  // SOURCES
  case requite::Opcode::MODULE:
    return "module";

  // ERROR HANDLING AND DEBUGGING
  case requite::Opcode::ASSERT:
    return "assert";
  case requite::Opcode::TRY:
    return "try";
  case requite::Opcode::CATCH:
    return "catch";
  case requite::Opcode::THROW:
    return "throw";
  case requite::Opcode::MAY_THROW:
    return "may_throw";
  case requite::Opcode::THROWS:
    return "throws";

  // ATTRIBUTES
  case requite::Opcode::EXTERNAL:
    return "external";
  case requite::Opcode::C:
    return "c";
  case requite::Opcode::NOT_FINAL:
    return "not_final";
  case requite::Opcode::MAY_DISCARD:
    return "may_discard";
  case requite::Opcode::NO_SHORT_CIRCUIT:
    return "no_short_circuit";
  case requite::Opcode::NO_REMAINDER:
    return "no_remainder";
  case requite::Opcode::INLINE:
    return "inline";
  case requite::Opcode::MANGLED_NAME:
    return "mangled_name";
  case requite::Opcode::_MANGLED_NAME_OF_SYMBOL:
    return "_mangled_name_of_symbol";
  case requite::Opcode::PACK:
    return "pack";

  // REFLECTED VALUES
  case requite::Opcode::SIZE:
    return "size";
  case requite::Opcode::_SIZE_OF_VALUE:
    return "_size_of_value";
  case requite::Opcode::_SIZE_OF_TYPE:
    return "_size_of_type";
  case requite::Opcode::DEPTH:
    return "depth";
  case requite::Opcode::_DEPTH_OF_VALUE:
    return "_depth_of_value";
  case requite::Opcode::_DEPTH_OF_TYPE:
    return "_depth_of_type";
  case requite::Opcode::COUNT:
    return "count";
  case requite::Opcode::_COUNT_OF_VALUE:
    return "_count_of_value";
  case requite::Opcode::_COUNT_OF_TYPE:
    return "_count_of_type";
  case requite::Opcode::NAME:
    return "name";
  case requite::Opcode::_NAME_OF_VALUE:
    return "_name_of_value";
  case requite::Opcode::_NAME_OF_SYMBOL:
    return "_name_of_symbol";
  case requite::Opcode::LINE:
    return "line";
  case requite::Opcode::_LINE_OF_VALUE:
    return "_line_of_value";
  case requite::Opcode::_LINE_OF_SYMBOL:
    return "_line_of_symbol";
  case requite::Opcode::COLUMN:
    return "column";
  case requite::Opcode::_COLUMN_OF_VALUE:
    return "_column_of_value";
  case requite::Opcode::_COLUMN_OF_SYMBOL:
    return "_column_of_symbol";
  case requite::Opcode::IS:
    return "is";
  case requite::Opcode::ARE_SAME:
    return "are_same";

    // REFLECTED SYMBOLS
  case requite::Opcode::TYPE:
    return "type";
  case requite::Opcode::_TYPE_OF_VALUE:
    return "_type_of_value";
  case requite::Opcode::UNDERLYING:
    return "underlying";
  case requite::Opcode::_UNDERLYING_OF_TYPE:
    return "_underlying_of_type";

  case requite::Opcode::__LAST:
    break;
  }
  return "__error";
}

constexpr requite::Opcode getUniversalizedValue(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::FROM_FRONT:
    return requite::Opcode::_FROM_FRONT_OF_VALUE;
  case requite::Opcode::FROM_BACK:
    return requite::Opcode::_FROM_BACK_OF_VALUE;
  case requite::Opcode::TRUNCATE_FRONT:
    return requite::Opcode::_TRUNCATE_FRONT_OF_VALUE;
  case requite::Opcode::TRUNCATE_BACK:
    return requite::Opcode::_TRUNCATE_BACK_OF_VALUE;
  case requite::Opcode::AT:
    return requite::Opcode::_AT_OFFSET_FROM_VALUE;
  case requite::Opcode::ADDRESS:
    return requite::Opcode::_ADDRESS_OF_VALUE;
  case requite::Opcode::COPY:
    return requite::Opcode::_COPY_VALUE;
  case requite::Opcode::MOVE:
    return requite::Opcode::_MOVE_VALUE;
  case requite::Opcode::DESTROY:
    return requite::Opcode::_DESTROY_VALUE;
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT:
    return requite::Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return requite::Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::SIZE:
    return requite::Opcode::_SIZE_OF_VALUE;
  case requite::Opcode::DEPTH:
    return requite::Opcode::_DEPTH_OF_VALUE;
  case requite::Opcode::COUNT:
    return requite::Opcode::_COUNT_OF_VALUE;
  case requite::Opcode::NAME:
    return requite::Opcode::_NAME_OF_VALUE;
  case requite::Opcode::LINE:
    return requite::Opcode::_LINE_OF_VALUE;
  case requite::Opcode::COLUMN:
    return requite::Opcode::_COLUMN_OF_VALUE;
  case requite::Opcode::TYPE:
    return requite::Opcode::_TYPE_OF_VALUE;
  default:
    break;
  }
  return requite::Opcode::__ERROR;
}

constexpr requite::Opcode getUniversalizedSymbol(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::MANGLED_NAME:
    return requite::Opcode::_MANGLED_NAME_OF_SYMBOL;
  case requite::Opcode::SIZE:
    return requite::Opcode::_SIZE_OF_TYPE;
  case requite::Opcode::DEPTH:
    return requite::Opcode::_DEPTH_OF_TYPE;
  case requite::Opcode::COUNT:
    return requite::Opcode::_COUNT_OF_TYPE;
  case requite::Opcode::NAME:
    return requite::Opcode::_NAME_OF_SYMBOL;
  case requite::Opcode::COLUMN:
    return requite::Opcode::_COLUMN_OF_SYMBOL;
  case requite::Opcode::IS:
    return requite::Opcode::ARE_SAME;
  case requite::Opcode::UNDERLYING:
    return requite::Opcode::_UNDERLYING_OF_TYPE;
  default:
    break;
  }
  return requite::Opcode::__NONE;
}

constexpr bool getIsValid(requite::Opcode opcode) {
  return opcode != requite::Opcode::__NONE &&
         opcode != requite::Opcode::__ERROR &&
         static_cast<unsigned>(opcode) < requite::OPCODE_COUNT;
}

constexpr bool getIsInternalUseOnly(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_INTERNAL_USE_ONLY);
  return has_flags;
}

constexpr bool getIsIntermediateOperation(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_INTERMEDIATE_OPERATION);
  return has_flags;
}

constexpr bool getIsConverging(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_CONVERGING);
  return has_flags;
}

constexpr bool getHasTextData(requite::Opcode opcode) {
  return opcode == requite::Opcode::__STRING_LITERAL ||
         opcode == requite::Opcode::__CODEUNIT_LITERAL ||
         opcode == requite::Opcode::__IDENTIFIER_LITERAL;
}

constexpr bool getHasIntegerData(requite::Opcode opcode) {
  return opcode == requite::Opcode::__INTEGER_LITERAL;
}

constexpr bool getHasScopeData(requite::Opcode opcode) {
  return opcode == requite::Opcode::IF || opcode == requite::Opcode::ELSE_IF ||
         opcode == requite::Opcode::ELSE || opcode == requite::Opcode::SWITCH ||
         opcode == requite::Opcode::CASE ||
         opcode == requite::Opcode::DEFAULT_CASE ||
         opcode == requite::Opcode::FOR || opcode == requite::Opcode::WHILE ||
         opcode == requite::Opcode::DO_WHILE ||
         opcode == requite::Opcode::FOR_EACH ||
         opcode == requite::Opcode::LOOP || opcode == requite::Opcode::SCOPE ||
         opcode == requite::Opcode::TABLE;
}

constexpr bool getHasObjectData(requite::Opcode opcode) {
  return opcode == requite::Opcode::OBJECT;
}

constexpr bool getHasOverloadData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ENTRY_POINT ||
         opcode == requite::Opcode::FUNCTION ||
         opcode == requite::Opcode::METHOD ||
         opcode == requite::Opcode::CONSTRUCTOR ||
         opcode == requite::Opcode::DESTRUCTOR;
}

constexpr bool getHasLabelData(requite::Opcode opcode) {
  return opcode == requite::Opcode::LABEL;
}

constexpr bool getHasAliasData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ALIAS;
}

constexpr bool getHasVariableData(requite::Opcode opcode) {
  return opcode == requite::Opcode::_LOCAL ||
         opcode == requite::Opcode::GLOBAL ||
         opcode == requite::Opcode::PROPERTY ||
         opcode == requite::Opcode::CONSTANT;
}

constexpr bool getHasAnonymousFunctionData(requite::Opcode opcode) {
  return opcode == requite::Opcode::_ANONYMOUS_FUNCTION;
}

} // namespace requite
