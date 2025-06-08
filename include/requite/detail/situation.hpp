// SPDX-FileCopyrightText: 2025 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/unreachable.hpp>

namespace requite {

template <requite::Situation SITUATION_PARAM>
constexpr llvm::StringRef getName() {
  if constexpr (SITUATION_PARAM == requite::Situation::NONE) {
    return "NONE";
  } else if constexpr (SITUATION_PARAM == requite::Situation::ROOT_STATEMENT) {
    return "ROOT_STATEMENT";
  } else if constexpr (SITUATION_PARAM == requite::Situation::BASE_STATEMENT) {
    return "BASE_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::GLOBAL_STATEMENT) {
    return "GLOBAL_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::OBJECT_STATEMENT) {
    return "OBJECT_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_LOCAL_STATEMENT) {
    return "MATTE_LOCAL_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_LOCAL_STATEMENT) {
    return "VALUE_REFLECTIVE_LOCAL_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_LOCAL_STATEMENT) {
    return "SYMBOL_REFLECTIVE_LOCAL_STATEMENT";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::MATTE_DESTINATION) {
    return "MATTE_DESTINATION";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_DESTINATION) {
    return "VALUE_REFLECTIVE_DESTINATION";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_DESTINATION) {
    return "SYMBOL_REFLECTIVE_DESTINATION";
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_VALUE) {
    return "MATTE_VALUE";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_VALUE) {
    return "VALUE_REFLECTIVE_VALUE";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_VALUE) {
    return "SYMBOL_REFLECTIVE_VALUE";
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_JUNCTION) {
    return "MATTE_JUNCTION";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_JUNCTION) {
    return "VALUE_REFLECTIVE_JUNCTION";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_JUNCTION) {
    return "SYMBOL_REFLECTIVE_JUNCTION";
  } else if constexpr (SITUATION_PARAM == requite::Situation::MATTE_SYMBOL) {
    return "MATTE_SYMBOL";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::VALUE_REFLECTIVE_SYMBOL) {
    return "VALUE_REFLECTIVE_SYMBOL";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::SYMBOL_REFLECTIVE_SYMBOL) {
    return "SYMBOL_REFLECTIVE_SYMBOL";
  } else if constexpr (SITUATION_PARAM == requite::Situation::VALUE_BINDING) {
    return "VALUE_BINDING";
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_BINDING) {
    return "SYMBOL_BINDING";
  } else if constexpr (SITUATION_PARAM == requite::Situation::NAMED_FIELD) {
    return "NAMED_FIELD";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::POSITIONAL_FIELD) {
    return "POSITIONAL_FIELD";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STRUCTURED_BINDING) {
    return "STRUCTURED_BINDING";
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_NAME) {
    return "SYMBOL_NAME";
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return "SYMBOL_PATH";
  } else if constexpr (SITUATION_PARAM == requite::Situation::SWITCH_CASE) {
    return "SWITCH_CASE";
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::LAST_SWITCH_CASE) {
    return "LAST_SWITCH_CASE";
  } else if constexpr (SITUATION_PARAM == requite::Situation::CAPTURE) {
    return "CAPTURE";
  } else if constexpr (SITUATION_PARAM == requite::Situation::INTEGER_LITERAL) {
    return "INTEGER_LITERAL";
  } else {
    static_assert(false, "invalid situation");
  }
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
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_JUNCTION;
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getIsSymbolSituation() {
  return SITUATION_PARAM == requite::Situation::MATTE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::VALUE_REFLECTIVE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::SYMBOL_REFLECTIVE_SYMBOL ||
         SITUATION_PARAM == requite::Situation::POSITIONAL_FIELD;
}

template <requite::Situation SITUATION_PARAM>
constexpr bool getCanBeSituation(requite::Opcode opcode) {
  if constexpr (SITUATION_PARAM == requite::Situation::NONE) {
    return false;
  } else if constexpr (SITUATION_PARAM == requite::Situation::ROOT_STATEMENT) {
    return requite::getCanBeRootStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::BASE_STATEMENT) {
    return requite::getCanBeBaseStatementSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::GLOBAL_STATEMENT) {
    return requite::getCanBeGlobalStatementSituation(opcode);
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
                       requite::Situation::OBJECT_STATEMENT) {
    return requite::getCanBeObjectStatementSituation(opcode);
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
  } else if constexpr (SITUATION_PARAM == requite::Situation::VALUE_BINDING) {
    return requite::getCanBeValueBindingSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_BINDING) {
    return requite::getCanBeSymbolBindingSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::NAMED_FIELD) {
    return requite::getCanBeNamedFieldSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::POSITIONAL_FIELD) {
    return requite::getCanBePositionalFieldSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::STRUCTURED_BINDING) {
    return requite::getCanBeStructuredBindingSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_NAME) {
    return requite::getCanBeSymbolNameSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return requite::getCanBeSymbolPathSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::SWITCH_CASE) {
    return requite::getCanBeSwitchCaseSituation(opcode);
  } else if constexpr (SITUATION_PARAM ==
                       requite::Situation::LAST_SWITCH_CASE) {
    return requite::getCanBeLastSwitchCaseSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::CAPTURE) {
    return requite::getCanBeCaptureSituation(opcode);
  } else if constexpr (SITUATION_PARAM == requite::Situation::INTEGER_LITERAL) {
    return requite::getCanBeIntegerLiteralSituation(opcode);
  } else {
    static_assert(false, "invalid situation");
  }
}

template <requite::Situation SITUATION_PARAM>
constexpr requite::Situation getNextScopeStatementSituation() {
  if constexpr (SITUATION_PARAM == requite::Situation::ROOT_STATEMENT) {
    return requite::Situation::BASE_STATEMENT;
  } else if constexpr (SITUATION_PARAM == requite::Situation::BASE_STATEMENT) {
    return requite::Situation::GLOBAL_STATEMENT;
  } else if constexpr (SITUATION_PARAM ==
                           requite::Situation::GLOBAL_STATEMENT ||
                       SITUATION_PARAM ==
                           requite::Situation::OBJECT_STATEMENT ||
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
                           requite::Situation::SYMBOL_REFLECTIVE_VALUE) {
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
                       SITUATION_PARAM ==
                           requite::Situation::POSITIONAL_FIELD) {
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
                           requite::Situation::SYMBOL_REFLECTIVE_VALUE) {
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
                       SITUATION_PARAM ==
                           requite::Situation::POSITIONAL_FIELD) {
    return requite::Situation::SYMBOL_REFLECTIVE_SYMBOL;
  } else if constexpr (SITUATION_PARAM == requite::Situation::SYMBOL_NAME ||
                       SITUATION_PARAM == requite::Situation::SYMBOL_PATH) {
    return SITUATION_PARAM;
  } else {
    static_assert(false, "invalid situation");
  }
}

constexpr bool getCanBeNoneSituation(requite::Opcode opcode) { return false; }

constexpr bool getCanBeRootStatementSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::MODULE;
}
constexpr bool getCanBeConvergingSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_CONVERGING);
}

constexpr bool getCanBeBaseStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_BASE_STATEMENT);
}

constexpr bool getCanBeGlobalStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_GLOBAL_STATEMENT);
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

constexpr bool getCanBeObjectStatementSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_OBJECT_STATEMENT);
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

constexpr bool getCanBeValueBindingSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_BIND_VALUE ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE;
}

constexpr bool getCanBeSymbolBindingSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_BIND_SYMBOL ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL;
}

constexpr bool getCanBePositionalFieldSituation(requite::Opcode opcode) {
  return requite::_getHasFlags(opcode, requite::_opcode::_MATTE_SYMBOL) ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE ||
         opcode == requite::Opcode::_DEFAULT_VALUE;
}

constexpr bool getCanBeStructuredBindingSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_LOCAL ||
         opcode == requite::Opcode::_ASSIGN;
}

constexpr bool getCanBeNamedFieldSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::_BIND_SYMBOL ||
         opcode == requite::Opcode::_BIND_SYMBOL_OR_DEFAULT_SYMBOL ||
         opcode == requite::Opcode::_DEFAULT_VALUE ||
         opcode == requite::Opcode::_BIND_VALUE_OR_DEFAULT_VALUE;
}

constexpr bool getCanBeSymbolNameSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__IDENTIFIER_LITERAL ||
         opcode == requite::Opcode::_IDENTIFY;
}

constexpr bool getCanBeSymbolPathSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__IDENTIFIER_LITERAL ||
         opcode == requite::Opcode::_IDENTIFY ||
         opcode == requite::Opcode::_REFLECT_SYMBOL;
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

constexpr bool getCanBeIntegerLiteralSituation(requite::Opcode opcode) {
  return opcode == requite::Opcode::__INTEGER_LITERAL;
}

} // namespace requite