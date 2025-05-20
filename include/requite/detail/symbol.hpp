// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

constexpr requite::RootSymbolType getRootSymbolType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::INFERENCE:
    return requite::RootSymbolType::INFERENCE;
  case requite::Opcode::VOID:
    return requite::RootSymbolType::VOID;
  case requite::Opcode::VARIADIC_ARGUMENTS:
    return requite::RootSymbolType::VARIADIC_ARGUMENTS;
  case requite::Opcode::NULL_:
    return requite::RootSymbolType::NULL_;
  case requite::Opcode::BOOLEAN:
    return requite::RootSymbolType::BOOLEAN;
  case requite::Opcode::WORD:
    return requite::RootSymbolType::WORD;
  case requite::Opcode::SIGNED_INTEGER:
    return requite::RootSymbolType::SIGNED_INTEGER;
  case requite::Opcode::UNSIGNED_INTEGER:
    return requite::RootSymbolType::UNSIGNED_INTEGER;
  case requite::Opcode::BINARY_HALF_FLOAT:
    return requite::RootSymbolType::BINARY_HALF_FLOAT;
  case requite::Opcode::BINARY_SINGLE_FLOAT:
    return requite::RootSymbolType::BINARY_SINGLE_FLOAT;
  case requite::Opcode::BINARY_DOUBLE_FLOAT:
    return requite::RootSymbolType::BINARY_DOUBLE_FLOAT;
  case requite::Opcode::BINARY_QUAD_FLOAT:
    return requite::RootSymbolType::BINARY_QUAD_FLOAT;
  case requite::Opcode::C_CHAR:
    return requite::RootSymbolType::C_CHAR;
  case requite::Opcode::UTF8:
    return requite::RootSymbolType::UTF8;
  case requite::Opcode::NO_PARAMETER_SIGNATURE:
    [[fallthrough]];
  case requite::Opcode::NAMED_PARAMETER_SIGNATURE:
    [[fallthrough]];
  case requite::Opcode::POSITIONAL_PARAMETER_SIGNATURE:
    return requite::RootSymbolType::SIGNATURE;
  case requite::Opcode::TUPLE:
    return requite::RootSymbolType::TUPLE;
  case requite::Opcode::ANONYMOUS_OBJECT:
    return requite::RootSymbolType::ANONYMOUS_OBJECT;
  case requite::Opcode::OBJECT:
    return requite::RootSymbolType::OBJECT;
  case requite::Opcode::TABLE:
    return requite::RootSymbolType::TABLE;
  case requite::Opcode::ALIAS:
    return requite::RootSymbolType::ALIAS;
  case requite::Opcode::GLOBAL:
    [[fallthrough]];
  case requite::Opcode::LOCAL:
    [[fallthrough]];
  case requite::Opcode::PROPERTY:
    return requite::RootSymbolType::VARIABLE;
  case requite::Opcode::MODULE:
    return requite::RootSymbolType::MODULE;
  case requite::Opcode::LABEL:
    return requite::RootSymbolType::LABEL;
  default:
    break;
  }
  return requite::RootSymbolType::NONE;
}

constexpr bool getIsNullary(requite::RootSymbolType type) {
  switch (type) {
  case requite::RootSymbolType::NONE:
    return false;
  case requite::RootSymbolType::INFERENCE:
    return true;
  case requite::RootSymbolType::VOID:
    return true;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    return true;
  case requite::RootSymbolType::NULL_:
    return true;
  case requite::RootSymbolType::BOOLEAN:
    return true;
  case requite::RootSymbolType::WORD:
    return false;
  case requite::RootSymbolType::SIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    return true;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    return true;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    return true;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    return true;
  case requite::RootSymbolType::C_CHAR:
    return true;
  case requite::RootSymbolType::UTF8:
    return true;
  case requite::RootSymbolType::SIGNATURE:
    return false;
  case requite::RootSymbolType::TUPLE:
    return false;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return false;
  case requite::RootSymbolType::OBJECT:
    return false;
  case requite::RootSymbolType::TABLE:
    return false;
  case requite::RootSymbolType::ALIAS:
    return false;
  case requite::RootSymbolType::VARIABLE:
    return false;
  case requite::RootSymbolType::PROCEDURE:
    return false;
  case requite::RootSymbolType::NAMED_PROCEDURE_GROUP:
    return false;
  case requite::RootSymbolType::MODULE:
    return false;
  case requite::RootSymbolType::LABEL:
    return false;
  }
  return false;
}

constexpr bool getHasDepth(requite::RootSymbolType type) {
  switch (type) {
  case requite::RootSymbolType::NONE:
    return false;
  case requite::RootSymbolType::INFERENCE:
    return false;
  case requite::RootSymbolType::VOID:
    return false;
  case requite::RootSymbolType::BOOLEAN:
    return false;
  case requite::RootSymbolType::WORD:
    return true;
  case requite::RootSymbolType::SIGNED_INTEGER:
    return true;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    return true;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    return false;
  case requite::RootSymbolType::C_CHAR:
    return false;
  case requite::RootSymbolType::UTF8:
    return false;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    return false;
  case requite::RootSymbolType::NULL_:
    return false;
  case requite::RootSymbolType::OBJECT:
    return false;
  case requite::RootSymbolType::TABLE:
    return false;
  case requite::RootSymbolType::ALIAS:
    return false;
  case requite::RootSymbolType::VARIABLE:
    return false;
  case requite::RootSymbolType::SIGNATURE:
    return false;
  case requite::RootSymbolType::TUPLE:
    return false;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return false;
  case requite::RootSymbolType::PROCEDURE:
    return false;
  case requite::RootSymbolType::NAMED_PROCEDURE_GROUP:
    return false;
  case requite::RootSymbolType::MODULE:
    return false;
  case requite::RootSymbolType::LABEL:
    return false;
  }
  REQUITE_UNREACHABLE();
}

constexpr bool getHasUserAttributes(requite::RootSymbolType type) {
  switch (type) {
  case requite::RootSymbolType::NONE:
    return false;
  case requite::RootSymbolType::INFERENCE:
    return false;
  case requite::RootSymbolType::VOID:
    return false;
  case requite::RootSymbolType::BOOLEAN:
    return false;
  case requite::RootSymbolType::WORD:
    return false;
  case requite::RootSymbolType::SIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::UNSIGNED_INTEGER:
    return false;
  case requite::RootSymbolType::BINARY_HALF_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_SINGLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_DOUBLE_FLOAT:
    return false;
  case requite::RootSymbolType::BINARY_QUAD_FLOAT:
    return false;
  case requite::RootSymbolType::C_CHAR:
    return false;
  case requite::RootSymbolType::UTF8:
    return false;
  case requite::RootSymbolType::VARIADIC_ARGUMENTS:
    return false;
  case requite::RootSymbolType::NULL_:
    return false;
  case requite::RootSymbolType::OBJECT:
    return true;
  case requite::RootSymbolType::TABLE:
    return false;
  case requite::RootSymbolType::ALIAS:
    return true;
  case requite::RootSymbolType::VARIABLE:
    return true;
  case requite::RootSymbolType::SIGNATURE:
    return false;
  case requite::RootSymbolType::TUPLE:
    return false;
  case requite::RootSymbolType::ANONYMOUS_OBJECT:
    return false;
  case requite::RootSymbolType::PROCEDURE:
    return true;
  case requite::RootSymbolType::NAMED_PROCEDURE_GROUP:
    return false;
  case requite::RootSymbolType::MODULE:
    return false;
  case requite::RootSymbolType::LABEL:
    return false;
  }
  REQUITE_UNREACHABLE();
}

constexpr requite::SubSymbolType getSubSymbolType(requite::Opcode opcode) {
  switch (opcode) {
  case requite::Opcode::ARRAY:
    return requite::SubSymbolType::ARRAY;
  case requite::Opcode::REFERENCE:
    return requite::SubSymbolType::REFERENCE;
  case requite::Opcode::STOLEN_REFERENCE:
    return requite::SubSymbolType::STOLEN_REFERENCE;
  case requite::Opcode::FAT_POINTER:
    return requite::SubSymbolType::FAT_POINTER;
  case requite::Opcode::POINTER:
    return requite::SubSymbolType::POINTER;
  default:
    break;
  }
  return requite::SubSymbolType::NONE;
}

constexpr bool getHasCount(requite::SubSymbolType type) {
  switch (type) {
  case requite::SubSymbolType::NONE:
    return false;
  case requite::SubSymbolType::ARRAY:
    return true;
  case requite::SubSymbolType::REFERENCE:
    return false;
  case requite::SubSymbolType::STOLEN_REFERENCE:
    return false;
  case requite::SubSymbolType::FAT_POINTER:
    return false;
  case requite::SubSymbolType::POINTER:
    return false;
  }
  return false;
}

} // namespace requite