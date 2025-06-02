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
  CONVERGING,
  ROOT_STATEMENT,
  BASE_STATEMENT,
  GLOBAL_STATEMENT,
  OBJECT_STATEMENT,
  LOCAL_STATEMENT,
  DESTINATION,
  MATTE_VALUE,
  VALUE_REFLECTIVE_VALUE,
  SYMBOL_REFLECTIVE_VALUE,
  MATTE_SYMBOL,
  VALUE_REFLECTIVE_SYMBOL,
  SYMBOL_REFLECTIVE_SYMBOL,
  ATTRIBUTE,
  VALUE_BINDING,
  SYMBOL_BINDING,
  DESTINATION_BINDING,
  NAMED_FIELD,
  POSITIONAL_FIELD,
  TEMPLATE_PARAMETER,
  SYMBOL_NAME,
  SYMBOL_PATH,
  SWITCH_CASE,
  LAST_SWITCH_CASE,
  CAPTURE
};

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr llvm::StringRef getName();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getIsValue();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextScopeStatementSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextValueReflectiveSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextSymbolReflectiveSituation();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr requite::Situation getNextSymbolBindingSitaution();

template <requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getCanBeSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeNoneSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeNoneSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeConvergingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeRootStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeBaseStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeGlobalStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeLocalStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeObjectStatementSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteDestinationSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveValueSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeMatteSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueReflectiveSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolReflectiveSymbolSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeAttributeSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeValueBindingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolBindingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeDestinationBindingSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeNamedFieldSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBePositionalFieldSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeTemplateParameterSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolNameSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSymbolPathSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeSwitchCaseSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool
getCanBeLastSwitchCaseSituation(requite::Opcode opcode);

[[nodiscard]] constexpr bool getCanBeCaptureSituation(requite::Opcode opcode);

} // namespace requite

#include <requite/detail/situation.hpp>