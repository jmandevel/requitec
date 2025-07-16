// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/assert.hpp>
#include <requite/unreachable.hpp>
#include <requite/utility.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>
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
  _CONVERGING = requite::getBit(0),
  _INTERNAL_USE_ONLY = requite::getBit(1),
  _INTERMEDIATE_OPERATION = requite::getBit(2),
  _MODULE_STATEMENT = requite::getBit(3),
  _TABLE_STATEMENT = requite::getBit(4),
  _MATTE_LOCAL_STATEMENT = requite::getBit(5),
  _VALUE_REFLECTIVE_LOCAL_STATEMENT = requite::getBit(6),
  _SYMBOL_REFLECTIVE_LOCAL_STATEMENT = requite::getBit(7),
  _OBJECT_STATEMENT = requite::getBit(8),
  _MATTE_DESTINATION = requite::getBit(9),
  _VALUE_REFLECTIVE_DESTINATION = requite::getBit(10),
  _SYMBOL_REFLECTIVE_DESTINATION = requite::getBit(11),
  _MATTE_VALUE = requite::getBit(12),
  _VALUE_REFLECTIVE_VALUE = requite::getBit(13),
  _SYMBOL_REFLECTIVE_VALUE = requite::getBit(14),
  _MATTE_JUNCTION = requite::getBit(15),
  _VALUE_REFLECTIVE_JUNCTION = requite::getBit(16),
  _SYMBOL_REFLECTIVE_JUNCTION = requite::getBit(17),
  _MATTE_SYMBOL = requite::getBit(18),
  _VALUE_REFLECTIVE_SYMBOL = requite::getBit(19),
  _SYMBOL_REFLECTIVE_SYMBOL = requite::getBit(20),
  _ANY = _MODULE_STATEMENT | _TABLE_STATEMENT | _MATTE_LOCAL_STATEMENT |
         _VALUE_REFLECTIVE_LOCAL_STATEMENT |
         _SYMBOL_REFLECTIVE_LOCAL_STATEMENT | _OBJECT_STATEMENT |
         _MATTE_DESTINATION | _VALUE_REFLECTIVE_DESTINATION |
         _SYMBOL_REFLECTIVE_DESTINATION | _MATTE_VALUE |
         _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE | _MATTE_JUNCTION |
         _VALUE_REFLECTIVE_JUNCTION | _SYMBOL_REFLECTIVE_JUNCTION |
         _MATTE_SYMBOL | _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL
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

  // HANDLES
  case Opcode::__LOCAL_HANDLE:
    return _INTERNAL_USE_ONLY | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL;

  // LITERALS
  case Opcode::__INTEGER_LITERAL:
    return _INTERNAL_USE_ONLY | _MATTE_VALUE;
  case Opcode::__FRACTIONAL_LITERAL:
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
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL;

  // ERROR
  case Opcode::__ERROR:
    return _NONE;

  // SITUATIONAL
  case Opcode::_CALL_OR_SIGNATURE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_DESTINATION |
           _MATTE_VALUE | _MATTE_SYMBOL | _MATTE_LOCAL_STATEMENT;
  case Opcode::_BIND_VALUE_OR_DEFAULT_VALUE:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_TRIP:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_SYMBOL;

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
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL;
  case Opcode::_REFLECT_SYMBOL:
    return _CONVERGING | _INTERMEDIATE_OPERATION | _MATTE_DESTINATION |
           _VALUE_REFLECTIVE_DESTINATION | _SYMBOL_REFLECTIVE_DESTINATION |
           _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE |
           _MATTE_JUNCTION | _VALUE_REFLECTIVE_JUNCTION |
           _SYMBOL_REFLECTIVE_JUNCTION | _MATTE_SYMBOL |
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL;

  // MEMBERS
  case Opcode::_MEMBER_VALUE_OF_VALUE_PATH:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::_MEMBER_SYMBOL_OF_VALUE_PATH:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_MEMBER_VALUE_OF_SYMBOL_PATH:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::_MEMBER_SYMBOL_OF_SYMBOL_PATH:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;

  // EXTENSIONS
  case Opcode::_EXTEND:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;

  // BIND
  case Opcode::_BIND_VALUE:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_BIND_SYMBOL:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_DEFAULT_VALUE:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_DEFAULT_SYMBOL:
    return _INTERMEDIATE_OPERATION;

  // APPLY
  case Opcode::_ASCRIBE_LAST_BRANCH:
    return _INTERMEDIATE_OPERATION | _MODULE_STATEMENT | _TABLE_STATEMENT |
           _OBJECT_STATEMENT | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_SYMBOL;
  case Opcode::_ASCRIBE_FIRST_BRANCH:
    return _INTERMEDIATE_OPERATION | _MODULE_STATEMENT | _TABLE_STATEMENT |
           _OBJECT_STATEMENT | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION | _MATTE_SYMBOL;
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
           _VALUE_REFLECTIVE_SYMBOL | _SYMBOL_REFLECTIVE_SYMBOL;

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
  case Opcode::_COMPILE_TIME_CONCATINATE:
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
  case Opcode::_AT_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::ADDRESS:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::_ADDRESS_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_VALUE |
           _MATTE_JUNCTION;
  case Opcode::STEAL:
    return _VALUE_REFLECTIVE_DESTINATION | _VALUE_REFLECTIVE_VALUE |
           _VALUE_REFLECTIVE_JUNCTION;
  case Opcode::_STEAL_VALUE:
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
  case Opcode::_ARRAY:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_REFERENCE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_POINTER:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_FAT_POINTER:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_VARIANT:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;

  // TYPE MODIFIER
  case Opcode::MUTABLE:
    return _MATTE_VALUE;
  case Opcode::CONSTANT:
    return _MATTE_VALUE;
  case Opcode::VOLATILE:
    return _MATTE_VALUE;
  case Opcode::ATOMIC:
    return _MATTE_VALUE;
  case Opcode::NULL_TERMINATED:
    return _MATTE_VALUE;
  case Opcode::OWNING:
    return _MATTE_VALUE;

  // FIELD RULES
  case Opcode::_POSITIONAL_FIELDS_END:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_NAMED_FIELDS_BEGIN:
    return _INTERMEDIATE_OPERATION;
  case Opcode::_POSITIONAL_FIELDS_END_AND_NAMED_FIELDS_BEGIN:
    return _INTERMEDIATE_OPERATION;

  // TRIPS
  case Opcode::_TUPLE_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_TUPLE_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_NULL_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_NULL_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::_STRUCTURED_BINDING:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::_IGNORE:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;

  // STATIC POLYMORPHISM
  case Opcode::TEMPLATE:
    return _MATTE_VALUE;
  case Opcode::_SPECIALIZATION:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE | _MATTE_SYMBOL;
  case Opcode::BAKE:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_BAKE_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;

  // PROCEDURES
  case Opcode::_CALL:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_DESTINATION |
           _MATTE_VALUE | _MATTE_LOCAL_STATEMENT;
  case Opcode::_SIGNATURE:
    return _INTERMEDIATE_OPERATION | _MATTE_SYMBOL;
  case Opcode::DESTROY:
    return _VALUE_REFLECTIVE_LOCAL_STATEMENT;
  case Opcode::_DESTROY_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::DROP:
    return _VALUE_REFLECTIVE_LOCAL_STATEMENT;
  case Opcode::_DROP_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::ENTRY_POINT:
    return _MODULE_STATEMENT;
  case Opcode::FUNCTION:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::METHOD:
    return _OBJECT_STATEMENT;
  case Opcode::EXTENSION:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::CONSTRUCTOR:
    return _OBJECT_STATEMENT;
  case Opcode::DESTRUCTOR:
    return _OBJECT_STATEMENT;
  case Opcode::_ANONYMOUS_FUNCTION:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::_CAPTURE:
    return _NONE;

  // CONTROL FLOW
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
  case Opcode::LABEL:
    return _MATTE_VALUE;

  // SYMBOLS
  case Opcode::OBJECT:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::TABLE:
    return _MODULE_STATEMENT | _TABLE_STATEMENT;
  case Opcode::ALIAS:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::_LOCAL:
    return _INTERMEDIATE_OPERATION | _MATTE_LOCAL_STATEMENT;
  case Opcode::GLOBAL:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::PROPERTY:
    return _OBJECT_STATEMENT;

  // VALUES
  case Opcode::TRUE:
    return _MATTE_VALUE;
  case Opcode::FALSE:
    return _MATTE_VALUE;
  case Opcode::VALUE:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::INDEX:
    return _MATTE_VALUE;
  case Opcode::INDETERMINATE:
    return _MATTE_VALUE;
  case Opcode::NO_DEFAULT_VALUE:
    return _MATTE_VALUE;
  case Opcode::_TACIT_COUNT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::THIS:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::RESULT:
    return _MATTE_DESTINATION | _MATTE_VALUE | _MATTE_JUNCTION;
  case Opcode::ARGUMENT:
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
  case Opcode::TACIT:
    return _MATTE_SYMBOL;
  case Opcode::VOID:
    return _MATTE_SYMBOL;
  case Opcode::BOOLEAN:
    return _MATTE_SYMBOL;
  case Opcode::WORD:
    return _MATTE_SYMBOL;
  case Opcode::SIGNED:
    return _MATTE_SYMBOL;
  case Opcode::UNSIGNED:
    return _MATTE_SYMBOL;
  case Opcode::BFLOAT16:
    return _MATTE_SYMBOL;
  case Opcode::BINARY16:
    return _MATTE_SYMBOL;
  case Opcode::BINARY32:
    return _MATTE_SYMBOL;
  case Opcode::BINARY64:
    return _MATTE_SYMBOL;
  case Opcode::BINARY128:
    return _MATTE_SYMBOL;
  case Opcode::UTF8:
    return _MATTE_SYMBOL;

  // VARIADIC ARGUMENTS
  case Opcode::VARIADIC_ARGUMENTS:
    return _MATTE_SYMBOL;
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
    return _NONE;
  case Opcode::DEFAULT_CASE:
    return _NONE;
  case Opcode::FOR:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::WHILE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::DO_WHILE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::LOOP:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::SCOPE:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::_OPEN_INLINE_SCOPE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_JUNCTION |
           _MATTE_VALUE;
  case Opcode::_CLOSED_INLINE_SCOPE:
    return _INTERMEDIATE_OPERATION | _MATTE_DESTINATION | _MATTE_JUNCTION |
           _MATTE_VALUE;

  // ACCESS MODIFIERS
  case Opcode::PRIVATE:
    return _MATTE_VALUE;
  case Opcode::PROTECTED:
    return _MATTE_VALUE;
  case Opcode::EXPORT:
    return _MATTE_VALUE;

  // SYMBOL GRAPH
  case Opcode::IMPORT:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;
  case Opcode::USE:
    return _MODULE_STATEMENT | _TABLE_STATEMENT | _OBJECT_STATEMENT |
           _MATTE_LOCAL_STATEMENT;

  // SOURCES
  case Opcode::MODULE:
    return _NONE;

  // ERROR HANDLING AND DEBUGGING
  case Opcode::ASSERT:
    return _MATTE_LOCAL_STATEMENT;
  case Opcode::UNREACHABLE:
    return _MATTE_LOCAL_STATEMENT;

  // ATTRIBUTES
  case Opcode::NOT_FINAL:
    return _MATTE_VALUE;
  case Opcode::MAY_DISCARD:
    return _MATTE_VALUE;
  case Opcode::INLINE:
    return _MATTE_VALUE;
  case Opcode::MANGLED_NAME:
    return _SYMBOL_REFLECTIVE_VALUE | _MATTE_VALUE;
  case Opcode::_MANGLED_NAME_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::PACK:
    return _MATTE_VALUE;

  // REFLECTED VALUES
  case Opcode::SIZE:
    return _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_SIZE_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::DEPTH:
    return _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_DEPTH_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::COUNT:
    return _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_COUNT_OF_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::LENGTH:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_LENGTH_OF_VALUE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::NAME:
    return _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_NAME_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::LINE:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_LINE_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::COLUMN:
    return _MATTE_VALUE | _VALUE_REFLECTIVE_VALUE | _SYMBOL_REFLECTIVE_VALUE;
  case Opcode::_COLUMN_OF_SYMBOL:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::IS:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_VALUE_OF_VARIANT_IS_TYPE:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
  case Opcode::GET:
    return _VALUE_REFLECTIVE_VALUE;
  case Opcode::_GET_VALUE_OF_VARIANT:
    return _INTERMEDIATE_OPERATION | _MATTE_VALUE;
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

  // HANDLES
  case requite::Opcode::__LOCAL_HANDLE:
    return "__local_handle";

  // LITERALS
  case requite::Opcode::__INTEGER_LITERAL:
    return "__integer_literal";
  case requite::Opcode::__FRACTIONAL_LITERAL:
    return "__fractional_literal";
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
  case requite::Opcode::_MEMBER_VALUE_OF_VALUE_PATH:
    return "_member_value_of_value_path";
  case requite::Opcode::_MEMBER_SYMBOL_OF_VALUE_PATH:
    return "_member_symbol_of_value_path";
  case requite::Opcode::_MEMBER_VALUE_OF_SYMBOL_PATH:
    return "_member_value_of_symbol_path";
  case requite::Opcode::_MEMBER_SYMBOL_OF_SYMBOL_PATH:
    return "_member_symbol_of_symbol_path";

  // EXTENSIONS
  case requite::Opcode::_EXTEND:
    return "_extend";

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
  case requite::Opcode::_ASCRIBE_LAST_BRANCH:
    return "_ascribe_last_branch";
  case requite::Opcode::_ASCRIBE_FIRST_BRANCH:
    return "_ascribe_first_branch";
  case requite::Opcode::_CAST:
    return "_cast";
  case requite::Opcode::STRINGIFY:
    return "stringify";
  case requite::Opcode::CODEUNIFY:
    return "codeunify";
  case requite::Opcode::_IDENTIFY:
    return "_identify";

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
  case requite::Opcode::_COMPILE_TIME_CONCATINATE:
    return "_compile_time_concatinate";
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
  case requite::Opcode::_AT_VALUE:
    return "_at_value";
  case requite::Opcode::ADDRESS:
    return "address";
  case requite::Opcode::_ADDRESS_VALUE:
    return "_address_value";

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
  case requite::Opcode::STEAL:
    return "steal";
  case requite::Opcode::_STEAL_VALUE:
    return "_steal_value";
  case requite::Opcode::SWAP:
    return "swap";

  // SUBTYPE
  case requite::Opcode::_ARRAY:
    return "_array";
  case requite::Opcode::_REFERENCE:
    return "_reference";
  case requite::Opcode::_POINTER:
    return "_pointer";
  case requite::Opcode::_FAT_POINTER:
    return "_fat_pointer";
  case requite::Opcode::_VARIANT:
    return "_variant";

  // TYPE MODIFIER
  case requite::Opcode::MUTABLE:
    return "mutable";
  case requite::Opcode::CONSTANT:
    return "constant";
  case requite::Opcode::VOLATILE:
    return "volatile";
  case requite::Opcode::ATOMIC:
    return "atomic";
  case requite::Opcode::NULL_TERMINATED:
    return "null_terminated";
  case requite::Opcode::OWNING:
    return "owning";

  // FIELD RULES
  case requite::Opcode::_POSITIONAL_FIELDS_END:
    return "_positional_fields_end";
  case requite::Opcode::_NAMED_FIELDS_BEGIN:
    return "_named_fields_begin";
  case requite::Opcode::_POSITIONAL_FIELDS_END_AND_NAMED_FIELDS_BEGIN:
    return "_positional_fields_end_and_named_fields_begin";

  // TRIPS
  case requite::Opcode::_TUPLE_VALUE:
    return "_tuple_value";
  case requite::Opcode::_TUPLE_TYPE:
    return "_tuple_type";
  case requite::Opcode::_NULL_VALUE:
    return "_null_value";
  case requite::Opcode::_NULL_TYPE:
    return "_null_type";
  case requite::Opcode::_STRUCTURED_BINDING:
    return "_structured_binding";
  case requite::Opcode::_IGNORE:
    return "_ignore";

  // STATIC POLYMORPHISM
  case requite::Opcode::TEMPLATE:
    return "template";
  case requite::Opcode::_SPECIALIZATION:
    return "_specialization";
  case requite::Opcode::BAKE:
    return "bake";
  case requite::Opcode::_BAKE_VALUE:
    return "_bake_value";

  // PROCEDURES
  case requite::Opcode::_CALL:
    return "_call";
  case requite::Opcode::_SIGNATURE:
    return "_signature";
  case requite::Opcode::DESTROY:
    return "destroy";
  case requite::Opcode::_DESTROY_VALUE:
    return "_destroy_value";
  case requite::Opcode::DROP:
    return "drop";
  case requite::Opcode::_DROP_VALUE:
    return "_drop_value";
  case requite::Opcode::ENTRY_POINT:
    return "entry_point";
  case requite::Opcode::FUNCTION:
    return "function";
  case requite::Opcode::METHOD:
    return "method";
  case requite::Opcode::EXTENSION:
    return "extension";
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

  // VALUES
  case requite::Opcode::TRUE:
    return "true";
  case requite::Opcode::FALSE:
    return "false";
  case requite::Opcode::VALUE:
    return "value";
  case requite::Opcode::INDEX:
    return "index";
  case requite::Opcode::INDETERMINATE:
    return "indeterminate";
  case requite::Opcode::NO_DEFAULT_VALUE:
    return "no_default_value";
  case requite::Opcode::_TACIT_COUNT:
    return "_tacit_count";
  case requite::Opcode::THIS:
    return "this";
  case requite::Opcode::RESULT:
    return "result";
  case requite::Opcode::ARGUMENT:
    return "argument";
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
  case requite::Opcode::TACIT:
    return "tacit";
  case requite::Opcode::VOID:
    return "void";
  case requite::Opcode::BOOLEAN:
    return "boolean";
  case requite::Opcode::WORD:
    return "word";
  case requite::Opcode::SIGNED:
    return "signed";
  case requite::Opcode::UNSIGNED:
    return "unsigned";
  case requite::Opcode::BFLOAT16:
    return "bfloat16";
  case requite::Opcode::BINARY16:
    return "binary16";
  case requite::Opcode::BINARY32:
    return "binary32";
  case requite::Opcode::BINARY64:
    return "binary64";
  case requite::Opcode::BINARY128:
    return "binary128";
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
  case requite::Opcode::LOOP:
    return "loop";
  case requite::Opcode::SCOPE:
    return "scope";
  case requite::Opcode::_OPEN_INLINE_SCOPE:
    return "_open_inline_scope";
  case requite::Opcode::_CLOSED_INLINE_SCOPE:
    return "_closed_inline_scope";

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
  case requite::Opcode::UNREACHABLE:
    return "unreachable";

  // ATTRIBUTES
  case requite::Opcode::NOT_FINAL:
    return "not_final";
  case requite::Opcode::MAY_DISCARD:
    return "may_discard";
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
  case requite::Opcode::_SIZE_OF_TYPE:
    return "_size_of_type";
  case requite::Opcode::DEPTH:
    return "depth";
  case requite::Opcode::_DEPTH_OF_TYPE:
    return "_depth_of_type";
  case requite::Opcode::COUNT:
    return "count";
  case requite::Opcode::_COUNT_OF_TYPE:
    return "_count_of_type";
  case requite::Opcode::LENGTH:
    return "length";
  case requite::Opcode::_LENGTH_OF_VALUE:
    return "_length_of_value";
  case requite::Opcode::NAME:
    return "name";
  case requite::Opcode::_NAME_OF_SYMBOL:
    return "_name_of_symbol";
  case requite::Opcode::LINE:
    return "line";
  case requite::Opcode::_LINE_OF_SYMBOL:
    return "_line_of_symbol";
  case requite::Opcode::COLUMN:
    return "column";
  case requite::Opcode::_COLUMN_OF_SYMBOL:
    return "_column_of_symbol";
  case requite::Opcode::IS:
    return "is";
  case requite::Opcode::_VALUE_OF_VARIANT_IS_TYPE:
    return "_value_of_variant_is_type";
  case requite::Opcode::GET:
    return "get";
  case requite::Opcode::_GET_VALUE_OF_VARIANT:
    return "_get_value_of_variant";
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
    return requite::Opcode::_AT_VALUE;
  case requite::Opcode::ADDRESS:
    return requite::Opcode::_ADDRESS_VALUE;
  case requite::Opcode::COPY:
    return requite::Opcode::_COPY_VALUE;
  case requite::Opcode::STEAL:
    return requite::Opcode::_STEAL_VALUE;
  case requite::Opcode::MOVE:
    return requite::Opcode::_MOVE_VALUE;
  case requite::Opcode::DESTROY:
    return requite::Opcode::_DESTROY_VALUE;
  case requite::Opcode::DROP:
    return requite::Opcode::_DROP_VALUE;
  case requite::Opcode::BAKE:
    return requite::Opcode::_BAKE_VALUE;
  case requite::Opcode::FIRST_VARIADIC_ARGUMENT:
    return requite::Opcode::_FIRST_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return requite::Opcode::_NEXT_VARIADIC_ARGUMENT_OF_VALUE;
  case requite::Opcode::LENGTH:
    return requite::Opcode::_LENGTH_OF_VALUE;
  case requite::Opcode::IS:
    return requite::Opcode::_VALUE_OF_VARIANT_IS_TYPE;
  case requite::Opcode::GET:
    return requite::Opcode::_GET_VALUE_OF_VARIANT;
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

constexpr bool getHasTableData(requite::Opcode opcode) {
  return opcode == requite::Opcode::TABLE;
}

constexpr bool getHasObjectData(requite::Opcode opcode) {
  return opcode == requite::Opcode::OBJECT;
}

constexpr bool getHasOverloadData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ENTRY_POINT ||
         opcode == requite::Opcode::FUNCTION ||
         opcode == requite::Opcode::METHOD ||
         opcode == requite::Opcode::EXTENSION ||
         opcode == requite::Opcode::CONSTRUCTOR ||
         opcode == requite::Opcode::DESTRUCTOR;
}

constexpr bool getHasLabelData(requite::Opcode opcode) {
  return opcode == requite::Opcode::LABEL;
}

constexpr bool getHasAliasData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ALIAS;
}

constexpr bool getHasLocalData(requite::Opcode opcode) {
  return opcode == requite::Opcode::__LOCAL_HANDLE ||
         opcode == requite::Opcode::_LOCAL;
}

constexpr bool getHasGlobalData(requite::Opcode opcode) {
  return opcode == requite::Opcode::GLOBAL;
}

constexpr bool getHasPropertyData(requite::Opcode opcode) {
  return opcode == requite::Opcode::PROPERTY;
}

constexpr bool getHasAnonymousFunctionData(requite::Opcode opcode) {
  return opcode == requite::Opcode::_ANONYMOUS_FUNCTION;
}

constexpr bool getHasImportData(requite::Opcode opcode) {
  return opcode == requite::Opcode::IMPORT;
}

constexpr bool getHasUseData(requite::Opcode opcode) {
  return opcode == requite::Opcode::USE;
}

constexpr bool getHasBlockData(requite::Opcode opcode) {
  return opcode == requite::Opcode::IF || opcode == requite::Opcode::ELSE_IF ||
         opcode == requite::Opcode::ELSE || opcode == requite::Opcode::SWITCH ||
         opcode == requite::Opcode::CASE ||
         opcode == requite::Opcode::DEFAULT_CASE ||
         opcode == requite::Opcode::FOR || opcode == requite::Opcode::LOOP ||
         opcode == requite::Opcode::SCOPE;
}

} // namespace requite
