// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr llvm::StringRef getDescription(requite::Situation situation) {
  using namespace requite;
  using S = Situation;
  switch (situation) {
  case S::NONE:
    return "no situation";
  case S::ROOT_STATEMENT:
    return "module root statement";
  case S::TOP_STATEMENT:
    return "top-level statement";
  case S::TABLE_STATEMENT:
    return "table statement";
  case S::OBJECT_STATEMENT:
    return "object statement";
  case S::LOCAL_STATEMENT:
    return "local statement";
  case S::VALUE:
    return "value";
  case S::REFLECTION:
    return "reflection";
  case S::ARGUMENT:
    return "argument";
  case S::PARAMETER:
    return "parameter";
  case S::BINDING:
    return "binding";
  case S::DESTINATION:
    return "destination";
  case S::ALTERNATIVE:
    return "alternative";
  case S::NAME:
    return "name";
  case S::PATH:
    return "path";
  case S::ATTRIBUTE:
    return "attribute";
  case S::LONG_RANGE_STAGE:
    return "long range stage";
  case S::SHORT_RANGE_STAGE:
    return "short range stage";
  case S::CASE:
    return "case";
  case S::LAST_CASE:
    return "last case";
  case S::CAPTURE:
    return "capture";
  case S::STRING_LITERAL:
    return "string literal";
  default:
    break;
  }
  return "unknown";
}

constexpr bool getIsValueRelated(requite::Situation situation) {
  return situation == requite::Situation::VALUE ||
         situation == requite::Situation::REFLECTION ||
         situation == requite::Situation::ARGUMENT ||
         situation == requite::Situation::PARAMETER ||
         situation == requite::Situation::DESTINATION;
}

constexpr bool getIsLocalStatementRelated(requite::Situation situation) {
  return situation == requite::Situation::LOCAL_STATEMENT ||
         situation == requite::Situation::REFLECTION;
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getCanBeSituation(requite::Opcode opcode) {
  using namespace requite;
  using S = Situation;
  constexpr S SP = SITUATION_PARAM;
  if constexpr (SP == S::NONE) {
    return getCanBeNone(opcode);
  } else if constexpr (SP == S::ROOT_STATEMENT) {
    return getCanBeRootStatement(opcode);
  } else if constexpr (SP == S::TOP_STATEMENT) {
    return getCanBeTopStatement(opcode);
  } else if constexpr (SP == S::TABLE_STATEMENT) {
    return getCanBeTableStatement(opcode);
  } else if constexpr (SP == S::OBJECT_STATEMENT) {
    return getCanBeObjectStatement(opcode);
  } else if constexpr (SP == S::LOCAL_STATEMENT) {
    return getCanBeLocalStatement(opcode);
  } else if constexpr (SP == S::VALUE) {
    return getCanBeValue(opcode);
  } else if constexpr (SP == S::REFLECTION) {
    return getCanBeReflection(opcode);
  } else if constexpr (SP == S::ARGUMENT) {
    return getCanBeArgument(opcode);
  } else if constexpr (SP == S::PARAMETER) {
    return getCanBeParameter(opcode);
  } else if constexpr (SP == S::BINDING) {
    return getCanBeBinding(opcode);
  } else if constexpr (SP == S::DESTINATION) {
    return getCanBeDestination(opcode);
  } else if constexpr (SP == S::ALTERNATIVE) {
    return getCanBeAlternative(opcode);
  } else if constexpr (SP == S::NAME) {
    return getCanBeName(opcode);
  } else if constexpr (SP == S::PATH) {
    return getCanBePath(opcode);
  } else if constexpr (SP == S::ATTRIBUTE) {
    return getCanBeAttribute(opcode);
  } else if constexpr (SP == S::LONG_RANGE_STAGE) {
    return getCanBeLongRangeStage(opcode);
  } else if constexpr (SP == S::SHORT_RANGE_STAGE) {
    return getCanBeShortRangeStage(opcode);
  } else if constexpr (SP == S::CASE) {
    return getCanBeCase(opcode);
  } else if constexpr (SP == S::LAST_CASE) {
    return getCanBeLastCase(opcode);
  } else if constexpr (SP == S::CAPTURE) {
    return getCanBeCapture(opcode);
  } else if constexpr (SP == S::STRING_LITERAL) {
    return getCanBeStringLiteral(opcode);
  } else {
    static_assert(false, "invalid situation");
  }
}

constexpr bool getCanBeNone(requite::Opcode opcode) {
  return opcode == requite::Opcode::__NONE;
}

constexpr bool getCanBeRootStatement(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_ROOT_STATEMENT);
}

constexpr bool getCanBeTopStatement(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_TOP_STATEMENT);
}

constexpr bool getCanBeTableStatement(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_TABLE_STATEMENT);
}

constexpr bool getCanBeObjectStatement(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_OBJECT_STATEMENT);
}

constexpr bool getCanBeLocalStatement(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_LOCAL_STATEMENT);
}

constexpr bool getCanBeValue(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_VALUE);
}

constexpr bool getCanBeReflection(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_REFLECTION);
}

constexpr bool getCanBeArgument(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_ARGUMENT);
}

constexpr bool getCanBeParameter(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_PARAMETER);
}

constexpr bool getCanBeBinding(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_BINDING);
}

constexpr bool getCanBeDestination(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_DESTINATION);
}

constexpr bool getCanBeAlternative(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_ALTERNATIVE);
}

constexpr bool getCanBeName(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_NAME);
}

constexpr bool getCanBePath(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_PATH);
}

constexpr bool getCanBeAttribute(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_ATTRIBUTE);
}

constexpr bool getCanBeLongRangeStage(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_LONG_RANGE_STAGE);
}

constexpr bool getCanBeShortRangeStage(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_SHORT_RANGE_STAGE);
}

constexpr bool getCanBeCase(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_CASE);
}

constexpr bool getCanBeLastCase(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_LAST_CASE);
}

constexpr bool getCanBeCapture(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_CAPTURE);
}

constexpr bool getCanBeStringLiteral(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                                        requite::_opcode::_STRING_LITERAL);
}

} // namespace requite