// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

constexpr llvm::StringRef getName(requite::Situation situation) {
  using namespace requite;
  switch (situation) {
  case Situation::NONE:
    return "NONE";
  case Situation::MATTE_ROOT_STATEMENT:
    return "MATTE_ROOT_STATEMENT";
  case Situation::MATTE_MODULE_STATEMENT:
    return "MODULE_STATEMENT";
  case Situation::MATTE_TABLE_STATEMENT:
    return "MATTE_TABLE_STATEMENT";
  case Situation::MATTE_OBJECT_STATEMENT:
    return "MATTE_OBJECT_STATEMENT";
  case Situation::MATTE_LOCAL_STATEMENT:
    return "MATTE_LOCAL_STATEMENT";
  case Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT:
    return "VALUE_REFLECTIVE_LOCAL_STATEMENT";
  case Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT:
    return "SYMBOL_REFLECTIVE_LOCAL_STATEMENT";
  case Situation::MATTE_DESTINATION:
    return "MATTE_DESTINATION";
  case Situation::VALUE_REFLECTIVE_DESTINATION:
    return "VALUE_REFLECTIVE_DESTINATION";
  case Situation::SYMBOL_REFLECTIVE_DESTINATION:
    return "SYMBOL_REFLECTIVE_DESTINATION";
  case Situation::MATTE_VALUE:
    return "MATTE_VALUE";
  case Situation::VALUE_REFLECTIVE_VALUE:
    return "VALUE_REFLECTIVE_VALUE";
  case Situation::SYMBOL_REFLECTIVE_VALUE:
    return "SYMBOL_REFLECTIVE_VALUE";
  case Situation::MATTE_JUNCTION:
    return "MATTE_JUNCTION";
  case Situation::VALUE_REFLECTIVE_JUNCTION:
    return "VALUE_REFLECTIVE_JUNCTION";
  case Situation::SYMBOL_REFLECTIVE_JUNCTION:
    return "SYMBOL_REFLECTIVE_JUNCTION";
  case Situation::MATTE_SYMBOL:
    return "MATTE_SYMBOL";
  case Situation::VALUE_REFLECTIVE_SYMBOL:
    return "VALUE_REFLECTIVE_SYMBOL";
  case Situation::SYMBOL_REFLECTIVE_SYMBOL:
    return "SYMBOL_REFLECTIVE_SYMBOL";
  case Situation::ARGUMENT:
    return "ARGUMENT";
  case Situation::PARAMETER:
    return "PARAMETER";
  case Situation::PARAMETER_VALUE:
    return "PARAMETER_VALUE";
  case Situation::STATIC_ARGUMENT:
    return "STATIC_ARGUMENT";
  case Situation::STATIC_PARAMETER:
    return "STATIC_PARAMETER";
  case Situation::STATIC_PARAMETER_VALUE:
    return "STATIC_PARAMETER_VALUE";
  case Situation::ALTERNATIVE:
    return "ALTERNATIVE";
  case Situation::STRUCTURED_BINDING:
    return "STRUCTURED_BINDING";
  case Situation::SYMBOL_NAME:
    return "SYMBOL_NAME";
  case Situation::SYMBOL_PATH:
    return "SYMBOL_PATH";
  case Situation::ATTRIBUTE:
    return "ATTRIBUTE";
  case Situation::LONG_RANGE_STAGE:
    return "LONG_RANGE_STAGE";
  case Situation::SHORT_RANGE_STEP:
    return "SHORT_RANGE_STEP";
  case Situation::SHORT_RANGE_WHILE:
    return "SHORT_RANGE_WHILE";
  case Situation::SWITCH_CASE:
    return "SWITCH_CASE";
  case Situation::LAST_SWITCH_CASE:
    return "LAST_SWITCH_CASE";
  case Situation::CAPTURE:
    return "CAPTURE";
  case Situation::STRING_LITERAL:
    return "STRING_LITERAL";
  }
  return "NONE";
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getIsValueSituation() {
  return SITUATION_PARAM == requite::Situation::MATTE_DESTINATION ||
         SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_DESTINATION ||
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_DESTINATION ||
         SITUATION_PARAM == requite::Situation::MATTE_VALUE ||
         SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_VALUE ||
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_VALUE ||
         SITUATION_PARAM == requite::Situation::MATTE_JUNCTION ||
         SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_JUNCTION ||
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_JUNCTION ||
         SITUATION_PARAM == requite::Situation::ARGUMENT;
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getIsSymbolSituation() {
  return SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::PARAMETER ||
         SITUATION_PARAM == requite::Situation::PARAMETER_VALUE ||
         SITUATION_PARAM == requite::Situation::STATIC_PARAMETER_VALUE ||
         SITUATION_PARAM == requite::Situation::ALTERNATIVE;
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getCanBeSituation(requite::Opcode opcode) {
  if constexpr (SITUATION_PARAM == requite::Situation::NONE) {
    return false;
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_ROOT_STATEMENT) {
    return requite::getCanBeMatteRootStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_MODULE_STATEMENT) {
    return requite::getCanBeMatteModuleStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_TABLE_STATEMENT) {
    return requite::getCanBeMatteTableStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_OBJECT_STATEMENT) {
    return requite::getCanBeMatteObjectStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_LOCAL_STATEMENT) {
    return requite::getCanBeMatteLocalStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT) {
    return requite::getCanBeValueReflectiveLocalStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT) {
    return requite::getCanBeSymbolReflectiveLocalStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_DESTINATION) {
    return requite::getCanBeMatteDestinationSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_DESTINATION) {
    return requite::getCanBeValueReflectiveDestinationSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_DESTINATION) {
    return requite::getCanBeSymbolReflectiveDestinationSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    return requite::getCanBeMatteValueSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_VALUE) {
    return requite::getCanBeValueReflectiveValueSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_VALUE) {
    return requite::getCanBeSymbolReflectiveValueSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION) {
    return requite::getCanBeMatteJunctionSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_JUNCTION) {
    return requite::getCanBeValueReflectiveJunctionSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_JUNCTION) {
    return requite::getCanBeSymbolReflectiveJunctionSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL) {
    return requite::getCanBeMatteSymbolSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_SYMBOL) {
    return requite::getCanBeValueReflectiveSymbolSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_SYMBOL) {
    return requite::getCanBeSymbolReflectiveSymbolSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::ARGUMENT) {
    return requite::getCanBeArgumentSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::PARAMETER) {
    return requite::getCanBeParameterSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::PARAMETER_VALUE) {
    return requite::getCanBeParameterValueSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::STATIC_ARGUMENT) {
    return requite::getCanBeStaticArgumentSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STATIC_PARAMETER) {
    return requite::getCanBeStaticParameterSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STATIC_PARAMETER_VALUE) {
    return requite::getCanBeStaticParameterValueSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::ALTERNATIVE) {
    return requite::getCanBeAlternativeSituation(opcode);

  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STRUCTURED_BINDING) {
    return requite::getCanBeStructuredBindingSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_NAME) {
    return requite::getCanBeSymbolNameSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return requite::getCanBeSymbolPathSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::ATTRIBUTE) {
    return requite::getCanBeAttributeSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::LONG_RANGE_STAGE) {
    return requite::getCanBeLongRangeStageSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SHORT_RANGE_STEP) {
    return requite::getCanBeShortRangeStepSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SHORT_RANGE_WHILE) {
    return requite::getCanBeShortRangeWhileSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SWITCH_CASE) {
    return requite::getCanBeSwitchCaseSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::LAST_SWITCH_CASE) {
    return requite::getCanBeLastSwitchCaseSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::CAPTURE) {
    return requite::getCanBeCaptureSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::STRING_LITERAL) {
    return requite::getCanBeStringLiteralSituation(opcode);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
constexpr requite::Situation getNextScopeStatementSituation() {
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_ROOT_STATEMENT) {
    return requite::Situation::MATTE_MODULE_STATEMENT;
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_MODULE_STATEMENT) {
    return requite::Situation::MATTE_TABLE_STATEMENT;
  } else if constexpr (SITUATION_PARAM ==
                           requite::Situation::MATTE_TABLE_STATEMENT ||
                       SITUATION_PARAM ==
                           requite::Situation::MATTE_OBJECT_STATEMENT ||
                       SITUATION_PARAM ==
                           requite::Situation::MATTE_LOCAL_STATEMENT) {
    return SITUATION_PARAM;
  } else {
    return requite::Situation::MATTE_LOCAL_STATEMENT;
  }
}

template <requite::Situation SITUATION_PARAM>
constexpr requite::Situation getNextValueReflectiveSituation() {
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_DESTINATION ||
                SITUATION_PARAM ==
                    requite::Situation::VALUE_REFLECTIVE_DESTINATION ||
                SITUATION_PARAM ==
                    requite::Situation::SYMBOL_REFLECTIVE_DESTINATION) {
    return requite::Situation::VALUE_REFLECTIVE_DESTINATION;
  } else if constexpr (
      SITUATION_PARAM == requite::Situation::MATTE_LOCAL_STATEMENT ||
      SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT ||
      SITUATION_PARAM ==
          requite::Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT) {
    return requite::Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_VALUE ||
                       SITUATION_PARAM == requite::Situation::ARGUMENT) {
    return requite::Situation::VALUE_REFLECTIVE_VALUE;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_JUNCTION ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_JUNCTION) {
    return requite::Situation::VALUE_REFLECTIVE_JUNCTION;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_SYMBOL ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_SYMBOL ||
                       SITUATION_PARAM == requite::Situation::PARAMETER ||
                       SITUATION_PARAM == requite::Situation::PARAMETER_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::STATIC_PARAMETER_VALUE ||
                       SITUATION_PARAM == requite::Situation::ALTERNATIVE) {
    return requite::Situation::VALUE_REFLECTIVE_SYMBOL;
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return requite::Situation::SYMBOL_PATH;
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
constexpr requite::Situation getNextSymbolReflectiveSituation() {
  if constexpr (SITUATION_PARAM == requite::Situation::MATTE_DESTINATION ||
                SITUATION_PARAM ==
                    requite::Situation::VALUE_REFLECTIVE_DESTINATION ||
                SITUATION_PARAM ==
                    requite::Situation::SYMBOL_REFLECTIVE_DESTINATION) {
    return requite::Situation::SYMBOL_REFLECTIVE_DESTINATION;
  } else if constexpr (
      SITUATION_PARAM == requite::Situation::MATTE_LOCAL_STATEMENT ||
      SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT ||
      SITUATION_PARAM ==
          requite::Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT) {
    return requite::Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_VALUE ||
                       SITUATION_PARAM == requite::Situation::ARGUMENT) {
    return requite::Situation::SYMBOL_REFLECTIVE_VALUE;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_JUNCTION ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_JUNCTION) {
    return requite::Situation::SYMBOL_REFLECTIVE_JUNCTION;
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
                       SITUATION_PARAM ==
                           requite::Situation::VALUE_REFLECTIVE_SYMBOL ||
                       SITUATION_PARAM ==
                           requite::Situation::SYMBOL_REFLECTIVE_SYMBOL ||
                       SITUATION_PARAM == requite::Situation::PARAMETER ||
                       SITUATION_PARAM == requite::Situation::PARAMETER_VALUE ||
                       SITUATION_PARAM ==
                           requite::Situation::STATIC_PARAMETER_VALUE ||
                       SITUATION_PARAM == requite::Situation::ALTERNATIVE) {
    return requite::Situation::SYMBOL_REFLECTIVE_SYMBOL;
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_NAME ||
                       SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return SITUATION_PARAM;
  } else {
    static_assert(false, "invalid situation");
  }
}

constexpr bool getCanBeNoneSituation(requite::Opcode opcode) { return false; }

constexpr bool getCanBeConvergingSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_CONVERGING);
}

constexpr bool getCanBeMatteRootStatementSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_MODULE_ROOT;
}

constexpr bool getCanBeMatteModuleStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_MATTE_MODULE_STATEMENT);
}

constexpr bool getCanBeMatteTableStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_MATTE_TABLE_STATEMENT);
}

constexpr bool getCanBeMatteObjectStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_MATTE_OBJECT_STATEMENT);
}

constexpr bool getCanBeMatteLocalStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_MATTE_LOCAL_STATEMENT);
}

constexpr bool
getCanBeValueReflectiveLocalStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(
      opcode, requite::_opcode::_VALUE_REFLECTIVE_LOCAL_STATEMENT);
}

constexpr bool
getCanBeSymbolReflectiveLocalStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(
      opcode, requite::_opcode::_SYMBOL_REFLECTIVE_LOCAL_STATEMENT);
}

constexpr bool getCanBeMatteDestinationSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_DESTINATION);
}

constexpr bool
getCanBeValueReflectiveDestinationSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_VALUE_REFLECTIVE_DESTINATION);
}

constexpr bool
getCanBeSymbolReflectiveDestinationSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(
      opcode, requite::_opcode::_SYMBOL_REFLECTIVE_DESTINATION);
}

constexpr bool getCanBeMatteValueSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_VALUE);
}

constexpr bool getCanBeValueReflectiveValueSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_VALUE_REFLECTIVE_VALUE);
}

constexpr bool getCanBeSymbolReflectiveValueSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_SYMBOL_REFLECTIVE_VALUE);
}

constexpr bool getCanBeMatteJunctionSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_JUNCTION);
}

constexpr bool
getCanBeValueReflectiveJunctionSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_VALUE_REFLECTIVE_JUNCTION);
}

constexpr bool
getCanBeSymbolReflectiveJunctionSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_SYMBOL_REFLECTIVE_JUNCTION);
}

constexpr bool getCanBeMatteSymbolSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL);
}

constexpr bool getCanBeValueReflectiveSymbolSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_VALUE_REFLECTIVE_SYMBOL);
}

constexpr bool getCanBeSymbolReflectiveSymbolSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode,
                               requite::_opcode::_SYMBOL_REFLECTIVE_SYMBOL);
}

constexpr bool getCanBeArgumentSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_VALUE) ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
         opcode == requite::Opcode::_BIND_VALUE;
}

constexpr bool getCanBeParameterSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL) ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
         opcode == requite::Opcode::_BIND_SYMBOL ||
         opcode == requite::Opcode::_NAMED_FIELDS_BEGIN ||
         opcode == requite::Opcode::_POSITIONAL_FIELDS_END;
}

constexpr bool getCanBeParameterValueSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL) ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
         opcode == requite::Opcode::_DEFAULT_VALUE;
}

constexpr bool getCanBeStaticArgumentSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_POSITIONAL_VALUE ||
         opcode == requite::Opcode::_POSITIONAL_SYMBOL ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
         opcode == requite::Opcode::_BIND_VALUE ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
         opcode == requite::Opcode::_BIND_SYMBOL;
}

constexpr bool getCanBeStaticParameterSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
         opcode == requite::Opcode::_BIND_SYMBOL;
}

constexpr bool getCanBeStaticParameterValueSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL) ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
         opcode == requite::Opcode::_DEFAULT_VALUE ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
         opcode == requite::Opcode::_DEFAULT_SYMBOL;
}

constexpr bool getCanBeAlternativeSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL) ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL;
}

constexpr bool getCanBeStructuredBindingSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_LOCAL ||
         opcode == requite::Opcode::_ASSIGN;
}

constexpr bool getCanBeSymbolNameSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__IDENTIFIER_LITERAL ||
         opcode == requite::Opcode::IDENTIFY;
}

constexpr bool getCanBeSymbolPathSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__IDENTIFIER_LITERAL ||
         opcode == requite::Opcode::IDENTIFY ||
         opcode == requite::Opcode::_REFLECT_SYMBOL;
}

constexpr bool getCanBeAttributeSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_ATTRIBUTE);
}

constexpr bool getCanBeLongRangeStageSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::FOR || opcode == requite::Opcode::DO ||
         opcode == requite::Opcode::WHILE || opcode == requite::Opcode::UNTIL ||
         opcode == requite::Opcode::STEP || opcode == requite::Opcode::WHEN;
}

constexpr bool getCanBeShortRangeStepSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_SHORT_STEP_ADD ||
         opcode == requite::Opcode::_SHORT_STEP_SUBTRACT ||
         opcode == requite::Opcode::_SHORT_STEP_MULTIPLY ||
         opcode == requite::Opcode::_SHORT_STEP_DIVIDE ||
         opcode == requite::Opcode::_SHORT_STEP_MODULUS;
}

constexpr bool getCanBeShortRangeWhileSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_SHORT_WHILE_LESS ||
         opcode == requite::Opcode::_SHORT_WHILE_GREATER ||
         opcode == requite::Opcode::_SHORT_WHILE_LESS_EQUAL ||
         opcode == requite::Opcode::_SHORT_WHILE_GREATER_EQUAL ||
         opcode == requite::Opcode::_SHORT_WHILE_EQUAL ||
         opcode == requite::Opcode::_SHORT_WHILE_NOT_EQUAL;
}

constexpr bool getCanBeSwitchCaseSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::CASE;
}

constexpr bool getCanBeLastSwitchCaseSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::CASE ||
         opcode == requite::Opcode::DEFAULT_CASE;
}

constexpr bool getCanBeCaptureSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_CAPTURE;
}

constexpr bool getCanBeStringLiteralSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__STRING_LITERAL;
}

} // namespace requite