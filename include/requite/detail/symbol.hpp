// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

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
  case requite::RootSymbolType::SCOPE:
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
  case requite::RootSymbolType::SCOPE:
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