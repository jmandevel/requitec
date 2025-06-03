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
  _ROOT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(2)),
  _BASE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(3)),
  _GLOBAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(4)),
  _MATTE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(5)),
  _VALUE_REFLECTIVE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(6)),
  _SYMBOL_REFLECTIVE_LOCAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(7)),
  _OBJECT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(8)),
  _MATTE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(9)),
  _VALUE_REFLECTIVE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(10)),
  _SYMBOL_REFLECTIVE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(11)),
  _MATTE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(12)),
  _VALUE_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(13)),
  _SYMBOL_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(14)),
  _MATTE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(15)),
  _VALUE_REFLECTIVE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(16)),
  _SYMBOL_REFLECTIVE_JUNCTION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(17)),
  _MATTE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(18)),
  _VALUE_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(19)),
  _SYMBOL_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(20)),
  _ATTRIBUTE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(21)),
  _VALUE_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(22)),
  _SYMBOL_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(23)),
  _NAMED_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(24)),
  _POSITIONAL_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(25)),
  _TEMPLATE_PARAMETER =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(26)),
  _SYMBOL_NAME =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(27)),
  _SYMBOL_PATH =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(28)),
  _SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(29)),
  _LAST_SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(30))
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
  case Opcode::_NONE:
    return _INTERNAL_USE_ONLY;

  // LITERALS
  case Opcode::_INTEGER_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::_REAL_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::_STRING_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::_CODEUNIT_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::_IDENTIFIER_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL | _SYMBOL_NAME |
           _SYMBOL_PATH;

  // ERROR
  case Opcode::_ERROR:
    return _NONE;

  // SITUATIONAL
  case Opcode::SITUATIONAL_LOGICAL_AND_OR_DOUBLE_REFERENCE:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_SUBTRACT_OR_NEGATE:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_MULTIPLY_OR_POINTER:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_DIVIDE_OR_FAT_POINTER:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_MODULUS_OR_ARRAY:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_BITWISE_AND_OR_REFERENCE:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_CALL_OR_SIGNATURE:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_BIND_VALUE_OR_DEFAULT_VALUE:
    return _VALUE_BINDING | _NAMED_FIELD | _POSITIONAL_FIELD |
           _TEMPLATE_PARAMETER;
  case Opcode::SITUATIONAL_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return _SYMBOL_BINDING | _NAMED_FIELD | _TEMPLATE_PARAMETER;
  case Opcode::SITUATIONAL_TRIP:
    return _MATTE_JUNCTION | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::SITUATIONAL_CONDUIT:
    return _MATTE_JUNCTION;
  case Opcode::SITUATIONAL_INFERENCED_TYPE_OR_INDETERMINATE:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // LOGICAL
  case Opcode::LOGICAL_AND:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::LOGICAL_OR:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::LOGICAL_COMPLEMENT:
    return _MATTE_VALUE;

  // COMPAIRSON
  case Opcode::GREATER:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::GREATER_EQUAL:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::LESS:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::LESS_EQUAL:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::EQUAL:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::NOT_EQUAL:
    return _CONVERGING | _MATTE_VALUE;

  // REFLECT
  case Opcode::REFLECT_VALUE:
    return _CONVERGING | _MATTE_DESTINATION | _VALUE_REFLECTIVE_DESTINATION |
           _SYMBOL_REFLECTIVE_DESTINATION | _MATTE_VALUE |
           _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL |
           _POSITIONAL_FIELD;
  case Opcode::REFLECT_SYMBOL:
    return _CONVERGING | _MATTE_DESTINATION | _VALUE_REFLECTIVE_DESTINATION |
           _SYMBOL_REFLECTIVE_DESTINATION | _MATTE_VALUE |
           _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL |
           _POSITIONAL_FIELD | _SYMBOL_PATH;

  // MEMBERS
  case Opcode::MEMBER_VALUE_OF_VALUE:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::MEMBER_SYMBOL_OF_VALUE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::MEMBER_VALUE_OF_SYMBOL:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::MEMBER_SYMBOL_OF_SYMBOL:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // BIND
  case Opcode::BIND_VALUE:
    return _VALUE_BINDING | _TEMPLATE_PARAMETER;
  case Opcode::BIND_SYMBOL:
    return _SYMBOL_BINDING | _NAMED_FIELD | _TEMPLATE_PARAMETER;
  case Opcode::DEFAULT_VALUE:
    return _NAMED_FIELD | _POSITIONAL_FIELD | _TEMPLATE_PARAMETER;
  case Opcode::DEFAULT_SYMBOL:
    return _TEMPLATE_PARAMETER;

  // APPLY
  case Opcode::ASCRIBE:
    return _BASE_STATEMENT | _GLOBAL_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION | _MATTE_SYMBOL |
           _POSITIONAL_FIELD;
  case Opcode::CAST:
    return _MATTE_VALUE;
  case Opcode::STRINGIFY:
    return _MATTE_VALUE;
  case Opcode::CODEUNIFY:
    return _MATTE_VALUE;
  case Opcode::IDENTIFY:
    return _INTERNAL_USE_ONLY | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL | _SYMBOL_NAME |
           _SYMBOL_PATH | _POSITIONAL_FIELD;
  case Opcode::BAKE:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE |
           _SYMBOL_REFLECTIVE_VALUE | _MATTE_SYMBOL | _VALUE_REFLECTIVE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_PATH | _POSITIONAL_FIELD;

  // ARITHMETIC
  case Opcode::ADD:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::SUBTRACT:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::MULTIPLY:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::DIVIDE:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::MODULUS:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::NEGATE:
    return _MATTE_VALUE;

  // BITWISE
  case Opcode::BITWISE_CAST:
    return _MATTE_VALUE;
  case Opcode::BITWISE_OR:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::BITWISE_AND:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::BITWISE_XOR:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::BITWISE_COMPLEMENT:
    return _MATTE_VALUE;
  case Opcode::BITWISE_SHIFT_LEFT:
    return _MATTE_VALUE;
  case Opcode::BITWISE_SHIFT_RIGHT:
    return _MATTE_VALUE;
  case Opcode::BITWISE_ROTATE_LEFT:
    return _MATTE_VALUE;
  case Opcode::BITWISE_ROTATE_RIGHT:
    return _MATTE_VALUE;

  // MEMORY
  case Opcode::CONCATINATE:
    return _CONVERGING | _MATTE_VALUE;
  case Opcode::FROM_FRONT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::FROM_FRONT_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::FROM_BACK:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::FROM_BACK_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::TRUNCATE_FRONT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::TRUNCATE_FRONT_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::TRUNCATE_BACK:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::TRUNCATE_BACK_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::AT:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::AT_OFFSET_FROM_VALUE:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::DEREFERENCE:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::DEREFERNECE_VALUE:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;

  // ASSIGNMENT
  case Opcode::ASSIGN:
    return _CONVERGING | _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ASSIGN_ADD:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ASSIGN_SUBTRACT:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ASSIGN_MULTIPLY:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ASSIGN_DIVIDE:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::ASSIGN_MODULUS:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;

  // MOVE SEMANTICS
  case Opcode::COPY:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::COPY_VALUE:
    return _MATTE_VALUE;
  case Opcode::MOVE:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::MOVE_VALUE:
    return _MATTE_VALUE;
  case Opcode::SWAP:
    return _MATTE_LOCAL_STATEMENT;

  // SUBTYPE
  case Opcode::ARRAY:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::REFERENCE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::STOLEN_REFERENCE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::POINTER:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::FAT_POINTER:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;

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
  case Opcode::TUPLE:
    return _MATTE_VALUE;
  case Opcode::ANONYMOUS_OBJECT:
    return _MATTE_VALUE;
  case Opcode::NULL_:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::IGNORE:
    return _MATTE_DESTINATION;

  // TEMPLATES
  case Opcode::TEMPLATE:
    return _ATTRIBUTE;
  case Opcode::SPECIALIZATION:
    return _MATTE_VALUE | _MATTE_SYMBOL | _POSITIONAL_FIELD;

  // PROCEDURES
  case Opcode::NO_PARAMETER_SIGNATURE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::NAMED_PARAMETER_SIGNATURE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::POSITIONAL_PARAMETER_SIGNATURE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
  case Opcode::NO_ARGUMENT_CALL:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::NAMED_ARGUMENT_CALL:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_LOCAL_STATEMENT;
  case Opcode::POSITIONAL_ARGUMENT_CALL:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::DESTROY:
    return _VALUE_REFLECTIVE_LOCAL_STATEMENT;
  case Opcode::DESTROY_VALUE:
    return _MATTE_LOCAL_STATEMENT;
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
  case Opcode::ANONYMOUS_FUNCTION:
    return _MATTE_VALUE;
  case Opcode::CAPTURE:
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
  case Opcode::LOCAL:
    return _MATTE_LOCAL_STATEMENT;
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
  case Opcode::INDETERMINATE:
    return _MATTE_VALUE;
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
  case Opcode::INFERENCED_TYPE:
    return _MATTE_SYMBOL | _POSITIONAL_FIELD;
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
  case Opcode::FIRST_VARIADIC_ARGUMENT_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::NEXT_VARIADIC_ARGUMENT:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::NEXT_VARIADIC_ARGUMENT_OF_VALUE:
    return _MATTE_VALUE;

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
  case Opcode::VALUE_CONDUIT:
    return _MATTE_VALUE;
  case Opcode::JUNCTION_CONDUIT:
    return _MATTE_JUNCTION;
  case Opcode::DESTINATION_CONDUIT:
    return _MATTE_DESTINATION;

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
  case Opcode::POSITIONAL_ARGUMENTS:
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
  case Opcode::MANGLED_NAME_OF_SYMBOL:
    return _MATTE_VALUE;
  case Opcode::PACK:
    return _ATTRIBUTE;

  // REFLECTED VALUES
  case Opcode::SIZE:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::SIZE_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::SIZE_OF_TYPE:
    return _MATTE_VALUE;
  case Opcode::DEPTH:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::DEPTH_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::DEPTH_OF_TYPE:
    return _MATTE_VALUE;
  case Opcode::COUNT:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::COUNT_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::COUNT_OF_TYPE:
    return _MATTE_VALUE;
  case Opcode::NAME:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::NAME_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::NAME_OF_SYMBOL:
    return _MATTE_VALUE;
  case Opcode::LINE:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::LINE_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::LINE_OF_SYMBOL:
    return _MATTE_VALUE;
  case Opcode::COLUMN:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::COLUMN_OF_VALUE:
    return _MATTE_VALUE;
  case Opcode::COLUMN_OF_SYMBOL:
    return _MATTE_VALUE;
  case Opcode::IS:
    return _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::ARE_SAME:
    return _MATTE_VALUE;

  // REFLECTED SYMBOLS
  case Opcode::TYPE:
    return _VALUE_REFLECTIVE_SYMBOL;
  case Opcode::TYPE_OF_VALUE:
    return _MATTE_SYMBOL;
  case Opcode::UNDERLYING:
    return _SYMBOL_REFLECTIVE_SYMBOL;
  case Opcode::UNDERLYING_OF_TYPE:
    return _MATTE_SYMBOL;

  case Opcode::_LAST:
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
  case requite::Opcode::_NONE:
    return "_none";

  // LITERALS
  case requite::Opcode::_INTEGER_LITERAL:
    return "_integer_literal";
  case requite::Opcode::_REAL_LITERAL:
    return "_real_literal";
  case requite::Opcode::_STRING_LITERAL:
    return "_string_literal";
  case requite::Opcode::_CODEUNIT_LITERAL:
    return "_codeunit_literal";
  case requite::Opcode::_IDENTIFIER_LITERAL:
    return "_identifier_literal";

  // ERRORS
  case requite::Opcode::_ERROR:
    return "_error";

  // SITUATIONAL
  case requite::Opcode::SITUATIONAL_LOGICAL_AND_OR_DOUBLE_REFERENCE:
    return "situational_logical_and_or_double_reference";
  case requite::Opcode::SITUATIONAL_SUBTRACT_OR_NEGATE:
    return "situational_subtract_or_negate";
  case requite::Opcode::SITUATIONAL_MULTIPLY_OR_POINTER:
    return "situational_multiply_or_pointer";
  case requite::Opcode::SITUATIONAL_DIVIDE_OR_FAT_POINTER:
    return "situational_divide_or_fat_pointer";
  case requite::Opcode::SITUATIONAL_MODULUS_OR_ARRAY:
    return "situational_modulus_or_array";
  case requite::Opcode::SITUATIONAL_BITWISE_AND_OR_REFERENCE:
    return "situational_bitwise_and_or_reference";
  case requite::Opcode::SITUATIONAL_CALL_OR_SIGNATURE:
    return "situational_call_or_signature";
  case requite::Opcode::SITUATIONAL_BIND_VALUE_OR_DEFAULT_VALUE:
    return "situational_bind_value_or_default_value";
  case requite::Opcode::SITUATIONAL_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return "situational_bind_symbol_or_default_symbol";
  case requite::Opcode::SITUATIONAL_TRIP:
    return "situational_trip";
  case requite::Opcode::SITUATIONAL_CONDUIT:
    return "situational_conduit";
  case requite::Opcode::SITUATIONAL_INFERENCED_TYPE_OR_INDETERMINATE:
    return "situational_inferenced_type_or_indeterminate";

  // LOGICAL
  case requite::Opcode::LOGICAL_AND:
    return "logical_and";
  case requite::Opcode::LOGICAL_OR:
    return "logical_or";
  case requite::Opcode::LOGICAL_COMPLEMENT:
    return "logical_complement";

  // COMPARISON
  case requite::Opcode::GREATER:
    return "greater";
  case requite::Opcode::GREATER_EQUAL:
    return "greater_equal";
  case requite::Opcode::LESS:
    return "less";
  case requite::Opcode::LESS_EQUAL:
    return "less_equal";
  case requite::Opcode::EQUAL:
    return "equal";
  case requite::Opcode::NOT_EQUAL:
    return "not_equal";

  // REFLECT
  case requite::Opcode::REFLECT_VALUE:
    return "reflect_value";
  case requite::Opcode::REFLECT_SYMBOL:
    return "reflect_symbol";

  // MEMBERS
  case requite::Opcode::MEMBER_VALUE_OF_VALUE:
    return "member_value_of_value";
  case requite::Opcode::MEMBER_SYMBOL_OF_VALUE:
    return "member_symbol_of_value";
  case requite::Opcode::MEMBER_VALUE_OF_SYMBOL:
    return "member_value_of_symbol";
  case requite::Opcode::MEMBER_SYMBOL_OF_SYMBOL:
    return "member_symbol_of_symbol";

  // BIND
  case requite::Opcode::BIND_VALUE:
    return "bind_value";
  case requite::Opcode::BIND_SYMBOL:
    return "bind_symbol";
  case requite::Opcode::DEFAULT_VALUE:
    return "default_value";
  case requite::Opcode::DEFAULT_SYMBOL:
    return "default_symbol";

  // APPLY
  case requite::Opcode::ASCRIBE:
    return "ascribe";
  case requite::Opcode::CAST:
    return "cast";
  case requite::Opcode::STRINGIFY:
    return "stringify";
  case requite::Opcode::CODEUNIFY:
    return "codeunify";
  case requite::Opcode::IDENTIFY:
    return "identify";
  case requite::Opcode::BAKE:
    return "bake";

  // ARITHMETIC
  case requite::Opcode::ADD:
    return "add";
  case requite::Opcode::SUBTRACT:
    return "subtract";
  case requite::Opcode::MULTIPLY:
    return "multiply";
  case requite::Opcode::DIVIDE:
    return "divide";
  case requite::Opcode::MODULUS:
    return "modulus";
  case requite::Opcode::NEGATE:
    return "negate";

  // BITWISE
  case requite::Opcode::BITWISE_CAST:
    return "bitwise_cast";
  case requite::Opcode::BITWISE_OR:
    return "bitwise_or";
  case requite::Opcode::BITWISE_AND:
    return "bitwise_and";
  case requite::Opcode::BITWISE_XOR:
    return "bitwise_xor";
  case requite::Opcode::BITWISE_COMPLEMENT:
    return "bitwise_complement";
  case requite::Opcode::BITWISE_SHIFT_LEFT:
    return "bitwise_shift_left";
  case requite::Opcode::BITWISE_SHIFT_RIGHT:
    return "bitwise_shift_right";
  case requite::Opcode::BITWISE_ROTATE_LEFT:
    return "bitwise_rotate_left";
  case requite::Opcode::BITWISE_ROTATE_RIGHT:
    return "bitwise_rotate_right";

  // MEMORY
  case requite::Opcode::CONCATINATE:
    return "concatinate";
  case requite::Opcode::FROM_FRONT:
    return "from_front";
  case requite::Opcode::FROM_FRONT_OF_VALUE:
    return "from_front_of_value";
  case requite::Opcode::FROM_BACK:
    return "from_back";
  case requite::Opcode::FROM_BACK_OF_VALUE:
    return "from_back_of_value";
  case requite::Opcode::TRUNCATE_FRONT:
    return "truncate_front";
  case requite::Opcode::TRUNCATE_FRONT_OF_VALUE:
    return "truncate_front_of_value";
  case requite::Opcode::TRUNCATE_BACK:
    return "truncate_back";
  case requite::Opcode::TRUNCATE_BACK_OF_VALUE:
    return "truncate_back_of_value";
  case requite::Opcode::AT:
    return "at";
  case requite::Opcode::AT_OFFSET_FROM_VALUE:
    return "at_offset_from_value";
  case requite::Opcode::DEREFERENCE:
    return "dereference";
  case requite::Opcode::DEREFERNECE_VALUE:
    return "dereference_value";

  // ASSIGNMENT
  case requite::Opcode::ASSIGN:
    return "assign";
  case requite::Opcode::ASSIGN_ADD:
    return "assign_add";
  case requite::Opcode::ASSIGN_SUBTRACT:
    return "assign_subtract";
  case requite::Opcode::ASSIGN_MULTIPLY:
    return "assign_multiply";
  case requite::Opcode::ASSIGN_DIVIDE:
    return "assign_divide";
  case requite::Opcode::ASSIGN_MODULUS:
    return "assign_modulus";

  // MOVE SEMANTICS
  case requite::Opcode::COPY:
    return "copy";
  case requite::Opcode::COPY_VALUE:
    return "copy_value";
  case requite::Opcode::MOVE:
    return "move";
  case requite::Opcode::MOVE_VALUE:
    return "move_value";
  case requite::Opcode::SWAP:
    return "swap";

  // SUBTYPE
  case requite::Opcode::ARRAY:
    return "array";
  case requite::Opcode::REFERENCE:
    return "reference";
  case requite::Opcode::STOLEN_REFERENCE:
    return "stolen_reference";
  case requite::Opcode::POINTER:
    return "pointer";
  case requite::Opcode::FAT_POINTER:
    return "fat_pointer";

  // TYPE MODIFIER
  case requite::Opcode::MUTABLE:
    return "mutable";
  case requite::Opcode::VOLATILE:
    return "volatile";
  case requite::Opcode::ATOMIC:
    return "atomic";
  case requite::Opcode::NULL_TERMINATED:
    return "null_terminated";

  // TRIPS
  case requite::Opcode::TUPLE:
    return "tuple";
  case requite::Opcode::ANONYMOUS_OBJECT:
    return "anonymous_object";
  case requite::Opcode::NULL_:
    return "null";
  case requite::Opcode::IGNORE:
    return "ignore";

  // TEMPLATES
  case requite::Opcode::TEMPLATE:
    return "template";
  case requite::Opcode::SPECIALIZATION:
    return "specialization";

  // PROCEDURES
  case requite::Opcode::NO_PARAMETER_SIGNATURE:
    return "no_parameter_signature";
  case requite::Opcode::NAMED_PARAMETER_SIGNATURE:
    return "named_parameter_signature";
  case requite::Opcode::POSITIONAL_PARAMETER_SIGNATURE:
    return "positional_parameter_signature";
  case requite::Opcode::NO_ARGUMENT_CALL:
    return "no_argument_call";
  case requite::Opcode::NAMED_ARGUMENT_CALL:
    return "named_argument_call";
  case requite::Opcode::POSITIONAL_ARGUMENT_CALL:
    return "positional_argument_call";
  case requite::Opcode::DESTROY:
    return "destroy";
  case requite::Opcode::DESTROY_VALUE:
    return "destroy_value";
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
  case requite::Opcode::ANONYMOUS_FUNCTION:
    return "anonymous_function";
  case requite::Opcode::CAPTURE:
    return "capture";

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
  case requite::Opcode::LOCAL:
    return "local";
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
  case requite::Opcode::INDETERMINATE:
    return "indeterminate";
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
  case requite::Opcode::INFERENCED_TYPE:
    return "inferenced_type";
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
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT_OF_VALUE:
    return "first_variadic_argument_of_value";
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT_OF_VALUE:
    return "next_variadic_argument_of_value";

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
  case requite::Opcode::VALUE_CONDUIT:
    return "value_conduit";
  case requite::Opcode::JUNCTION_CONDUIT:
    return "junction_conduit";
  case requite::Opcode::DESTINATION_CONDUIT:
    return "destination_conduit";

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
  case requite::Opcode::POSITIONAL_ARGUMENTS:
    return "positional_arguments";
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
  case requite::Opcode::MANGLED_NAME_OF_SYMBOL:
    return "mangled_name_of_symbol";
  case requite::Opcode::PACK:
    return "pack";

  // REFLECTED VALUES
  case requite::Opcode::SIZE:
    return "size";
  case requite::Opcode::SIZE_OF_VALUE:
    return "size_of_value";
  case requite::Opcode::SIZE_OF_TYPE:
    return "size_of_type";
  case requite::Opcode::DEPTH:
    return "depth";
  case requite::Opcode::DEPTH_OF_VALUE:
    return "depth_of_value";
  case requite::Opcode::DEPTH_OF_TYPE:
    return "depth_of_type";
  case requite::Opcode::COUNT:
    return "count";
  case requite::Opcode::COUNT_OF_VALUE:
    return "count_of_value";
  case requite::Opcode::COUNT_OF_TYPE:
    return "count_of_type";
  case requite::Opcode::NAME:
    return "name";
  case requite::Opcode::NAME_OF_VALUE:
    return "name_of_value";
  case requite::Opcode::NAME_OF_SYMBOL:
    return "name_of_symbol";
  case requite::Opcode::LINE:
    return "line";
  case requite::Opcode::LINE_OF_VALUE:
    return "line_of_value";
  case requite::Opcode::LINE_OF_SYMBOL:
    return "line_of_symbol";
  case requite::Opcode::COLUMN:
    return "column";
  case requite::Opcode::COLUMN_OF_VALUE:
    return "column_of_value";
  case requite::Opcode::COLUMN_OF_SYMBOL:
    return "column_of_symbol";
  case requite::Opcode::IS:
    return "is";
  case requite::Opcode::ARE_SAME:
    return "are_same";

    // REFLECTED SYMBOLS
  case requite::Opcode::TYPE:
    return "type";
  case requite::Opcode::TYPE_OF_VALUE:
    return "type_of_value";
  case requite::Opcode::UNDERLYING:
    return "underlying";
  case requite::Opcode::UNDERLYING_OF_TYPE:
    return "underlying_of_type";

  case requite::Opcode::_LAST:
    break;
  }
  return "_error";
}

constexpr requite::Opcode getUniversalizedValue(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::FROM_FRONT:
    return requite::Opcode::FROM_FRONT_OF_VALUE;
  case requite::Opcode::FROM_BACK:
    return requite::Opcode::FROM_BACK_OF_VALUE;
  case requite::Opcode::TRUNCATE_FRONT:
    return requite::Opcode::TRUNCATE_FRONT_OF_VALUE;
  case requite::Opcode::TRUNCATE_BACK:
    return requite::Opcode::TRUNCATE_BACK_OF_VALUE;
  case requite::Opcode::AT:
    return requite::Opcode::AT_OFFSET_FROM_VALUE;
  case requite::Opcode::DEREFERENCE:
    return requite::Opcode::DEREFERNECE_VALUE;
  case requite::Opcode::COPY:
    return requite::Opcode::COPY_VALUE;
  case requite::Opcode::MOVE:
    return requite::Opcode::MOVE_VALUE;
  case requite::Opcode::DESTROY:
    return requite::Opcode::DESTROY_VALUE;
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT:
    return requite::Opcode::FIRST_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return requite::Opcode::NEXT_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::SIZE:
    return requite::Opcode::SIZE_OF_VALUE;
  case requite::Opcode::DEPTH:
    return requite::Opcode::DEPTH_OF_VALUE;
  case requite::Opcode::COUNT:
    return requite::Opcode::COUNT_OF_VALUE;
  case requite::Opcode::NAME:
    return requite::Opcode::NAME_OF_VALUE;
  case requite::Opcode::LINE:
    return requite::Opcode::LINE_OF_VALUE;
  case requite::Opcode::COLUMN:
    return requite::Opcode::COLUMN_OF_VALUE;
  case requite::Opcode::TYPE:
    return requite::Opcode::TYPE_OF_VALUE;
  default:
    break;
  }
  return requite::Opcode::_ERROR;
}

constexpr requite::Opcode getUniversalizedSymbol(requite::Opcode opcode) {
  switch (opcode) {
    case requite::Opcode::MANGLED_NAME:
      return requite::Opcode::MANGLED_NAME_OF_SYMBOL;
    case requite::Opcode::SIZE:
      return requite::Opcode::SIZE_OF_TYPE;
    case requite::Opcode::DEPTH:
      return requite::Opcode::DEPTH_OF_TYPE;
    case requite::Opcode::COUNT:
      return requite::Opcode::COUNT_OF_TYPE;
    case requite::Opcode::NAME:
      return requite::Opcode::NAME_OF_SYMBOL;
    case requite::Opcode::COLUMN:
      return requite::Opcode::COLUMN_OF_SYMBOL;
    case requite::Opcode::IS:
      return requite::Opcode::ARE_SAME;
    case requite::Opcode::UNDERLYING:
      return requite::Opcode::UNDERLYING_OF_TYPE;
    default:
      break;
  }
  return requite::Opcode::_NONE;
}

constexpr bool getIsValid(requite::Opcode opcode) {
  return opcode != requite::Opcode::_NONE &&
         opcode != requite::Opcode::_ERROR &&
         static_cast<unsigned>(opcode) < requite::OPCODE_COUNT;
}

constexpr bool getIsInternalUseOnly(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_INTERNAL_USE_ONLY);
  return has_flags;
}

constexpr bool getIsConverging(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_opcode::_CONVERGING);
  return has_flags;
}

constexpr bool getHasTextData(requite::Opcode opcode) {
  return opcode == requite::Opcode::_STRING_LITERAL ||
         opcode == requite::Opcode::_CODEUNIT_LITERAL ||
         opcode == requite::Opcode::_IDENTIFIER_LITERAL;
}

constexpr bool getHasIntegerData(requite::Opcode opcode) {
  return opcode == requite::Opcode::_INTEGER_LITERAL;
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
  return opcode == requite::Opcode::LOCAL ||
         opcode == requite::Opcode::GLOBAL ||
         opcode == requite::Opcode::PROPERTY ||
         opcode == requite::Opcode::CONSTANT;
}

constexpr bool getHasAnonymousFunctionData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ANONYMOUS_FUNCTION;
}

} // namespace requite
