// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <llvm/ADT/StringRef.h>

namespace requite {

// NOTE: Can not ever have situation that can be either value or type. this
// would cause ambiguity.

// NOTE: a field is a symbol that may have a default value, like a signature
// parameter. named fields have names with symbol_binding operations, and
// postional fields do not.

enum class Situation {
  NONE,
  MATTE_ROOT_STATEMENT,
  MATTE_MODULE_STATEMENT,
  MATTE_TABLE_STATEMENT,
  MATTE_OBJECT_STATEMENT,
  MATTE_LOCAL_STATEMENT,
  VALUE_REFLECTIVE_LOCAL_STATEMENT,
  SYMBOL_REFLECTIVE_LOCAL_STATEMENT,
  MATTE_DESTINATION,
  VALUE_REFLECTIVE_DESTINATION,
  SYMBOL_REFLECTIVE_DESTINATION,
  MATTE_VALUE,
  VALUE_REFLECTIVE_VALUE,
  SYMBOL_REFLECTIVE_VALUE,
  MATTE_JUNCTION,
  VALUE_REFLECTIVE_JUNCTION,
  SYMBOL_REFLECTIVE_JUNCTION,
  MATTE_SYMBOL,
  VALUE_REFLECTIVE_SYMBOL,
  SYMBOL_REFLECTIVE_SYMBOL,
  ARGUMENT,
  PARAMETER,
  PARAMETER_VALUE,
  STATIC_ARGUMENT,
  STATIC_PARAMETER,
  STATIC_PARAMETER_VALUE,
  STRUCTURED_BINDING,
  SYMBOL_NAME,
  SYMBOL_PATH,
  ATTRIBUTE,
  SWITCH_CASE,
  LAST_SWITCH_CASE,
  CAPTURE,
  STRING_LITERAL
};

[[nodiscard]] constexpr llvm::StringRef getName(requite::Situation situation);

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getIsValue();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextScopeStatementSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextValueReflectiveSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextSymbolReflectiveSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getIsValueSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getIsSymbolSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getCanBeSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeNoneSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeNoneSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeConvergingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteRootStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteModuleStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteTableStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteObjectStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteLocalStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveLocalStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveLocalStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteDestinationSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveDestinationSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveDestinationSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeLocalDestinationSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteJunctionSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveJunctionSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveJunctionSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeArgumentSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeParameterSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeParameterValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeStaticArgumentSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeStaticParameterSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeStaticParameterValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeStructuredBindingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolNameSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolPathSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeAttributeSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSwitchCaseSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeLastSwitchCaseSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeCaptureSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeStringLiteralSituation(requite::Opcode opcode);

} // namespace requite

#include <requite/detail/situation.hpp>