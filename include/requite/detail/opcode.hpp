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

enum _OpcodeFlags : std::uint32_t {
  // FLAGS                                                              // TAG
  _OPCODE_FLAG_NONE =
      0, //                                                             //
  _OPCODE_FLAG_CONVERGING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(0)), // a
  _OPCODE_FLAG_INTERNAL_USE_ONLY =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(1)), // b
  _OPCODE_FLAG_ROOT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(2)), // c
  _OPCODE_FLAG_BASE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(3)), // d
  _OPCODE_FLAG_GLOBAL_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(4)), // e
  _OPCODE_FLAG_OBJECT_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(5)), // f
  _OPCODE_FLAG_MATTE_DESTINATION =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(6)), // g
  _OPCODE_FLAG_MATTE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(7)), // h
  _OPCODE_FLAG_VALUE_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(8)), // i
  _OPCODE_FLAG_SYMBOL_REFLECTIVE_VALUE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(9)), // j
  _OPCODE_FLAG_MATTE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(10)), // k
  _OPCODE_FLAG_VALUE_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(11)), // l
  _OPCODE_FLAG_SYMBOL_REFLECTIVE_SYMBOL =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(12)), // m
  _OPCODE_FLAG_MATTE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(13)), // n
  _OPCODE_FLAG_VALUE_REFLECTIVE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(14)), // o
  _OPCODE_FLAG_SYMBOL_REFLECTIVE_STATEMENT =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(15)), // p
  _OPCODE_FLAG_ATTRIBUTE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(16)), // q
  _OPCODE_FLAG_VALUE_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(17)), // r
  _OPCODE_FLAG_SYMBOL_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(18)), // s
  _OPCODE_FLAG_DESTINATION_BINDING =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(19)), // t
  _OPCODE_FLAG_NAMED_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(20)), // u
  _OPCODE_FLAG_POSITIONAL_FIELD =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(21)), // v
  _OPCODE_FLAG_TEMPLATE_PARAMETER =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(22)), // w
  _OPCODE_FLAG_SYMBOL_NAME =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(23)), // x
  _OPCODE_FLAG_SYMBOL_PATH =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(24)), // y
  _OPCODE_FLAG_SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(25)), // z
  _OPCODE_FLAG_LAST_SWITCH_CASE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(26)), // A
  _OPCODE_FLAG_CAPTURE =
      (static_cast<std::uint32_t>(1) << static_cast<std::uint32_t>(27)), // B
};

[[nodiscard]] constexpr std::string_view _getFlagsTag(requite::Opcode opcode) {
  using Flags = requite::_OpcodeFlags;
  switch (opcode) {
  case requite::Opcode::_NONE:
    return "b";

  // LITERALS
  case requite::Opcode::_INTEGER_LITERAL:
    return "bh";
  case requite::Opcode::_REAL_LITERAL:
    return "bh";
  case requite::Opcode::_STRING_LITERAL:
    return "bh";
  case requite::Opcode::_CODEUNIT_LITERAL:
    return "bh";
  case requite::Opcode::_IDENTIFIER_LITERAL:
    return "bghijklmvxy";

  // ERRORS
  case requite::Opcode::_ERROR:
    return "b";

  // SITUATIONAL
  case requite::Opcode::SITUATIONAL_LOGICAL_AND_OR_DOUBLE_REFERENCE:
    return "hkv";
  case requite::Opcode::SITUATIONAL_ADD_OR_NULL_TERMINATED:
    return "hkv";
  case requite::Opcode::SITUATIONAL_SUBTRACT_OR_NEGATE:
    return "h";
  case requite::Opcode::SITUATIONAL_MULTIPLY_OR_POINTER:
    return "hkv";
  case requite::Opcode::SITUATIONAL_DIVIDE_OR_FAT_POINTER:
    return "hkv";
  case requite::Opcode::SITUATIONAL_MODULUS_OR_ARRAY:
    return "hkv";
  case requite::Opcode::SITUATIONAL_BITWISE_AND_OR_REFERENCE:
    return "hkv";
  case requite::Opcode::SITUATIONAL_CALL_OR_SIGNATURE:
    return "hknv";
  case requite::Opcode::SITUATIONAL_BIND_VALUE_OR_DEFAULT_VALUE:
    return "ruvw";
  case requite::Opcode::SITUATIONAL_BIND_SYMBOL_OR_DEFAULT_SYMBOL:
    return "suw";
  case requite::Opcode::SITUATIONAL_TRIP:
    return "ghkv";
  case requite::Opcode::SITUATIONAL_INFERENCE_OR_INDETERMINATE:
    return "hkv";

  // LOGICAL
  case requite::Opcode::LOGICAL_AND:
    return "ah";
  case requite::Opcode::LOGICAL_OR:
    return "ah";
  case requite::Opcode::LOGICAL_COMPLEMENT:
    return "h";

  // COMPARISON
  case requite::Opcode::GREATER:
    return "ah";
  case requite::Opcode::GREATER_EQUAL:
    return "ah";
  case requite::Opcode::LESS:
    return "ah";
  case requite::Opcode::LESS_EQUAL:
    return "ah";
  case requite::Opcode::EQUAL:
    return "ah";
  case requite::Opcode::NOT_EQUAL:
    return "ah";

  // REFLECT
  case requite::Opcode::REFLECT_VALUE:
    return "aghknv";
  case requite::Opcode::REFLECT_SYMBOL:
    return "aghknv";

  // BIND
  case requite::Opcode::BIND_VALUE:
    return "rw";
  case requite::Opcode::BIND_SYMBOL:
    return "suw";
  case requite::Opcode::DEFAULT_VALUE:
    return "uvw";
  case requite::Opcode::DEFAULT_SYMBOL:
    return "w";

  // APPLY
  case requite::Opcode::ASCRIBE:
    return "defhkv";
  case requite::Opcode::CAST:
    return "h";
  case requite::Opcode::STRINGIFY:
    return "h";
  case requite::Opcode::CODEUNIFY:
    return "h";
  case requite::Opcode::IDENTIFY:
    return "bghijklmxyv";
  case requite::Opcode::BAKE:
    return "";

  // ARITHMETIC
  case requite::Opcode::ADD:
    return "ah";
  case requite::Opcode::SUBTRACT:
    return "ah";
  case requite::Opcode::MULTIPLY:
    return "ah";
  case requite::Opcode::DIVIDE:
    return "ah";
  case requite::Opcode::MODULUS:
    return "ah";
  case requite::Opcode::NEGATE:
    return "h";

  // BITWISE
  case requite::Opcode::BITWISE_CAST:
    return "h";
  case requite::Opcode::BITWISE_OR:
    return "ah";
  case requite::Opcode::BITWISE_AND:
    return "ah";
  case requite::Opcode::BITWISE_XOR:
    return "ah";
  case requite::Opcode::BITWISE_COMPLEMENT:
    return "h";
  case requite::Opcode::BITWISE_SHIFT_LEFT:
    return "h";
  case requite::Opcode::BITWISE_SHIFT_RIGHT:
    return "h";
  case requite::Opcode::BITWISE_ROTATE_LEFT:
    return "h";
  case requite::Opcode::BITWISE_ROTATE_RIGHT:
    return "h";

  // MEMORY
  case requite::Opcode::CONCATINATE:
    return "ah";
  case requite::Opcode::FROM_FRONT:
    return "h";
  case requite::Opcode::FROM_BACK:
    return "h";
  case requite::Opcode::TRUNCATE_FRONT:
    return "h";
  case requite::Opcode::TRUNCATE_BACK:
    return "h";
  case requite::Opcode::AT:
    return "gh";
  case requite::Opcode::DEREFERENCE:
    return "gh";

  // ASSIGNMENT
  case requite::Opcode::ASSIGN:
    return "aghn";
  case requite::Opcode::ASSIGN_ADD:
    return "ghn";
  case requite::Opcode::ASSIGN_SUBTRACT:
    return "ghn";
  case requite::Opcode::ASSIGN_MULTIPLY:
    return "ghn";
  case requite::Opcode::ASSIGN_DIVIDE:
    return "ghn";
  case requite::Opcode::ASSIGN_MODULUS:
    return "ghn";

  // MOVE SEMANTICS
  case requite::Opcode::COPY:
    return "i";
  case requite::Opcode::MOVE:
    return "i";
  case requite::Opcode::SWAP:
    return "n";

  // SUBTYPE
  case requite::Opcode::ARRAY:
    return "kv";
  case requite::Opcode::REFERENCE:
    return "kv";
  case requite::Opcode::STOLEN_REFERENCE:
    return "kv";
  case requite::Opcode::POINTER:
    return "kv";
  case requite::Opcode::FAT_POINTER:
    return "kv";

  // TYPE MODIFIER
  case requite::Opcode::MUTABLE:
    return "q";
  case requite::Opcode::VOLATILE:
    return "q";
  case requite::Opcode::ATOMIC:
    return "q";
  case requite::Opcode::NULL_TERMINATED:
    return "q";
  case requite::Opcode::BAKED:
    return "";

  // TRIPS
  case requite::Opcode::TUPLE:
    return "h";
  case requite::Opcode::ANONYMOUS_OBJECT:
    return "h";
  case requite::Opcode::NULL_:
    return "hkv";
  case requite::Opcode::IGNORE:
    return "g";

  // TEMPLATES
  case requite::Opcode::TEMPLATE:
    return "q";
  case requite::Opcode::SPECIALIZATION:
    return "hkv";

  // PROCEDURES
  case requite::Opcode::NO_PARAMETER_SIGNATURE:
    return "kv";
  case requite::Opcode::NAMED_PARAMETER_SIGNATURE:
    return "kv";
  case requite::Opcode::POSITIONAL_PARAMETER_SIGNATURE:
    return "kv";
  case requite::Opcode::NO_ARGUMENT_CALL:
    return "ghn";
  case requite::Opcode::NAMED_ARGUMENT_CALL:
    return "ghn";
  case requite::Opcode::POSITIONAL_ARGUMENT_CALL:
    return "ghn";
  case requite::Opcode::DESTROY:
    return "i";
  case requite::Opcode::ENTRY_POINT:
    return "d";
  case requite::Opcode::FUNCTION:
    return "def";
  case requite::Opcode::METHOD:
    return "f";
  case requite::Opcode::CONSTRUCTOR:
    return "f";
  case requite::Opcode::DESTRUCTOR:
    return "f";
  case requite::Opcode::ANONYMOUS_FUNCTION:
    return "h";
  case requite::Opcode::CAPTURE:
    return "B";

  // CONTROL FLOW
  case requite::Opcode::RETURN:
    return "n";
  case requite::Opcode::BREAK:
    return "n";
  case requite::Opcode::CONTINUE:
    return "n";
  case requite::Opcode::FALLTHROUGH:
    return "n";
  case requite::Opcode::EXIT:
    return "n";
  case requite::Opcode::GOTO:
    return "n";
  case requite::Opcode::LABEL:
    return "q";

  // SYMBOLS
  case requite::Opcode::OBJECT:
    return "defn";
  case requite::Opcode::TABLE:
    return "defn";
  case requite::Opcode::ALIAS:
    return "defn";
  case requite::Opcode::LOCAL:
    return "n";
  case requite::Opcode::GLOBAL:
    return "defn";
  case requite::Opcode::PROPERTY:
    return "f";

  // VALUES
  case requite::Opcode::TEMP:
    return "gh";
  case requite::Opcode::TRUE:
    return "h";
  case requite::Opcode::FALSE:
    return "h";
  case requite::Opcode::VALUE:
    return "gh";
  case requite::Opcode::INDEX:
    return "h";
  case requite::Opcode::EXCEPTION:
    return "gh";
  case requite::Opcode::INDETERMINATE:
    return "h";
  case requite::Opcode::NO_DEFAULT_VALUE:
    return "h";
  case requite::Opcode::INFERENCED_COUNT:
    return "h";
  case requite::Opcode::THIS:
    return "gh";
  case requite::Opcode::RESULT:
    return "gh";
  case requite::Opcode::INPUT:
    return "gh";
  case requite::Opcode::OUTPUT:
    return "gh";
  case requite::Opcode::ADDRESS_SIZE:
    return "h";
  case requite::Opcode::ADDRESS_DEPTH:
    return "h";
  case requite::Opcode::BITS_PER_BYTE:
    return "h";

  // BUILTIN TYPES
  case requite::Opcode::INFERENCE:
    return "kv";
  case requite::Opcode::VOID:
    return "kv";
  case requite::Opcode::BOOLEAN:
    return "kv";
  case requite::Opcode::WORD:
    return "kv";
  case requite::Opcode::SIGNED_INTEGER:
    return "kv";
  case requite::Opcode::UNSIGNED_INTEGER:
    return "kv";
  case requite::Opcode::BINARY_HALF_FLOAT:
    return "kv";
  case requite::Opcode::BINARY_SINGLE_FLOAT:
    return "kv";
  case requite::Opcode::BINARY_DOUBLE_FLOAT:
    return "kv";
  case requite::Opcode::BINARY_QUAD_FLOAT:
    return "kv";
  case requite::Opcode::C_CHAR:
    return "kv";
  case requite::Opcode::UTF8:
    return "kv";

  // VARIADIC ARGUMENTS
  case requite::Opcode::VARIADIC_ARGUMENTS:
    return "kv";
  case requite::Opcode::START_VARIADIC_ARGUMENT:
    return "i";
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return "i";

  // SCOPES
  case requite::Opcode::IF:
    return "n";
  case requite::Opcode::ELSE_IF:
    return "n";
  case requite::Opcode::ELSE:
    return "n";
  case requite::Opcode::SWITCH:
    return "n";
  case requite::Opcode::CASE:
    return "zA";
  case requite::Opcode::DEFAULT_CASE:
    return "A";
  case requite::Opcode::FOR:
    return "n";
  case requite::Opcode::WHILE:
    return "n";
  case requite::Opcode::DO_WHILE:
    return "n";
  case requite::Opcode::FOR_EACH:
    return "n";
  case requite::Opcode::LOOP:
    return "n";
  case requite::Opcode::SCOPE:
    return "n";
  case requite::Opcode::CONDUIT:
    return "gh";

  // ACCESS MODIFIERS
  case requite::Opcode::PRIVATE:
    return "q";
  case requite::Opcode::PROTECTED:
    return "q";
  case requite::Opcode::EXPORT:
    return "q";

  // SYMBOL GRAPH
  case requite::Opcode::IMPORT:
    return "defn";
  case requite::Opcode::USE:
    return "defn";

  // SOURCES
  case requite::Opcode::MODULE:
    return "c";

  // ERROR HANDLING AND DEBUGGING
  case requite::Opcode::ASSERT:
    return "n";
  case requite::Opcode::INFO:
    return "n";
  case requite::Opcode::WARN:
    return "n";
  case requite::Opcode::ERROR:
    return "n";
  case requite::Opcode::TRY:
    return "n";
  case requite::Opcode::CATCH:
    return "n";
  case requite::Opcode::THROW:
    return "n";
  case requite::Opcode::MAY_THROW:
    return "q";
  case requite::Opcode::THROWS:
    return "q";

  // ATTRIBUTES
  case requite::Opcode::EXTERNAL:
    return "q";
  case requite::Opcode::C:
    return "q";
  case requite::Opcode::POSITIONAL_ARGUMENTS:
    return "q";
  case requite::Opcode::NOT_FINAL:
    return "q";
  case requite::Opcode::MAY_DISCARD:
    return "q";
  case requite::Opcode::NO_SHORT_CIRCUIT:
    return "q";
  case requite::Opcode::NO_REMAINDER:
    return "q";
  case requite::Opcode::INLINE:
    return "q";
  case requite::Opcode::MANGLED_NAME:
    return "jq";
  case requite::Opcode::PACK:
    return "q";

  // REFLECTION
  case requite::Opcode::SIZE:
    return "j";
  case requite::Opcode::DEPTH:
    return "j";
  case requite::Opcode::COUNT:
    return "j";
  case requite::Opcode::TYPE:
    return "lm";
  case requite::Opcode::NAME:
    return "hj";
  case requite::Opcode::LINE:
    return "hj";
  case requite::Opcode::COLUMN:
    return "j";
  case requite::Opcode::UNDERLYING:
    return "lm";
  case requite::Opcode::IS:
    return "j";

  case requite::Opcode::_LAST:
    break;
  }
  return "";
}

[[nodiscard]] constexpr requite::_OpcodeFlags
_getFlags(requite::Opcode opcode) {
  std::string_view tag = requite::_getFlagsTag(opcode);
  using Flags = requite::_OpcodeFlags;
  Flags flags = Flags::_OPCODE_FLAG_NONE;
  for (char c : tag) {
    switch (c) {
    case 'a':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_CONVERGING) |
          requite::getUnderlying(flags));
      break;
    case 'b':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_INTERNAL_USE_ONLY) |
          requite::getUnderlying(flags));
      break;
    case 'c':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_ROOT_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'd':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_BASE_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'e':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_GLOBAL_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'f':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_OBJECT_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'g':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_MATTE_DESTINATION) |
          requite::getUnderlying(flags));
      break;
    case 'h':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_MATTE_VALUE) |
          requite::getUnderlying(flags));
      break;
    case 'i':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_VALUE_REFLECTIVE_VALUE) |
          requite::getUnderlying(flags));
      break;
    case 'j':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SYMBOL_REFLECTIVE_VALUE) |
          requite::getUnderlying(flags));
      break;
    case 'k':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_MATTE_SYMBOL) |
          requite::getUnderlying(flags));
      break;
    case 'l':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_VALUE_REFLECTIVE_SYMBOL) |
          requite::getUnderlying(flags));
      break;
    case 'm':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SYMBOL_REFLECTIVE_SYMBOL) |
          requite::getUnderlying(flags));
      break;
    case 'n':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_MATTE_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'o':
      flags = static_cast<Flags>(
          requite::getUnderlying(
              Flags::_OPCODE_FLAG_VALUE_REFLECTIVE_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'p':
      flags = static_cast<Flags>(
          requite::getUnderlying(
              Flags::_OPCODE_FLAG_SYMBOL_REFLECTIVE_STATEMENT) |
          requite::getUnderlying(flags));
      break;
    case 'q':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_ATTRIBUTE) |
          requite::getUnderlying(flags));
      break;
    case 'r':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_VALUE_BINDING) |
          requite::getUnderlying(flags));
      break;
    case 's':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SYMBOL_BINDING) |
          requite::getUnderlying(flags));
      break;
    case 't':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_DESTINATION_BINDING) |
          requite::getUnderlying(flags));
      break;
    case 'u':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_NAMED_FIELD) |
          requite::getUnderlying(flags));
      break;
    case 'v':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_POSITIONAL_FIELD) |
          requite::getUnderlying(flags));
      break;
    case 'w':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_TEMPLATE_PARAMETER) |
          requite::getUnderlying(flags));
      break;
    case 'x':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SYMBOL_NAME) |
          requite::getUnderlying(flags));
      break;
    case 'y':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SYMBOL_PATH) |
          requite::getUnderlying(flags));
      break;
    case 'z':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_SWITCH_CASE) |
          requite::getUnderlying(flags));
      break;
    case 'A':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_LAST_SWITCH_CASE) |
          requite::getUnderlying(flags));
      break;
    case 'B':
      flags = static_cast<Flags>(
          requite::getUnderlying(Flags::_OPCODE_FLAG_CAPTURE) |
          requite::getUnderlying(flags));
      break;
    }
  }
  return flags;
}

[[nodiscard]] constexpr std::array<requite::_OpcodeFlags, requite::OPCODE_COUNT>
_getOpcodeFlagArray() {
  std::array<requite::_OpcodeFlags, requite::OPCODE_COUNT> flags{};
  for (unsigned opcode_i = 0;
       opcode_i < static_cast<unsigned>(requite::Opcode::_LAST); ++opcode_i) {
    const requite::Opcode opcode = static_cast<requite::Opcode>(opcode_i);
    flags[opcode_i] = requite::_getFlags(opcode);
  }
  return flags;
}

constexpr std::array<requite::_OpcodeFlags, requite::OPCODE_COUNT>
    _OPCODE_FLAGS = requite::_getOpcodeFlagArray();

[[nodiscard]] constexpr bool _getHasFlags(requite::Opcode opcode,
                                          requite::_OpcodeFlags flags) {
  const unsigned opcode_i = static_cast<unsigned>(opcode);
  REQUITE_ASSERT(opcode_i < requite::OPCODE_COUNT);
  requite::_OpcodeFlags opcode_flags = requite::_OPCODE_FLAGS[opcode_i];
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
  case requite::Opcode::SITUATIONAL_ADD_OR_NULL_TERMINATED:
    return "situational_add_or_null_terminated";
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
  case requite::Opcode::SITUATIONAL_INFERENCE_OR_INDETERMINATE:
    return "situational_inference_or_indeterminate";

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
  case requite::Opcode::FROM_BACK:
    return "from_back";
  case requite::Opcode::TRUNCATE_FRONT:
    return "truncate_front";
  case requite::Opcode::TRUNCATE_BACK:
    return "truncate_back";
  case requite::Opcode::AT:
    return "at";
  case requite::Opcode::DEREFERENCE:
    return "dereference";

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
  case requite::Opcode::MOVE:
    return "move";
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
  case requite::Opcode::BAKED:
    return "baked";

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
  case requite::Opcode::INFERENCE:
    return "inference";
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
  case requite::Opcode::C_CHAR:
    return "c_char";
  case requite::Opcode::UTF8:
    return "utf8";

  // VARIADIC ARGUMENTS
  case requite::Opcode::VARIADIC_ARGUMENTS:
    return "variadic_arguments";
  case requite::Opcode::START_VARIADIC_ARGUMENT:
    return "start_variadic_argument";
  case requite::Opcode::NEXT_VARIADIC_ARGUMENT:
    return "next_variadic_argument";

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
  case requite::Opcode::CONDUIT:
    return "conduit";

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
  case requite::Opcode::INFO:
    return "info";
  case requite::Opcode::WARN:
    return "warn";
  case requite::Opcode::ERROR:
    return "error";
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
  case requite::Opcode::PACK:
    return "pack";

  // REFLECTION
  case requite::Opcode::SIZE:
    return "size";
  case requite::Opcode::DEPTH:
    return "depth";
  case requite::Opcode::COUNT:
    return "count";
  case requite::Opcode::TYPE:
    return "type";
  case requite::Opcode::NAME:
    return "name";
  case requite::Opcode::LINE:
    return "line";
  case requite::Opcode::COLUMN:
    return "column";
  case requite::Opcode::UNDERLYING:
    return "underlying";
  case requite::Opcode::IS:
    return "is";

  case requite::Opcode::_LAST:
    break;
  }
  return "_error";
}

constexpr bool getIsValid(requite::Opcode opcode) {
  return opcode != requite::Opcode::_NONE &&
         opcode != requite::Opcode::_ERROR &&
         static_cast<unsigned>(opcode) < requite::OPCODE_COUNT;
}

constexpr bool getIsInternalUseOnly(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_OPCODE_FLAG_INTERNAL_USE_ONLY);
  return has_flags;
}

constexpr bool getIsConverging(requite::Opcode opcode) {
  const bool has_flags =
      requite::_getHasFlags(opcode, requite::_OPCODE_FLAG_CONVERGING);
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
  return opcode == requite::Opcode::SCOPE;
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
         opcode == requite::Opcode::CONSTRUCTOR ||
         opcode == requite::Opcode::DESTRUCTOR;
}

constexpr bool getHasAliasData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ALIAS;
}

constexpr bool getHasVariableData(requite::Opcode opcode) {
  return opcode == requite::Opcode::LOCAL ||
         opcode == requite::Opcode::GLOBAL ||
         opcode == requite::Opcode::PROPERTY;
}

constexpr bool getHasAnonymousFunctionData(requite::Opcode opcode) {
  return opcode == requite::Opcode::ANONYMOUS_FUNCTION;
}

} // namespace requite
