// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/opcode.hpp>

#include <llvm/ADT/StringRef.h>

namespace requite {

enum class Situation {
  NONE,
  ROOT_STATEMENT,
  TOP_STATEMENT,
  TABLE_STATEMENT,
  OBJECT_STATEMENT,
  LOCAL_STATEMENT,
  VALUE,
  REFLECTION,
  ASCRIBED_REFLECTION,
  ARGUMENT,
  PARAMETER,
  BINDING,
  DESTINATION,
  ALTERNATIVE,
  NAME,
  PATH,
  ATTRIBUTE,
  LONG_RANGE_STAGE,
  SHORT_RANGE_STAGE,
  CASE,
  LAST_CASE,
  CAPTURE,
  STRING_LITERAL
};

[[nodiscard]] constexpr llvm::StringRef
getDescription(requite::Situation situation);
[[nodiscard]] constexpr bool getIsValueRelated(requite::Situation situation);
[[nodiscard]] constexpr bool getIsLocalStatementRelated(requite::Situation situation);
template<requite::Situation SITUATION_PARAM>
[[nodiscard]] constexpr bool getCanBeSituation(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeNone(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeRootStatement(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeTopStatement(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeTableStatement(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeObjectStatement(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeLocalStatement(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeValue(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeReflection(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeAscribedReflection(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeArgument(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeParameter(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeBinding(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeDestination(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeAlternative(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeName(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBePath(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeAttribute(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeLongRangeStage(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeShortRangeStage(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeCase(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeLastCase(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeCapture(requite::Opcode opcode);
[[nodiscard]] constexpr bool getCanBeStringLiteral(requite::Opcode opcode);

} // namespace requite

#include <requite/detail/situation.hpp>